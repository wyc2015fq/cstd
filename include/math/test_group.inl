// 求置换群（permutation group）的一个排列的 k-cycle 分解
// 把一个排列（置换群的一个元素） (4 1 5 2 3) 分解成 (1 4 2)(3 5)，
int k_cycle_decomp(const int* a, int n)
{
  int i, mark[10] = {0}; // mark[0] 未用
  for (i = 0; i < n; ++i)  {
    if (!mark[i]) {
      int j = a[i];
      do {
        mark[j - 1] = 1;
        printf("%d ", j);
        j = a[j - 1];
      }
      while (j != a[i]);
      printf("\n");
    }
  }
  return 0;
}
int test_group()
{
  int aa[] = {4, 1, 5, 2, 3};
  k_cycle_decomp(aa, countof(aa));
  return 0;
}

