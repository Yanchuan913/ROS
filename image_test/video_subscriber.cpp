#include<ros/ros.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<image_transport/image_transport.h>

#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<stdio.h>
#include<math.h>
#include<vector>


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
std::cout<<"fuck2"<<std::endl;
	ROS_INFO("Received \n");
	try{
        cv::imshow( "video", cv_bridge::toCvShare(msg, "bgr8")->image );
        cv::waitKey(30);
    }
    catch( cv_bridge::Exception& e )
    {
        ROS_ERROR( "Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str() );
    }
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_listener");
	ros::NodeHandle n;
    cv::namedWindow("video");
    cv::startWindowThread();
std::cout<<"fuck0"<<std::endl;
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe( "camera/image", 1, imageCallback);
std::cout<<"fuck1"<<std::endl;
	
	ros::spin();
std::cout<<"fuck3"<<std::endl;
    cv::destroyWindow("video");
std::cout<<"fuck4"<<std::endl;
	return 0;
}

