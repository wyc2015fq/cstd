# Android WebView的那些坑 - weixin_33985507的博客 - CSDN博客
2016年11月14日 23:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
最近接手的一些需求与H5相关，自然而然就涉及到WebView这神器，大家都知道这里面的水很深。
下面我要说的就是我在这里面遇到的一些坑，今天下午下班被老大就到办公室，学习了3个小时，
刚从办公室里面出来，回到座位上，唤醒电脑就看到了测试妹子发过了的bug：说收到用户反馈说
灰度包中的包月页面弹不出来手机键盘，用户无法输入手机号码。
收到这个消息我还没有反应过来，还有测试妹子说包月页面是H5页面，那问题就是WebView了。
我回头看下下我写的webView基类，是这样：
> 
public classBaseWebViewextendsWebView {
private static finalStringTAG="BaseWebView";
publicBaseWebView(Context context) {
this(context,null);
}
publicBaseWebView(Context context,AttributeSet attrs) {
this(context,attrs,0);
}
publicBaseWebView(Context context,AttributeSet attrs, intdefStyleAttr) {
super(context,attrs,defStyleAttr);
init(context);
}
@TargetApi(VERSION_CODES.LOLLIPOP)
publicBaseWebView(Context context,AttributeSet attrs, intdefStyleAttr, intdefStyleRes) {
super(context,attrs,defStyleAttr,defStyleRes);
init(context);
}
publicBaseWebView(Context context,AttributeSet attrs, intdefStyleAttr, booleanprivateBrowsing) {
super(context,attrs,defStyleAttr,privateBrowsing);
init(context);
}
}
简单点就是这个样子的：
![3684356-f72dea39bb416c67.png](https://upload-images.jianshu.io/upload_images/3684356-f72dea39bb416c67.png)
咋一看，没有什么问题啊！！！但是可以怀疑任何人，千万不要怀疑测试。于是我想起了一个金句：有任何问题，去看源码。
用道上人的说法是：look the fucking code。
于是我转手就跳转到了源码里面：
> 
public classWebViewextendsMockView {
/**
* Construct a new WebView with a Context object.
*@paramcontextA Context object used to access application assets.
*/
publicWebView(Context context) {
this(context, null);
}
publicWebView(Context context,AttributeSet attrs) {
this(context,attrs,com.android.internal.R.attr.webViewStyle)
}
publicWebView(Context context,AttributeSet attrs, intdefStyle) {
super(context,attrs,defStyle);
}
...
}
简单点就是这个样子的：
![3684356-6bf686e1104538df.png](https://upload-images.jianshu.io/upload_images/3684356-6bf686e1104538df.png)
图片上标注的就是我惊奇的发现。这个真的是打脸啊。于是我就按照源码的写了。果然问题就解决了。
ps：这次的问题，算是我长久以来存在的缺陷，这是我之前在自定义基础控件时，时常忽略的问题，也给了我深刻的
认识，所以才有了这篇小的TIPS，与群共勉！
