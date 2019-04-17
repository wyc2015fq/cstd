# Android动画实践结论 - DEVELOPER - CSDN博客





2014年08月16日 19:09:35[学术袁](https://me.csdn.net/u012827205)阅读数：1084








实践出真知：

***一>平移动画***

```java
overridePendingTransition(R.anim.enterAnim,R.anim.exitAnim);
```

方法中第一个参数表示进enterAnim，第二个参数表示出exitAnim；怎么理解呢？

比如：如果我写出这样的一个效果，如同在网易新闻上的一样。当你点击一条新闻的时候，好像当前页面不动，然后所请求的新闻页面从右向左逐渐平移过来并完全遮掩（覆盖）当前页面。如果实现它，看XML代码：

```java
/**  enterAnim  */
```

```java
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@android:anim/accelerate_interpolator" >
    <translate
        android:duration="500"
        android:fromXDelta="100%"
        android:toXDelta="0%" 
        android:interpolator="@android:anim/accelerate_interpolator"/>

</set>
```

```java
/** exitAnim  */
```

```java
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@android:anim/accelerate_interpolator" >
    <translate
        android:duration="500"
        android:fromXDelta="0%"
        android:interpolator="@android:anim/decelerate_interpolator"
        android:toXDelta="0%" />

</set>
```

然后，当你要跳转到另一页面时候在startActivity(Intent intent);之后放置上面的方法。

运行一下效果你就会知道，所谓的“进”就等同于，当前不想要看到的页面。咱要让它消失；相反，则”出“就是让你想要看到的新页面呈现出来并遮住(覆盖)当前页；

当点击返回时候，应用效果和上面的点击效果相反；效果：当前页向右平移撤去，原先当前页下面的页面原封的呈现出来。

当然还是使用上面的方法，此时XML代码如下；

```java
/** enterAnim  */
```

```java
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@android:anim/accelerate_interpolator" >       
         <translate
        android:duration="500"
        android:fromXDelta="0%"
        android:interpolator="@android:anim/accelerate_interpolator"
        android:toXDelta="0%" /> 

</set>
```

```java
/** exitAnim  */
```

```java
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
    android:interpolator="@android:anim/accelerate_interpolator" >
        
        <translate
        android:duration="500"
        android:fromXDelta="0%"
        android:interpolator="@android:anim/decelerate_interpolator"
        android:toXDelta="100%" />

</set>
```

依然使用方法:

```java
overridePendingTransition(R.anim.enterAnim,R.anim.exitAnim);
```



思路与请求效果思路相反；

其实，平移就是相当于一条河流；首先要知道该河流要想那个方向走。

比如向左：设置定方法的第一个参数（令当前页面向左平移）是从右向左移动，紧跟着请求页面（第二个参数）也是同方向从右向左平移。这样才能实现平移的流动性；



待续。。。。









