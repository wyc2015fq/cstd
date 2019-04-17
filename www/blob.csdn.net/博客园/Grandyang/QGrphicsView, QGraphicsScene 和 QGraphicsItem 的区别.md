# QGrphicsView, QGraphicsScene 和 QGraphicsItem 的区别 - Grandyang - 博客园







# [QGrphicsView, QGraphicsScene 和 QGraphicsItem 的区别](https://www.cnblogs.com/grandyang/p/4285232.html)







初学Qt的人会经常搞不清这三个图像类QGrphicsView, QGraphicsScene 和 QGraphicsItem，它们到底有什么区别呢？

QGrphicsView类实际上是为QGraphicsScene类的内容提空了一个控件，它在一个可滑动视图空间内可视化QGraphicsScene的内容。

QGraphicsScene类提供了一个平面来管理大量的2D的图像项目，像点，直线，多边形，圆等。它是QGraphicsItem类的容器。

打个比方来说，QGrphicsView类就像是一片空旷的场地，QGraphicsScene类是上面搭建的一个舞台，QGraphicsItem类是舞台上的各种东西。












