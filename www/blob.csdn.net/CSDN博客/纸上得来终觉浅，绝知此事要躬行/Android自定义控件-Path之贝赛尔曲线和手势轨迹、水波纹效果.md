# Android自定义控件-Path之贝赛尔曲线和手势轨迹、水波纹效果 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月05日 10:53:15[boonya](https://me.csdn.net/boonya)阅读数：5145








[《Android自定义控件三部曲文章索引》](http://blog.csdn.net/harvic880925/article/details/50995268)

[**文章出处：**](http://blog.csdn.net/harvic880925/article/details/50995268)[http://blog.csdn.net/harvic880925/article/details/50995587](http://blog.csdn.net/harvic880925/article/details/50995587)



从这篇开始，我将延续androidGraphics系列文章把图片相关的知识给大家讲完，这一篇先稍微进阶一下，给大家把《android Graphics（二）：路径及文字》略去的quadTo（二阶贝塞尔）函数，给大家补充一下。


本篇最终将以两个例子给大家演示贝塞尔曲线的强大用途： 
**1、手势轨迹**
![](https://img-blog.csdn.net/20160328094622304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

利用贝塞尔曲线，我们能实现平滑的手势轨迹效果 
**2、水波纹效果**


![](https://img-blog.csdn.net/20160328094655217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


电池充电时，有些手机会显示水波纹效果，就是这样做出来的。 

废话不多说，开整吧


## 一、概述

在[《android Graphics（二）：路径及文字》](http://blog.csdn.net/harvic880925/article/details/38926877)中我们略去了有关所有贝赛尔曲线的知识，在Path中有四个函数与贝赛尔曲线有关：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //二阶贝赛尔
- publicvoid quadTo(float x1, float y1, float x2, float y2)  
- publicvoid rQuadTo(float dx1, float dy1, float dx2, float dy2)  
- //三阶贝赛尔
- publicvoid cubicTo(float x1, float y1, float x2, float y2,float x3, float y3)  
- publicvoid rCubicTo(float x1, float y1, float x2, float y2,float x3, float y3)  


这里的四个函数的具体意义我们后面会具体详细讲解，我们这篇也就是利用这四个函数来实现我们的贝赛尔曲线相关的效果的。

### 1、贝赛尔曲线来源

在数学的数值分析领域中，贝赛尔曲线（Bézier曲线）是电脑图形学中相当重要的参数曲线。更高维度的广泛化贝塞尔曲线就称作贝塞尔曲面，其中贝塞尔三角是一种特殊的实例。


贝塞尔曲线于1962年，由法国工程师皮埃尔·贝塞尔（Pierre Bézier）所广泛发表，他运用贝塞尔曲线来为汽车的主体进行设计。贝塞尔曲线最初由Paul de Casteljau于1959年运用de Casteljau算法开发，以稳定数值的方法求出贝塞尔曲线。


### 2、贝赛尔曲线公式

这部分是很有难度的，大家做好准备了哦 


#### 一阶贝赛尔曲线 

其公式可概括为：
![](https://img-blog.csdn.net/20160328202051449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对应动画演示为：


![](https://img-blog.csdn.net/20160328202312248)


P0为起点、P1为终点，t表示当前时间，B(t)表示公式的结果值。 

注意，曲线的意义就是公式结果B(t)随时间的变化，其取值所形成的轨迹。在动画中，黑色点表示在当前时间t下公式B(t)的取值。而红色的那条线就不在各个时间点下不同取值的B(t)所形成的轨迹。

总而言之：对于一阶贝赛尔曲线，大家可以理解为在起始点和终点形成的这条直线上，匀速移动的点。 


#### 二阶贝赛尔曲线 

同样，先来看看二阶贝赛尔曲线的公式（虽然看不懂，呵呵）


![](https://img-blog.csdn.net/20160328202422676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


大家也不用研究这个公式了，没一定数学功底也研究不出来了啥，咱还是看动画吧


![](https://img-blog.csdn.net/20160328202508739)


在这里P0是起始点,P2是终点，P1是控制点 

假设将时间定在t=0.25的时刻，此时的状态如下图所示：


![](https://img-blog.csdn.net/20160328202531208)


首先，P0点和P1点形成了一条贝赛尔曲线，还记得我们上面对一阶贝赛尔曲线的总结么：就是一个点在这条直线上做匀速运动；所以P0-P1这条直线上的移动的点就是Q0；


同样，P1,P2形成了一条一阶贝赛尔曲线，在这条一阶贝赛尔曲线上，它们的随时间移动的点是Q1 

最后，动态点Q0和Q1又形成了一条一阶贝赛尔曲线，在它们这条一阶贝赛尔曲线动态移动的点是B 

而B的移动轨迹就是这个二阶贝赛尔曲线的最终形态。从上面的讲解大家也可以知道，之所以叫它二阶贝赛尔曲线是因为，B的移动轨迹是建立在两个一阶贝赛尔曲线的中间点Q0,Q1的基础上的。


在理解了二阶贝赛尔曲线的形成原理以后，我们就不难理解三阶贝赛尔曲线了 


#### 三阶贝赛尔曲线 

同样，先列下基本看不懂的公式


![](https://img-blog.csdn.net/20160328202609692)


这玩意估计也看不懂，讲了也没什么意义，还是结合动画来吧


![](https://img-blog.csdn.net/20160328202640208)


同样，我们取其中一点来讲解轨迹的形成原理，当t=0.25时,此时状态如下：


![](https://img-blog.csdn.net/20160328202725661)


同样，P0是起始点，P3是终点；P1是第一个控制点，P2是第二个控制点； 

首先，这里有三条一阶贝赛尔曲线，分别是P0-P1,P1-P2,P2-P3; 

他们随时间变化的点分别为Q0，Q1，Q2 

然后是由Q0，Q1，Q2这三个点，再次连接，形成了两条一阶贝赛尔曲线，分别是Q0—Q1,Q1—Q2;他们随时间变化的点为R0,R1 

同样，R0和R1同样可以连接形成一条一阶贝赛尔曲线，在R0—R1这条贝赛尔曲线上随时间移动的点是B 

而B的移动轨迹就是这个三阶贝赛尔曲线的最终形状。 

从上面的解析大家可以看出，所谓几阶贝赛尔曲线，全部是由一条条一阶贝赛尔曲线搭起来的； 

在上图中，形成一阶贝赛尔曲线的直线是灰色的，形成二阶贝赛尔曲线线是绿色的，形成三阶贝赛尔曲线的线是蓝色的。 

在理解了上面的二阶和三阶贝赛尔曲线以后，我们再来看几个贝赛尔曲线的动态图 


#### 四阶贝赛尔曲线

![](https://img-blog.csdn.net/20160328202826223)


#### 五阶贝赛尔曲线

![](https://img-blog.csdn.net/20160328202946178)


这里就不再一一讲解形成原理了，大家理解了二阶和三阶贝赛尔曲线以后，这两条的看看就好了，想必大家也是能自己推出四阶贝赛尔曲线的形成原理的。


### 3、贝赛尔曲线与PhotoShop钢笔工具

如果有些同学不懂PhotoShop，这篇文章可能就会有些难度了，本篇文章主要是利用PhotoShop的钢笔工具来得出具体贝塞尔图像的


这么屌的贝赛尔曲线，在专业绘图工具PhotoShop中当然会有它的踪影，它就是钢笔工具，钢笔工具所使用的路径弯曲效果就是二阶贝赛尔曲线。 

我来给大家演示一下钢笔工具的用法：


![](https://img-blog.csdn.net/20160328203123694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们拿最终成形的图形来看一下为什么钢笔工具是二阶贝赛尔曲线：


![](https://img-blog.csdn.net/20160328203210900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


右图演示的假设某一点t=0.25时，动态点B的位置图 

同样，这里P0是起始点，P2是终点，P1是控制点； 

P0-P1、P1-P2形成了第一层的一阶贝赛尔曲线。它们随时间的动态点分别是Q0,Q1 

动态点Q0,Q1又形成了第二层的一阶贝赛尔曲线，它们的动态点是B.而B的轨迹跟钢笔工具的形状是完全一样的。所以钢笔工具的拉伸效果是使用的二阶贝赛尔曲线！ 

这个图与上面二阶贝赛尔曲线t=0.25时的曲线差不多，大家理解起来难度也不大。 

这里需要注意的是，我们在使用钢笔工具时，拖动的是P5点。其实二阶贝赛尔曲线的控制点是其对面的P1点，钢笔工具这样设计是当然是因为操作起来比较方便。 

好了，对贝赛尔曲线的知识讲了那么多，下面开始实战了，看在代码中，贝赛尔曲线是怎么来做的。


## 二、Android中贝赛尔曲线之quadTo

在开篇中，我们已经提到，在Path类中有四个方法与贝赛尔曲线相关，分别是：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- //二阶贝赛尔
- publicvoid quadTo(float x1, float y1, float x2, float y2)  
- publicvoid rQuadTo(float dx1, float dy1, float dx2, float dy2)  
- //三阶贝赛尔
- publicvoid cubicTo(float x1, float y1, float x2, float y2,float x3, float y3)  
- publicvoid rCubicTo(float x1, float y1, float x2, float y2,float x3, float y3)  


在这四个函数中quadTo、rQuadTo是二阶贝赛尔曲线，cubicTo、rCubicTo是三阶贝赛尔曲线；我们这篇文章以二阶贝赛尔曲线的quadTo、rQuadTo为主，三阶贝赛尔曲线cubicTo、rCubicTo用的使用方法与二阶贝赛尔曲线类似，用处也比较少，这篇就不再细讲了。

### 1、quadTo使用原理

这部分我们先来看看quadTo函数的用法，其定义如下：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicvoid quadTo(float x1, float y1, float x2, float y2)  


参数中(x1,y1)是控制点坐标，(x2,y2)是终点坐标 
大家可能会有一个疑问：有控制点和终点坐标，那起始点是多少呢？ 
整条线的起始点是通过Path.moveTo(x,y)来指定的，而如果我们连续调用quadTo()，前一个quadTo()的终点，就是下一个quadTo()函数的起点；如果初始没有调用Path.moveTo(x,y)来指定起始点，则默认以控件左上角(0,0)为起始点；大家可能还是有点迷糊，下面我们就举个例子来看看


我们利用quadTo()来画下面的这条波浪线：
![](https://img-blog.csdn.net/20160328203834572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最关键的是如何来确定控制点的位置！前面讲过，PhotoShop中的钢笔工具是二阶贝赛尔曲线，所以我们可以利用钢笔工具来模拟画出这条波浪线来辅助确定控制点的位置


![](https://img-blog.csdn.net/20160328203922416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面我们来看看这个路径轨迹中，控制点分别在哪个位置


![](https://img-blog.csdn.net/20160328220950371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们先看P0-P2这条轨迹，P0是起点，假设位置坐标是(100,300)，P2是终点，假充位置坐标是(300,300)；在以P0为起始点，P2为终点这条二阶贝赛尔曲线上，P1是控制点，很明显P1大概在P0,P2中间的位置，所以它的X坐标应该是200，关于Y坐标，我们无法确定，但很明显的是P1在P0,P2点的上方，也就是它的Y值比它们的小，所以根据钢笔工具上面的位置，我们让P1的比P0,P2的小100;所以P1的坐标是（200，200）


同理，不难求出在P2,P4这条二阶贝赛尔曲线上，它们的控制点P3的坐标位置应该是(400,400)；P3的X坐标是400是，因为P3点是P2,P4的中间点；与P3与P1距离P0-P2-P4这条直线的距离应该是相等的。P1距离P0-P2的值为100；P3距离P2-P4的距离也应该是100，这样不难算出P3的坐标应该是(400,400)；


下面开始是代码部分了。


### 2、示例代码

#### (1)、自定义View 

我们知道在动画绘图时，会调用onDraw(Canvas canvas)函数，我们如果重写了onDraw(Canvas canvas)函数，那么我们利用canvas在上面画了什么，就会显示什么。所以我们自定义一个View




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- public MyView(Context context) {  
- super(context);  
-     }  
- 
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
- 
-         Paint paint = new Paint();  
-         paint.setStyle(Paint.Style.STROKE);  
-         paint.setColor(Color.GREEN);  
- 
-         Path path = new Path();  
-         path.moveTo(100,300);  
-         path.quadTo(200,200,300,300);  
-         path.quadTo(400,400,500,300);  
- 
-         canvas.drawPath(path,paint);  
-     }  
- }  


这里最重要的就是在onDraw(Canvas canvas)中创建Path的过程，我们在上面已经提到，第一个起始点是需要调用path.moveTo(100,300)来指定的，之后后一个path.quadTo的起始点是以前一个path.quadTo的终点为起始点的。有关控制点的位置如何查找，我们上面已经利用钢笔工具给大家讲解了，这里就不再细讲。
所以，大家在自定义控件的时候，要多跟UED沟通，看他们是如何来实现这个效果的，如果是用的钢笔工具，那我们也可以效仿使用二阶贝赛尔曲线来实现。

### 2、使用MyView 

在自定义控件以后，然后直接把它引入到主布局文件中即可(main.xml)




**[html]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:orientation="vertical"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent">
- 
- <com.harvic.BlogBerzMovePath.MyView
- android:layout_width="match_parent"
- android:layout_height="match_parent"/>
- </LinearLayout>


由于直接做为控件显示，所以MainActivity不需要额外的代码即可显示，MainActivity代码如下：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyActivity extends Activity {  
- /**
-      * Called when the activity is first created.
-      */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
-     }  
- }  

**源码在文章底部给出 **
通过这个例子希望大家知道两点：
- 整条线的起始点是通过Path.moveTo(x,y)来指定的，如果初始没有调用Path.moveTo(x,y)来指定起始点，则默认以控件左上角(0,0)为起始点；
- 而如果我们连续调用quadTo()，前一个quadTo()的终点，就是下一个quadTo()函数的起点；

## 三、手指轨迹

要实现手指轨迹其实是非常简单的，我们只需要在自定义中拦截OnTouchEvent，然后根据手指的移动轨迹来绘制Path即可。 

要实现把手指的移动轨迹连接起来，最简单的方法就是直接使用Path.lineTo()就能实现把各个点连接起来。


### 1、实现方式一：Path.lineTo(x,y)

我们先来看看效果图：
![](https://img-blog.csdn.net/20160328204347574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


#### （1）、自定义View——MyView

首先，我们自定义一个View，完整代码如下：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- 
- private Path mPath = new Path();  
- public MyView(Context context) {  
- super(context);  
-     }  
- 
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-     }  
- 
- @Override
- publicboolean onTouchEvent(MotionEvent event) {  
- switch (event.getAction()){  
- case MotionEvent.ACTION_DOWN: {  
-                 mPath.moveTo(event.getX(), event.getY());  
- returntrue;  
-             }  
- case MotionEvent.ACTION_MOVE:  
-                 mPath.lineTo(event.getX(), event.getY());  
-                 postInvalidate();  
- break;  
- default:  
- break;  
-         }  
- returnsuper.onTouchEvent(event);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-         Paint paint = new Paint();  
-         paint.setColor(Color.GREEN);  
-         paint.setStyle(Paint.Style.STROKE);  
- 
-         canvas.drawPath(mPath,paint);  
-     }  
- 
- publicvoid reset(){  
-         mPath.reset();  
-         invalidate();  
-     }  
- }  


最重要的位置就是在重写onTouchEvent的位置：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicboolean onTouchEvent(MotionEvent event) {  
- switch (event.getAction()){  
- case MotionEvent.ACTION_DOWN: {  
-             mPath.moveTo(event.getX(), event.getY());  
- returntrue;  
-         }  
- case MotionEvent.ACTION_MOVE:  
-             mPath.lineTo(event.getX(), event.getY());  
-             postInvalidate();  
- break;  
- default:  
- break;  
-     }  
- returnsuper.onTouchEvent(event);  
- }  


当用户点击屏幕的时候，我们调用mPath.moveTo(event.getX(), event.getY());然后在用户移动手指时使用mPath.lineTo(event.getX(), event.getY());将各个点串起来。然后调用postInvalidate()重绘；


Path.moveTo()和Path.lineTo()的用法，大家如果看了[《android Graphics（二）：路径及文字》](http://blog.csdn.net/harvic880925/article/details/38926877)之后，理解起来应该没什么难度，但这里有两个地方需要注意

第一：有关在case MotionEvent.ACTION_DOWN时return true的问题：return true表示当前控件已经消费了下按动作，之后的ACTION_MOVE、ACTION_UP动作也会继续传递到当前控件中；如果我们在case MotionEvent.ACTION_DOWN时return false，那么后序的ACTION_MOVE、ACTION_UP动作就不会再传到这个控件来了。有关动作拦截的知识，后续会在这个系列中单独来讲，大家先期待下吧。

第二：这里重绘控件使用的是postInvalidate();而我们以前也有用Invalidate()函数的。这两个函数的作用都是用来重绘控件的，但区别是Invalidate()一定要在UI线程执行，如果不是在UI线程就会报错。而postInvalidate()则没有那么多讲究，它可以在任何线程中执行，而不必一定要是主线程。其实在postInvalidate()就是利用handler给主线程发送刷新界面的消息来实现的，所以它是可以在任何线程中执行，而不会出错。而正是因为它是通过发消息来实现的，所以它的界面刷新可能没有直接调Invalidate()刷的那么快。


所以在我们确定当前线程是主线程的情况下，还是以invalide()函数为主。当我们不确定当前要刷新页面的位置所处的线程是不是主线程的时候，还是用postInvalidate为好；


这里我是故意用的postInvalidate()，因为onTouchEvent()本来就是在主线程中的，使用Invalidate()是更合适的。当我们 

有关OnDraw函数就没什么好讲的，就是把path给画出来：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-     Paint paint = new Paint();  
-     paint.setColor(Color.GREEN);  
-     paint.setStyle(Paint.Style.STROKE);  
- 
-     canvas.drawPath(mPath,paint);  
- }  


最后，我还额外写了一个重置函数：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicvoid reset(){  
-     mPath.reset();  
-     invalidate();  
- }  


#### (2)、主布局 

然后看看布局文件(mian.xml)




**[html]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:orientation="vertical"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent">
- <Button
- android:id="@+id/reset"
- android:layout_width="match_parent"
- android:layout_height="wrap_content"
- android:text="reset"/>
- 
- <com.harvic.BlogMovePath.MyView
- android:id="@+id/myview"
- android:layout_width="match_parent"
- android:layout_height="match_parent"/>
- </LinearLayout>


没什么难度，就是把自定义控件添加到布局中 

### （3）、MyActivity 

然后看MyActivity的操作：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyActivity extends Activity {  
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
- 
- final MyView myView = (MyView)findViewById(R.id.myview);  
-         findViewById(R.id.reset).setOnClickListener(new View.OnClickListener() {  
- @Override
- publicvoid onClick(View v) {  
-                 myView.reset();  
-             }  
-         });  
-     }  
- }  


这里实现的就是当点击按钮时，调用 myView.reset()来重置画布； 
**源码在文章底部给出 **

#### （4）、使用Path.lineTo()所存在问题

上面我们虽然实现了，画出手指的移动轨迹，但我们仔细来看看画出来的图：
![](https://img-blog.csdn.net/20160328205051550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们把S放大，明显看出，在两个点连接处有明显的转折，而且在S顶部位置横纵坐标变化比较快的位置，看起来跟图片这大后的马赛克一样；利用Path绘图，是不可能出现马赛克的，因为除了Bitmap以外的任何canvas绘图全部都是矢量图，也就是利用数学公式来作出来的图，无论放在多大屏幕上，都不可能会出现马赛克！这里利用Path绘图，在S顶部之所以看起来像是马赛克是因为这个S是由各个不同点之间连线写出来的，而之间并没有平滑过渡，所以当坐标变化比较剧烈时，线与线之间的转折就显得特别明显了。


所以要想优化这种效果，就得实现线与线之间的平滑过渡，很显然，二阶贝赛尔曲线就是干这个事的。下面我们就利用我们新学的Path.quadTo函数来重新实现下移动轨迹效果。


### 2、实现方式二（优化）：使用Path.quadTo()函数实现过渡

#### (1)、原理概述 

我们上面讲了，使用Path.lineTo()的最大问题就是线段转折处不够平滑。Path.quadTo()可以实现平滑过渡，但使用Path.quadTo()的最大问题是，如何找到起始点和结束点。


下图中，有用绿点表示的三个点，连成的两条直线，很明显他们转折处是有明显折痕的
![](https://img-blog.csdn.net/20160328205249349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面我们在PhotoShop中利用钢笔工具，看如何才能实现这两条线之间的转折


![](https://img-blog.csdn.net/20160328205647198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20160328205659876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


从这两个线段中可以看出，我们使用Path.lineTo（）的时候，是直接把手指触点A,B,C给连起来。 

而钢笔工具要实现这三个点间的流畅过渡，就只能将这两个线段的中间点做为起始点和结束点，而将手指的倒数第二个触点B做为控制点。 

大家可能会觉得，那这样，在结束的时候，A到P0和P1到C1的这段距离岂不是没画进去？是的，如果Path最终没有close的话，这两段距离是被抛弃掉的。因为手指间滑动时，每两个点间的距离很小，所以P1到C之间的距离可以忽略不计。


下面我们就利用这种方法在photoshop中求证，在连接多个线段时，是否能行？


![](https://img-blog.csdn.net/20160328205739767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在这个图形中，有很多点连成了弯弯曲曲的线段，我们利用上面我们讲的，将两个线段的中间做为二阶贝尔赛曲线的起始点和终点，把上一个手指的位置做为控制点，来看看是否真的能组成平滑的连线


整个连接过程如动画所示：


![](https://img-blog.csdn.net/20160328210009658)


在最终的路径中看来，各个点间的连线是非常平滑的。从这里也可以看出，在为了实现平滑效果，我们只能把开头的线段一半和结束的线段的一半抛弃掉。 

在讲了原理之后，下面就来看看在代码中如何来实现吧。 


#### （2）、自定义View 

先贴出完整代码然后再细讲：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- private Path mPath = new Path();  
- privatefloat mPreX,mPreY;  
- 
- public MyView(Context context) {  
- super(context);  
-     }  
- 
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-     }  
- 
- @Override
- publicboolean onTouchEvent(MotionEvent event) {  
- switch (event.getAction()){  
- case MotionEvent.ACTION_DOWN:{  
-                 mPath.moveTo(event.getX(),event.getY());  
-                 mPreX = event.getX();  
-                 mPreY = event.getY();  
- returntrue;  
-             }  
- case MotionEvent.ACTION_MOVE:{  
- float endX = (mPreX+event.getX())/2;  
- float endY = (mPreY+event.getY())/2;  
-                 mPath.quadTo(mPreX,mPreY,endX,endY);  
-                 mPreX = event.getX();  
-                 mPreY =event.getY();  
-                 invalidate();  
-             }  
- break;  
- default:  
- break;  
-         }  
- returnsuper.onTouchEvent(event);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-         Paint paint = new Paint();  
-         paint.setStyle(Paint.Style.STROKE);  
-         paint.setColor(Color.GREEN);  
-         paint.setStrokeWidth(2);  
- 
-         canvas.drawPath(mPath,paint);  
-     }  
- 
- publicvoid reset(){  
-         mPath.reset();  
-         postInvalidate();  
-     }  
- }  


最难的部分依然是onTouchEvent函数这里：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicboolean onTouchEvent(MotionEvent event) {  
- switch (event.getAction()){  
- case MotionEvent.ACTION_DOWN:{  
-             mPath.moveTo(event.getX(),event.getY());  
-             mPreX = event.getX();  
-             mPreY = event.getY();  
- returntrue;  
-         }  
-         …………  
-     }  
- returnsuper.onTouchEvent(event);  
- }  


在ACTION_DOWN的时候，利用 mPath.moveTo(event.getX(),event.getY())将Path的初始位置设置到手指的触点处，如果不调用mPath.moveTo的话，会默认是从(0,0)开始的。然后我们定义两个变量mPreX，mPreY来表示手指的前一个点。我们通过上面的分析知道，这个点是用来做控制点的。最后return true让ACTION_MOVE,ACTION_UP事件继续向这个控件传递。


在ACTION_MOVE时：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- case MotionEvent.ACTION_MOVE:{  
- 
- float endX = (mPreX+event.getX())/2;  
- float endY = (mPreY+event.getY())/2;  
-     mPath.quadTo(mPreX,mPreY,endX,endY);  
-     mPreX = event.getX();  
-     mPreY =event.getY();  
-     invalidate();  
- }  


我们先找到结束点，我们说了结束点是这个线段的中间位置，所以很容易求出它的坐标endX,endY；控制点是上一个手指位置即mPreX,mPreY;那有些同学可能会问了，那起始点是哪啊。在开篇讲quadTo()函数时，就已经说过，第一个起始点是Path.moveTo(x,y)定义的，其它部分，一个quadTo的终点，是下一个quadTo的起始点。


所以这里的起始点，就是上一个线段的中间点。所以，这样就与钢笔工具绘制过程完全对上了：把各个线段的中间点做为起始点和终点，把终点前一个手指位置做为控制点。 

后面的onDraw()和reset()函数就没什么难度了，上面的例子中也讲过了，就不再赘述了 

最终的效果图如下：
![](https://img-blog.csdn.net/20160328210113032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同样把lineTo和quadTo实现的S拿来对比下：
![](https://img-blog.csdn.net/20160328210149409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从效果图中可以明显可以看出，通过quadTo实现的曲线更顺滑 
**源码在文章底部给出 **

Ok啦，quadeTo的用法，到这里就结束了，下部分再来讲讲rQuadTo的用法及波浪动画效果


![](https://img-blog.csdn.net/20160328210306753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 四、Path.rQuadTo()

### 1、概述

该函数声明如下




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicvoid rQuadTo(float dx1, float dy1, float dx2, float dy2)  

**其中：**

- dx1:控制点X坐标，表示相对上一个终点X坐标的位移坐标，可为负值，正值表示相加，负值表示相减；
- dy1:控制点Y坐标，相对上一个终点Y坐标的位移坐标。同样可为负值，正值表示相加，负值表示相减；
- dx2:终点X坐标，同样是一个相对坐标，相对上一个终点X坐标的位移值，可为负值，正值表示相加，负值表示相减；
- dy2:终点Y坐标，同样是一个相对，相对上一个终点Y坐标的位移值。可为负值，正值表示相加，负值表示相减；


这四个参数都是传递的都是相对值，相对上一个终点的位移值。 

比如，我们上一个终点坐标是(300,400)那么利用rQuadTo(100,-100,200,100)； 

得到的控制点坐标是（300+100,400-100）即(500,300) 

同样，得到的终点坐标是(300+200,400+100)即(500,500) 

所以下面这两段代码是等价的： 

利用quadTo定义绝对坐标




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- path.moveTo(300,400);  
- path.quadTo(500,300,500,500);  


与利用rQuadTo定义相对坐标



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- path.moveTo(300,400);  
- path.rQuadTo(100,-100,200,100)  


### 2、使用rQuadTo实现波浪线

在上篇中，我们使用quadTo实现了一个简单的波浪线：
![](https://img-blog.csdn.net/20160328210540879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20160328210604520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


各个点具体计算过程，在上篇已经计算过了，下面是上篇中onDraw的代码：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
- 
-     Paint paint = new Paint();  
-     paint.setStyle(Paint.Style.STROKE);  
-     paint.setColor(Color.GREEN);  
- 
-     Path path = new Path();  
-     path.moveTo(100,300);  
-     path.quadTo(200,200,300,300);  
-     path.quadTo(400,400,500,300);  
- 
-     canvas.drawPath(path,paint);  
- }  


下面我们将它转化为rQuadTo来重新实现下：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
- 
-     Paint paint = new Paint();  
-     paint.setStyle(Paint.Style.STROKE);  
-     paint.setColor(Color.GREEN);  
- 
-     Path path = new Path();  
-     path.moveTo(100,300);  
-     path.rQuadTo(100,-100,200,0);  
-     path.rQuadTo(100,100,200,0);  
-     canvas.drawPath(path,paint);  
- }  


简单来讲，就是将原来的：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- path.moveTo(100,300);  
- path.quadTo(200,200,300,300);  
- path.quadTo(400,400,500,300);  


转化为：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- path.moveTo(100,300);  
- path.rQuadTo(100,-100,200,0);  
- path.rQuadTo(100,100,200,0);  

第一句：path.rQuadTo(100,-100,200,0);是建立在（100,300）这个点基础上来计算相对坐标的。


所以 
控制点X坐标=上一个终点X坐标+控制点X位移 = 100+100=200； 

控制点Y坐标=上一个终点Y坐标+控制点Y位移 = 300-100=200； 

终点X坐标 = 上一个终点X坐标+终点X位移 = 100+200=300； 

终点Y坐标 = 上一个终点Y坐标+控制点Y位移 = 300+0=300; 

所以这句与path.quadTo(200,200,300,300);对等的 
第二句：path.rQuadTo(100,100,200,0);是建立在它的前一个终点即(300,300)的基础上来计算相对坐标的！


所以 
控制点X坐标=上一个终点X坐标+控制点X位移 = 300+100=200； 

控制点Y坐标=上一个终点Y坐标+控制点Y位移 = 300+100=200； 

终点X坐标 = 上一个终点X坐标+终点X位移 = 300+200=500； 

终点Y坐标 = 上一个终点Y坐标+控制点Y位移 = 300+0=300; 

所以这句与path.quadTo(400,400,500,300);对等的 

最终效果也是一样的。 

通过这个例子，只想让大家明白一点：rQuadTo(float dx1, float dy1, float dx2, float dy2)中的位移坐标，都是以上一个终点位置为基准来做偏移的！

## 五、实现波浪效果

本节完成之后，将实现文章开头的波浪效果，如下。
![](https://img-blog.csdn.net/20160328210844115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 1、实现全屏波纹

上面我们已经能够实现一个波形，只要我们再多实现几个波形，就可以覆盖整个屏幕了。
![](https://img-blog.csdn.net/20160328210940537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对应代码如下：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- private Paint mPaint;  
- private Path mPath;  
- privateint mItemWaveLength = 400;  
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-         mPath = new Path();  
-         mPaint = new Paint();  
-         mPaint.setColor(Color.GREEN);  
-         mPaint.setStyle(Paint.Style.STROKE);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-         mPath.reset();  
- int originY = 300;  
- int halfWaveLen = mItemWaveLength/2;  
-         mPath.moveTo(-mItemWaveLength,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-             mPath.rQuadTo(halfWaveLen/2,-50,halfWaveLen,0);  
-             mPath.rQuadTo(halfWaveLen/2,50,halfWaveLen,0);  
-         }  
- 
-         canvas.drawPath(mPath,mPaint);  
-     }  
- }  


最难的部分依然是在onDraw函数中：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-     mPath.reset();  
- int originY = 300;  
- int halfWaveLen = mItemWaveLength/2;  
-     mPath.moveTo(-mItemWaveLength,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-         mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-         mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
-     }  
-     canvas.drawPath(mPath,mPaint);  
- }  


我们将mPath的起始位置向左移一个波长：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- mPath.moveTo(-mItemWaveLength,originY);  


然后利用for循环画出当前屏幕中可能容得下的所有波：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-     mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-     mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
- }  


mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);画的是一个波长中的前半个波，mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);画的是一个波长中的后半个波。大家在这里可以看到，屏幕左右都多画了一个波长的图形。这是为了波形移动做准备的。


到这里，我们是已经能画出来一整屏幕的波形了，下面我们把整体波形闭合起来
![](https://img-blog.csdn.net/20160328211245195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中，图中红色区域是我标出来利用lineTo闭合的区域




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- private Paint mPaint;  
- private Path mPath;  
- privateint mItemWaveLength = 400;  
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-         mPath = new Path();  
-         mPaint = new Paint();  
-         mPaint.setColor(Color.GREEN);  
-         mPaint.setStyle(Paint.Style.FILL_AND_STROKE);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-         mPath.reset();  
- int originY = 300;  
- int halfWaveLen = mItemWaveLength/2;  
-         mPath.moveTo(-mItemWaveLength+dx,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-             mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-             mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
-         }  
-         mPath.lineTo(getWidth(),getHeight());  
-         mPath.lineTo(0,getHeight());  
-         mPath.close();  
- 
-         canvas.drawPath(mPath,mPaint);  
-     }  
- }  


这段代码相比上面的代码，增加了两部分内容： 

第一，将paint设置为填充：mPaint.setStyle(Paint.Style.FILL_AND_STROKE); 

第二，将path闭合：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- mPath.moveTo(-mItemWaveLength+dx,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-     mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-     mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
- }  
- mPath.lineTo(getWidth(),getHeight());  
- mPath.lineTo(0,getHeight());  
- mPath.close();  


### 2、实现移动动画

让波纹动起来其实挺简单，利用调用在path.moveTo的时候，将起始点向右移动即可实现移动，而且只要我们移动一个波长的长度，波纹就会重合，就可以实现无限循环了。


为此我们定义一个动画：




**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicvoid startAnim(){  
-     ValueAnimator animator = ValueAnimator.ofInt(0,mItemWaveLength);  
-     animator.setDuration(2000);  
-     animator.setRepeatCount(ValueAnimator.INFINITE);  
-     animator.setInterpolator(new LinearInterpolator());  
-     animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {  
- @Override
- publicvoid onAnimationUpdate(ValueAnimator animation) {  
-             dx = (int)animation.getAnimatedValue();  
-             postInvalidate();  
-         }  
-     });  
-     animator.start();  
- }  


动画的长度为一个波长，将当前值保存在类的成员变量dx中； 

然后在画图的时候，在path.moveTo（）中加上现在的移动值dx:mPath.moveTo(-mItemWaveLength+dx,originY); 

完整的绘图代码如下：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-     mPath.reset();  
- int originY = 300;  
- int halfWaveLen = mItemWaveLength/2;  
-     mPath.moveTo(-mItemWaveLength+dx,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-         mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-         mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
-     }  
-     mPath.lineTo(getWidth(),getHeight());  
-     mPath.lineTo(0,getHeight());  
-     mPath.close();  
- 
-     canvas.drawPath(mPath,mPaint);  
- }  


完整的MyView代码如下：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyView extends View {  
- private Paint mPaint;  
- private Path mPath;  
- privateint mItemWaveLength = 400;  
- privateint dx;  
- public MyView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-         mPath = new Path();  
-         mPaint = new Paint();  
-         mPaint.setColor(Color.GREEN);  
-         mPaint.setStyle(Paint.Style.FILL_AND_STROKE);  
-     }  
- 
- @Override
- protectedvoid onDraw(Canvas canvas) {  
- super.onDraw(canvas);  
-         mPath.reset();  
- int originY = 300;  
- int halfWaveLen = mItemWaveLength/2;  
-         mPath.moveTo(-mItemWaveLength+dx,originY);  
- for (int i = -mItemWaveLength;i<=getWidth()+mItemWaveLength;i+=mItemWaveLength){  
-             mPath.rQuadTo(halfWaveLen/2,-100,halfWaveLen,0);  
-             mPath.rQuadTo(halfWaveLen/2,100,halfWaveLen,0);  
-         }  
-         mPath.lineTo(getWidth(),getHeight());  
-         mPath.lineTo(0,getHeight());  
-         mPath.close();  
- 
-         canvas.drawPath(mPath,mPaint);  
-     }  
- 
- publicvoid startAnim(){  
-         ValueAnimator animator = ValueAnimator.ofInt(0,mItemWaveLength);  
-         animator.setDuration(2000);  
-         animator.setRepeatCount(ValueAnimator.INFINITE);  
-         animator.setInterpolator(new LinearInterpolator());  
-         animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {  
- @Override
- publicvoid onAnimationUpdate(ValueAnimator animation) {  
-                 dx = (int)animation.getAnimatedValue();  
-                 postInvalidate();  
-             }  
-         });  
-         animator.start();  
-     }  
- }  


然后在MyActivity中开始动画：



**[java]**[view plain](http://blog.csdn.net/harvic880925/article/details/50995587#)[copy](http://blog.csdn.net/harvic880925/article/details/50995587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- publicclass MyActivity extends Activity {  
- /**
-      * Called when the activity is first created.
-      */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
- final MyView myView = (MyView)findViewById(R.id.myview);  
-         myView.startAnim();  
-     }  
- }  


这样就实现了动画：
![](https://img-blog.csdn.net/20160328211527899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果把波长设置为1000，就可以实现本段开篇的动画了。 

如果想让波纹像开篇时那要同时向下移动，大家只需要在path.moveTo(x,y)的时候，通过动画同时移动y坐标就可以了，代码比较简单，而且本文实在是太长了，具体实现就不再讲了，大家可以在源码中加以尝试。

**源码在文章底部给出 **

好了，本篇文章到这里就结束了





如果本文有帮到你，记得加关注哦

**源码下载地址：[http://download.csdn.net/detail/harvic880925/9476153](http://download.csdn.net/detail/harvic880925/9476153)**





