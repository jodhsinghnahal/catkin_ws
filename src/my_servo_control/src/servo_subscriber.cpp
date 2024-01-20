// servo_controller.cpp


#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <pigpiod_if2.h>
#include <ros/ros.h>
#include <pigpio.h>


// Define GPIO pins for servo
const int servoPin = 18;  // GPIO pin connected to the servo


// Global variable to store the received encoder position
int receivedEncoderPosition = 0;


// Callback function for receiving encoder values
void encoderCallback(const std_msgs::Int32::ConstPtr& msg) {
    receivedEncoderPosition = msg->data;
}


int main(int argc, char **argv) {
    // Initialize ROS node
    ros::init(argc, argv, "servo_controller");
    ros::NodeHandle nh;


    // Set up the servo pin and initialize Pigpio
    if (gpioInitialise() < 0) {
        ROS_ERROR("Failed to initialize Pigpio library");
        return 1;
    }


    gpioSetMode(servoPin, PI_OUTPUT);


    // Create a ROS subscriber for encoder values
    ros::Subscriber encoder_sub = nh.subscribe("encoder_values", 10, encoderCallback);


    // Set the loop rate (adjust as needed)
    ros::Rate loop_rate(10);  // 10 Hz


    while (ros::ok()) {
        // Limit encoder position to a specific range (adjust as needed)
        int limitedEncoderPosition = std::min(std::max(receivedEncoderPosition, 0), 100);


        // Map encoder position to servo pulse width (adjust as needed)
        int pulseWidth = 500 + limitedEncoderPosition * 19;  // Adjust multiplier based on your servo


        // Control the servo with the calculated pulse width
        gpioServo(servoPin, pulseWidth);


        ros::spinOnce();
        loop_rate.sleep();
    }


    // Release resources and clean up Pigpio
    gpioTerminate();


    return 0;
}

