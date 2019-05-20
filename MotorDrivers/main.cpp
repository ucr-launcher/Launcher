#include "mbed.h"

DigitalOut motor(D8);
DigitalOut direction(D9);


void turn_stepper_left(int steps){
    direction.write(0);
    for(int i = 0; i < steps; i++){
        motor.write(0);
        wait_ms(1);
        motor.write(1);
    }
}

void turn_stepper_right(int steps){
    direction.write(1);
    for(int i = 0; i < steps; i++){
        motor.write(0);
        wait_ms(1);
        motor.write(1);
    }
}

int main() {
    while (1) {
        turn_stepper_left(500);
        turn_stepper_right(500);
    }
}
