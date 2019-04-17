# [ZT] matlab中plot画图参数的设置 - Grandyang - 博客园







# [[ZT] matlab中plot画图参数的设置](https://www.cnblogs.com/grandyang/p/4704264.html)








一、Matlab绘图中用到的直线属性包括：

（1）LineStyle：线形

（2）LineWidth：线宽

（3）Color：颜色

（4）MarkerType：标记点的形状

（5）MarkerSize：标记点的大小

（6）MarkerFaceColor：标记点内部的填充颜色

（7）MarkerEdgeColor：标记点边缘的颜色
**1、线形**标记符    线形
 -           实线
 --          虚线
 ：         点线
 -.         点横线


2、点形标记符    点形
 +          加号
 o          圆圈
 *          星号
 .          实心点
 x         叉号
 s         正方形
 d         钻石形
 ^         上三角形
 v         下三角形
 >        右三角形
 <        左三角形
 p        五角星形
 h        六角星形


3、颜色标记符    颜色
 r             红
 g            绿
 b            蓝
 c          蓝绿
 m         紫红
 y           黄
 k           黑
 w          白

**二、描绘不同的直线**
当我们需要对不同类别的数据点进行连线的时候，可以设置以下属性来区分不同类别的点和相应的直线：
（1）LineStyle：线形
（2）MarkerSymbol：点形
（3）Color：颜色
例如：
plot(x,y,'-.or','MarkerFaceColor','g')
其中线形为点横线，数据点形状为圆圈，线条和数据点边缘的颜色都是红色，数据点的填充颜色为绿色。
三、描绘数据点的分布
有时候我们希望画出数据点的分布情况，不需要画出这些点之间的连线，则可以按以下方式实现：
plot(x,y,'d')
即只定义数据点的点形。

最后给出一个例子说明怎样运用以上属性来画图：

plot(t,sin(2*t),'-mo',...
                'LineWidth',2,...
                'MarkerEdgeColor','k',...
                'MarkerFaceColor',[.49 1 .63],...
                'MarkerSize',12)




本文引用地址：[http://blog.sciencenet.cn/blog-281551-573856.html](http://blog.sciencenet.cn/blog-281551-573856.html)此文来自科学网张红艳博客，转载请注明出处。
  












