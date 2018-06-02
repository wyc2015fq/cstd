
if (1 == cn)
{
  int x, y;
  sumtype s;
  sqsumtype sq;
  sumtype* buf = 0;

  memset(sum, 0, (size.w + 1)*sizeof(sum[0]));
  sum += sumstep + 1;

  if (sqsum) {
    memset(sqsum, 0, (size.w + 1)*sizeof(sqsum[0]));
    sqsum += sqsumstep + 1;
  }

  if (tilted) {
    memset(tilted, 0, (size.w + 1)*sizeof(tilted[0]));
    tilted += tiltedstep + 1;
  }

  if (sqsum == 0 && tilted == 0) {
    for (y = 0; y < size.h; y++, src += srcstep, sum += sumstep) {
      sum[-1] = 0;

      for (x = 0, s = 0; x < size.w; x++) {
        sumtype t = cast_macro(src[x]);
        s += t;
        sum[x] = sum[x - sumstep] + s;
      }
    }
  }
  else if (tilted == 0) {
    for (y = 0; y < size.h; y++, src += srcstep, sum += sumstep, sqsum += sqsumstep) {
      sum[-1] = 0;
      sqsum[-1] = 0;

      for (x = 0, s = 0, sq = 0; x < size.w; x++) {
        worktype it = src[x];
        sumtype t = cast_macro(it);
        sqsumtype tq = cast_sqr_macro(it);
        s += t;
        sq += tq;
        t = sum[x - sumstep] + s;
        tq = sqsum[x - sqsumstep] + sq;
        sum[x] = t;
        sqsum[x] = tq;
      }
    }
  }
  else {
    if (sqsum == 0) {
      assert(0);
      return CC_NULLPTR_ERR;
    }

    buf = (sumtype*)malloc((size.w + 1) * sizeof(buf[0]));
    sum[-1] = tilted[-1] = 0;
    sqsum[-1] = 0;

    for (x = 0, s = 0, sq = 0; x < size.w; x++) {
      worktype it = src[x];
      sumtype t = cast_macro(it);
      sqsumtype tq = cast_sqr_macro(it);
      buf[x] = tilted[x] = t;
      s += t;
      sq += tq;
      sum[x] = s;
      sqsum[x] = sq;
    }

    if (size.w == 1) {
      buf[1] = 0;
    }

    for (y = 1; y < size.h; y++) {
      worktype it;
      sumtype t0;
      sqsumtype tq0;

      src += srcstep;
      sum += sumstep;
      sqsum += sqsumstep;
      tilted += tiltedstep;

      it = src[0/*x*/];
      s = t0 = cast_macro(it);
      sq = tq0 = cast_sqr_macro(it);

      sum[-1] = 0;
      sqsum[-1] = 0;
      /*tilted[-1] = buf[0];*/
      tilted[-1] = tilted[-tiltedstep];

      sum[0] = sum[-sumstep] + t0;
      sqsum[0] = sqsum[-sqsumstep] + tq0;
      tilted[0] = tilted[-tiltedstep] + t0 + buf[1];

      for (x = 1; x < size.w - 1; x++) {
        sumtype t1 = buf[x];
        buf[x - 1] = t1 + t0;
        it = src[x];
        t0 = cast_macro(it);
        tq0 = cast_sqr_macro(it);
        s += t0;
        sq += tq0;
        sum[x] = sum[x - sumstep] + s;
        sqsum[x] = sqsum[x - sqsumstep] + sq;
        t1 += buf[x + 1] + t0 + tilted[x - tiltedstep - 1];
        tilted[x] = t1;
      }

      if (size.w > 1) {
        sumtype t1 = buf[x];
        buf[x - 1] = t1 + t0;
        it = src[x];    /*+*/
        t0 = cast_macro(it);
        tq0 = cast_sqr_macro(it);
        s += t0;
        sq += tq0;
        sum[x] = sum[x - sumstep] + s;
        sqsum[x] = sqsum[x - sqsumstep] + sq;
        tilted[x] = t0 + t1 + tilted[x - tiltedstep - 1];
        buf[x] = t0;
      }
    }
  }
  if (buf) {
    free(buf);
    buf = 0;
  }
}
else
{
  int x, y;

  memset(sum, 0, (size.w + 1)*cn * sizeof(sum[0]));
  sum += sumstep + cn;

  if (sqsum) {
    memset(sqsum, 0, (size.w + 1)*cn * sizeof(sqsum[0]));
    sqsum += sqsumstep + cn;
  }

  size.w *= cn;

  if (sqsum == 0) {
    for (y = 0; y < size.h; y++, src += srcstep, sum += sumstep) {
      for (x = -cn; x < 0; x++) {
        sum[x] = 0;
      }

      for (x = 0; x < size.w; x++) {
        sum[x] = cast_macro(src[x]) + sum[x - cn];
      }

      for (x = 0; x < size.w; x++) {
        sum[x] = sum[x] + sum[x - sumstep];
      }
    }
  }
  else {
    for (y = 0; y < size.h; y++, src += srcstep,
        sum += sumstep, sqsum += sqsumstep) {
      for (x = -cn; x < 0; x++) {
        sum[x] = 0;
        sqsum[x] = 0;
      }

      for (x = 0; x < size.w; x++) {
        worktype it = src[x];
        sumtype t = cast_macro(it) + sum[x - cn];
        sqsumtype tq = cast_sqr_macro(it) + sqsum[x - cn];
        sum[x] = t;
        sqsum[x] = tq;
      }

      for (x = 0; x < size.w; x++) {
        sumtype t = sum[x] + sum[x - sumstep];
        sqsumtype tq = sqsum[x] + sqsum[x - sqsumstep];
        sum[x] = t;
        sqsum[x] = tq;
      }
    }
  }
}


#undef cast_macro
#undef cast_sqr_macro
