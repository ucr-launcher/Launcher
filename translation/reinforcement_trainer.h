#include <stack>
#include <fstream>

#ifndef S_UTILS_H
#define S_UTILS_H
	#include "s_utils.h"
#endif

//**** Variables ****

// Input set that tells the trainer how the algorithm failed
const char POSITIVE_RESULT = 'p';
const char NEGATIVE_RESULT = 'n';
const char TERMINATE = 'x'; // to finish training and exit

//Identifiers for actions that the trainer can take.
enum decision { INCREASE_X, DECREASE_X, INCREASE_Y, DECREASE_Y, INCREASE_Z, DECREASE_Z};

const float LEARNING_RATE = 1; // size of the step taken in responce to reinforcement--quantity by which each value is modified.

const std::string PROMPT = "Enter bias: "; // user prompt
const std::string MODEL_NAME = "model.grp3"; // the name of the file where the model's progress is saved

//**** Functions ****

void save_model(); //dumps the current training values into a text file

void train(); //performs the actual training