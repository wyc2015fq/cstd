//%%两核滤波器
//%%先计算一个单高斯滤波器，然后图像滤波。最后通过列的距离相减
int g_dist = 10; //%%需要调整的参数
const double H1[] = {//
  0.018467626572808, 0.031032977397465, 0.036894437952942, 0.031032977397465, 0.018467626572808,
  0.031032977397465, 0.052147777753398, 0.061997368994434, 0.052147777753398, 0.031032977397465,
  0.036894437952942, 0.061997368994434, 0.073707335725952, 0.061997368994434, 0.036894437952942,
  0.031032977397465, 0.052147777753398, 0.061997368994434, 0.052147777753398, 0.031032977397465,
  0.018467626572808, 0.031032977397465, 0.036894437952942, 0.031032977397465, 0.018467626572808
};

int bianma4(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double A_j1[(IRIS_H) * (IRIS_W)];
  double A_j2[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, A_j1, IRIS_W);
  //cvShowImage("A_j1", IRIS_H, IRIS_W, A_j1, IRIS_W, 1);
  COPY((IRIS_H) * (IRIS_W), A_j1, A_j2);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      A_j2[j * IRIS_W + i] = A_j1[j * IRIS_W + i] - A_j1[j * IRIS_W + i + g_dist];
    }
  }

  //%%二值化
  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      if (A_j2[(i * IRIS_W + j)] > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}

//三核滤波器
int bianma5(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double B1[(IRIS_H) * (IRIS_W)];
  double B3[(IRIS_H) * (IRIS_W)];
  double B4[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, B1, IRIS_W);

  COPY((IRIS_H) * (IRIS_W), B1, B3);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      B3[j * IRIS_W + i] = B1[j * IRIS_W + i] - 2 * B1[j * IRIS_W + i + g_dist];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B3, B4);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - 2 * g_dist); ++i) {
      B4[j * IRIS_W + i] = B3[j * IRIS_W + i] + B1[j * IRIS_W + i + 2 * g_dist];
    }
  }

  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      if (B4[(i * IRIS_W + j)] > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}
//两核滤波器,带角度旋转的
//先计算一个单高斯滤波器，然后图像滤波。最后通过列的距离相减
int bianma6(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double A_j1[(IRIS_H) * (IRIS_W)];
  double A_j2[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, A_j1, IRIS_W);
  //cvShowImage("A_j1", IRIS_H, IRIS_W, A_j1, IRIS_W, 1);
  COPY((IRIS_H) * (IRIS_W), A_j1, A_j2);

  for (j = 0; j < (a - g_dist); ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      A_j2[j * IRIS_W + i] = A_j1[j * IRIS_W + i] - A_j1[(j + g_dist) * IRIS_W + i + g_dist];
    }
  }

  //%%二值化
  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      if (A_j2[(i * IRIS_W + j)] > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}

//三核滤波器
int bianma7(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double A_j1[(IRIS_H) * (IRIS_W)];
  double A_j2[(IRIS_H) * (IRIS_W)];

  double B1[(IRIS_H) * (IRIS_W)];
  double B3[(IRIS_H) * (IRIS_W)];
  double B4[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, B1, IRIS_W);

  COPY((IRIS_H) * (IRIS_W), B1, A_j1);
  COPY((IRIS_H) * (IRIS_W), A_j1, A_j2);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      A_j2[j * IRIS_W + i] = A_j1[j * IRIS_W + i] - A_j1[j * IRIS_W + i + g_dist];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B1, B3);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      B3[j * IRIS_W + i] = B1[j * IRIS_W + i] - 2 * B1[j * IRIS_W + i + g_dist];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B3, B4);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - 2 * g_dist); ++i) {
      B4[j * IRIS_W + i] = B3[j * IRIS_W + i] + B1[j * IRIS_W + i + 2 * g_dist];
    }
  }

  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      double B5 = B4[(i * IRIS_W + j)] + A_j2[(i * IRIS_W + j)];

      if (B5 > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}

//三核滤波器
int bianma8(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  int dist1 = 5;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double A1[(IRIS_H) * (IRIS_W)];
  double A2[(IRIS_H) * (IRIS_W)];
  double B1[(IRIS_H) * (IRIS_W)];
  double B3[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, A1, IRIS_W);
  COPY((IRIS_H) * (IRIS_W), A1, A2);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - dist1); ++i) {
      A2[j * IRIS_W + i] = A1[j * IRIS_W + i] - A1[j * IRIS_W + i + dist1];
    }
  }

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, B1, IRIS_W);
  COPY((IRIS_H) * (IRIS_W), B1, B3);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      B3[j * IRIS_W + i] = B1[j * IRIS_W + i] - B1[j * IRIS_W + i + g_dist];
    }
  }

  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      double B5 = B3[(i * IRIS_W + j)] + A2[(i * IRIS_W + j)];

      if (B5 > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}

//三核滤波器
int bianma9(Iris_t* ir)
{
  const unsigned char* A = ir->data;
  unsigned char* A_f = ir->Juanl;
  int i, j, a = IRIS_H, b = IRIS_W;
  int fn = 5;
  double alf = 1.7;
  int dist1 = 5;
  // H1=fspecial('gaussian',fn,alf);

  //A_j1 = imfilter(A,H1);
  double B1[(IRIS_H) * (IRIS_W)];
  double B2[(IRIS_H) * (IRIS_W)];
  double B3[(IRIS_H) * (IRIS_W)];
  double B4[(IRIS_H) * (IRIS_W)];
  double B5[(IRIS_H) * (IRIS_W)];

  Imfilter_f64(IRIS_H, IRIS_W, A, IRIS_W, 5, 5, H1, B1, IRIS_W);
  COPY((IRIS_H) * (IRIS_W), B1, B2);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - dist1); ++i) {
      B2[j * IRIS_W + i] = B1[j * IRIS_W + i] - 2 * B1[j * IRIS_W + i + dist1];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B2, B3);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - 2 * dist1); ++i) {
      B3[j * IRIS_W + i] = B2[j * IRIS_W + i] + B1[j * IRIS_W + i + 2 * dist1];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B1, B4);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - g_dist); ++i) {
      B4[j * IRIS_W + i] = B1[j * IRIS_W + i] - 2 * B1[j * IRIS_W + i + g_dist];
    }
  }

  COPY((IRIS_H) * (IRIS_W), B4, B5);

  for (j = 0; j < IRIS_H; ++j) {
    for (i = 0; i < (b - 2 * g_dist); ++i) {
      B5[j * IRIS_W + i] = B4[j * IRIS_W + i] + B1[j * IRIS_W + i + 2 * g_dist];
    }
  }

  for (i = 0; i < a; ++i) {
    for (j = 0; j < b; ++j) {
      double B6 = B3[(i * IRIS_W + j)] + B5[(i * IRIS_W + j)];

      if (B6 > 0) {
        A_f[(i * IRIS_W + j)] = 255;
      }
      else {
        A_f[(i * IRIS_W + j)] = 0;
      }
    }
  }

  return 0;
}
