// Reads the IR remote and prints each button's code on the serial monitor.
// Wiring: IR receiver G -> GND, R -> pin 13 (used as power), Y -> pin 12 (signal).
#include <IRremote.hpp>

const int IR_SIGNAL_PIN = 12;
const int IR_POWER_PIN = 13;

void setup() {
  Serial.begin(9600);
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);  // gives power to the IR receiver
  IrReceiver.begin(IR_SIGNAL_PIN);
  Serial.println("Ready. Press a button on the remote.");
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
}
