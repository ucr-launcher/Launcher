#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int16.h>

//--------- GLOBAL VARIABLES LOCAL TO STM32 MAIN -----------
DigitalOut my_led(LED1);

int data_recieved     = 0;

//--------  END GLOBAL VARIABLES DECLARATION ---------------


//--------- ROS NODE CONFIGURATION -------------------------
ros::NodeHandle nh;

// Ros message data containers
std_msgs::Int16 recieve_msg;
std_msgs::Int16 transmit_msg;

// Set up Publishers
ros::Publisher acknowledge_publisher("acknowledge", &transmit_msg);
            
// Set up Subscribers
void reciever_callback( const std_msgs::Int16& msg)
{
    // Assign msg.data to local variables
    data_recieved = msg.data;
}

ros::Subscriber<std_msgs::Int16> reciever_subscriber("test", reciever_callback);


//---------- END CONFIGURATION  ----------------------------

int main() {
    
    // Initialize Node and publisher/subscriber
    nh.initNode();
    nh.advertise(acknowledge_publisher);
    nh.subscribe(reciever_subscriber);
    
    while(1) {
        
        // Use local variables for computations here
        
        
        // Publish any data necessary to be fed back to pc
        transmit_msg.data = data_recieved;
        acknowledge_publisher.publish(&transmit_msg);
        
        // Spin ROS Node -- get data from callbacks
        nh.spinOnce();
        wait_ms(5);
    }
}