// Cat feeder gate: an IR remote moves the servo arm that opens and closes the funnel.
//   CH+ : open and stay open
//   CH  : open for OPEN_TIME_MS, then close by itself
//   CH- : close
// Wiring: servo orange -> pin 9, red -> 5V, brown -> GND.
//         IR receiver G -> GND, R -> pin 13 (used as power), Y -> pin 12 (signal).
#include <Servo.h>
#include <IRremote.hpp>

const int SERVO_PIN = 9;
const int IR_SIGNAL_PIN = 12;
const int IR_POWER_PIN = 13;

const int CLOSED_ANGLE = 155;  // found with calibrate_gate: the cap covers the neck
const int OPEN_ANGLE = 65;     // 90 degrees from closed
const unsigned long OPEN_TIME_MS = 2500;

const uint8_t BUTTON_CH_MINUS = 0x45;
const uint8_t BUTTON_CH = 0x46;
const uint8_t BUTTON_CH_PLUS = 0x47;

Servo gate;
bool timedOpen = false;
unsigned long openedAt = 0;

void openGate() {
  gate.write(OPEN_ANGLE);
}

void closeGate() {
  gate.write(CLOSED_ANGLE);
  timedOpen = false;
}

void setup() {
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);  // gives power to the IR receiver
  IrReceiver.begin(IR_SIGNAL_PIN);
  gate.attach(SERVO_PIN);
  closeGate();
}

void loop() {
  if (IrReceiver.decode()) {
    bool isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
    if (!isRepeat) {  // ignore the repeats sent while a button is held
      uint8_t button = IrReceiver.decodedIRData.command;
      if (button == BUTTON_CH_PLUS) {
        timedOpen = false;
        openGate();
      } else if (button == BUTTON_CH) {
        openGate();
        timedOpen = true;
        openedAt = millis();
      } else if (button == BUTTON_CH_MINUS) {
        closeGate();
      }
    }
    IrReceiver.resume();
  }

  if (timedOpen && millis() - openedAt >= OPEN_TIME_MS) {
    closeGate();
  }
}
