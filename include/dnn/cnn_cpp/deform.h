
#pragma once
#include "util.h"

void corrupt(vec_t& in, float_t corruption_level, float_t min_value, vec_t *out) {
    *out = in;

    vec_t v(out->size());

    uniform_rand(v.begin(), v.end(), 0.0, 1.0);

    int n = out->size();
    for (int i = 0; i < n; i++)
        (*out)[i] = (v[i] <= corruption_level) ? min_value : (*out)[i];
}

