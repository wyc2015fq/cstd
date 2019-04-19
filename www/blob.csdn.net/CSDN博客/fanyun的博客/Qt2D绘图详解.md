# Qt2D绘图详解 - fanyun的博客 - CSDN博客
2017年03月31日 12:41:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：245

Qt中提供了强大的2D绘图系统，可以使用相同的API在屏幕上和绘图·设备上进行绘制，主要基于QPainter、QPainterDevice和QPainterEngine这3个类。
1、QPainter执行绘图操作，QPainter可以绘制一切简单的图形，从简单的一条直线到任何复杂的图形。QPainter类可以在一切继承QPainterDevice的子类上进行绘制操作。
2、QPainterDevice提供绘图设备，是一个二维空间的抽象，
3、QPainterEngine提供一些接口。
