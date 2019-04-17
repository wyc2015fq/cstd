# 初识View-FOUR - DEVELOPER - CSDN博客





2017年12月11日 14:08:41[学术袁](https://me.csdn.net/u012827205)阅读数：75
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)









![这里写图片描述](https://img-blog.csdn.net/20171211140736124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 开篇短叙

做开发很久了，却一直没有真正的去面对自定义View这一块蛋糕。平时，在github或者博客上自学了很多这方面的知识，但是一直不是很系统，总是在真正使用的时候发现，自己在这方面的知识储备显得-很“笨拙”，只能应对一些简单的！！借着最近项目不紧张，再次进行View的系统学习，并在这里记录一下自己的学习过程。

### 摘要
- layout()作用？layout()和onLayout()关系
- onDraw()作用？onDraw()、dispatchDraw()和draw()关系

#### 概括性描述：

对于自定义View来实现自己的需求控件，在一些关键方法的使用上，比如：onMeasure()、onLayout()和onDraw()。 

onMeasure()功能作为测量。以ViewGroup作为父控件（容器）且具有子控件（非容器）的自定义的View来说，若是已确定摆放的方式，通常会在自定义的容器中重写onMeasure()方法。然后以measureChildren 、 measureChild()或者measureChildWithMargin()根据具体情况来实现具体的测量功能。

```java
... onMeasure(..){
  ...
  //measureChildren();
  measureChild();
  //measureChildWithMargin();
  ...
}
```

layout()本质上就是为View设置四个坐标值，从而可以精确的绘制在指定区域。而在使用的过程中，我们经常会遇到另外一个方法onLayout()。并且onLayout()都是在ViewGroup（容器）中重写，其作用是来确定容器中的每个子控件的位置。然后具体的位置摆放需要方法layout()实现，通过其四个参数左、上、右、下。而不同的容器的布局策略不尽相同如，RelaytiveLayout和LinearLayout。从而他们在方法onLayout()中的实现方式也不尽相同。

```java
... onLayout(..){
  ...
  view.layout();
  ...
}
```

draw()就是对View(非容器)的具体绘制。并在onDraw()方法中实现。平时自定义View的时候，也许会遇到一个方法dispatchDraw()，他的出现表示对当前容器中的子View进行绘制，且该子View是容器。也即是说为了进行层层绘制。

### layout() 分析
- 简单说
View 的 layout() 方法用于 View 确定自己本身在其父 View 的位置 

ViewGroup 的 onLayout() 方法用于确定子 View 的位置
先简单使用代码理解一下：

```java
public class MyViewGroup extends ViewGroup {

    public MyViewGroup(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

//首先使用这个方法获取child的尺寸
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int childCount = getChildCount();
        if(childCount > 0){
            View child = getChildAt(0);

            measureChild(child,widthMeasureSpec,heightMeasureSpec);
        }
    }

//然后在这个方法中来确定view的位置
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        int childCount = getChildCount();
        if(childCount > 0){
            View child = getChildAt(0);
            int measuredWidth = child.getMeasuredWidth();
            int measuredHeight = child.getMeasuredHeight();

            //确定自身在父容器中的位置
            child.layout(0,0,measuredWidth,measuredHeight);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
    }
}
```

ViewGroup 首先调用 layout() 来确定自己本身在其父 View 中的位置，然后调用 onLayout() 确定每个子 View 的位置，每个子 View 又会调用 View 的 layout() 方法来确定自己在 ViewGroup 的位置。 

看源码体会！

```java
// LinearLyout 源码
protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if (mOrientation == VERTICAL) {
            layoutVertical(l, t, r, b);
        } else {
            layoutHorizontal(l, t, r, b);
        }
    }
```

```java
//layoutVertical
void layoutVertical(int left, int top, int right, int bottom) {
    final int paddingLeft = mPaddingLeft;
    int childTop;
    int childLeft;
    final int width = right - left;
    int childRight = width - mPaddingRight;

    int childSpace = width - paddingLeft - mPaddingRight;

    final int count = getVirtualChildCount();

    final int majorGravity = mGravity & Gravity.VERTICAL_GRAVITY_MASK;
    final int minorGravity = mGravity & Gravity.RELATIVE_HORIZONTAL_GRAVITY_MASK;

    switch (majorGravity) {
          case Gravity.BOTTOM:
              childTop = mPaddingTop + bottom - top - mTotalLength;
               break;

          case Gravity.CENTER_VERTICAL:
              childTop =mPaddingTop+(bottom-top-mTotalLength) / 2;
              break;

          case Gravity.TOP:
          default:
              childTop = mPaddingTop;
              break;
    }

    for (int i = 0; i < count; i++) {
        final View child = getVirtualChildAt(i);
        if (child == null) {
            childTop += measureNullChild(i);
        } else if (child.getVisibility() != GONE) {
            final int childWidth = child.getMeasuredWidth();
            final int childHeight = child.getMeasuredHeight();

            final LinearLayout.LayoutParams lp =
                        (LinearLayout.LayoutParams) child.getLayoutParams();

            int gravity = lp.gravity;
            if (gravity < 0) {
                gravity = minorGravity;
            }
            final int layoutDirection = getLayoutDirection();
            final int absoluteGravity = Gravity.getAbsoluteGravity(gravity, layoutDirection);
            switch (absoluteGravity & Gravity.HORIZONTAL_GRAVITY_MASK) {
                case Gravity.CENTER_HORIZONTAL:
                    childLeft = paddingLeft + ((childSpace - childWidth) / 2)
                                + lp.leftMargin - lp.rightMargin;
                    break;

                case Gravity.RIGHT:
                    childLeft = childRight - childWidth - lp.rightMargin;
                    break;

                case Gravity.LEFT:
                default:
                    childLeft = paddingLeft + lp.leftMargin;
                    break;
            }

            if (hasDividerBeforeChildAt(i)) {
                childTop += mDividerHeight;
            }

            childTop += lp.topMargin;
            //在 setChildFrame() 中又调用了 View 的 layout() 方法来确定子 View自身在父容器中的位置
            setChildFrame(child,childLeft,childTop+ getLocationOffset(child),
                        childWidth, childHeight);
            childTop += childHeight + lp.bottomMargin + getNextLocationOffset(child);

            i += getChildrenSkipCount(child, i);
        }
    }
}
```

然后简单看下 onLayout源码：

```java
//确定自己在父容器中的位置
public void layout(int l, int t, int r, int b) {
        if ((mPrivateFlags3 & PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT) != 0) {
            onMeasure(mOldWidthMeasureSpec, mOldHeightMeasureSpec);
            mPrivateFlags3 &= ~PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT;
        }

        int oldL = mLeft;
        int oldT = mTop;
        int oldB = mBottom;
        int oldR = mRight;

        boolean changed = isLayoutModeOptical(mParent) ?
                setOpticalFrame(l, t, r, b) : setFrame(l, t, r, b);

        if (changed || (mPrivateFlags & PFLAG_LAYOUT_REQUIRED) == PFLAG_LAYOUT_REQUIRED) {
        //此方法是一个空方法，因为onLayout在layout方法中被调用，onLayout应该确定并为其子View分配位置。
        //而只有ViewGroup（容器）才有子View。但是ViewGroup 的 onLayout()是一个抽象方法，说明ViewGroup 的子类都必须重写这个方法，这样才能实现自己特有的逻辑功能：如，RelativeLayout和LinearLayout。
        //只有具有子View的派生类（容器）才具备调用此方法（此时不会为空的）来确定并分配子view位置功能。
            onLayout(changed, l, t, r, b);
            mPrivateFlags &= ~PFLAG_LAYOUT_REQUIRED;

            ListenerInfo li = mListenerInfo;
            if (li != null && li.mOnLayoutChangeListeners != null) {
                ArrayList<OnLayoutChangeListener> listenersCopy =
                        (ArrayList<OnLayoutChangeListener>)li.mOnLayoutChangeListeners.clone();
                int numListeners = listenersCopy.size();
                for (int i = 0; i < numListeners; ++i) {
                    listenersCopy.get(i).onLayoutChange(this,l,t,r,b,oldL,oldT,oldR,oldB);
                }
            }
        }

        mPrivateFlags &= ~PFLAG_FORCE_LAYOUT;
        mPrivateFlags3 |= PFLAG3_IS_LAID_OUT;
    }
```

### onDraw() & Canvas()分析

首先简单表示一下。因为我之前在自定义view时候，在用到onDraw方法，其中有两个经常用到的方法，canvas.save() 和 canvas.restore()。当时很恍然，不清楚。知道后来才知道，**canvas.save()**表示对当前已经绘制的图形的锁定。而所谓锁定，就是指之后的任何操作不会对锁定前的图形产生影响，并且随之便会产生一个透明的图层(layer)，使得之后的draw操作都会执行在这个新的透明图层上。**canvas.restore()**表示将图像最外层的图层和图层下已经形成的图像进行糅合，生成一个新的图像。当然，该操作之后依然可以继续在该新图像上进行绘制，表示当前操作的所有操作已经被解锁定！请留心：save() 和 restore() 最好配对使用，若 调用次数restore() > save() ，可能会造成异常。如下代码：

```java
@Override
protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    canvas.drawColor(Color.GREEN);
    Paint paint=new Paint();
    paint.setTextSize(70);
    paint.setColor(Color.BLUE);
    canvas.drawText("Before clip", 20, 80, paint);
    canvas.save();
    Rect rect=new Rect(20,200,900,1000);
    canvas.clipRect(rect);
    canvas.drawColor(Color.YELLOW);
    paint.setColor(Color.BLACK);
    canvas.drawText("After clip", 10, 310, paint);
    canvas.restore();
    paint.setColor(Color.RED);
    canvas.drawText("Test", 20, 170, paint);
}
```

我们在进行图或者控件的绘制的时候，会用到 canvas ，那么如果我们自己进行创建对象而非使用系统的canvas对象，这时会有两个构造方法需要我们进行选择。看一下，

```java
/**
*官方不推荐通过该无参的构造方法生成一个canvas。如果要这么做那就需要调用setBitmap( )为其设置一个 
* Bitmap。为什么Canvas非要一个Bitmap对象呢？原因很简单：Canvas需要一个Bitmap对象来保存像素，如果画
* 的东西没有地方可以保存，又还有什么意义呢？
*/
public Canvas() {
    if (!isHardwareAccelerated()) {
        mNativeCanvasWrapper = initRaster(null);
        mFinalizer = new CanvasFinalizer(mNativeCanvasWrapper);
    } else {
        mFinalizer = null;
    }
}
```

既然不推荐这么做，那就我们接着看有参的构造方法,

```java
public Canvas(Bitmap bitmap) {
    if (!bitmap.isMutable()) {
        throw new IllegalStateException("Immutable bitmap passed to Canvas constructor");
    }
    throwIfCannotDraw(bitmap);
    mNativeCanvasWrapper = initRaster(bitmap);
    mFinalizer = new CanvasFinalizer(mNativeCanvasWrapper);
    mBitmap = bitmap;
    mDensity = bitmap.mDensity;
}
```

方法传入了一个 Bitmap 作为参数来保持绘制图像的信息。使用 canvas 来进行绘制，

```java
protected void drawOnBitmap(){
    Bitmap bitmap = Bitmap.createBitmap(800, 400, Bitmap.Config.ARGB_8888);
    Canvas canvas = new Canvas(bitmap);
    canvas.drawColor(Color.Blue);
    Paint paint = new Paint();
    paint.setColor(Color.RED);
    paint.setTextSize(60);
    canvas.drawText("This is my view", 150, 200, paint);
    mImageView.setImageBitmap(bitmap);
}
```

我们知道 onDraw( ) 的输入参数是一个 canvas，它与我们自己创建的 canvas 不同。这个系统传递给我们的 canvas 来自于 ViewRootImpl 的 Surface，在绘图时系统将会 SkBitmap 设置到 SkCanvas 中并返回与之对应 Canvas。所以，在 onDraw() 中也是有一个 Bitmap 的，只是这个 Bitmap 是由系统创建的罢了。 

既然已经提到了 onDraw( ) 我们就利用 onDraw( )方法来画一些常见的图形。



