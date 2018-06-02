int baum(hmm_t* hmm, int K, const img_t* samples)
{
  img_t* mix = hmm->mix;                 //高斯混合
  int N = hmm->N;                //HMM状态数
  int D = hmm->D;
  // int K = samples->h; //语音样本数
  int SIZE = samples->w; //参数阶数
  int i = 0, j, k, l, t;
  img_t im[10] = {0};
  img_t* nommean = im + i++;
  img_t* nomvar = im + i++;
  param_t* param = NULL;
  img_t* trans = hmm->trans;
  MYREALLOC(param, K);
  MEMSET(param, 0, K);
  //imshowmat_f8(samples);cvWaitKey(-1);
  // 计算前向, 后向概率矩阵, 考虑多观察序列和下溢问题
  printf("计算样本参数...\n");
  for (k = 0; k < K; ++k) {
    //fprintf("//d ",k)
    getparam(hmm, samples + k, param + k);
  }
  //fprintf("\n")
  // 重估转移概率矩阵A: trans
  printf("重估转移概率矩阵A...\n");
  for (i = 0; i < N - 1; ++i) {
    double* trans0 = trans->tt.f8 + i * trans->w;
    double denom = 0;
    for (k = 0; k < K; ++k) {
      img_t* ksai = param[k].ksai;
      double* ksai0 = ksai->tt.f8;
      for (t = 0; t < ksai->h; ++t) {
        double* ksai1 = ksai0 + t * ksai->w + i * N;
        for (j = 0; j < N; ++j) {
          denom += ksai1[j];
        }
      }
    }
    for (j = i; j <= i + 1; ++j) {
      double nom = 0;
      int ij = i * N + j;
      for (k = 0; k < K; ++k) {
        img_t* ksai = param[k].ksai;
        double* ksai0 = ksai->tt.f8 + ij;
        for (t = 0; t < ksai->h; ++t) {
          nom += *ksai0;
          ksai0 += ksai->w;
        }
      }
      trans0[j] = nom / denom;
    }
  }
  //imshowmat_f8(trans);cvWaitKey(-1);
  imsetsize_f8(nommean, SIZE, 1, 1);
  imsetsize_f8(nomvar, SIZE, 1, 1);
  // 重估混合高斯的参数
  printf("重估混合高斯的参数...\n");
  for (i = 0; i < N; ++i) {
    img_t* mix0 = mix + i;
    double* nommean0 = nommean->tt.f8;
    double* nomvar0 = nomvar->tt.f8;
    double denom, nom, denom0 = 0;
    for (k = 0; k < K; ++k) {
      param_t* param0 = param + k;
      img_t* gama0 = param0->gama;
      const img_t* samples0 = samples + k;
      int T = samples0->h;
      int maxM = gama0->w / N;
      for (t = 0; t < T; ++t) {
        for (j = 0; j < hmm->M[i]; ++j) {
          denom0 += gama0->tt.f8[t * gama0->w + i * maxM + j];
        }
      }
    }
    for (j = 0; j < hmm->M[i]; ++j) {
      double* mean0 = mix0->tt.f8 + j * mix0->w;
      double* var0 = mean0 + D;
      double* weight0 = var0 + D;
      //fprintf("//d,//d ",i,j)
      // 计算各pdf的均值和方差
      for (l = 0; l < SIZE; ++l) {
        nommean0[l] = 0;
        nomvar0[l] = 0;
      }
      denom = 0;
      for (k = 0; k < K; ++k) {
        param_t* param0 = param + k;
        const img_t* gama0 = param0->gama;
        const img_t* samples0 = samples + k;
        int T = samples0->h;
        int maxM = gama0->w / N;
        //imshowmat_f8(gama0);cvWaitKey(-1);
        for (t = 0; t < T; ++t) {
          double* x = samples0->tt.f8 + t * samples0->w;
          double gt = gama0->tt.f8[t * gama0->w + i * maxM + j];
          for (l = 0; l < SIZE; ++l) {
            double d = x[l] - mean0[l];
            nommean0[l] += gt * x[l];
            nomvar0[l] += gt * (d * d);
          }
          denom += gt;
        }
      }
      for (l = 0; l < SIZE; ++l) {
        mean0[l] = nommean0[l] / denom;
        var0[l] = nomvar0[l] / denom;
      }
      // 计算各pdf的权
      nom = 0;
      denom = 0;
      for (k = 0; k < K; ++k) {
        param_t* param0 = param + k;
        img_t* gama0 = param0->gama;
        const img_t* samples0 = samples + k;
        int T = samples0->h;
        int maxM = gama0->w / N;
        for (t = 0; t < T; ++t) {
          nom += gama0->tt.f8[t * gama0->w + i * maxM + j];
        }
        //tmp = param(k).gama(:,i,:);
        //denom = denom + sum(tmp(:));
      }
      *weight0 = nom / denom0;
      //imshowmat_f8(hmm->mix);cvWaitKey(-1);
    }
    //fprintf("\n")
  }
  imfrees2(im);
  return 0;
}

