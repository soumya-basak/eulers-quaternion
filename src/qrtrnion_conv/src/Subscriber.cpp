#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
//#include <Quaternion.h>
#include <eigen3/Eigen/Geometry>

#include "geometry_msgs/Pose.h"
#include "ros/ros.h"
#include "tf2/LinearMath/Quaternion.h"
#include "/opt/ros/kinetic/include/tf/LinearMath/Quaternion.h"
#include "geometry_msgs/Quaternion.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float64.h"
#include <tf/transform_datatypes.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

using namespace std_msgs;
class client 
{
    private:
        int Arr[3];
        ros::NodeHandle n;
        ros::Subscriber sub3;
        ros::Publisher pub3;
        std_msgs::Float64 output_msg;
        std_msgs::Float64MultiArray arr;
        double x, y, z;
        double Qx, Qy, Qz, Qw;  /*quarternion coordinates*/
        double cr,sr,cp,sp,cy,sy; /*cr = cos(Roll), sr = sin(Roll) */
        //tf2::Quaternion q_orig, q_rot, q_new;

    public:
        client()
        {
            sub3 = n.subscribe("array", 100, &client::arrayCallback, this);
            
            pub3 = n.advertise<std_msgs::Float64MultiArray>("output",10);
        }
        void arrayCallback(const std_msgs::Float64MultiArray::ConstPtr& array)
        {
            arr.data.clear();
            int i = 0;
            
             std::vector<double>::const_iterator it = array->data.begin();
             x = *it; 
             it++;
             y = *it;
             it++;
             z = *it;

        // Get the original orientation of 'commanded_pose'
        //tf2::convert(array.pose.orientation , q_orig);
        
        //q_rot.setRPY(x,y,z);
        //q_new = q_rot*q_orig;  // Calculate the new orientation
        //q_new.normalize();

        // Stuff the new rotation back into the pose. This requires conversion into a msg type
        //tf2::convert(q_new, array.pose.orientation);

        /*do trigonometry manually conversion*/
        cy = cos(z * 0.5);
        sy = sin(z * 0.5);
        cp = cos(y * 0.5);
        sp = sin(y * 0.5);
        cr = cos(x * 0.5);
        sr = sin(x * 0.5);

        /*formula to convert euler to quarternion*/
        Qw = cy * cp * cr + sy * sp * sr;
        Qx  = cy * cp * sr - sy * sp * cr;
        Qy = sy * cp * sr + cy * sp * cr;
        Qz = sy * cp * cr - cy * sp * sr;

        
        //Automatic conversion of quarternion using API
        //tf2::Quaternion myQuaternion;
        //myQuaternion.setRPY(x,y,z);  // Create this quaternion from roll/pitch/yaw (in radians)
        arr.data.push_back(Qw);
        arr.data.push_back(Qx);
        arr.data.push_back(Qy);
        arr.data.push_back(Qz);

        ROS_INFO("I published the quaternion!");
        //Publish array
	    pub3.publish(arr);
        ros::spinOnce();
        //Let the world know
        ROS_INFO("I published the quaternion!");
        //Do this.
	    //ros::spin();
	   //Added a delay so not to spam
	   //sleep(2);
       //pub3.publish(output_msg);
        //ros::spin();
         //ROS_INFO_STREAM(q); 
        }

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "test");
    client c;
    ros::spin();
    return 0;
}
