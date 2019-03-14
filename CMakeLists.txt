cmake_minimum_required(VERSION 2.8.3)
project(imu_interface_broadcaster)
find_package(
    catkin REQUIRED COMPONENTS 
    roscpp 
    rospy 
    serial
    message_generation
)
add_message_files(
    DIRECTORY msg FILES imu_interface_msg.msg
)
generate_messages(
    DEPENDENCIES std_msgs
)
catkin_package(
CATKIN_DEPENDS message_runtime serial
)
include_directories(include ${catkin_INCLUDE_DIRS})
#add_executable(imu_interface_broadcaster_node src/imu_interface_broadcaster.cpp src/serial.c src/packet.c src/imu_data_decode.c)
#add_dependencies(imu_interface_broadcaster_node imu_interface_broadcaster_messages_cpp)
#target_link_libraries(imu_interface_broadcaster_node ${catkin_LIBRARIES})

#
add_executable(imu_data_node src/imu_data.cpp src/serial.c src/packet.c src/imu_data_decode.c )
add_dependencies(imu_data_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(imu_data_node ${catkin_LIBRARIES})

