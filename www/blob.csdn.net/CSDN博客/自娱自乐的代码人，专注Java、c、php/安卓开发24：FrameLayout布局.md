# 安卓开发24：FrameLayout布局 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年08月23日 14:32:37[initphp](https://me.csdn.net/initphp)阅读数：3028
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









### FrameLayout布局

FrameLayout是五大布局中最简单的一个布局。FrameLayout布局中的元素会根据先后顺序重叠起来。利用FrameLayout布局元素重叠的特性，我们一般可以做一些层的隐藏和显示，以及在一个图片上放置另外一个小图标等这样的功能。




看代码：



```
<?xml version="1.0" encoding="utf-8"?>
<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >

    <!-- 底图 -->

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical" >

        <ImageView
            android:id="@+id/imageView1"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:gravity="center_horizontal"
            android:src="@drawable/test1" />
    </LinearLayout>

    <!-- 这个视图会覆盖在上面那个图的上方 -->

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical" >

        <ImageView
            android:id="@+id/imageView2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_marginLeft="100dp"
            android:layout_marginTop="100dp"
            android:src="@drawable/ic_launcher" />

    </LinearLayout>

    <!-- 不可见视图 -->
    <ImageView
        android:id="@+id/imageView3"
        android:visibility="gone"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:src="@drawable/jhs_button1_h4" />

</FrameLayout>
```


效果：第一张图在最底下，第二张图在上面，第三张图被gone隐藏了。

![](https://img-blog.csdn.net/20130823143156359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




