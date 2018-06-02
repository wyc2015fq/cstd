{
  int i, j;
  weak_type* weak = (weak_type*)ca->weak;
  double v = 0;

  for (i = 0; i < ca->stage_len; ++i) {
    score = 0;

    for (j = 0; j < ca->stage[i].len; ++j, ++weak) {
      weak_macro(weak, v);
      score += v;
    }

    if (score < ca->stage[i].thd) {
      ispass = 0;
      break;
    }
  }
}
#undef weak_macro
