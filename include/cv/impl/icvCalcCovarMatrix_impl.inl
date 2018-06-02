{
  typedef double dsttype;
  int i, j;
  int vec_delta = 0, vec_step = 0;
  int is_covar_normal = (flags & CC_COVAR_NORMAL) != 0;
  double scale;
  dsttype* tempvec = 0;
  int avg_rows, avg_cols;
  int count, mean_len;
  if(flags & CC_COVAR_COLS) {
    mean_len = size.h;
    count = size.w;
  } else {
    mean_len = size.w;
    count = size.h;
  }

  if (flags & (CC_COVAR_ROWS | CC_COVAR_COLS)) {
    int order = ((flags & CC_COVAR_ROWS) != 0) ^ ((flags & CC_COVAR_NORMAL) == 0);
    if (!(flags & CC_COVAR_USE_AVG)) {
      cvReduceAvg_64f(vec, vecstep, avg, 1, 1, size, (flags & CC_COVAR_COLS) > 0);
    }

    scale = !(flags & CC_COVAR_SCALE) ? 1. : 1. / count;
    cvMulTransposed_64f(vec, vecstep, cov, covstep, order, avg, 0, size, size.w, scale);
    return 0;
  }
  
#if 0
  scale = !(flags & CC_COVAR_SCALE) ? 1. : 1. / count;

  if (is_covar_normal) {
    if (count <= 0) {
      CC_ERROR(CC_StsBadSize, "The number of vectors is zero or negative");
    }

    //if (cov->rows != contsize.w) { CC_ERROR(CC_StsUnmatchedSizes, "The size of input vectors does not match with the size of covariation matrix");  }
  }

  tempvec = MALLOC(dsttype, size.h * size.w);

  //else if (count != cov->rows) { CC_ERROR(CC_StsUnmatchedSizes, "The vector count and covariance matrix size do not match"); }

  if (!(flags & (CC_COVAR_ROWS | CC_COVAR_COLS))) {
    if (!(flags & CC_COVAR_USE_AVG)) {
      FILL(mean_len, avg, 0);
      if(flags & CC_COVAR_COLS) {
        for (i = 0; i < size.h; i++) {
          double* vec1 = vec + i* vecstep, sum=0;
          for (j=0; j<size.w; ++j) {
            sum += vec1[j];
          }
          avg[i] = sum/size.w;
        }
      } else {
        for (i = 0; i < size.h; i++) {
          double* vec1 = vec + i* vecstep;
          for (j=0; j<size.w; ++j) {
            avg[j] += vec1[j];
          }
        }
        for (j=0; j<size.w; ++j) {
          avg[j] /= size.h;
        }
      }
    }
  }

  if (!is_covar_normal) {
    for (i = 0; i < count; i++) {
      int a, b, delta;

      if (!(i & 1)) {
        a = 0, b = i + 1, delta = 1;
      }
      else {
        a = i, b = -1, delta = -1;
      }

      for (j = a; j != b; j += delta) {
        double result = 0;
        int* v_i, *v_j;
        int step_i, step_j;

        if (!vecmat) {
          v_i = vecdata[i].ptr;
          v_j = vecdata[j].ptr;
          step_i = vecdata[i]->step;
          step_j = vecdata[j]->step;
        }
        else {
          v_i = vecmat + vec_delta * i;
          v_j = vecmat + vec_delta * j;
          step_i = step_j = vec_step;
        }

        //dot_func(v_i, step_i, v_j, step_j, avg, avgstep, srcsize, &result);

        {
          cov[i * covstep + j] = cov[j * covstep + i] = (dsttype)(result * scale);
        }
      }
    }
  }
  
  else
  {
    uchar* cov_ptr = cov;
    int cov_size = cov->rows;

    cvZero(cov);

    for (i = 0; i < count; i++) {
      int* v;
      int vstep;

      if (!vecmat) {
        v = vecdata[i].ptr;
        vstep = vecdata[i]->step;
      }
      else {
        v = vecmat + vec_delta * i;
        vstep = vec_step;
      }

      //ext_func(v, vstep, avg, avgstep, cov_ptr, covstep, srcsize, tempvec);
    }

    for (i = 0; i < cov_size; i++) {
      for (j = 0; j <= i; j++) {
        double* cov1 = (cov_ptr + i * covstep) + j;
        double* cov2 = (cov_ptr + j * covstep) + i;

        if (flags & CC_COVAR_SCALE) {
          *cov1 = *cov2 = *cov1 * scale;
        }
        else {
          *cov2 = *cov1;
        }
      }
    }
  }

  cFree(&vecdata);
  cvReleaseMat(&tempvec);
#endif
}