# YUV视频格式到RGB32格式转换的速度优化 中篇 - gauss的专栏 - CSDN博客
2012年02月26日 00:24:59[gauss](https://me.csdn.net/mathlmx)阅读数：315标签：[优化																[parallel																[list																[byte																[测试																[callback](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=parallel&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)
个人分类：[图形与图像处理](https://blog.csdn.net/mathlmx/article/category/605907)
tag: YUV,YCbCr,YUV到RGB颜色转换,YUV解码,VFW,视频,MMX,SSE,多核优化
摘要: 我们得到的很多视频数据(一些解码器的输出或者摄像头的输出等)都使用了一种
叫YUV的颜色格式；本文介绍了常见的YUV视频格式(YUY2/YVYU/UYVY/I420/YV12等)到
RGB颜色格式的转换,并尝试对转化的速度进行优化； 
  全文 分为:    
    《上篇》文章首先介绍了YUV颜色格式，并介绍了YUV颜色格式和RGB颜色格式之
间的相互转换；然后重点介绍了YUYV视频格式到RGB32格式的转化，并尝试进行了一
些速度优化；
    《中篇》尝试使用MMX/SSE指令对前面实现的解码器核心进行速度优化；然
后简要介绍了一个使用这类CPU特殊指令时的代码框架，使得解码程序能够根据运行时
的CPU指令支持情况动态调用最佳的实现代码；并最终提供一个多核并行的优化版本；
    《下篇》介绍YUV类型的其他种类繁多的视频数据编码格式；并将前面实现的解码
器核心(在不损失代码速度的前提下)进行必要的修改，使之适用于这些YUV视频格式
的解码；
    (2007.11.25  优化了一下DECODE_YUYV_SSE，使用预读优化;调整了一下MMX指令的定义方式，结构更好一些) 
    (2007.11.13  修正了一下颜色转换公式中的系数) 
    (2007.11.05  修改函数DECODE_YUYV_AutoEx中的一个bug)
正文:
  代码使用C++,编译器:VC2005
  涉及到汇编的时候假定为x86平台；
  现在的高清视频帧尺寸越来越大，所以本文测试的图片大小将使用1024x576和
1920x1080两种常见的帧尺寸来测试解码器速度；
  测试平台:(CPU:AMD64x2 4200+(2.37G);   内存:DDR2 677(双通道); 编译器:VC2005)
  测试平台:(CPU:Intel Core2 4400(2.00G);内存:DDR2 667(双通道); 编译器:VC2005)
  请先参看《YUV视频格式到RGB32格式转换的速度优化 上篇》，
本文章将继续成倍的提高其速度!
A:使用MMX指令来继续优化YUYV视频格式到RGB32格式的转换函数
  现在绝大多数的x86CPU都支持MMX指令，它是一种单指令多数据流的指令集(SIMD)；
MMX指令能够同时操作8个byte或者4个short等; 在YUV转换到RGB的运算中,为了保持精
度选用一次运算4个short数据类型；那外考虑如果在一个寄存器中保存YUYV四个数据，
整个运算写起来会较麻烦，且算法受到Y、U、V三个颜色存放位置的影响严重；而且考
虑到除了packed模式外很多YUV视频数据都为planar模式，所以想把Y、U、V先分离到
各自的寄存器再运算，看起来舒服得多，那么运算核心将用一个寄存器保存4个U，一
个寄存器保存4个V，对应8个Y，也就是说核心转换代码运行一遍可以输出8个RGB32比
特颜色；
  所以我们先来实现一个通用的MMX实现的转换核心：
  我们约定输入:  
        mm0 = Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 
        mm1 = 00 u3 00 u2 00 u1 00 u0 
        mm2 = 00 v3 00 v2 00 v1 00 v0   
  通过edx指向的内存输出:
        [edx -- edx+8*4]
  由于主要的计算使用short精度，那些系数就不能使用16位的定点数了；为了不超出short的
范围可以使用13位的定点数(再大就会溢出了)；
  MMX实现的转换核心(使用了宏来实现)：YUV422ToRGB32_MMX:
  (系数的由来/数据在MMX寄存器的大致流动都有较详细的注释；
      如果有人进一步改进了这个核心,请告诉我:) 
typedef unsigned __int64  UInt64;
const  UInt64   csMMX_16_b      =0x1010101010101010; // byte{16,16,16,16,16,16,16,16}
const  UInt64   csMMX_128_w     =0x0080008000800080; //short{  128,  128,  128,  128}
const  UInt64   csMMX_0x00FF_w  =0x00FF00FF00FF00FF; //掩码
const  UInt64   csMMX_Y_coeff_w =0x2543254325432543; //short{ 9539, 9539, 9539, 9539} =1.164383*(1<<13)
const  UInt64   csMMX_U_blue_w  =0x408D408D408D408D; //short{16525,16525,16525,16525} =2.017232*(1<<13)
const  UInt64   csMMX_U_green_w =0xF377F377F377F377; //short{-3209,-3209,-3209,-3209} =(-0.391762)*(1<<13)
const  UInt64   csMMX_V_green_w =0xE5FCE5FCE5FCE5FC; //short{-6660,-6660,-6660,-6660} =(-0.812968)*(1<<13)
const  UInt64   csMMX_V_red_w   =0x3313331333133313; //short{13075,13075,13075,13075} =1.596027*(1<<13)
//一次处理8个颜色输出
#define YUV422ToRGB32_MMX(out_RGB_reg,WriteCode)                                                 /
/*input :  mm0 = Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0    */                                                /
/*         mm1 = 00 u3 00 u2 00 u1 00 u0    */                                                /
/*         mm2 = 00 v3 00 v2 00 v1 00 v0    */                                                /
/*output : [out_RGB_reg -- out_RGB_reg+8*4]                 */                                /
                                                                                                 /
          asm   psubusb     mm0,csMMX_16_b        /* mm0 : Y -= 16                       */      /
          asm   psubsw      mm1,csMMX_128_w       /* mm1 : u -= 128                      */      /
          asm   movq        mm7,mm0                                                              /
          asm   psubsw      mm2,csMMX_128_w       /* mm2 : v -= 128                      */      /
          asm   pand        mm0,csMMX_0x00FF_w    /* mm0 = 00 Y6 00 Y4 00 Y2 00 Y0       */      /
          asm   psllw       mm1,3/* mm1 : u *= 8                        */      /
          asm   psllw       mm2,3/* mm2 : v *= 8                        */      /
          asm   psrlw       mm7,8/* mm7 = 00 Y7 00 Y5 00 Y3 00 Y1       */      /
          asm   movq        mm3,mm1                                                              /
          asm   movq        mm4,mm2                                                              /
                                                                                                 /
          asm   pmulhw      mm1,csMMX_U_green_w   /* mm1 = u * U_green                   */      /
          asm   psllw       mm0,3/* y*=8                                */      /
          asm   pmulhw      mm2,csMMX_V_green_w   /* mm2 = v * V_green                   */      /
          asm   psllw       mm7,3/* y*=8                                */      /
          asm   pmulhw      mm3,csMMX_U_blue_w                                                   /
          asm   paddsw      mm1,mm2                                                              /
          asm   pmulhw      mm4,csMMX_V_red_w                                                    /
          asm   movq        mm2,mm3                                                              /
          asm   pmulhw      mm0,csMMX_Y_coeff_w                                                  /
          asm   movq        mm6,mm4                                                              /
          asm   pmulhw      mm7,csMMX_Y_coeff_w                                                  /
          asm   movq        mm5,mm1                                                              /
          asm   paddsw      mm3,mm0               /* mm3 = B6 B4 B2 B0       */                  /
          asm   paddsw      mm2,mm7               /* mm2 = B7 B5 B3 B1       */                  /
          asm   paddsw      mm4,mm0               /* mm4 = R6 R4 R2 R0       */                  /
          asm   paddsw      mm6,mm7               /* mm6 = R7 R5 R3 R1       */                  /
          asm   paddsw      mm1,mm0               /* mm1 = G6 G4 G2 G0       */                  /
          asm   paddsw      mm5,mm7               /* mm5 = G7 G5 G3 G1       */                  /
                                                                                                 /
          asm   packuswb    mm3,mm4               /* mm3 = R6 R4 R2 R0 B6 B4 B2 B0 to [0-255] */ /
          asm   packuswb    mm2,mm6               /* mm2 = R7 R5 R3 R1 B7 B5 B3 B1 to [0-255] */ /
          asm   packuswb    mm5,mm1               /* mm5 = G6 G4 G2 G0 G7 G5 G3 G1 to [0-255] */ /
          asm   movq        mm4,mm3                                                              /
          asm   punpcklbw   mm3,mm2               /* mm3 = B7 B6 B5 B4 B3 B2 B1 B0     */        /
          asm   punpckldq   mm1,mm5               /* mm1 = G7 G5 G3 G1 xx xx xx xx     */        /
          asm   punpckhbw   mm4,mm2               /* mm4 = R7 R6 R5 R4 R3 R2 R1 R0     */        /
          asm   punpckhbw   mm5,mm1               /* mm5 = G7 G6 G5 G4 G3 G2 G1 G0     */        /
                                                                                                 /
/*out*/                                                                          /
          asm   pcmpeqb     mm2,mm2               /* mm2 = FF FF FF FF FF FF FF FF     */        /
                                                                                                 /
          asm   movq        mm0,mm3                                                              /
          asm   movq        mm7,mm4                                                              /
          asm   punpcklbw   mm0,mm5             /* mm0 = G3 B3 G2 B2 G1 B1 G0 B0       */        /
          asm   punpcklbw   mm7,mm2             /* mm7 = FF R3 FF R2 FF R1 FF R0       */        /
          asm   movq        mm1,mm0                                                              /
          asm   movq        mm6,mm3                                                              /
          asm   punpcklwd   mm0,mm7             /* mm0 = FF R1 G1 B1 FF R0 G0 B0       */        /
          asm   punpckhwd   mm1,mm7             /* mm1 = FF R3 G3 B3 FF R2 G2 B2       */        /
          asm   WriteCode   [out_RGB_reg],mm0                                                    /
          asm   movq        mm7,mm4                                                              /
          asm   punpckhbw   mm6,mm5             /* mm6 = G7 B7 G6 B6 G5 B5 G4 B4       */        /
          asm   WriteCode   [out_RGB_reg+8],mm1                                                  /
          asm   punpckhbw   mm7,mm2             /* mm7 = FF R7 FF R6 FF R5 FF R4      */         /
          asm   movq        mm0,mm6                                                              /
          asm   punpcklwd   mm6,mm7             /* mm6 = FF R5 G5 B5 FF R4 G4 B4      */         /
          asm   punpckhwd   mm0,mm7             /* mm0 = FF R7 G7 B7 FF R6 G6 B6      */         /
          asm   WriteCode  [out_RGB_reg+8*2],mm6                                                 /
          asm   WriteCode  [out_RGB_reg+8*3],mm0                       
  YUV视频格式到RGB32格式的转换函数，MMX指令实现版本
#define YUYV_Loader_MMX(in_yuv_reg)                                                         /
          asm   movq        mm0,[in_yuv_reg  ] /*mm0=V1 Y3 U1 Y2 V0 Y1 U0 Y0  */                /
          asm   movq        mm4,[in_yuv_reg+8] /*mm4=V3 Y7 U3 Y6 V2 Y5 U2 Y4  */                /
          asm   movq        mm1,mm0                                                             /
          asm   movq        mm5,mm4                                                             /
          asm   psrlw       mm1,8/*mm1=00 V1 00 U1 00 V0 00 U0  */                /
          asm   psrlw       mm5,8/*mm5=00 V3 00 U3 00 V2 00 U2  */                /
          asm   pand        mm0,csMMX_0x00FF_w /*mm0=00 Y3 00 Y2 00 Y1 00 Y0  */                /
          asm   pand        mm4,csMMX_0x00FF_w /*mm4=00 Y7 00 Y6 00 Y5 00 Y4  */                /
          asm   packuswb    mm1,mm5            /*mm1=V3 U3 V2 U2 V1 U1 V0 U0  */                /
          asm   movq        mm2,mm1                                                             /
          asm   packuswb    mm0,mm4            /*mm0=Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0  */                /
          asm   psllw       mm1,8/*mm1=U3 00 U2 00 U1 00 U0 00  */                /
          asm   psrlw       mm2,8/*mm2=00 V3 00 V2 00 V1 00 V0  */                /
          asm   psrlw       mm1,8/*mm1=00 U3 00 U2 00 U1 00 U0  */
void DECODE_YUYV_MMX_line(TARGB32* pDstLine,const TUInt8* pYUYV,long width)
    {
long expand8_width=(width>>3)<<3;
if (expand8_width>0)
        {
            asm
            {
                mov     ecx,expand8_width
                mov     eax,pYUYV
                mov     edx,pDstLine
                lea     eax,[eax+ecx*2]
                lea     edx,[edx+ecx*4]
                neg     ecx
              loop_beign:
                YUYV_Loader_MMX(eax+ecx*2)
                YUV422ToRGB32_MMX(edx+ecx*4,movq)
                add     ecx,8
                jnz     loop_beign
                mov     pYUYV,eax
                mov     pDstLine,edx
            }
        }
//处理边界
        DECODE_YUYV_Common_line(pDstLine,pYUYV,width-expand8_width);
    }
void DECODE_YUYV_MMX(const TUInt8* pYUYV,const TPicRegion& DstPic)
{
    assert((DstPic.width &1)==0); 
long YUV_byte_width=(DstPic.width>>1)<<2;
    TARGB32* pDstLine=DstPic.pdata; 
for (long y=0;y<DstPic.height;++y)
    {
        DECODE_YUYV_MMX_line(pDstLine,pYUYV,DstPic.width);
        pYUYV+=YUV_byte_width;
        ((TUInt8*&)pDstLine)+=DstPic.byte_width;
    }    
    asm emms
}
速度测试:
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
//                       |        1024x576       |       1920x1080       |
//------------------------------------------------------------------------------
//                       |  AMD64x2  |   Core2   |  AMD64x2  |   Core2   |
//------------------------------------------------------------------------------
//DECODE_YUYV_MMX          585.4 FPS  569.8 FPS    169.8 FPS  160.4 FPS
////////////////////////////////////////////////////////////////////////////////
B.使用SSE中的软件预取和禁止写缓存来改进MMX版本
  这里的改动其实很小，只是把YUV422ToRGB32_MMX中颜色数据保存操作
    movq [mem],mmx_reg 修改成 movntq [mem],mmx_reg
  然后再处理完成后调用sfence缓存刷新指令。
  完整代码如下:
//使用软件预读和禁止写缓存优化
#define  YUV422ToRGB32_SSE(out_RGB_reg) YUV422ToRGB32_MMX(out_RGB_reg,movntq)
void DECODE_YUYV_SSE_line(TARGB32* pDstLine,const TUInt8* pYUYV,long width)
    {
long expand8_width=(width>>3)<<3;
if (expand8_width>0)
        {
            asm
            {
                mov     ecx,expand8_width
                mov     eax,pYUYV
                mov     edx,pDstLine
                lea     eax,[eax+ecx*2]
                lea     edx,[edx+ecx*4]
                neg     ecx
              loop_beign:
                YUYV_Loader_MMX(eax+ecx*2)
                prefetchnta [eax+ecx*2+64*4]  //预读
                YUV422ToRGB32_SSE(edx+ecx*4)
                add     ecx,8
                jnz     loop_beign
                mov     pYUYV,eax
                mov     pDstLine,edx
            }
        }
//处理边界
        DECODE_YUYV_Common_line(pDstLine,pYUYV,width-expand8_width);
    }
void DECODE_YUYV_SSE(const TUInt8* pYUYV,const TPicRegion& DstPic)
{
    assert((DstPic.width &1)==0); 
long YUV_byte_width=(DstPic.width>>1)<<2;
    TARGB32* pDstLine=DstPic.pdata; 
for (long y=0;y<DstPic.height;++y)
    {
        DECODE_YUYV_SSE_line(pDstLine,pYUYV,DstPic.width);
        pYUYV+=YUV_byte_width;
        ((TUInt8*&)pDstLine)+=DstPic.byte_width;
    } 
    asm sfence
    asm emms
}
速度测试:
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
//                       |        1024x576       |       1920x1080       |
//------------------------------------------------------------------------------
//                       |  AMD64x2  |   Core2   |  AMD64x2  |   Core2   |
//------------------------------------------------------------------------------
//DECODE_YUYV_SSE          770.3 FPS    741.9 FPS   220.0 FPS   209.7 FPS
////////////////////////////////////////////////////////////////////////////////
C.使用CPU特殊指令的一般框架
  我的blog文章中，经常使用MMX/SSE等特殊指令，都是只给出代码实现；它们离实际项目代
码还有一点距离；在实际的项目中需要一种机制使得开发的软件能够根据运行的CPU的特性动
态的决定调用最优化的实现版本；
  在x86CPU上可以使用CPUID指令来得到各种关于当前CPU的特性，包括制造商、CPU家族号、
缓存信息、是否支持MMX/SSE/SSE2指令集 等等；
  要使用CPUID指令，首先应该判断CPU是否支持该指令；方法是判断EFLAGS寄存器的第21位
是否可以改写；如果可以改写，那么说明这块CPU支持CPUID指令； 函数实现如下:
bool  _CPUSupportCPUID()
    {
longint CPUIDInfOld=0;
longint CPUIDInfNew=0;
try
      {
        asm
        {
            pushfd                   // 保存原 EFLAGS
            pop     eax
            mov     edx,eax
            mov     CPUIDInfOld,eax  //
            xor     eax,00200000h    // 改写 第21位
            push    eax
            popfd                    // 改写 EFLAGS
            pushfd                   // 保存新 EFLAGS
            pop     eax              
            mov     CPUIDInfNew,eax
            push    edx              // 恢复原 EFLAGS
            popfd
        }
return (CPUIDInfOld!=CPUIDInfNew);  // EFLAGS 第21位 可以改写
      }
catch(...)
      {
returnfalse;
      }
    }
  //那么判断CPU是否支持MMX指令的函数如下:
bool  _CPUSupportMMX()  //判断CPU是否支持MMX指令
    {
if (!_CPUSupportCPUID())
returnfalse;
longint MMXInf=0;
try
      {
        asm
     {
          push  ebx
          mov   eax,1
          cpuid
          mov   MMXInf,edx
          pop   ebx
        }
        MMXInf=MMXInf & (1<<23);  //检测edx第23位
return (MMXInf==(1<<23));
      }
catch(...)
      {
returnfalse;
      }
    }
  //判断CPU是否支持SSE指令的函数如下:
bool  _CPUSupportSSE()  //判断CPU是否支持SSE指令
    {
if (!_CPUSupportCPUID())
returnfalse;
longint SSEInf=0;
try
      {
        asm
     {
          push  ebx
          mov   eax,1
          cpuid
          mov   SSEInf,edx
          pop   ebx
        }
        SSEInf=SSEInf & (1<<25);  //检测edx第25位
return  (SSEInf==(1<<25));
      }
catch(...)
      {
returnfalse;
      }
    }
  //  由于SSE的寄存器是比较后期加入的，某些较老的操作系统可能不支持这些寄存器
  //的任务切换保存；可以用触发异常的方式来判断操作系统是否支持SSE;
bool  _SystemSupportSSE()  //判断操作系统是否支持SSE指令
    {
//触发异常来判断
try
      {
        asm
        {
//movups     xmm0,xmm0
            asm _emit 0x0F asm _emit 0x10 asm _emit 0xC0
        }
returntrue;
      }
catch(...)
      {
returnfalse;
      }
    }
//定义常量，用以在程序作为分支条件
constbool _IS_MMX_ACTIVE=_CPUSupportMMX();
constbool _IS_SSE_ACTIVE=_CPUSupportSSE() && _SystemSupportSSE();
D.根据运行的CPU支持的指令集来动态调用不同的解码器实现
typedef void (*TDECODE_YUYV_line_proc)(TARGB32* pDstLine,const TUInt8* pYUYV,long width);
const TDECODE_YUYV_line_proc DECODE_YUYV_Auto_line=
        ( _IS_MMX_ACTIVE ? (_IS_SSE_ACTIVE ? DECODE_YUYV_SSE_line : DECODE_YUYV_MMX_line) : DECODE_YUYV_Common_line );
__forceinline void DECODE_filish()
{
if (_IS_MMX_ACTIVE)
    {
if (_IS_SSE_ACTIVE) {  asm sfence }
        asm emms
    }
}
void DECODE_YUYV_Auto(const TUInt8* pYUYV,const TPicRegion& DstPic)
{
    assert((DstPic.width &1)==0); 
long YUV_byte_width=(DstPic.width>>1)<<2;
    TARGB32* pDstLine=DstPic.pdata; 
for (long y=0;y<DstPic.height;++y)
    {
        DECODE_YUYV_Auto_line(pDstLine,pYUYV,DstPic.width);
        pYUYV+=YUV_byte_width;
        ((TUInt8*&)pDstLine)+=DstPic.byte_width;
    } 
    DECODE_filish();
}
在我的两台测试电脑上速度同DECODE_YUYV_SSE，因为它们都支持MMX和SSE;
E.YUYV视频格式解码器的并行化实现
  这个比较简单，将图像分为多个块交给多个CPU同时执行就可以了；代码如下:
  (  这里利用CWorkThreadPool类来并行执行任务;  参见我的Blog文
章《并行计算简介和多核CPU编程Demo》,里面有CWorkThreadPool类的完整源代码)
#include "WorkThreadPool.h"
struct TDECODE_YUYV_Parallel_WorkData
{
const TUInt8* pYUYV;
    TPicRegion    DstPic;
};
void DECODE_YUYV_Parallel_callback(void* wd)
{
    TDECODE_YUYV_Parallel_WorkData* WorkData=(TDECODE_YUYV_Parallel_WorkData*)wd;
    DECODE_YUYV_Auto(WorkData->pYUYV,WorkData->DstPic);
}
void DECODE_YUYV_Parallel(const TUInt8* pYUYV,const TPicRegion& DstPic)
{
long work_count=CWorkThreadPool::best_work_count();
    std::vector<TDECODE_YUYV_Parallel_WorkData>   work_list(work_count);
    std::vector<TDECODE_YUYV_Parallel_WorkData*>  pwork_list(work_count);
long cheight=DstPic.height / work_count; 
for (long i=0;i<work_count;++i)
    {
        work_list[i].pYUYV=pYUYV+i*cheight*(DstPic.width*2);
        work_list[i].DstPic.pdata=DstPic.pixel_pos(0,cheight*i);
        work_list[i].DstPic.byte_width=DstPic.byte_width;
        work_list[i].DstPic.width=DstPic.width;
        work_list[i].DstPic.height=cheight;
        pwork_list[i]=&work_list[i];
    }
    work_list[work_count-1].DstPic.height=DstPic.height-cheight*(work_count-1);
    CWorkThreadPool::work_execute(DECODE_YUYV_Parallel_callback,(void**)&pwork_list[0],work_count);
}
速度测试:
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
//                       |        1024x576       |       1920x1080       |
//------------------------------------------------------------------------------
//                       |  AMD64x2  |   Core2   |  AMD64x2  |   Core2   |
//------------------------------------------------------------------------------
//DECODE_YUYV_Parallel    1433.9 FPS  1417.1 FPS    414.1 FPS  286.3 FPS
////////////////////////////////////////////////////////////////////////////////
F.另一种更灵活的任务分配方案
  我的Blog文章中，涉及到并行的时候，一般都是前面那种简单的平均任务分配模式；
这里再实现一种复杂一点的分配方案：线程执行完自己分配的任务后，尝试帮助其它
线程执行没有完成的任务；(以单行为最小可分配任务粒度)； 这有一个优点，就是
在多任务环境下，能够更好地利用全部的CPU资源; 实现如下:
    __forceinline void DECODE_YUYV_AutoLock_line(TARGB32* pDstLine,const TUInt8* pYUYV,long width,volatilelong*  Lock)
    {
//任务领取
if ((*Lock)!=0) return;
long lock_value=InterlockedIncrement(Lock);//也可以用带lock前缀的inc指令来代替这个windows调用 
//警告: 在以后更多个核的电脑上，这里的lock造成的潜在冲突没有测试过
if (lock_value>=2) return;
//lock_value==1时，任务领取成功
//执行任务
        DECODE_YUYV_Auto_line(pDstLine,pYUYV,width);
    }
    __forceinline void DECODE_YUYV_AutoEx(const TUInt8* pYUYV,const TPicRegion& DstPic,volatilelong* LockList,long  begin_y0)
    {
        assert((DstPic.width &1)==0); 
long YUV_byte_width=(DstPic.width>>1)<<2;
        TARGB32* pDstLine=DstPic.pdata; 
long y;
const TUInt8* pYUYV_b=pYUYV+(YUV_byte_width*begin_y0);
        TARGB32* pDstLine_b=(TARGB32*)(((TUInt8*)DstPic.pdata)+(DstPic.byte_width*begin_y0));
for (y=begin_y0;y<DstPic.height;++y)
        {
            DECODE_YUYV_AutoLock_line(pDstLine_b,pYUYV_b,DstPic.width,&LockList[y]);
            pYUYV_b+=YUV_byte_width;
            ((TUInt8*&)pDstLine_b)+=DstPic.byte_width;
        } 
for (y=0;y<begin_y0;++y)
        {
            DECODE_YUYV_AutoLock_line(pDstLine,pYUYV,DstPic.width,&LockList[y]);
            pYUYV+=YUV_byte_width;
            ((TUInt8*&)pDstLine)+=DstPic.byte_width;
        } 
        DECODE_filish();
    }
struct TDECODE_YUYV_ParallelEx_WorkData
{
const TUInt8*   pYUYV;
    TPicRegion      DstPic;
long*           LockList;
long            begin_y0;
};
void DECODE_YUYV_ParallelEx_callback(void* wd)
{
    TDECODE_YUYV_ParallelEx_WorkData* WorkData=(TDECODE_YUYV_ParallelEx_WorkData*)wd;
    DECODE_YUYV_AutoEx(WorkData->pYUYV,WorkData->DstPic,(volatilelong*)WorkData->LockList,WorkData->begin_y0);
}
void DECODE_YUYV_ParallelEx(const TUInt8* pYUYV,const TPicRegion& DstPic)
{
long work_count=CWorkThreadPool::best_work_count();
    std::vector<TDECODE_YUYV_ParallelEx_WorkData>   work_list(work_count);
    std::vector<TDECODE_YUYV_ParallelEx_WorkData*>  pwork_list(work_count);
    std::vector<long>  lock_list(DstPic.height);
for (long y=0;y<DstPic.height;++y)
        lock_list[y]=0;
long cheight=DstPic.height / work_count; 
for (long i=0;i<work_count;++i)
    {
        work_list[i].pYUYV=pYUYV;
        work_list[i].DstPic=DstPic;
        work_list[i].begin_y0=i*cheight;
        work_list[i].LockList=&lock_list[0];
        pwork_list[i]=&work_list[i];
    }
    CWorkThreadPool::work_execute(DECODE_YUYV_ParallelEx_callback,(void**)&pwork_list[0],work_count);
}
速度测试:
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
//                       |        1024x576       |       1920x1080       |
//------------------------------------------------------------------------------
//                       |  AMD64x2  |   Core2   |  AMD64x2  |   Core2   |
//------------------------------------------------------------------------------
//DECODE_YUYV_ParallelEx  1387.5 FPS   1359.2 FPS    409.9 FPS  287.4 FPS
////////////////////////////////////////////////////////////////////////////////
G:把测试成绩放在一起
////////////////////////////////////////////////////////////////////////////////
//测试平台:(CPU:AMD64x2 4200+(2.37G);   内存:DDR2 677(双通道); 编译器:VC2005)
//测试平台:(CPU:Intel Core2 4400(2.00G);内存:DDR2 667(双通道); 编译器:VC2005)
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
//                       |        1024x576       |       1920x1080       |
//------------------------------------------------------------------------------
//                       |  AMD64x2  |   Core2   |  AMD64x2  |   Core2   |
//------------------------------------------------------------------------------
//DECODE_YUYV_Float         55.0 FPS   63.7 FPS     15.6 FPS   18.0 FPS
//DECODE_YUYV_Int          137.1 FPS  131.9 FPS     39.0 FPS   37.1 FPS
//DECODE_YUYV_RGBTable     164.8 FPS  152.9 FPS     47.1 FPS   43.7 FPS
//DECODE_YUYV_Table        146.1 FPS  151.3 FPS     41.8 FPS   43.5 FPS
//DECODE_YUYV_TableEx      236.5 FPS  300.5 FPS     68.1 FPS   85.0 FPS
//DECODE_YUYV_Common       250.7 FPS  287.1 FPS     71.9 FPS   80.7 FPS
//DECODE_YUYV_MMX          585.4 FPS  569.8 FPS    169.8 FPS  160.4 FPS
//DECODE_YUYV_SSE          770.3 FPS  741.9 FPS    220.0 FPS  209.7 FPS
//DECODE_YUYV_Auto                    (同DECODE_YUYV_SSE)
//DECODE_YUYV_Parallel    1433.9 FPS 1417.1 FPS    414.1 FPS  286.3 FPS
//DECODE_YUYV_ParallelEx  1387.5 FPS 1359.2 FPS    409.9 FPS  287.4 FPS
////////////////////////////////////////////////////////////////////////////////
 (欢迎提出不足和改进意见；文章下篇将开始支持更多类型的YUV视频数据格式)
