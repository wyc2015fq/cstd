#ifndef _CUDA_GMM_H_
#define _CUDA_GMM_H_

// 类型定义.
typedef unsigned char uchar;

// GMM类前项声明.
struct GMM;

// 函数声明.

// 建立GMM类对象.
GMM* CreateGMM(int Height, int Width, int Step);

// 销毁GMM类对象.
void DestroyGMM(struct GMM **pGMM);

// 建立GMM背景模型.
int SetupBGModel(struct GMM *pGMM, const uchar *src, int num);

// 前景检测.
int ForegroundDetect(GMM *pGMM, const uchar *src);

// 返回检测结果二值图.
const int* GetBinResult(GMM *pGMM);

#endif