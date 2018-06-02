#ifndef MDL_COMMONS_H
#define MDL_COMMONS_H

// #define NEED_DUMP true
#define MULTI_THREAD true
/**
 * This is an empirical value indicating how many inception layers could be accelerated by multi-thread.
 */
#define MAX_INCEPTION_NUM  9

#ifdef ANDROID
#include <android/log.h>
#include "neon_mathfun.h"
#endif

#ifdef __arm__
#include <arm_neon.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>



#ifdef ANDROID
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, mdl::log_tag, __VA_ARGS__); printf(__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARNING, mdl::log_tag, __VA_ARGS__); printf(__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, mdl::log_tag, __VA_ARGS__); printf(__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, mdl::log_tag, __VA_ARGS__); printf(__VA_ARGS__)
#else
#define LOGI printf
#define LOGW printf
#define LOGD printf
#define LOGE printf
#endif




const char *log_tag = "MDL LOG built on " __DATE__ " " __TIME__;

const int string_size = 30;

const int model_version = 1;

const char* matrix_name_data = "data";

const char* matrix_name_test_data = "test-data";

inline bool is_a_ge_zero_and_a_lt_b(int a, int b) {
    return static_cast<unsigned>(a) < static_cast<unsigned>(b);
}

void im2col(const float *data_im, const int channels, const int height,
            const int width, const int kernel_size,
            const int pad, const int stride, float *data_col) {
    const int output_h = (height + 2 * pad - kernel_size) / stride + 1;
    const int output_w = (width + 2 * pad - kernel_size) / stride + 1;
    const int channel_size = height * width;
    for (int channel = channels; channel--; data_im += channel_size) {
        for (int kernel_row = 0; kernel_row < kernel_size; kernel_row++) {
            for (int kernel_col = 0; kernel_col < kernel_size; kernel_col++) {
                int input_row = -pad + kernel_row;
                for (int output_rows = output_h; output_rows; output_rows--) {
                    if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
                        for (int output_cols = output_w; output_cols; output_cols--) {
                            *(data_col++) = 0;
                        }
                    } else {
                        int input_col = -pad + kernel_col;
                        for (int output_col = output_w; output_col; output_col--) {
                            if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                                *(data_col++) = data_im[input_row * width + input_col];
                            } else {
                                *(data_col++) = 0;
                            }
                            input_col += stride;
                        }
                    }
                    input_row += stride;
                }
            }
        }
    }
}


bool equal(float a, float b) {
    const float EPSILON = 1e-5;
    if (fabsf(a - b) < EPSILON) {
        return true;
    }
    return false;
}

void copy(int length, float *x, float *y) {
    if (x != y) {
        memcpy(y, x, sizeof(float) * length);
    }
}


#endif
