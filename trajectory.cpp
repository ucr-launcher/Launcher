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
const float x_scalar = 1.0; // platform rotation
const float y_scalar = 1.0; // platform vertical orientation
const float z_scalar = 1.0; // PWM speed

//@Params: 1 = the location of the target on the x-axis (in pixels) | 2 = the location of the target on the y-axis (in pixels)|  3 = the location of the target on the z-axis (in mm)
void find_trajectory(vector<int> location_data) {  

	if (location_data.size() < 3) { //check if the input vector has the correct size
		cout << "Error: Location_data vector has wrong size! Expected: 3. Got: " << location_data.size() << endl;
		return;
	}
	
	location_data.at(0) = location_data.at(0) * x_scalar;
	location_data.at(1) = location_data.at(1) * y_scalar;
	location_data.at(2) = location_data.at(2) * z_scalar;
	
}

int main() {
	
	//*** Setup ***//
	cout << "Setting up.." << endl;
	time_t t = std::time(0); 
	srand(t);
	int x_loc = rand() % image_frame_width + 1;
	int y_loc = rand() % image_frame_height + 1;
	int z_loc = rand() % maximum_depth + 1;
	//*** End of setup ***//
	
	cout << "Running algorithm" << endl;
	vector<int> location_data;
	location_data.push_back(x_loc);
	location_data.push_back(y_loc);
	location_data.push_back(z_loc);
	cout<< "Size: " << location_data.size() << endl;
	
	find_trajectory(location_data);
	
	cout << "Printing data.." << endl;
	cout << "launch_data length: " + location_data.size() << endl;
	for (int i : location_data) {
		cout << i << endl;
	}
	
	return 0;
}