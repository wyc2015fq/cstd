
#include <stdlib.h>
#include <math.h>

#define round(x)  (x < 0 ? ceil((x) - 0.5) : floor((x) + 0.5))

void RGB2Lab(double R, double G, double B, double* L, double* a, double* b);
void Lab2RGB(double L, double a, double b, double* R, double* G, double* B);

int imBiFilter(int src_height, int src_width, double* src,
    double* des, int w, double sigma_d, double sigma_r);

#define FOREACH_DO(_N, _I, _DO)  for (_I=0; _I<(_N); ++_I) {_DO;}

int imCartoon(int src_height, int src_width, unsigned char* src, int src_step)
{
  int       i, j, r, c;
  int     w = 5;
  int     quant_levels = 8;
  double  sigma_d = 3, sigma_r = 0.1;
  double  max_gradient = 0.2;
  double  shapness_1 = 3, shapness_2 = 14;
  double  min_edge_strength = 0.1;
  int src_d_height = src_height;
  int src_d_width = src_width;
  int height = src_d_height;
  int width = src_d_width;
  int B_height = src_d_height;
  int B_width = src_d_width;
  int B1_height = src_d_height;
  int B1_width = src_d_width;
  int G_height = src_d_height;
  int G_width = src_d_width;
  int E_height = src_d_height;
  int E_width = src_d_width;
  int qB1_height = src_d_height;
  int qB1_width = src_d_width;
  int _N;
  double  dq;
  double  PixR, PixG, PixB;
  double* pData = NULL, *B, *B1, *GX, *GY, *G, *E, *qB1;
  double* src_d = (double*) pmalloc(sizeof(double) * 3 * width * height);

  if (src_d == NULL) {
    return 0;
  }

  for (r = 0; r < height; r++) {
    for (c = 0; c < width; c++) {
      int index = r * width + c;
      pData = src_d + index * 3;
      RGB2Lab((src[index * 3 + 2]) / 255., (src[index * 3 + 1]) / 255., (src[index * 3]) / 255., pData, pData + 1, pData + 2);
    }
  }

  B_height = src_d_height;
  B_width = src_d_width;
  B = (double*) pmalloc(sizeof(double) * B_height * B_width * 3);

  if (B == NULL) {
    return 0;
  }

  imBiFilter(src_d_height, src_d_width, src_d, B, w, sigma_d, sigma_r);
  B1_height = B_height;
  B1_width = B_width;
  _N = B1_height * B1_width;
  B1 = (double*) pmalloc(sizeof(double) * _N);
  GX = (double*) pmalloc(sizeof(double) * _N);
  GY = (double*) pmalloc(sizeof(double) * _N);
  G = (double*) pmalloc(sizeof(double) * _N);

  if (B1 == NULL || GX == NULL || GY == NULL) {
    return 0;
  }

  for (i = 0; i < B1_height; i++) {
    for (j = 0; j < B1_width; j++) {
      int index = i * B1_width + j;
      B1[index] = B[index * 3] / 100;
    }
  }

  //M_gradient(&B1, &GX, &GY);
  //M_square(&GX, &GX);
  //M_square(&GY, &GY);
  //M_sqrt(&G, &G);
  //M_Plus(&GX, &GY, &G);
  FOREACH_DO(_N, i,
      GX[i] = (i >= 1 && i < (_N - 1)) ? ((B1[i + 1] - B1[i - 1]) / 2.) : 0;
      GY[i] = (i >= B1_width && i < (_N - B1_width)) ? ((B1[i + B1_width] - B1[i - B1_width]) / 2.) : 0;
      GX[i] *= GX[i];
      GY[i] *= GY[i];
      G[i] = sqrt(GX[i] + GY[i]);
            );

  for (i = 0; i < G_height; i++) {
    for (j = 0; j < G_width; j++) {
      int index = i * G_width + j;

      if (G[index] > max_gradient) {
        G[index] = 1;
      }
      else {
        G[index] /= max_gradient;
      }
    }
  }

  pfree(GX);
  pfree(GY);
  E_height = G_height;
  E_width = G_width;
  E = (double*) pmalloc(sizeof(double) * E_height * E_width);

  if (E == NULL) {
    return 0;
  }

  for (i = 0; i < E_height; i++) {
    for (j = 0; j < E_width; j++) {
      int index = i * E_width + j;

      if (G[index] < min_edge_strength) {
        E[index] = 0;
      }
      else {
        E[index] = G[index];
      }
    }
  }

  //MS_Mul(&G, shapness_2 - shapness_1, &G);
  //MS_Plus(&G, shapness_1, &G);
  {
    double ss = shapness_2 - shapness_1;
    FOREACH_DO(_N, i, G[i] = G[i] * ss + shapness_1);
  }
  qB1_height = B1_height;
  qB1_width = B1_width;
  qB1 = (double*) pmalloc(sizeof(double) * qB1_height * qB1_width);

  if (qB1 == NULL) {
    return 0;
  }

  {
    dq = 100. / (quant_levels - 1);

    for (i = 0; i < B1_height; i++) {
      for (j = 0; j < B1_width; j++) {
        int index = i * B1_width + j;
        B1[index] = B[index * 3];
        qB1[index] = dq * (round(B1[index] / dq));
      }
    }
  }

  //M_Minus(&B1, &qB1, &B1);
  //M_dotProduct(&G, &B1, &G);
  //M_tanh(&G, &G);
  //MS_Mul(&G, dq / 2, &G);
  //M_Plus(&qB1, &G, &qB1);
  {
    double ss = dq / 2.;
    FOREACH_DO(_N, i, B1[i] -= qB1[i]);
    FOREACH_DO(_N, i, G[i] = ss * tanh(G[i] * B1[i]));
    FOREACH_DO(_N, i, qB1[i] += G[i]);
  }

  for (i = 0; i < B_height; i++) {
    for (j = 0; j < B_width; j++) {
      int index = i * B_width + j;
      B[index * 3] = qB1[index];
      pData = src_d + index * 3;
      Lab2RGB(B[index * 3], B[index * 3 + 1], B[index * 3 + 2], pData + 2, pData + 1, pData);
    }
  }

  for (i = 0; i < src_d_height; i++) {
    for (j = 0; j < src_d_width; j++) {
      int     index = i * src_d_width + j;
      double  scale = 1 - E[index];
      src_d[index * 3] *= scale;
      src_d[index * 3 + 1] *= scale;
      src_d[index * 3 + 2] *= scale;
      PixR = src_d[index * 3] * 255;
      PixG = src_d[index * 3 + 1] * 255;
      PixB = src_d[index * 3 + 2] * 255;
      src[3 * index] = (unsigned char)((PixR >= 0) ? (PixR <= 255 ? PixR : 255) : 0);
      src[3 * index + 1] = (unsigned char)((PixG >= 0) ? (PixG <= 255 ? PixG : 255) : 0);
      src[3 * index + 2] = (unsigned char)((PixB >= 0) ? (PixB <= 255 ? PixB : 255) : 0);
    }
  }

  pfree(src_d);
  pfree(B);
  pfree(B1);
  pfree(G);
  pfree(E);
  pfree(qB1);
  return 1;
}

