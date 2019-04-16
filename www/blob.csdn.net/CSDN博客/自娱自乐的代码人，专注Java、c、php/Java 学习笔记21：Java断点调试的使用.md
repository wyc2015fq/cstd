# Java 学习笔记21：Java断点调试的使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月11日 19:22:49[initphp](https://me.csdn.net/initphp)阅读数：9351
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









Java断点调试是写Java程序一个非常基本功。下面我们就介绍一下简单的Java断点调试功能。
- 我们给findAllGuestbook()方法添加了两个断点，eclispce最左边有两个绿点的就是断点的点。断点需要Ctrl + Shift + B 快捷键就可以部署断点了。
![](https://img-my.csdn.net/uploads/201212/11/1355224983_6662.jpg)



- 然后我们需要运行的是Debug模式，在eclispce上面的一只甲壳虫模样的按钮。

![](https://img-my.csdn.net/uploads/201212/11/1355225132_7700.jpg)


- 然后点击debug模式 运行程序，如果是web，那么需要在浏览器中访问以下需要请求的url地址，然后打开eclispce，程序就会在断点处中止。


![](https://img-my.csdn.net/uploads/201212/11/1355225260_4542.jpg)



- 如果需要继续下一个断点，可以按F8。同时，在上面的调试框中，有很多关于断点的数据信息。

![](https://img-my.csdn.net/uploads/201212/11/1355225403_6191.jpg)





