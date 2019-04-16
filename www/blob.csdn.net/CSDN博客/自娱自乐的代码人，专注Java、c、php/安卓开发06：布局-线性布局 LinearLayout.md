# 安卓开发06：布局-线性布局 LinearLayout - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月24日 13:56:40[initphp](https://me.csdn.net/initphp)阅读数：1541
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









LinearLayout把视图组织成一行或一列。子视图能被安排成垂直的或水平的。线性布局是非常常用的一种布局方式。

请看一个布局例子：



```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >

    <!-- 大的框架，横着布局 -->

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="horizontal" >

        <!-- 线性布局-竖着布局 -->

        <LinearLayout
            android:layout_width="100dp"
            android:layout_height="fill_parent"
            android:orientation="vertical" >

            <Button
                android:id="@+id/button1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Button1" />

            <Button
                android:id="@+id/button1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Button2" />
        </LinearLayout>

        <!-- 线性布局-横着着布局 -->
        <LinearLayout
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:orientation="horizontal" >

            <Button
                android:id="@+id/button1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Button3" />

            <Button
                android:id="@+id/button1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Button4" />
        </LinearLayout>
    </LinearLayout>

</LinearLayout>
```


效果图：



![](https://img-blog.csdn.net/20130624142531156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




线性布局框架的一个属性表：


|属性|描述|
|----|----|
|layout_width|指定View或ViewGroup的宽度|
|layout_height|指定View或ViewGroup的高度|
|layout_marginTop|指定View或ViewGroup上方的额外空间|
|layout_marginBottom|指定View或ViewGroup下方的额外空间|
|layout_marginLeft|指定View或ViewGroup左侧的额外空间|
|layout_marginRight|指定View或ViewGroup右侧的额外空间|
|layout_gravity|指定View或ViewGroup中的子视图的排列位置|
|layout_weight|指定指派给View或ViewGroup的额外空间尺寸|
|layout_x|指定View或ViewGroup的x坐标|
|layout_y|指定View或ViewGroup的y坐标|




可以看到layout_width和layout_hight中经常有**fill_parent**，**wrap_content**，**match_parent**来区分宽度和高度。这三者什么区别呢？

**fill_parent**：

设置一个构件的布局为fill_parent将强制性地使构件扩展，以填充布局单元内尽可能多的空间。这跟Windows控件的dockstyle属性大体一致。设置一个顶部布局或控件为fill_parent将强制性让它布满整个屏幕。

**wrap_content**：

设置一个视图的尺寸为wrap_content将强制性地使视图扩展以显示全部内容。以TextView和ImageView控件为例，设置为wrap_content将完整显示其内部的文本和图像。布局元素将根据内容更改大小。设置一个视图的尺寸为wrap_content大体等同于设置Windows控件的Autosize属性为True。

**match_parent**：

Android2.2中match_parent和fill_parent是一个意思 .两个参数意思一样，match_parent更贴切，于是从2.2开始两个词都可以用。那么如果考虑低版本的使用情况你就需要用fill_parent了

**视图和视图组概念：**

视图：就是显示在屏幕上的一个组件（Widget）。View的例子：按钮（Button）、标签（TextView）和文本框（EditText）。每个“视图”（View）都继承自基类android.view.View。

视图组：可以包含一个或多个View。ViewGroup本身就是一种特殊的View，它提供了一个布局，可以使用这个布局去组织一系列的View视图。LinearLayout和FrameLayout。每个“ViewGroup”都继承自基类android.view.ViewGroup




