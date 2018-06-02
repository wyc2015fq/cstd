
int test_solve()
{
  double aa[] = {
    12, 13,
    14, 15,
  };
  double bb[] = {21, 23};
  double xx[] = {21, 23};
  double cc[] = {21, 23};
  SOLVE2(aa, bb, xx);
  M2MULV2(cc, aa, xx);
  ASSERT(cc[0] == bb[0]);
  return 0;
}