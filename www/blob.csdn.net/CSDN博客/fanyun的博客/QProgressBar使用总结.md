# QProgressBar使用总结 - fanyun的博客 - CSDN博客
2017年12月21日 19:00:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2051
         QProgressBar样式如果直接调用Qt自带的，会发现很丑！那么怎么美化一下呢？以下介绍几种样式美化方式：
 1.纯色进度
```cpp
setStyleSheet（“{border:2px solid grey;border-radius:5px;text-align:center;}”）；
```
```cpp
progressBar::chunk
{
   background-color:#05B8CC;
   width:20px;
}
```
显示效果如图1所示：
![](https://img-blog.csdn.net/20171221190302268)
                                                                                            图1
2.格子色彩条
        显示效果如图2所示：
```cpp
setStyleSheet（“{ border:2px solid grey;border-radius:5px;text-align:center;}”）；
```
```cpp
progressBar::chunk
{
   background-color:#CD96CD;
   width:10px;
   margin:0.5px;
}
```
```cpp
```
                                                                                 图2
3.加载文件延迟，或者加载出错，给出一个繁忙提示，等待系统容错处理结束，再继续恢复加载
QProgressBar *pProgressBar = new QProgressBar(this);
pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
pProgressBar->setMinimum(0);  // 最小值
pProgressBar->setMaximum(0);  // 最大值样式：
```
progressBar::chunk
{
   background-color:rgb(33, 44, 55);
   width:10px;
   margin:0.5px;
}
```
显示效果如图3所示：
![](https://img-blog.csdn.net/20171228102512545)
                                                                            图3
4.显示进度格式设置
(1).%p - 被完成百分比所取代
           百分比的公式为：(value() - minimum()) / (maximum() - minimum())。
(2).%v - 被当前值所取代
(3).%m - 被总步数所取代
