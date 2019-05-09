#include <iostream> //For console output
#include <string.h>
#include <ctime>  
#include <cstdlib>
#include <vector>   //for Vectors

using namespace std;

const int image_frame_width  = 600; //width of the image from which the trajectory was pulled (in pixels)
const int image_frame_height = 400; //height of the image from which the trajectory data was pulled (in pixels)
const int maximum_depth = 5791;     //maximum depth that the sensor could read (in mm)

//Scalars for tuning the launching algorithm.
const float x_scalar = 1.3; // platform rotation
const float y_scalar = 1.2; // platform vertical orientation
const float z_scalar = 0.5; // PWM speed

//Flat values which can be appended to the values to finely-tune the trajectories.
const float x_const = 0.0;
const float y_const = 0.0;
const float z_const = 0.0;

void find_trajectory(vector<float> &data);