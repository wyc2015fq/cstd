# Android应用资源---状态颜色列表（Color State List） - 一世豁然的专栏 - CSDN博客





2017年04月24日 20:47:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：899








本博客转载于：[http://blog.csdn.net/think_soft/article/details/7476019](http://blog.csdn.net/think_soft/article/details/7476019)






ColorStateList是能够把申请的颜色定义在XML中的一个对象，但是它实际上会根据使用这个ColorStateList对象的View对象的状态，来改变颜色。例如，Button控件存在几种不同的状态之一（按下、聚焦、或者都不是），并且它要使用一个颜色状态列表，在每个状态期间，可以提供不同的颜色。

可以在一个XML文件中描述这个状态列表。每个颜色被定义在一个<selector>元素内部的<item>元素中。每个<item>元素使用各种属性来描述它应该用于那个状态中。

每个状态改变期间，会从上到下遍历状态列表，第一个被找到的与当前状态匹配的颜色会被使用，这个选择不是基于最佳匹配，而是简单选择遇到的第一个满足条件的项目。

注意：如果要提供静态颜色资源，请使用简单的Color值。

**文件位置（FILE LOCATION）：**

Res/color/filename.xml，文件名被做资源的ID。

**被编译的资源数据类型（COMPLILED RESOURCE DATATYPE）：**

资源指向一个ColorStateList对象。

**资源引用（RESOURCE REFERENCE）：**

在[Java](http://lib.csdn.net/base/javase)代码中：R.color.filename

在XML中：@[package:]color/filename

**语法（SYNTAX）：**


<?xml version="1.0" encoding="utf-8"?>
<[selector](http://developer.android.com/guide/topics/resources/color-list-resource.html#selector-element)xmlns:[Android](http://lib.csdn.net/base/android)="http://schemas.android.com/apk/res/android">
<[item](http://developer.android.com/guide/topics/resources/color-list-resource.html#item-element)
android:color="*hex_color*"
android:state_pressed=["true" | "false"]
android:state_focused=["true" | "false"]
android:state_selected=["true" | "false"]
android:state_checkable=["true" | "false"]
android:state_checked=["true" | "false"]
android:state_enabled=["true" | "false"]
android:state_window_focused=["true" | "false"] />
</selector>




**元素（ELEMENTS）：**

**<selector>**

必须的，它必须是根元素。包含一个或多个<item>元素。

**属性（ATTRIBUTES）：**

xmlns:android：

字符串，必须的。它定义了XML的命名空间，必须是“http://schemas.android.com/apk/res/android”。

**<item>**

定义某个状态期间使用的颜色，颜色会做为这个元素的一个属性。它必须是<selector>元素的子元素。

**属性（ATTRIBUTES）：**

**android:color**

十六进制颜色，必须的。颜色是用RGB值来指定的，并且可选择alpha通道。

这个值始终是用#字符开头，后面跟的是Appha-Red-Green-Blue信息，格式如下：

        #RGB

        #ARGB

        #RRGGBB

        #AARRGGBB

**android:state_pressed**

一个布尔值，如果这个项目是在对象被按下时使用，那么就要设置为true。（如，按钮被触摸或点击时。）false应该用于默认的非按下状态。

**android:state_focused**

一个布尔值，如果这个项目是在对象获取焦点时使用，那么就要设置为true。如，一个选项标签被打开时。如果这个项目要用于对象没有被被选择的时候，那么就要设置为false。

**android:state_checkable**

一个布尔值，如果这个项目要用于对象的可选择状态，那么就要设置为true。如果这个项目要用于不可选状态，那么就要设置为false。（它只用于一个对象在可选和不可选之间的转换）。

**android:state_checked**

一个布尔值，如果这个项目要用于对象被勾选的时候，那么就要设置为true。否者设为false。

**android:state_enabled**

一个布尔值，如果这个项目要用于对象可用状态（接受触摸或点击事件的能力），那么就要设置为true，否者设置为false。

**android:state_window_focused**

一个布尔值，如果这个项目要用于应用程序窗口的有焦点状态（应用程序是在前台），那么就要设置为true，否者设置false。



**注意：要记住，状态列表中一个与对象当前状态匹配的项目会被使用。因此，如果列表中的第一项没有包含以上任何一种状态属性，那么每次都会使用这个项目，因此默认设置应该始终被放到最后。**

例子（EXAMPLE）：

XML文件被保存在res/color/button_text.xml中：

<?xml version="1.0" encoding="utf-8"?>
<selectorxmlns:android="http://schemas.android.com/apk/res/android">
<itemandroid:state_pressed="true"
android:color="#ffff0000"/><!--
 pressed -->
<itemandroid:state_focused="true"
android:color="#ff0000ff"/><!--
 focused -->
<itemandroid:color="#ff000000"/><!--
 default -->
</selector>

布局XML会把这个颜色列表应用与一个View对象：


<Button
android:layout_width="fill_parent"
android:layout_height="wrap_content"
android:text="@string/button_text"
android:textColor="@color/button_text"/>





