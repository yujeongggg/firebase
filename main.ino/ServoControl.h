#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

class ServoControl {
public:
    ServoControl(int pin);
    void activateServo(int times, int delayTime);
private:
    Servo myservo;
    int servoPin;
};

#endif // SERVO_CONTROL_H
