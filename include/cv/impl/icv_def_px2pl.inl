{
  //#define  icv_def_px2pl_c2(arrtype, len)
  int j, len;
  const arrtype* src = (const arrtype*)src0;
  len = size.w;
  srcstep /= sizeof(arrtype);
  dststep /= sizeof(arrtype);

  for (i = 0; i < 4 && 0 == (mask & 1); ++i) {
    mask >>= 1;
    src++;
  }

  srcstep -= size.w * (cn);

  switch (mask) {
  case 1: {
    arrtype* plane0 = (arrtype*)dst[0];

    for (; size.h--; src += srcstep) {
      for (j = 0; j <= (len) - 4; j += 4, (src) += 4 * (cn)) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[(cn)];

        (plane0)[j] = t0;
        (plane0)[j + 1] = t1;

        t0 = (src)[(cn) * 2];
        t1 = (src)[(cn) * 3];

        (plane0)[j + 2] = t0;
        (plane0)[j + 3] = t1;
      }

      for (; j < (len); j++, (src) += (cn)) {
        (plane0)[j] = (src)[0];
      }


    }
  }
  break;

  case 3: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 2) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[1];

        plane0[j] = t0;
        plane1[j] = t1;
      }

      plane0 += dststep;
      plane1 += dststep;

    }
  }
  break;

  case 5: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 2) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[2];

        plane0[j] = t0;
        plane1[j] = t1;
      }

      plane0 += dststep;
      plane1 += dststep;

    }
  }
  break;

  case 9: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 2) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[3];

        plane0[j] = t0;
        plane1[j] = t1;
      }

      plane0 += dststep;
      plane1 += dststep;

    }
  }
  break;

  case 7: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];
    arrtype* plane2 = (arrtype*)dst[2];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 3) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[1];
        arrtype t2 = (src)[2];

        plane0[j] = t0;
        plane1[j] = t1;
        plane2[j] = t2;
      }

      plane0 += dststep;
      plane1 += dststep;
      plane2 += dststep;

    }
  }
  break;

  case 11: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];
    arrtype* plane2 = (arrtype*)dst[2];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 3) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[1];
        arrtype t2 = (src)[3];

        plane0[j] = t0;
        plane1[j] = t1;
        plane2[j] = t2;
      }

      plane0 += dststep;
      plane1 += dststep;
      plane2 += dststep;

    }
  }
  break;

  case 13: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];
    arrtype* plane2 = (arrtype*)dst[2];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 3) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[2];
        arrtype t2 = (src)[3];

        plane0[j] = t0;
        plane1[j] = t1;
        plane2[j] = t2;
      }

      plane0 += dststep;
      plane1 += dststep;
      plane2 += dststep;

    }
  }
  break;

  case 4: {
    arrtype* plane0 = (arrtype*)dst[0];
    arrtype* plane1 = (arrtype*)dst[1];
    arrtype* plane2 = (arrtype*)dst[2];
    arrtype* plane3 = (arrtype*)dst[3];

    for (; size.h--; src += srcstep) {
      for (j = 0; j < (len); j++, (src) += 4) {
        arrtype t0 = (src)[0];
        arrtype t1 = (src)[1];

        plane0[j] = t0;
        plane1[j] = t1;

        t0 = (src)[2];
        t1 = (src)[3];

        plane2[j] = t0;
        plane3[j] = t1;
      }

      plane0 += dststep;
      plane1 += dststep;
      plane2 += dststep;
      plane3 += dststep;

    }
  }
  break;

  default:
    ASSERT(0);
    break;
  }
}