#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
//#include <tf2/LinearMath/Quaternion.h>
//#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");
  ros::NodeHandle n;

  tf::TransformListener listener;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  ros::Rate rate(10.0);
  nav_msgs::Odometry odom;

  while (n.ok()){
    tf::StampedTransform transform;
    geometry_msgs::Twist  twist;
    ros::spinOnce();               // check for incoming messages    

    try{
      listener.lookupTransform("/base_link", "/odom", ros::Time(0), transform);
      listener.lookupTwist("/base_link", "/odom", ros::Time(0), ros::Duration(0.1),twist);
      ROS_INFO("don't worry! odom is working");
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    odom.header.stamp = ros::Time::now();
    odom.header.frame_id = "odom";
    //set the position
    odom.pose.pose.position.x = transform.getOrigin().x();
    odom.pose.pose.position.y = transform.getOrigin().y();
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation.x = transform.getRotation().getAxis().getX();
    odom.pose.pose.orientation.y = transform.getRotation().getAxis().getY();
    odom.pose.pose.orientation.z = transform.getRotation().getAxis().getZ();
    odom.pose.pose.orientation.w = transform.getRotation().getW();
    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = twist.linear.x;
    odom.twist.twist.linear.y = twist.linear.y;
    odom.twist.twist.angular.z = twist.angular.z;

    //publish the message
    odom_pub.publish(odom);
    rate.sleep();
  }
  return 0;
};

