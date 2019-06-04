#include "mbed.h"
#include "Servo.h"

SPI spi(D11, D12, D13);
DigitalOut LEFT_MOTOR(D14);
DigitalOut RIGHT_MOTOR(D15);
Servo myservo(D6);

#define input_start 2.3 // a
#define input_end 19.7  // b

#define output_start 0  // c
#define output_end 64   // d

// speed is any integer between 0 and 64.
// this corresponds to a resistor value between 0 and 100K Ohms
void set_speed(int speed){
    spi.write(0x00);
    spi.write(speed);
}

//
void build_speed(int speed){
    for (int i = 1; i <= speed; i++){
        set_speed(i);
        wait(0.25);
    }
}

int map_distance(float distance){
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (distance - input_start);
}

void drop_ball(){

    myservo.write(0);
    wait(3);

    myservo.write(140);
    wait(3);
}

// 19 ==> 13 feet
// 9 ==> 6 feet


int main() {

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 10MHz clock rate
    spi.format(8,0);
    spi.frequency(1000000); // this should be 10 MHz


    // deselect the right motor
    RIGHT_MOTOR.write(1);
    LEFT_MOTOR.write(1);

    while(1) {

        // select the left motor
        LEFT_MOTOR.write(0);
        RIGHT_MOTOR.write(0);

        // speed zero corresponds to the motors being off
        set_speed(0);
        wait(10);

        // you MUST rev the motor first
        set_speed(1);
        wait(3);

        // now you BUILD the speed. Don't set speed or the
        // current will flow to the motor with less resistivity
        build_speed(map_distance(9));
        drop_ball();
        wait(10);


        // deselect motor
        LEFT_MOTOR.write(1);
        RIGHT_MOTOR.write(1);
    }
}
