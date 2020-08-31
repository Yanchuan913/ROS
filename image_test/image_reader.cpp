#include <ros/ros.h>
#include <iostream>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
 
int main(int argc, char** argv)
{
std::cout<<0<<std::endl;
ros::init(argc, argv, "image_reader");
ros::NodeHandle nh;
cv::Mat image = cv::imread("/home/nuc3/2.png", CV_LOAD_IMAGE_COLOR);
cv::namedWindow("view",CV_WINDOW_AUTOSIZE);
cv::imshow("view", image);
cv::waitKey(0);
}
