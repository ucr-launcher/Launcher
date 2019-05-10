#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int16.h>

//--------- GLOBAL VARIABLES LOCAL TO STM32 MAIN -----------
DigitalOut my_led(LED1);

int xaxis_location       = 0;
int num_steps            = 0;
double kinect_fov_middle = 28.5;
double degrees_per_step  = 1.8;

int direction            = 0;


DigitalOut D_8(D8);
DigitalOut D_9(D9);
DigitalOut D_10(D12);
DigitalOut D_11(D11);

//--------  END GLOBAL VARIABLES DECLARATION ----------------


//--------------  ORIENTATION MAPPER ------------------------
// Maps value from incoming pixel value to correct range
float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Maps pixels to number of steps needed to turn n degrees. 
// Returns a 1 or 0 to determine whether to make left or right turns.
int map_orientation(int x){
    // Pixels to degrees
    double degrees = 0;
    degrees = map(x, 0, 640, 0,57);
    
    // degrees to steps
    num_steps = (kinect_fov_middle - degrees)/ degrees_per_step;
    
    // Left or right turn
    if (num_steps < 0){
        return 1;
    }else{
        return 0;
    }
}

//-----------------  END MAPPER -----------------------------


//--------  STEPPER MOTOR HELPER FUNCTIONS   ----------------
void turn_left_n_steps(int steps){
    for (int i = 0; i < steps; i++) {
            
        D_8.write(1);
        D_9.write(1);
        D_10.write(0);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(1);
        D_10.write(1);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(0);
        D_10.write(1);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(1);
        D_9.write(0);
        D_10.write(0);
        D_11.write(1);
        
        wait_ms(10);
    }
    
}

void turn_right_n_steps(int steps) {
       for (int i = 0; i < steps; i++) {
            
        D_8.write(1);
        D_9.write(0);
        D_10.write(0);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(0);
        D_10.write(1);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(1);
        D_10.write(1);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(1);
        D_9.write(1);
        D_10.write(0);
        D_11.write(0);
        
        wait_ms(10);
    }
}


//--------------  END HELPER FUNCTION ----------------------- 


//--------- ROS NODE CONFIGURATION -------------------------
ros::NodeHandle nh;

// Ros message data containers
std_msgs::Int16 transmit_msg;

// Set up Publishers
//ros::Publisher acknowledge_publisher("acknowledge", &transmit_msg);
            
// Set up Subscribers
void xaxis_callback( const std_msgs::Int16& msg)
{
    // Assign msg.data to local variables
    xaxis_location = msg.data;
}

ros::Subscriber<std_msgs::Int16> xaxis_subscriber("xaxis_location", xaxis_callback);


//---------- END CONFIGURATION  ----------------------------

int main() {
    
    // Initialize Node and publisher/subscriber
    nh.initNode();
    //nh.advertise(acknowledge_publisher);
    nh.subscribe(xaxis_subscriber);
    
    while(1) {
        
        // Use local variables for computations here
        if (xaxis_location == 0)
        {
            // do nothing
        }
        else if (xaxis_location > 320)
        {
            turn_right_n_steps(5);     
        }
        else if (xaxis_location < 320)
        {
            turn_left_n_steps(5);
        }
        
        //---------------------- FIXME -- TEST --------------------------
        // Value returned is a 1 or 0 to determin Left or Right turns
        direction = map_orientation(xaxis_location);
        
        if (direction){
            turn_right_n_steps(num_steps);
        }else if(!direction){
            turn_left_n_steps(num_steps);
        }
        //----------------------------------------------------------------
        
        // Publish any data necessary to be fed back to pc
        //transmit_msg.data = xaxis_location;
        //acknowledge_publisher.publish(&transmit_msg);
        
        // Spin ROS Node -- get data from callbacks
        nh.spinOnce();
        wait_ms(5);
    }
}
