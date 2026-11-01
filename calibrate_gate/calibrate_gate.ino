// Finds the servo angles where the gate is closed and open.
//   CH+ : turn the arm 5 degrees up (hold to keep moving)
//   CH- : turn the arm 5 degrees down
//   CH  : mark the current angle (1st mark = closed, 2nd mark = open)
// The angle is printed on the serial monitor at 9600 baud.
// Wiring is the same as cat_feeder.ino.
#include <Servo.h>
#include <IRremote.hpp>

const int SERVO_PIN = 9;
const int IR_SIGNAL_PIN = 12;
const int IR_POWER_PIN = 13;

const uint8_t BUTTON_CH_MINUS = 0x45;
const uint8_t BUTTON_CH = 0x46;
const uint8_t BUTTON_CH_PLUS = 0x47;

Servo gate;
int angle = 0;
int marks = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);
  IrReceiver.begin(IR_SIGNAL_PIN);
  gate.attach(SERVO_PIN);
  gate.write(angle);
  Serial.println("Ready. CH+ and CH- move the arm, CH marks the angle.");
  Serial.print("angle ");
  Serial.println(angle);
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t button = IrReceiver.decodedIRData.command;
    bool isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
    if (button == BUTTON_CH_PLUS || button == BUTTON_CH_MINUS) {
      angle += (button == BUTTON_CH_PLUS) ? 5 : -5;
      angle = constrain(angle, 0, 180);
      gate.write(angle);
      Serial.print("angle ");
      Serial.println(angle);
    } else if (button == BUTTON_CH && !isRepeat) {
      marks++;
      Serial.print("MARK ");
      Serial.print(marks);
      Serial.print(" angle ");
      Serial.println(angle);
    }
    IrReceiver.resume();
  }
}
