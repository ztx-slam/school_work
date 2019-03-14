#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/Imu.h>
#include <sstream>
#include "imu_interface_broadcaster/imu_interface_msg.h"
#include "sensor_msgs/MagneticField.h"
#include "packet.h"
#include "imu_data_decode.h"
#include "serial.h"
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "imu_interface_broadcaster");
  ros::NodeHandle n;
  //ros::Publisher broad_caster_pub = n.advertise<imu_interface_broadcaster::imu_interface_msg>("imu_interface_broadcast", 1000);
  //ros::Publisher IMU_pub = n.advertise<sensor_msgs::Imu>("/gpsimu_driver/imu_data", 20);
  ros::Publisher IMU_pub = n.advertise<sensor_msgs::Imu>("/gpsimu_driver/imu_data", 200);
  ros::Publisher MAG_pub = n.advertise<sensor_msgs::MagneticField>("imu/mag", 4);//20
  ros::Rate loop_rate(10);//10
  int count = 0;
  /**
   * Parameters to get IMU datas
   */
  int16_t Acc[3] = {0};
  int16_t Gyo[3] = {0};
  int16_t Mag[3] = {0};
  float Eular[3] = {0};
  float Quat[4] = {0};
  open_serial_port();
  while (ros::ok())
  {
    read_once(Acc, Gyo, Mag, Eular, Quat);
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    sensor_msgs::Imu imu_data;
    sensor_msgs::MagneticField mag_data;
    std::stringstream ss;
    ss << "\r\b";
    ss << "Acc:" << Acc[0] << "," << Acc[1] << "," << Acc[2] << "\r\n";
    ss << "Gyo:" << Gyo[0] << "," << Gyo[1] << "," << Gyo[2] << "\r\n";
    ss << "Mag:" << Mag[0] << "," << Mag[1] << "," << Mag[2] << "\r\n";
    ss << "Eular:" << Eular[0] << "," << Eular[1] << "," << Eular[2] << "\r\n";
    ss << "Quat:" << Quat[0] << "," << Quat[1] << "," << Quat[2] << "," << Quat[3] <<"\r\n";
   // ROS_INFO("四元数获取值 w: %s", Quat[0]);
   // ROS_INFO("四元数获取值 x: %s", Quat[1]);
  //  ROS_INFO("四元数获取值 y: %s", Quat[2]);
  //  ROS_INFO("四元数获取值 z: %s", Quat[3]);
    std::cout<<"acc "<<Acc[0]<<" "<<Acc[1]<<" "<<Acc[2]<<" "<<std::endl;

    mag_data.header.stamp = ros::Time::now();
    mag_data.header.frame_id = "/velodyne:";
    mag_data.magnetic_field.x = Mag[0];
    mag_data.magnetic_field.y = Mag[1];
    mag_data.magnetic_field.z = Mag[2];

    imu_data.header.stamp = ros::Time::now();
    imu_data.header.frame_id = "/velodyne:";
    imu_data.orientation.x = Quat[1];
    imu_data.orientation.y = Quat[2];
    imu_data.orientation.z =- Quat[3];
    imu_data.orientation.w = -Quat[0];

    imu_data.orientation_covariance[0] = 1000000;
    imu_data.orientation_covariance[1] = 0;
    imu_data.orientation_covariance[2] = 0;
    imu_data.orientation_covariance[3] = 0;
    imu_data.orientation_covariance[4] = 1000000;
    imu_data.orientation_covariance[5] = 0;
    imu_data.orientation_covariance[6] = 0;
    imu_data.orientation_covariance[7] = 0;
    imu_data.orientation_covariance[8] = 0.000001;


    imu_data.angular_velocity.x = 0.0;
    imu_data.angular_velocity.y = 0.0;
    imu_data.angular_velocity.z = (double)-1*(Gyo[2]*3.14/(180*100));

    imu_data.linear_acceleration.x = (double)((Acc[0]/100.f));
    imu_data.linear_acceleration.y = (double)((Acc[1]/100.f));
    imu_data.linear_acceleration.z = (double)((Acc[2]/100.f));

   // imu_data.linear_acceleration.x = (double)(1*((Acc[0]+24) * 9.80665/3000.f));
   // imu_data.linear_acceleration.y = (double)(-1*((Acc[1]-1070) * 9.80665/3000.f));
   // imu_data.linear_acceleration.z = (double)(-1*((Acc[2]+10) * 9.80665/3000.f));

   // imu_data.linear_acceleration.x = (double)(Acc[0]* 9.81/4096.f);
   // imu_data.linear_acceleration.y = (double)(Acc[1] * 9.81/4096.f);
   // imu_data.linear_acceleration.z = (double)(Acc[2] * 9.81/4096.f);

    /**
     * The publish() function is how you send messages.
     */
    IMU_pub.publish(imu_data);
    MAG_pub.publish(mag_data);
    ros::spinOnce();
    loop_rate.sleep();
  }
  close_serial_port();
  return 0;
}
