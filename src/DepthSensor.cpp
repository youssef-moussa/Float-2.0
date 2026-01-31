#include "DepthSensor.h"
#include <Wire.h>
#include <MS5837.h>

static MS5837 sensor;

bool DepthSensor::begin() {

  Wire.begin();

  if (!sensor.init()) {
    initialized = false;
    return false;
  }

  // BAR30 = MS5837-30BA
  sensor.setModel(MS5837::MS5837_30BA);

  // Fresh water (important!)
  sensor.setFluidDensity(997);  // kg/m^3

  initialized = true;
  return true;
}

float DepthSensor::readDepth() {

  if (!initialized) return 0.0;

  sensor.read();
  return sensor.depth();  // meters
}

float DepthSensor::readPressure() {

  if (!initialized) return 0.0;

  sensor.read();
  return sensor.pressure(); // mbar
}

float DepthSensor::readTemperature() {

  if (!initialized) return 0.0;

  sensor.read();
  return sensor.temperature(); // °C
}
