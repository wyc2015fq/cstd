{
  int i, j;
  uchar r, g, b, a = 0;

  for (i = 0; i < h; ++i) {
    const a_type* A = (a_type*)(A0 + i * al);
    b_type* B = (b_type*)(B0 + i * bl);

    for (j = 0; j < w; ++j, A += ai, B += bi) {
      GET_RGB();
      PUT_RGB();
    }
  }
}

#undef PUT_RGB
