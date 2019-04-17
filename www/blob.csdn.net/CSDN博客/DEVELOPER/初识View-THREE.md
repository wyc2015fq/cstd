# 初识View-THREE - DEVELOPER - CSDN博客





2017年12月08日 15:23:09[学术袁](https://me.csdn.net/u012827205)阅读数：58
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)

所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![这里写图片描述](https://img-blog.csdn.net/20171208134150936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 开篇短叙

做开发很久了，却一直没有真正的去面对自定义View这一块蛋糕。平时，在github或者博客上自学了很多这方面的知识，但是一直不是很系统，总是在真正使用的时候发现，自己在这方面的知识储备显得-很“笨拙”，只能应对一些简单的！！借着最近项目不紧张，再次进行View的系统学习，并在这里记录一下自己的学习过程。

### 摘要
- onMeasure() 如何复杂？
- measuireChildren()、measuireChild()和measuireChildrenWithMargin()如何发挥作用？
- setMeasuredDimension() 还熟悉不？
- 为什么LayoutParams 类要定义在ViewGroup中？
- 为什么View中会有一个mLayoutParams 变量？
- MeasureSpec 类是嘛的？
- onMeasure 测量流程

#### onMeasure() 如何复杂
- onMeasure()
onMeasure，这个方法是用来测量控件大小的。并且这个方法会应用到我们在自定义View或ViewGroup的时候。而针对onMeasure方法，它是由父控件调用的，目的是用来测量子控件大小。ViewGroup需要重写onMeasure方法测量子控件的宽高和自己的宽高，然后实现onLayout方法摆放子控件。而 View则是需要重写onMeasure根据测量模式和父控件给出的建议的宽高值计算自己的宽高，然后再父控件为其指定的区域绘制自己的图形。
比如定义一个scrollLayout  继承ViewGroup时：

```java
@Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            View childView = getChildAt(i);
            // 为ScrollerLayout中的每一个子控件测量大小
            measureChild(childView, widthMeasureSpec, heightMeasureSpec);
        }
    }
```

比如定义一个loadingButton  继承Button时：

```java
@Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);

        int width = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);

        mWidth = width;
        mHeight = height;

        centerX = mWidth / 2;
        centerY = mHeight / 2;

        radius = mHeight / 8;
        Log.d("allen", "onMeasure: " + centerX);
    }
```

从上面两种代码可以看出，onMeasure 这个方法常在View和ViewGroup被自定义时候被调用。但是他们在使用方式大不相同！因为在ViewGroup中在调用onMeasure来测量子控件大小，而动真格的时刻是在`measureChild(childView, widthMeasureSpec, heightMeasureSpec);` 这句代码上。并且，还不止这一句代码有这种功能，还有方法 `measuireChildrenWithMargin()` 。

#### measuireChildren()、measuireChild()和measuireChildrenWithMargin()如何发挥作用

对于他们的关系，我来使用代码来理解：

```java
/**
  *遍历ViewGroup中所有的子控件，调用measuireChild测量宽高
  */
 protected void measureChildren (int widthMeasureSpec, int heightMeasureSpec) {
    final int size = mChildrenCount;
    final View[] children = mChildren;
    for (int i = 0; i < size; ++i) {
        final View child = children[i];
        if ((child.mViewFlags & VISIBILITY_MASK) != GONE) {
            //测量某一个子控件宽高
            measureChild(child, widthMeasureSpec, heightMeasureSpec);
        }
    }
}
```

```java
protected void measureChild(View child, int parentWidthMeasureSpec,
            int parentHeightMeasureSpec) {
        final LayoutParams lp = child.getLayoutParams();

        final int childWidthMeasureSpec = getChildMeasureSpec(parentWidthMeasureSpec,
                mPaddingLeft + mPaddingRight, lp.width);
        final int childHeightMeasureSpec = getChildMeasureSpec(parentHeightMeasureSpec,
                mPaddingTop + mPaddingBottom, lp.height);

        child.measure(childWidthMeasureSpec, childHeightMeasureSpec);
    }
```

```java
protected void measureChildWithMargins(View child,
            int parentWidthMeasureSpec, int widthUsed,
            int parentHeightMeasureSpec, int heightUsed) {
        final MarginLayoutParams lp = (MarginLayoutParams) child.getLayoutParams();

        final int childWidthMeasureSpec = getChildMeasureSpec(parentWidthMeasureSpec,
                mPaddingLeft + mPaddingRight + lp.leftMargin + lp.rightMargin
                        + widthUsed, lp.width);
        final int childHeightMeasureSpec = getChildMeasureSpec(parentHeightMeasureSpec,
                mPaddingTop + mPaddingBottom + lp.topMargin + lp.bottomMargin
                        + heightUsed, lp.height);

        child.measure(childWidthMeasureSpec, childHeightMeasureSpec);
    }
```

从代码上，我们可以得出结论：
- measureChildren()
遍历所有子控件挨个测量;- measuireChild()
与measureChildren是互相依赖的关系，并且是最终测量子控件的方法；- measuireChildrenWithMargin()
也是作为最终测量子控件的方法；measureChildWithMargins跟measureChild的区别就是父控件支不支持margin属性； 

支不支持margin属性对子控件的测量是有影响的，比如我们的屏幕是1080x1920的，子控件的宽度为填充父窗体，如果使用了marginLeft并设置值为100；  

在测量子控件的时候，如果用measureChild，计算的宽度是1080，而如果是使用measureChildWithMargins，计算的宽度是1080-100 = 980。- getChildMeasureSpec 作用
measureChildWithMargins跟measureChild 都调用了getChildMeasureSpec 这个方法，其作用就是通过父控件的宽高约束规则和父控件加在子控件上的宽高布局参数生成一个子控件的约束。
```java
protected void onMeasure( int widthMeasureSpec, int heightMeasureSpec) {
    setMeasuredDimension( getDefaultSize(getSuggestedMinimumWidth(), widthMeasureSpec),
            getDefaultSize(getSuggestedMinimumHeight(), heightMeasureSpec));
}
/**
 * 为宽度获取一个建议最小值
 */
protected int getSuggestedMinimumWidth () {
    return (mBackground == null) ? mMinWidth : max(mMinWidth , mBackground.getMinimumWidth());
}
/**
 * 获取默认的宽高值
 */
public static int getDefaultSize (int size, int measureSpec) {
    int result = size;
    int specMode = MeasureSpec. getMode(measureSpec);
    int specSize = MeasureSpec. getSize(measureSpec);
    switch (specMode) {
    case MeasureSpec. UNSPECIFIED:
        result = size;
        break;
    case MeasureSpec. AT_MOST:
    case MeasureSpec. EXACTLY:
        result = specSize;
        break;
    }
    return result;
}
```
- 从代码上，我们看到一个方法setMeasuredDimension()，这家伙是干嘛的？
- setMeasuredDimension()
在onMeasure方法被调用过后，需要调用setMeasuredDimension方法来保存已经测量的宽、高值，如果不调用这个方法，可能会产生不可预测的问题。
#### 为什么LayoutParams 类要定义在ViewGroup中

#### 为什么View中会有一个mLayoutParams 变量

看代码：

```java
<declare-styleable name= "ViewGroup_Layout">
    <attr name ="layout_width" format="dimension">
        <enum name ="fill_parent" value="-1" />
        <enum name ="match_parent" value="-1" />
        <enum name ="wrap_content" value="-2" />
    </attr >
    <attr name ="layout_height" format="dimension">
        <enum name ="fill_parent" value="-1" />
        <enum name ="match_parent" value="-1" />
        <enum name ="wrap_content" value="-2" />
    </attr >
</declare-styleable >
```

```java
< declare-styleable name ="ViewGroup_MarginLayout">
    <attr name ="layout_width" />   <!--使用已经定义过的属性-->
    <attr name ="layout_height" />
    <attr name ="layout_margin" format="dimension"  />
    <attr name ="layout_marginLeft" format= "dimension"  />
    <attr name ="layout_marginTop" format= "dimension" />
    <attr name ="layout_marginRight" format= "dimension"  />
    <attr name ="layout_marginBottom" format= "dimension"  />
    <attr name ="layout_marginStart" format= "dimension"  />
    <attr name ="layout_marginEnd" format= "dimension"  />
</declare-styleable >
```

上述两块儿的代码分别是LayoutParams和MarginLayoutParams的布局属性定义代码。从关系上来说，MarginLayoutParams是LayoutParams的子类，因为在ViewGroup中有两个内部类ViewGroup.LayoutParams和ViewGroup. MarginLayoutParams。当然从上述代码也能看出眉目，子类当然就是父类的功能属性上的扩展。

**为什么LayoutParams 类要定义在ViewGroup中？** ViewGroup是所有容器的基类，一个控件需要被包裹在一个容器中，这个容器必须提供一种规则控制子控件的摆放，比如你的宽高是多少，距离那个位置多远等。所以ViewGroup有义务提供一个布局属性类，用于控制子控件的布局属性。

**为什么View中会有一个mLayoutParams 变量？ **我们在之前学习自定义控件的时候学过自定义属性，我们在构造方法中，初始化布局文件中的属性值，我们姑且把属性分为两种。一种是本View的绘制属性，比如TextView的文本、文字颜色、背景等，这些属性是跟View的绘制相关的。另一种就是以“layout_”打头的叫做布局属性，这些属性是父控件对子控件的大小及位置的一些描述属性，这些属性在父控件摆放它的时候会使用到，所以先保存起来，而这些属性都是ViewGroup.LayoutParams定义的，所以用mLayoutParams 这个变量保存着。

#### MeasureSpec 类是嘛的

它是View中的一个静态内部类，是由尺寸和模式组合而成的一个值，用来描述父控件对子控件尺寸的约束，看看他的部分源码，一共有三种模式，然后提供了合成和分解的方法。

```java
//父控件不强加任何约束给子控件，它可以是它想要任何大小。
    public static final int UNSPECIFIED = 0 << MODE_SHIFT;  //0

    //父控件决定给孩子一个精确的尺寸
    public static final int EXACTLY     = 1 << MODE_SHIFT;  //1073741824

    //父控件会给子控件尽可能大的尺寸
    public static final int AT_MOST     = 2 << MODE_SHIFT;   //-2147483648
```

View源码：

```java
public static int makeMeasureSpec(@IntRange(from = 0, to = (1 << MeasureSpec.MODE_SHIFT) - 1) int size,  
                                  @MeasureSpecMode int mode) {  
    if (sUseBrokenMakeMeasureSpec) {  
        return size + mode;  
    } else {  
        return (size & ~MODE_MASK) | (mode & MODE_MASK);  
    }  
}  

public static int getMode(int measureSpec) {  
    //noinspection ResourceType  
    return (measureSpec & MODE_MASK);  
}  

public static int getSize(int measureSpec) {  
    return (measureSpec & ~MODE_MASK);  
}
```
- MeasureSpec信息量好大，包含了SpecMode和SpecSize，她们又分别用来干什么的？
SpecMode:测量模式。 

SpecSize:在某种测量模式下的规格大小
**从View源码中，分别看看三个方法的功能：**
- makeMeasureSpec方法：这个方法通过将SpecMode和SpecSize打包成一个int值MeasureSpec。从中我们也可以发现，把SpecMode和SpecSize打包成一个int值是为了避免过多的对象内存分配。
- getMode方法：这个方法把SpecMode解包了。
- getSize方法：这个方法把SpecSize解包了。

#### onMeasure 测量流程

![这里写图片描述](https://img-blog.csdn.net/20171208152942187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

比如以下布局代码：
```java
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/container"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingLeft="10px"
    tools:context=".MainActivity">

    <LinearLayout
        android:id="@+id/innerL"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent"
        android:orientation="horizontal"
        android:paddingLeft="20px">

        <TextView
            android:id="@+id/tView"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="TextView"/>
        <Button
            android:id="@+id/btn"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="Button"/>
    </LinearLayout>

</RelativeLayout>
```

测量逻辑：测量的时候，父控件（RelativeLayout）的onMeasure方法会遍历他所有的子控件（LinearLayout、TextView、Button），挨个调用子控件的measure方法，measure方法会调用控件自己拥有的方法onMeasure，之后会调用setMeasureDimension方法来保存测量的大小。一次遍历下来，第一个子控件以及这个子控件中的所有子控件都会完成测量工作，每次测量子控件方法是measureChild() 和measureChildWithMargins() 。最后父控件所有的子控件都完成测量以后会调用setMeasureDimension方法保存自己的测量大小。




