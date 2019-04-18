# SLAM: 图像角点检测的Fast算法（时间阈值实验） - wishchinYang的专栏 - CSDN博客
2015年10月25日 16:26:36[wishchin](https://me.csdn.net/wishchin)阅读数：1783
        作为角点检测的一种快速方法，FastCornerDetect算法比Harris方法、SIft方法都要快一些，应用于实时性要求较高的场合，可以直接应用于SLAM的随机匹配过程。算法来源于2006年的Edward Rosten
 和 Tom Drummond的论文[“Machine learning for high-speed corner
 detection”](http://dl.acm.org/citation.cfm?id=2094478)，并在（在2010年再次被[修订](http://dl.acm.org/citation.cfm?id=1687114)）新版本中再次被提出。
       方法的官方网站：[http://www.edwardrosten.com/work/fast.html](http://www.edwardrosten.com/work/fast.html)
       网站保存有各种语言和各个版本的源代码，可以直接编译运行。
**1. OpenCV Fast算法的运行结果和运行时间**
**检测结果**：
```cpp
cv::FastFeatureDetector fast( threshold , false );   //threshold ==20
fast .detect (image,keypoints);
```
![](https://img-blog.csdn.net/20151025160234904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**检测消耗时间**： 大约 3~20 毫秒
图像大小：479 x 359
![](https://img-blog.csdn.net/20151025162322474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.算法分析**
**减小** cv::FastFeatureDetector fast( threshold , false );   //threshold ==20
**的阈值可以检测更多的角点**。
![](https://img-blog.csdn.net/20151025162127784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
时间上不会有太大的变化...
