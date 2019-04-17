# 初识View-TWO - DEVELOPER - CSDN博客





2017年12月07日 12:41:47[学术袁](https://me.csdn.net/u012827205)阅读数：81
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)

所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![励志语录](https://img-blog.csdn.net/20171207094742377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 开篇短叙

做开发很久了，却一直没有真正的去面对自定义View这一块蛋糕。平时，在github或者博客上自学了很多这方面的知识，但是一直不是很系统，总是在真正使用的时候发现，自己在这方面的知识储备显得-很“笨拙”，只能应对一些简单的！！借着最近项目不紧张，再次进行View的系统学习，并在这里记录一下自己的学习过程。

## 摘要
- 屏幕探究
- 坐标系探究

## 详述

### 屏幕探究

![这里写图片描述](https://img-blog.csdn.net/20171207110503750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从图中，我们可以看到手机屏幕的区域划分。然后可以获取屏幕中对应区域中的尺寸。但是需要注意，在获取屏幕某个区域尺寸的时候，要在 Activity/View#onWindowFoucusChanged 方法之后。为什么呢？onWindowFoucusChanged 这个方法的含义是，View 已经被初始化完毕了，宽、高已经准备好了，这个时候去获取宽、高是没有问题的。还有，onWindowFoucusChanged 也许会被调用多次，因为当Activity 的窗口得到和失去焦点的时候都会被调用一次。 
**获取**屏幕某个区域（控件）尺寸，有以下几种方式，表示**View已经被初始化完毕了**
- 方式1

```java
@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imgView = (ImageView) findViewById(R.id.imgView);
        imgView.post(new Runnable() {
            @Override
            public void run() {
                getApplicationMeasure();
                //getViewMeasure();
            }
        });
    }

    //应用程序App区域的宽、高 尺寸获取
    private void getApplicationMeasure() {
        Rect rectApp = new Rect();
        getWindow().getDecorView().getWindowVisibleDisplayFrame(rectApp);
        int statusBarHight = rectApp.top;//状态栏高度
        int appWidth = rectApp.width();
        int appHight = rectApp.height();
    }

    //View布局区域的宽、高尺寸获取
    private void getViewMeasure() {
        Rect rectView = new Rect();
        getWindow().findViewById(Window.ID_ANDROID_CONTENT).getDrawingRect(rectView);
        int viewWidth = rectView.width();
        int viewHight = rectView.height();
    }
```
- 方式2

```java
@Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if(hasFocus){
            // TODO: 2017/12/7  获取尺寸
            int hight = imgView.getMeasuredHeight();
        }
    }
```
- 方式3

```java
@Override
    protected void onStart() {
        super.onStart();
        ViewTreeObserver observer = imgView.getViewTreeObserver();
        observer.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                // TODO: 2017/12/7  获取尺寸
                imgView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                int hight = imgView.getMeasuredHeight();
            }
        });
    }
```

### 坐标系探究

**介绍：** 安卓手机屏幕的坐标系也是一个经典的三维坐标。现以你手持phone，并且正在刷今日头条的情景条件来分析。此时，手机的上边缘-指向右的方向为 X轴的正方向；手机边缘左边缘-指向下的方向为Y轴的正方向；手机的屏幕指向你的大脸盘子的方向为Z轴的正方向；明白了吗？！

![这里写图片描述](https://img-blog.csdn.net/20171207113137770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上图详述了View在屏幕中的静态坐标系，也给我们展示了当用户手指触摸屏幕时MotionEvent 所展示的一些坐标表示方法。**需要我们认真的理解！**

![这里写图片描述](https://img-blog.csdn.net/20171207113842041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171207115122444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而这二张图所表示的内容
|View方法|View1结果|View2结果|结论依据|
|----|----|----|----|
|getLocalVisibleRect|(0, 0 , 410, 100)|(0, 0 , 410, 470)|getGlobalVisibleRect 方法的作用是获取视图本身可见的坐标区域，坐标以自己的左上角为原点（0，0）|
|getGlobalVisibleRect|(30, 100 , 440, 200)|(30, 250 , 440, 720)|getGlobalVisibleRect 方法的作用是获取视图在屏幕坐标中的可视区域，坐标以手机屏幕的左上角为原点（0，0）|

#### getLocalVisibleRect

*(0, 0 , 410, 100) = （30-30， 100-100， 440-30， 200-100）*
*(0, 0 , 410, 470) = （30-30， 250-250， 440-30， 720-250）*

#### getGlobalVisibleRect

就是通过view的顶点坐标直接拿来使用就是，但是是**可见区域**。

### 弹性滑动
- Scroller 

弹性滑动的对象，用于实现View的弹性滑动。当使用View的scrollTo / scrollBy 方法进行滑动的时候，其过程是瞬间完成的。这样对于用户来说，体验当然不好。这时候就能狗使用Scroller 来实现过渡效果滑动。 

** 注意 **Scroller 本身无法让View 进行弹性滑动，他需要和View 的computeScroll 方法配合使用才能够共同完成这个功能。
```java
//在自定义的View类中编辑
    Scroller scroller = new Scroller(this);
    //缓慢的滑倒指定的位置
    private void smootScroollTo(int destX, int destY){
        int scrollX = getScrollX();
        int delta = destX - scrollX;
        scroller.startScroll(scrollX, 0, delta, 0, 1000);
        //1000ms 内滑向 destX， 效果就是慢慢的滑动
        invalidate();
    }

    @Override
    public void computeScroll() {
        super.computeScroll();
        if(scroller.computeScrollOffset()){
            scrollTo(scroller.getCurrX(), scroller.getCurrY());
            postInvalidate();
        }
    }
```
- scrollTo/scrollBy 

scrollTo/scrollBy 只能改变View 的内容位置，而不能改变View 在布局中的位置。其移动单位是**像素**。滑动过程中，如果从左向右滑动，mScrollX<滑动的距离>为负值，反之为正值；从上向下滑动，mScrollY<滑动的距离>为负值，反之为正值；scrollBy(int x, int y) 相当于在scrollTo()的基础上继续滑动x，y，即每次的累加，看源码可知：

```java
//View的源码
    @Override
    public void scrollBy(@Px int x, @Px int y) {
        scrollTo(mScrollX + x, mScrollY + y);
    }
```




