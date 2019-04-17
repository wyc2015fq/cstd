# Android应用资源之动画资源（Animation Resources） - 一世豁然的专栏 - CSDN博客





2017年04月24日 20:44:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：249








本博客转载于：[http://mobile.51cto.com/aprogram-431219.htm](http://mobile.51cto.com/aprogram-431219.htm)










有两种类型的动画资源：

属性动画


在设定的时间内，通过修改与Animator类相关的对象的属性值来创建一个动画。

视图动画


有两种类型的视图动画框架


补间动画（Tween animation）：通过执行通过执行一系列的与Animation类相关的单一图片的转换来创建动画。


帧动画（Frame animation）：通过显示与AnimationDrawable类的顺序相关的图片序列来创建动画。

属性动画


动画被定义在要修改的目标对象属性的XML中，如设定时间内的背景色或alpha值。

文件位置（FILE LOCATION）：


res/animator/filename.xml，文件名被用作资源ID。

被编译的资源类型（COMPILED RSOURCE DATATYPE）：


资源要指向ValueAnimator、ObjectAnimator或AnimatorSet类型

资源引用（RESOURCE REFERENCE）：


在Java代码中：R.animation.filename


在XML文件中：@[package:]animator/filename

语法（SYNTAX）：
- <set 
-   android:ordering=["together" | "sequentially"]> 
- 
-     <objectAnimator 
-         android:propertyName="string"
-         android:duration="int"
-         android:valueFrom="float | int | color"
-         android:valueTo="float | int | color"
-         android:startOffset="int"
-         android:repeatCount="int"
-         android:repeatMode=["repeat" | "reverse"] 
-         android:valueType=["intType" | "floatType"]/> 
- 
-     <animator 
-         android:duration="int"
-         android:valueFrom="float | int | color"
-         android:valueTo="float | int | color"
-         android:startOffset="int"
-         android:repeatCount="int"
-         android:repeatMode=["repeat" | "reverse"] 
-         android:valueType=["intType" | "floatType"]/> 
- 
-     <set> 
-         ... 
-     </set> 
- </set> 


这个文件必须有一个单一的跟元素：既可以是<set>、<objectAnimator>也可以是<valueAnimator>。可以把动画元素一起组织到<set>元素中，包括其他的<set>元素。

元素（ELEMENTS）：


<set>


能够包含其他动画元素的一个容器（<objectAnimator>、<valueAnimator>、或其他的<set>元素）。代表一个AnimatorSet对象。


可以指定<set>嵌套标签，把更多的动画组织到一起。每个<set>能够定义它自己的顺序属性。

属性（ATTRIBUTES）：


android:ordering


关键属性，它指定该集合中动画的播放顺序。
|值|说明|
|----|----|
|sequentially|在本组集合中按顺序播放动画。|
|together(默认)|同一时间播放本组集合中的动画|


<objectAnimator>


在特定的设定时间内一个对象的具体动画属性，代表了一个ObjectAnimator对象。

属性（ATTRIBUTES）：

android:propertyName


字符串类型，必须的。通过这个名字来引用动画的对象属性。例如，可以给View对象指定“alpha”或“backgroundColor”属性。ObjectAnimator元素没有暴露target属性，因此不能在XML声明中把对象设置给动画。必须通过调用loadAnimator()方法和setTarget()方法把动画XML资源填充到包含这个属性的目标对象中。

android:valueTo


浮点类型、整型、或颜色值，必须的。它是动画属性的结束值。颜色是用六个十六进制数表示（如#333333）。

android:valueFrom


浮点类型、整型、或颜色值，它是动画属性的开始值，如果没有指定，那么动画会在用属性的get方法获取值的时刻开始。颜色是六位十六进制数表示（如#333333）。

android:duration


整数类型。以毫秒为单位，指定动画时间，默认是300毫秒。

android:startOffset


整数类型。在start()方法被调用之后，动画延迟的毫秒数。

android:repeatCount


整数类型。指定动画的重复次数。设置为“-1”，说明要无限次重复，或指定一个正整数。如，设定为1，说明动画第一次运行之后，还要重复一次，因此动画要播放两次。默认值是0，意味着不重复。

android:repeatMode


正整数。指定在动画到达结尾时的行为方式。只有android:repeatCount被设置为正整数或“-1”时，这个属性才有效果。如果设置为“reverse”，那么每次重复时会逆向播放动画，如果设置为“repeat”，那么每次重复时，会从动画的开头来播放动画。

android:valueType


关键属性。如果值是颜色值，那么不用指定这个属性。动画框架会自动处理颜色值。
|值|说明|
|----|----|
|intType|指定动画的值是整数|
|floatType（默认）|指定动画的值是浮点数|

<animator>


在设定时间内的动画。代表了一个ValueAnimator对象。

属性（ATTRIBUTES）：

android:valueTo


浮点型、整型或颜色值，必须的。它是动画结束的的值。颜色值用六位十六进制数来代表（如，#333333）

android:valueFrom


浮点型、整型或颜色值，必须的。它是动画开始的值。颜色值用六位十六进制数来代表（如，#333333）

android:duration


整数。以毫秒为单位，指定动画的播放时间。默认是300毫秒。

android:startOffset


整数。指定start()方法被调用后，动画延迟的毫秒数。

android:repeatCount


整数。指定动画的重复次数。如果设置为“-1”，则动画会无限次重复，或者指定一个正整数。例如，设定为“1”，那么动画在初始运行一次之后，会再重复播放一次，因此动画总共播放了两次。它的默认值是0，这意味着动画重复。

android:repeatMode


整数。它指定动画到达结尾时重复的行为方式。android:repeatCount必须被设置为正整数或“-1”时，这个属性才会有效果。如果设置为“reverse”，动画在每次重播时，会逆向播放；如果设置为“repeat”，那么动画每次重播时，会重开始的位置播放。

android:valueType


关键属性，如果值是颜色，则不用指定这个属性。因为动画框架会自动的处理颜色值
|值|说明|
|----|----|
|intType|指定动画的值是整数|
|floatType（默认）|指定动画的值是浮点数|

例子（EXAMPLE）：


这个XML文件被保存在res/animator/property_animator.xml：
- <setandroid:ordering="sequentially">
- <set>
- <objectAnimator
- android:propertyName="x"
- android:duration="500"
- android:valueTo="400"
- android:valueType="intType"/>
- <objectAnimator
- android:propertyName="y"
- android:duration="500"
- android:valueTo="300"
- android:valueType="intType"/>
- </set>
- <objectAnimator
- android:propertyName="alpha"
- android:duration="500"
- android:valueTo="1f"/>
- </set>


要运行这个动画，就必须在代码中把这个XML资源填充给一个AnimatorSet对象，然后在启动这个动画集之前，把目标对象设置个所有的动画。调用setTarget方法可以方便的把一个单一对象设置给AnimatorSet对象的所有子对象。代码如下：
- AnimatorSet set = (AnimatorSet) AnimatorInflater.loadAnimator(myContext,    R.anim.property_animator); 
- 
- set.setTarget(myObject);set.start();




