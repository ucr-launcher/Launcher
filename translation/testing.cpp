//Testing file. Uses the Catch2 framework: https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#top

#include "catch.hpp"
#include <random>

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
	#include "trajectory.h"
#endif

#ifndef REINFORCEMENT_TRAINER_H
#define REINFORCEMENT_TRAINER_H
	#include "reinforcement_trainer.h"
#endif

//Test the find_trajectory's error checking by seeing if it responds correctly to being given vectors of a wrong size
TEST_CASE( "Vector is wrong size, error thrown", "[find_trajectory_input_size_check]" ) {
	
	time_t t = std::time(0); 
	srand(t);
	
	std::vector<float> v_large; //vector that is too large
	std::vector<float> v_small; //vector that is too small
	std::vector<float> v_reg;   //vector that is the right size

	for (int i = 0; i < 4; i++) {
		v_large.push_back(rand() % 100);
	}

	for (int i = 0; i < 2; i++) {
		v_small.push_back(rand() % 100);
	}
	
	for (int i = 0; i < 3; i++) {
		v_reg.push_back(rand() % 100);
	}
	
    REQUIRE(find_trajectory(v_small) == -1);
	REQUIRE(find_trajectory(v_large) == -1);
	REQUIRE(find_trajectory(v_reg)   == 0);
}

TEST_CASE("Vector does not change in size", "[find_trajectory_output_size_check") {
	
	time_t t = std::time(0); 
	srand(t);
	
	std::vector<float> coordinates;
	coordinates.push_back(rand() % 100);
	coordinates.push_back(rand() % 100);
	coordinates.push_back(rand() % 100);
	
	REQUIRE(coordinates.size() == 3); //Ensure that the vector is the right size before translation
	
	find_trajectory(coordinates); //translate the coordinate vector
	
	REQUIRE(coordinates.size() == 3); //Ensure that the vector is still the right size after translation
}