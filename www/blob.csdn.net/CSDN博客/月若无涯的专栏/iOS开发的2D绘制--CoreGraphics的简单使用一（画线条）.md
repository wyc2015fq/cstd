# iOS开发的2D绘制--CoreGraphics的简单使用一（画线条） - 月若无涯的专栏 - CSDN博客
2013年07月02日 14:31:20[月若无涯](https://me.csdn.net/u010124617)阅读数：2822
在iOS开发中，经常遇到需要重新绘制某些特定区域，比如画两条不同颜色的线、画一些文字什么的，那么如何去操作呢？
我将分几篇文章，慢慢的介绍一下iOS开发当中的2D绘制问题。
**********正式开始了*********月若无涯原创********
首先我们在每次创建一个view的时候，应该都能看到有如下这样的部分的代码：
/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect
 {
 // Drawing code
 }
 */
那么这里是用来做什么的呢？看注释可以知道，这里就是用来重绘你需要的内容的地方。默认情况下是被注释掉的。
所以，如果我们要重绘，第一步就是取消这里的注释，或者说，我们需要在
- (void)drawRect:(CGRect)rect
这方法里进行重写，来完成绘制的工作。
废话不多说，先来个简单的，咱们就画一条绿色的线
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context=UIGraphicsGetCurrentContext();//获取绘图用的图形上下文
CGContextSetFillColorWithColor(context, [UIColorwhiteColor].CGColor);//填充色设置成白色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画线了
CGContextSetStrokeColorWithColor(context, [UIColorgreenColor].CGColor);//设置线的颜色
CGContextMoveToPoint(context,20,20);//画线的起始点位置
CGContextAddLineToPoint(context,200,60);//画第一条线的终点位置
CGContextStrokePath(context);//把线在界面上绘制出来
}
看看效果：
![](https://img-blog.csdn.net/20130702141030140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一条绿色的线，绘制在白色的背景上。
好了，那么增加难度，画许多条线，其中最后我们增加一条红色的线，注意我们的最后的颜色发生了改变，怎么操作，看代码：
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context=UIGraphicsGetCurrentContext();
CGContextSetFillColorWithColor(context, [UIColorwhiteColor].CGColor);//填充色设置成白色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画线了
CGContextSetStrokeColorWithColor(context, [UIColorgreenColor].CGColor);//设置线的颜色
CGContextMoveToPoint(context,20,20);//画线的起始点位置
CGContextAddLineToPoint(context,200,60);//从(20,20)到(200，60)，此时画笔位置在(200，60)
CGContextAddLineToPoint(context,240,90);//从(200，60)到(240，90)，此时画笔位置在(240，90)
CGContextAddLineToPoint(context,80,120);//从(240，90)到(80，120)，此时画笔位置在(80，120)
//下面开始画红色的条线
CGContextStrokePath(context);//把之前的相同颜色的线先画出来，因为我们要换个颜色，如果不换颜色，可以不用这句
CGContextMoveToPoint(context,70,130);//由于上面的线已经画了，我们已经没有可用的画笔了，所以重新设置画线的起始点位置
CGContextSetStrokeColorWithColor(context, [UIColorredColor].CGColor);//再次设置线的颜色
CGContextAddLineToPoint(context,230,180);//画第二条线的终点位置
CGContextStrokePath(context);//把线在界面上绘制出来
}
再看看效果：
![](https://img-blog.csdn.net/20130702142537062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
画线的工作基本上就是这样，是不是很简单？
