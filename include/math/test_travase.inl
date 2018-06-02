int gcd(int a , int b)
{
  int t = 0;
  while (b) {
    t = b, b = a % b, a = t;
  }
  return t;
}
// 辗转相除法的递归算法
int gcd2(int a, int b)
{
  return b == 0 ? a : gcd(b, a % b);
}
int travase(int* a, int x)    //数组起始地址，数组长度，数组 循环右移多少位
{
  int i = 0;
  int j = x - 1, t;
  while (i < j) {
    SWAP(a[i], a[j], t), i++, j--;
  }
  return 0;
}
int movedata(int* a, int x)
{
  if (x == 1) {
    return 0;
  }
  travase(a, x - 1);
  travase(a, x);
  return 0;
}
// 矩阵原地转置
int transposeInpalce(int m, int n, int* arr, int step)
{
  int from , to, offset;
  int i, j;
  to = offset = 0;
  for (i = 0; i < n; i++) {
    for (j = 0 , from = i + offset; j < m ; j++) {
      movedata(arr + to, from - to + 1);
      from += step;
      to++;
    }
    step--;
    offset += m - 1;
  }
  return 0;
}
int print(int m, int n, const int* arr, int step)
{
  int i = 0, j = 0;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      printf("%d, ", arr[i * step + j]);
    }
  }
  printf("\n");
  return 0;
}
int transposeInpalce21(int m, int n, int* arr, int step)
{
  int t, m1, n1, i, j, k = 0;
  t = gcd(m, n);
  m1 = m / t, n1 = n / t;
  for (i = 0; i < m1 * n1; ++i) {
    j = (i % n1) * m1 + (i / n1);
    if (i == j) {
      ++k;
    }
  }
  return k;
}
int transposeInpalce22(int m, int n, int* arr, int step)
{
  int t, m1, n1, i, i0, j, k = 0;
  t = gcd(m, n);
  m1 = m / t, n1 = n / t;
  if (transposeInpalce21(m, n, arr, step) != 3) {
    return 0;
  }
  for (i0 = i = 1; 1;) {
    j = (i % n1) * m1 + (i / n1);
    ++k;
    if (j == i0) {
      break;
    }
    i = j;
  }
  return k;
}
int transposeInpalce2(int m, int n, int* arr, int step)
{
  int t, m1, n1, i, j, k = 0;
  t = gcd(m, n);
  m1 = m / t, n1 = n / t;
  if (transposeInpalce21(m, n, arr, step) != 3) {
    return 0;
  }
  k = transposeInpalce22(m, n, arr, step);
  printf("===%3d %3d %3d %3d\n", m1, n1, (m1 * n1), k);
  return k;
}
int test_travase()
{
  int i, j, m, n, step;
  int* A;
  int* B;
  A = malloc(2000000);
  B = malloc(2000000);
  for (i = 0; i < 100; ++i) {
    m = 3 + rand() % 10;
    n = 3 + rand() % 10;
    step = n;
    FILL(m * n, A, rand());
    COPY(m * n, B, A);
    {
      //utime_start(_start_time);
      //transposeInpalce(m, n, A, step);
      transposeInpalce2(m, n, A, step);
      //printf("%f ", utime_elapsed(_start_time));
    }
    for (j = 0; j < m * n; ++j) {
      ASSERT(A[j] == B[j]);
    }
    //printf("\n");
  }
  free(A);
  free(B);
  return 0;
}

