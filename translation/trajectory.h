#include <iostream> //For console output
#include <string.h>
#include <ctime>  
#include <cstdlib>
#include <vector>   //for Vectors

using namespace std;

const int x_max  = 640; //width of the image from which the trajectory was pulled (in pixels)
const int y_max  = 420; //height of the image from which the trajectory data was pulled (in pixels)
const int z_max  = 100; //Maximum PWM speed that the DC motors can spin at. Since the period is unknown at this time, the maximum values are also unknown.

//Scalars for tuning the launching algorithm.
const float x_scalar = 0.1; // platform rotation
const float y_scalar = 0.1; // platform vertical orientation
const float z_scalar = 0.1; // PWM speed

//Flat values which can be appended to the values to finely-tune the trajectories.
const float x_const = 0.0;
const float y_const = 0.0;
const float z_const = 0.0;

int find_trajectory(vector<float> &data);