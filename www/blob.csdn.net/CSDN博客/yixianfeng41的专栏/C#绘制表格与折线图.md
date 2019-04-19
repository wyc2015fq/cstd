# C#绘制表格与折线图 - yixianfeng41的专栏 - CSDN博客
2015年07月08日 15:55:55[第2梦](https://me.csdn.net/yixianfeng41)阅读数：3626
做过一个用折线图实时展示温度，脉搏数据的功能，现在将折线图绘制相关的东西总结下。
效果图一：
![](https://img-blog.csdn.net/20150708155533989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
效果图二：
![](https://img-blog.csdn.net/20150708155547429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
数据是随机生成的，随着数据增加，折线会向左移动.........
主要参数设置：
```
public void init_squareValue(int beginx, int beginy, int intervalx, int intervaly)  //x,y轴的开始位置，x,y轴方格线之间的间隔
public void init_coordinatesValue(string xunit, string yunit, float beginyvalue, float everyYvalue)   //x,y轴单位，y轴坐标初始值，y轴每一个vlaue
public void initPen(Pen squarepen, Pen linepen, SolidBrush covaluesolidbrush, Pen coLinepen, Pen pointpen)  //各种pen的设置
```
主要方法：
```
public void DrawCoordinateLine(Graphics g)  //画坐标轴
public void DrawSquare(Graphics g)   //画方格线
public void DrawCoordinatesValues(Graphics g, int interval_xv, int interval_yv)  //画坐标轴值，每隔interval_xv格横坐标值显示，每隔interval_yv纵坐标显示
public void DrawEveryPoint(Graphics g, List<float> list_point, Pen pointPen, bool full = false, int pointR = 2)  //画每一个点
public void DrawLinkPoint(Graphics g, List<float> list_point, Pen linePen)   //画折线
```
开始时，每改变一下折线图，就要改变好多东西，很是麻烦，后来我把绘制的东西的全部放到了一个类中，想要绘制什么样的折线图时，调用一下相关设置参数和绘制的方法，方便多了。
完整的类，以及相关调用实现，我都整理成了一个TEST测试程序，上面效果图就是这个程序运行的结果，大家可以看看,[DrawTable](http://download.csdn.net/detail/yixianfeng41/8880677)。
