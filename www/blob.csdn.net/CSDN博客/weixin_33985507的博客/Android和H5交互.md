# Android和H5交互 - weixin_33985507的博客 - CSDN博客
2017年12月11日 23:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
Android与H5的交互比较简单，这里记录一下。
##### WebView的基本设置
```
WebSettings settings = wv.getSettings();
     //让webView支持JS
     settings.setJavaScriptEnabled(true);
       //设置客户端
        wv.setWebViewClient(new WebViewClient() {
            //覆写shouldOverrideUrlLoading实现内部显示网页
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
            //页面开始加载
            @Override
            public void onPageStarted(WebView view, String url, Bitmap favicon) {
                super.onPageStarted(view, url, favicon);
            }
            //页面加载完成
            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
            }
        });
        
      //设置这个客户端，可以设置加载网页的进度条（布局就不写了，就是在WebView上面放一个ProgressBar）
        wv.setWebChromeClient(new WebChromeClient() {
            @Override
            public void onProgressChanged(WebView view, int newProgress) {
                if (null != mProgressBar1) {
                    if (newProgress == 100) {
                        mProgressBar1.setVisibility(View.GONE);//加载完网页进度条消失
                    } else {
                        mProgressBar1.setVisibility(View.VISIBLE);//开始加载网页时显示进度条
                        mProgressBar1.setProgress(newProgress);//设置进度值
                    }
                }
            }
        });
  
 //设置返回键动作（防止按返回键直接退出程序)
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (wv.canGoBack()) {//当webview不是处于第一页面时，返回上一个页面
                wv.goBack();
                return true;
            } else {
                finish();
            }
        }
        return super.onKeyDown(keyCode, event);
    }   
//WebView加载本地网页（H5本地页面一般放在assets目录下，路径： 如果html文件存于assets：则加前缀：file:///android_asset/     如果在Sdcard直接使用file:///sdcard/ or file:/sdcard也可以）  
 wv.loadUrl("file:///android_asset/xxx.html");
```
##### Android调用H5的方法
```
//调用无参的Js方法,      括号里：“javascript:”  +  js方法名(H5里面定义的名称)
 wv.loadUrl("javascript:message1()");   
//调用有参数的Js方法 ，而且要注意，参数要加单引号
wv.loadUrl("javascript:message2('" + message+ "')");
```
##### H5调用 Android的方法
1.建立内部类，在内部类中增加Js要调用Android的方法，需要增加注解
```
public class JsClass {
        @JavascriptInterface
        public void toMessage( String id) {
           
        }
    }  
//将Android传递给Js (  "android" 表示在H5那边要进行调用方法的主体，例如：android.toMessage(id) )
wv.addJavascriptInterface(new JsClass(), "android");
```
2.不需要建立内部类，直接将自身传递过去，自身实现方法。
`wv.addJavascriptInterface(this, "android");`
##### 优化
WebView的自带的JavascriptInterface的接口使用会有安全隐患，使用[JSBridge](https://link.jianshu.com?t=https://github.com/lzyzsd/JsBridge)
##### H5调用Android另外一种方式：scheme
使用场景：一般用于在H5页面中，点击对应的商品、广告等，进入App对应的页面
```
在功能清单文件中，对应的Activity中设置相关参数
<activity android:name=".TestActivity">
            <intent-filter>
              //这里是对应到H5页面中链接里面写的路径（aa 是 scheme   host 是主机名称  path 是路径）
                <data android:scheme="abc" android:host="123" android:path="/path"/>
                //下面3个是固定的
                <action android:name="android.intent.action.VIEW"/>
                <category android:name="android.intent.category.DEFAULT"/>
                <category android:name="android.intent.category.BROWSABLE"/>
            </intent-filter>
        </activity>
//WebView中设置   
wv.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageStarted(WebView view, String url, Bitmap favicon) {
                super.onPageStarted(view, url, favicon);
              //通过对URl的解析来决定调转到哪个页面
                if (url.contains("aa")) {
                    Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
                    startActivity(intent);
                }
            }
```
