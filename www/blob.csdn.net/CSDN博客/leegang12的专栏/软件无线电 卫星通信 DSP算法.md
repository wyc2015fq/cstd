# 软件无线电 卫星通信 DSP算法 - leegang12的专栏 - CSDN博客
2013年03月07日 21:32:15[leegang12](https://me.csdn.net/leegang12)阅读数：1425
个人分类：[无线通信基带处理算法](https://blog.csdn.net/leegang12/article/category/896922)
1、  DSP算法：载波频偏算法,调用FFT函数
#include <DSPF_sp_cfftr2_dit.h>
#include <DSPF_sp_bitrev_cplx.h>
#include <DSPF_sp_maxidx.h>
2、DSP算法：匹配滤波
#include <DSPF_sp_fir_gen.h>
DSPF_sp_fir_gen(dataII,R,dataIIY,169,5656);
DSPF_sp_fir_gen(dataQ,R,dataQY,169,5656);
