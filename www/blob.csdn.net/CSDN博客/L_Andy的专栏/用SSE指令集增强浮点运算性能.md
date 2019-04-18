# 用SSE指令集增强浮点运算性能 - L_Andy的专栏 - CSDN博客

2018年04月27日 10:06:26[卡哥](https://me.csdn.net/L_Andy)阅读数：249



代码效率优化

1，算法优化/改变算法

2，改变结构,传输数据等方面

3，改变指令（cpu指令集来进行优化）

SSE是很常见的一个X86平台的指令集，早在P4时代就已经出现了。后来INTEL又接连着推出了SSE2，SSE3，SSE4等（不过可没有SSE5，原本规划是有的，后来INTEL独立发展了新一代AVX指令集旨在取代SSE，关于AVX现在资料还不是很多，用的也没有SSE普遍。毕竟支持AVX的CPU也不多，像我的T4400就不支持）。

废话不多说，还是来点实在的东西。大家都知道浮点数运算比起整数运算，速度的确是非常缓慢，很多领域比如图像处理中，需要大量用到浮点数运算，此时CPU就是一个很显著的瓶颈，为了提高浮点数性能，我们有两个方法：

1，化浮点为整形：即尽量通过某种数学变换将原来的浮点数运算变成整数运算。

2，使用SSE这类指令集：显然这种方法是本文重点，不过方法1也会一并用起。

以一个很常见的图像彩色转灰度为例。

根据色彩学上的一些理论，将一个RGB彩色像素转换成灰度，实际上是一个1*3矩阵和一个3*1矩阵相乘，说白了就是如下过程：

设原像素为p0 = （r0，g0，b0），转换为s=（r0*0.3，g0*0.6，b0*0.1），然后新的灰度像素p1=（s，s，s）。

这里可以看到，求得s值这一步中，有三次浮点运算，我们可以用方法1将这里的浮点运算暂时化为整数（全部乘以10），即

s=（r0*3，g0*6，b0*1），最后一次性除以10。

具体代码如下：

void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount) 

{ 

  DWORD dwRGBSum = 0; 

  for(DWORD dwIndex = 0; dwIndex < size; dwIndex+=3) 

  { 

    dwRGBSum =    

      1 * pIn[dwIndex+0] +     //Blue  

      6 * pIn[dwIndex+1] +     //Green  

      3 * pIn[dwIndex+2];        //Red  

    dwRGBSum /= 10.0; 

    pIn[dwIndex+0] = dwRGBSum; 

    pIn[dwIndex+1] = dwRGBSum; 

    pIn[dwIndex+2] = dwRGBSum; 

  } 

} 


现在我们再来使用SSE来进一步优化。

SSE一次性可以处理128位的运算，即4个浮点数。因而我们将四次除法放在一次进行，核心的一个数据结构是__m128，这是一个联合体，具体参见其源码。

SSE中批量浮点数乘法对应的C函数是_mm_mul_ps。用法可以参考MSDN或者INTEL官方网站上的一个PDF。


void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount) 

{ 

  UINT16 dwRGBSum0 = 0; 

  UINT16 dwRGBSum1 = 0; 

  UINT16 dwRGBSum2 = 0; 

  UINT16 dwRGBSum3 = 0; 

  for(DWORD idx = 0; idx < size; idx+=12) 

  { 

    dwRGBSum0 =    

      1 * pIn[idx+0] +     //Blue 

      6 * pIn[idx+1] +     //Green 

      3 * pIn[idx+2];        //Red 

    dwRGBSum1 =    

      1 * pIn[idx+3] +     //Blue 

      6 * pIn[idx+4] +     //Green 

      3 * pIn[idx+5];        //Red 

    dwRGBSum2 =    

      1 * pIn[idx+6] +     //Blue 

      6 * pIn[idx+7] +     //Green 

      3 * pIn[idx+8];        //Red 

    dwRGBSum3 =    

      1 * pIn[idx+9] +     //Blue 

      6 * pIn[idx+10] +     //Green 

      3 * pIn[idx+11];        //Red 

    __m128 old = _mm_set_ps(dwRGBSum0, dwRGBSum1, dwRGBSum2, dwRGBSum3); 

    __m128 ret = _mm_mul_ps(old, vec); 

    pIn[idx+0] = pIn[idx+1] = pIn[idx+2] = (BYTE)ret.m128_f32[3]; 

    pIn[idx+3] = pIn[idx+4] = pIn[idx+5] = (BYTE)ret.m128_f32[2]; 

    pIn[idx+6] = pIn[idx+7] = pIn[idx+8] = (BYTE)ret.m128_f32[1]; 

    pIn[idx+9] = pIn[idx+10] = pIn[idx+11] = (BYTE)ret.m128_f32[0]; 

  } 

}

代码看上去要比原来的复杂许多，不过其实原理很简单的，原来一次性处理一个像素，现在一次性处理4个，性能和效率大大提升了。

这个代码其实还可以优化的，因为SSE内除了浮点可以批量处理，整数也是可以的，对应的数据结构是__m128i。涉及到一些矩阵的知识，我就不多说了。

上面这个优化性能测试结果还是很明显的，原来的程序对一个2560*1600，24位色深的图片进行转换，需要将近800ms的时间，优化后，只需450ms了，提高了将近一倍。

![](https://img-blog.csdn.net/20180427100827217)

