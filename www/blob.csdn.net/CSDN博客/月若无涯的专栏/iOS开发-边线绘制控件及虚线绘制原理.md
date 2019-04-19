# iOS开发-边线绘制控件及虚线绘制原理 - 月若无涯的专栏 - CSDN博客
2015年03月30日 14:10:37[月若无涯](https://me.csdn.net/u010124617)阅读数：2376
最近的项目设计稿中，出现了虚线分割线这种东西，想起之前自己做的一个用于绘制各种边线效果的控件[YRBorderView](https://github.com/YueRuo/YRBorderView)，但是只支持实线条，立刻对其进行了升级。
支持四条边线不同颜色，不同粗细，不同弧度，支持裁剪掉边线外部，支持虚线。 
效果图： 
![效果图](https://img-blog.csdn.net/20150330131602677)
需要试试这控件的请戳：[YRBorderView](https://github.com/YueRuo/YRBorderView)
关于简单的线条和图形绘制请看我之前的CoreGraphics系列博客： 
[http://blog.csdn.net/u010124617/article/details/9222753](http://blog.csdn.net/u010124617/article/details/9222753)
这里再说明下虚线的绘制，利用CGContextSetLineDash，对于等宽的虚线，一般情况下： 
```
CGFloat lengths[] = {间隔长度}; 
CGContextSetLineDash(context, 0, lengths,1);
```
就可以了。
如果需要更加复杂的绘制，则需要来分析下这个方法的参数： 
```
CGContextSetLineDash(CGContextRef c, CGFloat phase, 
  const CGFloat lengths[], size_t count)
```
其中：
- 第一个参数是绘制使用的上下文
- phase参数表示绘制第一个线段的时候跳过多少距离
- lengths参数是绘制的间隔距离数组
- count是第三个参数lengths数组的长度
比较难理解的是lengths这个参数，其实它是这样的：
想想你的虚线，每一段实线和每一段虚线都是一个个小线条，只不过其中一部分可见，一部分不可见，那么对于每个小线条的绘制规则是按照lengths数组中的值来继续的。 
例如对于lengths[] = {5,10,15}; 
第一段长度是5         - - -（线） 
第二段长度是10        - - -（空白） 
第三段长度是15        - - -（线） 
第四段长度是5         - - -（空白） 
第五段长度是10        - - -（线） 
……
这样便构成了一条不规则的虚线，如下图 
![这里写图片描述](https://img-blog.csdn.net/20150330140126476)
如果此时我们的第二个参数设置是3，那么第一个小段的长度就会是5-3=2，然后继续是10、15…..之类的
希望能帮助到大家。
