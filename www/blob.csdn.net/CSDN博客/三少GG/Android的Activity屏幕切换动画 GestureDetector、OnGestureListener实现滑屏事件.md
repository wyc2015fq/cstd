# Android的Activity屏幕切换动画||GestureDetector、OnGestureListener实现滑屏事件 - 三少GG - CSDN博客
2012年04月17日 15:24:35[三少GG](https://me.csdn.net/scut1135)阅读数：2811标签：[android																[encoding																[animation																[float																[layout																[button](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
### [[置顶] Android的Activity屏幕切换动画(一)-左右滑动切换](http://blog.csdn.net/heavenforgold/article/details/7074136)
分类： [Android](http://blog.csdn.net/heavenforgold/article/category/952853)2011-12-15
 14:53133人阅读[评论](http://blog.csdn.net/heavenforgold/article/details/7074136#comments)(0)[收藏](http://blog.csdn.net/heavenforgold/article/details/7074136)[举报](http://blog.csdn.net/heavenforgold/article/details/7074136#report)
这段时间一直在忙Android的项目，总算抽出点时间休息一下，准备把一些项目用到的Android经验分享一下。
在Android开发过程中，经常会碰到Activity之间的切换效果的问题，下面介绍一下如何实现左右滑动的切换效果，首先了解一下Activity切换的实现，从Android2.0开始在Activity增加了一个方法：
public void overridePendingTransition(int
 enterAnim, int exitAnim)
其中：
enterAnim 定义Activity进入屏幕时的动画
exitAnim 定义Activity退出屏幕时的动画
overridePendingTransition 方法必须在startActivity()或者 finish()方法的后面。
Android已经内置了几种动画效果，可以见 android.R.anim 类。一般情况下我们需要自己定义屏幕切换的效果。首先我们先了解Activity的位置定义，如下图：
![](http://hi.csdn.net/attachment/201112/15/0_132393186688Gh.gif)
从上图可以看出，以手机屏幕下面边未X轴，屏幕左边为Y轴，当Activity在X轴值为-100%p时，刚好在屏幕的左边（位置1），当X轴值为0%p时，刚好再屏幕内(位置2），当X=100%p时刚好在屏幕右边（位置3）。
清楚了位置后，我们就可以实现左右滑动的切换效果，首先让要退出的Activity从位置2移动到位置1，同时让进入的Activity从位置3移动位置2，这样就能实现从左右切换效果。
实现过程如下，首先定义2个动画，在 res目录创建anim目录， 然后在目录创建动画的xml文件：out_to_left.xml (从左边退出动画) 、in_from_right.xml(从右边进入动画)
out_to_left.xml (从 位置2 移动到 位置1)
**[html]**[view
 plain](http://blog.csdn.net/heavenforgold/article/details/7074136#)[copy](http://blog.csdn.net/heavenforgold/article/details/7074136#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <setxmlns:android="http://schemas.android.com/apk/res/android"android:interpolator="@android:anim/accelerate_interpolator">
- <translateandroid:fromXDelta="0%p"android:toXDelta="-100%p"
- android:duration="500"/>
- </set>
in_from_right.xml (从
 位置3 移动到 位置2)
**[html]**[view
 plain](http://blog.csdn.net/heavenforgold/article/details/7074136#)[copy](http://blog.csdn.net/heavenforgold/article/details/7074136#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <setxmlns:android="http://schemas.android.com/apk/res/android"android:interpolator="@android:anim/accelerate_interpolator">
- <translateandroid:fromXDelta="100%p"android:toXDelta="0%p"
- android:duration="500"/>
- </set>
注： android:fromXDelta 动画开始的位置，  android:toXDelta 动画结束的位置，android:duration动画的时间。
Android 代码如下：
**[java]**[view
 plain](http://blog.csdn.net/heavenforgold/article/details/7074136#)[copy](http://blog.csdn.net/heavenforgold/article/details/7074136#)
- publicclass LeftRightSlideActivity extends Activity {  
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);     
- 
-         Button button = (Button)findViewById(R.id.button1);  
-         button.setOnClickListener(new View.OnClickListener() {    
- @Override
- publicvoid onClick(View v) {  
-                 Intent intent = new Intent();  
-                 intent.setClass(LeftRightSlideActivity.this, SlideSecondActivity.class);  
-                 startActivity(intent);  
- //设置切换动画，从右边进入，左边退出
-                 overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);                  
-             }  
-         });  
-     }  
- }  
效果图如下：
![](http://hi.csdn.net/attachment/201112/15/0_13239320557ZuU.gif)![](http://hi.csdn.net/attachment/201112/15/0_13239320608AcY.gif)
虽然左右滑动切换的实现很简单，但是对于原理的理解很重要，掌握了原理可以充分发挥想象力设计出各种各样的动画效果，希望对一些入门的新手有帮助。
以后在慢慢整理一些项目中用到的切换动画效果。
代码见：[点击打开链接](http://www.oschina.net/action/code/download?code=7692&id=12253)
### [[置顶] Android的Activity屏幕切换动画(二)-左右滑动深入与实战](http://blog.csdn.net/heavenforgold/article/details/7081484)
2011-12-18 13:29306人阅读[评论](http://blog.csdn.net/heavenforgold/article/details/7081484#comments)(0)[收藏](http://blog.csdn.net/heavenforgold/article/details/7081484)[举报](http://blog.csdn.net/heavenforgold/article/details/7081484#report)
上一篇文章将了 Android的左右滑动切换(见[](http://blog.csdn.net/heavenforgold/article/details/7074136)[Android的Activity屏幕切换动画(一)-左右滑动切换](http://blog.csdn.net/heavenforgold/article/details/7074136)），实现过程是非常简单，一些新手可能会向深入了了解Activity切换的原理，下面主要对左右滑动进行深入的探讨，并以项目中的一个切换效果来进一步了解。
Activity的切换效果使用的是Android的动画效果，Android的动画在官方有相关资料：[http://developer.android.com/guide/topics/graphics/animation.html](http://developer.android.com/guide/topics/graphics/animation.html)和[http://developer.android.com/guide/topics/resources/animation-resource.html](http://developer.android.com/guide/topics/resources/animation-resource.html)，Activity的切换动画实际上是Android的View
 Animation(视图动画)中的Tween Animation效果，Tween Animation分为4种动画效果，分别是：alpha (透明变化) translate(位置移动) scale(缩放) rotate(旋转), 而左右滑动切换使用的是 translate(位置移动)的效果，在下一篇我们再讨论一下 alpha (透明变化) 、scale(缩放)、 rotate(旋转) 这三种效果，本篇只将 translate(位置移动)。
Translate动画是非常好理解，就是定义一个开始的位置和一个结束位置，定义移动时间，然后就能自动产生移动动画。Android的translate移动方向有 横向(X) 竖向(Y), 左右滑动使用了横向移动效果，对于竖向(Y)的位置如下：
![](http://hi.csdn.net/attachment/201112/18/0_1324186299hno4.gif)
定义一个向上退出的动画（从位置2 移动位置3）和从下面进入（从位置1移动位置2）的动画定义文件如下：
out_to_up.xml (从屏幕上面退出)
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<translate xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:interpolator=*"@android:anim/accelerate_interpolator"*
    android:fromYDelta=*"0%p"*
android:toYDelta=*"-100%p"*
    android:duration=*"1000"*>
</translate>
in_from_down.xml (从屏幕下面进入)
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<translate xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:interpolator=*"@android:anim/accelerate_interpolator"*
    android:fromYDelta=*"100%p"*
    android:toYDelta=*"0%p"*
    android:duration=*"1000"*>
</translate>
接下来讲一讲translate的几个重要的属性：
android:interpolator: 加速器，非常有用的属性，可以简单理解为动画的速度，可以是越来越快，也可以是越来越慢，或者是先快后忙，或者是均匀的速度等等，对于值如下：
@android:anim/accelerate_interpolator： 越来越快
@android:anim/decelerate_interpolator：越来越慢
@android:anim/accelerate_decelerate_interpolator：先快后慢
@android:anim/anticipate_interpolator: 先后退一小步然后向前加速
@android:anim/overshoot_interpolator：快速到达终点超出一小步然后回到终点
@android:anim/anticipate_overshoot_interpolator：到达终点超出一小步然后回到终点
@android:anim/bounce_interpolator：到达终点产生弹球效果，弹几下回到终点
@android:anim/linear_interpolator：均匀速度。
android:duration: 动画运行时间，定义在多次时间（ms）内完成动画
android:startOffset: 延迟一定时间后运行动画
fromXDelta: X轴方向开始位置，可以是%，也可以是具体的像素 具体见图
toXDelta:   X轴方向结束位置，可以是%，也可以是具体的像素
fromYDelta: Y轴方向开始位置，可以是%，也可以是具体的像素
toYDelta:    Y轴方向结束位置，可以是%，也可以是具体的像素
当你了解上面的属性后，你可以组合出很多有趣的位置移动效果。比如使用加速器：@android:anim/bounce_interpolator 可以产生弹球落地时的效果。
在实际项目中需要在进行只使用“左右上下滑动”的效果可能还不够炫，希望在切换时能产生更动态的效果，比如开始切换时第1个Activity先向后退一步，然后在向左方向退出屏幕。然后第2个Activity紧跟后面从右边进入屏幕，到达终点时有一个动态效果。在了解了动画加速器后，大家都知道可以采用：anticipate_overshoot_interpolator 、anticipate_interpolator、overshoot_interpolator这三个加速器实现左右滑动切换时启动或者结束的动态效果。
动画文件定义如下：
dync_out_to_left.xml 
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<translate xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:duration=*"500"*
    android:fromXDelta=*"0%p"*
    android:interpolator=*"@android:anim/anticipate_interpolator"*
    android:toXDelta=*"-100%p"* />
dync_in_from_right.xml
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<translate xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:duration=*"500"*
    android:fromXDelta=*"100%p"*
    android:interpolator=*" @android:anim/anticipate_overshoot_interpolator "*
    android:toXDelta=*"0%p"* />
但是再使用以上动画配置，会出现以下问题：
1.     第1个Activity使用anticipate_interpolator时，开始移动时先向后退一步，然后向前移动。但向后退一步步伐太大，略显夸张，实际应用中只希望退后一小小步就行了。
2.     由于开始向后退一步，额外增加了移动时间，导致前一Activity和后一个Activity的时间不同步。
为了解决以上问题，重新定义动态效果，将第1个Activity的移动分为2个动画效果：（1）用200毫秒时间先向后移动2%p的位置 (2) 延迟200毫秒后从2%p位置向前移动到-100%p位置。 然后第2个Activity从延迟200毫秒然后从102%p位置移动到0%p位置。
动画定义如下：
new_dync_out_to_left.xml
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<set xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:shareInterpolator=*"false"* >
    <translate
        android:duration=*"200"*
        android:fromXDelta=*"0%p"*
        android:interpolator=*"@android:anim/accelerate_decelerate_interpolator"*
        android:toXDelta=*"2%p"* />
    <translate
        android:duration=*"1000"*
        android:fromXDelta=*"2%p"*
        android:interpolator=*"@android:anim/accelerate_interpolator"*
        android:startOffset=*"200"*
        android:toXDelta=*"-100%p"* />
</set>
new_dync_in_from_right.xml
<?xml version=*"1.0"* encoding=*"utf-8"*?>
<translate xmlns:android=*"http://schemas.android.com/apk/res/android"*
    android:duration=*"1000"*
    android:fromXDelta=*"102%p"*
    android:interpolator=*"@android:anim/anticipate_overshoot_interpolator"*
    android:startOffset=*"200"*
    android:toXDelta=*"0%p"* />
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [Android滑动切换activity](http://gaofen100.iteye.com/blog/1249834)
屏幕切换指的是在同一个Activity内屏幕见的切换。
通过GestureDetector、OnGestureListener实现滑屏事件。ViewFlipper是继承至FrameLayout的，所以它是一个Layout里面可以放置多个View。示例中第一页仅放了一个按钮BUTTON，向下滑屏时，每页都只放了一个TEXTVIEW。
页面：
<?xml version="1.0" encoding="utf-8"?> 
<LinearLayout xmlns:[Android](http://gaofen100.iteye.com/blog/index.php?doc-view-1789)="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)" 
android:orientation="vertical" 
android:layout_width="fill_parent" 
android:layout_height="fill_parent" 
> 
<ViewFlipper android:id="@+id/ViewFlipper01" 
android:layout_width="fill_parent" android:layout_height="fill_parent"> 
</ViewFlipper> 
</LinearLayout> 
Activity代码:
view plaincopy to clipboardprint?
package com.yfz; 
import android.app.Activity; 
import android.os.Bundle; 
import android.util.Log; 
import android.view.GestureDetector; 
import android.view.MotionEvent; 
import android.view.View; 
import android.view.GestureDetector.OnGestureListener; 
import android.view.ViewGroup.LayoutParams; 
import android.view.animation.AnimationUtils; 
import android.widget.Button; 
import android.widget.ExpandableListView; 
import android.widget.TextView; 
import android.widget.ViewFlipper; 
public class Flip extends Activity implements OnGestureListener { 
private GestureDetector detector; 
private ViewFlipper flipper; 
/** Called when the activity is first created. */ 
@Override 
public void onCreate(Bundle savedInstanceState) { 
super.onCreate(savedInstanceState); 
setContentView(R.layout.main); 
flipper = (ViewFlipper) this.findViewById(R.id.ViewFlipper01); 
flipper.addView(addButtonByText("按钮"),new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT)); 
detector = new GestureDetector(this); 
} 
public View addButtonByText(String text){ 
Button btn = new Button(this); 
btn.setText(text); 
return btn; 
} 
public View addTextByText(String text){ 
TextView tv = new TextView(this); 
tv.setText(text); 
tv.setGravity(1); 
return tv; 
} 
@Override 
public boolean onTouchEvent(MotionEvent event) { 
Log.i("Fling", "Activity onTouchEvent!"); 
return this.detector.onTouchEvent(event); 
} 
@Override 
public boolean onDown(MotionEvent e) { 
// TODO Auto-generated method stub 
return false; 
} 
/** 
* 监听滑动 
*/ 
@Override 
public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, 
float velocityY) { 
// TODO Auto-generated method stub 
Log.i("Fling", "Fling Happened!"); 
if (e1.getX() - e2.getX() > 120) { 
this.flipper.setInAnimation(AnimationUtils.loadAnimation(this,R.anim.push_left_in)); 
this.flipper.setOutAnimation(AnimationUtils.loadAnimation(this,R.anim.push_left_out)); 
this.flipper.addView(addTextByText("文本框"),new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT)); 
this.flipper.showNext(); 
return true; 
} else if (e1.getX() - e2.getX() < -120) { 
this.flipper.setInAnimation(AnimationUtils.loadAnimation(this,R.anim.push_right_in)); 
this.flipper.setOutAnimation(AnimationUtils.loadAnimation(this,R.anim.push_right_out)); 
this.flipper.showPrevious(); 
return true; 
} 
return true; 
} 
@Override 
public void onLongPress(MotionEvent e) { 
// TODO Auto-generated method stub 
} 
@Override 
public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, 
float distanceY) { 
// TODO Auto-generated method stub 
return false; 
} 
@Override 
public void onShowPress(MotionEvent e) { 
// TODO Auto-generated method stub 
} 
@Override 
public boolean onSingleTapUp(MotionEvent e) { 
// TODO Auto-generated method stub 
return false; 
} 
} 
这里实现的功能是从右往左滑动则切换到上一个View，从左往右滑动则切换到下一个View，并且使用不同的in、out 动画使切换效果看起来统一一些。
滑屏实现：
通过android.view.GestureDetector类可以检测各种手势事件，该类有两个回调接口分别用来通知具体的事件：
GestureDetector.OnDoubleTapListener：用来通知DoubleTap事件，类似于鼠标的双击事件，该接口有如下三个回调函数：
1. onDoubleTap(MotionEvent e)：通知DoubleTap手势，
2. onDoubleTapEvent(MotionEvent e)：通知DoubleTap手势中的事件，包含down、up和move事件（这里指的是在双击之间发生的事件，例如在同一个地方双击会产生DoubleTap手势，而在DoubleTap手势里面还会发生down和up事件，这两个事件由该函数通知）；
3. onSingleTapConfirmed(MotionEvent e)：用来判定该次点击是SingleTap而不是DoubleTap，如果连续点击两次就是DoubleTap手势，如果只点击一次，系统等待一段时间后没有收到第二次点击则判定该次点击为SingleTap而不是DoubleTap，然后触发SingleTapConfirmed事件。
GestureDetector.OnGestureListener：用来通知普通的手势事件，该接口有如下六个回调函数：
1. onDown(MotionEvent e)：down事件；
2. onSingleTapUp(MotionEvent e)：一次点击up事件；
3. onShowPress(MotionEvent e)：down事件发生而move或则up还没发生前触发该事件；
4. onLongPress(MotionEvent e)：长按事件；
5. onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)：滑动手势事件；
6. onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)：在屏幕上拖动事件。
本次仅用到了OnGestureListener类中的onFling方法。 其他手势事件各位可以自己回去试验。
简单说明：
要实现屏幕切换的话，首先需要定义一个GestureDetector：
private GestureDetector mGestureDetector;
并在onCreate函数中初始化：
mGestureDetector = new GestureDetector(this);
同时Activity要继承OnGestureListener接口，并实现其中的onFling方法。
另外Activity的onTouchEvent事件也要实现！！
另外本例View切换时还有动画效果。使用Animation类实现，相关的函数： 
setInAnimation：设置View进入屏幕时候使用的动画，该函数有两个版本:
一个接受单个参数，类型为android.view.animation.Animation；
一个接受两个参数，类型为Context和int，分别为Context对象和定义Animation的resourceID。
setOutAnimation: 设置View退出屏幕时候使用的动画，参数setInAnimation函数一样。
showNext： 调用该函数来显示FrameLayout里面的下一个View。
showPrevious： 调用该函数来显示FrameLayout里面的上一个View。
动画源文件: （在res目录下，创建一个anim文件夹，把下面的文件都放在这里）
push_left_in.xml
view plaincopy to clipboardprint?
<?xml version="1.0" encoding="utf-8"?> 
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
<translate android:fromXDelta="100%p" android:toXDelta="0" 
android:duration="500" /> 
<alpha android:fromAlpha="0.1" android:toAlpha="1.0" 
android:duration="500" /> 
</set> 
push_left_out.xml:
view plaincopy to clipboardprint?
<?xml version="1.0" encoding="utf-8"?> 
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
<translate android:fromXDelta="0" android:toXDelta="-100%p" 
android:duration="500" /> 
<alpha android:fromAlpha="1.0" android:toAlpha="0.1" 
android:duration="500" /> 
</set> 
push_right_in.xml：
view plaincopy to clipboardprint?
<?xml version="1.0" encoding="utf-8"?> 
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
<translate android:fromXDelta="-100%p" android:toXDelta="0" 
android:duration="500" /> 
<alpha android:fromAlpha="0.1" android:toAlpha="1.0" 
android:duration="500" /> 
</set> 
push_right_out.xml：
view plaincopy to clipboardprint?
<?xml version="1.0" encoding="utf-8"?> 
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"> 
<translate android:fromXDelta="0" android:toXDelta="100%p" 
android:duration="500" /> 
<alpha android:fromAlpha="1.0" android:toAlpha="0.1" 
android:duration="500" /> 
</set> 
