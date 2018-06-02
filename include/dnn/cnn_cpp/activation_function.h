
#ifndef _ACTIVATION_FUNCTION_H_
#define _ACTIVATION_FUNCTION_H_

#include "util.h"

static float_t identity_activation_f(float_t x) { return x; }
static float_t identity_activation_df(float_t f_x) { return 1; }  
//std::pair<float_t, float_t> scale() { return std::make_pair(0.1, 0.9); }

float_t sigmoid_activation_f(float_t x) { return 1.0 / (1.0 + exp(-x)); }
float_t sigmoid_activation_df(float_t f_x) { return f_x * (1.0 - f_x); }
//std::pair<float_t, float_t> scale() { return std::make_pair(0.1, 0.9); }


float_t rectified_linear_f(float_t x) { return MAX((float_t)0.0, x); }
float_t rectified_linear_df(float_t f_x) { return f_x > 0.0 ? 1.0 : 0.0; }
// std::pair<float_t, float_t> scale() { return std::make_pair(0.1, 0.9); }


float_t tanh_activation_f(float_t x) {
    const float_t ep = exp(x);
    const float_t em = exp(-x); 
    return (ep - em) / (ep + em);
}

// fast approximation of tanh (improve 2-3% speed in LeNet-5)
/*float_t f(float_t x) {
    const float_t x2 = x * x;
    x *= 1.0 + x2 * (0.1653 + x2 * 0.0097);
    return x / std::sqrt(1.0 + x * x);// invsqrt(static_cast<float>(1.0 + x * x));
}*/

float_t tanh_activation_df(float_t f_x) { return 1.0 - f_x * f_x; }
//std::pair<float_t, float_t> scale() const { return std::make_pair(-0.8, 0.8); }

/*float invsqrt(float x) const {
    float x2 = x * 0.5f;
    long i = *reinterpret_cast<long*>(&x);

    i = 0x5f3759df - (i >> 1);
    x = *reinterpret_cast<float*>(&i);
    x = x * (1.5f - (x2 * x * x));
    return x;
}*/



#define ACTIVATION_DEFDEF(DEF) \
DEF(identity_activation, 0.1, 0.9) \
DEF(sigmoid_activation, 0.1, 0.9) \
DEF(rectified_linear, 0.1, 0.9) \
DEF(tanh_activation, -0.8, 0.8) \

#define ACTIVATION_DEF(a, b, c)  a ## _init(struct Activation* s) { (s->name = #a, s->scale.first = (float_t)b, s->scale.second = (float_t)c, s->f = a ## _f, s->df = a ## _df); }
ACTIVATION_DEFDEF(ACTIVATION_DEF)
#undef ACTIVATION_DEF



#endif // _ACTIVATION_FUNCTION_H_
