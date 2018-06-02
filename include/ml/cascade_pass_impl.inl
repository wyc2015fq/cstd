{
  int iw = 0;
  int i = 0, j = 0;
  double score = 0;
  const weaktype* weak = (const weaktype*)ca->weak;

  //assert(NULL!=ca);
  if (ca->is_nesting) {
    if (ca->stagelen > 0) {
      for (j = 0; j < ca->stage[0].len; ++iw) {
        score += weak_macro(iw);
      }

      if (score >= ca->stage[i].thd) {
        for (i = 1; i < ca->stagelen; ++i) {
          score = weak_macro2(score, iw);
          ++iw;

          for (j = 1; j < ca->stage[i].len; ++j, ++iw) {
            score += weak_macro(iw);
          }

          if (score < ca->stage[i].thd) {
            break;
          }
        }
      }
    }
  }
  else {
    for (i = 0; i < ca->stagelen; ++i) {
      score = 0;

      for (j = 0; j < ca->stage[i].len; ++j, ++iw) {
        score += weak_macro(iw);
      }

      if (score < ca->stage[i].thd) {
        break;
      }
    }
  }

  *pispass = i == ca->stagelen;
  *pscore = score;
}
