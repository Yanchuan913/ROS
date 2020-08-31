#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/Point.h>


using namespace std;
using namespace cv;
using namespace message_filters;
using namespace sensor_msgs;
using namespace geometry_msgs;

geometry_msgs::Point position;

cv::Point2f midpoint_calculator(Mat img_src)
{
	cv::Mat img_src_gray, img_src_gray_blur;
	cv::cvtColor(img_src, img_src_gray, CV_RGB2GRAY);
	cv::GaussianBlur(img_src_gray, img_src_gray_blur, cv::Size(5, 5), 2, 2);
	double tmpCountMinVal = 0, tmpCountMaxVal = 0;
	cv::minMaxLoc(img_src_gray_blur, &tmpCountMinVal, &tmpCountMaxVal, 0, 0);
	cout << "tmpCountMaxVal" << tmpCountMaxVal << std::endl;
	cv::Point2d mid_point;
	double rows_sum = 0, cols_sum = 0, count = 0;
	for (int i = 0; i < img_src_gray_blur.rows; i++)
		for (int j = 0; j < img_src_gray_blur.cols; j++)
			if (img_src_gray_blur.at<uchar>(i, j) == tmpCountMaxVal)
			{
				rows_sum = rows_sum + i;
				cols_sum = cols_sum + j;
				count++;
				//cv:Point p(j,i);
				//cv::circle(img_src_gray_blur,p,8,Scalar(0,0,255));
			}
	mid_point.x = cols_sum / count;
	mid_point.y = rows_sum / count;
	//cv::imshow("img_src_gray_blur",img_src_gray_blur);
	return mid_point;
}

//cv::Point3d imageCallback(const ImageConstPtr& msg_left,const ImageConstPtr& msg_right,cv::Point3d &light_pose)
//void imageCallback(const sensor_msgs::ImageConstPtr& msg_left)
void imageCallback(const ImageConstPtr& msg_left,const ImageConstPtr& msg_right)
{
	ROS_INFO("Received \n");
  try
  {
	double fx = 711,fy =711,cx = 600,cy = 371.4;
	double b = -119.9;

	cv::Mat img_left = cv_bridge::toCvShare(msg_left, "bgr8")->image;
	cv::Mat img_right = cv_bridge::toCvShare(msg_right, "bgr8")->image;     
	cv::Point2d left_mid_point,right_mid_point;
	left_mid_point = midpoint_calculator(img_left);
	right_mid_point = midpoint_calculator(img_right);
	cv::circle(img_left,left_mid_point,8,Scalar(0,0,255));
	cv::imshow("img_left",img_left);
	cout<<"left_mid_point"<<left_mid_point<<endl;
	cout<<"right_mid_point"<<right_mid_point<<endl;
	
	double disparity = right_mid_point.x - left_mid_point.x;
	cv::Point3d light_pose;
	double x = (left_mid_point.x - cx) / fx;
	double y = (left_mid_point.y - cy) / fy;
	double depth = fx * b / (disparity);
	light_pose.x = x * depth;
	light_pose.y = y * depth;
	light_pose.z = depth;
	
	position.x = light_pose.x;
	position.y = light_pose.y;
	position.z = light_pose.z;
	//cout<<"light_pose"<<light_pose<<endl;
	//return light_pose;
        cv::waitKey(100);
}
	

  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_left->encoding.c_str());
  }
}
 
int main(int argc, char **argv)
{

std::cout<<"fuck0"<<std::endl;
ros::init(argc, argv, "image_listener");
ros::NodeHandle nh;

//cv::Point3d light_pose;
//geometry_msgs::Point position;
//ros::Subscriber sub = nh.subscribe("mynteye/left/image_color", 1, imageCallback);
message_filters::Subscriber<Image> image_left(nh,"mynteye/left/image_color",1);
message_filters::Subscriber<Image> image_right(nh,"mynteye/right/image_color",1);
TimeSynchronizer<Image,Image> left_and_right(image_left,image_right,10);
left_and_right.registerCallback(boost::bind(&imageCallback,_1,_2));
//left_and_right.registerCallback(boost::bind(&imageCallback,_1,_2));

ros::Publisher pose_pub = nh.advertise<geometry_msgs::Point>("light_pose",1000);
ros::Rate loop_rate(10);
//cv::namedWindow("view");
//cv::startWindowThread();
//image_transport::ImageTransport it(nh);
//std::cout<<"fuck1"<<std::endl;
//image_transport::Subscriber sub = it.subscribe("mynteye/left/image_color", 1, imageCallback);
std::cout<<"fuck2"<<std::endl;



while(ros::ok())
{

pose_pub.publish(position);
//cout<<position<<endl;
ros::spinOnce();
loop_rate.sleep();
}
//ros::spin();
//std::cout<<"fuck3"<<std::endl;
//cv::destroyWindow("view");
}
