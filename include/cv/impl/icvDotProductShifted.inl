
{
  double result = 0;

  for (; size.h--; vec1 += vecstep1, vec2 += vecstep2, avg += avgstep) {
    int x;

    for (x = 0; x <= size.w - 4; x += 4) {
      result += (load_macro(vec1[x]) - avg[x]) * (load_macro(vec2[x]) - avg[x]) +
      (load_macro(vec1[x + 1]) - avg[x + 1]) * (load_macro(vec2[x + 1]) - avg[x + 1]) +
      (load_macro(vec1[x + 2]) - avg[x + 2]) * (load_macro(vec2[x + 2]) - avg[x + 2]) +
      (load_macro(vec1[x + 3]) - avg[x + 3]) * (load_macro(vec2[x + 3]) - avg[x + 3]);
    }

    for (; x < size.w; x++) {
      result += (load_macro(vec1[x]) - avg[x]) * (load_macro(vec2[x]) - avg[x]);
    }
  }

  *_result = result;
}

#undef load_macro
