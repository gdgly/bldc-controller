#include "pid.h"

/* Initialization and configuration functions */
PID::PID (float kp, float ki, float kd, float interval) {
  interval_ = interval;
  kp_ = kp, ki_ = ki/interval, kd_ = kd;
  min_ = 0.0f;
  max_ = 0.0f;

  target_ = 0.0f;

  windup_ = 0.0f;

  alpha_ = 0.0f;
  deriv_ = 0.0f;
}

void PID::setGains (float kp, float ki, float kd) {
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

void PID::setLimits (float min, float max) {
  min_ = min;
  max_ = max;
  target_ = (max + min) / 2.0f;
}

void PID::setTarget (float target) {
  target_ = target;
}

void PID::setAlpha (float alpha) {
  alpha_ = alpha;
}
/* End of Initialization and Config Functions */

/* Compute code for PID */
// Windup limited by proportional term exceeding bounds!
float PID::computeIntegral (float err, float p_out) {
  windup_ += err;

  if (windup_*ki_ > max_) {
    windup_ = max_/ki_;
  } else if (windup_*ki_ < min_) {
    windup_ = min_/ki_;
  }

  return windup_;
}

// Use alpha to approximate derivative over time (remove HF noise in err)
float PID::computeDerivative (float err) {
  deriv_ = (alpha_ * err) + ((1-alpha_) * deriv_);
  return deriv_;
}

float PID::compute (float val) {
  float err = target_ - val;
  float p, i, d;

  p = kp_ * err;
  i = ki_ * computeIntegral(err, p);
  d = kd_ * computeDerivative(err);

  float output = p + i + d;
  if (output > max_) {
    output = max_;
  } else if (output < min_) {
    output = min_;
  }

  return output;
}
/* End of PID Compute */