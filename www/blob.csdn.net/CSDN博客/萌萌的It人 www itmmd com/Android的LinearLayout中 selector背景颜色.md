
# Android的LinearLayout中 selector背景颜色 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月16日 15:09:09[Jlins](https://me.csdn.net/dyllove98)阅读数：8475


把linearLayout 当成按钮使用，我想通过时间触发动态的改变  linear layout 的背景颜色，通过不同的颜色展示不同的状态, 我想通过selector来实现，但是完全没有效果
我看了下其他的解决方法，都说需要添加点击属性时间，但是我的代码已经添加了
我的 LinearLayout  包含了2个小的 LinearLayout  ，没个小LinearLayout  又包含了9个TextView，这18个TextVIew把我的布局填充满了.
我的想法是，每个TextVIew被点击的时候改变 LinearLayout  的背景颜色。
我把所有的Textview都添加了 点击事件，但是还是不起效果。
selector jbbtn.xml
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|selector|xmlns:android|=|"http://schemas.android.com/apk/res/android"|>
|<|item|android:state_enabled|=|"true"|android:state_pressed|=|"true"
|android:drawable|=|"@drawable/jbbtn_pressed"|/>
|<|item|android:state_enabled|=|"true"
|android:drawable|=|"@drawable/jbstyle_transparent"|/>
|<|item|android:state_enabled|=|"false"|android:drawable|=|"@drawable/jbbtn_disabled"|/>
|</|selector|>
|My LinearLayout
|<|LinearLayout
|android:id|=|"@+id/llCurrents"
|android:background|=|"@drawable/jbbtn"
|android:layout_width|=|"wrap_content"
|android:layout_height|=|"match_parent"
|android:layout_alignParentBottom|=|"true"
|android:layout_alignParentLeft|=|"true"
|android:layout_alignTop|=|"@+id/llTimer"
|android:layout_below|=|"@id/btnMenu"
|android:layout_marginRight|=|"3sp"
|android:clickable|=|"true"
|android:focusable|=|"true"
|android:orientation|=|"horizontal"
|android:padding|=|"3sp"|>
|~~~~~~

|</|LinearLayout|>
|


### 处理方法
我也是用线性布局当做一个按钮，我没有绑定任何点击事件，它就能够工作，我为标准的按钮建立了一个样式，我只是不停的分配样式到按钮
把 linearlayout 当成一个 button:
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|<|LinearLayout
|style|=|"@style/btn_stand"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"wrap_content"
|android:onClick|=|"onClickLLButton"
|android:orientation|=|"vertical"|>
|<|TextView
|android:layout_width|=|"match_parent"
|android:layout_height|=|"wrap_content"
|android:gravity|=|"center"
|android:text|=|"Button Label"|/>
|<|TextView
|android:id|=|"@+id/tvLLButton"
|android:layout_height|=|"0px"
|android:layout_weight|=|"1"
|android:gravity|=|"center"
|android:text|=|"Button Info"|/>
|</|LinearLayout|>
|My style definition for the button:
|<|style|name|=|"btn_stand"|parent|=|"AppBaseTheme"|>
|<|item|name|=|"android:background"|>@drawable/btn_stand_sel</|item|>
|<|item|name|=|"android:textColor"|>@drawable/btn_stand_text_color</|item|>
|<|item|name|=|"android:minHeight"|>48dp</|item|>
|<|item|name|=|"android:paddingLeft"|>5dp</|item|>
|<|item|name|=|"android:paddingRight"|>5dp</|item|>
|</|style|>
|

我的@drawable/btn_stan_sel 文件:
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|selector|xmlns:android|=|"http://schemas.android.com/apk/res/android"|>
|<!-- disabled state -->
|<|item|android:drawable|=|"@drawable/btn_stand_disabled"|android:state_enabled|=|"false"|/>
|<!-- enabled and pressed state -->
|<|item|android:drawable|=|"@drawable/btn_stand_pressed"|android:state_enabled|=|"true"|android:state_pressed|=|"true"|/>
|<!-- enabled and focused state -->
|<|item|android:drawable|=|"@drawable/btn_stand_focused"|android:state_enabled|=|"true"|android:state_focused|=|"true"|/>
|<!-- enabled state -->
|<|item|android:drawable|=|"@drawable/btn_stand_enabled"|android:state_enabled|=|"true"|/>
|</|selector|>
|

我的绘制文件为重复的为每一个状态绘制不同的颜色
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|shape|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|android:shape|=|"rectangle"|>
|<|stroke
|android:width|=|"1dp"
|android:color|=|"@color/stroke"|/>
|<|solid|android:color|=|"@color/blue"|/>
|<|corners|android:radius|=|"6dp"|/>
|</|shape|>
|


原文地址：[http://www.itmmd.com/201411/186.html ](http://www.itmmd.com/201411/186.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

