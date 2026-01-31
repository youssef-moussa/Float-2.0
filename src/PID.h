#ifndef PID_H
#define PID_H

class PID {
  public:
    PID(float kp, float ki, float kd,
        float outMin, float outMax);

    void setTunings(float kp, float ki, float kd);
    void setOutputLimits(float minOut, float maxOut);
    void reset();

    float compute(float setpoint, float measurement);

  private:
    float Kp, Ki, Kd;
    float outputMin, outputMax;

    float integral;
    float prevError;

    unsigned long lastTime;
};

#endif
