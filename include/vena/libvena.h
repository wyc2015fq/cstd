
#ifndef _LIBVENA_H_
#define _LIBVENA_H_

#define VENA_DEBUG 0 // 调试模式
#define VENA_STDH  96 //归一化图片高
#define VENA_STDW  64 //归一化图片宽
#define VENA_FEATLEN  (64*59) // 特征最小字节

//#undef GETARG
//#undef GETARGS
// buf, buflen 需要的内存至少大于 10M
// h, w, img, imgstep, imgcn cn 输入图像 支持 3(彩色) 和 1(灰度)
// n 库中特征个数
// featlib 特征库 可以为NULL
// featstep 一个特征占用字节数 必须>=VENA_FEATLEN
// imstd 归一化 图片 [VENA_STDH*VENA_STDW] 可以为NULL
// feat 提取当前图片的特征 可以为NULL
// pid 输出id
// pdis 输出距离
// 函数返回 1表示检测到 0表示没检测到

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) int  __stdcall vena_recog_bf(unsigned char* buf, int buflen, int h, int w, unsigned char* img, int imgstep, int imgcn, int n, unsigned char* featlib, int featstep, unsigned char* imstd, unsigned char* feat, int* pid, int* pdis);

#ifdef __cplusplus
}
#endif

#endif // _LIBVENA_H_
