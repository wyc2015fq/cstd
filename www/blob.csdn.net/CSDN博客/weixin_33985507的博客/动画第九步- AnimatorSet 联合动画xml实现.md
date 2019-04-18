# 动画第九步->AnimatorSet:联合动画xml实现 - weixin_33985507的博客 - CSDN博客
2016年12月21日 16:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
##### [极客学院Animation教程](https://link.jianshu.com?t=http://wiki.jikexueyuan.com/project/android-animation/)讲解的很详细，点击进入哦
这里为学习的整理和补充O(∩_∩)O
### 一、xml中标签对应的Animator
```
<animator />:对应 ValueAnimator
<objectAnimator />:对应 ObjectAnimator
<set />:对应 AnimatorSet
```
### 二、<animator />:对应 ValueAnimator
```
<animator
    android:duration="int"
    android:valueFrom="float | int | color"
    android:valueTo="float | int | color"
  //对应 startDelay(long delay)函数
    android:startOffset="int"
    android:repeatCount="int"
    android:repeatMode=["repeat" | "reverse"]
    android:valueType=["intType" | "floatType"]
    android:interpolator=["@android:interpolator/XXX"]/>
```
```
ValueAnimator valueAnimator = (ValueAnimator) AnimatorInflater.loadAnimator(context,
            R.animator.value_animator);
valueAnimator.start();
```
### 三、<objectAnimator />:对应 ObjectAnimator
```
<objectAnimator
    android:propertyName="string"
    android:duration="int"
    android:valueFrom="float | int | color"
    android:valueTo="float | int | color"
    android:startOffset="int"
    android:repeatCount="int"
    android:repeatMode=["repeat" | "reverse"]
    android:valueType=["intType" | "floatType"]
    android:interpolator=["@android:interpolator/XXX"]/>
```
```
ObjectAnimator animator = (ObjectAnimator) AnimatorInflater.loadAnimator(context,
        R.animator.object_animator);
animator.setTarget(mTv1);
animator.start();
```
### 四、<set />:对应 AnimatorSet
```
<set
  android:ordering=["together" | "sequentially"]>
    <objectAnimator
        android:propertyName="string"
        android:duration="int"
        android:valueFrom="float | int | color"
        android:valueTo="float | int | color"
        android:startOffset="int"
        android:repeatCount="int"
        android:repeatMode=["repeat" | "reverse"]
        android:valueType=["intType" | "floatType"]/>
    <animator
        android:duration="int"
        android:valueFrom="float | int | color"
        android:valueTo="float | int | color"
        android:startOffset="int"
        android:repeatCount="int"
        android:repeatMode=["repeat" | "reverse"]
        android:valueType=["intType" | "floatType"]/>
    <set>
        ...
    </set>
</set>
```
```
AnimatorSet set = (AnimatorSet) AnimatorInflater.loadAnimator(MyActivity.this,
        R.animator.set_animator);
set.setTarget(mTv1);
set.start();
```
demo:
```
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android"
     android:ordering="together">
    <objectAnimator
            android:propertyName="TranslationX"
            android:duration="500"
            android:valueFrom="0"
            android:valueTo="400"
            android:valueType="floatType"/>
    <objectAnimator
            android:propertyName="TranslationY"
            android:duration="500"
            android:valueFrom="0"
            android:valueTo="300"
            android:valueType="floatType"/>
</set>
```
