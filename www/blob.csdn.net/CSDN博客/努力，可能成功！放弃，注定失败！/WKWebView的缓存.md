# WKWebView的缓存 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:52:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：16标签：[WKWebView的缓存](https://so.csdn.net/so/search/s.do?q=WKWebView的缓存&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
 相信使用过webView的人都挺烦每次都有一段空白期，用户体验感极差，所以公司要求在webView上增加缓存，废话不多说了，进入正题！
### 1.使用WKWebView
```
// 1.简单创建WKWebView
    WKWebView *webView = [[WKWebView alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, self.view.frame.size.height - 20)];
    webView.navigationDelegate = self;
    [self.view addSubview:webView];
    // 2.创建请求
    NSURL *URL = [NSURL URLWithString:@"xxxxxxxxx"];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:URL];
    // 3.加载网页
    [webView loadRequest:request];
```
### 2.最重要的部分来了，重写NSURLProtocol
思路逻辑：
  使用webView加载页面的时候，如果有缓存，就先加载缓存，没有就请求网络，然后使用系统自带的缓存方式存储；当重新请求网络，就算下与最近一次网络请求相隔有没一个小时，有的话就重新请求网络。这边借用下一大神的图片
![](https://upload-images.jianshu.io/upload_images/6955990-10287e9178b484ca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/497/format/webp)
cache策略.png
我们这边继续使用大神写好的一个自定义的**NSURLProtocol**。
使用方法是：
1、**pod 'JWNetAutoCache'**
2、在需要开启的时候调用
```
[JWCacheURLProtocol startListeningNetWorking];
```
3、使用结束后调用：
```
[JWCacheURLProtocol cancelListeningNetWorking];
```
然而这个对于WKWebView来说，它是不走**NSURLProtocol**的，看到这，惊不惊喜，意不意外！！！不过别急，离成功就差一步了。当然，关于UIWebView的更详细操作请看[UIWebview使用缓存并且保证实时性](https://www.jianshu.com/p/69468d0587b6),
### 3.让WKWebView走NSURLProtocol
  在 WKWebView 中的请求只是象征性走
```
+ (BOOL) canInitWithRequest:(NSURLRequest *)request
```
之后的整个请求流程似乎就与 NSURLProtocol 完全无关了，但是在UIWwbView中确是正常的，不过由于我之前在WKWebView的代理中实现了一些东西，所以没改变使用WKWebView。一开始一脸懵逼，不懂为啥，不过幸亏在一大神那得到了解决方法，由于大神说的很详细，所以我这边就不重述了(**呼呼，其实是我自己也讲不太清楚**)。
下面来操作一番：
1、先下载一大神写的**NSURLProtocol**d的分类[NSURLProtocol+WKWebVIew](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FLiuShuoyu%2FHybirdWKWebVIew%2F)
2、然后在**viewDidLoad**里通过注册把**http**和**https**请求交给**NSURLProtocol**处理。
```
[NSURLProtocol wk_registerScheme:@"http"];
[NSURLProtocol wk_registerScheme:@"https"];
```
到目前为止，可以说是大功告成了。关于这一方面的，想了解更多的请看[iOS WKWebView (NSURLProtocol)拦截js、css，图片资源](https://www.jianshu.com/p/4fc13d4d5607)。
参考文献：
[iOS WKWebView (NSURLProtocol)拦截js、css，图片资源](https://www.jianshu.com/p/4fc13d4d5607)
[UIWebview使用缓存并且保证实时性](https://www.jianshu.com/p/69468d0587b6)
作者：深漂的小C
链接：https://www.jianshu.com/p/99698fcaf166
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
