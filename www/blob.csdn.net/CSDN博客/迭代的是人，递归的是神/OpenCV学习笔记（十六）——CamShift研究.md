# OpenCV学习笔记（十六）——CamShift研究 - 迭代的是人，递归的是神 - CSDN博客





2011年10月31日 20:57:43[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：12873
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









CamShitf算法，即Continuously Apative Mean-Shift算法，基本思想就是对视频图像的多帧进行MeanShift运算，将上一帧结果作为下一帧的初始值，迭代下去。基本步骤为：

1.选取关键区域

2.计算该区域的颜色概率分布--反向投影图

3.用MeanShift算法找到下一帧的特征区域

4.标记并重复上述步骤

该算法的关键就是可以在目标大小发生改变的时候，可以自适应的调整目标区域继续跟踪

在进行CamShitf和MeanShift算法的时候，需要输入反向投影图，这就要求有个很重要的预处理过程是计算反向投影图。对应的函数为calcBackProject。所谓反向投影图就是一个概率密度图。calcBackProject的输入通常为目标区域的直方图和待跟踪图像的直方图，输出与待跟踪图像大小相同，每一个像素点表示该点为目标区域的概率。这个点越亮，该点属于物体的概率越大。这样的输入参数太适合做MeanS算法了。关于反向直方图，可以参考一下这篇文章[http://blog.163.com/thomaskjh@126/blog/static/370829982010112810358501/](http://blog.163.com/thomaskjh@126/blog/static/370829982010112810358501/)

具体代码如下：



```cpp
calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
backproj &= mask;
RotatedRect trackBox = CamShift(backproj, trackWindow,
                                TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
```





