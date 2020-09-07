cmake_minimum_required(VERSION 2.8.3)
project(odom_lookup)
 
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  tf2
  tf2_ros  
  tf
  image_transport
  roscpp
  rospy
  std_msgs
  sensor_msgs
  geometry_msgs
)
#增加的这句
find_package(OpenCV REQUIRED)
 
catkin_package(
CATKIN_DEPENDS cv_bridge image_transport
#  INCLUDE_DIRS include
#  LIBRARIES ros_cv_image
#  CATKIN_DEPENDS cv_bridge image_transport roscpp rospy std_msgs
#  DEPENDS system_lib
)
#添加 ${OpenCV_INCLUDE_DIRS}
include_directories(
 ./include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
 
## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
 
## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
 
add_executable(odom odom.cpp)
target_link_libraries(odom ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
 

