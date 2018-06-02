int g_dist2 = 2; //需要调整的参数

int meansure1(Iris_t* ir)
{
  unsigned char* T = ir->data;
  unsigned char* M = ir->mask;
  unsigned char* B = ir->code;
  unsigned char* newM = ir->newMask;
  int x1 = 30, x2 = 360;

  //
  int fn = 5;
  double alf = 1.7;
  int fn2;
  int i, j;
  int t1, n1, t2, n2, k, n3;
  //A_j1 = imfilter(A,H1);
  double H[200];
  fn2 = 10 + g_dist2;
  //H=zeros(fn,fn2);
  FILL(200, H, 0.);

  //H(:,1:5)=H1;
  //H(:,(fn2-4):fn2)=-H1;
  for (i = 0; i < 5; ++i) {
    for (j = 0; j < 5; ++j) {
      double h1 = H1[i * 5 + j];
      H[i * fn2 + j] = h1;
      H[i * fn2 + fn2 - 5 + j] = -h1;
    }
  }

  //D=ones(5,20);
  //把图像划分成5*20有交叠的子区域
  t1 = 2;
  n1 = 13;
  t2 = 4;
  n2 = 85;
  k = 0;
  n3 = n1 * n2;
  FILL(n3, B, 0);
  FILL(n3, newM, 0);

  for (i = 1; i <= n1; ++i) {
    for (j = 1; j <= n2; ++j) {
      int a1, a2, b1, b2, c1, a, b;
      double e1 = 0;
      int e2 = 0;
      a1 = 1 + (i - 1) * t1;
      a2 = a1 + 4;
      b1 = 1 + (j - 1) * t2;
      b2 = b1 + 19;
      c1 = ((20 - fn2) / 2);

      for (a = a1; a <= a2; ++a) {
        for (b = b1; b < b1 + fn2; ++b) {
          double t = T[(a - 1) * IRIS_W + b + c1 - 1];
          double h = H[(a - a1) * fn2 + b - b1];
          e1 += t * h;
        }

        for (b = b1; b <= b2; ++b) {
          int t = M[(a - 1) * IRIS_W + b - 1];

          if (t != 0) {
            ++e2;
          }
        }
      }

      //A3 = A2.*H;
      //e1=sum(sum(A3));
      if (e1 > 0) {
        B[k] = 1;
      }
      else {
        B[k] = 0;
      }

      if (e2 > 60) {
        newM[k] = 1;
      }
      else {
        newM[k] = 0;
      }

      k = k + 1;
    }
  }

  return 0;
}

//带距离的三核滤波器
int meansure2(Iris_t* ir)
{
  unsigned char* T = ir->data;
  unsigned char* M = ir->mask;
  unsigned char* B = ir->code;
  unsigned char* newM = ir->newMask;
  int x1 = 30, x2 = 360;

  //
  int fn = 5;
  double alf = 1.7;
  int fn2;
  int i, j;
  int t1, n1, t2, n2, k, n3;
  //A_j1 = imfilter(A,H1);
  double H[200];

  fn2 = 3 * fn + 2 * g_dist2;
  //H=zeros(fn,fn2);
  FILL(200, H, 0.);

  //H(:,1:5)=H1;
  //H(:,(fn2-4):fn2)=-H1;
  for (i = 0; i < 5; ++i) {
    for (j = 0; j < 5; ++j) {
      double h1 = H1[i * 5 + j];
      H[i * fn2 + j] = h1;
      H[i * fn2 + 5 + g_dist2 + j] = -2 * h1;
      H[i * fn2 + fn2 - 5 + j] = h1;
    }
  }

  //把图像划分成5*30有交叠的子区域
  t1 = 2;
  n1 = 13;
  t2 = 2;
  n2 = 165;
  k = 0;
  n3 = n1 * n2;
  //B=zeros(n3,1);
  //newM=zeros(n3,1);
  FILL(n3, B, 0);
  FILL(n3, newM, 0);

  for (i = 1; i <= n1; ++i) {
    for (j = 1; j <= n2; ++j) {
      int a1, a2, b1, b2, c1, a, b;
      double e1 = 0;
      int e2 = 0;
      a1 = 1 + (i - 1) * t1;
      a2 = a1 + 4;
      b1 = 1 + (j - 1) * t2;
      b2 = b1 + 29;
      c1 = ((30 - fn2) / 2);

      //A1=T(a1:a2,b1:b2);
      //A2=A1(:,(1+c1):(c1+fn2));
      //A3 = A2.*H;
      //e1=sum(sum(A3));
      for (a = a1; a <= a2; ++a) {
        for (b = b1; b < b1 + fn2; ++b) {
          double t = T[(a - 1) * IRIS_W + b + c1 - 1];
          double h = H[(a - a1) * fn2 + b - b1];
          e1 += t * h;
        }

        for (b = b1; b <= b2; ++b) {
          int t = M[(a - 1) * IRIS_W + b - 1];

          if (t != 0) {
            ++e2;
          }
        }
      }

      if (e1 > 0) {
        B[k] = 1;
      }
      else {
        B[k] = 0;
      }

      if (e2 > 60) {
        newM[k] = 1;
      }
      else {
        newM[k] = 0;
      }

      k = k + 1;
    }
  }

  return 0;
}


