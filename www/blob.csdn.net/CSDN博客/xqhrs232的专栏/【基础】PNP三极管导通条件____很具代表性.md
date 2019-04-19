# 【基础】PNP三极管导通条件____很具代表性 - xqhrs232的专栏 - CSDN博客
2016年07月07日 22:36:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7527
个人分类：[各种电子元器件/硬件技术](https://blog.csdn.net/xqhrs232/article/category/6300755)
原文地址::[http://www.php230.com/wtoutiao/1175881.html](http://www.php230.com/wtoutiao/1175881.html)
工作好几年，三极管用的最多的，其实是开关电路。下面分别介绍PNP型和NPN型的三极管。先说PNP型的三极管，常用的型号有9012，8550等等。如何使用呢，如下图：
![](http://www.wxs1s.com.cn/m.php?p=http://mmbiz.qpic.cn/mmbiz/JLZn8IR4JdiccTYbuAnqv99icKH1yWX0Z7ol4QnVX1mJSULBLsXLQCed8FvutlibsVpffD2D2m6Q6mrszXDnnBIXA/0?wx_fmt=jpeg)
FM是一个蜂鸣器，8550是一个PNP型的三极管，C端接地，B端由单片机控制，E端通过FM接VCC。根据箭头的方向，E端高电压的时候，当B端也是高电压，那么E和C之间是断开的，当B端是低电压，那么E和C直接导通，实现开关的作用。简单的技巧：三极管上箭头所在方向的二极管，只要二极管正向导通，那么三极管上下就能导通。
NPN的三极管也是同样的道理，这里不做过多解释。
这里可以看到，三极管用作开关管的时候非常简单，根本不会涉及到任何所谓的公式、放大倍数计算等等。
