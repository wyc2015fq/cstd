# 安卓开发33：HorizontalScrollView 水平滚动 API - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月02日 09:44:58[initphp](https://me.csdn.net/initphp)阅读数：1768
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









用于布局的容器，可以放置让用户使用滚动条查看的视图层次结构，允许视图结构比手机的屏幕大。HorizontalScrollView是一种`FrameLayout`（框架布局），其子项被滚动查看时是整体移动的，并且子项本身可以是一个有复杂层次结构的布局管理器。一个常见的应用是子项在水平方向中，用户可以滚动显示顶层水平排列的子项(items)。

HorizontalScrollView不可以和ListView同时用，因为ListView有自己的滚动条设置。最重要的是，如果在需要显示很大的list的情况下，两者同时用则会使ListView在一些重要的优化上失效。出现这种失效的原因在于，HorizontalScrollView会强迫ListView用HorizontalScrollView本身提供的空间容器（infinite container）来显示完整的列表。

 类似的情况，`TextView`也有自己的滚动条，所以不需要ScrollView。但这两者是可以同时使用的，使用的结果会是在一个更大的容器里显示文本视图。

HorizontalScrollView只支持水平方向的滚动显示。



**公共方法**

public void **addView** (View child)

添加一个子视图。若这个子视图没有被设置布局参数，则使用ViewGroup的缺省参数。

参数

child         要添加的子视图



public void **addView** (View child,int index)

添加一个子视图。若这个子视图没有被设置布局参数，则使用ViewGroup的缺省参数。

参数

child         要添加的子视图

index       子视图要加入的位置



public void **addView** (View child,int index, ViewGroup.LayoutParams params)

添加一个带有指定布局参数的子视图。

参数

child         要添加的子视图

index       子视图要加入的位置

params   子视图的布局参数



public void **addView** (View child,ViewGroup.LayoutParams params)

添加一个带有指定布局参数的子视图。

参数

child         要添加的子视图

params   子视图的布局参数



public boolean **arrowScroll** (intdirection)

响应点击左右箭头时对滚动条的处理。

参数

direction          Thedirection corresponding to the arrow key that was pressed箭头按键所表示的方向

返回值

             若此事件成功完成，则返回true；否则返回false。



public void **computeScroll** ()

被父视图调用，用于必要时候对其子视图的值（mScrollX和mScrollY）进行更新。典型的情况如：父视图中某个子视图使用一个`Scroller`对象来实现滚动操作，会使得此方法被调用。



public boolean **dispatchKeyEvent**(KeyEvent event)

按照可以获得焦点的顺序（从视图树的顶端到当前获得焦点的视图），分派一个按键事件给下一个视图。若此视图为焦点视图，事件将会分派给它自己。否则它将按照顺序，分派给下一个节点。此方法同时触动所有按键监听器。

参数

event       被分派的事件.

返回值

             若事件被处理，则返回true；否则为false



public void **draw** (Canvas canvas)

手动绘制视图（及其子视图）到指定的画布(Canvas)。这个视图必须在调用这个函数之前做好了整体布局。。当实现一个视图时，不需要继承这个方法；相反，你应该实现`onDraw(Canvas)``方法。`

参数

canvas    绘制视图的画布



public boolean **executeKeyEvent**(KeyEvent event)

需要通过按键事件来实现滚动操作时，可以调用此方法。效果类似于由视图树型结构分派事件。

参数

event       需要执行的事件

返回值

             若事件被处理，则返回true；否则为false 



public void **fling** (int velocityX)

滚动视图的fling手势。

参数

velocityX          方向的初始速率。正值表示手指/光标向屏幕右边滑动，而内容相对向左滚动。



public boolean **fullScroll** (intdirection)

处理按下"home/end"快捷键之后的滚动响应。此方法会将视图移左或移右，同时将焦点赋予移动后可视的最左或最右的组件。如果没有任何组件适合得到焦点，此scrollview将收回焦点。

参数

direction          滚动方向：FOCUS_LEFT表示向视图的左边移动，FOCUS_RIGHT表示向视图的右边移动

返回值

             若此方法消耗(consumed)了按键事件则返回true，否则返回false。 



public int **getMaxScrollAmount** ()

返回值

             按左右箭头时视图可以滚动的最大值。



public boolean **isFillViewport** ()

表示此ScrollView的内容是否被拉伸以适应视口（viewport）的大小。

返回值

             若内容填充了视口则返回true，否则返回false。



public boolean **isSmoothScrollingEnabled**()

返回值

             按箭头方向滚动时，是否显示滚动的平滑效果。



public boolean **onInterceptTouchEvent**(MotionEvent ev)

使用此方法可以拦截所有触摸屏动作引发的事件。这意味着你可以监视分派给子项的事件，并且可以拿到任何当前手势的所有权。

使用此方法需谨慎。因为它与View.onTouchEvent(MotionEvent)有相当复杂的交互影响。这两者都必须同时正确地实现。事件将按以下顺序来被方法接收：

   1. 接收到down事件

   2. 事件将被视图组的一个子视图处理，或者被传递给自己的onTouchEvent()方法处理；这意味着你必须实现onTouchEvent()，并且返回true，这样才可以接着接受到其他的手势（而不是寻求一个父视图来处理它）。onTouchEvent()返回true后，你将不再接受到onInterceptTouchEvent()的任何事件，同时所有对触摸动作的处理必须像往常一样在onTouchEvent()中进行。

   3. 如果返回false，则接下来的每个事件（所有的up事件，包含最后一个up）将会首先被传递到这里，然后到目标对象view的onTouchEvent()。

   4. 如果返回ture，你将不会接收到以下任何事件：目标view将会接收到相同的事件，但是带着ACTION_CANCEL的动作。所有在此之后的事件将会被传递到你的onTouchEvent()方法中，并且不再在这里出现。

参数

ev    沿着树型结构往下分派的动作事件

         返回值

             若将动作事件从子视图中截获并通过onTouchEvent()将他们分派给当前ViewGroup，则返回true。当前目标将收到一个ACTION_CANCEL事件，并且不再会有其他消息被传递到这里。



public boolean **onTouchEvent**(MotionEvent ev)

此方法用于处理触摸屏的动作事件。

参数

ev    动作事件

         返回值

             若事件被成功处理，则返回true；否则返回false



public boolean **pageScroll** (intdirection)

处理按下"page up/down"快捷键之后的滚动响应。此方法会将视图往左或往右滚动一个页面的距离，同时将焦点赋予移动后可视的最左或最右的组件。如果没有任何组件适合得到焦点，此scrollview将收回焦点。

参数

direction          滚动方向：FOCUS_LEFT表示向视图的左边移动一个页面FOCUS_RIGHT表示向视图的右边移动一个页面

         返回值

             若此方法处理(consumed)了按键事件则返回true，否则返回false。



public void **requestChildFocus**(View child, View focused)

当父视图的一个子视图要获得焦点时，调用此方法。

参数

child         要获得焦点的子视图。此视图将包含焦点视图，但其本身不必为焦点。

focused   事实上拥有焦点的子视图的下层视图。



public boolean **requestChildRectangleOnScreen**(View child, Rect rectangle, boolean immediate)

当组里的某个子视图需要被定位在屏幕的某个矩形范围时，调用此方法。重载此方法的ViewGroup可确认以下几点：

    * 子项目将是组里的直系子项

    * 矩形将在子项目的坐标体系中

重载此方法的ViewGroup应该支持以下几点：

    * 若矩形已经是可见的，则没有东西会改变

    * 为使矩形区域全部可见，视图将可以被滚动显示

参数

child         发出请求的子视图

rectangle         子项目坐标系内的矩形，即此子项目希望在屏幕上的定位

immediate      设为true，则禁止动画和平滑移动滚动条

         返回值

             进行了滚动操作的这个组（group），是否处理此操作



public void **requestLayout** ()

当出现使视图布局失效的改变时，调用此方法。它将规划一个视图树的布局路径。



public void **scrollTo** (int x, inty)

设置视图滚动后的位置。这将引起onScrollChanged(int,int,int,int)的调用，同时使此视图失效。

此版本同时将滚动锁定于子视图的范围。

参数

x      要滚动到的x位置

y      要滚动到的y位置



public void **setFillViewport**(boolean fillViewport)

设置此滚动视图是否将内容宽度拉伸来适应视口（viewport）。

参数

fillViewport     设置为true表示将拉伸内容宽度；否则会设置为false。



public void **setOverScrollMode** (intmode)

为视图设置over-scroll模式。有效的over-scroll模式有`OVER_SCROLL_ALWAYS`（缺省值），`OVER_SCROLL_IF_CONTENT_SCROLLS`（只允许当视图内容大过容器时，进行over-scrolling）和`OVER_SCROLL_NEVER`。只有当视图可以滚动时，此项设置才起作用。

参数

mode       视图的新over-scroll模式值



public void **setSmoothScrollingEnabled**(boolean smoothScrollingEnabled)

设置是否呈现按下箭头后的平滑滚动效果（动画效果）。

参数

smoothScrollingEnabled          设置是否呈现平滑滚动效果



public final void **smoothScrollBy**(int dx, int dy)

类似`scrollBy(int, int)`，但是呈现平滑滚动，而非瞬间滚动（译者注：瞬间滚动——指不显示滚动过程，直接显示滚动后达到的位置）。

参数

dx    要滚动的X轴像素差值（译者注：横向像素差值）

dy    要滚动的Y轴像素差值（译者注：纵向像素差值）



public final void **smoothScrollTo**(int x, int y)

类似`scrollTo(int, int)`，但是呈现平滑滚动，而不是瞬间滚动。

参数

x      滚动要到达位置的X轴值

y      滚动要到达位置的Y轴值



**受保护方法**

         protected int **computeHorizontalScrollOffset**()

计算水平方向滚动条的滑块的偏移值。此值用来计算滚动时滑块的位置。

偏移值的范围可以以任何单位表示，但必须与computeHorizontalScrollRange()和computeHorizontalScrollExtent()的单位一致。

缺省的偏移值为视图滚动的偏移差值。

                   返回值

                      滚动条滑块在水平方向上的偏移值



protected int **computeHorizontalScrollRange**()

scroll view 的可滚动水平范围是所有子视图的宽度总合。

                   返回值

                      水平滚动条表示的全部水平滚动范围



protected int **computeScrollDeltaToGetChildRectOnScreen**(Rect rect)

计算X方向滚动的总合，以便在屏幕上显示子视图的完整矩形（或者，若矩形宽度超过屏幕宽度，至少要填满第一个屏幕大小）。

参数

rect          矩形

                   返回值

                      滚动差值



protected float **getLeftFadingEdgeStrength**()

返回左渐变边缘的强度或密集度。强度的值介于0.0（无渐变）到1.0（全渐变）之间。缺省实现只返回0.0或1.0，而不返回中间值。子类必须重载次方法来给滚动动作提供更平滑的渐变过程。

                   返回值

                      左渐变的强度，即介于0.0f和1.0f之间的浮点值



protected float **getRightFadingEdgeStrength**()

返回右渐变边缘的强度或密集度。强度的值介于0.0（无渐变）到1.0（全渐变）之间。缺省实现只返回0.0或1.0，而不返回中间值。子类必须重载此方法来给滚动动作提供更平滑的渐变过程。

                   返回值

                      右渐变的强度，即介于0.0f和1.0f之间的浮点值



protected void **measureChild** (Viewchild, int parentWidthMeasureSpec, int parentHeightMeasureSpec)

要求子视图测量自身，需要将视图的MeasureSpec和其附加内容同时考虑在内。getChildMeasureSpec在其中承担了重要角色，它计算出MeasureSpec，并传递给子视图。

参数

child         要测量的子视图

parentWidthMeasureSpec     此视图的宽度要求

parentHeightMeasureSpec    此视图的高度要求



protected void **measureChildWithMargins**(View child, int parentWidthMeasureSpec, int widthUsed, int parentHeightMeasureSpec,int heightUsed)

要求子视图测量自身，需要将视图的MeasureSpec、附加内容和边缘部分同时考虑在内。子项必须有MarginLayoutParams（边缘布局参数）。getChildMeasureSpec在其中承担了重要角色，它计算出MeasureSpec，并传递给子视图。

参数

child         要测量的子视图

parentWidthMeasureSpec     此视图的宽度要求

widthUsed       被父视图（也可能是其他子视图）占用的横向额外空间

parentHeightMeasureSpec    此视图的高度要求

heightUsed     被父视图（也可能是其他子视图）占用的纵向额外空间



protected void **onLayout** (booleanchanged, int l, int t, int r, int b)

当此视图要给每个子视图赋值大小和位置时，layout会调用此方法。子项的派生类应当重载此方法，并且调用各个子项的layout。

参数

changed           此视图有新的大小或位置

l       左边界位置，相对于父视图

t      上边界位置，相对于父视图

r      右边界位置，相对于父视图

b      下边界位置，相对于父视图



protected void **onMeasure** (intwidthMeasureSpec, int heightMeasureSpec)

调用此方法来确定本身和所包含内容的大小（宽度和高度）。此方法被measure(int,int)唤起，而且必须被子类重载以得到所包含内容的确切大小。

         注意：当重载此方法时，必须调用setMeasureDimension(int,int)来保存View的大小。如果没有做到，将会引发一个measure(int,int)抛出的IllegalStateException（非法状态错误）。超类onMeasure(int,int)可以被调用。

         编写基类的确认大小的方法，缺省情况下是根据其背景大小来确认，除非MeasureSepc允许有更大的高度或宽度。子类必须重载onMeasure(int,int)以得到对其内容大小的更准确的测量。

         若此方法被重载，它的子类需要确保其高度和宽度至少达到View所规定的最小值（可通过getSuggestedMinimumHeight()和getSuggestedMinimumWidth()得到）。

参数

widthMeasureSpec         受上一层大小影响下的对水平空间的要求。可参看View.MeasureSpec。

heightMeasureSpec        受上一层大小影响下的对垂直空间的要求。可参看View.MeasureSpec。



protected void **onOverScrolled** (intscrollX, int scrollY, boolean clampedX, boolean clampedY)

被`overScrollBy(int,int, int, int, int, int, int, int, boolean)`调用，来对一个over-scroll操作的结果进行响应。

参数

scrollX     新的X滚动像素值

scrollY     新的Y滚动像素值

clampedX        当scrollX被over-scroll的边界限制时，值为true

clampedY        当scrollY被over-scroll的边界限制时，值为true



protected boolean **onRequestFocusInDescendants**(int direction, Rect previouslyFocusedRect)

当在某个scroll view的子视图中寻找焦点时，需要小心不能让屏幕之外的组件得到焦点。这比缺省ViewGroup的实现代价更高，否则此行为被设为缺省。

参数

direction          值可以为FOCUS_UP，FOCUS_DOWN，FOCUS_LEFT或 FOCUS_RIGHT

previouslyFocusedRect 能够给出一个较好的提示的矩形（当前视图的坐标系统）表示焦点从哪里得来。如果没有提示则为null。

返回值

                      是否取到了焦点。



protected void **onSizeChanged** (intw, int h, int oldw, int oldh)

当View的大小改变时此方法被调用。如果View是刚刚被加入，则视之前的值为0。

参数

w     View的当前宽度

h      View的当前高度

oldw        View大小改变之前的宽度

oldh         View大小改变之前的高度



