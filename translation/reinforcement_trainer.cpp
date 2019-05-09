#ifndef TRAJECTORY_H
#define TRAJECTORY_H
	#include "trajectory.h"
#endif

#ifndef REINFORCEMENT_TRAINER_H
#define REINFORCEMENT_TRAINER_H
	#include "reinforcement_trainer.h"
#endif

#include <fstream>

using namespace std;

void save_model() {
	ofstream ofs;
	ofs.open (MODEL_NAME);
	ofs << x_scalar << endl << y_scalar << endl << z_scalar << endl << x_const << endl << y_const << endl << z_const << endl;
	ofs.close();
}

void train() {
}



