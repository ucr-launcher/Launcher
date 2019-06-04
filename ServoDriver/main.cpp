#include "mbed.h"
#include "Servo.h"

Servo myservo(D6);

void drop_ball(){

    myservo.write(0);
    wait(3);

    myservo.write(140);
    wait(3);
}

int main(){

    while (1) {
        drop_ball();
    }

    return 0;
}
