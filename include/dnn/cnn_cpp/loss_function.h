
#pragma once
#include "util.h"

// mean-squared-error loss function

float_t mse_f(float_t y, float_t t) {
  return (y - t) * (y - t) / 2;
}

float_t mse_df(float_t y, float_t t) {
  return y - t;
}


float_t cross_entropy_f(float_t y, float_t t) {
  return -t * log(y) - (1.0 - t) * log(1.0 - y);
}

float_t cross_entropy_df(float_t y, float_t t) {
  return (y - t) / (y * (1 - y));
}

#define LOSSFUNCTION_INIT_DEF(NAME)  void NAME ## _init(struct LossFunction* s) { (s->f = NAME ## _f, s->df = NAME ## _df); }

LOSSFUNCTION_INIT_DEF(mse)
LOSSFUNCTION_INIT_DEF(cross_entropy)
