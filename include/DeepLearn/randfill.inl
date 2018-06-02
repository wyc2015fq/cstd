
// 随机排列
//http://blog.csdn.net/devfun/article/details/6534476
int randperm(int N, int* arr)
{
  int i, t;

  for (i = 0; i < N; ++i) {
    arr[i] = i;
  }

  for (i = 0; i < N; ++i) {
    int j = mtrand32() % N; //从剩下的随机数里生成
    CV_SWAP(arr[i], arr[j], t);
  }

  return 0;
}

int randfill(int n, double* x, double a, double b)
{
  int i;

  for (i = 0; i < n; ++i) {
    double t = mtreal3();
    x[i] = t * a + b;
  }

  return 0;
}

int dmuladd(int n, const double* x, double* y, double a, double b)
{
  int i = 0;

  for (; i < n - 3; i += 4) {
    y[i] = x[i] * a + b;
    y[i + 1] = x[i + 1] * a + b;
    y[i + 2] = x[i + 2] * a + b;
    y[i + 3] = x[i + 3] * a + b;
  }

  for (; i < n; ++i) {
    y[i] = x[i] * a + b;
  }

  return 0;
}
int imdouble(const img_t* im, int type, img_t* im1, double a, double b)
{
  img_t im2[1] = {0};
  int n = im->h * im->w;

  if (im == im1) {
    imclone2(im, im2);
    im = im2;
  }

  imsetsize(im1, im->h, im->w, sizeof(double), im->f);
  castcpy(im1->tt.data, im->tt.data, n, CV_8F, type);
  dmuladd(n, (double*)im1->tt.data, (double*)im1->tt.data, a, b);
  imfree(im2);
  return 0;
}
