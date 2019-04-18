# Half-Pixel Offset 究竟是个什么鬼？ - tkokof1的专栏 - CSDN博客

2018年05月18日 20:31:38[tkokof1](https://me.csdn.net/tkokof1)阅读数：203


**友情提示***Half-Pixel Offset 其实算是个过时话题,请依据个人情况谨慎了解 :)*

讲述之前我们先明确几个概念:
- 
窗口由正方形[(注1)](#note_1)的像素(pixel)组成,每个像素只能显示一种颜色,并且**像素坐标的原点在左上角像素的中心点(重要)**

![以 3 * 3 的窗口为例](https://img-blog.csdn.net/20180518201418794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- 
纹理也是由正方形的纹素(texel)组成,每个纹素代表一种颜色,并且**纹素坐标的原点在左上角纹素的左上角(重要)**

![以 2 * 2 的纹理为例](https://img-blog.csdn.net/20180518201455359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- 
纹理的采样使用的是双线性(Bilinear)插值的方式(更多的细节可以看[这里](https://msdn.microsoft.com/en-us/library/bb172357%28v=vs.85%29.aspx))

![以采样 2 * 2 的纹理的(0.5, 0.5)点为例](https://img-blog.csdn.net/20180518201506489?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

需要了解的概念就是这些,现在我们尝试在像素坐标的原点处绘制一个 2 * 2 大小的正方形,还记的像素坐标的原点是在像素的中心吗?我们想要绘制的正方形大概是这个样子:

![示意图](https://img-blog.csdn.net/20180518201524175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于像素是离散的,我们需要将绘制的正方形与像素尽可能的”对齐”(这里涉及到光栅化的规则,有兴趣的朋友可以去[这里](https://msdn.microsoft.com/en-us/library/bb147314%28v=vs.85%29.aspx)了解),所以实际绘制的正方形是这个样子的:

![示意图](https://img-blog.csdn.net/20180518201601630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

考虑到我们是从像素坐标的原点开始定义正方形的,所以上图所示的实际绘制结果也是符合预期的(正方形左上角与窗口左上角是对齐的)

现在我们想要将上面的纹理映射到刚才所绘制的正方形上去,为此我们需要为正方形的每个顶点计算纹素坐标,计算过程很简单,相关结果如下图所示:

![示意图](https://img-blog.csdn.net/20180518201626989?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

简单想象一下,通过上面的纹理映射,我们期望得到的绘制结果是这个样子的:

![示意图](https://img-blog.csdn.net/20180518201648280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但实际上,我们得到的绘制结果却是这个样子的:

![示意图](https://img-blog.csdn.net/20180518201706958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

什么鬼 ?

不急,我们来简单梳理一下~

回忆一下最开始需要绘制的的正方形示意图,我们在上面标注下纹素坐标:

![示意图](https://img-blog.csdn.net/20180518201734764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

根据上图中像素对应的纹素坐标,我们可以计算出像素对应的纹素颜色(此处我们没有详细讲解计算的方法,不清楚的朋友可以理解为取纹素坐标附近的四个像素的加权平均值即可):

![计算公式](https://img-blog.csdn.net/20180518201751883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

于是我们便得到了上面那个令人诧异的绘制结果~

怎么修正这个问题呢?

一种方法是直接偏移像素的纹素坐标,拿上面的正方形绘制为例,我们在采样纹素点(0, 0)时做一个(0.25, 0.25)的偏移,即采样(0 + 0.25, 0 + 0.25)点的纹理,这样我们便能采样到预期的纹理颜色了

不过更通用的做法,还是直接偏移顶点的像素坐标,仍然拿上面的正方形绘制举例,我们对正方形的各个顶点做一个(-0.5, -0.5)像素的偏移,那么实际绘制的正方形就是这个样子的:

![示意图](https://img-blog.csdn.net/20180518201601630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时,各个像素中点对应的纹素坐标如下图所示:

![示意图](https://img-blog.csdn.net/2018051820264961?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

根据纹素坐标计算一下像素颜色即可发现我们采样到了预期的纹理颜色:

![计算公式](https://img-blog.csdn.net/20180518201937359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而上述那么(-0.5, -0.5)的像素偏移,即是 **Half-Pixel Offset**

Half-Pixel Offset 只会在 Direct3D 9 及之前的Direct3D版本上出现,本质原因是像素坐标和纹素坐标定义不一致,OpenGL的像素坐标和纹素坐标定义是一致的,Direct3D 10以后也统一了像素坐标和纹素坐标的定义, Half-Pixel Offset 的问题也就不再存在了

如果你对于这个话题还有进一步了解的兴趣,可以再看看[这里](https://msdn.microsoft.com/en-us/library/bb219690%28VS.85%29.aspx),[这里](https://msdn.microsoft.com/en-us/library/cc308049%28VS.85%29.aspx),[这里](https://aras-p.info/blog/2016/04/08/solving-dx9-half-pixel-offset/),和[这里](http://drilian.com/category/development/graphics/d3d9/)

##### 注1 : 严格来讲,像素是点,而不是正方形

