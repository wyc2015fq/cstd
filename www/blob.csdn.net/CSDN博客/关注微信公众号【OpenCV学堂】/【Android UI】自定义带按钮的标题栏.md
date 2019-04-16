# 【Android UI】自定义带按钮的标题栏 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年10月23日 08:56:36[gloomyfish](https://me.csdn.net/jia20003)阅读数：39966








自定义标题栏在很多的android app中很常见，可以说是一种很有用的UI设计方法。自

己也本着学习的态度，经过一番各种坑，终于实现了，现总结如下：

一：大致流程

1.      对指定的android activity设置自定义主题风格，其中自定义主题风格是关键

在android 4.0以上版本中如果使用Theme.Holo或者Theme.Light等，程序会

一直报错误-**you cannot combine custom title with other feature titles**

2.      在对应的Activity中加入代码



```java
super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE,R.layout.mycustomtitle);
```



3.      在styles.xml使用如下的自定义主题，发现只有使用这个默认主题才不出第一步的

错误，真是各种坑啊！



```
<resources>

    <style name="WindowTitleBackground" >   
        <item name="android:background">@color/blue</item>       
    </style>
    <style name="MyTheme" parent="android:Theme">
        <item name="android:windowTitleSize">60dp</item>
        <item name="android:windowTitleBackgroundStyle">@style/WindowTitleBackground</item>                
    </style>

</resources>
```
4.      关键技巧，使用RelativeLayout来对齐自定义Title的组件


二：测试MainActivity源代码



```java
package com.gloomyfish.titledemo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE,R.layout.mycustomtitle);
    }
    
}
```

三：XML资源文件



mycustomtitle.xml的内容



```java
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_gravity="fill_horizontal"
    android:orientation="horizontal"
    android:layout_height="fill_parent" >
    <Button  android:id="@+id/header_left_btn"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_marginLeft="5dp"
        android:layout_centerVertical="true"
        android:text="返回"
        android:textColor="#000000"/>
    
        <TextView android:id="@+id/header_text"
            android:layout_width="fill_parent"
            android:layout_height="fill_parent"
            android:layout_toRightOf="@+id/header_left_btn"
            android:layout_toLeftOf="@+id/header_right_btn"
            android:text="My Title Bar"
            android:textSize="20sp"
            android:textStyle="bold"
            android:textColor="#FFFFFF"
            android:gravity="center"
            android:layout_centerHorizontal="true"
            android:layout_centerVertical="true"
            android:singleLine="true" />
           
        <Button  android:id="@+id/header_right_btn"
            android:layout_width="wrap_content"
             android:layout_height="wrap_content"
            android:layout_alignParentRight="true"
            android:layout_marginRight="5dp"
             android:layout_centerVertical="true"
             android:text="图片"  
             android:textColor="#000000"/>

</RelativeLayout>
```

最后别忘记在androi的manifest配置文件中加上自定义的主题




`android:theme="@style/MyTheme" `
同时还要删除IDE默认生成的那些appTheme，不然也会一直报错！



最终效果如下：

![](https://img-blog.csdn.net/20131023085132015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


实现这个自定义标题栏的时候，看到stackoverflow上面说

如果使用Theme.Holo一定要换成Theme.Holo.NoActionBar主题

可以我换了以后发现，还是各种坑，一直不出效果。

所以我推荐一定要使用**android:theme**



