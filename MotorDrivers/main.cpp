#include "mbed.h"
 
DigitalOut D_8(D8);
DigitalOut D_9(D9);
DigitalOut D_10(D12);
DigitalOut D_11(D11);
DigitalOut myled(LED1);

Timer t; 


void turn_left_n_steps(int steps){
    for (int i = 0; i < steps; i++) {
            
        D_8.write(1);
        D_9.write(1);
        D_10.write(0);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(1);
        D_10.write(1);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(0);
        D_10.write(1);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(1);
        D_9.write(0);
        D_10.write(0);
        D_11.write(1);
        
        wait_ms(10);
    }
    
}

void turn_right_n_steps(int steps) {
       for (int i = 0; i < steps; i++) {
            
        D_8.write(1);
        D_9.write(0);
        D_10.write(0);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(0);
        D_10.write(1);
        D_11.write(1);
        
        wait_ms(10);
        
        D_8.write(0);
        D_9.write(1);
        D_10.write(1);
        D_11.write(0);
        
        wait_ms(10);
        
        D_8.write(1);
        D_9.write(1);
        D_10.write(0);
        D_11.write(0);
        
        wait_ms(10);
    }
}


int main() {
    while(1) {
        turn_right_n_steps(5);
        turn_left_n_steps(5);
    }
}
