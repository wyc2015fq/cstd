# Alpha颜色混合的魔法 下篇 - gauss的专栏 - CSDN博客
2012年02月25日 12:44:36[gauss](https://me.csdn.net/mathlmx)阅读数：288标签：[c																[border																[div																[dst																[distance																[图像处理](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=distance&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=border&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[图形与图像处理](https://blog.csdn.net/mathlmx/article/category/605907)
摘要：本系列文章介绍了一种在图像处理、2D游戏、3D游戏中经常使用的图片混合模型:Alpha颜色混合;
它就像神奇的魔法一样，在电脑屏幕上给我们展现出一个个绚丽多彩的世界!
全文 分为: 上篇 各种Alpha颜色混合方式
           下篇 其他一些颜色混合方案、补充 
tag:Alpha,Blend,透明,颜色混合,颜色混合公式
正文：  
  为了便于讨论，这里只处理32bit的ARGB颜色；
  代码使用C++,编译器:VC2005
  (一些数据定义等代码请参见《Alpha颜色混合的魔法 上篇》)
  (文章中的效果图片都是用给出的例子代码实际生成)
A: 利用Alpha颜色混合给图片上蒙一层颜色
void PicBlendColor(const TPicRegion& picDst,const TARGB32&  SrcColor)
{
    unsigned long Alpha=SrcColor.a;
    for (long y=0;y<picDst.height;++y)
    {
        for (long x=0;x<picDst.width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            DstColor.b=(DstColor.b*(255-Alpha) + SrcColor.b*Alpha)/255;
            DstColor.g=(DstColor.g*(255-Alpha) + SrcColor.g*Alpha)/255;
            DstColor.r=(DstColor.r*(255-Alpha) + SrcColor.r*Alpha)/255;
            DstColor.a=(DstColor.a*(255-Alpha) + SrcColor.a*Alpha)/255;
        }
    }
}
  函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)
                      源图片  
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlendColor_result.JPG)
       混合颜色(B=0,G=0,R=255,A=127)后的图片   
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlendColor_m_result.JPG)
   在多处混合3个不同颜色(B=0,G=0,R=255,A=127)、
          (B=0,G=255,R=0,A=127)、(B=255,G=0,R=0,A=127)后的图片    
B.用乘以一个颜色来调节图片颜色通道
void PicBlend_Mul(const TPicRegion& picDst,const TARGB32& MulColor)
{
    for (long y=0;y<picDst.height;++y)
    {
        for (long x=0;x<picDst.width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            DstColor.b=(DstColor.b*MulColor.b)/255;
            DstColor.g=(DstColor.g*MulColor.g)/255;
            DstColor.r=(DstColor.r*MulColor.r)/255;
            DstColor.a=(DstColor.a*MulColor.a)/255;
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)
                    源图片  
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Mul_result.JPG)
   乘以颜色(B=255,G=255,R=127,A=255)后的图片
      (相当于将红色通道的颜色值减半)   
C.Alpha通道的逆表示法的优势
  前面带Alpha通道的图片混合时使用的混合公式: 
      Dst=( Dst*(255-Src.Alpha) + Src*Src.Alpha ) / 255;
  为了优化运算,我们可以将图片格式改为Alpha的逆表示法；
       将颜色中的颜色值R保存为R*Alpha/255;
       将颜色中的颜色值G保存为G*Alpha/255;
       将颜色中的颜色值B保存为B*Alpha/255;
       将颜色中的Alpha通道值保存为RAlpha(RAlpha=255-Alpha);
     那么混合的时候可以简化为Dst=(Dst*Src.RAlpha)/255 + Src;
D.多个带Alpha通道的图片的绘制顺序的问题
  图片按透明属性可以分为3种：所有像素完全不透明、部分像素完全透
明另一部分完全不透明、带Alpha通道的图片(透明度为0到255)；做过3D
渲染的人应该都知道，在绘制纹理的时候，带Alpha通道的纹理需要最后
绘制，并且需要排好序才能正确绘制，而用其它两种就不需要排序；因为
可以使用深度缓冲区就能保证按任何绘制顺序都能正确的渲染图像(请自
己想想怎么实现:)  (补充: 实际上，就是排好序的带Alpha通道图片绘制
也不能保证就正确(排序只是是情况稍好些)；最简单的一种情况:3维空间
中的3张带Alpha通道图片相互压住；这时排序绘制也得到错误的渲染；游
戏中经常可以看到这种贴图错误；(一种解决办法是将相互压住的部分分裂
成多个纹理))
  我们来看看3个像素的Alpha混合的情况:
  三个颜色为 C1,C2,C3,先是C2混合到C1,得到C12,然后C3再混合到C12得到C123;
  由Alpha混合公式有:
    C12=(C1*(255-C2.A)+C2*C2.A)/255;
    C123=(C12*(255-C3.A)+C3*C3.A)/255;
        =( C1*(255-C2.A)(255-C3.A) + C2*C2.A*255 + C3.C3.A*255 - C2*C2.A*C3.A ) / (255*255)
  由该公式可以看看出，C1先混合C2和先混合C3将得到不同的混合结果(C2/C3的不对称性)；
  所以使用Alpha混合时要得到正确的混合结果必须保证图片颜色值的混合顺序；
  那么我想完成这样的一个功能是否就不能完成呢？我想“将需要依次绘制的两张
带Alpha通道图片(两次绘制)，预先合成为一张图片，而保持绘制它的时候和以前
的两次绘制得到的效果一致” 
  这个要求实际上要求更改3个颜色的绘制顺序，而得到相同的混合效果；这个功能是
能够完成的，只是需要一个新的混合公式(前面已经证明Alpha混合公式做不到)；
我们来推导一下这个有用的公式：
  三个颜色为 C1,C2,C3,假设用新公式混合C2,C3得到C23,然后C23再用Alpha混合到
C1从而得到C123';
  所以有C123'=(C1*(255-C23.A)+C23*C23.A)/255;
  由于要求C123'等于前面的C123,所以有:
  (C1*(255-C23.A)+C23*C23.A)/255 
   = ( C1*(255-C2.A)(255-C3.A) + C2*C2.A*255 + C3.C3.A*255 - C2*C2.A*C3.A ) / (255*255)
  即：C1*(255-C23.A)*255=C1*(255-C2.A)*(255-C3.A)            ----(1)
          C23*C23.A*255=C2*C2.A*255 + C3.C3.A*255 - C2*C2.A*C3.A ----(2)
  由(1)有: C23.A=(C2.A+C3.A) - (C2.A*C3.A/255)    ----(3)
  由(2)有: C23=(C2*C2.A*255 + C3.C3.A*255 - C2*C2.A*C3.A)/(C23.A*255) ----(4)
  (3),(4)就是我们需要的预处理混合公式;
  (提示: (4)中当C23.A=0的时候C23可以为任意值)
  (提示: 更多颜色的预先混合公式用类似的推导也很容易得到；用浮点颜色有利于提高精度，
       用Alpha通道的逆表示法在这里也有很多优势)
  (不知道谁能想到不排序也能得到正确的Alpha混合方式的快速方法，或者添加
新的混合参数通道,使图片不排序也能正确混合。)
(颜色之间还能进行很多其他类型的混合运算，下面做一些简单示例)
E. 颜色的最大值、最小值混合
void PicBlend_Max(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=max(DstColor.b , SrcColor.b);
            DstColor.g=max(DstColor.g , SrcColor.g);
            DstColor.r=max(DstColor.r , SrcColor.r);
            DstColor.a=max(DstColor.a , SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                 源图片0                             源图片1
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Max_result.JPG)
                            (源图片0-源图片1)效果   
void PicBlend_Min(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=min(DstColor.b , SrcColor.b);
            DstColor.g=min(DstColor.g , SrcColor.g);
            DstColor.r=min(DstColor.r , SrcColor.r);
            DstColor.a=min(DstColor.a , SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                 源图片0                             源图片1     
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Min_result.JPG)
                                  混合后效果
F. 颜色的相减和颜色距离
    inline long border_color_down(long color)
    {
        if (color<0)
            return 0;
        else
            return color;
    }
void PicBlend_Sub(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=border_color_down(DstColor.b - SrcColor.b);
            DstColor.g=border_color_down(DstColor.g - SrcColor.g);
            DstColor.r=border_color_down(DstColor.r - SrcColor.r);
            DstColor.a=border_color_down(DstColor.a - SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                   源图片0                             源图片1       
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Sub_result.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Subr_result.JPG)
            (源图片0-源图片1)效果                  (源图片1-源图片0)效果       
 (提示:比如用减去图片的亮度来得到物体的假阴影(需要考虑方向或倾斜))
void PicBlend_Distance(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=abs(DstColor.b - SrcColor.b);
            DstColor.g=abs(DstColor.g - SrcColor.g);
            DstColor.r=abs(DstColor.r - SrcColor.r);
            DstColor.a=abs(DstColor.a - SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                   源图片0                             源图片1       
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Distance_result.JPG)
                                    混合效果  
 (提示:比如用连续帧之间的颜色距离图，以得到视野中运动物体的信息(背景会被减掉))
G. 颜色除法
    inline long color_div(long color,long colordiv)
    {
        if (colordiv==0)
            return 255;
        else
            return border_color_up(color*255/colordiv);
    }
void PicBlend_Div(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=color_div(DstColor.b , SrcColor.b);
            DstColor.g=color_div(DstColor.g , SrcColor.g);
            DstColor.r=color_div(DstColor.r , SrcColor.r);
            DstColor.a=color_div(DstColor.a , SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                   源图片0                             源图片1            
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_Div_result.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_DivR_result.JPG)
          (源图片0 div 源图片1)效果             (源图片1 div 源图片0)效果     
H. 把颜色亮度当作为混合系数的混合效果
    inline unsigned long gray(const TARGB32& Color)
    {
        return (Color.r+Color.g+Color.b)/3;
    }
void PicBlend_MixByGray(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            unsigned long Alpha=(gray(SrcColor)+255-gray(DstColor))>>1;
            DstColor.b=(DstColor.b*(255-Alpha) + SrcColor.b*Alpha)/255;
            DstColor.g=(DstColor.g*(255-Alpha) + SrcColor.g*Alpha)/255;
            DstColor.r=(DstColor.r*(255-Alpha) + SrcColor.r*Alpha)/255;
            DstColor.a=(DstColor.a*(255-Alpha) + SrcColor.a*Alpha)/255;
        }
    }
} 
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                   源图片0                             源图片1                
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_MixByGray_result.JPG)
                                  混合后效果
    inline unsigned char mix_color(unsigned long a,unsigned long b)
    {
        return a+b-((a*b) >>7 );
    }
void PicBlend_MixByColor(const TPicRegion& picDst,const TPicRegion& picSrc)
{
    long width =min(picDst.width ,picSrc.width );
    long height=min(picDst.height,picSrc.height);
    for (long y=0;y<height;++y)
    {
        for (long x=0;x<width;++x)
        {
            TARGB32& DstColor=Pixels(picDst,x,y);
            TARGB32  SrcColor=Pixels(picSrc,x,y);
            DstColor.b=mix_color(DstColor.b,SrcColor.b);
            DstColor.g=mix_color(DstColor.g,SrcColor.g);
            DstColor.r=mix_color(DstColor.r,SrcColor.r);
            DstColor.a=mix_color(DstColor.a,SrcColor.a);
        }
    }
}
   函数效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_src0.JPG)
                   源图片0                             源图片1                
![](https://p-blog.csdn.net/images/p_blog_csdn_net/housisong/PicBlend_MixByColor_result.JPG)
                                   混合后效果
提示: 还有其他很多颜色之间的混合运算，你也可组合各种运算用到颜色的混合中，
也许你会得到一种新的有趣或有用的算法呢:)  
