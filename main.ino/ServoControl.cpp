#include "ServoControl.h"

ServoControl::ServoControl(int pin) {
    servoPin = pin;
    myservo.attach(servoPin);
}

void ServoControl::activateServo(int times, int delayTime) {
    for (int i = 0; i < times; i++) {
        // ���� ���͸� 0������ 180���� �ε巴�� ȸ��
        for (int pos = 0; pos <= 180; pos += 1) {
            myservo.write(pos);
            delay(delayTime); // delayTime ms ���
        }
        // ���� ���͸� 180������ 0���� �ε巴�� ȸ��
        for (int pos = 180; pos >= 0; pos -= 1) {
            myservo.write(pos);
            delay(delayTime); // delayTime ms ���
        }
    }
}
