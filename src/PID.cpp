#include "PID.h"
#include <Arduino.h>

PID::PID(float kp, float ki, float kd,
         float outMin, float outMax)
{
  Kp = kp;
  Ki = ki;
  Kd = kd;

  outputMin = outMin;
  outputMax = outMax;

  integral = 0.0;
  prevError = 0.0;
  lastTime = millis();
}

void PID::setTunings(float kp, float ki, float kd) {
  Kp = kp;
  Ki = ki;
  Kd = kd;
}

void PID::setOutputLimits(float minOut, float maxOut) {
  outputMin = minOut;
  outputMax = maxOut;
}

void PID::reset() {
  integral = 0.0;
  prevError = 0.0;
  lastTime = millis();
}

float PID::compute(float setpoint, float measurement) {

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;

  if (dt <= 0.0) return 0.0;

  float error = setpoint - measurement;

  // Integral (anti-windup)
  integral += error * dt;
  integral = constrain(integral, -10.0, 10.0);

  float derivative = (error - prevError) / dt;

  float output =
      (Kp * error) +
      (Ki * integral) +
      (Kd * derivative);

  output = constrain(output, outputMin, outputMax);

  prevError = error;
  lastTime = now;

  return output;
}
