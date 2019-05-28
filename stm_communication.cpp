#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int16.h>

//--------- GLOBAL VARIABLES LOCAL TO STM32 MAIN -----------
DigitalOut my_led(LED1);

int xaxis_location            = 0;
int num_steps                 = 0;
int prev_steps                = 0;
double kinect_fov_middle      = 28.5;
double degrees_per_step       = .035;
int direction_flag            = 0;


/*
DigitalOut D_8(D8);
DigitalOut D_9(D9);
DigitalOut D_10(D12);
DigitalOut D_11(D11);
*/

// step 
DigitalOut motor(D8);
// direction
DigitalOut direction(D9);


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
    degrees = map(x, 0, 800, 0,57);
    
    // degrees to steps
    num_steps = (kinect_fov_middle - degrees)/ degrees_per_step;
    
    // Left or right turn
    if (num_steps < 0){
        num_steps = num_steps*-1;
        return 1;
    }else{
        return 0;
    }
}

//-----------------  END MAPPER -----------------------------


//--------  STEPPER MOTOR HELPER FUNCTIONS   ----------------

void turn_stepper_right(int steps){
    direction.write(0);
    for(int i = 0; i < steps; i++){
        motor.write(0);
        wait_ms(1);
        motor.write(1);
    }
}

void turn_stepper_left(int steps){
    direction.write(1);
    for(int i = 0; i < steps; i++){
        motor.write(0);
        wait_ms(1);
        motor.write(1);
    }
}

//--------------  END HELPER FUNCTION ----------------------- 



//---------- ROS NODE CONFIGURATION -------------------------
ros::NodeHandle nh;

// Ros message data containers
std_msgs::Int16 transmit_msg;

// Set up Publishers
ros::Publisher acknowledge_publisher("state", &transmit_msg);

std_msgs::Int16 loop_msg;
ros::Publisher loop_publisher("loop", &loop_msg);
            
// Set up Subscribers
void xaxis_callback( const std_msgs::Int16& msg)
{
    // Assign msg.data to local variables
    xaxis_location = msg.data;
}

ros::Subscriber<std_msgs::Int16> xaxis_subscriber("xaxis_location", xaxis_callback);


//---------- END CONFIGURATION  ----------------------------

//--------------  LAUNCHING MECHANISM SM --------------------

enum States { detect, orient_x, orient_y, orient_z, launch, zero } state;


void launcher_sm(){

    
    Timer t;
    
    // Transitions
    switch(state) {
        case detect:
            // Keep searching until object is detected
            if (xaxis_location != 0)
            {
                state = orient_x;
            }else{
                state = detect;   
            }
            break;
            
        case orient_x:
            // Proceed to orient x-axis
            state = orient_y;
            break;   
             
        case orient_y:
            // Proceed to orient z-axis
            state = orient_z;
            break;
            
        case orient_z:
            // Proceed to launch projectile
            state = launch;
            break;           
            
        case launch:
            state = zero; 
            
            break;
        case zero:
            state = detect;
            break;
    }
    // Actions
    switch(state){
        case detect:
            break;
            
        case orient_x:
        
            // Value returned is a 1 or 0 to determin Left or Right turns-------
            direction_flag = map_orientation(xaxis_location);
        
             if (direction_flag){
                    turn_stepper_right(num_steps);
                } else if(!direction_flag){
                    turn_stepper_left(num_steps);
            }
            
            // Check if the current steps to take are not the same as the previous
            // If the prev steps and current steps are the same, the object is at
            // the same position
            /* 
            if ( abs(num_steps - prev_steps) > 5) {
               
            }
            */
        
            // Assign current steps to prev steps
            //prev_steps = num_steps;
            
            break;
            
        case orient_y:
            
            break;
            
        case orient_z:
            
            break;
            
        case launch:
            t.start();
            
            while(t.read() < 7){
                // do nothing    
                
                //loop_msg.data = 1 ;
                //loop_publisher.publish(&loop_msg);

            }
            
            //loop_msg.data = 0;
           // loop_publisher.publish(&loop_msg);
            
            t.stop();
            t.reset();
            break;
            
        case zero:
            if (!direction_flag){
                    turn_stepper_right(num_steps);
            } else if(direction_flag){
                    turn_stepper_left(num_steps);
            }
            break;
    }
    
}

//-------------- END STATE MACHINE --------------------------

int main() {
    
    // Initialize Node and publisher/subscriber
    nh.initNode();
    nh.advertise(acknowledge_publisher);
    nh.subscribe(xaxis_subscriber);
    
    nh.advertise(loop_publisher);
    
    state = detect;
    
    while(1) {
        
  
        //----------------------------------------------------------------
        
        // Call state machine
        launcher_sm();
        
        
        // Publish any data necessary to be fed back to pc
        transmit_msg.data = state;
        acknowledge_publisher.publish(&transmit_msg);
        
        
        
        // Spin ROS Node -- get data from callbacks
        nh.spinOnce();
        wait_ms(5);
    }
}
