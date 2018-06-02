//蛇形矩阵填数
// 打印出如下格式的数据：
//
// 解题思路：
// 如果我们将矩阵变一下形状，如下：
// 【原创】蛇形填数 - lvan - lvan的世界
// 这样就变成输出一个三角阵了，简单多了。只是呢，需要在某些行上做一些逆序变换。
// 奇数 时变换奇数行； 偶数时变换偶数行。
// 最后按照斜线将数填入原来矩阵中，OK！
// C程序如下：
int test_snak_matrix()
{
  int i, j;
  int n = 15;
  //输入输出矩阵的大小，不超过100，当然自己可以修改上限值。
  int a[100][100] = {0};
  int b[200][100] = {0};
  int num = 0;
  //构造三角阵数形
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j <= i; j++) {
      num++;
      b[i][j] = num;
    }
  }
  for (i = n - 1; i < 2 * n - 1; i++) {
    for (j = 0; j < 2 * n - 1 - i; j++) {
      num++;
      b[i][j] = num;
    }
  }
  //某些行数据逆序
  for (i = 0; i < n; i += 2) {
    int temp = 0;
    for (j = 0; j <= i / 2; j++) {
      temp = b[i][j];
      b[i][j] = b[i][i - j];
      b[i][i - j] = temp;
    }
  }
  for (i = (n + 1) / 2 * 2; i < 2 * n - 1; i += 2) {
    int temp = 0;
    for (j = 0; j < (2 * n - 1 - i) / 2; j++) {
      temp = b[i][j];
      b[i][j] = b[i][2 * n - 2 - i - j];
      b[i][2 * n - 2 - i - j] = temp;
    }
  }
  //按照斜线将数填回原矩阵
  for (i = 0; i < n; i++) {
    for (j = 0; j <= i; j++) {
      a[i - j][j] = b[i][j];
    }
  }
  for (i = n; i < 2 * n - 1; i++) {
    for (j = i - n + 1; j < n; j++) {
      a[i - j][j] = b[i][j - i + n - 1];
    }
  }
  //输出矩阵
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      printf("%3d,", a[i][j]);
    }
    printf("\n");
  }
  return 0;
}

