# QT QLabel设置字体的颜色 - xqhrs232的专栏 - CSDN博客
2018年08月29日 23:13:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：336
原文地址::[https://blog.csdn.net/superbfly/article/details/53199814](https://blog.csdn.net/superbfly/article/details/53199814)
相关文章
1、QLabel 字体大小、颜色、图片的应用（整理）----[https://blog.csdn.net/zhangwei3210001/article/details/45041709](https://blog.csdn.net/zhangwei3210001/article/details/45041709)
第一种，使用setPalette()方法如下：
```
QLabel *label = new QLabel(tr("Hello Qt!"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    label->setPalette(pe);
```
- 1
- 2
- 3
- 4
第二种，使用样式表如下：
```
setStyleSheet("color:red;"); 
    //setStyleSheet("color:#ff6600;");
```
- 1
- 2
第三种，使用QStyle，在Qt Demo中有一个很好的讲解QStyle的例子，可以参考学习。
第四种，使用一些简单的HTML格式：
`    QLabel *label = new QLabel("<h2><i>Hello</i><font color=red>Qt!</font></h2>");`- 1
我用的是第二种方法，直接通过 “color:#ff6600;”来设置颜色。
