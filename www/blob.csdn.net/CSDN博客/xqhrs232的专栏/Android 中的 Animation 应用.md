# Android 中的 Animation 应用 - xqhrs232的专栏 - CSDN博客
2010年12月31日 10:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2226标签：[animation																[android																[transformation																[float																[xml																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=transformation&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
Android 中的 Animation 应用（一）
原文地址::[http://ybxu-123.blog.163.com/blog/static/5947377020106632910792/](http://ybxu-123.blog.163.com/blog/static/5947377020106632910792/)
最近在应用中用到了 Animation 。在网上浏览了下，感觉有些东西讲得很精辟。因此，我想总结下。
Drawable 最强大的功能是：显示Animation。Android SDK介绍了2种Animation：
- Tween Animation(渐变动画)：通过对场景里的对象不断做图像变换(平移、缩放、旋转)产生动画效果 
- Frame Animation(帧动画)   ：顺序播放事先做好的图像，类似放电影 
在使用Animation前，我们先学习如何定义Animation，这对我们使用Animation会有很大的帮助。Animation是以XML格式定义的，定义好的XML文件存放在res/anim中。由于Tween Animation与Frame Animation的定义、使用都有很大的差异，我们将分开介绍，本篇幅中介绍Tween Animation的定义与使用，后续篇幅再详细介绍Frame Animation。按照XML文档的结构【父节点，子节点，属性】来介绍Tween
 Animation，其由4种类型：
- Alpha：渐变透明度动画效果 
- Scale：渐变尺寸伸缩动画效果 
- Translate：画面转换位置移动动画效果 
- Rotate：画面转换位置移动动画效果 
在介绍以上4种类型前，先介绍Tween Animation共同的节点属性。
zAdjustment[int]定义动画的Z Order的改变0：保持Z Order不变
1：保持在最上层
-1：保持在最下层
看了以上节点，大家是不是都想开始定义动画了。下面我们就开始结合具体的例子，介绍4种类型各自特有的节点元素。
toAlpha属性为动画结束时透明度
按照上面的讲述学习完了Tween Animation的定义，对Tween Animation有了详细的了解，再去了解下Android SDK的animation package（android.view.animation），其提供了操作Tween Animation所有的类。
Android SDK提供了基类：Animation，包含大量的set/getXXXX()函数来设置、读取Animation的属性，也就是前面表一中显示的各种属性。Tween Animation由4种类型：alpha、scale、translate、roate，在Android SDK中提供了相应的类，Animation类派生出了AlphaAnimation、ScaleAnimation、TranslateAnimation、RotateAnimation分别实现了平移、旋转、改变
 Alpha 值等动画，每个子类都在父类的基础上增加了各自独有的属性。再去看下这几个类的构造函数，是不是与我们在表二、表三、表四、表五种定义的属性完全一样。
[](http://www.moandroid.com/wp-content/uploads/2009/09/Animation.JPG)
![Android 中的 Animation 应用（一） - Langston - Langstons世界](http://img.ph.126.net/348BfgAbL-OB0_xL5wxWdA==/3734328516020388656.jpg)
 在了解了Tween Animation的定义，对android.view.animation有了一些基本的认识后，开始介绍Tween Animation如何使用。Android SDK提供了2种方法：1、直接从XML资源中读取Animation；2、使用Animation子类的构造函数来初始化Animation对象。第二种方法在看了Android
 SDK中各个类的说明就知道如何使用了，下面简要说明从XML资源中读取Animation，按照应用程序开发的过程，介绍整个使用的过程，如下：
- 创建Android工程； 
- 导入一张图片资源； 
- 在res/layout/main.xml中添加一个 ImageView Widget； 
- 在res下创建新的文件夹且命名为：anim，并在此文件夹下面定义 Animation XML 文件；
- 修改OnCreate()中的代码，显示动画资源； 
关键代码，解析如下：
//main.xml中的ImageView
ImageView spaceshipImage = (ImageView) findViewById(R.id.spaceshipImage);
//加载动画
Animation hyperspaceJumpAnimation =
AnimationUtils.loadAnimation(this, R.anim.hyperspace_jump);
//使用ImageView显示动画
spaceshipImage.startAnimation(hyperspaceJumpAnimation);
这里简要解析如下：
- AnimationUtils提供了加载动画的函数，除了函数loadAnimation()，其他的到Android SDK中去详细了解吧； 
- 所谓的动画，也就是对 view 的内容做一次图形变换；
Android 中的 Animation 应用（二）
原文地址::[http://ybxu-123.blog.163.com/blog/static/594737702010663391680/](http://ybxu-123.blog.163.com/blog/static/594737702010663391680/)
对Tween Animation的本质做个总结：Tween Animation通过对 View 的内容完成一系列的图形变换 (包括平移、缩放、旋转、改变透明度)来实现动画效果。具体来讲，预先定义一组指令，这些指令指定了图形变换的类型、触发时间、持续时间。这些指令可以是以 XML 文件方式定义，也可以是以源代码方式定义。程序沿着时间线执行这些指令就可以实现动画效果。
在这里，我们需要对2个问题进行深入的解析：
- 动画的运行时如何控制的？ 
- 动画的运行模式。 
### 如何控制动画的运行？
这个问题，我们也就也就是上一篇幅中提到的Tween Animation，估计大家对什么是Interpolator、到底有什么作用，还是一头雾水，在这里做个详细的说明。按照Android SDK中对interpolator的说明：interpolator定义一个动画的变化率（the rate of change）。这使得基本的动画效果(alpha, scale, translate, rotate）得以加速，减速，重复等。
用通俗的一点的话理解就是：动画的进度使用 Interpolator 控制。Interpolator 定义了动画的变化速度，可以实现匀速、正加速、负加速、无规则变加速等。Interpolator 是基类，封装了所有 Interpolator 的共同方法，它只有一个方法，即 getInterpolation (float input)，该方法 maps a point on the timeline to a multiplier to be applied to the transformations of
 an animation。Android 提供了几个 Interpolator 子类，实现了不同的速度曲线，如下：
|AccelerateDecelerateInterpolator|在动画开始与介绍的地方速率改变比较慢，在中间的时候加速|
|----|----|
|AccelerateInterpolator|在动画开始的地方速率改变比较慢，然后开始加速|
|CycleInterpolator|动画循环播放特定的次数，速率改变沿着正弦曲线|
|DecelerateInterpolator|在动画开始的地方速率改变比较慢，然后开始减速|
|LinearInterpolator|在动画的以均匀的速率改变|
对于 LinearInterpolator ，变化率是个常数，即 f (x) = x.
public float getInterpolation(float input) {
return input;
}
Interpolator其他的几个子类，也都是按照特定的算法，实现了对变化率。还可以定义自己的 Interpolator 子类，实现抛物线、自由落体等物理效果。
### 动画的运行模式
动画的运行模式有两种：
- 独占模式，即程序主线程进入一个循环，根据动画指令不断刷新屏幕，直到动画结束； 
- 中断模式，即有单独一个线程对时间计数，每隔一定的时间向主线程发通知，主线程接到通知后更新屏幕； 
### 额外补充说明：Transformation 类
Transformation 记录了仿射矩阵 Matrix，动画每触发一次，会对原来的矩阵做一次运算， View 的 Bitmap 与这个矩阵相乘就可实现相应的操作(旋转、平移、缩放等)。Transformation 类封装了矩阵和 alpha 值，它有两个重要的成员，一是 mMatrix，二是 mAlpha。Transformation 类图如下所示：
![Transformation](http://www.moandroid.com/wp-content/uploads/2009/09/Transformation.JPG)
### 总结说明
图形变换通过仿射矩阵实现。图形变换是图形学中的基本知识，简单来讲，每种变换都是一次矩阵运算。在 Android 中，Canvas 类中包含当前矩阵，当调用 Canvas.drawBitmap (bmp, x, y, Paint) 绘制时，Android 会先把 bmp 做一次矩阵运算，然后将运算的结果显示在 Canvas 上。这样，编程人员只需不断修改 Canvas 的矩阵并刷新屏幕，View 里的对象就会不停的做图形变换，因此就形成了动画。
Android 中的 Animation 应用（三）
原文地址::[http://ybxu-123.blog.163.com/blog/static/5947377020106645516596/](http://ybxu-123.blog.163.com/blog/static/5947377020106645516596/)
前面我们详细介绍了Tween  Aniamation，这节我将介绍另外一种动画Frame Animation。在前面已经说过，Frame Animation是顺序播放事先做好的图像，与电影类似。不同于animation package， Android SDK提供了另外一个类AnimationDrawable来定义、使用Frame Animation。
Frame Animation可以在XML Resource定义（还是存放到res/anim文件夹下），也可以使用AnimationDrawable中的API定义。由于Tween Animation与Frame Animation有着很大的不同，因此XML定义的格式也完全不一样，其格式是：首先是animation-list根节点，animation-list根节点中包含多个item子节点，每个item节点定义一帧动画：当前帧的drawable资源和当前帧持续的时间。下面对节点的元素加以说明：
|XML属性|说明|
|----|----|
|drawable|当前帧引用的drawable资源|
|duration|当前帧显示的时间（毫秒为单位）|
|oneshot|如果为true，表示动画只播放一次停止在最后一帧上，如果设置为false表示动画循环播放。|
|variablePadding|If true, allows the drawable’s padding to change based on the current state that is selected.|
|visible|规定drawable的初始可见性，默认为flase；|
下面就给个具体的XML例子，来定义一帧一帧的动画：
<animation-list xmlns:android=”http://schemas.android.com/apk/res/android”
android:oneshot=”true”>
   <item android:drawable=”@drawable/rocket_thrust1″ android:duration=”200″ />
   <item android:drawable=”@drawable/rocket_thrust2″ android:duration=”200″ />
   <item android:drawable=”@drawable/rocket_thrust3″ android:duration=”200″ />
</animation-list>
上面的XML就定义了一个Frame Animation，其包含3帧动画，3帧动画中分别应用了drawable中的3张图片：rocket_thrust1，rocket_thrust2，rocket_thrust3，每帧动画持续200毫秒。
然后我们将以上XML保存在res/anim/文件夹下，命名为rocket_thrust.xml，显示动画的代码，如下：在OnCreate()中增加如下代码：
ImageView rocketImage = (ImageView) findViewById(R.id.rocket_image);
rocketImage.setBackgroundResource(R.anim.rocket_thrust);
rocketAnimation = (AnimationDrawable) rocketImage.getBackground();
最后还需要增加启动动画的代码：
public boolean onTouchEvent(MotionEvent event) {
if (event.getAction() == MotionEvent.ACTION_DOWN) {
rocketAnimation.start();
return true;
}
return super.onTouchEvent(event);
}
代码运行的结果想必大家应该就知道了（3张图片按照顺序的播放一次），不过有一点需要强调的是：启动Frame Animation动画的代码rocketAnimation.start(); 不能在OnCreate()中，因为在OnCreate()中AnimationDrawable还没有完全的与ImageView绑定，在OnCreate()中启动动画，就只能看到第一张图片。
下面，阅读Android SDK中对AnimationDrawable的介绍，有个简单的了解：
void stop()停止当前动画
### 总结说明
Frame Animation的定义、使用比较简单，在这里已经详细介绍完了，更加深入的学习还是到Android SDK去仔细了解吧，在Android SDK中也包含很多这方面的例子程序。**注：Frame Animation 的XML 文件中不定义 interpolator 属性，因为定义它没有任何意义。**
