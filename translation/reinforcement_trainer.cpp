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
	
	// Continues to loop until the user enters the TERMINATE character
	while(feedback != TERMINATE) {
		
		cout << "Feedback: "; // User prompt
		cin >> feedback;      // Get user response
	
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
			case INFORM: //prints what each possible input char is and what it is for
				cout << "Positive Result: " << POSITIVE_RESULT << endl;
				cout << "Negative Result: " << NEGATIVE_RESULT << endl;
				cout << "Exit: " << TERMINATE << endl;
				cout << "Help: " << INFORM << endl;
				break;
			default:
				cout << "Warning: Character not recognized" << endl;
				//warn(string("That character is not recognized. Please enter ") + POSITIVE_RESULT + string(" for a positive response, ") + NEGATIVE_RESULT + string(" for a negative response, or ") + TERMINATE + string(" to exit"));
				break;
		}
	}

}



