//计算两个定序特征的汉明距离
double HDR(unsigned char* A1_f, unsigned char* B1, unsigned char* A2_f, unsigned char* B2)
{
  double result;
  int i;

  int fenzi = 0;
  int fenmu = 0;

  for (i = 0; i < IRIS_H * IRIS_W; ++i) {
    int B = B1[i] & B2[i];

    if (B > 0) {
      fenmu = fenmu + 1;

      if (A1_f[i] == A2_f[i]) {
        fenzi = fenzi;
      }
      else {
        fenzi = fenzi + 1;
      }
    }
  }

  if (fenmu > 0) {
    result = fenzi * 1. / fenmu;
  }
  else {
    result = 0;
  }

  return result;
}

double HDR2(unsigned char* A1_f, unsigned char* B1, unsigned char* A2_f, unsigned char* B2)
{
  double result;
  int i;

  int fenzi = 0;
  int fenmu = IRIS_H * IRIS_W;

  for (i = 0; i < IRIS_H * IRIS_W; ++i) {
    if (A1_f[i] == A2_f[i]) {
      fenzi = fenzi;
    }
    else {
      fenzi = fenzi + 1;
    }
  }

  result = fenzi * 1. / fenmu;
  return result;
}

const char* HDR_name = "HDR";
double HDR_iris(Iris_t* I1, Iris_t* I2)
{
  return HDR(I1->Juanl, I1->mask, I2->Juanl, I2->mask);
}