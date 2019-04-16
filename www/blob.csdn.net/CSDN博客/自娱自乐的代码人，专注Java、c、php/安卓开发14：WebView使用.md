# 安卓开发14：WebView使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月03日 17:36:55[initphp](https://me.csdn.net/initphp)阅读数：2742
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









WebView就是在安卓中嵌套网页。很实用，也非常简单。

直接上代码：

首先，使用WebView，需要对网络有访问权限，安卓的**AndroidManifest.xml**加入：


`<uses-permission android:name="android.permission.INTERNET" />`
**创建一个activity_main.xml的webView:**




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

    <!-- 一个WebView -->
    <WebView
        android:id="@+id/webview1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content" />

</RelativeLayout>
```

**MainActivity.java：**




```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class MainActivity extends Activity {
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        WebView wv = (WebView) findViewById(R.id.webview1);  
        
        WebSettings webSettings = wv.getSettings();  
        webSettings.setBuiltInZoomControls(true);  
        wv.loadUrl("http://initphp.com"); //加载我的PHP框架代码

    }

}
```

**效果：**


![](https://img-blog.csdn.net/20130703173647343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

****

**如果加载assets文件夹下面有一个HTML文件，也可以使用loadUrl()方法去把它加载到WebView中：**



```java
WebView wv = (WebView) findViewById(R.id.webview1);  
    wv.loadUrl("file:///android_asset/Index.html");
```





