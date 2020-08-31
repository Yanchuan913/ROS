#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
 
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	ROS_INFO("Received \n");
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
        cv::waitKey(1);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}
 
int main(int argc, char **argv)
{

std::cout<<"fuck0"<<std::endl;
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  //cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
std::cout<<"fuck1"<<std::endl;
  image_transport::Subscriber sub = it.subscribe("mynteye/left/image_color", 1, imageCallback);
std::cout<<"fuck2"<<std::endl;
  ros::spin();
std::cout<<"fuck3"<<std::endl;
  cv::destroyWindow("view");
}
