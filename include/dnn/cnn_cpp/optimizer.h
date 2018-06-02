
#pragma once
#include "util.h"

// gradient descent with 2nd-order update(LeCun,1998)
static void gradient_descent_levenberg_marquardt_update(struct Optimizer* s, float_t dW, float_t H, float_t *W) {
  *W -= (s->alpha / (H + s->mu)) * (dW); // 7.2%
}

void gradient_descent_levenberg_marquardt_init(struct Optimizer* s, float_t alpha = 0.00085, float_t mu = 0.02) {
  s->update = gradient_descent_levenberg_marquardt_update;
  s->alpha = alpha;
  s->mu = mu;
  s->requires_hessian = true;
}


// simple SGD algorithm
void gradient_descent_update(struct Optimizer* s, float_t dW, float_t H, float_t *W) {
  *W -= s->alpha * ((dW) + *W * s->lambda); // 7.2%
}


void gradient_descent_init(struct Optimizer* s, float_t alpha = 0.01, float_t lambda = 0.0) {
  s->update = gradient_descent_update;
  s->alpha = alpha;
  s->lambda = lambda;
  s->requires_hessian = false;
}
