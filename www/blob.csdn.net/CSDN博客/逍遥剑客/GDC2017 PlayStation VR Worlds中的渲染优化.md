# GDC2017: PlayStation VR Worlds中的渲染优化 - 逍遥剑客 - CSDN博客
2017年04月10日 00:44:55[xoyojank](https://me.csdn.net/xoyojank)阅读数：3437
介绍了一种很trick的优化方法: Checkerboard Rendering 
这里他们称之为Resolution Gradient 
![这里写图片描述](https://img-blog.csdn.net/20170410000058669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
主要的思想跟NVIDIA的MultiRes Shading差不多, 但是不用依赖硬件的特性才能实现. 简单一点说就是, 因为VR显示器透镜变形的原因, 边缘的渲染分辨率用不着那么高, 中心区域是全分辨率就好了.  
这个优化思路来自于GDC2016上”Fast and Flexible: Technical Art and Rendering For The Unknown”. 
![这里写图片描述](https://img-blog.csdn.net/20170410000611703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170410000630317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过4个2x2像素的Quad(这是做像素着色的基本单位)做Mask, 就能实现4个级别的分辨率 
![这里写图片描述](https://img-blog.csdn.net/20170410000837923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
空出来的那些黑色像素使用最近有效像素的颜色填充上, 需要一个额外的Dilate Pass(当然也可以整合到后期效果里, 直接在采样时就取有效像素) 
![这里写图片描述](https://img-blog.csdn.net/20170410001013283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
跟TemporalAA结合到一起可以解决走样的问题, 但是需要每帧使用不同的Mask 
![这里写图片描述](https://img-blog.csdn.net/20170410001339331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在60FPS的情况下, 结省了大约4ms!!! 
但是走样的问题还是挺明显的, 特别是在高频细节或者动态的像素上 
![这里写图片描述](https://img-blog.csdn.net/20170410001519453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170410001530344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170410001542582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
与4x4MSAA一起用, 调整MSAA的采样位置, 这样可以做到像素级的Mask, 而不是之前的Quad, 效果也就好多了 
![这里写图片描述](https://img-blog.csdn.net/20170410003437452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170410003457468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
还有一个问题, 如果不能开Early Z, 那么这个优化就没意义了, 解决办法是弄个PrePass 
![这里写图片描述](https://img-blog.csdn.net/20170410003722888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
未来可以根据画的物件类型来决定哪些像素使用低分辨率(Mask)来渲染
既然这个技术不依赖特定的显卡, 那么就可以在PC的VR游戏上用, 在PixelBound的情况下, 性能提升应该还是很可观的, 后续有时间可以尝试下
