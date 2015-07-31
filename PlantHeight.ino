#include "DEBUG.h"
#include "../../Projects/HydroManager/Commands.h"
#include "../../Projects/HydroManager/Config.h"
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
#define SENSOR_HEIGHT 2

generatorDeviceID gID;
eventStream *e;
int lastNotified;

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(19200);
  pinMode(RANGE_FINDER_DATA, INPUT);
  pinMode(RANGE_FINDER_POWER, OUTPUT);
  digitalWrite(RANGE_FINDER_POWER, HIGH);
  lastNotified = 0;
  Serial3.begin(BAUD_RATE);
  e = new eventStream(&Serial3,&gID);
  new eventIncoming(e, setHeight, SET_DISTANCE);
}

void loop() {
  e->check(2);
}

void setHeight(const unsigned long distanceToPlant) {
  const float potHeight = POT_HEIGHT;
  const float sensorHeight = SENSOR_HEIGHT;
  const float lightHeight = LIGHT_HEIGHT;
  const float tentHeight = TENT_HEIGHT;
  const float distanceToTop = getDistance();
  const float height = tentHeight - (potHeight + sensorHeight + lightHeight + distanceToPlant  + distanceToTop);
  DEBUG("Distance to top is " + String(distanceToTop) + ", Distance to plant is "+String(distanceToPlant)+", Plant is "+String(height) + " high");
  e->createEvent(height, SET_HEIGHT);
}

const unsigned long getDistance(void) {
  return (((float) pulseIn(RANGE_FINDER_DATA, HIGH)) / 147.0);
}

