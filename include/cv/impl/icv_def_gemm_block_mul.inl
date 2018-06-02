{
  int ii, jj, kk, n = _a_size.w, m = _d_size.w;
  const arrtype* _a_data = _a_data, *_b_data = _b_data;
  arrtype* a_buf = 0;
  size_t _a_step0, _a_step1, t_step;
  int do_acc = flags & 16;

  _a_step0 = _a_step;
  _a_step1 = 1;

  if (flags & CC_GEMM_A_T) {
    CC_SWAP(_a_step0, _a_step1, t_step);
    n = _a_size.h;
    a_buf = (arrtype*)malloc(n * sizeof(_a_data[0]));
  }

  if (flags & CC_GEMM_B_T) {
    /* second operand is transposed */
    for (ii = 0; ii < _d_size.h; ii++, _a_data += _a_step0, _d_data += _d_step) {
      _a_data = _a_data;
      _b_data = _b_data;

      if (a_buf) {
        for (kk = 0; kk < n; kk++) {
          a_buf[kk] = _a_data[_a_step1 * kk];
        }

        _a_data = a_buf;
      }

      for (jj = 0; jj < _d_size.w; jj++, _b_data += _b_step) {
        worktype s0 = do_acc ? _d_data[jj] : (0), s1 = (0);

        for (kk = 0; kk <= n - 2; kk += 2) {
          s0 += (worktype)(_a_data[kk]) * _b_data[kk];
          s1 += (worktype)(_a_data[kk + 1]) * _b_data[kk + 1];
        }

        for (; kk < n; kk++) {
          s0 += (worktype)(_a_data[kk]) * _b_data[kk];
        }

        _d_data[jj] = s0 + s1;
      }
    }
  }
  else
  {
    for (ii = 0; ii < _d_size.h; ii++, _a_data += _a_step0, _d_data += _d_step) {
      _a_data = _a_data, _b_data = _b_data;

      if (a_buf) {
        for (kk = 0; kk < n; kk++) {
          a_buf[kk] = _a_data[_a_step1 * kk];
        }

        _a_data = a_buf;
      }

      for (jj = 0; jj <= m - 4; jj += 4) {
        worktype s0, s1, s2, s3;
        const arrtype* b = _b_data + jj;

        if (do_acc) {
          s0 = _d_data[jj];
          s1 = _d_data[jj + 1];
          s2 = _d_data[jj + 2];
          s3 = _d_data[jj + 3];
        }
        else {
          s0 = s1 = s2 = s3 = (worktype)(0);
        }

        for (kk = 0; kk < n; kk++, b += _b_step) {
          worktype a = (_a_data[kk]);
          s0 += a * b[0];
          s1 += a * b[1];
          s2 += a * b[2];
          s3 += a * b[3];
        }

        _d_data[jj] = s0;
        _d_data[jj + 1] = s1;
        _d_data[jj + 2] = s2;
        _d_data[jj + 3] = s3;
      }

      for (; jj < m; jj++) {
        const arrtype* b = _b_data + jj;
        worktype s0 = do_acc ? _d_data[jj] : (worktype)(0);

        for (kk = 0; kk < n; kk++, b += _b_step) {
          s0 += (worktype)(_a_data[kk]) * b[0];
        }

        _d_data[jj] = s0;
      }
    }
  }
  free(a_buf);
}
