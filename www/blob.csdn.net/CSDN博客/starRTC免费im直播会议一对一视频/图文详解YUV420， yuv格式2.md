# 图文详解YUV420， yuv格式2 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月18日 12:21:35[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：582
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)
YUV格式有两大类：planar和packed。
对于planar的YUV格式，先连续存储所有像素点的Y，紧接着存储所有像素点的U，随后是所有像素点的V。
对于packed的YUV格式，每个像素点的Y,U,V是连续交*存储的。
YUV码流的存储格式其实与其采样的方式密切相关，主流的采样方式有三种，YUV4:4:4，YUV4:2:2，YUV4:2:0
以黑点表示采样该像素点的Y分量，以空心圆圈表示采用该像素点的UV分量。
    先记住下面这段话，以后提取每个像素的YUV分量会用到。
- YUV 4:4:4采样，每一个Y对应一组UV分量。 
- 
- YUV 4:2:2采样，每两个Y共用一组UV分量。 
- 
- YUV 4:2:0采样，每四个Y共用一组UV分量。 
**存储方式**
**（1）YUYV****格式 （属于YUV422）**每两个Y共用一组UV分量
YUYV为YUV422采样的存储格式中的一种，相邻的两个Y共用其相邻的两个Cb、Cr，对于像素点Y'00、Y'01
 而言，其Cb、Cr的值均为 Cb00、Cr00
FOURCC_YUYV = FOURCC('Y', 'U', 'Y', 'V'), // Alias for YUY2.
**（2） ****U****YVY 格式 （属于YUV422）**
每两个Y共用一组UV分量
**（3） YUV422****P****（属于YUV422）**
    YUV422P也属于YUV422的一种，它是一种Plane模式，先存放所有的Y分量，然后存储所有的U（Cb）分量，最后存储所有的V（Cr）分量，如上图所示。两个Y共用一个UV。比如，对于像素点Y'00、Y'01
 而言，其Cb、Cr的值均为 Cb00、Cr00。
**（4）****YV****12，YU12格式（属于YUV****420****）**
    YU12和YV12属于YUV420格式，也是一种Plane模式，将Y、U、V分量分别打包，依次存储。4个Y分量共用一组UV。注意，上图中，Y'00、Y'01、Y'10、Y'11共用Cr00、Cb00。
I420: YYYYYYYY UU VV =>YUV420P
YV12: YYYYYYYY VV UU =>YUV420P
NV12: YYYYYYYY UVUV =>YUV420SP
NV21: YYYYYYYY VUVU =>YUV420SP

**（5）****NV12、NV21****（属于YUV420）**
    NV12和NV21属于YUV420格式，是一种two-plane模式，即Y和UV分为两个Plane，但是UV（CbCr）为交错存储，而不是分为三个plane。其提取方式与上一种类似，即Y'00、Y'01、Y'10、Y'11共用Cr00、Cb00
===========================================
YUV420 planar数据，
 以720×488大小图象YUV420 planar为例，
其存储格式是： 共大小为(720×480×3>>1)字节，即wxh*1.5
分为三个部分:Y,U和V
Y分量：    (720×480)个字节  
U(Cb)分量：(720×480>>2)个字节 即1/4
V(Cr)分量：(720×480>>2)个字节
三个部分内部均是行优先存储，三个部分之间是Y,U,V
 顺序存储。
即YUV数据的0－－720×480字节是Y分量值，         
720×480－－720×480×5/4字节是U分量    
720×480×5/4 －－720×480×3/2字节是V分量。
=======================
对于所有YUV420图像，它们的Y值排列是完全相同的，因为只有Y的图像就是灰度图像。

[https://msdn.microsoft.com/en-us/library/windows/desktop/dd206750(v=vs.85).aspx](https://msdn.microsoft.com/en-us/library/windows/desktop/dd206750(v=vs.85).aspx)
 待看！！！
[http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html](http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html)
