
# Android 5.0 android:elevation 如果给button设置效果 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月16日 15:01:19[Jlins](https://me.csdn.net/dyllove98)阅读数：24125


看到android5.0版本中提供了  elevation  的功能，我在SDK Manager 下载了 Android 5.0  例子代码，看了下  ElevationBasic 例子，展示了2个view对象，一个圆形一个矩形，圆形设置了 android:elevation  =  30dp，具体例子如下
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
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<!--
|Copyright 2014 The Android Open Source Project

|Licensed under the Apache License, Version 2.0 (the "License");
|you may not use this file except in compliance with the License.
|You may obtain a copy of the License at

|http://www.apache.org/licenses/LICENSE-2.0

|Unless required by applicable law or agreed to in writing, software
|distributed under the License is distributed on an "AS IS" BASIS,
|WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
|See the License for the specific language governing permissions and
|limitations under the License.
|-
|-->
|<|FrameLayout|xmlns:android|=|"http://schemas.android.com/apk/res/android"
|xmlns:tools|=|"http://schemas.android.com/tools"
|android:layout_width|=|"match_parent"
|android:layout_height|=|"match_parent"|>
|<|View
|android:id|=|"@+id/floating_shape"
|android:layout_width|=|"80dp"
|android:layout_height|=|"80dp"
|android:layout_marginRight|=|"40dp"
|android:background|=|"@drawable/shape"
|android:elevation|=|"30dp"
|android:layout_gravity|=|"center"|/>
|<|View
|android:id|=|"@+id/floating_shape_2"
|android:layout_width|=|"80dp"
|android:layout_height|=|"80dp"
|android:layout_marginLeft|=|"25dp"
|android:background|=|"@drawable/shape2"
|android:layout_gravity|=|"center"|/>
|</|FrameLayout|>
|


在 Nexus 9 中跑这里例子，得到的效果如下
如果我把 组件换成按钮Button，移除掉按钮的其他属性，就不会显示 elevation  效果，如下
默认主题的按钮样式中包含了  android:elevation 和 android:translationZ 属性，你可以移除掉默认的属性动画，并设置你自己的android:stateListAnimator 属性
如下代码
|1
|2
|3
|4
|5
|6
|7
|8
|9
|<|Button
|...
|android:stateListAnimator|=|"@null"|/>
|<|Button
|...
|android:stateListAnimator|=|"@anim/my_animator"|/>
|

默认的动画（animator）在button_state_list_anim_material.xml定义。这里是一个展示状态下不可用，按钮处于点击状态的例子
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
|31
|<?|xml|version|=|"1.0"|encoding|=|"utf-8"|?>
|<|selector|xmlns:android|=|"http://schemas.android.com/apk/res/android"|>
|<|item|android:state_pressed|=|"true"|android:state_enabled|=|"true"|>
|<|set|>
|<|objectAnimator|android:propertyName|=|"translationZ"
|android:duration|=|"@integer/button_pressed_animation_duration"
|android:valueTo|=|"@dimen/button_pressed_z_material"
|android:valueType|=|"floatType"|/>
|<|objectAnimator|android:propertyName|=|"elevation"
|android:duration|=|"0"
|android:valueTo|=|"@dimen/button_elevation_material"
|android:valueType|=|"floatType"|/>
|</|set|>
|</|item|>
|<!-- 基本状态 -->
|<|item|android:state_enabled|=|"true"|>
|<|set|>
|<|objectAnimator|android:propertyName|=|"translationZ"
|android:duration|=|"@integer/button_pressed_animation_duration"
|android:valueTo|=|"0"
|android:startDelay|=|"@integer/button_pressed_animation_delay"
|android:valueType|=|"floatType"|/>
|<|objectAnimator|android:propertyName|=|"elevation"
|android:duration|=|"0"
|android:valueTo|=|"@dimen/button_elevation_material"
|android:valueType|=|"floatType"|/>
|</|set|>
|</|item|>
|...
|</|selector|>

|

原文地址：[http://www.itmmd.com/201412/240.html](http://www.itmmd.com/201412/240.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。


