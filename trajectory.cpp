#ifndef TRAJECTORY_H
#define TRAJECTORY_H
	#include "trajectory.h"
#endif

//@Params: a vector who's 0th index is the x location data, 1st index is the y location data, and the 2nd index is the z location data.
void find_trajectory(vector<float> &data) {  

	if (data.size() < 3) { //check if the input vector has the correct size
		cout << "Error: Location_data vector has wrong size! Expected: 3. Got: " << data.size() << endl;
		return;
	}
	
	//Transform the vector's contents using the scalars
	data.at(0) = data.at(0) * x_scalar + x_const;
	data.at(1) = data.at(1) * y_scalar + y_const;
	data.at(2) = data.at(2) * z_scalar + z_const;
	
}

/*
int main() {
	
	//*** Setup ***
	cout << "Setting up.." << endl;
	time_t t = std::time(0); 
	srand(t);
	int x_loc = rand() % image_frame_width + 1;
	int y_loc = rand() % image_frame_height + 1;
	int z_loc = rand() % maximum_depth + 1;
	//*** End of setup ***
	
	cout << "Running algorithm" << endl;
	vector<float> location_data;
	location_data.push_back(x_loc);
	location_data.push_back(y_loc);
	location_data.push_back(z_loc);
	cout<< "Size: " << location_data.size() << endl;
	
	find_trajectory(location_data);
	
	for (int i : location_data) {
		cout << i << endl;
	}
	
	return 0;
}
*/