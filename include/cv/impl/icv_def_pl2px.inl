{
  //#define  icv_def_px2pl_c2(arrtype, len)
  int j, len;
  arrtype* dst = (arrtype*)dst0;
  len = size.w;

  for (i = 0; i < 4 && 0 == (mask & 1); ++i) {
    mask >>= 1;
    dst++;
  }

  dststep -= size.w * cn;

  switch (mask) {
  case 1: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];

    for (; size.h--; dst += dststep) {
      for (j = 0; j <= (len) - 4; j += 4, (dst) += 4 * (cn)) {
        arrtype t0 = (plane0)[j];
        arrtype t1 = (plane0)[j + 1];

        (dst)[0] = t0;
        (dst)[(cn)] = t1;

        t0 = (plane0)[j + 2];
        t1 = (plane0)[j + 3];

        (dst)[(cn) * 2] = t0;
        (dst)[(cn) * 3] = t1;
      }

      for (; j < (len); j++, (dst) += (cn)) {
        (dst)[0] = (plane0)[j];
      }
    }
  }
  break;

  case 3: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 2) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];

        dst[0] = t0;
        dst[1] = t1;
      }

      plane0 += srcstep;
      plane1 += srcstep;
    }
  }
  break;

  case 5: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 2) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];

        dst[0] = t0;
        dst[1] = t1;
      }

      plane0 += srcstep;
      plane1 += srcstep;
    }
  }
  break;

  case 9: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 2) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];

        dst[0] = t0;
        dst[1] = t1;
      }

      plane0 += srcstep;
      plane1 += srcstep;
    }
  }
  break;

  case 7: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];
    const arrtype* plane2 = (const arrtype*)srcptr[2];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 3) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];
        arrtype t2 = plane2[j];

        dst[0] = t0;
        dst[1] = t1;
        dst[2] = t2;
      }

      plane0 += srcstep;
      plane1 += srcstep;
      plane2 += srcstep;
    }
  }
  break;

  case 11: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];
    const arrtype* plane2 = (const arrtype*)srcptr[2];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 3) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];
        arrtype t2 = plane2[j];

        dst[0] = t0;
        dst[1] = t1;
        dst[2] = t2;
      }

      plane0 += srcstep;
      plane1 += srcstep;
      plane2 += srcstep;
    }
  }
  break;

  case 13: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];
    const arrtype* plane2 = (const arrtype*)srcptr[2];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 3) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];
        arrtype t2 = plane2[j];

        dst[0] = t0;
        dst[1] = t1;
        dst[2] = t2;
      }

      plane0 += srcstep;
      plane1 += srcstep;
      plane2 += srcstep;
    }
  }
  break;

  case 4: {
    const arrtype* plane0 = (const arrtype*)srcptr[0];
    const arrtype* plane1 = (const arrtype*)srcptr[1];
    const arrtype* plane2 = (const arrtype*)srcptr[2];
    const arrtype* plane3 = (const arrtype*)srcptr[3];

    for (; size.h--; dst += dststep) {
      for (j = 0; j < (len); j++, (dst) += 4) {
        arrtype t0 = plane0[j];
        arrtype t1 = plane1[j];

        dst[0] = t0;
        dst[1] = t1;

        t0 = plane2[j];
        t1 = plane3[j];

        dst[2] = t0;
        dst[3] = t1;
      }

      plane0 += srcstep;
      plane1 += srcstep;
      plane2 += srcstep;
      plane3 += srcstep;
    }
  }
  break;

  default:
    ASSERT(0);
    break;
  }
}