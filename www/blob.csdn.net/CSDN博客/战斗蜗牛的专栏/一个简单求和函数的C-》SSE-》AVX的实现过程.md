# 一个简单求和函数的C-》SSE-》AVX的实现过程 - 战斗蜗牛的专栏 - CSDN博客





2014年08月06日 23:54:46[vbskj](https://me.csdn.net/vbskj)阅读数：3985









## 这篇文章写的非常好，特别是文中的链接也要仔细浏览。



## ======================================





## （转）Intrinsic—使用SSE、AVX指令集处理单精度浮点数组求和（支持vc、gcc，兼容Win、Linux、Mac）
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)(2013-01-14
 10:45:16)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载▼


|标签：### [杂谈](http://search.sina.com.cn/?c=blog&q=%D4%D3%CC%B8&by=tag)|分类：[汇编](http://blog.sina.com.cn/s/articlelist_1256290490_15_1.html)|
|----|----|


[[C] 跨平台使用Intrinsic函数范例1——使用SSE、AVX指令集 处理 单精度浮点数组求和（支持vc、gcc，兼容Windows、Linux、Mac）](http://www.cnblogs.com/zyl910/archive/2012/10/22/simdsumfloat.html)

！！

http://www.cnblogs.com/zyl910/archive/2012/10/22/simdsumfloat.html


本文面对对SSE等SIMD指令集有一定基础的读者，以单精度浮点数组求和为例演示了如何跨平台使用SSE、AVX指令集。因使用了stdint、 zintrin、ccpuid这三个模块，可以完全避免手工编写汇编代码，具有很高可移植性。支持vc、gcc编译器，在Windows、Linux、 Mac这三大平台上成功运行。

# 一、问题背景


　　最初，我们只能使用汇编语言来编写SIMD代码。不仅写起来很麻烦，而且易读性、可维护性、移植性都较差。

　　不久，VC、GCC等编译器相继支持了Intrinsic函数，使我们可以摆脱汇编，利用C语言来调用SIMD指令集，大大提高了易读性和可维护。而且移植性也有提高，能在同一编译器上实现32位与64位的平滑过渡。

　　但当代码在另一种编译器编译时，会遇到一些问题而无法编译。甚至在使用同一种编译器的不同版本时，也会遇到无法编译问题。


　　首先是整数类型问题——

　　传统C语言的short、int、long等整数类型是与平台相关的，不同平台上的位长是不同的（例如 Windows是LLP64模型，Linux、Mac等Unix系统多采用LP64模型）。而使用SSE等SIMD指令集时需要精确计算数据的位数，不同 位长的数据必须使用不同的指令来处理。

　　有一个解决办法，就是使用C99标准中stdint.h所提供的指定位长的整数类型。GCC对C99标准支持性较好，而VC的步骤很慢，貌似直到VC2010才支持stdint.h。而很多时候我们为了兼容旧代码，不得不使用VC6等老版本的VC编译器。


　　其次是Intrinsic函数的头文件问题，不同编译器所使用的头文件不同——

　　对于早期版本VC，需要根据具体的指令集需求，手动引入mmintrin.h、xmmintrin.h等头文件。对于VC2005或更高版本，引入intrin.h就行了，它会自动引入当前编译器所支持的所有Intrinsic头文件。

　　对于早期版本GCC，也是手动引入mmintrin.h、xmmintrin.h等头文件。而对于高版本的GCC，引入x86intrin.h就行了，它会自动引入当前编译环境所允许的Intrinsic头文件。


　　再次是当前编译环境下的Intrinsic函数集支持性问题——

　　对于VC来说，VC6支持MMX、3DNow!、SSE、 SSE2，然后更高版本的VC支持更多的指令集。但是，VC没有提供检测Intrinsic函数集支持性的办法。例如你在VC2010上编写了一段使用了 AVX Intrinsic函数的代码，但拿到VC2005上就不能通过编译了。其次，VC不支持64位下的MMX，这让一些老程序迁徙到64位版时遭来了一些麻 烦。

　　而对于GCC来说，它使用-mmmx、-msse等编译器开关来启用各种指令集，同时定义了对应的 __MMX__、__SSE__等宏，然后x86intrin.h会根据这些宏来声明相应的Intrinsic函数集。__MMX__、__SSE__等 宏可以帮助我们判断Intrinsic函数集是否支持，但这只是GCC的专用功能。

　　此外还有一些细节问题，例如某些Intrinsic函数仅在64下才能使用、有些老版本编译器的头文件缺少某个Intrinsic函数。所以我们希望有一种统一的方式来判断Intrinsic函数集的支持性。


　　除了编译期间的问题外，还有运行期间的问题——

　　在运行时，怎么检测当前处理器支持哪些指令集？

　　虽然X86体系提供了用来检测处理器的CPUID指令，但它没有规范的Intrinsic函数，在不同的编译器上的用法不同。

　　而且X86体系有很多种指令集，每种指令集具体的检测方法是略有区别的。尤其是SSE、AVX这样的SIMD指令集是需要操作系统配合才能正常使用的，所以在CPUID检查通过后，还需要进一步验证。

# 二、范例讲解

## 2.1 事先准备


　　为了解决上面提到的问题，我编写了三个模块——

stdint：智能支持C99的stdint.h，解决整数类型问题。最新版的地址是[http://www.cnblogs.com/zyl910/archive/2012/08/08/c99int.html](http://www.cnblogs.com/zyl910/archive/2012/08/08/c99int.html) 。

zintrin：在编译时检测Intrinsic函数集支持性，并自动引入相关头文件、修正细节问题。最新版的地址是[http://www.cnblogs.com/zyl910/archive/2012/10/01/zintrin_v101.html](http://www.cnblogs.com/zyl910/archive/2012/10/01/zintrin_v101.html) 。

ccpuid：在编译时检测指令集的支持性。最新版的地址是[http://www.cnblogs.com/zyl910/archive/2012/10/13/ccpuid_v103.html](http://www.cnblogs.com/zyl910/archive/2012/10/13/ccpuid_v103.html) 。


　　这三个模块的纯C版就是一个头文件，用起来很方便，将它们放在项目中，直接#include就行了。例如——

#define __STDC_LIMIT_MACROS 1 // C99整数范围常量. [纯C程序可以不用, 而C++程序必须定义该宏.]#include "zintrin.h" #include "ccpuid.h"




　　因为stdint.h会被zintrin.h或ccpuid.h引用，所以不需要手动引入它。

　　因为它们用到了C99整数范围常量，所以应该在程序的最前面定义__STDC_LIMIT_MACROS宏（或者可以在项目配置、编译器命令行等位置进行配置）。根据C99规范，纯C程序可以不用, 而C++程序必须定义该宏。本文为了演示，定义了该宏。

## 2.2 C语言版


　　我们先用C语言编写一个基本的单精度浮点数组求和函数——


// 单精度浮点数组求和_基本版.

//

// result: 返回数组求和结果.

// pbuf: 数组的首地址.

// cntbuf: 数组长度.

float sumfloat_base(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    for(i=0; i

    {

        s += pbuf[i];

    }

    return s;

}


该函数很容易理解——先将返回值赋初值0，然后循环加上数组中每一项的值。

## 2.3 SSE版

### 2.3.1 SSE普通版


　　SSE寄存器是128位的，对应__m128类型，它能一次能处理4个单精度浮点数。

　　很多SSE指令要求内存地址按16字节对齐。本文为了简化，假定浮点数组的首地址是总是16字节对齐的，仅需要考虑数组长度不是4的整数倍问题。

　　因使用了SSE Intrinsic函数，我们可以根据zintrin.h所提供的INTRIN_SSE宏进行条件编译。

　　代码如下——


#ifdef INTRIN_SSE

// 单精度浮点数组求和_SSE版.

float sumfloat_sse(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    size_t nBlockWidth = 4;    // 块宽. SSE寄存器能一次处理4个float.

    size_t cntBlock = cntbuf / nBlockWidth;    // 块数.

    size_t cntRem = cntbuf % nBlockWidth;    // 剩余数量.

    __m128 xfsSum = _mm_setzero_ps();    // 求和变量。[SSE] 赋初值0

    __m128 xfsLoad;    // 加载.

    const float* p = pbuf;    // SSE批量处理时所用的指针.

    const float* q;    // 将SSE变量上的多个数值合并时所用指针.


    // SSE批量处理.

    for(i=0; i

    {

        xfsLoad = _mm_load_ps(p);    // [SSE] 加载

        xfsSum = _mm_add_ps(xfsSum, xfsLoad);    // [SSE] 单精浮点紧缩加法

        p += nBlockWidth;

    }

    // 合并.

    q = (const float*)&xfsSum;

    s = q[0] + q[1] + q[2] + q[3];


    // 处理剩下的.

    for(i=0; i

    {

        s += p[i];

    }


    return s;

}


#endif    // #ifdef INTRIN_SSE


上述代码大致可分为四个部分——

1. 变量定义与初始化。

2. SSE批量处理。即对前面能凑成4个一组的数据，利用SSE的128位宽度同时对4个数累加。

3. 合并。将__m128上的多个数值合并到求和变量。因考虑某些编译器不能直接使用“.”来访问__m128变量中的数据，于是利用指针q来访问xfsSum中的数据。

4. 处理剩下的。即对尾部不能凑成4个一组的数据，采用基本的逐项相加算法。


　　上述代码总共用到了3个SSE Intrinsic函数——

_mm_setzero_ps：对应XORPS指令。将__m128上的每一个单精度浮点数均赋0值，伪代码：for(i=0;i<4;++i) C[i]=0.0f。

_mm_load_ps：对应MOVPS指令。从内存中对齐加载4个单精度浮点数到__m128变量，伪代码：for(i=0;i<4;++i) C[i]=_A[i]。

_mm_add_ps：对应ADDPS指令。相加，即对2个__m128变量的4个单精度浮点数进行垂直相加，伪代码：for(i=0;i<4;++i) C[i]=A[i]+B[i]。

### 2.3.2 SSE四路循环展开版


　　循环展开可以降低循环开销，提高指令级并行性能。

　　一般来说，四路循环展开就差不多够了。我们可以很方便的将上一节的代码改造为四路循环展开版——


// 单精度浮点数组求和_SSE四路循环展开版.

float sumfloat_sse_4loop(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 返回值.

    size_t i;

    size_t nBlockWidth = 4*4;    // 块宽. SSE寄存器能一次处理4个float，然后循环展开4次.

    size_t cntBlock = cntbuf / nBlockWidth;    // 块数.

    size_t cntRem = cntbuf % nBlockWidth;    // 剩余数量.

    __m128 xfsSum = _mm_setzero_ps();    // 求和变量。[SSE] 赋初值0

    __m128 xfsSum1 = _mm_setzero_ps();

    __m128 xfsSum2 = _mm_setzero_ps();

    __m128 xfsSum3 = _mm_setzero_ps();

    __m128 xfsLoad;    // 加载.

    __m128 xfsLoad1;

    __m128 xfsLoad2;

    __m128 xfsLoad3;

    const float* p = pbuf;    // SSE批量处理时所用的指针.

    const float* q;    // 将SSE变量上的多个数值合并时所用指针.


    // SSE批量处理.

    for(i=0; i

    {

        xfsLoad = _mm_load_ps(p);    // [SSE] 加载.

        xfsLoad1 = _mm_load_ps(p+4);

        xfsLoad2 = _mm_load_ps(p+8);

        xfsLoad3 = _mm_load_ps(p+12);

        xfsSum = _mm_add_ps(xfsSum, xfsLoad);    // [SSE] 单精浮点紧缩加法

        xfsSum1 = _mm_add_ps(xfsSum1, xfsLoad1);

        xfsSum2 = _mm_add_ps(xfsSum2, xfsLoad2);

        xfsSum3 = _mm_add_ps(xfsSum3, xfsLoad3);

        p += nBlockWidth;

    }

    // 合并.

    xfsSum = _mm_add_ps(xfsSum, xfsSum1);    // 两两合并(0~1).

    xfsSum2 = _mm_add_ps(xfsSum2, xfsSum3);    // 两两合并(2~3).

    xfsSum = _mm_add_ps(xfsSum, xfsSum2);    // 两两合并(0~3).

    q = (const float*)&xfsSum;

    s = q[0] + q[1] + q[2] + q[3];


    // 处理剩下的.

    for(i=0; i

    {

        s += p[i];

    }


    return s;

}

## 2.4 AVX版

### 2.4.1 AVX普通版


　　AVX寄存器是256位的，对应__m256类型，它能一次能处理8个单精度浮点数。

　　很多AVX指令要求内存地址按32字节对齐。本文为了简化，假定浮点数组的首地址是总是32字节对齐的，仅需要考虑数组长度不是8的整数倍问题。

　　因使用了AVX Intrinsic函数，我们可以根据zintrin.h所提供的INTRIN_AVX宏进行条件编译。


　　代码如下——


#ifdef INTRIN_AVX

// 单精度浮点数组求和_AVX版.

float sumfloat_avx(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    size_t nBlockWidth = 8;    // 块宽. AVX寄存器能一次处理8个float.

    size_t cntBlock = cntbuf / nBlockWidth;    // 块数.

    size_t cntRem = cntbuf % nBlockWidth;    // 剩余数量.

    __m256 yfsSum = _mm256_setzero_ps();    // 求和变量。[AVX] 赋初值0

    __m256 yfsLoad;    // 加载.

    const float* p = pbuf;    // AVX批量处理时所用的指针.

    const float* q;    // 将AVX变量上的多个数值合并时所用指针.


    // AVX批量处理.

    for(i=0; i

    {

        yfsLoad = _mm256_load_ps(p);    // [AVX] 加载

        yfsSum = _mm256_add_ps(yfsSum, yfsLoad);    // [AVX] 单精浮点紧缩加法

        p += nBlockWidth;

    }

    // 合并.

    q = (const float*)&yfsSum;

    s = q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7];


    // 处理剩下的.

    for(i=0; i

    {

        s += p[i];

    }


    return s;

}


#endif    // #ifdef INTRIN_AVX


由上可见，将SSE Intrinsic代码（sumfloat_sse）升级为 AVX Intrinsic代码（sumfloat_avx）是很容易的——

1. 升级数据类型，将__m128升级成了__m256。

2. 升级Intrinsic函数，在函数名中加入255。例如_mm_setzero_ps、_mm_load_ps、_mm_add_ps，对应的AVX版函数是 _mm256_setzero_ps、_mm256_load_ps、_mm256_add_ps。

3. 因位宽翻倍，地址计算与数据合并的代码需稍加改动。


　　当使用VC2010编译含有AVX的代码时，VC会提醒你——

warning C4752: 发现 Intel(R) 高级矢量扩展；请考虑使用 /arch:AVX


　　目前“/arch:AVX”尚未整合到项目属性的“C++\代码生成\启用增强指令集”中，需要手动在项目属性的“C++\命令行”的附加选项中加上“/arch:AVX”——

![](http://pic002.cnblogs.com/images/2012/169212/2012102222004895.png)


详见MSDN——
[http://msdn.microsoft.com/zh-cn/library/7t5yh4fd(v=vs.100).aspx](http://msdn.microsoft.com/zh-cn/library/7t5yh4fd(v=vs.100).aspx)

在 Visual Studio 中设置 /arch:AVX 编译器选项

1.打开项目的“属性页”对话框。 有关更多信息，请参见 如何：打开项目属性页。

2.单击“C/C++”文件夹。

3.单击“命令行”属性页。

4.在“附加选项”框中添加 /arch:AVX。

### 2.4.2 AVX四路循环展开版


　　同样的，我们可以编写AVX四路循环展开版——


// 单精度浮点数组求和_AVX四路循环展开版.

float sumfloat_avx_4loop(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    size_t nBlockWidth = 8*4;    // 块宽. AVX寄存器能一次处理8个float，然后循环展开4次.

    size_t cntBlock = cntbuf / nBlockWidth;    // 块数.

    size_t cntRem = cntbuf % nBlockWidth;    // 剩余数量.

    __m256 yfsSum = _mm256_setzero_ps();    // 求和变量。[AVX] 赋初值0

    __m256 yfsSum1 = _mm256_setzero_ps();

    __m256 yfsSum2 = _mm256_setzero_ps();

    __m256 yfsSum3 = _mm256_setzero_ps();

    __m256 yfsLoad;    // 加载.

    __m256 yfsLoad1;

    __m256 yfsLoad2;

    __m256 yfsLoad3;

    const float* p = pbuf;    // AVX批量处理时所用的指针.

    const float* q;    // 将AVX变量上的多个数值合并时所用指针.


    // AVX批量处理.

    for(i=0; i

    {

        yfsLoad = _mm256_load_ps(p);    // [AVX] 加载.

        yfsLoad1 = _mm256_load_ps(p+8);

        yfsLoad2 = _mm256_load_ps(p+16);

        yfsLoad3 = _mm256_load_ps(p+24);

        yfsSum = _mm256_add_ps(yfsSum, yfsLoad);    // [AVX] 单精浮点紧缩加法

        yfsSum1 = _mm256_add_ps(yfsSum1, yfsLoad1);

        yfsSum2 = _mm256_add_ps(yfsSum2, yfsLoad2);

        yfsSum3 = _mm256_add_ps(yfsSum3, yfsLoad3);

        p += nBlockWidth;

    }

    // 合并.

    yfsSum = _mm256_add_ps(yfsSum, yfsSum1);    // 两两合并(0~1).

    yfsSum2 = _mm256_add_ps(yfsSum2, yfsSum3);    // 两两合并(2~3).

    yfsSum = _mm256_add_ps(yfsSum, yfsSum2);    // 两两合并(0~3).

    q = (const float*)&yfsSum;

    s = q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7];


    // 处理剩下的.

    for(i=0; i

    {

        s += p[i];

    }


    return s;

}

## 2.5 测试框架

### 2.5.1 测试所用的数组


　　首先考虑一下测试所用的数组的长度应该是多少比较好。

　　为了避免内存带宽问题，这个数组最好能放在L1 Data Cache中。现在的处理器的L1 Data Cache一般是32KB，为了保险最好再除以2，那么数组的长度应该是 32KB/(2*sizeof(float))=4096。

　　其次考虑内存对齐问题，avx要求32字节对齐。我们可以定义一个ATTR_ALIGN宏来统一处理变量的内存对齐问题。

　　该数组定义如下——


// 变量对齐.

#ifndef ATTR_ALIGN

#  if defined(__GNUC__)    // GCC

#    define ATTR_ALIGN(n)    __attribute__((aligned(n)))

#  else    // 否则使用VC格式.

#    define ATTR_ALIGN(n)    __declspec(align(n))

#  endif

#endif    // #ifndef ATTR_ALIGN



#define BUFSIZE    4096    // = 32KB{L1 Cache} / (2 * sizeof(float))

ATTR_ALIGN(32) float buf[BUFSIZE];

### 2.5.2 测试函数


　　如果为每一个函数都编写一套测试代码，那不仅代码量大，而且不易维护。

　　可以考虑利用函数指针来实现一套测试框架。

　　因sumfloat_base等函数的签名是一致的，于是可以定义这样的一种函数指针——

// 测试时的函数类型

typedef float (*TESTPROC)(const float* pbuf, size_t cntbuf);


　　然后再编写一个对TESTPROC函数指针进行测试的函数——


// 进行测试

void runTest(const char* szname, TESTPROC proc)

{

    const int testloop = 4000;    // 重复运算几次延长时间，避免计时精度问题.

    const clock_t TIMEOUT = CLOCKS_PER_SEC/2;    // 最短测试时间.

    int i,j,k;

    clock_t    tm0, dt;    // 存储时间.

    double mps;    // M/s.

    double mps_good = 0;    // 最佳M/s. 因线程切换会导致的数值波动, 于是选取最佳值.

    volatile float n=0;    // 避免内循环被优化.

    for(i=1; i<=3; ++i)    // 多次测试.

    {

        tm0 = clock();

        // main

        k=0;

        do

        {

            for(j=1; j<=testloop; ++j)    // 重复运算几次延长时间，避免计时开销带来的影响.

            {

                n = proc(buf, BUFSIZE);    // 避免内循环被编译优化消掉.

            }

            ++k;

            dt = clock() - tm0;

        }while(dt

        // show

        mps = (double)k*testloop*BUFSIZE*CLOCKS_PER_SEC/(1024.0*1024.0*dt);    // k*testloop*BUFSIZE/(1024.0*1024.0) 将数据规模换算为M，然后再乘以 CLOCKS_PER_SEC/dt 换算为M/s .

        if (mps_good

        //printf("%s:\t%.0f M/s\t//%f\n", szname, mps, n);

    }

    printf("%s:\t%.0f M/s\t//%f\n", szname, mps_good, n);

}


　j是最内层的循环，负责多次调用TESTPROC函数指针。如果每调用一次TESTPROC函数指针后又调用clock函数，那会带来较大的计时开销，影响评测成绩。

　　k循环负责检测超时。当发现超过预定时限，便计算mps，即每秒钟处理了多少百万个单精度浮点数。然后存储最佳的mps。

　　i是最外层循环的循环变量，循环3次然后报告最佳值。

### 2.5.3 进行测试


　　在进行测试之前，需要对buf数组进行初始化，将数组元素赋随机值——

// init buf
srand( (unsigned)time( NULL ) );
for (i = 0; i < BUFSIZE; i++) 
  buf[i] = (float)(rand() & 0x3f); // 使用&0x3f是为了让求和后的数值不会超过float类型的有效位数，便于观察结果是否正确.




　　然后可以开始测试了——


// test

    runTest("sumfloat_base", sumfloat_base);    // 单精度浮点数组求和_基本版.

#ifdef INTRIN_SSE

    if (simd_sse_level(NULL) >= SIMD_SSE_1)

    {

        runTest("sumfloat_sse", sumfloat_sse);    // 单精度浮点数组求和_SSE版.

        runTest("sumfloat_sse_4loop", sumfloat_sse_4loop);    // 单精度浮点数组求和_SSE四路循环展开版.

    }

#endif    // #ifdef INTRIN_SSE

#ifdef INTRIN_AVX

    if (simd_avx_level(NULL) >= SIMD_AVX_1)

    {

        runTest("sumfloat_avx", sumfloat_avx);    // 单精度浮点数组求和_AVX版.

        runTest("sumfloat_avx_4loop", sumfloat_avx_4loop);    // 单精度浮点数组求和_AVX四路循环展开版.

    }

#endif    // #ifdef INTRIN_AVX


　INTRIN_SSE、INTRIN_AVX 宏是 zintrin.h 提供的，用于在编译时检测编译器是否支持SSE、AVX指令集。

　　simd_sse_level、simd_avx_level函数是 ccpuid.h 提供的，用于在运行时检测当前系统环境是否支持SSE、AVX指令集。

## 2.6 杂项


　　为了方便对比测试，可以在程序启动时显示程序版本、编译器名称、CPU型号信息。即在main函数中加上——


char szBuf[64];

    int i;


    printf("simdsumfloat v1.00 (�it)\n", INTRIN_WORDSIZE);

    printf("Compiler: %s\n", COMPILER_NAME);

    cpu_getbrand(szBuf);

    printf("CPU:\t%s\n", szBuf);

    printf("\n");


　　INTRIN_WORDSIZE 宏是 zintrin.h 提供的，为当前机器的字长。

　　cpu_getbrand是 ccpuid.h 提供的，用于获得CPU型号字符串。

　　COMPILER_NAME 是一个用来获得编译器名称的宏，它的详细定义是——


// Compiler name

#define MACTOSTR(x)    #x

#define MACROVALUESTR(x)    MACTOSTR(x)

#if defined(__ICL)    // Intel C++

#  if defined(__VERSION__)

#    define COMPILER_NAME    "Intel C++ " __VERSION__

#  elif defined(__INTEL_COMPILER_BUILD_DATE)

#    define COMPILER_NAME    "Intel C++ (" MACROVALUESTR(__INTEL_COMPILER_BUILD_DATE) ")"

#  else

#    define COMPILER_NAME    "Intel C++"

#  endif    // #  if defined(__VERSION__)

#elif defined(_MSC_VER)    // Microsoft VC++

#  if defined(_MSC_FULL_VER)

#    define COMPILER_NAME    "Microsoft VC++ (" MACROVALUESTR(_MSC_FULL_VER) ")"

#  elif defined(_MSC_VER)

#    define COMPILER_NAME    "Microsoft VC++ (" MACROVALUESTR(_MSC_VER) ")"

#  else

#    define COMPILER_NAME    "Microsoft VC++"

#  endif    // #  if defined(_MSC_FULL_VER)

#elif defined(__GNUC__)    // GCC

#  if defined(__CYGWIN__)

#    define COMPILER_NAME    "GCC(Cygmin) " __VERSION__

#  elif defined(__MINGW32__)

#    define COMPILER_NAME    "GCC(MinGW) " __VERSION__

#  else

#    define COMPILER_NAME    "GCC " __VERSION__

#  endif    // #  if defined(_MSC_FULL_VER)

#else

#  define COMPILER_NAME    "Unknown Compiler"

#endif    // #if defined(__ICL)    // Intel C++

# 三、全部代码

## 3.1 simdsumfloat.c


　　全部代码——


simdsumfloat.c


#define __STDC_LIMIT_MACROS    1    // C99整数范围常量. [纯C程序可以不用, 而C++程序必须定义该宏.]


#include

#include

#include


#include "zintrin.h"

#include "ccpuid.h"



// Compiler name

#define MACTOSTR(x)    #x

#define MACROVALUESTR(x)    MACTOSTR(x)

#if defined(__ICL)    // Intel C++

#  if defined(__VERSION__)

#    define COMPILER_NAME    "Intel C++ " __VERSION__

#  elif defined(__INTEL_COMPILER_BUILD_DATE)

#    define COMPILER_NAME    "Intel C++ (" MACROVALUESTR(__INTEL_COMPILER_BUILD_DATE) ")"

#  else

#    define COMPILER_NAME    "Intel C++"

#  endif    // #  if defined(__VERSION__)

#elif defined(_MSC_VER)    // Microsoft VC++

#  if defined(_MSC_FULL_VER)

#    define COMPILER_NAME    "Microsoft VC++ (" MACROVALUESTR(_MSC_FULL_VER) ")"

#  elif defined(_MSC_VER)

#    define COMPILER_NAME    "Microsoft VC++ (" MACROVALUESTR(_MSC_VER) ")"

#  else

#    define COMPILER_NAME    "Microsoft VC++"

#  endif    // #  if defined(_MSC_FULL_VER)

#elif defined(__GNUC__)    // GCC

#  if defined(__CYGWIN__)

#    define COMPILER_NAME    "GCC(Cygmin) " __VERSION__

#  elif defined(__MINGW32__)

#    define COMPILER_NAME    "GCC(MinGW) " __VERSION__

#  else

#    define COMPILER_NAME    "GCC " __VERSION__

#  endif    // #  if defined(_MSC_FULL_VER)

#else

#  define COMPILER_NAME    "Unknown Compiler"

#endif    // #if defined(__ICL)    // Intel C++



//////////////////////////////////////////////////

// sumfloat: 单精度浮点数组求和的函数

//////////////////////////////////////////////////


// 单精度浮点数组求和_基本版.

//

// result: 返回数组求和结果.

// pbuf: 数组的首地址.

// cntbuf: 数组长度.

float sumfloat_base(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    for(i=0; i

    {

        s += pbuf[i];

    }

    return s;

}


#ifdef INTRIN_SSE

// 单精度浮点数组求和_SSE版.

float sumfloat_sse(const float* pbuf, size_t cntbuf)

{

    float s = 0;    // 求和变量.

    size_t i;

    size_t nBlockWidth = 4;    // 块宽. SSE寄存器能一次处理4个float.

    size_t cntBlock = cntbuf / nBlockWidth;    // 块数.

    size_t cntRem = cntbuf % nBlockWidth;    // 剩余数量.

    __m128 xfsSum = _mm_setzero_ps();    // 求和变量。[SSE] 赋初值0

    __m128 xfsLoad;    // 加载.

    const float* p = pbuf;    // SSE批量处理时所用的指针.

    const float* q;    // 将SSE变量上的多个数值合并时所用指针.


    // SSE批量处理.

    for(i=0; i

    {

        xfsLoad = _mm_load_ps(p);    // [SSE] 加载

        xfsSum = _mm_add_ps(xfsSum, xfsLoad);    // [SSE] 单精浮点紧缩加法

        p += nBlockWidth;

    }

    // 合并.

    q = (const float*)&xfsSum;

    s = q[0] + q[1] + q[2] + q[3];


    // 处理剩下的.

    for(i=0; i

    {

        s += p[i];





