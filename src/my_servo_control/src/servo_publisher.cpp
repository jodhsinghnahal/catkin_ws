// encoder_publisher.cpp


#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <ros/ros.h>
#include <pigpio.h>


// Define GPIO pins for rotary encoder
const int encoderPinA = 23;  // GPIO pin A connected to the rotary encoder
const int encoderPinB = 24;  // GPIO pin B connected to the rotary encoder


// Global variable to store encoder position
volatile int encoderPosition = 0;


// Callback function for rotary encoder
void encoderCallback(int gpio, int level, uint32_t tick) {
    if (gpio == encoderPinA) {
        if (gpioRead(encoderPinB) == 0) {
            encoderPosition++;
        } else {
            encoderPosition--;
        }
    } else if (gpio == encoderPinB) {
        if (gpioRead(encoderPinA) == 0) {
            encoderPosition--;
        } else {
            encoderPosition++;
        }
    }
}


int main(int argc, char **argv) {
    // Initialize ROS node
    ros::init(argc, argv, "encoder_publisher");
    ros::NodeHandle nh;


    // Set up the rotary encoder
    gpioSetMode(encoderPinA, PI_INPUT);
    gpioSetMode(encoderPinB, PI_INPUT);
    gpioSetPullUpDown(encoderPinA, PI_PUD_UP);
    gpioSetPullUpDown(encoderPinB, PI_PUD_UP);
    gpioSetAlertFuncEx(encoderPinA, encoderCallback, nullptr);


    // Create a ROS publisher for encoder values
    ros::Publisher encoder_pub = nh.advertise<std_msgs::Int32>("encoder_values", 10);


    // Set the loop rate (adjust as needed)
    ros::Rate loop_rate(10);  // 10 Hz


    while (ros::ok()) {
        // Create a message and publish the encoder position
        std_msgs::Int32 msg;
        msg.data = encoderPosition;
        encoder_pub.publish(msg);


        ros::spinOnce();
        loop_rate.sleep();
    }


    // Release resources and clean up (no need to terminate Pigpio here)


    return 0;
}

