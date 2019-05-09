#ifndef TRAJECTORY_H
#define TRAJECTORY_H
	#include "trajectory.h"
#endif

#ifndef REINFORCEMENT_TRAINER_H
#define REINFORCEMENT_TRAINER_H
	#include "reinforcement_trainer.h"
#endif

using namespace std;

void save_model() {
	ofstream ofs;
	ofs.open (MODEL_NAME);
	ofs << x_scalar << endl << y_scalar << endl << z_scalar << endl << x_const << endl << y_const << endl << z_const << endl;
	ofs.close();
}

void train() {

	char feedback = 0;
	
	while(feedback != 'x') {
	
		switch(feedback) {
			case POSITIVE_RESULT:
				//do something to reinforce last decision
				break;
			case NEGATIVE_RESULT:
				//Undo last decision, try something else	
				break;
			case TERMINATE:
				//We need to exit, so just break;
				break;
			default:
				warn("That character is not recognized. Please enter " + POSITIVE_RESULT + " for a positive response, " + NEGATIVE_RESULT + " for a negative response, or " + TERMINATE + " to exit");
		
		}
	
	}

}



