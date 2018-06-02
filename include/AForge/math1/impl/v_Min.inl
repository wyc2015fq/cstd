
if (n>0) {
  int i = 1;
  imin = 0;
  for (; i < n; i++)
  {
    if (a[i] < a[imin])
    {
      imin = i;
    }
  }
}