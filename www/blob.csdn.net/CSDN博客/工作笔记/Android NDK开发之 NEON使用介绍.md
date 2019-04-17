# Android NDK开发之 NEON使用介绍 - 工作笔记 - CSDN博客





2016年01月05日 14:26:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：9883








首先找到了要在C源代码中只用NEON库需要的头文件 arm_neon.h、

```cpp
#include <arm_neon.h>  
//在代码中先添加了这行语句，然后执行ndk-build 却提示了错误  
//提示要增加什么标志，自己在 LOCAL_CXX_FLAGS 的后面添加了，但是仍然报错  
//后来搜索 NDK + NEON 终于找到了一点点苗头并开始发现。  
//遂总结如下内容
```
Android.mk 文件内容可以参考这个：
[http://download.csdn.net/download/carlonelong/4153631](http://download.csdn.net/download/carlonelong/4153631)

改后的文件如下：

```cpp
LOCAL_PATH := $(call my-dir)    
include $(CLEAR_VARS)    
# 这里填写要编译的源文件路径，这里只列举了一部分    
LOCAL_SRC_FILES := NcHevcDecoder.cpp JNI_OnLoad.cpp TAppDecTop.cpp    
# 默认包含的头文件路径    
LOCAL_C_INCLUDES := \    
$(LOCAL_PATH) \    
$(LOCAL_PATH)/..    
# -g 后面的一系列附加项目添加了才能使用 arm_neon.h 头文件  <pre name="code" class="cpp">  
# -mfloat-abi=softfp -mfpu=neon 使用 arm_neon.h 必须  
LOCAL_CFLAGS := -D__cpusplus -g -mfloat-abi=softfp -mfpu=neon -march=armv7-a -mtune=cortex-a8  
LOCAL_LDLIBS := -lz -llog  
TARGET_ARCH_ABI :=armeabi-v7aLOCAL_ARM_MODE := arm    
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)    
# 采用NEON优化技术    
LOCAL_ARM_NEON := true    
endif    
LOCAL_MODULE := NcHevcDecoder  
# 生成动态调用库  
include $(BUILD_STATIC_LIBRARY)
```
同时需要修改一下Application.mk文件，其内容如下：


参考：[http://blog.csdn.net/gg137608987/article/details/7565843](http://blog.csdn.net/gg137608987/article/details/7565843)


```cpp
APP_PROJECT_PATH := $(call my-dir)/..    
APP_PLATFORM := android-10    
APP_STL := stlport_static    
APP_ABI := armeabi-v7a    
APP_CPPFLAGS += -fexceptions
```
其中APP_ABI这句指定了编译的目标平台类型，可以针对不同平台进行优化。    当然这样指定了之后，就需要相应的设备支持NEON指令。    


网上有一个用NEON优化YUV转RGB的NEON优化例子，可以参见：[](http://hilbert-space.de/?p=22)

[http://hilbert-space.de/?p=22](http://hilbert-space.de/?p=22)


这里摘录一下其优化过程：

1、原始代码

```cpp
void reference_convert (uint8_t * __restrict dest, uint8_t * __restrict src, int n)    
    {    
      int i;    
      for (i=0; i<n; i++)    
      {    
        int r = *src++; // load red    
        int g = *src++; // load green    
        int b = *src++; // load blue     
       
        // build weighted average:    
        int y = (r*77)+(g*151)+(b*28);    
       
        // undo the scale by 256 and write to memory:    
        *dest++ = (y>>8);    
      }    
    }
```
2、使用NEON库进行代码优化   

Since NEON works in 64 or 128 bit registers it’s best to process eight pixels in parallel.<br>  

 That way we can exploit the parallel nature of the SIMD-unit. Here is what I came up with:  

因为NEON工作在64位或128位的寄存器上，因此最适合同时处理8个像素点的转换。

这样就形成了下面这样的代码  

```cpp
void neon_convert (uint8_t * __restrict dest, uint8_t * __restrict src, int n)    
    {    
      int i;    
      uint8x8_t rfac = vdup_n_u8 (77);       // 转换权值  R    
      uint8x8_t gfac = vdup_n_u8 (151);    // 转换权值  G    
      uint8x8_t bfac = vdup_n_u8 (28);      // 转换权值  B    
      n/=8;    
       
      for (i=0; i<n; i++)    
      {    
        uint16x8_t  temp;    
        uint8x8x3_t rgb  = vld3_u8 (src);    
        uint8x8_t result;    
       
        temp = vmull_u8 (rgb.val[0],      rfac);       // vmull_u8 每个字节（8bit）对应相乘，结果为每个单位2字节（16bit）    
        temp = vmlal_u8 (temp,rgb.val[1], gfac);  // 每个比特对应相乘并加上    
        temp = vmlal_u8 (temp,rgb.val[2], bfac);    
       
        result = vshrn_n_u16 (temp, 8);  // 全部移位8位    
        vst1_u8 (dest, result);   // 转存运算结果    
        src  += 8*3;     
        dest += 8;    
      }    
    }
```
vmull.u8 multiplies each byte of the first argument with each corresponding byte of the second argument. Each result becomes a 16 bit unsigned integer, so no overflow can happen. The entire result is returned as a 128 bit NEON register pair.
  vmlal.u8 does the same thing as vmull.u8 but also adds the content of another register to the result.  

So we end up with just three instructions for weighted average of eight pixels. Nice.  

Now it’s time to undo the scaling of the weight factors. To do so I shift each 16 bit result to the right by 8 bits. This equals to a division by 256. ARM NEON has lots of instructions to do the shift, but also a “narrow” variant exists. This one does two  

 things at once: It does the shift and afterwards converts the 16 bit integers back to 8 bit by removing all the high-bytes from the result. We get back from the 128 bit register pair to a single 64 bit register.

3、结果对比


（1）C语言NEON版本汇编

```cpp
/* 
    未进行汇编优化的结果 
    C-version:           15.1 cycles per pixel. 
    NEON-version:     9.9 cycles per pixel. 
    这里是说优化结果并不非常理想，所以查看了一下它的汇编文件 
    That’s only a speed-up of factor 1.5. I expected much more from the NEON implementation. It processes 8 pixels with just 6 instructions after all.   
    What’s going on here? A look at the assembler output explained it all. Here is the inner-loop part of the convert function: 
    */    
     160:   f46a040f        vld3.8  {d16-d18}, [sl]    
     164:   e1a0c005        mov     ip, r5    
     168:   ecc80b06        vstmia  r8, {d16-d18}    
     16c:   e1a04007        mov     r4, r7    
     170:   e2866001        add     r6, r6, #1      ; 0x1    
     174:   e28aa018        add     sl, sl, #24     ; 0x18    
     178:   e8bc000f        ldm     ip!, {r0, r1, r2, r3}    
     17c:   e15b0006        cmp     fp, r6    
     180:   e1a08005        mov     r8, r5    
     184:   e8a4000f        stmia   r4!, {r0, r1, r2, r3}    
     188:   eddd0b06        vldr    d16, [sp, #24]    
     18c:   e89c0003        ldm     ip, {r0, r1}    
     190:   eddd2b08        vldr    d18, [sp, #32]    
     194:   f3c00ca6        vmull.u8        q8, d16, d22    
     198:   f3c208a5        vmlal.u8        q8, d18, d21    
     19c:   e8840003        stm     r4, {r0, r1}    
     1a0:   eddd3b0a        vldr    d19, [sp, #40]    
     1a4:   f3c308a4        vmlal.u8        q8, d19, d20    
     1a8:   f2c80830        vshrn.i16       d16, q8, #8    
     1ac:   f449070f        vst1.8  {d16}, [r9]    
     1b0:   e2899008        add     r9, r9, #8      ; 0x8    
     1b4:   caffffe9        bgt     160
```
（2）NEON汇编优化

Since the compiler can’t generate good code I wrote the same loop in assembler.   

In a nutshell I just took the intrinsic based loop and converted the instructions one by one. The loop-control is a bit different, but that’s all.  

```cpp
// 这里针对生成的目标汇编代码进一步作了优化，优化的代码如下：    
     convert_asm_neon:    
       
          # r0: Ptr to destination data    
          # r1: Ptr to source data    
          # r2: Iteration count:    
       
            push        {r4-r5,lr}    
          lsr         r2, r2, #3    
       
          # build the three constants:    
          mov         r3, #77    
          mov         r4, #151    
          mov         r5, #28    
          vdup.8      d3, r3    
          vdup.8      d4, r4    
          vdup.8      d5, r5    
       
      .loop:    
       
          # load 8 pixels:    
          vld3.8      {d0-d2}, [r1]!    
       
          # do the weight average:    
          vmull.u8    q3, d0, d3    
          vmlal.u8    q3, d1, d4    
          vmlal.u8    q3, d2, d5    
       
          # shift and store:    
          vshrn.u16   d6, q3, #8    
          vst1.8      {d6}, [r0]!    
       
          subs        r2, r2, #1    
          bne         .loop    
       
          pop         { r4-r5, pc }
```
可以见到NEON优化在性能上提速了 7 倍多（同时处理8个像素），理论应该是8倍。
            


