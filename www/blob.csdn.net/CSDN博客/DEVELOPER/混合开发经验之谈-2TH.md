# 混合开发经验之谈-2TH - DEVELOPER - CSDN博客





2017年12月28日 13:39:58[学术袁](https://me.csdn.net/u012827205)阅读数：125
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









### 第二次bug 处理

在遇到上一次问题——页面不能通过正确的html地址加载显示页面内容。解决之后，又出现了新的问题！什么问题呢？地图这一个页面，太卡了，经常的出现**render error** cpu顶不住！怎么办？对于使用的AgentWeb，她的浏览器内核使用是`Chromium` ；[Crosswalk](https://crosswalk-project.org/documentation/about/faq_zh.html) 使用的不是Android的webview，而是一个有效的Chromium的修订版本基于Blink－Chromium，其中Chromium是Google的Chrome浏览器的开源基础。当然，Crosswalk 在性能上要远高于原生的webview，但是项目会徒然增加 20M+。然，综合考虑，决定换！

### Crosswalk 使用介绍

**配置**

```java
//首先：Manifest 中开启硬件加速
//Project:build.gradle
repositories {  
    maven {  
        url 'https://download.01.org/crosswalk/releases/crosswalk/android/maven2'}  
}  

//Module:build.gradle
dependencies {  
    compile 'org.xwalk:xwalk_core_library:23.53.589.4'  
}  

productFlavors {
        armv7 {
            ndk {
                abiFilters "armeabi-v7a"
            }
        }
        x86 {
            ndk {
                abiFilters "x86"
            }
        }
    }
```

使用 `https://download.01.org/crosswalk/releases/crosswalk/android/maven2` 跳入的网页中，选择需要的版本。

![这里写图片描述](https://img-blog.csdn.net/20171228133347826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ok，到内部核心代码的更换到H5端调用的交互方式的更换之后，运行检阅效果。发现，地图的渲染效率明显增加，页面的画面效果更加清晰逼真。运行结果表明，这个 crosswalk 虽然在空间体积上不让人满意外，其他效果还是挺不错的。

### 第三次bug 处理

但是好景不长，在进行项目测试的时候，又遇到了棘手的问题。4个tab页面的切换有时候**不灵了**，Jesus（**耶稣**）！！这就有些匪夷所思了，**效果是这样的：** 进入主页面时，默认第一个tab被选中，且内容显示第一页面内容；然后点击第2或3或4，只是 tab进行了切换（选中状态变化），而tab对应的页面内容不变，特别需要强调的是tab的点击是有监听，无论页面切换与否都有监听。听上去是不是很扯？怎么解决，这问题过于刁专。 

 经过后期的项目**实践发现**，这些问题的**病因**在于(1)手机,(2)html页面。当然html页面我无法要求，毕竟不是我写的。所以只能提升移动端的性能来弥补。如果有机会你可以尝试一下，极度放慢viewpager的页面切换的时间，就会发现一切无问题。但是，只要极致缩短切换时间（当然都是追求更好的效果）就会发现，好手机没事，差点的手机要么切换不动，要么切换的两个页面内容有部分的重叠覆盖。

先介绍一下基础：基础框架是 TabHost + ViewPager（CusViewPager） + Fragment，所以在操作功能上跟微信是一模一样，既可以点击tab进行切换，也可以通过滑动进行切换。但是考虑到第一页面的地图手势很复杂，于是要禁用手势的滑动切换。然后就重写了一个 ViewPager，禁止了滑动操作功能。又考虑到想要更靠近微信的操作，所有在重写的 ViewPager中也把切换的动画给禁止了。

经过各种尝试之后，发现了一个**突破点**、**曙光**。 

发现给页面切换添加切换的动画，整个切换就没有问题了。说来也是很神奇，有切换动画和没有切换动画，对于 TabHost + ViewPager（CusViewPager） + Fragment 而言，不就是拿到监听 Fragment 发生变换吗？但事实是监听有，变换不执行。唯有添加切换的动画，才能执行。 

重写ViewPager的切换动画的基类 PageTransformer：
```java
public abstract class ABaseTransformer implements PageTransformer {

    /**
     * Called each {@link #transformPage(View, float)}.
     * 
     * @param page
     *            Apply the transformation to this page
     * @param position
     *            Position of page relative to the current front-and-center position of the pager. 0 is front and
     *            center. 1 is one full page position to the right, and -1 is one page position to the left.
     */
    protected abstract void onTransform(View page, float position);

    /**
     * Apply a property transformation to the given page. For most use cases, this method should not be overridden.
     * Instead use {@link #transformPage(View, float)} to perform typical transformations.
     * 
     * @param page
     *            Apply the transformation to this page
     * @param position
     *            Position of page relative to the current front-and-center position of the pager. 0 is front and
     *            center. 1 is one full page position to the right, and -1 is one page position to the left.
     */
    @Override
    public void transformPage(View page, float position) {
        onPreTransform(page, position);
        onTransform(page, position);
        onPostTransform(page, position);
    }

    /**
     * If the position offset of a fragment is less than negative one or greater than one, returning true will set the
     * fragment alpha to 0f. Otherwise fragment alpha is always defaulted to 1f.
     * 
     * @return
     */
    protected boolean hideOffscreenPages() {
        return true;
    }

    /**
     * Indicates if the default animations of the view pager should be used.
     * 
     * @return
     */
    protected boolean isPagingEnabled() {
        return false;
    }

    /**
     * Called each {@link #transformPage(View, float)} before {{@link #onTransform(View, float)}.
     * <p>
     * The default implementation attempts to reset all view properties. This is useful when toggling transforms that do
     * not modify the same page properties. For instance changing from a transformation that applies rotation to a
     * transformation that fades can inadvertently leave a fragment stuck with a rotation or with some degree of applied
     * alpha.
     * 
     * @param page
     *            Apply the transformation to this page
     * @param position
     *            Position of page relative to the current front-and-center position of the pager. 0 is front and
     *            center. 1 is one full page position to the right, and -1 is one page position to the left.
     */
    protected void onPreTransform(View page, float position) {
        final float width = page.getWidth();

        page.setRotationX(0);
        page.setRotationY(0);
        page.setRotation(0);
        page.setScaleX(1);
        page.setScaleY(1);
        page.setPivotX(0);
        page.setPivotY(0);
        page.setTranslationY(0);
        page.setTranslationX(isPagingEnabled() ? 0f : -width * position);

        if (hideOffscreenPages()) {
            page.setAlpha(position <= -1f || position >= 1f ? 0f : 1f);
            page.setEnabled(false);
        } else {
            page.setEnabled(true);
            page.setAlpha(1f);
        }
    }

    /**
     * Called each {@link #transformPage(View, float)} after {@link #onTransform(View, float)}.
     * 
     * @param page
     *            Apply the transformation to this page
     * @param position
     *            Position of page relative to the current front-and-center position of the pager. 0 is front and
     *            center. 1 is one full page position to the right, and -1 is one page position to the left.
     */
    protected void onPostTransform(View page, float position) {
    }

    /**
     * Same as {@link Math#min(double, double)} without double casting, zero closest to infinity handling, or NaN support.
     * 
     * @param val
     * @param min
     * @return
     */
    protected static final float min(float val, float min) {
        return val < min ? min : val;
    }

}
```

切换动画的具体实现：

```java
/**
 * Created by YJH on 2017/12/27.
 * 功能：viewpager 的动画切换动画实现类
 * 为什么使用 动画呢？因为没有动画，在小米机子上，使用TabLayout+Fragment 进行点击按钮页面切换时，不能实现效果！（华为手机上没问题）
 * 虽然使用这种方法实现，但是还有一点，就是切换的动画太长，这时候，需要重写 类Scroller 降低动画的切换时间
 */
public class ForeToBackTransformer extends ABaseTransformer {

    @Override
    protected void onTransform(View view, float position) {
        final float height = view.getHeight();
        final float width = view.getWidth();
        final float scale = min(position > 0 ? 1f : Math.abs(1f + position), 0.5f);

        view.setScaleX(scale);
        view.setScaleY(scale);
        view.setPivotX(width * 0.5f);
        view.setPivotY(height * 0.5f);
        view.setTranslationX(position > 0 ? width * position : -width * position * 0.25f);
    }

}
```

好，我就给你添加切换的动画。满足你的需求！然后运行成功，bug解决。但效果不太满意，因为切换动画那是有动画的执行时间，有的甚至切换动画是各种各样，3D的什么的我不用，我使用的就是普通的变换，但是动画时间导致切换明显的空白延迟效果，怎么办？然后突然就想到能不能减少切换的动画时间，自己去设置？好，有了设想，就去搞。

```java
/**
 * Created by YJH on 2017/12/27.
 * 功能：viewpager 的动画切换动画实现类
 * 为什么使用 动画呢？因为没有动画，在小米机子上，使用TabLayout+Fragment 进行点击按钮页面切换时，不能实现效果！（华为手机上没问题）
 * 虽然使用这种方法实现，但是还有一点，就是切换的动画太长，这时候，需要重写 类Scroller 降低动画的切换时间
 */
public class FixedSpeedScroller extends Scroller {
    private int mDuration = 1;
    public FixedSpeedScroller(Context context) {
        super(context);
        // TODO Auto-generated constructor stub
    }

    public FixedSpeedScroller(Context context, Interpolator interpolator) {
        super(context, interpolator);
    }

    @Override
    public void startScroll(int startX, int startY, int dx, int dy, int duration) {
        // Ignore received duration, use fixed one instead
        super.startScroll(startX, startY, dx, dy, mDuration);
    }

    @Override
    public void startScroll(int startX, int startY, int dx, int dy) {
        // Ignore received duration, use fixed one instead
        super.startScroll(startX, startY, dx, dy, mDuration);
    }

    /**
     * 设置切换时间
     */
    public void setmDuration(int time) {
        mDuration = time;
    }

    /**
     * 获取切换时间
     */
    public int getmDuration() {
        return mDuration;
    }
}
```

找到并重写之后，就要到项目代码中去设置：

![这里写图片描述](https://img-blog.csdn.net/20171228142631830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

代码中可以看到 设置动画的代码：`viewPager.setPageTransformer(true, new ForeToBackTransformer());`

设置动画时间的代码:

```java
try {
            // 通过class文件获取mScroller属性
            //重写 类Scroller 降低动画的切换时间
            Field mField = ViewPager.class.getDeclaredField("mScroller");
            mField.setAccessible(true);
            mScroller = new FixedSpeedScroller(viewPager.getContext(), new AccelerateInterpolator());
            mField.set(viewPager, mScroller);
        } catch (Exception e) {
            e.printStackTrace();
        }
        mScroller.setmDuration(1);
```

到此，已完成了需求。谢谢我追求完美的不屑努力。。




