# 实例：iOS 中的 JS 交互 OC & Swift 双语 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月12日 21:35:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：554
# 写在前面
随着 App 开发日趋成熟，不少设计模式和开发流程都被应用其中，也跟着日趋成熟。其中一条相信很多人都听说过，不少项目也是这样做的：
> 
用 H5 页面适配代替 Native App 原生开发语音开发的页面
其实想一想不难理解为什么要这样做？优势有很多，应用场景也有很多。比方说一个非核心功能模块，需求多变需要实时更新功能，此时如果用 Native App 原生开发语言开发就显得有些不太适合，尤其是 iOS 方面自己写出的代码逻辑一时之间很难满足比较大的需求变动或者考虑到以后所有的变化可能性就需要维护代码，还要 Apple 审核方可更新 App Store 中的 App 版本，这时候用 Web 页面内嵌 App 可用的系统浏览器显示相关模块的页面给用户就显得靠谱许多。而且很多移动互联网公司都是 Android
 与 iOS 和 Web 风格几乎统一的三管齐下模式，用 Web 内嵌入 App 显示还省时省力，不仅轻松统一了 Android 和 iOS 客户端相关页面还帮两方的小伙伴节省出时间精力去做好客户端的核心功能。
# Web 页面与 Native App 交互
我们上面水了那么多话就是为了说明为啥要使用 Web 页面内嵌 App。那么接下来就要说一下如果采用了 Web 页面内嵌 App 的话需要解决的问题：
- 需要适配手机，pad，页面高仿 App 原生页面。（这个我们不管，工作板上找到 H5 的同事们，把需求丢给他们就好了） 
- 有些 Web 页面需要与 App 内部做交互，达到不仅看着像 App 原生界面，用起来还能跳转到我们其他 App 原生界面做交互的境界
暂时就想到这么多，不过满足了上面两条，相信这个 Web 页面内嵌对于用户来说就很成功了。
# 情景模拟
模拟还是拿自己说事儿吧，这些天负责做产品的社区模块（之前负责社区模块的女同事产假 ing，所以这块是大家分摊着做的），有个需求是社区中的帖子（图文混排）需要加入点击可以浏览大图以及保存的功能。可是 H5 那边的页面图片是没有点击事件的，这难不倒伟大的编码劳动人民。查了一下资料早有前辈遇到此坑，简单的介绍一下解决方法：就是利用 JS 嵌入 H5 页面，在 JS 中为 H5 中的图片绑定一个点击事件，并给点击事件写特定的标识；之后在点击事件触发时获取当前点击的图片 Url 进行浏览处理（浏览这块我在这里不赘述了，主要是把相关的
 JS 贴一下，简单解读一下）。
# 相关代码
先贴一下 OC 的代码，由于我们项目要兼顾 iOS 8.0 之前的 iOS 系统，所以我就先贴出 UIWebView 对应的代码，（关于 WKWebView 的之后放出）：
## 首先是加载请求
这块很不厚道的加载了我自己博客的一篇文章*（只是宣传绝非广告，我也不是专业有输出的技术博客牛人，个人博客实在是点击率可怜 T.T）*
### obj-C
```
#pragma mark - Foundations
- (void)doLoadRequest {
    NSURL *url = [NSURL URLWithString:@"http://blog-lision.com/2016/05/26/iOS-Multithreading/"];
    NSURLRequest *urlRequest = [NSURLRequest requestWithURL:url];
    [(UIWebView *)self.webView loadRequest:urlRequest];
}
- (void)showBigImgWithUrl:(NSString *)url {
    NSLog(@"查看大图，图片 url <%@>", url);
}
```
### Swift
```
// MARK: - Foundations
    func doLoadRequest() {
        let url = NSURL.init(string: "http://blog-lision.com/2016/05/26/iOS-Multithreading/");
        let urlRequest = NSURLRequest.init(URL: url!);
        webV.loadRequest(urlRequest);
    }
    func showBigImgWithUrl(url urlString : String) {
        print("查看大图，图片 url <%s>", urlString);
    }
```
## 再来截取请求
这里在开始加载请求时截获 url，并根据 url 中**特定标识**用 cocoa 原生 API 做相关动作，以达到 JS 与 App 内部交互的目的
### obj-C
```
#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    //预览图片, image-preview 与之前 JS 里面添加的要对应
    if ([request.URL.scheme isEqualToString:@"image-preview"]) {
        NSString *path = [request.URL.absoluteString substringFromIndex:[@"image-preview:" length]];
        path = [path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        //path 就是被点击图片的url
        if ([self.mImgUrls containsObject:path]) {
            [self showBigImgWithUrl:path];
        }
        return NO;
    }
    return YES;
}
```
### Swift
```
// MARK: - UIWebViewDelegate
    func webView(webView: UIWebView, shouldStartLoadWithRequest request: NSURLRequest, navigationType: UIWebViewNavigationType) -> Bool {
        //预览图片, image-preview 与之前 JS 里面添加的要对应
        if (request.URL?.scheme == "image-preview") {
            let length = request.URL?.scheme.characters.count;
            var path = ((request.URL?.absoluteString)! as NSString).substringFromIndex(length!);
            path = path.stringByAddingPercentEscapesUsingEncoding(NSUTF8StringEncoding)!;
            //path 就是被点击图片的url
            if (mImgUrls.contains(path)) {
                showBigImgWithUrl(url: path);
            }
            return false;
        }
        return true;
    }
```
## 完全加载后
完全加载之后按照我上述的情景应该要给当前页面的所有本来不具备点击事件（我的博客内部图片有点击事件，只是我项目中的实际需求没有，但是不便于给大家贴实际的链接，不过效果一样）添加点击事件，并设下标识，等点击之后从上面开始加载请求处截取对应标识的请求，用需求相关的事件调用即可。（**这里主要用 JS 嵌入页面来获取当前页面的所有图片并给图片加入点击事件**）
### obj-C
```
- (void)webViewDidFinishLoad:(UIWebView *)webView {
    //这里是js，主要目的实现对url的获取
    static  NSString * const jsGetImages =
    @"function getImages(){\
    var objs = document.getElementsByTagName(\"img\");\
    var imgScr = '';\
    for (var i = 0; i < objs.length; ++i){\
    imgScr = imgScr + objs[i].src + '+';\
    };\
    return imgScr;\
    };";
    [webView stringByEvaluatingJavaScriptFromString:jsGetImages]; // 注入js方法
    NSString *urlResurlt = [webView stringByEvaluatingJavaScriptFromString:@"getImages()"];
    self.mImgUrls = [NSMutableArray arrayWithArray:[urlResurlt componentsSeparatedByString:@"+"]];
    if (self.mImgUrls.count >= 2) {
        [self.mImgUrls removeLastObject];
    }
    // urlResurlt 就是获取到得所有图片的url的拼接；mImgUrls就是所有Url的数组
    // 添加图片可点击js
    [self.webView stringByEvaluatingJavaScriptFromString:@"function registerImageClickAction(){\
    var imgs = document.getElementsByTagName('img');\
    for (var i = 0; i < imgs.length; ++i){\
    img = imgs[i];\
    img.onclick = function(){\
    window.location.href = 'image-preview:' + this.src}\
    }\
    }"];
    [self.webView stringByEvaluatingJavaScriptFromString:@"registerImageClickAction();"];
}
```
### Swift
```
func webViewDidFinishLoad(webView: UIWebView) {
        var jsGetImages = "function getImages(){";
        jsGetImages += "var objs = document.getElementsByTagName(\"img\");";
        jsGetImages += "var imgScr = '';";
        jsGetImages += "for (var i = 0; i < objs.length; ++i){";
        jsGetImages += "imgScr = imgScr + objs[i].src + '+';";
        jsGetImages += "};";
        jsGetImages += "return imgScr;";
        jsGetImages += "};";
        webView.stringByEvaluatingJavaScriptFromString(jsGetImages);
        let urlResurlt = webView.stringByEvaluatingJavaScriptFromString("getImages()");
        mImgUrls = (urlResurlt?.componentsSeparatedByString("+"))!;
        if (mImgUrls.count >= 2) {
            mImgUrls.removeLast();
        }
        // urlResurlt 就是获取到得所有图片的url的拼接；mImgUrls就是所有Url的数组
        // 添加图片可点击js
        var jsRegisterImageClickAction = "function registerImageClickAction(){";
        jsRegisterImageClickAction += "var imgs = document.getElementsByTagName('img');";
        jsRegisterImageClickAction += "for (var i = 0; i < imgs.length; ++i){";
        jsRegisterImageClickAction += "img = imgs[i];";
        jsRegisterImageClickAction += "img.onclick = function(){";
        jsRegisterImageClickAction += "window.location.href = 'image-preview:' + this.src}";
        jsRegisterImageClickAction += "}";
        jsRegisterImageClickAction += "}";
        webView.stringByEvaluatingJavaScriptFromString("registerImageClickAction();");
    }
```
# 写在最后
其实以前总觉得手机原生的 SDK 做出来的页面更精致，而加载的页面怎么也不会超过原生的。而且 iOS 8.0 前的 UIWebView 你们都懂的，虽然有大神的各种优化方案但是还是不尽人意……可是随着时间的演变，Web 与 App 交互已经成为主流了。以前没怎么研究过这类交互以及 JS，不过JS 学起来还是很快的，语法也很简单，经过了这次之后我也对 JS 产生了不小的兴趣，以后有机会的话也会继续深入的。
文／Lision（简书作者）
原文链接：http://www.jianshu.com/p/5af39a34f45d
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
