#include "Servo.h"

Servo::Servo(PinName pin) : _servo(pin) {}
    _servo.period_ms(20);
}

Servo::~Servo(void) { }

void Servo::write(unsigned char degree) {
    convert(degree);
    _servo.pulsewidth_us(pulse);
}

void Servo::convert(unsigned char degree) {
    // 0~180 degree correspond to 500~2500
    pulse = degree * 11 + 500;
}
