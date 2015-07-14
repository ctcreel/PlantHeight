#include "DEBUG.h"
#include "../../Projects/HydroManager/Commands.h"
#include <SoftwareSerial.h>
#include <TextFinder.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "eventStream.h"
#include "eventHandler.h"
#include "eventCallBack.h"
#include "generatorDeviceID.h"

#define RANGE_FINDER_DATA 53
#define RANGE_FINDER_POWER 52
#define ACCEPTABLE_HEIGHT 18
#define MEASURE_EVERY 

generatorDeviceID gID;
SoftwareSerial ss(11,10);
eventStream *e;
int lastNotified;

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(19200);
  pinMode(RANGE_FINDER_DATA, INPUT);
  pinMode(RANGE_FINDER_POWER, OUTPUT);
  digitalWrite(RANGE_FINDER_POWER, HIGH);
  lastNotified = 0;
  ss.begin(BAUD_RATE);
  e = new eventStream(&ss,&gID);
  new eventIncoming(e, setHeight, SET_DISTANCE);
}

void loop() {
  e->check();
}

void setHeight(const unsigned long v) {
  const float potHeight = 15.0;
  const float lightHeight = 6.0;
  const float tentHeight = 70;
  float height = tentHeight - (((float) pulseIn(RANGE_FINDER_DATA, HIGH)) / 147.0 + (potHeight + lightHeight));
  DEBUG("Plant is "+String(height) + " high");
  e->createEvent(height, SET_HEIGHT);
}

