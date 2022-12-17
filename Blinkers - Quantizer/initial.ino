#include "scales.h"

// PINs
#define CV_IN 36
#define GATE_IN 16
#define DAC1 25

// CVs
long cvIn = 0, cvInBefore = 0, cvOut = 0, dbgIn = 0;

// sample and hold trigger input
int gateState = 0;
int lastGateState = 0;

void setup() {
  pinMode(CV_IN, INPUT);
  pinMode(GATE_IN, INPUT_PULLDOWN);
  pinMode(DAC1, OUTPUT);
  Serial.begin(9600);
}

void next() {
  cvInBefore = cvIn;
  cvIn = analogRead(CV_IN);
  cvOut = getClosestNote(cvIn);

  Serial.print(cvIn);
  Serial.print(" - ");
  Serial.println(cvOut);

  dacWrite(DAC1, map(cvOut, 0, 3264, 0, 255));
}

bool hasGateChanged(int gateState) {
  bool changed = false;
  
  if (gateState != lastGateState) {
    changed = gateState == HIGH;
  }

  lastGateState = gateState;
  
  return changed;
}

void loop() {
  gateState = digitalRead(GATE_IN);

  if (hasGateChanged(gateState)) {
    next();
  }
}