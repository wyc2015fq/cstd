

// Compute y = (shift + scale * x)^power
static void power_forward(const float* prevData, float* nextData, float power, float scale, float shift, float diff_scale, int count) {
  // Special case where we can ignore the input: scale or power is 0.
  if (diff_scale == (Dtype)(0)) {
    Dtype value = (power == 0) ? (Dtype)(1) : pow(shift, power);
    blas_sset(count, value, nextData);
    return;
  }
  blas_scopy(count, prevData, nextData);
  if (scale != (Dtype)(1)) {
    blas_sscal(count, scale, nextData);
  }
  if (shift != (Dtype)(0)) {
    blas_sadd_scalar(count, shift, nextData);
  }
  if (power != (Dtype)(1)) {
    blas_spowx(count, nextData, power, nextData);
  }
}
static void power_backward(const float* prevData, Dtype* prevDiff, const float* nextData, const float* nextDiff, float power, float scale, float shift, float diff_scale, int count) {
  if (1) {
    if (diff_scale == (Dtype)(0) || power == (Dtype)(1)) {
      blas_sset(count, diff_scale, prevDiff);
    } else {
      // Compute dy/dx = scale * power * (shift + scale * x)^(power - 1)
      //               = diff_scale * y / (shift + scale * x)
      if (power == (Dtype)(2)) {
        // Special case for y = (shift + scale * x)^2
        //     -> dy/dx = 2 * scale * (shift + scale * x)
        //              = diff_scale * shift + diff_scale * scale * x
        blas_saxpby(count, diff_scale * scale, prevData,
            (Dtype)(0), prevDiff);
        if (shift != (Dtype)(0)) {
          blas_sadd_scalar(count, diff_scale * shift, prevDiff);
        }
      } else if (shift == (Dtype)(0)) {
        // Special case for y = (scale * x)^power
        //     -> dy/dx = scale * power * (scale * x)^(power - 1)
        //              = scale * power * (scale * x)^power * (scale * x)^(-1)
        //              = power * y / x
        blas_sdiv(count, nextData, prevData, prevDiff);
        blas_sscal(count, power, prevDiff);
      } else {
        blas_scopy(count, prevData, prevDiff);
        if (scale != (Dtype)(1)) {
          blas_sscal(count, scale, prevDiff);
        }
        if (shift != (Dtype)(0)) {
          blas_sadd_scalar(count, shift, prevDiff);
        }
        blas_sdiv(count, nextData, prevDiff, prevDiff);
        if (diff_scale != (Dtype)(1)) {
          blas_sscal(count, diff_scale, prevDiff);
        }
      }
    }
    if (diff_scale != (Dtype)(0)) {
      blas_smul(count, nextDiff, prevDiff, prevDiff);
    }
  }
}

/**
 * @brief Computes @f$ y = (\alpha x + \beta) ^ \gamma @f$,
 *        as specified by the scale @f$ \alpha @f$, shift @f$ \beta @f$,
 *        and power @f$ \gamma @f$.
 */
static Dtype PowerLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int count = Data_count(prev->size);
  Dtype power = FARG(power);
  Dtype scale = FARG(scale);
  Dtype shift = FARG(shift);
  Dtype diff_scale = power * scale;
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      power_forward(prevData, nextData, power, scale, shift, diff_scale, count);
    }
    break;
  case Op_Backward:
    {
      const float* nextData = getData(next);
      const float* nextDiff = getDiff(next);
      const float* prevData = getData(prev);
      float* prevDiff = setDiff(prev);
      power_backward(prevData, prevDiff, nextData, nextDiff, power, scale, shift, diff_scale, count);
    }
    break;
  default:
    break;
  }
  return 0;
}

