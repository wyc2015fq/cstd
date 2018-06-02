//计算两个一维定序特征的汉明距离
double Hamm(unsigned char* A1, unsigned char* M1, unsigned char* A2, unsigned char* M2)
{
  double result = 0;
  int i, a = binum;

  int fenzi = 0;
  int fenmu = 0;

  for (i = 0; i < a; ++i) {
    int M = M1[i] && M2[i];

    if (M > 0) {
      fenmu = fenmu + 1;

      if (A1[i] == A2[i]) {
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

const char* Hamm_name = "Hamm";
double Hamm_iris(Iris_t* I1, Iris_t* I2)
{
  return Hamm(I1->code, I1->newMask, I2->code, I2->newMask);
}

