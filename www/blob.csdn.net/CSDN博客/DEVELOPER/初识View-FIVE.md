# 初识View-FIVE - DEVELOPER - CSDN博客





2017年12月12日 13:32:42[学术袁](https://me.csdn.net/u012827205)阅读数：88
个人分类：[Android](https://blog.csdn.net/u012827205/article/category/1804657)

所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![这里写图片描述](https://img-blog.csdn.net/20171212105145010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 开篇短叙

做开发很久了，却一直没有真正的去面对自定义View这一块蛋糕。平时，在github或者博客上自学了很多这方面的知识，但是一直不是很系统，总是在真正使用的时候发现，自己在这方面的知识储备显得-很“笨拙”，只能应对一些简单的！！借着最近项目不紧张，再次进行View的系统学习，并在这里记录一下自己的学习过程。

### 摘要
- GestureDetector
- 事件分发机制 Dispatch

```java
GestureDetector
public class GestureDetector 
extends Object 

java.lang.Object
   ↳    android.view.GestureDetector
```

### GestureDetector  的使用
- 对于如何使用 GestureDetector ？
**比如通过 GestureDetector的一个静态内部类 SimpleOnGestureListener实现一个简单的手势功能：双击”666”，此功能是对某控件 targetView 双击操作！**
```java
// 第一步.创建一个监听器的内部类作为操作的回调；就是监听用户进行手势之后的操作结果响应。
SimpleOnGestureListener listener = new SimpleOnGestureListener() {
    @Override public boolean onDoubleTap(MotionEvent e) {
        Toast.makeText(MainActivity.this, "双击666", Toast.LENGTH_SHORT).show();
        return super.onDoubleTap(e);
    }
};

// 第二步.创建手势监听器对象，传入监听器的一个内部类对象，该内部类实现的是操作回调。
//构造方法：GestureDetector(Context context, GestureDetector.OnGestureListener listener)
final GestureDetector gestureDetector= new GestureDetector(this, listener);

// 第三步.通过手势检测对象，给监听器设置数据源。
targetView .setOnTouchListener(new View.OnTouchListener() {
    @Override public boolean onTouch(View v, MotionEvent event) {
        //这一行代码是起到一个关键作用的，决定是否执行手势操作。
        //最后的OnTouchEvent 事件交给手势监听器
        return gestureDetector.onTouchEvent(event);
    }
});
```

手指在手机屏幕上左右滑动，再比如，在一个普通的Activity页面进行操作。

```java
@Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_UP){
            int count = event.getPointerCount() ;
            float x1 = event.getX(0) ;
            float x2 = event.getX(count - 1);
            float scrollDistance = x1 - x2;
            if (scrollDistance > 100){
                Logger.d("向左滑--->>");
            }else if (scrollDistance<-100){
                Logger.d("向右滑--->>");
            }else {
            }
        }
        //最后的OnTouchEvent 事件交给手势监听器
        return gestureDetector.onTouchEvent(event);
    }
```
- **总结：**
step 1.定义我们的手势监听器，并实现监听接口。 

step 2.实现监听接口中的方法。 

step 3.最后将OnTouchEvent 事件交给手势监听器 `gestureDetector` 处理。
### 事件分发机制 整理

![这里写图片描述](https://img-blog.csdn.net/20171213141912549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

展示一下我们平时写代码布局时，这些布局都处在布局结构中的什么位置：
```java
<com.ztman.cusview.customView.RootView xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="wrap_content">

    <com.ztman.cusview.customView.GroupA
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:background="#ff88aa">

        <View1
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_marginTop="15dp"
            android:background="#aa88ee"
            android:text="View1" />
    </com.ztman.cusview.customView.GroupA>


    <View2
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="View2"
        android:textColor="#ff88aa" />

</com.ztman.cusview.customView.RootView>
```

#### 当我们点击屏幕上的view，事件传递又是怎么进行的
||return ture|return false|return super.dispatch**|
|----|----|----|----|
|ViewGroup->dispatchTouchEvent|拦截消费掉|拦截回溯|继续向下传递|
|view->dispatchTouchEvent|消费掉|回溯|回溯|
|ViewGroup->onInterceptTouchEvent|拦截消费掉|拦截回溯|拦截回溯|
|view->onInterceptTouchEvent|无此功能|无此功能|无此功能|
|ViewGroup->onTouchEvent|拦截消费掉|拦截回溯|继续向下传递|
|view->onTouchEvent|消费掉|回溯|回溯|
- ViewGroup 想把传递进来的事件分发给自己的 onTouchEvent，需要通过拦截器onInterceptTouchEvent 方法 return true 把事件拦截下来；




