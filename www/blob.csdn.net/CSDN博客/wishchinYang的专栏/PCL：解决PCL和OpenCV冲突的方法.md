# PCL：解决PCL和OpenCV冲突的方法 - wishchinYang的专栏 - CSDN博客
2013年11月30日 17:04:06[wishchin](https://me.csdn.net/wishchin)阅读数：2076
不是PCL的问题，而是OpenCV的问题。
（1）：先包含PCL库，再包含OpenCV库；
（2）：把里面的UCHAR冲突全部换掉！  如果你有闲情逸致，用正则表达式 慢慢替换去吧！
（3）：或者把FLANN加上限制：：FLANN
（4）：**多个函数库时尽量使用如std::  cv:: pcl:: 等来代替 使用 using namespace std,using namespace pcl,using namespace cv**
