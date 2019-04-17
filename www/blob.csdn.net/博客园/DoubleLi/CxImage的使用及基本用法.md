# CxImage的使用及基本用法 - DoubleLi - 博客园






基本定义：CxImage类库是一个优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。

下载地址:http://www.codeproject.com/KB/graphics/cximage.aspx

配置环境:


1、拷贝CxImage到项目目录

a、编译CxImage

b、cximage600_full\CxImage:所有的头文件和源文件

c、查找所有的lib



2、添加头文件目录

CxImage\Include

3、添加库文件目录

CxImage\lib

3、添加链接
cximage.lib
demod.lib
j2k.lib
jasper.lib
jbig.lib
jpeg.lib
png.lib
tiff.lib
zlib.lib


4、程序中添加头文件

#include "ximage.h"



基本用法

打开:






**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- CxImage image;   
- if(image.load("name",类型))   
- {   
-     CDC *pDC = GetDC();   
-     image.Draw(pDC->GetSafeHDC(),CRect rect(0,0,100,100));   
-     pDC->DeleteDC;   
- }  





旋转:



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- CxImage smallImage;//旋转后的图片   
- image.Rotate(90,&smallImage);//旋转90，并且保存到smallImage中   
- smallImage.Save(保存的名字,类型);   





镜像:



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- CxImage ImgTmp = image;   
- if(ImgTmp.Mirror())   
- {   
- image.Draw(.... , .....);   
- }  






缩放：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- <div class="container"><div class="line number1 index0 alt2"><code class="cpp plain">CxImage samllImg; </code></div><div class="line number2 index1 alt1"><code class="cpp spaces"></code></div><div class="line number3 index2 alt2"><code class="cpp plain">image.Resample(新宽度,新高度,0,&smallImg); </code></div><div class="line number4 index3 alt1"><code class="cpp spaces"></code></div><div class="line number5 index4 alt2"><code class="cpp plain">smallImg.Save(</code><code class="cpp string">"自定第一大小.jpg"</code><code class="cpp plain">,CXIMAGE_SUPPORT_JPG);</code></div></div>  





剪辑：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- CDC *pDC=GetDC();   
- UpdateData();   
- CxImage smallImg;   
- tempimage.Crop(CRect(m_xTop,m_yTop,m_xWidth,m_yHeiht),&smallImg);   
- smallImg.Save("剪辑图片.jpg",CXIMAGE_SUPPORT_JPG);   
- smallImg.Draw(pDC->GetSafeHdc(),CRect(40,70,picwidth,picheight));   
- pDC->DeleteDC();   





混合：



**[cpp]**[view plain](http://blog.csdn.net/augusdi/article/details/8678156#)[copy](http://blog.csdn.net/augusdi/article/details/8678156#)



- CDC *pDC=GetDC();   
- CxImage smallImg;   
- smallImg.Load("混合源文件.jpg",CXIMAGE_SUPPORT_JPG);   
- tempimage.Mix(smallImg,CxImage::OpAvg,0,0,true);   
- tempimage.Save("Mix混合.jpg",CXIMAGE_SUPPORT_JPG);   
- tempimage.Draw(pDC->GetSafeHdc(),CRect(40,70,picwidth,picheight));   



[http://blog.csdn.net/whucv/article/details/7689397](http://blog.csdn.net/whucv/article/details/7689397)









