# RGB和YCbCr颜色空间的转换及优化算法 - Popeye_the_Sailor - CSDN博客
2017年10月14日 19:31:45[_Sailor_](https://me.csdn.net/lz0499)阅读数：556
## RGB和YCbCr颜色空间转换和优化
转载于[颜色空间系列3: RGB和YCbCr颜色空间的转换及优化算法](http://www.cnblogs.com/Imageshop/archive/2013/02/14/2911309.html)
   在常用的几种颜色空间中，YCbCr颜色空间在学术论文中出现的频率是相当高的，常用于肤色检测等等。其和RGB空间之间的相互转换公式在网上也有多种，我们这里取[http://en.wikipedia.org/wiki/YCbCr](http://en.wikipedia.org/wiki/YCbCr) 描述的JPG转换时使用的计算公式：
### 　　JPEG conversion
　　JFIF usage of JPEG allows Y′CbCr where Y′, CB and CR have the full 8-bit range of 0-255:[](http://en.wikipedia.org/wiki/YCbCr#cite_note-3)
![](https://img-blog.csdn.net/20171014193550000)
　　And back:
![](https://img-blog.csdn.net/20171014193554061)
     上述公式的主要优点是转换后的各分量的范围也在0到255之间，因此用 byte类型的变量即可容纳新的颜色空间。
     为了可以指定位移的大小，我们采用常量的定义方式是计算各放大系数。
```cpp
const float YCbCrYRF = 0.299F;              // RGB转YCbCr的系数(浮点类型）
        const float YCbCrYGF = 0.587F;
        const float YCbCrYBF = 0.114F;
        const float YCbCrCbRF = -0.168736F;        
        const float YCbCrCbGF = -0.331264F;
        const float YCbCrCbBF = 0.500000F;
        const float YCbCrCrRF = 0.500000F;
        const float YCbCrCrGF = -0.418688F;
        const float YCbCrCrBF = -0.081312F;
        const float RGBRYF = 1.00000F;            // YCbCr转RGB的系数(浮点类型）
        const float RGBRCbF = 0.0000F;
        const float RGBRCrF = 1.40200F;
        const float RGBGYF = 1.00000F;          
        const float RGBGCbF = -0.34414F;
        const float RGBGCrF = -0.71414F;
        const float RGBBYF = 1.00000F;          
        const float RGBBCbF = 1.77200F;
        const float RGBBCrF = 0.00000F; 
        const int Shift = 20;
        const int HalfShiftValue = 1 << (Shift - 1);
        const int YCbCrYRI = (int)(YCbCrYRF * (1 << Shift) + 0.5);         // RGB转YCbCr的系数(整数类型）
        const int YCbCrYGI = (int)(YCbCrYGF * (1 << Shift) + 0.5);
        const int YCbCrYBI = (int)(YCbCrYBF * (1 << Shift) + 0.5);
        const int YCbCrCbRI = (int)(YCbCrCbRF * (1 << Shift) + 0.5);
        const int YCbCrCbGI = (int)(YCbCrCbGF * (1 << Shift) + 0.5);
        const int YCbCrCbBI = (int)(YCbCrCbBF * (1 << Shift) + 0.5);
        const int YCbCrCrRI = (int)(YCbCrCrRF * (1 << Shift) + 0.5);
        const int YCbCrCrGI = (int)(YCbCrCrGF * (1 << Shift) + 0.5);
        const int YCbCrCrBI = (int)(YCbCrCrBF * (1 << Shift) + 0.5);
        const int RGBRYI = (int)(RGBRYF * (1 << Shift) + 0.5);              // YCbCr转RGB的系数(整数类型）
        const int RGBRCbI = (int)(RGBRCbF * (1 << Shift) + 0.5);
        const int RGBRCrI = (int)(RGBRCrF * (1 << Shift) + 0.5);
        const int RGBGYI = (int)(RGBGYF * (1 << Shift) + 0.5);
        const int RGBGCbI = (int)(RGBGCbF * (1 << Shift) + 0.5);
        const int RGBGCrI = (int)(RGBGCrF * (1 << Shift) + 0.5);
        const int RGBBYI = (int)(RGBBYF * (1 << Shift) + 0.5);
        const int RGBBCbI = (int)(RGBBCbF * (1 << Shift) + 0.5);
        const int RGBBCrI = (int)(RGBBCrF * (1 << Shift) + 0.5);
```
RGB2YCbCr
```cpp
public static void ToYCbCr(byte* From, byte* To, int Length = 1)
        {
            if (Length < 1) return;
            byte* End = From + Length * 3;
            int Red, Green, Blue;
            // int Y, Cb, Cr;
            while (From != End)
            {
                Blue = *From; Green = *(From + 1); Red = *(From + 2);
                // 无需判断是否存在溢出，因为测试过整个RGB空间的所有颜色值，无颜色存在溢出
                *To = (byte)((YCbCrYRI * Red + YCbCrYGI * Green + YCbCrYBI * Blue + HalfShiftValue) >> Shift);     
                *(To + 1) = (byte)( 128 + ( (YCbCrCbRI * Red + YCbCrCbGI * Green + YCbCrCbBI * Blue + HalfShiftValue) >> Shift));
                *(To + 2) = (byte) (128+( (YCbCrCrRI * Red + YCbCrCrGI * Green + YCbCrCrBI * Blue + HalfShiftValue) >> Shift));
               // *To = (byte)Y;          // 不要把直接计算的代码放在这里，会降低速度，
                //*(To + 1) = (byte)Cb;
                //*(To + 2) = (byte)Cr;
                From += 3;
                To += 3;
            }
        }
```
被注释掉的代码时原始的，因为这种比较简单的代码，直接对表达式进行强制类型转换比用中间变量要少几条汇编码，并且中间变量越少，在编译后越有可能让CPU用寄存器来缓存一些变量，而不是用内存。
    比如我们比较下注释部分和上述代码的反编译码：
    注释掉的部分的反编译码：
```cpp
Y = (YCbCrYRI * Red + YCbCrYGI * Green + YCbCrYBI * Blue + HalfShiftValue) >> Shift;
0000003a  imul        eax,eax,4C8B4h 
 imul        edx,ebx,9645Ah 
 add         eax,edx 
 imul        edx,edi,1D2F2h 
0000004e  lea         eax,[eax+edx+00080000h] 
 sar         eax,14h 
 mov         dword ptr [ebp-18h],eax 
                *To = (byte)Y;    
0000005b  mov         byte ptr [esi],al
```
上述代码的反编译码：
```cpp
*To = (byte) ( (YCbCrYRI * Red + YCbCrYGI * Green + YCbCrYBI * Blue + HalfShiftValue) >> Shift);
0000003a  imul        eax,ebx,4C8B4h 
 imul        edx,edi,9645Ah 
 add         eax,edx 
 imul        edx,dword ptr [ebp-14h],1D2F2h 
0000004f  lea         eax,[eax+edx+00080000h] 
 sar         eax,14h 
 mov         byte ptr [esi],al
```
   当然，如果循环中的代码复杂一些，这个就不一定了。
     YCbCr转为RGB空间的代码：
```cpp
public static void ToRGB(byte* From, byte* To, int Length = 1)
        {
            if (Length < 1) return;
            byte* End = From + Length * 3;
            int Red, Green, Blue;
            int Y, Cb, Cr;
            while (From != End)
            {
                Y = *From; Cb = *(From + 1)-128; Cr = *(From + 2)-128;
                Red = Y + ((RGBRCrI * Cr + HalfShiftValue) >> Shift);
                Green = Y + ((RGBGCbI * Cb + RGBGCrI * Cr+ HalfShiftValue) >> Shift);
                Blue = Y + ((RGBBCbI * Cb + HalfShiftValue) >> Shift);
                if (Red > 255) Red = 255; else if (Red < 0) Red = 0;
                if (Green > 255) Green = 255; else if (Green < 0) Green = 0;    // 编译后应该比三目运算符的效率高
                if (Blue > 255) Blue = 255; else if (Blue < 0) Blue = 0;
                *To = (byte)Blue;                                               // 由于不是一一对应的，需要判断是否越界
                *(To + 1) = (byte)Green;
                *(To + 2) = (byte)Red;
                From += 3;
                To += 3;
            }
        }
```
