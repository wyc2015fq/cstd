#include "inithmm.inl"
#include "mixture.inl"
#include "getparam.inl"
#include "baum.inl"
#include "viterbi.inl"
//输入:
// samples -- 样本结构
// M -- 为每个状态指定pdf个数,如:[3 3 3 3]
//输出:
// hmm -- 训练完成后的hmm
int trainhmm(hmm_t* hmm, int K, const img_t* samples, int N, const int* M)
{
  int i = 0, k;
  img_t im[10] = {0};
  img_t* q = im + i++;
  double pout1 = 0;
  double pout = 0;
  int loop;
  // 计算语音参数
  //disp('正在计算语音参数');
  //inithmm(hmm, K, samples, N, M);
  for (loop = 0; loop < 40; ++loop) {
    printf("第%d遍训练\n", loop + 1);
    baum(hmm, K, samples);
    //计算总输出概率
    pout = 0;
    for (k = 0; k < K; ++k) {
      pout += viterbi(hmm, samples + k, q);
    }
    printf("总和输出概率(log)=%f\n", pout);
    //比较两个HMM的距离
    if (loop > 1) {
      double ss = fabs((pout - pout1) / pout);
      if (ss < 5e-6) {
        printf("收敛!\n");
        return 0;
      }
    }
    pout1 = pout;
  }
  printf("迭代40次仍不收敛, 退出\n");
  imfrees2(im);
  return 0;
}

