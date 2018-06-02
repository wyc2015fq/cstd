{
  char* i0 = ptr, *i1, *i3, *i4;
  int prev_s = -1, s, s_end;

  assert(1 < nbd && nbd < 128);

  s_end = s = is_hole ? 0 : 4;

  do
  {
    s = (s - 1) & 7;
    i1 = i0 + deltas[s];

    if (*i1 != 0) {
      break;
    }
  }
  while (s != s_end);

  if (s == s_end) {           /* single pixel domain */
    *i0 = (char)(nbd | 0x80);
    if(s != prev_s)
    {
      iVC_add(vc, pt.x, pt.y);
    }
  }
  else
  {
    i3 = i0;
    prev_s = s ^ 4;

    /* follow border */
    for (;;) {
      s_end = s;

      for (;;) {
        i4 = i3 + deltas[++s];

        if (*i4 != 0) {
          break;
        }
      }

      s &= 7;

      // check "right" bound
      if ((unsigned)(s - 1) < (unsigned) s_end) {
        *i3 = (char)(nbd | 0x80);
      }
      else if (*i3 == 1) {
        *i3 = (char) nbd;
      }

      if(s != prev_s)
      {
        iVC_add(vc, pt.x, pt.y);
      }
      pt.x += icvCodeDeltas[s].x, pt.y += icvCodeDeltas[s].y;

      prev_s = s;

      if (i4 == i0 && i3 == i1) {
        break;
      }

      i3 = i4;
      s = (s + 4) & 7;
    }                       /* end of border following loop */
  }
}
