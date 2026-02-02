#ifndef DEPTH_SENSOR_H
#define DEPTH_SENSOR_H

class DepthSensor {
  public:
    bool begin();
    float readDepth();        // meters
    float readPressure();     // mbar
    float readTemperature();  // °C

  private:
    bool initialized = false;
};

#endif
