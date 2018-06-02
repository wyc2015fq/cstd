
int draw_chain(HDDC hDC, int n, const char* c4, double x, double y, double len, double wline, COLORREF clr)
{
  int i;

  if (0 == n) {
    drawaa_ellipse(hDC, x, y, wline, wline, clr, 0);
  }

  for (i = 0; i < n; ++i) {
    POINT pp = code_deltas4[c4[i]];
    drawaa_strokeline(hDC, x, y, x + pp.x * len, y + pp.y * len, clr, wline);
    x += pp.x * len, y += pp.y * len;
  }

  return 0;
}
int draw_chains(HDDC hDC, const chains_t* chs, double x, double y, double len, double wline, COLORREF clr)
{
  int i;

  for (i = 0; i < chs->nch; ++i) {
    draw_chain(hDC, chs->ch[i].len, chs->ch[i].code, chs->ch[i].pt.x * len + x, chs->ch[i].pt.y * len + y, len, wline, clr);
  }

  return 0;
}

int test_freeman(HDDC hDC)
{
#if 0
  uchar im[] = {
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
  };
  int h = 3, w = 3;
#else
  uchar im[] = {
    1, 0, 1, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 1, 1,
    1, 0, 1, 0, 1,
    1, 1, 1, 1, 1,
  };
  int h = 6, w = 5;
#endif
  int t;
  chains_t ch1[1] = {0};
  chains_t ch2[1] = {0};
  chains_t ch3[1] = {0};
  chains_t ch4[1] = {0};
  freeman(h, w, im, w, ch1, 4);
  t = chain4_area2(ch1->ch->len, ch1->ch->code);
  draw_chains(hDC, ch1, 100, 100, 40, 4, _RGBA(255, 0, 0, 123));
  t = chains4_ext(ch1, ch2);
  t = chains4_ext(ch2, ch3);
  t = chains4_ext(ch3, ch4);
  //t = chain4_area(ch2->len, ch2->code);
  draw_chains(hDC, ch2, 120, 120, 40, 2, _RGBA(0, 0, 255, 123));
  draw_chains(hDC, ch4, 140, 100, 40, 2, _RGBA(0, 255, 0, 123));
  free_chains(ch1);
  free_chains(ch2);
  free_chains(ch3);
  free_chains(ch4);
  return 0;
}

