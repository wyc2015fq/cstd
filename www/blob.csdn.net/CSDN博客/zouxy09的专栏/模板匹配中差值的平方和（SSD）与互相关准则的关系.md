
# 模板匹配中差值的平方和（SSD）与互相关准则的关系 - zouxy09的专栏 - CSDN博客


2013年01月28日 17:25:36[zouxy09](https://me.csdn.net/zouxy09)阅读数：21455


**模板匹配中差值的平方和（SSD）与互相关准则的关系**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
模板匹配TemplateMatching是在图像中寻找目标的方法之一。原理很简单，就是在一幅图像中寻找和模板图像（patch）最相似的区域。在OpenCV中有对应的函数可以调用：
void matchTemplate( const Mat& image, const Mat& templ, Mat&result, int method );
该函数的功能为，在输入源图像Sourceimage（I）中滑动框，寻找各个位置与模板图像Template image（T）的相似度，并将结果保存在结果矩阵result matrix（R）中。该矩阵的每一个点的亮度表示与模板T的匹配程度。然后可以通过函数minMaxLoc定位矩阵R中的最大值（该函数也可以确定最小值）。那通过什么去评价两个图像相似呢？这就存在一个评价准则，也就是参数method，它可以有以下值（匹配的方法）：
CV_TM_SQDIFF平方差匹配法，最好的匹配为0，值越大匹配越差；
CV_TM_SQDIFF_NORMED 归一化平方差匹配法；
CV_TM_CCORR相关匹配法，采用乘法操作，数值越大表明匹配越好；
CV_TM_CCORR_NORMED 归一化相关匹配法；
CV_TM_CCOEFF相关系数匹配法，最好的匹配为1，-1表示最差的匹配；
CV_TM_CCOEFF_NORMED 归一化相关系数匹配法；
前面两种方法为越小的值表示越匹配，后四种方法值越大越匹配。
其中：
**CV_TM_SQDIFF为：**Sumof Squared Difference (SSD) 差值的平方和：
![](https://img-my.csdn.net/uploads/201301/28/1359365384_3637.jpg)

**CV_TM_CCORR 为：**Cross Correlation互相关：
![](https://img-my.csdn.net/uploads/201301/28/1359365398_6568.jpg)
SSD可以看成是欧式距离的平方。我们把SSD展开，可以得到：
![](https://img-my.csdn.net/uploads/201301/28/1359365422_9592.jpg)
可以看到，上式的第一项（模板图像T的能量）是一个常数，第三项（图像I局部的能量）也可以近似一个常数，那么可以看到，剩下的第二项就是和cross correlation一样的，也就是互相关项。而SSD是数值越大，相似度越小，cross correlation是数值越大，相似度越大。
参考：
Konstantinos G. Derpanis 等《RelationshipBetween the Sum of Squared Diﬀerence (SSD) and Cross Correlation for TemplateMatching》

