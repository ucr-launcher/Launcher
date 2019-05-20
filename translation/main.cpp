//Main file. All global operations go here.

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
	#include "trajectory.h"
#endif

#ifndef REINFORCEMENT_TRAINER_H
#define REINFORCEMENT_TRAINER_H
	#include "reinforcement_trainer.h"
#endif

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, char* argv[]) {
	
	//Perform tests
	int result = Catch::Session().run( argc, argv );
	
	train();
	
	return 0;
}