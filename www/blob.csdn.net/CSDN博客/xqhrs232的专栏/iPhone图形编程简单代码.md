# iPhone图形编程简单代码 - xqhrs232的专栏 - CSDN博客
2019年02月22日 17:32:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：36
原文地址::[https://blog.csdn.net/xiaoliuxingzi/article/details/7967603](https://blog.csdn.net/xiaoliuxingzi/article/details/7967603)
相关代码
1、iPhone应用程序编程指南(图形和描画)----[https://blog.csdn.net/andy_jiangbin/article/details/9991399](https://blog.csdn.net/andy_jiangbin/article/details/9991399)
2、iPhone应用程序编程指南(图形和描画)----[http://blog.sina.com.cn/s/blog_7695024501012noc.html](http://blog.sina.com.cn/s/blog_7695024501012noc.html)
3、iOS 图形编程总结----[https://www.2cto.com/kf/201411/348869.html](https://www.2cto.com/kf/201411/348869.html)
4、iOS 编程的图形对象地切割方法,按钮切成三角形----[http://blog.sina.com.cn/s/blog_5fae23350102wmlu.html](http://blog.sina.com.cn/s/blog_5fae23350102wmlu.html)
5、iPhone游戏开发：使用到的工具和技术----[https://blog.csdn.net/huangtingting_ios/article/details/52289920](https://blog.csdn.net/huangtingting_ios/article/details/52289920)
6、2D图形编程指南----[https://blog.csdn.net/dangyalingengjia/article/details/77451059](https://blog.csdn.net/dangyalingengjia/article/details/77451059)
作者:[小小](http://xiaoxiaoblog.sinaapp.com/) /发表于1 小时前 /分类:[iOS](http://1.xiaoxiaoblog.sinaapp.com/category/%E7%BC%96%E7%A0%81%E5%BF%AB%E4%B9%90/ios/)
－(void)drawInContext:(CGContextRef)context
{
// Drawing with a white stroke color
CGContextSetRGBStrokeColor(context, 1.0, 1.0, 1.0, 1.0);
// And drawing with a blue fill color
// 这是为了下面的填充方法准备的，填充方法会自动来找这个填充的颜色
CGContextSetRGBFillColor(context, 0.0, 0.0, 1.0, 1.0);
// Draw them with a 2.0 stroke width so they are a bit more visible.
CGContextSetLineWidth(context, 2.0);
// Add Rect to the current path, then stroke it
CGContextAddRect(context, CGRectMake(30.0, 30.0, 60.0, 60.0));
// 记住fill是对某个区域进行填充，stroke是对你指定的路径的一个描绘
//CGContextFillRect(context, CGRectMake(30.0, 30.0, 60.0, 60.0));
CGContextStrokePath(context);
// Stroke Rect convenience that is equivalent to above
// 跟上面等价,就不用在写CGContextStrokePath(context);这句话了，直接出图
CGContextStrokeRect(context, CGRectMake(30.0, 120.0, 60.0, 60.0));
// Stroke rect convenience equivalent to the above, plus a call to CGContextSetLineWidth().
CGContextStrokeRectWithWidth(context, CGRectMake(30.0, 210.0, 60.0, 60.0), 10.0);
// Demonstate the stroke is on both sides of the path.
// 保存当前图形的上下文，根据这个例子，我总结一下，当我在某些情况发生时，我只想改变某个部分的颜色，或则时大小子类的
// 这时候为了不影响后面的图形创建。例如下面的rects数组里面的三个矩形框，我不想让这个也跟着变化，所以我把即将改变的图形
// 上下文先给保存起来，这样在保存之后无论做什么样的改变，只要我在这个改变完成后在恢复回来，就不会影响我下面的图形状态
// 说白了图形上下文，我比做一张纸，我在这张纸的某个地方画了红色的图，然后我又在其他地方画了一个蓝色的图。蓝色图的配置参数
// 还是我之前的哪些设置，不会变
CGContextSaveGState(context);
CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0);
CGContextStrokeRectWithWidth(context, CGRectMake(30.0, 210.0, 60.0, 60.0), 2.0);
CGContextRestoreGState(context);
CGRect rects[] = 
{
CGRectMake(120.0, 30.0, 60.0, 60.0),
CGRectMake(120.0, 120.0, 60.0, 60.0),
CGRectMake(120.0, 210.0, 60.0, 60.0),
};
// Bulk call to add rects to the current path.
CGContextAddRects(context, rects, sizeof(rects)/sizeof(rects[0]));
CGContextStrokePath(context);
// Create filled rectangles via two different paths.
// 创建填充矩形，用两个不同的路径
// Add/Fill path
CGContextAddRect(context, CGRectMake(210.0, 30.0, 60.0, 60.0));
CGContextFillPath(context);
// Fill convienience. (便利方法,对比下就知道)
CGContextFillRect(context, CGRectMake(210.0, 120.0, 60.0, 60.0));
}
