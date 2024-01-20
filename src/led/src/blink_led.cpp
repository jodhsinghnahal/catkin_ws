#include <ros/ros.h>
#include <pigpio.h>

#define LED_PIN 18 // change pin number here

int main (int argc, char **argv)
{
    ros::init(argc, argv, "test_pigpio_ros");
    ros::NodeHandle nh;

    if (gpioInitialise() < 0) {
        ROS_ERROR("pigpio initialization failed.");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);
    ROS_INFO("GPIO has been set as OUTPUT.");

    while (ros::ok())
    {
        gpioWrite(LED_PIN, 1); // Set GPIO pin to HIGH
        ROS_INFO("Set GPIO HIGH");
        ros::Duration(1.0).sleep();
        gpioWrite(LED_PIN, 0); // Set GPIO pin to LOW
        ROS_INFO("Set GPIO LOW");
        ros::Duration(1.0).sleep();
    }

    gpioTerminate();

    return 0;
}

