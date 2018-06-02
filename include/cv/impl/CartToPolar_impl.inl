{

  arrtype* mag_buffer = 0;
  arrtype* x_buffer = 0;
  arrtype* y_buffer = 0;
  int block_size = 0;
  int coi1 = 0, coi2 = 0, coi3 = 0, coi4 = 0;
  TypeId type;
  int x, y;
  int cont_flag = CC_MAT_CONT_FLAG;

  block_size = MIN(size.w, ICC_MATH_BLOCK_SIZE);

  if (type == CC_32F && mag) {
    mag_buffer = (arrtype*)cvStackAlloc(block_size * sizeof(arrtype));
  }

  for (y = 0; y < size.h; y++) {
    arrtype* xarr = (xmat + xstep * y);
    arrtype* yarr = (ymat + ystep * y);
    arrtype* magarr = mag ? (mag + magstep * y) : 0;
    arrtype* angle_data = angle ? (angle + anglestep * y) : 0;

    if (mag) {
#include "icv_def_sqr_magnitude_func.inl"

      for (x = 0; x < size.w; ++x) {
        magarr[x] = sqrt(magarr[x]);
      }
    }

    if (angle) {
      icvFastArctan_32f(yarr + x, xarr + x, angle_data + x, len);

      if (!angle_in_degrees) {
        icvScale_32f(angle_data + x, angle_data + x, len, (arrtype)(CC_PI / 180.), 0);
      }
    }
  }

#if 0
  {
    for (y = 0; y < size.h; y++) {
      double* xarr = (double*)(xmat + xstep * y);
      double* yarr = (double*)(ymat + ystep * y);
      double* mag_data = mag ? (double*)(mag + mag->s * y) : 0;
      double* angle_data = angle ? (double*)(angle + angle->s * y) : 0;

      for (x = 0; x < size.w; x += block_size) {
        int len = MIN(size.w - x, block_size);

        if (angle) {
          icvCvt_64f32f(xarr + x, x_buffer, len);
          icvCvt_64f32f(yarr + x, y_buffer, len);
        }

        if (mag) {
          icvSqrMagnitude_64f(xarr + x, yarr + x, mag_data + x, len);
          icvSqrt_64f(mag_data + x, mag_data + x, len);
        }

        if (angle) {
          icvFastArctan_32f(y_buffer, x_buffer, x_buffer, len);

          if (!angle_in_degrees) {
            icvScale_32f(x_buffer, x_buffer, len, (arrtype)(CC_PI / 180.), 0);
          }

          icvCvt_32f64f(x_buffer, angle_data + x, len);
        }
      }
    }
  }
#endif
}
