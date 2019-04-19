# Qt QDateEdit 设置下拉日历 - xqhrs232的专栏 - CSDN博客
2018年08月23日 20:23:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：469
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/qq_26782015/article/details/78115196](https://blog.csdn.net/qq_26782015/article/details/78115196)
相关文章
1、单击QDateEdit直接调用QCalendarWidget日历控件----[https://blog.csdn.net/caokunchao/article/details/79172003](https://blog.csdn.net/caokunchao/article/details/79172003)
2、Qt 之 QDateEdit 和 QTimeEdit----[https://blog.csdn.net/liang19890820/article/details/52400160](https://blog.csdn.net/liang19890820/article/details/52400160)
Qt QDateEdit 设置下拉日历:
![](https://img-blog.csdn.net/20151108210745714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过成员函数setCalendarPopup(bool)设置一下属性，就可以使其以日历的方式显示。
` ``QDateEdit date = new QDateEdit();``date->setCalendarPopup(true);`
