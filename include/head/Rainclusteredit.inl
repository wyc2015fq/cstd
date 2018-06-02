
#if _MSC_VER>1200
//#define _USEOPENMP
#endif
#ifdef _USEOPENMP
#include "omp.h"
#endif

int cvShowImage_u16(const char* name, int h, int w, const unsigned short* A, int step, int cn);


// Edit by hudalikm
// Time：2013.06.09
// Second edit time: 2013.07.04
// 这个方法的好处在于它不需要设置太多的参数，针对不同高度的人和不同高度的摄像机也有很好的鲁棒性，
// 参数几乎不用任何调整即可直接使用。缺点就是要求人不能有太大的动作，
// 例如举手等，而且如果拉着拉杆箱也不能处理，会在拉杆箱上有错误检测。如果有兴趣，
// 可以在这方面有所改进。
// 快速算法改进
// 参数有待验证
// 注意某些深陷点的计算，有可能超出边界
// buflen=12*h*w
int Rainclusteredit(int h, int w, const DEPTH_TYPE* I0, int I0_step, int RN, int RD, int beishu, DEPTH_TYPE* ClusterI, int ClusterI_step, unsigned char* buf, int buflen)
{
  int* index, *I, *g;
  int i, j, k, al = w, pos, pos1, MaxValue = 0, FillValue, sizeforeground, RainNum, thd = 0;
  int neighbour[8];
  int neighbour8[][2] = { -1, 0, 0, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1}; //和当前像素坐标相加得到八个邻域坐标
  BUFUSEBEGIN(buf, buflen);

  for (k = 0; k < 8; ++k) {
    neighbour[k] = neighbour8[k][0] * al + neighbour8[k][1];
  }

  index = (int*)BUFMALLOC(int, h * w);
  I = (int*)BUFMALLOC(int, h * w);
  g = (int*)BUFMALLOC(int, h * w);
  BUFBOUNDCHECK();
  memset(index, 0, h * w * sizeof(int));
  memset(I, 0, h * w * sizeof(int));
  memset(g, 0, h * w * sizeof(int));
  // 设置非前景区域填充值(黑色0区域填充值)，目的方便边界处理，相于与构造一个水池的围墙
  FillValue = 99999999;
  k = 0;

  // 找出图像中的非零点
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      pos = i * al + j;
      I[pos] = I0[i * I0_step + j];

      //if (i>10 && i<50 && j>10 && j<50) {I[pos]=255;}
      if (I[pos] > thd && i > 0 && j > 0 && i < h - 1 && j < w - 1) {
        index[k++] = pos;
      }
      else {
        I[pos] = FillValue; // 非前景区域填充
      }
    }
  }

  // 前景点数目
  sizeforeground = k;
  //RainNum=100*sizeforeground/25; // 雨滴数目，也就是总的外循环次数
  RainNum = sizeforeground * beishu; // 雨滴数目，也就是总的外循环次数

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      pos = i * al + j;
      g[pos] = 0;
    }
  }

  srand(56);
#ifdef _USEOPENMP
  //printf("Number of core : %d\n", omp_get_num_procs());
  //printf("Number of threads : %d\n", omp_get_num_threads());
  #pragma omp parallel for
#endif

  for (i = 0; i < RainNum; ++i) {
    //int Rands=i%sizeforeground; // 模拟雨滴的随机降落
    int Rands = rand() % sizeforeground; // 模拟雨滴的随机降落
    int W = RN;
    int r = RD;
    int findt = 0; // 用于指示是初始雨滴还是流下的雨滴
    int repnum = 0, Flg = 0, Value, DirectInd;
    int d[8] = {0};
    int Index[8] = {0};
    int m, tt;
    pos = index[Rands];

    while (W > 0) {
      if (pos < 0 || pos > h * w) {
        break;
      }

      tt = I[pos] + g[pos];

      for (m = 0; m < 8; ++m) {
        pos1 = pos + neighbour[m];
        d[m] = I[pos1] + g[pos1] - tt;
      }

      repnum = 1; // 最小值的个数
      Index[0] = 0;

      // 寻找最d中最小值的索引
      for (m = 1; m < 8; ++m) {
        if (d[m] < d[Index[0]]) {
          Index[0] = m;
          repnum = 1;
        }
        else if (d[m] == d[Index[0]]) {
          Index[repnum++] = m;
        }
      }

      if (findt == 0) { // 初始雨滴
        Flg = Index[rand() % repnum]; // 由于是初始雨滴，那么从最小值中随机挑选一个方向
        Value = d[Flg]; // 雨滴流向的最小值

        if (Value < 0) {
          pos += neighbour[Flg]; // 坐标跳转
        }
        else {
          g[pos] += MIN(r, W);
          W = W - r;
        }
      }
      else {  // 非初始雨滴
        int Indicate = 0;
        DirectInd = 0; // 用于指示最小值方向之值是否为0

        // 如果有与上一步最小值方向相同的方向，优先选择该方向
        for (m = 0; m < repnum; ++m) {
          if (Flg == Index[m]) {
            Indicate = 1;
            break;
          }
        }

        if (Indicate) {
          Value = d[Flg]; // 雨滴流向的最小值

          if (Value == 0) { //　如果最小值等于０，则DirectInd=1
            DirectInd = 1;
          }
        }
        else {
          // 如果与上一部方向不同，且最小值个数不止一个的时候，则随机选取一个方向作为流动方向(0和小于0的情况是否考虑)
          Flg = Index[rand() % repnum]; // 由于是初始雨滴，那么从最小值中随机挑选一个方向
          Value = d[Flg]; // 雨滴流向的最小值
        }

        if (DirectInd != 1) { // 假如最小值不是0
          if (Value < 0) { // 那么当最小值小于0时，执行坐标跳转
            pos += neighbour[Flg]; // 坐标跳转
          }
          else {
            g[pos] += MIN(r, W);
            W = W - r;
          }
        }
        else {
          // 当最小值为0(但是只是针对与上一步方向相同的0值处理，与上一步方向不同的0值处理则不在此列)
          pos += neighbour[Flg]; // 坐标跳转
        }
      }

      findt = findt + 1; // 标记当前雨滴寻优的步数，用于指示是否是初始雨滴
    }

#if 0

    if (0 && RainNum > 2200) {
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          ClusterI[i * ClusterI_step + j] = I0[i * I0_step + j] > thd ? 255 : 0;
        }
      }

      cvShowImage_u16("III", h, w, ClusterI, al, 1);

      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          ClusterI[i * ClusterI_step + j] = g[i * I0_step + j] > 1 ? 255 : 0;
        }
      }

      cvShowImage_u16("ClusterI", h, w, ClusterI, al, 1);

      cvWaitKey(-1);
    }

#endif
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      ClusterI[i * ClusterI_step + j] = g[i * I0_step + j];
    }
  }

#if 0

  if (0 && RainNum > 200) {
    cvShowImage_u16("ClusterI", h, w, ClusterI, al, 1);
    cvWaitKey(-1);
  }

#endif
  //free(g);
  //free(I);
  //free(index);
  BUFUSEEND();
  return 0;
}
