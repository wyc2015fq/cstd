# Python Graphics 画图 - YZXnuaa的博客 - CSDN博客
2018年01月25日 11:00:01[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1546
使用Python进行图像编程，要使用到Graphics库。Graphics库可以从http://mcsp.wartburg.edu/zelle/python/graphics.py获取。在Windows平台上将下载的graphics.py保存在C:\Python31\Lib\site-packages即可。
下面的代码显示了如何画点、画线、画圆、画矩形、画椭圆以及显示文字。
**[python]**[view plain](http://blog.csdn.net/vernice/article/details/46392187#)[copy](http://blog.csdn.net/vernice/article/details/46392187#)
- from graphics import *  
- 
- #设置画布窗口名和尺寸
- win = GraphWin('CSSA', 700, 700)   
- 
- #画点
- pt = Point(100, 100)  
- pt.draw(win)  
- 
- #画圆
- cir = Circle(Point(200, 200), 75)  
- cir.draw(win)  
- cir.setOutline('red') #外围轮廓颜色
- cir.setFill('yellow')   #填充颜色
- 
- #画线
- line = Line(Point(650, 100), Point(250, 100))  
- line.draw(win)  
- 
- #画矩形
- rect = Rectangle(Point(300, 300), Point(400, 400))  
- rect.setFill('red') #填充颜色
- rect.draw(win)  
- 
- #画椭圆
- oval = Oval(Point(450, 450), Point(600, 600))  
- oval.setFill('red') #填充颜色
- oval.draw(win)  
- 
- #显示文字
- message = Text(Point(win.getWidth()/2, 20), 'Click anywhere to quit.')  
- message.draw(win)  
- 
- #关闭画布窗口
- win.getMouse()  
- win.close()  
![](https://img-blog.csdn.net/20180125110523000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
