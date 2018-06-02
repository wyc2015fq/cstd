#include "cstd.h"
#include "cvhmmobs.inl"
#include "cv1/utils.inl"
#include "cv1/copy.inl"
#include "cv1/mathfuncs.inl"
#include "cvhmm.inl"
#include "hmmface.inl"
void ExtractDCT(float* src, float* dst, int num_vec, int dst_len)
{
  int i;
  float* src_ = src + 1;
  float* dst_ = dst;
  for (i = 0; i < num_vec; i++) {
    memcpy(dst_, src_, dst_len * sizeof(float));
    src_ += dst_len + 1;
    dst_ += dst_len;
  }
}
int hmmface_init(hmmface_t* s)
{
  CvSize m_delta = {4, 4};
  CvSize m_obsSize = {3, 3};
  CvSize m_dctSize = {12, 12};
  int m_stnum[32] = {5, 3, 6, 6, 6, 3};
  int i;
  s->m_suppress_intensity = FALSE;
  s->m_delta = m_delta;
  s->m_obsSize = m_obsSize;
  s->m_dctSize = m_dctSize;
  s->m_vectSize = m_obsSize.h * m_obsSize.w;
  MEMCPY(s->m_stnum, m_stnum, 32);
  for (i = 0; i < 128; i++) {
    s->m_mixnum[i] = 3;
  }
  //suppress first DCT coefficient
  if (s->m_suppress_intensity) {
    s->m_vectSize--;
  }
  return 0;
}
int hmmface_im2obs(const hmmface_t* s, const img_t* im, CvImgObsInfo* obs_info_array)
{
  int i, num_img = im->f;
  for (i = 0; i < num_img; i++) {
    const uchar* data = im->tt.data + i * im->h * im->s;
    CvImgObsInfo* info = obs_info_array + i;
    CvSize roi = cvSize(im->w, im->h);
    CvSize num_obs;//用DCT系数计算的图像块大小
    CV_COUNT_OBS(&roi, &(s->m_dctSize), &(s->m_delta), &num_obs);   //在给定的图像和结构中提取的观测值
    icvCreateObsInfo(info, num_obs, s->m_vectSize);  //创建结构用于储存图像观察向量
    //IplImage* normalized_image = cvCreateImage( roi, IPL_DEPTH_8U, 1 );
    //NormalizeImageForHMM( ipl_scaled, normalized_image );
    if (s->m_suppress_intensity) {
      float* observations = (float*)malloc(num_obs.h * num_obs.w * (s->m_vectSize + 1) * sizeof(float));
      //从图像中提取的观测向量，即DCT系数
      icvImgToObs_DCT_8u32f_C1R(data, im->s, roi, observations, s->m_dctSize, s->m_obsSize, s->m_delta);
      ExtractDCT(observations, info->obs, num_obs.h * num_obs.w, s->m_vectSize);
      free(observations);
    }
    else {
      icvImgToObs_DCT_8u32f_C1R(data, im->s, roi, info->obs, s->m_dctSize, s->m_obsSize, s->m_delta);
    }
    //cvReleaseImage( &normalized_image );
  }
  return 0;
}
#include "ui/window.inl"
CvEHMM* hmmface_train(const hmmface_t* s, const img_t* im)
{
  int i, j = 0;
  CvEHMM* hmm = NULL;
  int num_img = im->f;//创建观察信息
  CvImgObsInfo* obs_info_array = NULL;
  //training loop can be not converged
  const int max_iterations = 80;
  BOOL trained = FALSE;
  float old_likelihood = 0, likelihood = 0;
  int counter = 0;
  //Create observation info
  MYREALLOC(obs_info_array, num_img);
  hmmface_im2obs(s, im, obs_info_array);
  hmm = icvCreate2DHMM(s->m_stnum, s->m_mixnum, s->m_vectSize);
  for (i = 0; i < num_img; i++) {
    icvUniformImgSegm(obs_info_array + i, hmm); //以HMM状态统一分割图像观测值
  }
  icvInitMixSegm(obs_info_array, num_img, hmm);
  while ((!trained) && (counter < max_iterations)) {
    counter++;
    icvEstimateHMMStateParams(obs_info_array, num_img, hmm); //计算每个HMM状态中的所有内在参数
    icvEstimateTransProb(obs_info_array, num_img, hmm);  //运用现有的图像观测值分割为所有嵌入和内部的HMM函数，计算可能的变换矩阵
    likelihood = 0;
    for (j = 0; j < num_img; j++) {
      icvEstimateObsProb(obs_info_array + j, hmm); //计算每个内部HMM函数状态下的每个观测值的高斯概率
      likelihood += icvEViterbi(obs_info_array + j, hmm);
    }
    likelihood /= num_img * obs_info_array[0].obs_size;
    icvMixSegmL2(obs_info_array, num_img, hmm);
    trained = (fabs(likelihood - old_likelihood) < 0.01);
    old_likelihood = likelihood;
  }
  //后续处理
  for (i = 0; i < num_img; i++) {
    icvReleaseObsInfo(&(obs_info_array[i]));
  }
  FREE(obs_info_array);
  return hmm;
}
int hmmface_recognize(const hmmface_t* s, int n, CvEHMM** hmms, const img_t* im, int* idx, double* outlikelihood)
{
  int i, j, num_img = im->f;//创建观察信息
  CvImgObsInfo* obs_info_array = NULL;
  double likelihood, maxlikelihood = 0;
  MYREALLOC(obs_info_array, num_img);
  hmmface_im2obs(s, im, obs_info_array);
  for (j = 0; j < num_img; ++j) {
    CvImgObsInfo* info = obs_info_array + j;
    idx[j] = -1;
    maxlikelihood = -FLT_MAX;
    for (i = 0 ; i < n; ++i) { //计算要识别的人与库中的人相似度
      CvEHMM* hmm = hmms[i];
      icvInitMixSegm(info, 1, hmm);
      icvEstimateObsProb(info, hmm);  //计算每个内部HMM函数状态下的每个观测值的高斯概率
      likelihood = icvEViterbi(info, hmm);  //为嵌入的HMM函数实行Viterbi算法
      if (likelihood > maxlikelihood) {
        maxlikelihood = likelihood;
        idx[j] = i;
      }
    }
    if (outlikelihood) {
      outlikelihood[j] = maxlikelihood;
    }
  }
  FREE(obs_info_array);
  return 0;
}
#include "img/imgio.inl"
int test_hmmface()
{
  //enum {menN = 15, picN = 11};
  enum {menN = 40, picN = 10};
  hmmface_t s[1] = {0};
  CvEHMM* hmm[menN] = {0};
  const char* path = NULL;
  vstr_t sv[1] = {0};
  img_t im[menN*2] = {0};
  img_t* imtrain = im;
  img_t* imtest = im + menN;
  int i, j, err=0, all=0;
  path = "E:/pub/bin/face/Yale100x100";
  path = "E:/pub/bin/face/ORL/ORL92112";
  _chdir(path);
  vstr_load("list.txt", sv);
  ASSERT(sv->n == picN * menN);
  hmmface_init(s);
  for (i = 0; i < menN; ++i) {
    int t = picN/2;
    imload_images2(imtrain + i, t, sv->v + i * picN, 0);
    imload_images2(imtest + i, picN - t, sv->v + i * picN + t, 0);
    printf("%3d", i);
    {utime_start(_start_time);
    hmm[i] = hmmface_train(s, imtrain + i);
    printf(" %f\n", utime_elapsed(_start_time));}
    //hmmface_save(s, hmm[i], "hmm.txt");
  }
  printf("训练完毕\n");
  err = 0;
  all = 0;
  for (i = 0; i < menN; ++i) {
    int idx[menN] = {0};
    double like[menN];
    img_t* imt = imtest+i;
    hmmface_recognize(s, menN, hmm, imt, idx, like);
    //printf("id = %d\n", i);
    for (j = 0; j < imt->f; ++j) {
      err += idx[j]!=i;
      printf("%d %f\n", idx[j], like[j]);
    }
    //printf("\n");
    all += imt->f;
    all += imt->f;
    printf("err rate %f\n", err*1./all);
  }
  imfrees2(im); // err rate 0.027500
  return 0;
}

