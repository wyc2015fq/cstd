#include <math.h>
// 随机数发生器，这样使得其余部分的代码称为平台无关的。
// 得到一个随机数种子，可以更具自己的情况修改该函数
int hmmgetseed(void)
{
  return (17 * rand());
}
// 设置随机数种子
void hmmsetseed(int seed)
{
  srand(seed);
}
// 得到0－1之间的一个双精度随机数
double hmmgetrand(void)
{
  return (double) rand() / RAND_MAX;
}
//int N; /* 状态数； Q={1,2,...,N} */
//int M; /* 观察值数; V={1,2,...,M} */
//double* pi; /* pi[N] pi[i] 是进入Markov链的状态分布 */
//double* A; /* A[N*N]. a[i*N+j] 从状态i转移到状态j的转移概率*/
//double* B; /* B[N*M]. b[j*M+k] 在状态j观察到观察值k的概率 */
#define HMM_DEF() int N=0, M=0; double* pi=0, *A=0, *B=0
#define HMM_GET_PARAM(phmm) HMM_DEF(); hmm_get_param(phmm, &N, &M, &pi, &A, &B)
int hmm_get_param(void* s, int* pN, int* pM, double** ppi, double** pA, double** pB)
{
  int* p = (int*)(s);
  int N = p[0], M = p[1];
  double* p2 = (double*)(p + 2);
  if (pN) {
    *pN = N;
  }
  if (pM) {
    *pM = M;
  }
  if (ppi) {
    *ppi = p2;
  }
  if (pA) {
    *pA = p2 + N;
  }
  if (pB) {
    *pB = p2 + N + N * N;
  }
  return 0;
}
int hmm_resize(void* pphmm, int N, int M, double** ppi, double** pA, double** pB)
{
  uchar** pphmm1 = (uchar**)pphmm;
  int size = 2 * sizeof(int) + (N + N * N + N * M) * sizeof(double);
  int* p;
  double* p2;
  MYREALLOC(*pphmm1, size);
  p = (int*)(*pphmm1);
  p2 = (double*)(p + 2);
  p[0] = N, p[1] = M;
  if (ppi) {
    *ppi = p2;
  }
  if (pA) {
    *pA = p2 + N;
  }
  if (pB) {
    *pB = p2 + N + N * N;
  }
  return size;
}
// 序列的生成等函数
// 产生初始状态
int GenInitalState(void* phmm)
{
  double val, accum;
  int i, q_t;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  q_t = N;
  for (i = 0; i < N; ++i) {
    if (val < pi[i] + accum) {
      q_t = i;
      break;
    }
    else {
      accum += pi[i];
    }
  }
  return q_t;
}
// 生产下一个状态
int GenNextState(void* phmm, int q_t)
{
  double val, accum;
  int j, q_next;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  q_next = N;
  for (j = 0; j < N; ++j) {
    if (val < A[q_t * N + j] + accum) {
      q_next = j;
      break;
    }
    else {
      accum += A[q_t * N + j];
    }
  }
  return q_next;
}
// 生成观察值
int GenSymbol(void* phmm, int q_t)
{
  double val, accum;
  int j, o_t;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  o_t = M;
  for (j = 0; j < M; ++j) {
    if (val < B[q_t * M + j] + accum) {
      o_t = j;
      break;
    }
    else {
      accum += B[q_t * M + j];
    }
  }
  return o_t;
}
// 函数名称 - GenSequenceArray
// 功能 - 生产随机状态
// 参数 - phmm,void结构指针
// seed:随机数种子
// T - 观察序列长度
// O[T] - 观察值序列
// q[T] - 产生的序列
void GenSequenceArray(void* phmm, int seed, int T, int* O, int* q) //O q 为动态分配数组
{
  int t = 0;
  hmmsetseed(seed);
  q[1] = GenInitalState(phmm);
  O[1] = GenSymbol(phmm, q[1]);
  for (t = 1; t < T; ++t) {
    q[t] = GenNextState(phmm, q[t - 1]);
    O[t] = GenSymbol(phmm, q[t]);
  }
}
// HMM文件的读写等操作
// 函数名称 - ReadHMM
// 功能 - 读取void结构
// 参数 - fp - 文件指针
// phmm - void结构指针，保存void结构
// 返回值 - 无
int ReadHMM(FILE* fp, void* pphmm)
{
  int i, j, k, N, M;
  double* pi, *A, *B;
  fscanf(fp, "M= %d\n", &(M));
  fscanf(fp, "N= %d\n", &(N));
  hmm_resize(pphmm, N, M, &pi, &A, &B);
  fscanf(fp, "pi:\n");
  for (i = 0; i < N; ++i) {
    fscanf(fp, "%lf", &(pi[i]));
  }
  fscanf(fp, "A:\n");
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      fscanf(fp, "%lf", &(A[i * N + j]));
    }
    fscanf(fp, "\n");
  }
  fscanf(fp, "B:\n");
  for (j = 0; j < N; ++j) {
    for (k = 0; k < M; ++k) {
      fscanf(fp, "%lf", &(B[j * M + k]));
    }
    fscanf(fp, "\n");
  }
  return 0;
}
// 函数名称 - InitHMM
// 功能 - 初始化void结构
// 参数 - phmm - void结构指针
// N - 状态数
// M - 可观察值得个数
// seed - 随机数种子
// 返回值 - 无
void InitHMM(void* pphmm, int N, int M, int seed)
{
  int i, j, k;
  double sum;
  double* pi, *A, *B;
  hmm_resize(pphmm, N, M, &pi, &A, &B);
  /* 初始化随机数发生器 */
  hmmsetseed(seed);
  for (i = 0; i < N; ++i) {
    sum = 0.0;
    for (j = 0; j < N; ++j) {
      A[i * N + j] = hmmgetrand();
      sum += A[i * N + j];
    }
    for (j = 0; j < N; ++j) {
      A[i * N + j] /= sum;
    }
  }
  for (j = 0; j < N; ++j) {
    sum = 0.0;
    for (k = 0; k < M; ++k) {
      B[j * M + k] = hmmgetrand();
      sum += B[j * M + k];
    }
    for (k = 0; k < M; ++k) {
      B[j * M + k] /= sum;
    }
  }
  sum = 0.0;
  for (i = 0; i < N; ++i) {
    pi[i] = hmmgetrand();
    sum += pi[i];
  }
  for (i = 0; i < N; ++i) {
    pi[i] /= sum;
  }
}
// 函数名称 - PrintHMM
// 功能 - 保存void结构
// 参数 - fp - 文件指针
// phmm - void结构指针
// 返回值 - 无
void PrintHMM(FILE* fp, void* phmm)
{
  int i, j, k;
  HMM_GET_PARAM(phmm);
  fprintf(fp, "M= %d\n", M);
  fprintf(fp, "N= %d\n", N);
  fprintf(fp, "A:\n");
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      fprintf(fp, "%f ", A[i * N + j]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "B:\n");
  for (j = 0; j < N; ++j) {
    for (k = 0; k < M; ++k) {
      fprintf(fp, "%f ", B[j * M + k]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "pi:\n");
  for (i = 0; i < N; ++i) {
    fprintf(fp, "%f ", pi[i]);
  }
  fprintf(fp, "\n\n");
}
// 给定观察值序列和void模型，利用前向算法求取其概率
// 函数名称 - hmm_forward
// 功能 - 后向算法估计参数
// 参数 - phmm:指向void的指针
// T - 观察值序列的长度
// O - 观察值序列
// alpha:运算中用到的临时数组
// pprob:返回值，所要求的概率**
int hmm_forward(void* phmm, int T, const int* O, double* alpha, double* pprob)
{
  int i, j; //状态下标
  int t; //时间下标
  double sum; //局部总和
  HMM_GET_PARAM(phmm);
  //1,初始化
  for (i = 0; i < N; ++i) {
    alpha[i] = pi[i] * B[i * M + O[0]];
  }
  //2,递归
  for (t = 1; t < T; ++t) {
    for (j = 0; j < N; ++j) {
      sum = 0.0;
      for (i = 0; i < N; ++i) {
        sum += alpha[(t - 1) * N + i] * (A[i * N + j]);
      }
      alpha[t * N + j] = sum * (B[j * M + O[t]]);
    }
  }
  //3,终止
  if (pprob) {
    sum = 0.0;
    for (i = 0; i < N; ++i) {
      sum += alpha[(T - 1) * N + i];
    }
    *pprob = sum;
  }
  return 0;
}
// 功能 - 前向算法估计参数
// 参数 - phmm - 指向void的指针
// T - 观察值序列的长度
// O - 观察值序列
// alpha - 运算中用到的临时数组
// scale - 比例因子
// ppmh - 返回值，所要求的概率
// pprob 是对数概率
int hmm_ForwardWithScale(void* phmm, int T, int* O, double* alpha, double* scale, double* pprob)
{
  int i, j; //状态下标
  int t; //时间下标
  double sum; //局部总和
  HMM_GET_PARAM(phmm);
  //1，初始化
  sum = 0.0;
  for (i = 0; i < N; ++i) {
    alpha[i] = pi[i] * (B[i * M + O[0]]);
    sum += alpha[i];
  }
  scale[0] = sum;
  for (i = 0; i < N; ++i) {
    alpha[i] /= scale[0];
  }
  //2,递归
  for (t = 1; t < T; ++t) {
    scale[t] = 0.0;
    for (j = 0; j < N; ++j) {
      sum = 0.0;
      for (i = 0; i < N; ++i) {
        sum += alpha[(t - 1) * N + i] * (A[i * N + j]);
      }
      alpha[t * N + j] = sum * (B[j * M + O[t]]);
      scale[t] += alpha[t * N + j];
    }
    for (j = 0; j < N; ++j) {
      alpha[t * N + j] /= scale[t];
    }
  }
  //3,终止
  if (pprob) {
    sum = 0.0;
    for (t = 0; t < T; ++t) {
      sum += log(scale[t]);
    }
    *pprob = sum;
  }
  return 0;
}
// 后向算法估计参数 给定观察值序列和void模型，利用前向后向算法求取其概率
// 参数 - phmm - 指向void的指针
// T - 观察值序列的长度
// O - 观察值序列
// beta - 运算中用到的临时数组
// pprob - 返回值，所要求的概率
//
int hmm_backward(void* phmm, int T, const int* O, double* beta, double* pprob)
{
  int i, j; /* 状态 */
  int t; /* 时间下标 */
  double sum;
  HMM_GET_PARAM(phmm);
  /* 1. 初始化 */
  for (i = 0; i < N; ++i) {
    beta[(T - 1)*N + i] = 1.0;
  }
  /* 2. 递归 */
  for (t = T - 2; t >= 0; --t) {
    for (i = 0; i < N; ++i) {
      sum = 0.0;
      for (j = 0; j < N; ++j) {
        sum += A[i * N + j] * (B[j * M + O[t + 1]]) * beta[(t + 1) * N + j];
      }
      beta[t * N + i] = sum;
    }
  }
  /* 3. 终止 */
#if 0
  *pprob = 0.0;
  for (i = 0; i < N; ++i) {
    *pprob += beta[i];
  }
#else
  *pprob = 0.0;
  for (i = 0; i < N; ++i) {
    *pprob += beta[i] * pi[i] * B[i * M];
  }
#endif
  return 0;
}
// 后向算?ü兰撇问ù壤蜃有拚?
// phmm - 指向void的指针
// T - 观察值序列的长度
// O[T] - 观察值序列
// beta[T*N] - 运算中用到的临时数组
// scale[T] - 比例因子数组
// pprob - 返回值，所要求的概率
int hmm_BackwardWithScale(void* phmm, int T, int* O, double* beta, double* scale, double* pprob)
{
  int i, j; /* 状态指示 */
  int t; /* 时间下标 */
  double sum;
  HMM_GET_PARAM(phmm);
  /* 1. 初始化 */
  for (i = 0; i < N; ++i) {
    beta[(T - 1)*N + i] = 1.0 / scale[T - 1];
  }
  /* 2. 递归 */
  for (t = T - 2; t >= 0; --t) {
    for (i = 0; i < N; ++i) {
      sum = 0.0;
      for (j = 0; j < N; ++j) {
        sum += A[i * N + j] * (B[j * M + O[t + 1]]) * beta[(t + 1) * N + j];
      }
      beta[t * N + i] = sum / scale[t];
    }
  }
  return 0;
}
//
#define VITHUGE 100000000000.0
// 给定HMM和观察序列，求最可能的状态
// phmm - void结构指针
// T - 观察值的个数
// O[T] - 观察序列
// delta[T*N] psi[T*N] 为中间变量
// q[T] - 求得的最佳状态序列
// pprob - 概率
//
int hmm_Viterbi(void* phmm, int T, const int* O, double* delta, int* psi, int* q, double* pprob)
{
  int i, j; /* 状态下标 */
  int t; /* 时间下标 */
  int maxvalind;
  double maxval, val;
  HMM_GET_PARAM(phmm);
  /* 1. 初始化 */
  for (i = 0; i < N; ++i) {
    delta[i] = pi[i] * (B[i * M + O[0]]);
    psi[i] = 0;
  }
  /* 2. 递归 */
  for (t = 1; t < T; ++t) {
    for (j = 0; j < N; ++j) {
      maxval = delta[(t - 1) * N] * (A[j]);
      maxvalind = 0;
      for (i = 1; i < N; ++i) {
        val = delta[(t - 1) * N + i] * (A[i * N + j]);
        if (val > maxval) {
          maxval = val;
          maxvalind = i;
        }
      }
      delta[t * N + j] = maxval * (B[j * M + O[t]]);
      psi[t * N + j] = maxvalind;
    }
  }
  /* 3. 终止 */
  *pprob = delta[(T - 1) * N];
  q[T - 1] = 0;
  for (i = 1; i < N; ++i) {
    if (delta[(T - 1)*N + i] > *pprob) {
      *pprob = delta[(T - 1) * N + i];
      q[T - 1] = i;
    }
  }
  /* 4. Path (state sequence) backtracking */
  for (t = T - 1; t > 0; --t) {
    q[t - 1] = psi[t * N + q[t]];
  }
  return 0;
}
/**************************************************************************
** 函数名称 - hmm_ViterbiLog
** 功能 - hmm_Viterbi算法
** 参数 - phmm - void结构指针
** T - 观察值的个数
** O - 观察序列
** delta，psi为中间变量
** q - 求得的最佳状态序列
** pprob - 概率
**/
int hmm_ViterbiLog(void* phmm, int T, int* O, double* delta, int* psi, int* q, double* pprob)
{
  int i, j; /* 状态下标 */
  int t; /* 时间下标 */
  int maxvalind;
  double maxval, val;
  double* biot = 0;
  HMM_GET_PARAM(phmm);
  /* 0. 预处理 */
  for (i = 0; i < N; ++i) {
    pi[i] = log(pi[i]);
  }
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      A[i * N + j] = log(A[i * N + j]);
    }
  }
  MYREALLOC(biot, N * T);
  for (i = 0; i < N; ++i)
    for (t = 0; t < T; ++t) {
      biot[i * N + t] = log(B[i * M + O[t]]);
    }
  /* 1. 初始化 */
  for (i = 0; i < N; ++i) {
    delta[1 * N + i] = pi[i] + biot[i * N + 1];
    psi[1 * N + i] = 0;
  }
  /* 2. 递归 */
  for (t = 2; t <= T; ++t) {
    for (j = 0; j < N; ++j) {
      maxval = -VITHUGE;
      maxvalind = 1;
      for (i = 0; i < N; ++i) {
        val = delta[t - 1 * N + i] + (A[i * N + j]);
        if (val > maxval) {
          maxval = val;
          maxvalind = i;
        }
      }
      delta[t * N + j] = maxval + biot[j * N + t];
      psi[t * N + j] = maxvalind;
    }
  }
  /* 3. 终止 */
  *pprob = -VITHUGE;
  q[T] = 1;
  for (i = 0; i < N; ++i) {
    if (delta[T * N + i] > *pprob) {
      *pprob = delta[T * N + i];
      q[T] = i;
    }
  }
  /* 4. 回溯 */
  for (t = T - 1; t >= 1; t--) {
    q[t] = psi[t * N + q[t]];
  }
  FREE(biot);
  return 0;
}
#if 0
#define DELTA 0.001
// 计算Gamma
void ComputeGamma(void* phmm, int T, double* gamma, double* xi)
{
  int i, j;
  int t;
  double denominator;
  HMM_GET_PARAM(phmm);
  for (t = 0; t < T; ++t) {
    for (i = 0; i < N; ++i) {
      gamma[t * N + i] = 0;
    }
  }
  for (t = 0; t < T; ++t) {
    denominator = 0.0;
    for (i = 0; i < N; ++i) {
      for (j = 1; j < N; j++) {
        gamma[t * N + i] += xi[t * N + i * N + j];
      }
      denominator += gamma[t * N + i];
    }
    for (i = 0; i < N; ++i) {
      gamma[t * N + i] = gamma[t * N + i] / denominator;
    }
  }
}
// 计算Xi
void ComputeXi(void* phmm, int T, int* O, double* alpha, double* beta, double* xi)
{
  int i, j;
  int t;
  double sum;
  HMM_GET_PARAM(phmm);
  for (t = 0; t < T - 1; ++t) {
    sum = 0.0;
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j) {
        xi[t * N + i * N + j] = alpha[t * N + i] * beta[t * N + j] * (A[i * N + j]) * (B[j * M + O[t]]);
        sum += xi[t * N + i * N + j];
      }
    }
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j) {
        xi[t * N + i * N + j] /= sum;
      }
    }
  }
}
// phmm - void模型指针
// T - 观察序列长度
// O - 观察序列
// alpha，beta，gamma，pniter均为中间变量
// plogprobinit - 初始概率
// plogprobfinal 最终概率
int BaumWelch(void* phmm, int T, const int* O, double* alpha, double* beta,
    double* gamma, int* pniter, double* plogprobinit, double* plogprobfinal)
{
  int i, j, k;
  int t, l = 0;
  double logprobf, logprobb;
  double numeratorA, denominatorA;
  double numeratorB, denominatorB;
  double* xi = 0, *scale = 0;
  double delta, logprobprev, test;
  double deltaprev = 10e-70;
  MYREALLOC(scale, (T + T * N * N)); //申请一个1*T双精度实数数组
  xi = scale + T; //
  hmm_ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
  // hmm_forward( phmm, T, O, alpha,&logprobf );
  *plogprobinit = logprobf; /* log P(O |初始状态) */
  *plogprobinit = pow(10, *plogprobinit);
  // hmm_BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
  hmm_backward(phmm, T, O, beta, &logprobb);
  test = logprobb; //临时加的值，用于测试
  ComputeXi(phmm, T, O, alpha, beta, xi);//非用户接口函数
  ComputeGamma(phmm, T, gamma, xi); //非用户接口函数
  logprobprev = logprobf;
  do {
    /* 重新估计 t=1 时，状态为i 的频率 */
    for (i = 0; i < N; ++i) {
      pi[i] = .001 + .999 * gamma[1 * N + i]; //pi的初始化
    }
    /* 重新估计转移矩阵和观察矩阵 */
    for (i = 0; i < N; ++i) {
      denominatorA = 0.0;
      for (t = 0; t < T - 1; ++t) {
        denominatorA += gamma[t * N + i];
      }
      for (j = 0; j < N; ++j) {
        numeratorA = 0.0;
        for (t = 0; t < T - 1; ++t) {
          numeratorA += xi[t * N + i * N + j];
        }
        A[i * N + j] = .001 + .999 * numeratorA / denominatorA;
      }
      denominatorB = denominatorA + gamma[T * N + i];
      for (k = 0; k < M; ++k) {
        numeratorB = 0.0;
        for (t = 0; t < T; ++t) {
          if (O[t] == k) {
            numeratorB += gamma[t * N + i];
          }
        }
        B[i * M + k] = .001 + .999 * numeratorB / denominatorB;
      }
    }
    hmm_ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
    hmm_BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
    ComputeXi(phmm, T, O, alpha, beta, xi);
    ComputeGamma(phmm, T, gamma, xi);
    /* 计算两次直接的概率差 */
    delta = logprobf - logprobprev;
    logprobprev = logprobf;
    l++;
  }
  while (delta < DELTA); /* 如果差的不太大，表明收敛，退出 */
  *pniter = l;
  *plogprobfinal = logprobf; /* log P(O|estimated model) */
  FREE(scale);
  return 0;
}
#endif

