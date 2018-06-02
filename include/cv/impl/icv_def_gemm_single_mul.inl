{
  int ii, jj, kk, n = _a_size.w, m = _d_size.w, drows = _d_size.h;
  const arrtype* __a_data = _a_data, *__b_data = _b_data, *__c_data = _c_data;
  arrtype* a_buf = 0;
  size_t _a_step0, _a_step1, _c_step0, _c_step1, _t_step;

  _a_step0 = _a_step;
  _a_step1 = 1;

  if (!_c_data) {
    _c_step0 = _c_step1 = 0;
  }
  else if (!(flags & CC_GEMM_C_T)) {
    _c_step0 = _c_step, _c_step1 = 1;
  }
  else
  {
    _c_step0 = 1, _c_step1 = _c_step;
  }

  if (flags & CC_GEMM_A_T) {
    CC_SWAP(_a_step0, _a_step1, _t_step);
    n = _a_size.h;

    if (_a_step > 1 && n > 1) {
      a_buf = (arrtype*)malloc(n * sizeof(_a_data[0]));
    }
  }

  if (n == 1) { /* external product */
    arrtype* b_buf = 0;

    if (_a_step > 1) {
      a_buf = (arrtype*)malloc(drows * sizeof(_a_data[0]));

      for (kk = 0; kk < drows; kk++) {
        a_buf[kk] = _a_data[_a_step * kk];
      }

      _a_data = a_buf;
    }

    if (_b_step > 1) {
      b_buf = (arrtype*)malloc(_d_size.w * sizeof(b_buf[0]));

      for (jj = 0; jj < _d_size.w; jj++) {
        b_buf[jj] = _b_data[jj * _b_step];
      }

      _b_data = b_buf;
    }

    for (ii = 0; ii < drows; ii++, __c_data += _c_step0, _d_data += _d_step) {
      worktype al = (worktype)(_a_data[ii]) * alpha;
      _c_data = __c_data;

      for (jj = 0; jj <= _d_size.w - 2; jj += 2, _c_data += 2 * _c_step1) {
        worktype s0 = al * _b_data[jj];
        worktype s1 = al * _b_data[jj + 1];

        if (!_c_data) {
          _d_data[jj] = (arrtype)(s0);
          _d_data[jj + 1] = (arrtype)(s1);
        }
        else {
          _d_data[jj] = (arrtype)(s0 + _c_data[0] * beta);
          _d_data[jj + 1] = (arrtype)(s1 + _c_data[_c_step1] * beta);
        }
      }

      for (; jj < _d_size.w; jj++, _c_data += _c_step1) {
        worktype s0 = al * _b_data[jj];

        if (!_c_data) {
          _d_data[jj] = (arrtype)(s0);
        }
        else {
          _d_data[jj] = (arrtype)(s0 + _c_data[0] * beta);
        }
      }
    }
    if (b_buf) {
      free(b_buf);
  }
  }
  else if (flags & CC_GEMM_B_T) { /* A * Bt */
    for (ii = 0; ii < drows; ii++, __a_data += _a_step0, __c_data += _c_step0, _d_data += _d_step) {
      _a_data = __a_data;
      _b_data = __b_data;
      _c_data = __c_data;

      if (a_buf) {
        for (kk = 0; kk < n; kk++) {
          a_buf[kk] = _a_data[_a_step1 * kk];
        }

        _a_data = a_buf;
      }

      for (jj = 0; jj < _d_size.w; jj++, _b_data += _b_step, _c_data += _c_step1) {
        worktype s0 = (0), s1 = (0), s2 = (0), s3 = (0);

        for (kk = 0; kk <= n - 4; kk += 4) {
          s0 += (worktype)(_a_data[kk]) * _b_data[kk];
          s1 += (worktype)(_a_data[kk + 1]) * _b_data[kk + 1];
          s2 += (worktype)(_a_data[kk + 2]) * _b_data[kk + 2];
          s3 += (worktype)(_a_data[kk + 3]) * _b_data[kk + 3];
        }

        for (; kk < n; kk++) {
          s0 += (worktype)(_a_data[kk]) * _b_data[kk];
        }

        s0 = (s0 + s1 + s2 + s3) * alpha;

        if (!_c_data) {
          _d_data[jj] = (arrtype)(s0);
        }
        else {
          _d_data[jj] = (arrtype)(s0 + _c_data[0] * beta);
        }
      }
    }
  }
  else if (_d_size.w * sizeof(_d_data[0]) <= 1600) {
    for (ii = 0; ii < drows; ii++, __a_data += _a_step0, __c_data += _c_step0, _d_data += _d_step) {
      _a_data = __a_data, _c_data = __c_data;

      if (a_buf) {
        for (kk = 0; kk < n; kk++) {
          a_buf[kk] = _a_data[_a_step1 * kk];
        }

        _a_data = a_buf;
      }

      for (jj = 0; jj <= m - 4; jj += 4, _c_data += 4 * _c_step1) {
        const arrtype* b = __b_data + jj;
        worktype a, s0 = (0), s1 = (0), s2 = (0), s3 = (0);

        for (kk = 0; kk < n; kk++, b += _b_step) {
          a = (_a_data[kk]);
          s0 += a * b[0];
          s1 += a * b[1];
          s2 += a * b[2];
          s3 += a * b[3];
        }

        if (!_c_data) {
          _d_data[jj] = (arrtype)(s0 * alpha);
          _d_data[jj + 1] = (arrtype)(s1 * alpha);
          _d_data[jj + 2] = (arrtype)(s2 * alpha);
          _d_data[jj + 3] = (arrtype)(s3 * alpha);
        }
        else {
          s0 = s0 * alpha;
          s1 = s1 * alpha;
          s2 = s2 * alpha;
          s3 = s3 * alpha;
          _d_data[jj] = (arrtype)(s0 + _c_data[0] * beta);
          _d_data[jj + 1] = (arrtype)(s1 + _c_data[_c_step1] * beta);
          _d_data[jj + 2] = (arrtype)(s2 + _c_data[_c_step1 * 2] * beta);
          _d_data[jj + 3] = (arrtype)(s3 + _c_data[_c_step1 * 3] * beta);
        }
      }

      for (; jj < m; jj++, _c_data += _c_step1) {
        const arrtype* b = __b_data + jj;
        worktype s0 = (0);

        for (kk = 0; kk < n; kk++, b += _b_step) {
          s0 += (worktype)(_a_data[kk]) * b[0];
        }

        s0 = s0 * alpha;

        if (!_c_data) {
          _d_data[jj] = (arrtype)(s0);
        }
        else {
          _d_data[jj] = (arrtype)(s0 + _c_data[0] * beta);
        }
      }
    }
  }
  else
  {
    worktype* d_buf = (worktype*)malloc(m * sizeof(d_buf[0]));

    for (ii = 0; ii < drows; ii++, __a_data += _a_step0, __c_data += _c_step0, _d_data += _d_step) {
      _a_data = __a_data;
      _b_data = __b_data;
      _c_data = __c_data;

      if (a_buf) {
        for (kk = 0; kk < n; kk++) {
          a_buf[kk] = __a_data[_a_step1 * kk];
        }

        _a_data = a_buf;
      }

      for (jj = 0; jj < m; jj++) {
        d_buf[jj] = (worktype)(0);
      }

      for (kk = 0; kk < n; kk++, _b_data += _b_step) {
        worktype t0, t1, al = (_a_data[kk]);

        for (jj = 0; jj <= m - 4; jj += 4) {
          t0 = d_buf[jj] + _b_data[jj] * al;
          t1 = d_buf[jj + 1] + _b_data[jj + 1] * al;
          d_buf[jj] = t0;
          d_buf[jj + 1] = t1;
          t0 = d_buf[jj + 2] + _b_data[jj + 2] * al;
          t1 = d_buf[jj + 3] + _b_data[jj + 3] * al;
          d_buf[jj + 2] = t0;
          d_buf[jj + 3] = t1;
        }

        for (; jj < m; jj++) {
          d_buf[jj] += _b_data[jj] * al;
        }
      }

      if (!_c_data) {
        for (jj = 0; jj < m; jj++) {
          _d_data[jj] = (arrtype)(d_buf[jj] * alpha);
        }
      }
      else {
        for (jj = 0; jj < m; jj++, _c_data += _c_step1) {
          worktype t = d_buf[jj] * alpha;
          _d_data[jj] = (arrtype)(t + _c_data[0] * beta);
        }
      }
    }
    if (d_buf) {
      free(d_buf);
    }
  }
  if (a_buf) {
    free(a_buf);
  }
}

