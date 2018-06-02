//整数划分
//分类： Algorithms 2008-12-19 23:41 1212人阅读 评论(0) 收藏 举报
//iostream
//直接帖程序好像有点不厚道，偶的神哪，就让我不厚道这么一次吧：
//n划分的最大整数n1<=m的划分个数
int q(int n, int m)
{
  if (n < 1 || m < 1) {
    return 0;
  }
  if (n == 1 || m == 1) {
    return 1;
  }
  if (n < m) {
    return q(n, n);
  }
  if (n == m) {
    return q(n, n - 1) + 1;
  }
  return q(n, m - 1) + q(n - m, m);
}
//正奇数划分
int q2(int n, int m)
{
  if (n < 1 || m < 1) {
    return 0;
  }
  if (n == 1 || m == 1) {
    return 1;
  }
  if (n < m) {
    return q2(n, n);
  }
  if (n == m && n % 2 == 1) {
    return q2(n, n - 1) + 1;
  }
  if (n == m && n % 2 == 0) {
    return q2(n, n - 1);
  }
  //if(n>m && n%2==1)
  return q2(n, m - 2) + q2(n - m, m);
  //return q2(n,m-2)+q2(n-m,m);
}
//不同正整数划分(将n划分成不同正整数相加)
int q3(int n, int m)
{
  if (n == 1 && m == 1) {
    return 1;
  }
  if (n < 0 || m <= 1) {
    return 0;
  }
  if (n < m) {
    return q3(n, n);
  }
  if (n == m) {
    return q3(n, n - 1) + 1;
  }
  return q3(n, m - 1) + q3(n - m, m - 1);
}
int test_integer_division()
{
  int i, n, m;
  for (i = 0; i < 10; ++i) {
    n = rand() % 20;
    m = rand() % 20;
    n = i + 5;
    m = 2;
    printf("%4d %4d %4d %4d %4d %4d\n", n, m, q(n, n), q(n, m), q2(n, n), q3(n, n));
  }
  return 0;
}

