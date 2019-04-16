# 基于OpenCV实现的Android移动端口红AR - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年06月28日 09:04:40[gloomyfish](https://me.csdn.net/jia20003)阅读数：6239








这个是我们去年想做的一个项目，后来因为各种原因就此搁浅了。但是算法部分我已经把它基本成型了，对各种光线条件下都可以准确的找到嘴唇，提取唇形、然后通过色彩渲染自动变化颜色，实现各种颜色的口红实时渲染，基于OpenCV与Android NDK完成的算法演示。

算法的主要思路首先是通过人脸检测寻找到人脸区域，一旦找到之后就会使用跟踪算法对人脸部位进行跟踪、人脸检测算法可以选择Face++或者OpenCV自带的算法，然后对下部区域进行嘴唇检测，找到之后，选择不同光照下的嘴唇图像，提取ROI如下：

![这里写图片描述](https://img-blog.csdn.net/20170628085351622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

提取了超过1000张作为算法测试，根据这些提取得到上部唇形 
![这里写图片描述](https://img-blog.csdn.net/20170628085457947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
进一步处理之后得到下部唇形状 
![这里写图片描述](https://img-blog.csdn.net/20170628085525681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对整个唇形数据处理之后得到 
![这里写图片描述](https://img-blog.csdn.net/20170628085617105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
基本的命中率在99%左右，通过缓存等技术手段，达到实时不丢帧渲染，最终的渲染效果如下： 红色简单着色 
![这里写图片描述](https://img-blog.csdn.net/20170628085905816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
蓝色简单着色 
![这里写图片描述](https://img-blog.csdn.net/20170628085938296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
算法不足之处 

渲染这块后来因为各种原因，没有去做，主要是没有钱继续下去，就成这个样子！但是整个基于OpenCV实现的移动端AR口红渲染整个实现步骤基本如此！

**欢迎大家留言！！！**








