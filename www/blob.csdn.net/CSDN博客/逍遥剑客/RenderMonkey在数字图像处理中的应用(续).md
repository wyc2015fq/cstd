# RenderMonkey在数字图像处理中的应用(续) - 逍遥剑客 - CSDN博客
2007年10月04日 02:23:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2680
**空间域图像处理**
在空间域图像处理中, 通常都是基于模板的算法. 即通过当前像素周围像素的颜色值来决定当前像素的颜色值. 下面举例说明:
**1.****模糊**
**![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_13.JPG)**
一目了然, 就是把当前像素的颜色用上下左右4个像素的颜色值的和的平均值来代替, 这样就模糊了….
那么, 怎么用RenderMonkey来实现呢?
从前边我们知道, 可以用texCoord.x, texCoord.y来表示像素的坐标, 然后用tex2D()函数来查找图像中指定坐标的颜色值. 不过要注意, 坐标的单位不是像素, 而是纹理坐票, 每个像素对应这里的图像宽度或高度的倒数. 所以, 我们先引入这两个值:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_7.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_8.JPG)加入完毕后, 左边显示两个float值
好了, 这样一来, Color=(S1+S2+S3+S4)/4中的S1的颜色值就可以这样取到:
float4 colorS1 = tex2D( Texture0, texCoord + float2(0,-fInverseViewportWidth) );
为了计算方便, 我们把模板定义成一个滤波器数组:
constfloat4 samples[4] = {
-1.0, 0.0, 0, 0.25,
1.0, 0.0, 0, 0.25,
0.0, .0, 0, 0.25,
0.0 -1.0, 0, 0.25
};
这样颜色的计算就可以通过一个for循环来玩成:
for(int i = 0; i < 4; i++)
   {
       texColor += samples[i].w * tex2D(
          Texture0, TexCoord + 
float2(samples[i].x*fInverseViewportWidth, samples[i].y*fInverseViewportHeight)
       ); 
   }
效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_9.JPG)
以下是整个PixelShader的代码:
sampler Texture0;//源图像
float fInverseViewportWidth;//宽度的倒数
float fInverseViewportHeight;//高度的倒数
constfloat4 samples[4] = {      //滤波器
   -1, 0, 0, 0.25,
    1, 0, 0, 0.25,
    0, 1, 0, 0.25,
    0,-1, 0, 0.25
};
float4 ps_main(float2 TexCoord : TEXCOORD0
               ) : COLOR0
{
float4 texColor = (float4)0;    //初始化颜色值
for(int i = 0; i < 4; i++)
   {
       texColor += samples[i].w * tex2D(
          Texture0, TexCoord + 
float2(samples[i].x*fInverseViewportWidth, samples[i].y*fInverseViewportHeight)
       ); 
   }
return texColor;
}
明白了上面的程序, 那么其它效果就很好做了.
**2.****边缘检测和锐化**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_10.JPG)
|相似的, 边缘检测的滤波器可以这样写:constfloat4 samples[6] = {-1.0, 1.0, 0, 1.0,0.0, 1.0, 0, 2.0,1.0, 1.0, 0, 1.0, -1.0, -1.0, 0, -1.0,0.0, -1.0, 0, -2.0,1.0, -1.0, 0, -1.0};|效果:![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_11.JPG)|
|----|----|
|锐化滤波器:constfloat4 samples[5] = {0.0, 0.0, 0, 11.0/3.0,0.0, 1.0, 0, -2.0/3.0,0.0, -1.0, 0, -2.0/3.0,-1.0, 0.0, 0, -2.0/3.0,1.0, 0.0, 0, -2.0/3.0};|效果:![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_12.JPG)|
有了这些基础, 相信一般的图像处理算法都没什么问题了. 如果需要更多的数学函数的支持, 可以参照这里:
Intrinsic Functions (DirectX HLSL):
[http://msdn2.microsoft.com/en-us/library/bb509611.aspx](http://msdn2.microsoft.com/en-us/library/bb509611.aspx)
这篇文章不错, 同时也推荐一下:
Direct3D提高篇:HLSL编程实现PhotoShop滤镜效果:
[http://tech.it168.com/n/2007-03-29/200703291522292.shtml](http://tech.it168.com/n/2007-03-29/200703291522292.shtml)
