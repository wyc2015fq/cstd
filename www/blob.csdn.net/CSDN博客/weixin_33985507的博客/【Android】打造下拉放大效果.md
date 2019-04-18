# 【Android】打造下拉放大效果 - weixin_33985507的博客 - CSDN博客
2018年06月24日 06:24:44[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
## 前言
在其他App上看到了这样的一个效果，感觉有点意思，于是决定实现一个类似的效果。
（*其实是iOS的同学在是现功能的时候随意发挥了一下*）
效果大概值这样子的：
**UI看完后**
“这个效果不错啊”
“要不你们Android也么做？” 于是~~
**作为一个有追求的程序员，决定也要实现一个这样的效果**（满脑子都是草泥马在奔腾）
## 思路
这样的效果嘛~~
利用自定义的**ViewGroup**，通过对手势的处理，应该就能实现了吧？
主要应该分两部分：
- 判断手势，如果为下拉操作，获取下拉的距离来实现**View**的放大
- 当手松开的时候，重置**View**的高度
比较麻烦的应该是在第一部分，需要对事件的分发有一些理解。
## 关于事件分发
说到手势的判断，难免需要对事件分发进行处理。
**下拉部分**
1、在`onInterceptTouchEvent`中对事件进行处理，如果为下拉事件，则将该事件拦截，交给`onTouchEvent`处理；
2、在`onTouchEvent`中通过计算得到下拉的距离，然后动态改变**Header**的配置，实现放大的效果。
**重置部分**
在`onTouchEvent`的**ACTION_UP**中重置**Header**，实现回弹
> 
对事件分发不了解的，这边有两篇不错的文章
[Android事件分发机制 详解攻略，您值得拥有](https://link.juejin.im?target=https%3A%2F%2Fblog.csdn.net%2Fcarson_ho%2Farticle%2Fdetails%2F54136311)[图解 Android 事件分发机制](https://link.juejin.im?target=https%3A%2F%2Fwww.jianshu.com%2Fp%2Fe99b5e8bd67b)
## 实现
知道思路以后，实现起来就比较简单了
#### 拦截事件
创建一个**ViewGroup**（这么命名为**FlexibleLayout**）继承**LinearLayout**。
**onInterceptTouchEvent的处理**
- 在`onInterceptTouchEvent`中的**DOWN**事件中记录触摸位置
- 在**MOVE**事件中判断是否为下拉动作，若为下拉事件则进行拦截。
```java
public class FlexibleLayout extends LinearLayout{
    /**
     * true：开始下拽
     */
    private boolean mIsBeingDragged;
    /**
     * 初始坐标
     */
    private float mInitialY, mInitialX;
    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        switch (ev.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mInitialX = ev.getX();
                mInitialY = ev.getY();
                mIsBeingDragged = false;
                break;
            case MotionEvent.ACTION_MOVE:
                float diffY = ev.getY() - mInitialY;
                float diffX = ev.getX() - mInitialX;
                if (diffY > 0 && diffY / Math.abs(diffX) > 2) {
                    mIsBeingDragged = true;
                    return true;
                }
                break;
        }
        return super.onInterceptTouchEvent(ev);
    }
}
复制代码
```
先通过两个条件判断是否为下拉事件：
- **diffY > 0** : **Y**轴向下滑动
- **diffY / Math.abs(diffX) > 2**: **Y**轴滑动的距离超过**X**轴的两倍
然后通过`mIsBeingDragged`来标记开始拖拽
**onTouchEvent的处理**
- 在`onTouchEvent`中的**MOVE**事件中获取**Y**轴移动的距离，动态改变头部的大小
- 在**UP**或**CANCEL**事件中，重置头部
```java
@Override
public boolean onTouchEvent(MotionEvent ev) {
    switch (ev.getAction()) {
        case MotionEvent.ACTION_MOVE:
            if (mIsBeingDragged) {
                //得到下拉的距离
                float diffY = ev.getY() - mInitialY;
                changeHeader((int) diffY);
                return true;
            }
            break;
        case MotionEvent.ACTION_CANCEL:
        case MotionEvent.ACTION_UP:
            if (mIsBeingDragged) {
                //重置头部
                resetHeader();
                return true;
            }
            break;
    }
    return super.onTouchEvent(ev);
}
复制代码
```
**修改头部大小**
得到下拉的距离后，就可以来改变**Header**的大小，实现放大效果了。
**放大、重置的部分大家可以自由发挥**
这里利用`Math.pow(offsetY, 0.8)`得到实际需要增加的高度，通过计算得到对应的宽度以及偏移（类似阻尼效果）。
```java
/**
 * 改变头部大小
 * @param offsetY
 */
private void changeHeader(int offsetY) {
    int pullOffset = (int) Math.pow(offsetY, 0.8);
    int newHeight = pullOffset + mHeaderHeight;
    int newWidth = (int) ((((float) newHeight / mHeaderHeight)) * mHeaderWidth);
    mHeaderView.getLayoutParams().height = newHeight;
    mHeaderView.getLayoutParams().width = newWidth;
    int margin = (newWidth - mHeaderWidth) / 2;
    mHeaderView.setTranslationX(-margin);
    mHeaderView.requestLayout();
}
复制代码
```
**重置头部**
直接将宽高以及偏移设置成原来的参数即可。
（如果觉得这样重置过程不够丝滑，可以通过动画来完成一个流畅的重置效果，这里就不演示了）
```java
/**
 * 重置头部
 */
private void resetHeader() {
    mHeaderView.getLayoutParams().height = mHeaderHeight;
    mHeaderView.getLayoutParams().width = mHeaderWidth;
    mHeaderView.setTranslationX(0);
    mHeaderView.requestLayout();
}
复制代码
```
到这里，一个简易拉下放大的效果就做完了。试试效果
**使用**
直接在需要下拉放大的布局外面套上**FlexibleLayout**即可，例如**ScrollView**
```xml
<com.gavin.view.flexible.FlexibleLayout 
    android:layout_width="match_parent"
    android:layout_height="match_parent">
    <ScrollView
        ... >
        <LinearLayout
            ... >
            <ImageView
                android:id="@+id/iv_header"
                ... />
        </LinearLayout>
    </ScrollView>
</com.gavin.view.flexible.FlexibleLayout>
复制代码
```
**效果**
ScrollVIew：
RecyclerView：
CoordinatorLayout:
大功告成！！！
当然里面还有一些细节的处理，比如下拉的条件、回弹的动画、最大高度等，具体内容的可以在[源码](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FGavin-ZYX%2FFlexibleLayout)中看到。
## 完善
完成下拉放大后，貌似把一个很重要的功能遗忘了下拉刷新 ？？
光顾这下拉放大，刷新怎么办？【黑人问号】
这个功能留着下周实现吧，我的7小时睡眠已遥遥无期~~
虽然没有直接实现下拉刷新的功能，不过源码中已经暴露了一个下拉的监听，你也可以通过这个监听实现下拉刷新的操作
```java
public interface OnPullListener {
    /**
     * 下拉
     * @param offset
     */
    void onPull(int offset);
    /**
     * 松开
     */
    void onRelease();
}
复制代码
```
到这里就结束了
## 下拉刷新（2018-6-24补充）
（来还上周欠下的债~~）
和下拉放大类似，通过希手指下滑的监听，利用**View**的**translationY**和**rotation**实现移动和旋转。
具体的实现过程这里就不贴出来了，直接看效果吧
有兴趣的可以直接去[Github](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FGavin-ZYX%2FFlexibleLayout)上看源码以及用法。
## 源码
[Github](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FGavin-ZYX%2FFlexibleLayout)
## 参考
[PullZoomView](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FFrank-Zhu%2FPullZoomView)[Android事件分发机制 详解攻略，您值得拥有](https://link.juejin.im?target=https%3A%2F%2Fblog.csdn.net%2Fcarson_ho%2Farticle%2Fdetails%2F54136311)
> 
以上有错误之处，感谢指出
