#include "math\math.def"

int getEllipsepar(int N, const IPOINT* vec_point, double* vec_result)
{
  double x3y1 = 0, x1y3 = 0, x2y2 = 0, yyy4 = 0, xxx3 = 0, xxx2 = 0, x2y1 = 0, yyy3 = 0, x1y2 = 0 , yyy2 = 0, x1y1 = 0, xxx1 = 0, yyy1 = 0;
  int m_i;

  for (m_i = 0; m_i < N; ++m_i) {
    double xi = vec_point[m_i].x;
    double yi = vec_point[m_i].y;
    x3y1 += xi * xi * xi * yi;
    x1y3 += xi * yi * yi * yi;
    x2y2 += xi * xi * yi * yi;
    yyy4 += yi * yi * yi * yi;
    xxx3 += xi * xi * xi;
    xxx2 += xi * xi;
    x2y1 += xi * xi * yi;

    x1y2 += xi * yi * yi;
    yyy2 += yi * yi;
    x1y1 += xi * yi;
    xxx1 += xi;
    yyy1 += yi;
    yyy3 += yi * yi * yi;
  }

  {
    double resul1 = -(x3y1);
    double resul2 = -(x2y2);
    double resul3 = -(xxx3);
    double resul4 = -(x2y1);
    double resul5 = -(xxx2);
    double A1 = x2y2, B1 = x1y3, C1 = x2y1, D1 = x1y2, E1 = x1y1;
    double A2 = x1y3, B2 = yyy4, C2 = x1y2, D2 = yyy3, E2 = yyy2;
    double A3 = x2y1, B3 = x1y2, C3 = xxx2, D3 = x1y1, E3 = xxx1;
    double A4 = x1y2, B4 = yyy3, C4 = x1y1, D4 = yyy2, E4 = yyy1;
    double A5 = x1y1, B5 = yyy2, C5 = xxx1, D5 = yyy1, E5 = N;

    double Md[5];
    double Mb[5] = {resul1, resul2, resul3, resul4, resul5};
    double Ma[5 * 5] = {
      A1, B1, C1, D1, E1,
      A2, B2, C2, D2, E2,
      A3, B3, C3, D3, E3,
      A4, B4, C4, D4, E4,
      A5, B5, C5, D5, E5,
    };
    //cvSolve(Ma, Mb, Md);
    SOLVE5(Ma, Mb, Md);

    {
      double A = Md[0];
      double B = Md[1];
      double C = Md[2];
      double D = Md[3];
      double E = Md[4];

      double XC = (2 * B * C - A * D) / (A * A - 4 * B);
      double YC = (2 * D - A * D) / (A * A - 4 * B);

      double fenzi = 2 * (A * C * D - B * C * C - D * D + 4 * E * B - A * A * E);

      double fenmu = (A * A - 4 * B) * (B - sqrt(A * A + (1 - B) * (1 - B)) + 1);
      double femmu2 = (A * A - 4 * B) * (B + sqrt(A * A + (1 - B) * (1 - B)) + 1);
      double XA = sqrt(fabs(fenzi / fenmu));
      double XB = sqrt(fabs(fenzi / femmu2));
      double Xtheta = atan(sqrt((XA * XA - XB * XB * B) / (XA * XA * B - XB * XB)) + 0.0001) * 180 / 3.1415926;

      vec_result[0] = (XC);
      vec_result[1] = (YC);
      vec_result[2] = (XA);
      vec_result[3] = (XB);
      vec_result[4] = (Xtheta);
    }
  }

  return 0;
}
