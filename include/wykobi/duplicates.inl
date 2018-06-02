
int remove_duplicates(int n, double* begin)
{
  int i, j = 1;
  T previous;

#define GS_POINT_COMPARATOR(a, b) (a < b)
  QSORT(begin, begin + n, GS_POINT_COMPARATOR, double);
#undef GS_POINT_COMPARATOR

  previous = begin[0];

  for (i = 1; i < n; ++i) {
    if (begin[i] > previous) {
      begin[j++] = previous = begin[i];
    }
  }

  return n;
}

