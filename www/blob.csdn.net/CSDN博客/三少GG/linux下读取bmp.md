# linux下读取bmp - 三少GG - CSDN博客
2010年05月11日 17:28:00[三少GG](https://me.csdn.net/scut1135)阅读数：3411
由于windows中所用头文件"windows.h"在linux下不可用，故自己建立相关的头文件，定义bmp文件的存储结构及格式
代码如下：
/* linux_bmp.h*/
/*
 * Copyright (c)2010 Chris Grant
 *
 * Win32 USER structures and API
 */
#include <iostream>
typedef   struct   tagBITMAPFILEHEADER{   
        unsigned   short   bfType;       //2  **此处需留意**
        unsigned   long    bfSize;         //4                              /*   File   size   in   bytes                     */   
        unsigned   short   bfReserved1;   //2
        unsigned   short   bfReserved2;   //2 
        unsigned   long    bfOffBits;      //4                                  /*   Offset   to   image   data,   bytes   */   
  } __attribute__((packed))BITMAPFILEHEADER,   *PBITMAPFILEHEADER;  //**Attention:"__"是两个"_"! **** 字边界对齐！4字节对齐   **知道在linux的gcc下默认的是四字节的
typedef   struct   tagBITMAPINFOHEADER{   
        unsigned   long    biSize;              //4                 /*   Header   size   in   bytes            */   
         long         biWidth;               //4              /*   Width   of   image                         */   
         long         biHeight;                  //4          /*   Height   of   image                       */   
        unsigned short   biPlanes;        //2        /*   Number   of   colour   planes       */   
        unsigned short   biBitCount;       //2     /*   Bits   per   pixel                         */   
        unsigned   long biCompression;       //4           /*   Compression   type                     */   
        unsigned   long biSizeImage;    //4                  /*   Image   size   in   bytes               */   
        long   biXPelsPerMeter;     //4
        long   biYPelsPerMeter;      //4  /*   Pixels   per   meter                     */   
        unsigned   long biClrUsed;       //4                   /*   Number   of   colours                   */   
        unsigned   long biClrImportant;       //4         /*   Important   colours                   */   
  } __attribute__((packed))BITMAPINFOHEADER,*PBITMAPINFOHEADER;   
typedef   struct   tagRGBQUAD   {   
 unsigned   char   rgbBlue;   /*   Blue   value     */   
 unsigned   char   rgbGreen;   /*   Green   value   */   
 unsigned   char   rgbRed;   /*   Red   value       */   
 unsigned   char   rgbReserved;   /*   Reserved         */   
  }   RGBQUAD;   
将其include即可。
参考：
1.上篇博客
2. [http://m.cnblogs.com/51493/1385324.html](http://m.cnblogs.com/51493/1385324.html)
标题: 一个在LINUX下生成BMP的程序
很多人都搞不定**内存对齐的问题**，最近帮老焦他们写测试程序，用到一个BMP GENERATOR，写了个比较简单的版本，仅针对24位真彩，现把代码公布
#include <iostream>
using namespace std;
typedef long BOOL;
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
......
......
......
 3. [http://topic.csdn.net/t/20040921/04/3392666.html](http://topic.csdn.net/t/20040921/04/3392666.html)
# c语言实现bmp读写(linux上使用),一个奇怪的错误!欢迎诸位高手指教
我现在需要写一个程序,能够在linux上读写bmp文件,理论上十分简单,但是碰到如下错误,不知道因为什么原因.  
  我在读bmp文件头的时候,发现读出的数据根本不对  
  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/gdi/bitmaps_4v1h.asp是msdn上关于bmp的文档的文件头0x00   0x0D共有14byte,  
  文件头的数据结构其定义为  
  typedef   struct   tagBITMAPFILEHEADER   {    
      WORD         bfType;    
      DWORD       bfSize;    
      WORD         bfReserved1;    
      WORD         bfReserved2;    
      DWORD       bfOffBits;    
  }   BITMAPFILEHEADER,   *PBITMAPFILEHEADER;    
  由于我需要在linux上运行,不能用WORD和DWORD,我将WORD   换为unsigned   short将DWORD   换为unsigned   int,如下:  
  typedef   struct   {  
        unsigned   short   int   type;                                    
        unsigned   int   size;                                                
        unsigned   short   int   reserved1;  
        unsigned   short   int   reserved2;  
        unsigned   int   offset;                                            
  }   FILEHEADER;  
  但是该结构将有4+8+4+4+8共28byte(现在cpu的字长一般均为16位)  
  但是我   sizeof(FILEHEADER)则只有16byte,请问这些字节是怎么分配的啊?  
在Windows上的BITMAPFILEHEADER结构的长度并不是你声称的   4+8+4+4+8   =   28   字节。我在VC6下试了一下，是   2+4+2+2+4   =   14   字节。  
  至于你上面定义的 **FILEHEADER   结构，由于内存对齐的原因，采取不同的对齐策略会得到不同的长度。在Linux上GCC的默认对齐规则下，各个成员实际占用的内存分别为（括号内为类型长度）：4(2)、4(4)、2(2)、2(2)、4(4)，所以加起来就是   16   字节。**
  根据Windows中的测试结果，BITMAPFILEHEADER采取的内存对齐策略应该是   1   字节或者   2   字节对齐。在Linux中的程序也可采取类似的对齐规则以便统一结构的长度。  
  建议你不要倚赖于改变系统的默认的对齐规则的方法。最好在程序中依此读取每个成员（即你的方法二），而不是将整个文件头一次读取到结构中。这样虽然稍微麻烦一点，然而程序的通用性好。
typedef   struct   tagBITMAPFILEHEADER{  
        unsigned   short   bfType;                                   /*   Magic   identifier                         */  
        unsigned   bfSize;                                               /*   File   size   in   bytes                     */  
        unsigned   short   bfReserved1;  
        unsigned   short   bfReserved2;  
        unsigned   bfOffBits;                                         /*   Offset   to   image   data,   bytes   */  
  }   BITMAPFILEHEADER,   *PBITMAPFILEHEADER;  
  typedef   struct   tagBITMAPINFOHEADER{  
        unsigned   biSize;                               /*   Header   size   in   bytes             */  
        unsigned   biWidth;                             /*   Width   of   image                         */  
        unsigned   biHeight;                           /*   Height   of   image                       */  
        unsigned   short   biPlanes;               /*   Number   of   colour   planes       */  
        unsigned   short   biBitCount;           /*   Bits   per   pixel                         */  
        unsigned   biCompression;                 /*   Compression   type                     */  
        unsigned   biSizeImage;                     /*   Image   size   in   bytes               */  
        int   biXPelsPerMeter;  
        int   biYPelsPerMeter;                       /*   Pixels   per   meter                     */  
        unsigned   biClrUsed;                         /*   Number   of   colours                   */  
        unsigned   biClrImportant;               /*   Important   colours                   */  
  }   BITMAPINFOHEADER,   *PBITMAPINFOHEADER;  
  typedef   struct   tagRGBQUAD   {  
  unsigned   char   rgbBlue; /*   Blue   value     */  
  unsigned   char   rgbGreen; /*   Green   value   */  
  unsigned   char   rgbRed; /*   Red   value       */  
  unsigned   char   rgbReserved; /*   Reserved         */  
  }   RGBQUAD;  
  我分别用sizeof看他们的大小,分别为16,40,4,后面两个结构就和分别读出的总和一样?就是第一个不同,这是为什么啊?再一个,内存对齐策略是什么意思?
你的结构体肯定是有问题的，因为这种结构肯定是要按四个字节对奇的看看你写的，对不齐就设预留位，看看你写的。  
  typedef   struct   tagBITMAPFILEHEADER   {    
      WORD         bfType;    
      DWORD       bfSize;    
      WORD         bfReserved1;    
      WORD         bfReserved2;    
      DWORD       bfOffBits;    
  }   BITMAPFILEHEADER,   *PBITMAPFILEHEADER;    
  bfType二字节，bfSize四个字节没对齐，你还是找找正确的结构吧。
如果你使用GCC编译器，你可以改变一个结构的对齐方式。比如，想要取消你定义的结构的默认对齐，可以这么做：  
  typedef   struct   {  
  unsigned   short   int   type;  
  unsigned   int   size;  
  unsigned   short   int   reserved1;  
  unsigned   short   int   reserved2;  
  unsigned   int   offset;  
  }   __attribute__   ((packed))   FILEHEADER;  
  则   sizeof(FILEHEADER)   =   2   +   4   +   2   +   2   +   4   =   14，就变成跟Windows中的类似结构一样的了，你的方法一就能得到正确结果了。但是这样的修改内存对齐的方式在非   Intel   处理器上可能会造成运行错误（在Intel   及其兼容的处理器上运行正常，只是效率稍低些）。  
  想了解内存对齐（Memory   Alignment）的概念，你可以在   Google   中以此为关键字进行搜索。
