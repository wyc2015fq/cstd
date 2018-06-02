#define HMMMAXMIX 4
typedef struct mix_t {
  int M;
  img_t mean[1]; // M*SIZE
  img_t var[1]; // M*SIZE
  img_t weight[1];// M*1
} mix_t;
typedef struct hmm_t {
  int N, D;
  int M[HMMMAXMIX]; //N
  img_t init[1]; //N
  img_t trans[1]; //N*N
  img_t mix[HMMMAXMIX]; // N*M(i)*(SIZE+SIZE+1)
} hmm_t;
typedef struct param_t {
  int N, D;
  img_t c[1]; //N
  img_t alpha[1]; //N
  img_t beta[1]; //N
  img_t ksai[1]; //N
  img_t gama[1]; //N
} param_t;
#include "kmeans.inl"
int hmm_setsize(hmm_t* hmm, int N, int D, const int* M)
{
  int i;
  hmm->N = N;
  hmm->D = D;
  ASSERT(N <= HMMMAXMIX);
  MEMCPY(hmm->M, M, N);
  imsetsize_f8(hmm->init, N, 1, 1);
  imsetsize_f8(hmm->trans, N, N, 1);
  for (i = 0; i < N; ++i) {
    imsetsize_f8(hmm->mix + i, M[i], D + D + 1, 1);
  }
  return 0;
}
int getmix(const img_t* vector, int M, img_t* mix)
{
  int i = 0, SIZE = vector->w;
  img_t im1[10] = {0};
  img_t* nn = im1 + i++;
  img_t* counts = im1 + i++;
  img_t* m = im1 + i++;
  img_t* var = im1 + i++;
  img_t* weight = im1 + i++;
  //imsetsize_f8(mix, M, SIZE+SIZE+1);
  // 计算每个聚类的标准差, 对角阵, 只保存对角线上的元素
  kmeans(vector, M, nn, m, var, counts);
  // 计算每个聚类中的元素数, 归一化为各pdf的权重
  imsetsize_f8(weight, M, 1, 1);
  for (i = 0; i < weight->h; ++i) {
    weight->tt.f8[i] = counts->tt.s4[i] * 1. / vector->h;
  }
  imcath(3, m, mix);
  ASSERT(mix->w == (SIZE + SIZE + 1));
  //imshowmat_f8(vector);cvWaitKey(-1);
  //imshowmat_f8(mix);imshowmat_u4(nn);cvWaitKey(-1);
  // 保存结果
  imfrees2(im1);
  return 0;
}
int inithmm(hmm_t* hmm, int K, const img_t* samples, int N, const int* M)
{
  int i = 0, j, k;
  img_t im1[10] = {0};
  img_t* segment = im1 + i++;
  img_t* vector = im1 + i++;
  img_t* im2 = 0;
  //K = length(samples); //语音样本数
  //N = length(M); //状态数
  hmm->N = N;
  MEMCPY(hmm->M, M, N);
  // 初始概率矩阵
  imsetsize_f8(hmm->init, N, 1, 1);
  hmm->init->tt.f8[0] = 1;
  // 转移概率矩阵
  imsetsize_f8(hmm->trans, N, N, 1);
  for (i = 0; i < N - 1; ++i) {
    hmm->trans->tt.f8[i * N + i] = 0.5;
    hmm->trans->tt.f8[i * N + i + 1] = 0.5;
  }
  hmm->trans->tt.f8[N * N - 1] = 1;
  //imshowmat_f8(hmm->trans); cvWaitKey(-1);
  // 概率密度函数的初始聚类
  // 平均分段
  imsetsize_i4(segment, K, N + 1, 1);
  for (k = 0; k < K; ++k) {
    int T = samples[k].h, t = 0;
    for (j = 0; j < N; ++j) {
      segment->tt.s4[k * (N + 1) + j] = j * T / N;
    }
    segment->tt.s4[k * (N + 1) + j] = T;
  }
  //imshowmat_u4(segment); cvWaitKey(-1);
  //对属于每个状态的向量进行K均值聚类,得到连续混合正态分布
  MYREALLOC(im2, K);
  for (i = 0; i < N; ++i) {
    //把相同聚类和相同状态的向量组合到一个向量中
    for (k = 0; k < K; ++k) {
      int seg1 = segment->tt.s4[k * (N + 1) + i];
      int seg2 = segment->tt.s4[k * (N + 1) + i + 1];
      //vector = [vector ; samples(k).data(seg1:seg2,:)];
      imsubref(samples + k, iRECT(0, seg1, samples[k].w, seg2), im2 + k);
    }
    imcatv(K, im2, vector);
    //imshowmat_f8(vector); cvWaitKey(-1);
    getmix(vector, M[i], hmm->mix);
  }
  FREE(im2);
  //hmm->mix = mix;
  imfrees2(im1);
  return 0;
}

