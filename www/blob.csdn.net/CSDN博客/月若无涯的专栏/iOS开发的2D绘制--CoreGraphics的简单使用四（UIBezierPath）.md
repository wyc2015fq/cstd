# iOS开发的2D绘制--CoreGraphics的简单使用四（UIBezierPath） - 月若无涯的专栏 - CSDN博客
2013年07月29日 12:14:44[月若无涯](https://me.csdn.net/u010124617)阅读数：2244
前面几篇文章简单的介绍了使用2D绘制一些简单的使用，这篇文章介绍一个CoreGraphics中比较有意思的类UIBezierPath，为什么说它有意思呢，因为理论上用这个类可以完成任何的2D绘制。它使CoreGraphics框架中，针对绘图路径的一个封装类。
而且有一个mac下的比较著名的软件PaintCode，用来绘图后生成相应Objective-c代码的工具，其生成的代码也是基于UIBezierPath的。
大家可以去查看它的API了解它具体含有的方法，这里给出一个例子。
使用UIBezierPath绘制类似iOS扁平化之前的UINavigationBar返回按钮的效果。
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGFloat topEdgeInset = 10;//图形距顶部的距离
CGFloat leftEdgeInset = 0;//图形距离左侧的距离
CGFloat rightEdgeInset = 0;//图形距离右侧的距离
CGFloat arrowCuspX = leftEdgeInset;//箭头尖的位置的起点
CGFloat leftCornerX = arrowCuspX+10;//箭头尖左侧突出20
CGFloat rightCornerX = self.frame.size.width-rightEdgeInset;
CGFloat topY = topEdgeInset;
CGFloat midY = self.frame.size.height/2;
CGFloat bottomY = self.frame.size.height-topEdgeInset;
CGFloat cornerRadius=6;//右侧的圆角弧度
CGContextRef context = UIGraphicsGetCurrentContext();
//定义填充的颜色,这里我用红色
UIColor* fillColor = [UIColor redColor];
//定义一个渐变颜色，实现上面那个填充色的渐变
CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
NSArray* innerColorColors = [NSArray arrayWithObjects:
                                 (id)[UIColorwhiteColor].CGColor,
                                 (id)fillColor.CGColor,
                                 (id)[UIColorblackColor].CGColor,nil];
CGFloat innerColorLocations[] = {0,0.3,1};
CGGradientRef innerColor = CGGradientCreateWithColors(colorSpace, (CFArrayRef)innerColorColors,
 innerColorLocations);
//线条宽度设置为0
CGFloat bezierStrokeWidth = 0;
//UIBezierPath
UIBezierPath* bezierPath = [UIBezierPath bezierPath];
    [bezierPath moveToPoint:CGPointMake(arrowCuspX, midY)];
    [bezierPath addLineToPoint:CGPointMake(leftCornerX, bottomY)];
    [bezierPath addLineToPoint:CGPointMake(rightCornerX-cornerRadius, bottomY)];
    [bezierPath addQuadCurveToPoint:CGPointMake(rightCornerX, bottomY-cornerRadius) controlPoint:CGPointMake(rightCornerX,
 bottomY)];
    [bezierPath addLineToPoint:CGPointMake(rightCornerX, topY+cornerRadius)];
    [bezierPath addQuadCurveToPoint:CGPointMake(rightCornerX-cornerRadius, topY) controlPoint:CGPointMake(rightCornerX,
 topY)];
    [bezierPath addLineToPoint:CGPointMake(leftCornerX, topY)];
    [bezierPath addLineToPoint:CGPointMake(arrowCuspX, midY)];
    [bezierPath closePath];//关闭路径，如果当前没有完成封闭，则在起点和终点补上一条线
CGContextSaveGState(context);//先保存当前绘制
    [bezierPath addClip];//把此路径裁剪出来，否则渐变的绘制区域是整个当前的图形上下文
CGContextDrawLinearGradient(context, innerColor,CGPointMake(leftCornerX, topY),CGPointMake(leftCornerX,
 bottomY), 0);//绘制渐变
CGContextRestoreGState(context);//恢复绘制的内容到之前的的屏幕上
    bezierPath.lineWidth = bezierStrokeWidth;
    [bezierPathstroke];//路径绘制
//清理释放内存
CGGradientRelease(innerColor);
CGColorSpaceRelease(colorSpace);
}
看一下绘制出来的效果：
![](https://img-blog.csdn.net/20130729121301140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
希望这个例子能对大家有所帮助，共勉。
