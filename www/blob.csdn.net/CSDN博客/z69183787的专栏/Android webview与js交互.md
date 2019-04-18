# Android webview与js交互 - z69183787的专栏 - CSDN博客
2015年08月28日 15:13:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：649
对于android初学者应该都了解webView这个组件。之前我也是对其进行了一些简单的了解，但是在一个项目中不得不用webview的时候，发现了webview的强大之处，今天就分享一下使用webview的一些经验。
1、首先了解一下webview。
webview介绍的原文如下：A View that displays web pages. This class is the basis upon which you can roll your own web browser or simply display some online content within your Activity. It uses the WebKit rendering engine to display web pages and includes methods to
 navigate forward and backward through a history, zoom in and out, perform text searches and more.
从上面你应该了解到了基本功能，也就是显示网页。之所以我说webview功能强大是因为它和js的交互非常方便，很简单就可以实现。
2、webview能做什么？
①webView可以利用html做界面布局，虽然目前还比较少人这么使用，不过我相信当一些客户端需要复杂的图文（图文都是动态生成）混排的时候它肯定是个不错的选择。
②直接显示网页，这功能当然也是它最基本的功能。
③和js交互。（如果你的js基础比java基础好的话那么采用这种方式做一些复杂的处理是个不错的选择）。
3、如何使用webview？
这里直接用一个svn上取下的demo，先上demo后讲解。demo的结构图如下：
![](http://pic002.cnblogs.com/images/2012/318952/2012070209395373.png)
WebViewDemo.java
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.google.android.webviewdemo;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
/**
 * Demonstrates how to embed a WebView in your activity. Also demonstrates how
 * to have javascript in the WebView call into the activity, and how the activity 
 * can invoke javascript.
 * <p>
 * In this example, clicking on the android in the WebView will result in a call into
 * the activities code in {@link DemoJavaScriptInterface#clickOnAndroid()}. This code
 * will turn around and invoke javascript using the {@link WebView#loadUrl(String)}
 * method.
 * <p>
 * Obviously all of this could have been accomplished without calling into the activity
 * and then back into javascript, but this code is intended to show how to set up the 
 * code paths for this sort of communication.
 *
 */
public class WebViewDemo extends Activity {
    private static final String LOG_TAG = "WebViewDemo";
    private WebView mWebView;
    private Handler mHandler = new Handler();
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.main);
        mWebView = (WebView) findViewById(R.id.webview);
        WebSettings webSettings = mWebView.getSettings();
        webSettings.setSavePassword(false);
        webSettings.setSaveFormData(false);
        webSettings.setJavaScriptEnabled(true);
        webSettings.setSupportZoom(false);
        mWebView.setWebChromeClient(new MyWebChromeClient());
        mWebView.addJavascriptInterface(new DemoJavaScriptInterface(), "demo");
        mWebView.loadUrl("file:///android_asset/demo.html");
    }
    final class DemoJavaScriptInterface {
        DemoJavaScriptInterface() {
        }
        /**
         * This is not called on the UI thread. Post a runnable to invoke
         * loadUrl on the UI thread.
         */
        public void clickOnAndroid() {
            mHandler.post(new Runnable() {
                public void run() {
                    mWebView.loadUrl("javascript:wave()");
                }
            });
        }
    }
    /**
     * Provides a hook for calling "alert" from javascript. Useful for
     * debugging your javascript.
     */
    final class MyWebChromeClient extends WebChromeClient {
        @Override
        public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
            Log.d(LOG_TAG, message);
            result.confirm();
            return true;
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
demo.html
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<html>
    <script language="javascript">
        /* This function is invoked by the activity */
        function wave() {
            alert("1");
            document.getElementById("droid").src="android_waving.png";
            alert("2");
        }
    </script>
    <body>
        <!-- Calls into the javascript interface for the activity -->
        <a onClick="window.demo.clickOnAndroid()"><div style="width:80px;
            margin:0px auto;
            padding:10px;
            text-align:center;
            border:2px solid #202020;" >
                <img id="droid" src="android_normal.png"/><br>
                Click me!
        </div></a>
    </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
main.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    >
    
    <TextView  
        android:layout_width="fill_parent" 
        android:layout_height="wrap_content" 
        android:text="@string/intro"
        android:padding="4dip"
        android:textSize="16sp"
        />
    
    <WebView
        android:id="@+id/webview"
        android:layout_width="fill_parent" 
        android:layout_height="0dip"
        android:layout_weight="1"
        />
        
</LinearLayout>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4、如何交互？
①android如何调用js。
调用 形式：
```
mWebView.loadUrl("javascript:wave()");
```
其中wave（）是js中的一个方法，当然你可以把这个方法改成其他的方法，也就是android调用其他的方法。
②js如何调用android。
调用形式：
```
<a onClick="window.demo.clickOnAndroid()">
```
代码中的“demo”是在android中指定的调用名称，即
```
mWebView.addJavascriptInterface(new DemoJavaScriptInterface(), "demo");
```
代码中的clickOnAndroid()是“demo”对应的对象：new DemoJavaScriptInterface() 中的一个方法。
③双向交互。
当然是把前面的两种方式组合一下就可以了。
5、讲解demo。
现在你一定了解了android和js的交互了。是时候分析一些demo了，根据上面讲的你也应该比较清楚了。具体交互流程如下：
①点击图片，则在js端直接调用android上的方法clickOnAndroid()；
②clickOnAndroid()方法（利用线程）调用js的方法。
③被②调用的js直接控制html。
个人总结：利用webView的这种方式在有些时候UI布局就可以转成相应的html代码编写了，而html布局样式之类有DW这样强大的工具，而且网上很多源码，很多代码片。在UI和视觉效果上就会节省很多时间，重复发明轮子没有任何意义。
