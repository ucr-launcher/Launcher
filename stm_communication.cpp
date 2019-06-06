#include "mbed.h"
#include "Servo.h"
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>

//--------- GLOBAL VARIABLES LOCAL TO STM32 MAIN -----------
DigitalOut my_led(LED1);

int xaxis_location            = 0;
int num_steps                 = 0;
int prev_steps                = 0;
double kinect_fov_middle      = 28.5;
double degrees_per_step       = .028;
int direction_flag            = 0;
float depth                   = 0;

// step 
DigitalOut motor(D8);
// direction
DigitalOut direction(D9);


//--------  END GLOBAL VARIABLES DECLARATION ----------------

//----------------  DC MOTOR CONFIG ------------------------
SPI spi(D11, D12, D13);
DigitalOut LEFT_MOTOR(D14);
DigitalOut RIGHT_MOTOR(D15);
Servo myservo(D6);

#define input_start 2.3 // a
#define input_end 19.7  // b

#define output_start 0  // c
#define output_end 64   // d

// speed is any integer between 0 and 64.
// this corresponds to a resistor value between 0 and 100K Ohms
void set_speed(int speed){
    
    spi.write(0x00);
    spi.write(speed);
}

//
void build_speed(int speed){
    
    for (int i = 1; i <= speed; i++){
        set_speed(i);
        wait_ms(10);
    }
}

int map_distance(float distance){
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (distance - input_start);
}


void drop_ball(){
    
    myservo.write(0);
    wait(1);
    myservo.write(140);
    wait(1);
}


//----------------  END MOTOR CONFIG -----------------------


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
            
// Set up Subscribers
void xaxis_callback( const std_msgs::Int16& msg)
{
    // Assign msg.data to local variables
    xaxis_location = msg.data;
}

void depth_callback( const std_msgs::Float64& msg)
{
    depth = msg.data;   
}

ros::Subscriber<std_msgs::Int16>   xaxis_subscriber("xaxis_location", xaxis_callback);
ros::Subscriber<std_msgs::Float64> depth_subscriber("depth", depth_callback);

//---------- END CONFIGURATION  ----------------------------

//--------------  LAUNCHING MECHANISM SM --------------------

enum States { detect, orient_x, orient_y, orient_z, launch, zero } state;

int t = 0;
void launcher_sm(){

    
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
            if (t > 500){
                state = zero; 
                t = 0;
            }else{
                state = launch;   
            }
            break;
        case zero:
            state = detect;
            break;
    }
    // Actions
    switch(state){
        case detect:
            //set_speed(0); 
            break;
            
        case orient_x:
        
            // Value returned is a 1 or 0 to determin Left or Right turns-------
            direction_flag = map_orientation(xaxis_location);
        
             if (direction_flag){
                    turn_stepper_right(num_steps);
                } else if(!direction_flag){
                    turn_stepper_left(num_steps);
            }
            
            break;
            
        case orient_y:
            //set_speed(0); 
            break;
            
        case orient_z:
            // first select motor
            LEFT_MOTOR.write(0);
            RIGHT_MOTOR.write(0);
            
            // then proceed to rev the motor
            set_speed(1);
            wait_ms(10);
            
            // now build the speed
            build_speed(map_distance(2));
            
            
            break;
            
        case launch:
        
            // actuate the servo to drop the ball
            drop_ball();
            
            // turn off the motors
            set_speed(0);
            
            // deselect the motors
            LEFT_MOTOR.write(1);
            LEFT_MOTOR.write(1);
            
            t++;
            
            break;
            
        case zero:
            //set_speed(0); 
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
    nh.subscribe(depth_subscriber);
    
    state = detect;
    
    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 10MHz clock rate
    spi.format(8,0);
    spi.frequency(1000000); // this should be 10 MHz

    // select the motors
    RIGHT_MOTOR.write(0);
    LEFT_MOTOR.write(0);
    
    // speed zero corresponds to the motors being off
    set_speed(0);
    wait_ms(10);
    
    while(1) {
        //----------------------------------------------------------------
        set_speed(0);    
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
