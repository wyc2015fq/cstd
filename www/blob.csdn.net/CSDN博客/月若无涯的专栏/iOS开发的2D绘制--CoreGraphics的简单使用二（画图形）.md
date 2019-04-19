# iOS开发的2D绘制--CoreGraphics的简单使用二（画图形） - 月若无涯的专栏 - CSDN博客
2013年07月02日 15:21:57[月若无涯](https://me.csdn.net/u010124617)阅读数：2226
上一篇给大家简单介绍了如何绘制线条，这次我们升级了，我们开始绘制图形，
其实绘制图形也是很简单的，iOS的SDK给我们封装了许多好用的API，废话不多说，实战吧骚年。。。
我们先来画一个圆和一个矩形，看过我上一篇文章的朋友应该会知道，首先是drawRect重写，然后获得图形上下文CGContextRef，
好了，开搞：
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context = UIGraphicsGetCurrentContext();
CGContextSetFillColorWithColor(context, [UIColor grayColor].CGColor);//填充色设置成灰色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画图形了
CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);//设置线的颜色
//画矩形
CGContextAddRect(context,CGRectMake(20,20,100,100));//我们画了一个（20，20，100，100）的矩形
//画圆
CGContextAddEllipseInRect(context,CGRectMake(20,20,100,100));//我们又在此矩形内画了一个圆
CGContextStrokePath(context);//把线在界面上绘制出来
}
看看效果，有图有真相：
![](https://img-blog.csdn.net/20130702145739187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在，我们想把那个圆里面填充上红色，边线绿色加粗，并且让矩形的边线是蓝色
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context = UIGraphicsGetCurrentContext();
CGContextSetFillColorWithColor(context, [UIColor grayColor].CGColor);//填充色设置成灰色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画图形了
CGContextSetStrokeColorWithColor(context, [UIColor blueColor].CGColor);//设置线的颜色
//画矩形
CGContextAddRect(context,CGRectMake(20,20,100,100));//我们画了一个（20，20，100，100）的矩形
CGContextStrokePath(context);//把矩形在界面上绘制出来
//画圆
CGContextSetLineWidth(context,3);
CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);//设置填充颜色为红色
CGContextFillEllipseInRect(context,CGRectMake(20,20,100,100));//填充
CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);//设置线的颜色
CGContextAddEllipseInRect(context,CGRectMake(20,20,100,100));//我们又在此矩形内画了一个圆
CGContextStrokePath(context);//把线在界面上绘制出来
//    CGContextFillPath(context);//把所有的路径填满,这操作也可用于填充，但是需要有封闭的path
}
再看看效果：
![](https://img-blog.csdn.net/20130702150740593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个画图是不是太简单了点？好吧咱们来点看起来显高端的，咱们画一个类似于气泡聊天窗口的图形怎么样？
看代码：
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context = UIGraphicsGetCurrentContext();
CGContextSetFillColorWithColor(context, [UIColor grayColor].CGColor);//填充色设置成灰色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画图形了
CGContextSetFillColorWithColor(context, [UIColor whiteColor].CGColor);//气泡的填充色设置为白色
CGRect rrect = CGRectMake(45,20,200,50);
CGFloat radius = 6.0;//圆角的弧度
CGFloat arrowY = 40;//设置箭头的位置
CGFloat minx = CGRectGetMinX(rrect), midx = CGRectGetMidX(rrect), maxx
 = CGRectGetMaxX(rrect);
CGFloat miny = CGRectGetMinY(rrect), midy = CGRectGetMidY(rrect), maxy
 = CGRectGetMaxY(rrect);
// 
画一下小箭头
CGContextMoveToPoint(context, minx, arrowY);
CGContextAddLineToPoint(context,minx-7, arrowY-10.5);
CGContextAddLineToPoint(context,minx, arrowY-10);
//添加四个角的圆角弧度
CGContextAddArcToPoint(context, minx, miny, midx, miny, radius);
CGContextAddArcToPoint(context, maxx, miny, maxx, midy, radius);
CGContextAddArcToPoint(context, maxx, maxy, midx, maxy, radius);
CGContextAddArcToPoint(context, minx, maxy, minx, midy, radius);
//结束绘制
CGContextClosePath(context);//完成整个path
CGContextFillPath(context);//把整个path内部填充
}
看看这代码运行的效果：
![](https://img-blog.csdn.net/20130702152115250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
咋样，这个看起来还是很不错的吧？
