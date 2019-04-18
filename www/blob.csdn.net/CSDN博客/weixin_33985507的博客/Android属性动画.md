# Android属性动画 - weixin_33985507的博客 - CSDN博客
2016年11月15日 11:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
Android动画分为三种：1.Tween动画，也叫补间动画  2.帧动画  3.属性动画。今天要说的就是第三种动画，属性动画。属性动画主要是改变对象本身的属性来形成的动画。属性动画是Android3.0以后才开始引用的，所以只有在3.0以上才能够使用，如果要兼容3.0以下的，需要使用开源库。
下面就来简单介绍以下属性动画的基本用法。
属性动画主要包括：AnimatorSet, ValueAnimator，ObjectAnimator, TimeAnimator。它们之间的关系如图：
![1460594-170b13ace20a14f6.png](https://upload-images.jianshu.io/upload_images/1460594-170b13ace20a14f6.png)
**ValueAnimator**
ValueAnimator是直接继承Animator的类。这个类负责计算从开始值到结束值之间，在不同的时间点的值。直白的说就是在给定的时间段内，负责将开始值平滑的过渡到结束值。主要API执行动画的api有:
![1460594-c023b0e006377455.png](https://upload-images.jianshu.io/upload_images/1460594-c023b0e006377455.png)
可以看出参数都是多参的，里面可以放多个值，第一个值是startValue(开始值)，后面的几个参数就是动画的结束值。比如参数是（1，2）就是从值从1平滑变化到2，参数是（1，3，7）就是先从1变化到3，再从3变换到7，其中执行时间是各占一半，也就是说如果动画的总时间为2s,那么从1到3执行时间是1s,从3到7的执行时间是1S。
上代码：
```
private void testValueAnimator() {
    //1.0f-3.0f用50%的时间，3.0f-10.0f用剩下50%的时间
    ValueAnimator valueAnimator = ValueAnimator.ofFloat(1.0f, 3.0f, 10.0f);
    //每次动画的时间（不包括重复执行的时间）
    valueAnimator.setDuration(1000);
    valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
        @Override
        public void onAnimationUpdate(ValueAnimator animation) {
             //动画执行百分比
            float animatedFraction = animation.getAnimatedFraction();
            //动画现在的值
            Object animatedValue = animation.getAnimatedValue();
            //执行时间
            long currentPlayTime = animation.getCurrentPlayTime();
            Log.i(TAG,"ValueAnimator animatedFraction:"+animatedFraction);
            Log.i(TAG,"ValueAnimator animatedValue:"+animatedValue);
            Log.i(TAG,"ValueAnimator currentPlayTime:"+currentPlayTime);
        }
    });
    //
    valueAnimator.setRepeatCount(1);
    valueAnimator.start();
```
执行结果为：
其他几个方法的用法也是类似的。ValueAnimator也可以添加各种监听，比如上面代码添加的AnimatorUpdateListener就是用来实时监听动画执行情况的。还可以添加为动画添加开始、结束的监听，同之前的Tween动画一样。
```
valueAnimator.addListener(new Animator.AnimatorListener() {
        @Override
        public void onAnimationStart(Animator animation) {
        }
        @Override
        public void onAnimationEnd(Animator animation) {
        }
        @Override
        public void onAnimationCancel(Animator animation) {
        }
        @Override
        public void onAnimationRepeat(Animator animation) {
        }
    });
```
有时，我们并不需要对动画的开始、结束、取消、重复都进行监听，那么就可以选择这个AnimatorListenerAdapter来代替AnimatorListener，这里面帮我们实现了AnimatorListener接口的所有方法，当然方法里面都是空的，什么也没做，我们只需要重写需要监听的方法就OK了，这样代码又简洁了不少哦。
**ObjectAnimator**
ObjectAnimator是ValueAnimator的子类，从名字可以看出是一个关于对象的动画。事实上它确实是通过改变某个目标对象上的属性来产生动画的。比如修改一个view对象的旋转角度等。这个类修改的对象的属性必须有get和set方法，否则是不起作用的。
```
//绕z轴旋转
    ObjectAnimator objectAnimator = ObjectAnimator.ofFloat(textView, "rotation", 1,50);
    objectAnimator.setDuration(3000);
    objectAnimator.start();
```
上面代码就是将一个textview绕z轴从1度经过3s的时间平滑旋转到50度。同时，这个也是我们做view动画经常用到的一个类。
**AnimatorSet**
上面介绍的都是单一的动画，那么怎么在同一个对象上同时进行两个或者两个以上的动画呢，答案自然是用AnimatorSet了。首先来看一下AnimatorSet的API.
![1460594-56c932fd483c15aa.png](https://upload-images.jianshu.io/upload_images/1460594-56c932fd483c15aa.png)
So easy!从表面就可以看出来怎么用了。然鹅，AnimatorSet不仅仅可以这样用，还有其他的用法，比如：
![1460594-e6cd0e0ebf55798c.png](https://upload-images.jianshu.io/upload_images/1460594-e6cd0e0ebf55798c.png)
![1460594-ff30a8b1063e739d.png](https://upload-images.jianshu.io/upload_images/1460594-ff30a8b1063e739d.png)
首先调用play(animator1)方法返回一个builder,然后就可以调用after(animator2)、before(animator3)、with(animator4)等方法再次传入一个动画，并且还可以决定两个动画执行的先后顺序。after(animator2)表示animator2先执行，animator1后执行。before(animator3)表示animator1在animator3之前执行。with(animator4)自然就是两个动画同时执行，和playTogether()的功能很相似了。
```
ObjectAnimator rotationY = ObjectAnimator.ofFloat(tv, "rotationY", 0, 30);
    ObjectAnimator rotationX = ObjectAnimator.ofFloat(tv, "rotationX", 0, 30);
    AnimatorSet set=new AnimatorSet();
   //set.playTogether(rotationX,rotationY);
    set.play(rotationX).before(rotationY);
    set.setDuration(6000);
    set.start();
```
**xml动画**
对于Tween动画，我们都知道可以用xml文件来写动画，那么作为后起之秀的属性动画，在这方面自然不能落后了，也是可以用xml来写动画的。
比如ObjectAnimator的动画，我可以这样写：
```
<objectAnimator xmlns:android="http://schemas.android.com/apk/res/android"
android:duration="3000"
android:propertyName="translationX"
android:repeatCount="1"
android:repeatMode="reverse"
android:valueFrom="0"
android:valueTo="300"
android:valueType="floatType" />
```
在xml中我们无法设置target,需要在代码中来设置：
```
Animator animator = AnimatorInflater.loadAnimator(this, R.animator.animator);
  animator.setTarget(tv);
  animator.start();
```
ok，用xml来写属性动画和之前的Tween动画十分相似。
**TypeEvaluator**
估值器，一个用来负责计算动画当前值得类。比如我要把一个对象的属性xxx的值在3s内从1变到10，TypeEvaluator就负责计算在1秒的时候xxx的值应该是多少，在1.1s的时候xxx的值应该是多少。
![1460594-df7b4da9499d2bbe.png](https://upload-images.jianshu.io/upload_images/1460594-df7b4da9499d2bbe.png)
这个是TypeEvaluator需要实现的方法，这三个参数分别为fraction:当前动画执行的百分比   startValue:动画执行的初始值   endValue:动画执行的结束值。  返回值为在当前时间的动画值。如果为自己的动画设置了TypeEvaluator,那么动画的值将按照evaluate()返回的值来变化。
通常计算动画值得方法为：
startValue + fraction * (endValue - startValue);比如动画的值是从100变化到300，执行到20%时，此时动画的值为
100+20%*（300-100）=140。当然了，这个值的变化规则是我们自己定的，我们可以在方法里面根据需要随意的修改动画值得变化规则。看代码：
```
ValueAnimator ofObject = ObjectAnimator.ofObject(student,"score",new TypeEvaluator() {
        @Override
        public Object evaluate(float fraction, Object startValue, Object endValue) {
            int start = (int) startValue;
            int end = (int) endValue;
            float current = start + fraction * (end - start);
            //返回值乘以10
            return current*10;
            //return 5;
        }
    }, 100, 300);
    ofObject.setDuration(3000);
    ofObject.start();
```
这时候，student的score值就从1000逐渐变成了3000，而不是我们上面传入的从100到300.同时如果传入一个固定值5，那么score的值将一直是5， 打印结果为：
![1460594-31b619953a0800c5.png](https://upload-images.jianshu.io/upload_images/1460594-31b619953a0800c5.png)
Ok,属性动画就先介绍到这里了。
