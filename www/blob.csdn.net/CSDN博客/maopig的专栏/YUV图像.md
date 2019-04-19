# YUV图像 - maopig的专栏 - CSDN博客
2012年06月16日 16:55:02[maopig](https://me.csdn.net/maopig)阅读数：2815标签：[算法																[数据结构																[百度																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=百度&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[图像处理/去噪/增强](https://blog.csdn.net/maopig/article/category/1090397)
YUV420P，Y，U，V三个分量都是平面格式，分为I420和YV12。I420格式和YV12格式的不同处在U平面和V平面的位置不同。在I420格式中，U平面紧跟在Y平面之后，然后才是V平面（即：YUV）；但YV12则是相反（即：YVU）。
YUV420SP, Y分量平面格式，UV打包格式, 即NV12。
 NV12与NV21类似，U 和 V 交错排列,不同在于UV顺序。
I420: YYYYYYYY UU VV    =>YUV420P
YV12: YYYYYYYY VV UU    =>YUV420P
NV12: YYYYYYYY UVUV     =>YUV420SP
NV21: YYYYYYYY VUVU     =>YUV420SP
dxva 输出 NV12
x264 输入、输出 I420
void yuv420p_to_yuv422(uint8_t * yuv420[3], uint8_t * dest, int width, 
                        int height) 
{ 
    unsigned int x, y; 
     for (y = 0; y < height; ++y) { 
       uint8_t *Y = yuv420[0] + y * width; 
        uint8_t *Cb = yuv420[1] + (y / 2) * (width / 2); 
       uint8_t *Cr = yuv420[2] + (y / 2) * (width / 2); 
        for (x = 0; x < width; x += 2) { 
             *(dest + 0) = Y[0]; 
           *(dest + 1) = Cb[0]; 
           *(dest + 2) = Y[1]; 
            *(dest + 3) = Cr[0]; 
             dest += 4; 
             Y += 2; 
             ++Cb; 
             ++Cr; 
         } 
     } 
}
最近一周整理了一下网上关于YUV数据的转化和显示方面的资料, 与大家共享一下.
1. 关于YUV.
百度百科: [http://baike.baidu.com/view/189685.htm](http://baike.baidu.com/view/189685.htm)
英文资料: [http://en.wikipedia.org/wiki/YUV#Y.27UV420p_.28and_Y.27V12.29](http://www.cnblogs.cc2/#Y.27UV420p_.28and_Y.27V12.29)
YUV转换算法: [http://www.kldby.com/baidunews/index.php/article/jdgg/2010-10-04/10436.html](http://www.kldby.com/baidunews/index.php/article/jdgg/2010-10-04/10436.html)
YUV格式: [http://www.fourcc.org/yuv.php](http://www.fourcc.org/yuv.php)
自定义数据结构参考: [http://www.rosoo.net/a/200604/6051.html](http://www.rosoo.net/a/200604/6051.html)
2. YUV数据的显示.
目前尝试了两种显示YUV数据的方法 YUV表面显示和GDI显示.
YUV表面显示: 
     这个网上有很多源码, 当然为了大家省去一些查找和编译的苦恼, 提供我整理后的源码.
这种显示方式呢, 比较直接, CPU占用也不是很高, 而且不管是YUV420还是YUV420P(分辨率1280*720)都可以显示出来.
但是有一个缺点是: 只能创建一个显示画面, 就是要同时显示几个画面是不行的, 而且按下WIN键和D后, 画面还在屏幕上,
很无语. (我不确定此点是否正确, 我在一天之内没突破它的这个缺点后放弃).
GDI显示:
   就是用相关算法将YUV420P数据--->RGB24之后,使用StretchDIBits函数将每一帧的图像显示出来.
   在转化算法方面, 在网上找到不止10几种, 除去一些存在错误无法编译的外, 筛选了5种方法.
   此种显示方法上, CPU占用肯定比YUV表面显示高, 但至少可以多画面显示了,并且没有其他缺点.
   转化算法见下一篇随笔.
YUV420 显示源码 [http://files.cnblogs.com/doorsky/YUV%e6%98%be%e7%a4%ba.rar](http://files.cnblogs.com/doorsky/YUV%E6%98%BE%E7%A4%BA.rar)
YUV文件 [http://files.cnblogs.com/doorsky/YUV%e6%96%87%e4%bb%b6.rar](http://files.cnblogs.com/doorsky/YUV%E6%96%87%E4%BB%B6.rar)
(在本文中，U 一词相当于 Cb，V 一词相当于 Cr。)
YUV422 format as shown below
4:2:2 表示 2:1 的水平取样，没有垂直下采样
![](https://img-my.csdn.net/uploads/201206/30/1341044724_9151.JPG)
YUV420 format as shown below
4:2:0 表示 2:1 的水平取样，2:1 的垂直下采样. YUV4:2:0并不是说只有U（即 Cb）, V（即 Cr）一定为 0，而是指U：V互相援引，时见时隐，也就是说对于每一个行，只有一个U或者V份量，如果一行是4:2:0的话，下一行就是4:0:2，再下一行是4:2:0...以此类推。
![](https://img-my.csdn.net/uploads/201206/30/1341044820_4687.JPG)
