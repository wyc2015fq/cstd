# 关于DCDC模块电源滤波的问题 - xiahouzuoxin - CSDN博客





2014年01月11日 22:17:56[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：12683








**谨记**：DCDC模块要注意电源滤波的问题，尤其在AD和传感器供电电路中




使用的金升阳的WAR0512S-3WR2模块，如下图，没有加LC滤波电路

![](https://img-blog.csdn.net/20140111220356484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为该模块为开关电源，最初没经过计算，只在输出端加了10uF的滤波电容，电源输出的纹波如下图（注意，频率大约1KHz），这种电源的噪声一般是开关电源的开关频率造成的。

![](https://img-blog.csdn.net/20140111220313531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




在模块后添加LC滤波电路后，

![](https://img-blog.csdn.net/20140111220834343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


LC低通滤波电路的滤波频率约为

![](http://www.forkosh.com/mathtex.cgi?formdata=%5CSmall+f%3D%5Cfrac1%7B2*%5Cpi*%5Csqrt%7BLC%7D%7D%3D73.4+Hz)


足以滤除上面的约1KHz的开关电源噪声，滤波后电源的输出为下图。

![](https://img-blog.csdn.net/20140111220906921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





对此问题的深入分析要感谢我的研究生实验室同学！还要多学习。






