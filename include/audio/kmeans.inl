double distfun(int n, const double* A, const double* B)
{
  int i;
  double dis = 0;
  for (i = 0; i < n; ++i) {
    double a = A[i] - B[i];
    dis += a * a;
  }
  return dis;
}
int getmean(int h, int w, const double* A, int al, const int* idx, int* counts, double* mean, double* var)
{
  int i, j;
  for (i = 0; i < h; ++i) {
    counts[i] = 0;
  }
  for (i = 0; i < w; ++i) {
    mean[i] = 0;
  }
  for (i = 0; i < h; ++i) {
    const double* A1 = A + i * al;
    int id = idx[i];
    counts[id]++;
    for (j = 0; j < w; ++j) {
      mean[id] += A1[j];
    }
  }
  return 0;
}
int kmeans(const img_t* X, int K, img_t* idx0, img_t* C0, img_t* var0, img_t* counts0)
{
  int i = 0, j = 0, k, m = X->h, n = X->w;
  img_t im1[10] = {0};
  img_t* idx = im1 + i++;
  img_t* C = im1 + i++;
  img_t* D = im1 + i++;
  img_t* counts = im1 + i++;
  int changed = 0;
  double sumdis;
  K = MIN(K, m);
  imsetsize_i4(idx, m, 1, 1);
  imsetsize_i4(counts, K, 1, 1);
  imsetsize_f8(C, K, n, 1);
  imsetsize_f8(D, m, K, 1);
  for (k = 0; k < m; ++k) {
    //idx->tt.s4[k] = rand()%K;
    idx->tt.s4[k] = k % K;
  }
  for (i = 0; i < 100; ++i) {
    changed = 0;
    for (k = 0; k < K; ++k) {
      counts->tt.s4[k] = 0;
      for (j = 0; j < n; ++j) {
        C->tt.f8[k * n + j] = 0;
      }
    }
    for (k = 0; k < m; ++k) {
      int id = idx->tt.s4[k];
      counts->tt.s4[id]++;
      for (j = 0; j < n; ++j) {
        C->tt.f8[id * n + j] += X->tt.f8[k * n + j];
      }
    }
    for (k = 0; k < K; ++k) {
      int cn = counts->tt.s4[k];
      for (j = 0; j < n; ++j) {
        C->tt.f8[k * n + j] /= cn;
      }
    }
    //imshowmat_f8(C);cvWaitKey(-1);
    sumdis = 0;
    for (j = 0; j < m; ++j) {
      double mindis = 1000000000000;
      int id = -1;
      for (k = 0; k < K; ++k) {
        double dis = distfun(n, X->tt.f8 + j * n, C->tt.f8 + k * n);
        if (dis < mindis) {
          mindis = dis;
          id = k;
        }
      }
      sumdis += mindis;
      changed += idx->tt.s4[j] != id;
      idx->tt.s4[j] = id;
    }
    //printf("%d %f\n", i, sumdis);
    if (changed < 1) {
      //break;
    }
  }
  if (var0) {
    imsetsize_f8(var0, K, n, 1);
    for (k = 0; k < K; ++k) {
      counts->tt.s4[k] = 0;
      for (j = 0; j < n; ++j) {
        var0->tt.f8[k * n + j] = 0;
      }
    }
    for (k = 0; k < m; ++k) {
      int id = idx->tt.s4[k];
      counts->tt.s4[id]++;
      for (j = 0; j < n; ++j) {
        double a = X->tt.f8[k * n + j] - C->tt.f8[id * n + j];
        var0->tt.f8[id * n + j] += a * a;
      }
    }
    for (k = 0; k < K; ++k) {
      int cn = counts->tt.s4[k];
      for (j = 0; j < n; ++j) {
        double a = var0->tt.f8[k * n + j];
        var0->tt.f8[k * n + j] = a / cn;
      }
    }
    //imshowmat_f8(var0);cvWaitKey(-1);
  }
  //imshowmat_u4(counts);cvWaitKey(-1);
  //imshowmat_f8(C);cvWaitKey(-1);
  if (idx0) {
    imclone2(idx, idx0);
  }
  if (counts0) {
    imclone2(counts, counts0);
  }
  if (C0) {
    imclone2(C, C0);
  }
  imfrees2(im1);
  return 0;
}

