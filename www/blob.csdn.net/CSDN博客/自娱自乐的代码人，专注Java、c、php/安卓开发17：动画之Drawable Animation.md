# 安卓开发17：动画之Drawable Animation - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月09日 14:56:38[initphp](https://me.csdn.net/initphp)阅读数：1310
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









Drawable Animation:帧动画，说白了和GIF图片差不多。通过Drawable Animation就可以来模拟一些动画效果了。

首先我们在res文件夹下创建 anim 文件夹，主要放置动画效果的xml文件。并且创建一个test_anim.xml的文件：（需要有图片文件）




```
<?xml version="1.0" encoding="utf-8"?>
<!-- android:oneshot 如果为false 则会循环播放，否则就是一次 -->
<animation-list xmlns:android="http://schemas.android.com/apk/res/android"
    android:oneshot="true" >

    <!-- android:duration设置播放的间隔事件，这里是100毫秒 -->
    <item
        android:drawable="@drawable/jhs_button1_h1"
        android:duration="100"/>
    <item
        android:drawable="@drawable/jhs_button1_h2"
        android:duration="100"/>
    <item
        android:drawable="@drawable/jhs_button1_h3"
        android:duration="100"/>
    <item
        android:drawable="@drawable/jhs_button1_h4"
        android:duration="100"/>
    <item
        android:drawable="@drawable/jhs_button1_h5"
        android:duration="100"/>

</animation-list>
```



在activity_main.xml中，创建一个ImageView，主要动画就是在这个控件上面：





```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <!-- 下拉列表控件 -->

    <ImageView
        android:id="@+id/imageView1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentTop="true"
        android:layout_marginLeft="70dp"
        android:layout_marginTop="138dp"
        android:src="@drawable/jhs_button1_h" />

</RelativeLayout>
```


然后创建Main
```java
package com.example.test.com;


import android.app.Activity;
import android.graphics.drawable.AnimationDrawable;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        
        ImageView imageView = (ImageView) findViewById(R.id.imageView1);
        //设置背景图
        imageView.setBackgroundResource(R.anim.test_anim);
        //创建AnimationDrawable对象
        final AnimationDrawable anim = (AnimationDrawable) imageView.getBackground();
        //ImageView 设置点击事件的时候 触发动画
        imageView.setOnClickListener(new OnClickListener() {
            
            @Override
            public void onClick(View v) {
                anim.stop(); //首先需要将动画停止
                anim.start(); //然后开始动画
                
            }
        });
    }

}
```


效果：




![](https://img-blog.csdn.net/20130709145608234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







