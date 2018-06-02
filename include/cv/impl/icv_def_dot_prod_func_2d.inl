
{

  for (; size.h--; src1 += step1, src2 += step2) {
    int i;

    for (i = 0; i <= size.w - 4; i += 4) {
      temptype t0 = (temptype)src1[i] * src2[i];
      temptype t1 = (temptype)src1[i + 1] * src2[i + 1];
      t0 += (temptype)src1[i + 2] * src2[i + 2];
      t1 += (temptype)src1[i + 3] * src2[i + 3];
      sum += t0 + t1;
    }

    for (; i < size.w; i++) {
      sum += (temptype)src1[i] * src2[i];
    }
  }

}

