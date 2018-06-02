
{
  const arrtype* __c_data = _c_data;
  int jj;
  size_t _c_step0, _c_step1;

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

  for (; _d_size.h--; __c_data += _c_step0, d_buf += d_buf_step, _d_data += _d_step) {
    if (__c_data) {
      _c_data = __c_data;

      for (jj = 0; jj <= _d_size.w - 4; jj += 4, _c_data += 4 * _c_step1) {
        worktype t0 = alpha * d_buf[jj];
        worktype t1 = alpha * d_buf[jj + 1];
        t0 += beta * (worktype)(_c_data[0]);
        t1 += beta * (worktype)(_c_data[_c_step1]);
        _d_data[jj] = (arrtype)(t0);
        _d_data[jj + 1] = (arrtype)(t1);
        t0 = alpha * d_buf[jj + 2];
        t1 = alpha * d_buf[jj + 3];
        t0 += beta * (worktype)(_c_data[_c_step1 * 2]);
        t1 += beta * (worktype)(_c_data[_c_step1 * 3]);
        _d_data[jj + 2] = (arrtype)(t0);
        _d_data[jj + 3] = (arrtype)(t1);
      }

      for (; jj < _d_size.w; jj++, _c_data += _c_step1) {
        worktype t0 = alpha * d_buf[jj];
        _d_data[jj] = (arrtype)(t0 + beta * _c_data[0]);
      }
    }
    else {
      for (jj = 0; jj <= _d_size.w - 4; jj += 4) {
        worktype t0 = alpha * d_buf[jj];
        worktype t1 = alpha * d_buf[jj + 1];
        _d_data[jj] = (arrtype)(t0);
        _d_data[jj + 1] = (arrtype)(t1);
        t0 = alpha * d_buf[jj + 2];
        t1 = alpha * d_buf[jj + 3];
        _d_data[jj + 2] = (arrtype)(t0);
        _d_data[jj + 3] = (arrtype)(t1);
      }

      for (; jj < _d_size.w; jj++) {
        _d_data[jj] = (arrtype)(alpha * d_buf[jj]);
      }
    }
  }

  return CC_OK;
}
