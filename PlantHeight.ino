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
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

#define RANGE_FINDER_DATA 53
#define RANGE_FINDER_POWER 52
#define ACCEPTABLE_HEIGHT 18
#define SENSOR_HEIGHT 2

generatorDeviceID gID;
eventStream *e;

void setup() {
  Serial.begin(19200);
  pinMode(RANGE_FINDER_DATA, INPUT);
  pinMode(RANGE_FINDER_POWER, OUTPUT);
  digitalWrite(RANGE_FINDER_POWER, HIGH);
  Serial3.begin(BAUD_RATE);
  e = new eventStream(&Serial3,&gID);
  new eventIncoming(e, setHeight, SET_DISTANCE);
  new eventOutgoing(e, getHumidity, SET_HUMIDITY, GET_HUMIDITY);
  new eventOutgoing(e, getAirTemp, SET_AIR_TEMP, GET_AIR_TEMP);
}

void loop() {
  e->check(10);
/*
  Serial.print("Distance ");
  Serial.println(getDistance());
  Serial.print("Humidity ");
  Serial.println(getHumidity());
  Serial.print("Temp ");
  Serial.println(getAirTemp());
*/  
}

void setHeight(const unsigned long distanceToPlant) {
  const float potHeight = POT_HEIGHT;
  const float sensorHeight = SENSOR_HEIGHT;
  const float lightHeight = LIGHT_HEIGHT;
  const float tentHeight = TENT_HEIGHT;
  const float distanceToTop = getDistance();
  const float height = tentHeight - (potHeight + sensorHeight + lightHeight + distanceToPlant  + distanceToTop);
  DEBUG("Distance to top is " + String(distanceToTop) + ", Distance to plant is "+String(distanceToPlant)+", Plant is "+String(height) + " high");
  if(height < 0) {
    e->createEvent((const unsigned long) 0L, SET_HEIGHT);
  } else {
    e->createEvent(height, SET_HEIGHT);
  }
}

const unsigned long getDistance(void) {
  return (((float) pulseIn(RANGE_FINDER_DATA, HIGH)) / 147.0);
}

const unsigned long getHumidity(void) {
  float h = dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  
  return (unsigned long) h;
}


const unsigned long getAirTemp(void) {
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  Serial.print("Temperature: ");
  Serial.print(f);

  return (unsigned long) f;
}

