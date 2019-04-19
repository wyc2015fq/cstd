# iOS开发的2D绘制--CoreGraphics的简单使用三（画文字和图片） - 月若无涯的专栏 - CSDN博客
2013年07月02日 17:28:42[月若无涯](https://me.csdn.net/u010124617)阅读数：3035
在iOS开发当中，有时候我们可能会自己去绘制一些文字啊图片什么的，那么如何操作呢？
比如我想在界面上绘制出“你好我这句话是用自己绘制出来的哦”，怎么办呢？
同样是在重绘的时候，我们有两种方法，其中一种是使用
CGContextShowTextAtPoint
但是这个方法相对比较麻烦，不支持中文，而且需要做翻转等等，我们采用简单好用的，NSString 本身提供了draw的方法，比较简单并且好用：
同样的，对于图片来说，也推荐是使用UIImage自己提供的draw方法，下面给个例子，画文字和图片：
- (void)drawRect:(CGRect)rect
{
// Drawing code
CGContextRef context=UIGraphicsGetCurrentContext();
CGContextSetFillColorWithColor(context, [UIColor grayColor].CGColor);//填充色设置成灰色
CGContextFillRect(context,self.bounds);//把整个空间用刚设置的颜色填充
//上面是准备工作，下面开始画图形了
NSString *drawTextString=@"这是我自己绘制的文字哦";
CGContextSetTextDrawingMode (context,kCGTextFillStroke);//设置绘制方式
CGContextSetFillColorWithColor(context, [UIColor blueColor].CGColor);//填充色设置成蓝色，即文字颜色
CGContextSetLineWidth(context,0);//我们采用的是FillStroke的方式，所以要把边线去掉，否则文字会很粗
//绘制文字，注意，此方法从7.0开始逐渐废弃，慎用！！
    [drawTextString drawAtPoint:CGPointMake(20,20)withFont:[UIFont systemFontOfSize:15]];
//绘制图片
UIImage *image=[UIImage imageNamed:@"testPic.jpg"];
    [image drawAtPoint:CGPointMake(5,60)];//在某个点绘制，此时不能对图片大小进行调整
    [image drawInRect:CGRectMake(5,320,100,80)];//在某个区域绘制，可以对图片进行大小调整
}
看看效果：
![](https://img-blog.csdn.net/20130702172128453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEyNDYxNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
简单好用吧？
需要说明的是，这种绘制文字的方式能满足一般的重绘需求，能实现简单的定制，但是相对来说文字格式比较单一，如果涉及到更加复杂的文字排版，比如调整行距、字间距、某个特殊文字的字号、某段的颜色等等，可以使用更为犀利的CoreText来实现，效果更好。
今天又仔细看了一下iOS7.0的SDK，发现NSString的draw的方法全部都标记上了准备废弃。。。悲剧了，看来从此中文的重绘都只能使用CoreText了。后面有机会给大家介绍。
另外，图片的绘制这里，我仅仅是给大家一个示范，真的涉及到定制某个界面的时候，尽可能的还是使用UIImageView添加到视图，因为在这里绘制的效果虽然不错，但是如果控件频繁的调用重绘，那么这里的图片绘制操作会相对比较耗时，开发者去处理的话还是比较麻烦的。图片的绘制大多数情况下用于对图片的处理，比如两个图片合成一张、图片裁剪等等，后面有空会介绍。
