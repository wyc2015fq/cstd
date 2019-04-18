# NestedScrollingParent, NestedScrollingChild  详解 - weixin_33985507的博客 - CSDN博客
2018年01月07日 16:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
之前听同事提起过 NestedScrollingView，但是一直没有时间去了解，最近一段时间比较空，才开始去了解。先点开，看 NestedScrollingView 源码：
```
public class NestedScrollView extends FrameLayout implements NestedScrollingParent, NestedScrollingChild, ScrollingView
```
可以发现实现了两个没有见过的接口：NestedScrollingParent, NestedScrollingChild，也是这篇文章的主角。 Android 就是通过这两个接口, 来实现 子 View 与父View 之间的嵌套滑动。这样的嵌套滑动机制是在 Android 发布 Lollipop 之后提供的。不过同样在Support v7 中同样支持了。同时 RecycleView  以及 Android 5.0 以上的系统原声 View 大部分都已经支持嵌套滑动了 。
NestedScrolling 机制能够让父 View 和子 View 在滚动式进行配合，而要实现这样的交互机制，首先父 view 要实现 NestedScrollingParent 接口，而子 View 需要实现 NestedScrollingChild 接口，在这套机制中子 View是发起者，父 view 是接受回调并做出响应的。
## 四个主要接口
当我们自己再去写的时候，会碰到以下这四个类（接口）：
```
// 主要接口
NestedScrollingChild
NestedScrollingParent
// 帮助类
NestedScrollingChildHelper
NestedScrollingParentHelper
```
正如前面所说的，很多 view 现在都实现了这两个接口：
- 
NestedScrollView 已经实现了 NestedScrollingChild 和 NestedScrollingParent 两个接口
- 
RecycleView 已经实现了 NestedScrollingChild
- 
CoordinatorLayout 实现了 NestedScrollingParent
那实现这两个接口都需要做哪些工作呢？
要实现 NestedScrollingChild 接口，那么需要重写下面几个方法：
```
// 开始、停止嵌套滚动
public boolean startNestedScroll(int axes); 
public void stopNestedScroll();
// 触摸滚动相关
public boolean dispatchNestedPreScroll(int dx, int dy, int[] consumed, int[] offsetInWindow);
public boolean dispatchNestedScroll(int dxConsumed, int dyConsumed, int dxUnconsumed, int dyUnconsumed, int[] offsetInWindow);
// 惯性滚动相关 public boolean dispatchNestedPreFling(float velocityX, float velocityY);
public boolean dispatchNestedFling(float velocityX, float velocityY, boolean consumed);
public boolean dispatchNestedPreScroll(int dx, int dy, int[] consumed, int[] offsetInWindow)；
// 设置是否能够滑动
public void setNestedScrollingEnabled(boolean enabled)；
public boolean isNestedScrollingEnabled()；
```
 要实现 NestedScrollingParent 接口，那么需要重写下面几个方法：
```
// 当开启、停止嵌套滚动时被调用
public boolean onStartNestedScroll(View child, View target, int nestedScrollAxes);
public void onNestedScrollAccepted(View child, View target, int nestedScrollAxes);
public void onStopNestedScroll(View target);
// 当触摸嵌套滚动时被调用
public void onNestedPreScroll(View target, int dx, int dy, int[] consumed);
public void onNestedScroll(View target, int dxConsumed, int dyConsumed, int dxUnconsumed, int dyUnconsumed);
// 当惯性嵌套滚动时被调用
public boolean onNestedPreFling(View target, float velocityX, float velocityY);
public boolean onNestedFling(View target, float velocityX, float velocityY, boolean consumed);
```
现在我们大概有些了解了，但是这些方法之间的逻辑关系是怎样的呢？
## 调用流程
整个对应流程是这样
|子view|父view|
|----|----|
|startNestedScroll|onStartNestedScroll、onNestedScrollAccepted|
|dispatchNestedPreScroll|onNestedPreScroll|
|dispatchNestedScroll|onNestedScroll|
|stopNestedScroll|onStopNestedScroll|
具体说明如下：
- 
在子 view 需要滑动的时候例如 ACTION_DOWN 的时候就要调用 startNestedScroll(ViewCompat.SCROLL_AXIS_HORIZONTAL | ViewCompat.SCROLL_AXIS_VERTICAL) 方法来告诉父 view 自己要开始滑动了（实质上是寻找能够配合 child 进行嵌套滚动的 parent）。
- 
父 view 会收到 onStartNestedScroll 回调从而决定是不是要配合子view做出响应。如果需要配合，此方法会返回 true。继而 onStartNestedScroll（）回调会被调用。
- 
在滑动事件产生但是子 view 还没处理前可以调用 dispatchNestedPreScroll(0,dy,consumed,offsetInWindow) 这个方法把事件传给父 view 这样父 view 就能在onNestedPreScroll 方法里面收到子 view 的滑动信息，然后做出相应的处理把处理完后的结果通过 consumed 传给子 view。
- 
dispatchNestedPreScroll（）之后，child可以进行自己的滚动操作。
- 
如果父 view 需要在子 view 滑动后处理相关事件的话可以在子 view 的事件处理完成之后调用 dispatchNestedScroll 然后父 view 会在 onNestedScroll 收到回调。
- 
最后，滑动结束，调用 onStopNestedScroll() 表示本次处理结束。
过程清楚之后，我们再来看看一些方法的具体的含义，这样更好理解。
```
public boolean dispatchNestedPreScroll(int dx, int dy, int[] consumed, int[] offsetInWindow) ;
```
此方法的前两个参数用于告诉父 View 此次要滚动的距离；而第三第四个参数用于子 view 获取父 view 消费掉的距离和父 view 位置的偏移量。`consumed `参数是一个 `int `型的数组，长度为 2，第一个元素是父 view 消费的`x`方向的滚动距离；第二个元素是父 view 消费的 `y `方向的滚动距离，如果这两个值不为0，则子view需要对滚动的量进行一些修正。正因为有了这个参数，使得我们处理滚动事件的时候，思路更加清晰，不会像以前一样被一堆的滚动参数搞混。
如果parent消费了一部分或全部距离，则此方法返回true。
```
public boolean dispatchNestedScroll(int dxConsumed, int dyConsumed, int dxUnconsumed, int dyUnconsumed, int[] offsetInWindow);
```
在子 view 自己进行滚动之后调用此方法，询问父view是否还要进行余下 (unconsumed) 的滚动。
前四个参数为输入参数，用于告诉父view已经消费和尚未消费的距离，最后一个参数为输出参数，用于子view获取父view位置的偏移量。
返回值：  true if the event was dispatched, false if it could not be dispatched.
```
public void stopNestedScroll();
```
 最后，stopNestedScroll()方法与startNestedScroll(int axes)对应，用于结束嵌套滚动流程；而惯性滚动相关的两个方法与触摸滚动相关的两个方法类似，这里不再赘述。
 这里看三个例子，
1、[https://github.com/543441727/MyNestedScrolling](https://github.com/543441727/MyNestedScrolling)
2、这个是我改编的上面一个人写的代码，让它能够像下拉刷新那样工作：
[https://github.com/huanshen/NestedScrollingParent-Child](https://github.com/huanshen/NestedScrollingParent-Child)
3、[https://github.com/qstumn/RefreshLayout](https://github.com/qstumn/RefreshLayout)
## 简单来说：
在 NestedScrollingChild 处理点击事件，然后发起滚动请求；
在 NestedScrollingParent 的 onNestedPreScroll 处理事件，是不是要自己消耗；
对于同时这两个接口的ViewGroup，需要在这两个里面都进行处理。如果子类是listview，则可以通过点击事件的拦截来进行控制，如果是recyclerView ,那么就不是点击事件的控制了。
[参考文章：](https://github.com/huanshen/Learn-Android)
[NestedScrollingParent, NestedScrollingChild  详解](http://blog.csdn.net/chen930724/article/details/50307193)
[NestedScrolling的使用及滚动型的惯性滑动](http://renyuan-1991.iteye.com/blog/2262643)
[Android -- NestedScrolling滑动机制](http://www.cnblogs.com/wjtaigwh/p/6398562.html)
