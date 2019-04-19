# DM365 color space - maopig的专栏 - CSDN博客
2012年05月04日 15:40:14[maopig](https://me.csdn.net/maopig)阅读数：1707标签：[dst																[存储																[byte																[框架																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
YUV的几种格式
420P：420P数据的存放方式一般是先存放Y，然后存放U，最后存放V的数据，每一个像素使用12bits(1.5BYTE)保存。
422P：422P数据的存放方式也是先存放Y，然后存放U，最后存放V的数据，每一个像素使用16bits（2BYTE）保存。
420PSemi，422PSemi：这是TI新添加的一种格式，在DM6467的输出当中使用的是422PSemi来进行输出，所以将YUV的数据输出之前都需要将其他格式的YUV数据转换为422PSemi格式。
这两种格式所占用的存储空间分别和420P, 422P所占的存储空间一致。
420PSemi：先存放Y的数据，接着U和V的数据交叉存放，对于422PSemi也是一样。
举个例子：
对于30*20像素的420P的YUV数据，在存储的时候会先存30*20字节的Y，然后存放15*10字节的U，最后存放15*10的V。
如果是420PSemi，那么会先存放30*20字节的Y，接着U和V会交叉存放，分别存放15*10字节，这样420P和420PSemi所占空间均为30*20*3/2字节。
YUV格式简介
YUV主要用于优化彩色[视频](http://baike.baidu.com/view/16215.htm)信号的传输，使其向后相容老式黑白电视。与RGB视频信号传输相比，它最大的优点在于只需占用极少的频宽（RGB要求三个独立的视频信号同时传输）。其中 “Y”表示明亮度（Luminance或Luma），也就是灰阶值；而“U”和“V”
 表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。“亮度”是透过RGB输入信号来建立的，方 法是将RGB信号的特定部分叠加到一起。“色度”则定义了颜色的两个方面─色调与饱和度，分别用Cr和CB来表示。其中，Cr反映了RGB输入信号红色部 分与RGB信号亮度值之间的差异。而Cb反映的是RGB输入信号蓝色部分与RGB信号亮度值之同的差异。
采用YUV色彩空间的重要性是它的亮度信号Y和色度信号U、V是分离的。如果只有Y信号分量而
没有U、V分量，那么这样表示的图像就是黑白灰度图像。彩色电视采用YUV空间正是为了用亮度信号Y解决彩色电视机与黑白电视机的兼容问题，使黑白电视机 也能接收彩色电视信号。
在TI的DMAI框架中，对于OMAP3530，视频输出的时候使用的是V4L2_PIX_FMT_YUYV (’YUYV’)， 而DM6467采用的是V4L2_PIX_FMT_YUV422UVP。
下面的话是从DMAI的ColorSpace.h中摘出来的：
而我们在使用YUVPlayer的时候，只有420P, 422P等能够播放，而420PSemi和422PSemi不能播放，所以在存储和读取的时候需要转换，下面是一个转化函数：
void yuv_420p_to_420psemi(int32_t height, int32_t width, int32_t lineLength, char *src, char *dst)
{
int32_t i, j;
int32_t size;
int32_t quarter_size;
size = height * lineLength;
quarter_size = size / 4;
for (i=0; i<height; i++)
{
for (j=0; j<width; j++)
{
memcpy(dst, src, width);
}
dst += lineLength;
src += width;
}
for (i=0; i<height/2; i++)
{
for (j=0; j<width; j += 2)
{
*(dst+j+i*lineLength) = *src++;
}
}
for (i=0; i<height/2; i++)
{
for (j=1; j<width; j += 2)
{
*(dst+j+i*lineLength) = *src++;
}
}
}
