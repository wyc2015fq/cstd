# Android material design - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月16日 10:51:23[boonya](https://me.csdn.net/boonya)阅读数：992








关于Material Design的开源库，本人已经在GitHub上创建了一个项目[MaterialDesignCenter](https://github.com/lightSky/MaterialDesignCenter)，用于收集Material Design的相关资源,会不定时更新，大家可以star或者提交自己的MaterialDesign的资源，欢迎fork和star。

**Material Design**

官方Material Design详细介绍文档：[http://www.google.com/design/spec/material-design/introduction.html](http://www.google.com/design/spec/material-design/introduction.html)

Material Design是Android 5.0系统的重头戏，并在以后App中将成为一种设计标准，而且随着已支持Android 5.0 Lollipop的Nexus 6的开售，Google希望开发者更快的支持Material Design，连续发了3篇文章，来帮助开发者如何在自己的App 中实现Material Design。并表示：expect Winter 2014 to be a big quarter for design on Android.

目前使用过的Material Design 的Google App有 Google Play，Inbox（相当的酷炫，大家可以申请Invite，当然也可以通过已经拿到Invite的朋友帮忙激活），显然Material Design是必然的趋势，Google Play上越来越多的应用开始支持Material Design，因此我觉得写这样一篇文章也变得迫不及待。

官方Blog关于Material Design的专题文章一共有三篇：

### [AppCompat v21 — Material Design for Pre-Lollipop Devices!](http://android-developers.blogspot.jp/2014/10/appcompat-v21-material-design-for-pre.html)

### [Implementing material design in your Android app](http://android-developers.blogspot.jp/2014/10/implementing-material-design-in-your.html)

### [Material Design on Android Checklist](http://android-developers.blogspot.jp/2014/10/material-design-on-android-checklist.html)

因为第一篇已经在刚发出的时候翻译过了：[AppCompat V21：将 Materia Design 兼容到5.0之前的设备](http://blog.csdn.net/xushuaic/article/details/40212327) ，现在我们要介绍的是后两篇文章，后两篇主要从整体设计介绍了一个标准的Material Design的App应该遵循哪些设计标准，并告诉你如何在你的代码中实现，并介绍了如何向前兼容。其实后两篇在整体结构上是差不多的，只是在[Implementing
 material design in your Android app](http://android-developers.blogspot.jp/2014/10/implementing-material-design-in-your.html)中，侧重的是Material Design的设计标准，而[Material Design on Android Checklist](http://android-developers.blogspot.jp/2014/10/material-design-on-android-checklist.html)则侧重的是实现细节和注意事项，重点强调的部分为Signature
 element，实现部分为InCode。

尽管每一篇文章经过了仔细的斟酌和措辞，但还是觉得有一些设计方面的专业词汇表达的不是很好。针对文章翻译，如果有更好的建议，欢迎提出。除了翻译外，该篇文章还有一部分重要的内容，就是收集 了关于Material Design的资源和GitHub上一些关于Material Design的开源库，让大家更快更好的在自己的App中，应用Material Design。废话不多说，上正文。

**在你的App中实现Material Design**

Material design 是一种达到可视化，交互性，动效以及多屏幕适应的全面设计。Android 5.0 Lollipop和已经更新的support libraries将会帮助你构建Material UI。这里提供了一些API和Widget帮助你实现MaterialDesign设计。

## 一、Tangible Surfaces 有型的外观

UI由数字化的纸墨构成。表层和它带有的阴影为应用提供了可视化的效果。你可以触摸并观察他的运动，这个数字化的设计具有移动，扩张，或者转换的能力，帮助你创建灵活的UI。

**1.1 Shadows**

表面的位置和深度导致了在亮度和阴影方面的微弱变化。新的elevation 属性允许你在Z轴上指定View的位置，然后框架会为该View底部的items加上实时的动态阴影。你可以在布局中明确的以dp为单位设置它的高度。



```
```xml
<
```

```xml
ImageView
```

```xml
…
```



```xml

```

```xml
android:elevation
```

```xml
=
```

```xml
"8dp"
```

```xml
/>
```
```



你也可以在代码中通过getElevation()/setElevation()设置。阴影是添加到轮廓上的，默认来自背景。比如，你可以设置一个圆形的drawable作为floating action button的 背景。它会自动转化成适当的阴影。如果你需要手指去控制一个View的阴影，可以设置一个ViewOutlineProvider，它允许你在getOutline()中提供一个自定义的Outline.

**1.2 Cards**

对于创建带有不同信息内容的碎片，Cards是一种常见的模式。supportV7下的CardView可以帮助你方便的实现，并提供了轮廓和阴影。



```
```xml
<
```

```xml
android.support.v7.widget.CardView
```



```xml

```

```xml
android:layout_width
```

```xml
=
```

```xml
"match_parent"
```



```xml

```

```xml
android:layout_height
```

```xml
=
```

```xml
"wrap_content"
```

```xml
>
```



```xml

```

```xml
<!-- Your card content -->
```





```xml
</
```

```xml
android.support.v7.widget.CardView
```

```xml
>
```
```



CardView继承自FrameLayout并提供了默认的elevation 和 圆角，使得cards在不同的平台间保持一致。你也可以通过cardElevation和cardCornerRadius属性自定义它们的值。注意，Cards不是唯一实现维度效果的方式，你应该警惕过度的使用Cards:[http://www.google.com/design/spec/components/cards.html#cards-usage](http://www.google.com/design/spec/components/cards.html#cards-usage)

## 二、Print-like Design 打印式风格

Material利用了经典的打印设计，使内容前部和中心的布局变得整洁和简约。刻意大胆的颜色选择，刻意的空白，美观的印刷风格和强烈的格子线条为你创建一种有层次有意义有焦点的效果。

**2.1 Typography 活字印刷**

Android 5.0更新了Roboto样式，不论多大的text，展示起来都会美观和简洁。添加了一种新的中等高度属性(android:fontFamily=”sans-serif-medium”) 和新的AppAppearance样式为了平衡内容密度和阅读的舒适感,实现了推荐的打印式缩放的。比如你可以简单的通过 android:textAppearance=”@android:style/TextAppearance.Material.Title” 设置  “Title”风格。在旧的版本中可以使用AppCompat
 support library的样式： “@style/TextAppearance.AppCompat.Title”.

**2.2 Color**

调色板为你的应用带来了品牌营销和个性化，通过以下属性可以方便的控制UI的着色：

![](https://img-blog.csdn.net/20141101114107468)

**主调色彩**：主调色彩是用于应用品牌推广的色彩。作为action bar的背景色最近的任务title和其它边缘效果。

**强调色彩**：鲜明的扩展了主调色彩。应用于框架的控制。比如EditText，Switch

**主调的暗色**： Darker作为主调色彩的加深，应用于状态栏 status bar.

更多用于控制色彩的属性，可以参考：colorControlNormal.colorControlActivated.colorControlHighlight, colorButtonNormal, colorSwitchThumbNormal, colorEdgeEffect, statusBarColor and navigationBarColor.

AppCompat 为以上功能提供了大量的子集，允许你为Lollipop之前的系统控制色彩。

**2.3 Dynamic color 动态的色彩**

Material Design 鼓励动态色彩的使用，特别是当你拥有丰富的iamge的时候。新的Palette的support library 支持从图片中抽取一小部分的色彩去设计你的UI。创造出一种更逼真的体验。你也可以添加动静的结合效果，比如前部的text color的变化，如下两种TextView部分色彩的切换：

![](https://img-blog.csdn.net/20141101191330069)![](https://img-blog.csdn.net/20141101191449453)

## 三、Authentic Motion 真实的动效

有型的表面不应该像电影中的跳跃式的切换，它们的移动应该帮助引导用户的焦点，建立控件关系以及保持连贯性。Meterial 响应触摸事件去确认你的交互，所有的变化会从你的Touch点辐射开来。所有的运动都是有意义并且友好的，有助于用户更好的理解。

**3.1 Activity + Fragment切换**

通过声明“shared elements” ，在两种状态之间你可以创建一个平滑的切换，

album_grid.xml





```
```xml
<
```

```xml
ImageView
```



```xml

```

```xml
…
```



```xml

```

```xml
android:transitionName
```

```xml
=
```

```xml
"@string/transition_album_cover"
```

```xml
/>
```
```




album_details.xml






```
```xml
<
```

```xml
ImageView
```



```xml

```

```xml
…
```



```xml

```

```xml
android:transitionName
```

```xml
=
```

```xml
"@string/transition_album_cover"
```

```xml
/>
```
```




AlbumActivity.java






```
```java
Intent intent =
```

```java
new
```

```java
Intent();
```



```java
String transitionName = getString(R.string.transition_album_cover);
```



```java
…
```



```java
ActivityOptionsCompat options = ActivityOptionsCompat.makeSceneTransitionAnimation(activity,
```



```java

```

```java
albumCoverImageView,
```

```java
// The view which starts the transition
```



```java

```

```java
transitionName
```

```java
// The transitionName of the view we’re transitioning to
```



```java

```

```java
);
```



```java
ActivityCompat.startActivity(activity, intent, options.toBundle());
```
```




这里我们在两个不同的屏幕间定义了相同的transitionName 。当你启动一个新的Activity，它的切换将会自动的动画起来。对于进入和退出的元素，你可以自由编排。

**3.2 Ripples（涟漪）**

![](https://img-blog.csdn.net/20141101114717864)

Materials以一种泼墨的涟漪的方式响应用户的触摸事件。你可以通过使用或者继承Theme.Material主题，即可达到这种默认的效果。你可以通过简单的封装将这种ripple效果添加到自己的drawables上。自定义View应该使用  View#drawableHotspotChanged 回调方法把touch的位置传播到它们的drawables上，以便ripple可以知道起始点。





```
```xml
<
```

```xml
ripple
```



```xml

```

```xml
xmlns:android
```

```xml
=
```

```xml
"[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
```



```xml

```

```xml
android:color
```

```xml
=
```

```xml
"@color/accent_dark"
```

```xml
>
```



```xml

```

```xml
<
```

```xml
item
```

```xml
>
```



```xml

```

```xml
<
```

```xml
shape
```



```xml

```

```xml
android:shape
```

```xml
=
```

```xml
"oval"
```

```xml
>
```



```xml

```

```xml
<
```

```xml
solid
```

```xml
android:color
```

```xml
=
```

```xml
"?android:colorAccent"
```

```xml
/>
```



```xml

```

```xml
</
```

```xml
shape
```

```xml
>
```



```xml

```

```xml
</
```

```xml
item
```

```xml
>
```



```xml
</
```

```xml
ripple
```

```xml
>
```
```




**3.3 StateListAnimator**

Materials 也响应了用户抬起手指的操作。类似磁铁吸引的效果。你可以通过tranlationZ属性实现。它类似于elevation属性，但主要是用于时间较短，转瞬即逝的效果，比如 Z = elevation + translationZ。这个新的stateListAnimator 属性允许你在用户touch的时候简单的在translationZ上动画。button默认会有这种效果。

layout/your_layout.xml





```
```xml
<
```

```xml
ImageButton
```

```xml
…
```



```xml

```

```xml
android:stateListAnimator
```

```xml
=
```

```xml
"@anim/raise"
```

```xml
/>
```
```



anim/raise.xml



```
```xml
<
```

```xml
selector
```

```xml
xmlns:android
```

```xml
=
```

```xml
"[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
```

```xml
>
```



```xml

```

```xml
<
```

```xml
item
```

```xml
android:state_enabled
```

```xml
=
```

```xml
"true"
```

```xml
android:state_pressed
```

```xml
=
```

```xml
"true"
```

```xml
>
```



```xml

```

```xml
<
```

```xml
objectAnimator
```



```xml

```

```xml
android:duration
```

```xml
=
```

```xml
"@android:integer/config_shortAnimTime"
```



```xml

```

```xml
android:propertyName
```

```xml
=
```

```xml
"translationZ"
```



```xml

```

```xml
android:valueTo
```

```xml
=
```

```xml
"@dimen/touch_raise"
```



```xml

```

```xml
android:valueType
```

```xml
=
```

```xml
"floatType"
```

```xml
/>
```



```xml

```

```xml
</
```

```xml
item
```

```xml
>
```



```xml

```

```xml
<
```

```xml
item
```

```xml
>
```



```xml

```

```xml
<
```

```xml
objectAnimator
```



```xml

```

```xml
android:duration
```

```xml
=
```

```xml
"@android:integer/config_shortAnimTime"
```



```xml

```

```xml
android:propertyName
```

```xml
=
```

```xml
"translationZ"
```



```xml

```

```xml
android:valueTo
```

```xml
=
```

```xml
"0dp"
```



```xml

```

```xml
android:valueType
```

```xml
=
```

```xml
"floatType"
```

```xml
/>
```



```xml

```

```xml
</
```

```xml
item
```

```xml
>
```



```xml
</
```

```xml
selector
```

```xml
>
```
```




**3.4 Reveal**

material 过渡效果中的一个标志是通过延伸一个圆形的面板来展示新的内容。增强了作为所有转换起点的触摸点, 它是迅速向外扩散的. 你可以使用下面的Animator来实现：



```
```java
Animator reveal = ViewAnimationUtils.createCircularReveal(
```



```java
viewToReveal,
```

```java
// The new View to reveal
```



```java
centerX,
```

```java
// x co-ordinate to start the mask from
```



```java
centerY,
```

```java
// y co-ordinate to start the mask from
```



```java
startRadius,
```

```java
// radius of the starting mask
```



```java
endRadius);
```

```java
// radius of the final mask
```



```java
reveal.start();
```
```



**3.4 Interpolators 插值器**

![](https://img-blog.csdn.net/20141101114835989)

运动应该是慎重的，平滑且精确的。而不是简单的渐入渐出。在 Material Design中，对象更倾向于快速的并舒适的启动。通过下面的示例可以看到，在将要到达终点前的那段位置花费了更少的时间。总的来说，用户不会等待较长时间，运动的负面效果就变得最小化了。有一种新的插值器，可以达到这种效果[https://developer.android.com/reference/android/R.interpolator.html?utm_campaign=L-Developer-launch#fast_out_slow_in](https://developer.android.com/reference/android/R.interpolator.html?utm_campaign=L-Developer-launch#fast_out_slow_in)

对于进入和退出屏幕效果，请分别参考：  linear-out-slow-in 和 fast-out-linear-in interpolators respectively.

## 四、Adaptive Design (and UI Patterns)

material 最终的核心概念是创建一种自适应于不同大小形状设备的设计，从手表到TV。自适应设计技术帮助我们实现了不同的设备但相同的底层系统体现为不同的View。每个View对那个设备来说，都是量身定做的。色彩，图片，层次，空间的相对关系都保持不变。material design 系统提供了便利的组件和模式帮你构建一个如此有扩展性的设计

**Toolbar**

与Actionbar很类似，但是更加方便，不同于标准的Action bar，toolBar就像View层级中的任何一个东西。因此你可以在任何地方进行放置，与其它的View交错，移动，对滑动事件的响应等等。你可以通过调用Activity,setActionBar()方法把ToolBar作为你的Activity的ActionBar.

![](https://img-blog.csdn.net/20141101114912680)

在这个例子中，蓝色的toolbar是一个可扩展高度，覆盖在屏幕内容上侧并且提供了导航按钮。注意：在这个列表和细节中，使用的ToolBar超过了2个。

Material Design 让你的应用具有可理解，美观，逼真的动效及自适应的能力。希望你可以遵从这些设计原则并通过上述提供的new API和一些兼容库帮助你的App实现Material Design。

————————————————————————————————————————————-

**第二篇：Material Design Check List**

主要从细节实现方面提醒你应该如何更好的实现Material Design，标注Signature element为强调部分，标注InCode的为实现方式。

因为用户将很快使用Android5.0系统了，而且也会慢慢习惯Material Design的app，比如Gmail的未来替代者inbox，Google Play Tumblr等，因此Google为了开发者能够更快的支持Material Design的app以及Android 5.0，连续推出了3篇文章。在下面这篇文章中，Google为我们提供了一个表单，这个表单列出了Material Design应该具备的一些风格。你在实现Material Design的时候，可以参照下面的表单。整个表单根据Material
 Design的4个关键方面分为4个部分。下面我们就介绍一下整个表单内容。如果你使用了以下中的几个模块，特别是标注为signature elements的元素，并遵循传统的Android设计最佳实践，那么你已经朝着迷人的Material Design设计迈进了。

Tangible Surfaces 有形的外观

UI由不同层次的表层组成，每层的下面附有阴影。

Signature element: 阴影

阴影用于和前部的元素交互，帮助聚焦和建立层级效果。

In Code:在5.0系统里，这样的阴影效果直接由属性：android:elevation 和 android:translationZ 提供，而在之前的版本，阴影通常由PNG图片资源来实现。

![](https://img-blog.csdn.net/20141101115024128)

**Surfaces and layering.**

阴影和表层被用于连贯和结构性的情景中。每一个阴影标示着一个新的表层，这些表层的构建需要细心和深思熟虑。一个屏幕中通常拥有2到10个表层，避免太多的内嵌层。可滑动的内容滑动到屏幕的边界或者另一个表层后面的时候铸造了一层覆盖在另一个表层上的阴影。永远不要剪裁一个可见边界的元素这样会导致一种没有边界的感觉。换句话说，你应该很少滑动表层上的内容，而是滑动整个表层。

In Code: 在使用ListView和ScrollView的时候，使用 android:clipToPadding=false 来避免边界的剪裁。

表层应该是简单，而且是纯色的背景。

A Bold、Print-Like Aesthetic 一种加粗，打印风格的审美

你所展示的内容来自于一种经典的打印模式，重点在于色彩的使用，有语境的图片和结构性的空白区域。

![](https://img-blog.csdn.net/20141101115254125)

**Primary and accent colors.**

Signature element: 主调色彩和强调色彩

在颜色层面背景和关键的组件上，使用了一种主调色彩和一种强调色彩，比如text和checkbox。这种强调色彩和基调色彩形成了鲜明的对比，比如一个app可以使用一种深蓝色作为基调颜色，而把霓虹色作为强调的颜色。

这种强调的色彩是极其鲜明的，通常用于关键的组件上，引起用户的注意，比如一个floating button，selected tab 或表格区域。

**In code:** 通过在你的主题中设置 android:colorPrimary and android:colorAccent属性（如果使用appCompat，不用再去设置“android”前缀了），AppCompat将会自动的为那些text，checkbox和一些在L之前的组件着色。

**Signature element:透明的状态栏**

在5.0系统上，status bar会被填充匹配app的基调，或者当前屏幕的内容的色彩。对于全屏填充的Image效果，status bar可以是透明的。

可以通过在你的主题中设置android:colorPrimaryDark 或者 android:statusBarColor或者调用Window.setStatusBarColor来实现。

icon/photos/images和其它前部元素的表层会被“墨水”着色。它们不需要阴影也不用使用梯度效果。

颜色可以从图片中抽取用于适配在表层的UI元素。

**InCode:** 通过Palette support library来实现。

**Signature element:** 使用material design的icon

icons 的使用请遵从 system icon guidelines，标准的icons请使用material design的icon，（这个已经在GitHub中开源）

**向导：**http://www.google.com/design/spec/style/icons.html#icons-system-icons

**资源：**http://www.google.com/design/spec/resources/sticker-sheets.html#sticker-sheets-components

照片通常是沉浸并且是全屏的，比如，对于详情的界面，使用一种边缘到边缘的模式，甚至可以出现在app bar或者 status bar的底部。

In code: 新的 Toolbar widget 可以是透明的并且直接放在你的布局中，对于status bar的问题,看以参考在Stack Overflow 上的相关问题：

[http://stackoverflow.com/questions/26440879/how-do-i-use-drawerlayout-to-display-over-the-actionbar-toolbar-and-under-the-st/26440880](http://stackoverflow.com/questions/26440879/how-do-i-use-drawerlayout-to-display-over-the-actionbar-toolbar-and-under-the-st/26440880)

**Signature element:** 适当的尺寸

合适的地方，text内容，app titles,应该对齐3个keylines：

[http://www.google.com/design/spec/layout/metrics-and-keylines.html#metrics-and-keylines-keylines-and-spacing](http://www.google.com/design/spec/layout/metrics-and-keylines.html#metrics-and-keylines-keylines-and-spacing)

在手机上，那些 keylines 距离左边是16dp和72dp，距离屏幕右边缘是16dp。在平板上是24dp和80dp。

UI元素的对齐方式和大小应该根据一个8dp的网格。比如，app bar在手机上为56dp，在平板上是64dp。Padding和Margins可以使用像：8dp,16dp,24dp的值等等，更精确的text位置是使用一个4dp的网格

![](https://img-blog.csdn.net/20141101115351141)

**Keylines**

**Authentic Motion 真实的动效**

在不同的app上下文和状态下，动效可以提供一种可视化和连贯的体验。通过小规模的使用切换动画可以增加一些情趣。而不是为了动效而动效。总的来说，UI和内容元素不会简单的消失或者出现，它们会以动画的方式移动入到一个单元或者单独的出现。

**Signature element: “hero”切换动画**

当你按下一个条目去看详情的时候。这里有个叫“hero”的切换动画，它会移动并且缩放它所在的位置和详情屏幕之间的条目。

InCode：在SDK中它被叫做 “shared element transitions” ，support版本的FragmentTransaction也支持了shared element support。

![](https://img-blog.csdn.net/20141101115533149)
**“Hero” Transiton**

Signature element: Ripple effects

Ripple effects 为你按下条目的增加了反馈效果。

InCode：默认的 android:selectableItemBackground 和 android:selectableItemBackgroundBorderless 拥有这种效果，或者你可以使用RippleDrawable (<ripple>) 去自定义效果。在5.0之前的设备上，ripples没有这种预期的效果，因此，遵从默认的android:selectableItemBackground 行为。

Signature element:  circular “reveal” animation.

UI元素可以通过一个圆形的”reveal”动画出现。

InCode：查看文档或者ViewAnimationUtils类。

Signature element:Vector动画图标

在更微妙更赏心悦目的情况下使用动画，比如去转换icon 的状态和text的状态。比如，一个“+”图标可以变形为一个“x”标识，或者一个概述的图标可以变成一个栅栏效果。

InCode：icon的变化可以通过AnimatedStateListDrawable 和相应的XML文件来实现，你可以在Google I/O app源码中找到。它支持了动画的vector图标。

动画和切换通常在300ms内。

交叉淡入淡出通常被平移和滑动切换代替：竖直的滑动产生的底部导航，水平滑动产生的侧面导航。对于滑动的转换，加速和微小的减速要好于简单的线性移动。

可以参考：[http://www.google.com/design/spec/animation/authentic-motion.html#](http://www.google.com/design/spec/animation/authentic-motion.html#)

**Adaptive Design（UI Patterns）**

有型的外表，大胆的图形设计，加上有意义的动画效果带来了一种连贯跨设备的体验，无论在手机，平板，笔记本，TV，可穿戴设备，汽车上，等都有同样的体验效果。另外，下面的关键的UI 设计模式有助于在不同设备之间建立一种连贯的体验。

app使用了响应式设计，确保了在不同尺寸的屏幕和不同的方向上能够适当的布局。可以参考平板App样式的清单表去优化那些面向平板的App.[http://developer.android.com/distribute/essentials/quality/tablets.html](http://developer.android.com/distribute/essentials/quality/tablets.html%29)

在material design中，详情界面的切换通常都是“Hero”动画的。在多面板布局中，app可以使用多个toobar去代替一些下一步的相关操作。

Signature element: FAB

如果合适的话，App推荐floating action button作为一个屏幕上的关键操作。FAB是一个圆形外表，附带有阴影，表面色彩采用亮色，强调色。它可以用于执行一些主要的操作，比如，发送，撰写，创建，添加，或者搜索，大约8dp的elevation。通常悬浮在屏幕的右下角，或者边界的中心位置。

![](https://img-blog.csdn.net/20141101115728968)

The floating action button

**App Bar**

Signature element:

App使用了标准的Android App bar，这个App Bar没有使用app图标。颜色和印刷效果替代了品牌的推广。这个App Bar带有阴影，或者拥有一个带有阴影的表层。通常App Bar 4dp elevation。

InCode：在5.0中使用新的ToolBar Widget引入到View层级视图中。AppCompat提供了兼容的android.support.v7.widget.Toolbar来实现它。

app bar可能是标准高度的2到3倍，在滑动的时候，app bar可以恢复到正常高度。

app bar在有些情况下可以完全透明，可以在一个图片之上附有text和操作，比如Google Play Newsstand 应用。[http://www.google.com/design/spec/style/imagery.html#imagery-principles](http://www.google.com/design/spec/style/imagery.html#imagery-principles)

App bar title 应该和第二条keyline对齐。

InCode：在使用Toobar widge的时候，请使用 android:contentInsetStart属性。

在适当的时候，往下滑的时候，app bar可以滑出屏幕，为内容留下更多垂直方向的空间。往上滑动的时候，app bar可以恢复显示。（这个式样：quickReturn）

**Tabs**

Signature element:Tabs应该遵从最新的 material design 设计风格。在垂直方向没有分割线，如果app使用了top-level的话，tab可以作为app bar的一部分。[http://www.google.com/design/spec/components/tabs.html#tabs-usage](http://www.google.com/design/spec/components/tabs.html#tabs-usage)

InCode: 可以参考在SDK中的示例 SlidingTabsBasic 或者Google I/O app source（My Schedule 部分）另外加上 前两天分享的 SlidingTabStrip

**Tabs应该支持手势滑动**

InCode: 所有的Tabs应该使用support lib中的ViewPager 控件

在选中Tabs时，应该有一个前部颜色的变化或者使用强调色彩的颜色条。在tabs滑动的时候，这个线条应该平滑的响应。

![](https://img-blog.csdn.net/20141101115903609)

**抽屉导航**

Signature element:

如果你的app使用了导航的Drawer，它也应该遵从最新的material design风格。drawer应该出现在app bar上面，也应该以半透明的方式出现在status bar后面。

InCode：使用support lib 中的DrawerLayout控件和上述谈到的ToolBar 控件实现抽屉效果。在stack over flow中有相关的问题参考：

[http://stackoverflow.com/questions/26440879/how-do-i-use-drawerlayout-to-display-over-the-actionbar-toolbar-and-under-the-st/26440880](http://stackoverflow.com/questions/26440879/how-do-i-use-drawerlayout-to-display-over-the-actionbar-toolbar-and-under-the-st/26440880)

Signature element:左上角图标

在app bar最左边的icon是一个抽屉的指示器。http://www.google.com/design/spec/layout/structure.html#structure-app-bar

app icon 在app bar中是不可见的。可选择的，在早期版本中，如果app拥有一个抽屉，那么最左边的app icon会保留并且会变窄，比如在4.0中。

标准的抽屉：在手机上不要宽于320dp，在平板上不要宽于400dp,但是不要小于屏幕宽度 – 标准的toolbar高度，比如在Nexus5中：360dp – 56dp = 304dp

每一个抽屉条目的应该沿着grid网格，所有的行高为 48dp，上下8dp边距。

Text 和 icon 应该沿着主线

![](https://img-blog.csdn.net/20141101115945093)

越来越多的App来自google play生态系统将会更新到Material Design，预期2014冬季对于Android的设计来说是一个大的变更期。更多关于Material design设计资源，可以参考YouTube视频：[https://www.youtube.com/playlist?list=PLOU2XLYxmsIJFcNKpAV9B_aQmz2h68fw_](https://www.youtube.com/playlist?list=PLOU2XLYxmsIJFcNKpAV9B_aQmz2h68fw_)

更多的开发者资源，可以参考文档：[http://developer.android.com/training/material/index.html](http://developer.android.com/training/material/index.html)

以上全是理论知识，一个Material Design的App，应该遵循哪些标准，如果你已经了解了这些，那么接下来就可以去实现它们了，很多拓荒者为我们提供了很多开源库，大家可以直接用，但有些是向前兼容的，有的是4.0以上的，大家使用的时候注意下。

**Material Design开发资源**

**官方：**

Material Design介绍：[http://www.google.com/design/spec/material-design/introduction.html](http://www.google.com/design/spec/material-design/introduction.html)

设计资源：[https://github.com/google/material-design-icons](https://github.com/google/material-design-icons)

App:Google  2014: [https://github.com/google/iosched](https://github.com/google/iosched) （下载地址：[http://vdisk.weibo.com/s/av06eFs1kWSeE](http://vdisk.weibo.com/s/av06eFs1kWSeE) ,如果需要梯子去请求数据，文件夹下的另一个apk会帮到你）

**视频教程:**

[https://www.youtube.com/watch?v=97SWYiRtF0Y&feature=youtu.be](https://www.youtube.com/watch?v=97SWYiRtF0Y&feature=youtu.be)

[https://www.youtube.com/watch?v=S3H7nJ4QaD8](https://www.youtube.com/watch?v=S3H7nJ4QaD8)

示例集合

[https://github.com/rahulparsani/google-io-2014-compat](https://github.com/rahulparsani/google-io-2014-compat)

![](https://img-blog.csdn.net/20141031174001295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141031173911125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141031174045849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[https://github.com/romainguy/google-io-2014](https://github.com/romainguy/google-io-2014)

![](https://img-blog.csdn.net/20141101092355640)![](https://img-blog.csdn.net/20141101092312594)

[https://github.com/dexafree/MaterialList](https://github.com/dexafree/MaterialList)

![](https://img-blog.csdn.net/20141101111406848)![](https://img-blog.csdn.net/20141101111622600)

https://github.com/gabrielemariotti/cardslib

![](https://img-blog.csdn.net/20141101111954464)

**FAB**

[https://github.com/FaizMalkani/FloatingActionButton](https://github.com/FaizMalkani/FloatingActionButton)（无图）

[https://github.com/futuresimple/android-floating-action-button](https://github.com/futuresimple/android-floating-action-button)

![](https://img-blog.csdn.net/20141031175741052)![](https://img-blog.csdn.net/20141031175900250)

[https://github.com/telly/FloatingAction](https://github.com/telly/FloatingAction)

![](https://img-blog.csdn.net/20141031175941780)

[https://github.com/makovkastar/FloatingActionButton](https://github.com/makovkastar/FloatingActionButton)

![](https://img-blog.csdn.net/20141031173745856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[https://github.com/flavienlaurent/tickplusdrawable](https://github.com/flavienlaurent/tickplusdrawable)

![](https://img-blog.csdn.net/20141031181536923)

**Menu & Drawer**

[https://github.com/markushi/android-ui](https://github.com/markushi/android-ui)

![](https://img-blog.csdn.net/20141101082913420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

https://github.com/balysv/material-menu

![](https://img-blog.csdn.net/20141031180335218)

![](https://img-blog.csdn.net/20141031180256125)

[https://github.com/saulmm/Android-Material-Example](https://github.com/saulmm/Android-Material-Example)

![](https://img-blog.csdn.net/20141031181741677)![](https://img-blog.csdn.net/20141031180731185)

https://github.com/ikimuhendis/LDrawer

![](https://img-blog.csdn.net/20141031181405906)

**Ripple**

[https://github.com/markushi/android-ui](https://github.com/markushi/android-ui)

![](https://img-blog.csdn.net/20141101093222308)

[https://github.com/traex/RippleEffect](https://github.com/traex/RippleEffect)

![](https://img-blog.csdn.net/20141031174314080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[https://github.com/siriscac/RippleView](https://github.com/siriscac/RippleView)

![](https://img-blog.csdn.net/20141031174731527)

**Transition**

[https://github.com/saulmm/android_L_preview_example](https://github.com/saulmm/android_L_preview_example)(图太大)

[https://github.com/andkulikov/transitions-everywhere](https://github.com/andkulikov/transitions-everywhere)

![](https://img-blog.csdn.net/20141101111313106)

**Vector**

[https://github.com/chiuki/animated-vector-drawable](https://github.com/chiuki/animated-vector-drawable)

![](https://img-blog.csdn.net/20141101112245814)![](https://img-blog.csdn.net/20141101112406750)

**Dialog**

[https://github.com/drakeet/MaterialDialog](https://github.com/drakeet/MaterialDialog)

![](https://img-blog.csdn.net/20141031174559472)

[https://github.com/r0adkll/PostOffice](https://github.com/r0adkll/PostOffice)

![](https://img-blog.csdn.net/20141101090028184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141101090032443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141101090037513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVzaHVhaWM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[https://github.com/lewisjdeane/L-Dialogs](https://github.com/lewisjdeane/L-Dialogs)

![](https://img-blog.csdn.net/20141031174950203)

为了能够正常的学习，浏览官方文档，接触最新技术讯息，你需要一个梯子，这里介绍一个比较好的，本人用的就是这个挺不错：[云梯](http://refyt.com/?r=e6409c49188be579)








转载自：[Android开发中文站](http://www.androidchina.net) »
[进入Material Design时代](http://www.androidchina.net/1381.html)



