#include "ServoControl.h"

ServoControl::ServoControl(int pin) {
    servoPin = pin;
    myservo.attach(servoPin);
}

void ServoControl::activateServo(int times, int delayTime) {
    for (int i = 0; i < times; i++) {
        // 서보 모터를 0도에서 180도로 부드럽게 회전
        for (int pos = 0; pos <= 180; pos += 1) {
            myservo.write(pos);
            delay(delayTime); // delayTime ms 대기
        }
        // 서보 모터를 180도에서 0도로 부드럽게 회전
        for (int pos = 180; pos >= 0; pos -= 1) {
            myservo.write(pos);
            delay(delayTime); // delayTime ms 대기
        }
    }
}
