# iOS开发：WebKit、Safari以及SafariViewController - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月29日 09:32:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：525
自iOS问世以来，iOS 和web总是形影相随。毕竟，当iOS诞生的时候，网站已经有15年的历史了（第一个网站的创建甚至可以追溯到1991年，而iOS在2007年才出现）。iPhone出现的时候就已经有很多的web内容了，这样iOS必须提供一种方法来显示web内容。
在iOS 9中，我们有不同的方法来渲染web内容，并且根据具体的需求，开发者可以评估不同的方法，然后选择一个更适合他们的方法。让我们看看开发者可以采用的不同的解决方案。
**WebView**
WebKit是OS X和iOS中Safari背后的架构和渲染引擎。它允许解析和渲染html，加载并显示图片，以及执行JavaScript代码。
UIWebView类在iOS 2中引入，是UIKit的一部分。它现在被广泛地应用，而且已经有相当长的一段时间了。在一个iOS应用中，UIWebView是唯一一个可以嵌入web内容的工具。
在iOS 8上，Apple引入了WKWebView，以代替 iOS 上的UIWebView，以及OS X上的WebView。新的API接口给两个平台带来了完全相同的功能，简化了应用和网页之间的通信流程，同时多进程架构也可以用在Safari上。
WKWebView提供了许多以前只能在 Safari 上使用的特性。它使用硬件加速器和Core Animation提供一个60fps的滚动响应、快速执行JavaScript代码，和像内置在Safari上的滑动后退和缩放的手势。其超越UIWebView的一个主要改进就是简化了应用-网页（app-webpage）间的通信、简化了应用和其 web 内容之间交互和数据是如何进行传递的。
WKWebView很容易实现。这里用Swift语言写的代码证明了这点：
```js;toolbar
let viewFrame = ... //1
let webView = WKWebView.init(frame: viewFrame) //2
let url = NSURL(string: "https://www.invasivecode.com") //3
let request = NSURLRequest(URL: url!) //4
webView.loadRequest(request) //5
view.addSubview(webView) //6
```
以下是Objective-C版本：
```js;toolbar
CGRect viewFrame = ... //1
WKWebView *webView = [[WKWebView alloc] initWithFrame:viewFrame]; //2
NSURL *url = [NSURL URLWithString:@"https://www.invasivecode.com"]; //3
NSURLRequest *request = [NSURLRequest requestWithURL:url]; //4
[webView loadRequest:request]; //5
[self.view addSubview:webView]; //6
```
第一行定义了用于第二行初始化webView的frame。第三行创建并返回了一个用URL字符串初始化的NSURL对象。第四行创建并返回了一个URL请求。第五行里，网页加载了URL请求。最后，第六行把网页加到视图控制器里的页面上。
这边是以上代码运行出来的效果：
![ic-wkwebview.png](http://cc.cocimg.com/api/uploads/20160219/1455865425355051.png)
WKWebView是web内容的一个视图，它允许使用Javascript和html渲染。你可以添加自己的返回按钮，前进按钮和进度指示器。在iOS 9中，你可以另外安全地加载文件的url和数据。
WKWebsiteDataStore在iOS 9和OS X 10.11中引入，是一个新的API，它用于管理一个网站站点存储的数据，例如cookies，它是你网页的WKWebViewConfiguration上的一个可读写的属性。你可以根据类型或者时间来删除数据，例如cookies和缓存，你可以用非持久性数据存储来改变配置。
WKWebView提供了很多特性，如果你有web内容并且需要自定义，你可以用这个工具，使用Javascript参数来实时控制页面。
然而，如果你只是想显示web内容，你既可以用Safari，也可以用SFSafariViewController。让我们来看看这两种方法。
**Safari**
用Safari允许你将渲染 web 内容的任务委托 Safari。这样你就可以比在使用WKWebView时少写些代码了。
要从应用程序里启动Safari的话，你可以调用UIApplication中的openURL：方法，然后iOS会把Safari带到前台，把你的应用程序放到后台。
iOS9上的一个新的特性是允许用户通过一个返回按钮回到你的应用里，这个按钮自动出现在用户界面的左上角上（看下图里的A）。
由Safari提供的一样很好的用户体验，这对开发者来说是一个简单的解决方法。下一张图片突出了UI组件（看下图的B和C位置），你可以轻松获得这个功能。
![ic-safari.png](http://cc.cocimg.com/api/uploads/20160219/1455865521422373.png)
这里有一些例子代码向你展示了如何从你的应用里启动Safari，来渲染一些web内容：
Swift
```js;toolbar
@IBAction func showWebSite() {
    let url = NSURL(string: "https://www.invasivecode.com")
    UIApplication.sharedApplication().openURL(url!)
}
```
这有Objective-C语言写的相同源代码：
```js;toolbar
- (IBAction)showWebSite:(id)sender {
    NSURL *url = [NSURL URLWithString:@"https://www.invasivecode.com"];
    [[UIApplication sharedApplication] openURL:url];
}
```
**Safari View Controller**
让我们来看下SFSafariViewController。这个类已经在iOS9里引入了。使用SFSafariViewController，Safari被嵌入到你的应用程序里，用户就不用退出你的应用了。
Safari ViewController消除了干扰，把URL字段转换成只读的（看下图中的D），以及一个完成按钮用以回到你的应用中（看图片上的E）。
这里，有些例子代码展示了通过SafariViewController如何给你的用户提供web内容。
让我们先看看用Swift语言的实现：
```js;toolbar
@IBAction func showWebSite() {
    let url = NSURL(string: "https://www.invasivecode.com")
    let safariVC = SFSafariViewController(URL: url!)
    self.showViewController(safariVC, sender: nil)
}
```
现在来看下用Objective-C语言写的相同的实现：
```js;toolbar
- (IBAction)showWebSite:(id)sender {
    NSURL *url = [NSURL URLWithString:@"https://www.invasivecode.com"];
    SFSafariViewController *safariVC = [[SFSafariViewController alloc] initWithURL:url];
    [self showViewController:safariVC sender:nil];
}
```
以上代码的结果和下图的相似：
![ic-safariviewcontroller1.png](http://cc.cocimg.com/api/uploads/20160219/1455865680302062.png)
现在，让我们把你在应用程序中用到的SafariViewController提供的特点分一一分类。
**一个熟悉的用户界面**
SafariViewController提供了和Safari一样的界面，允许你在应用程序中无缝集成，更尊重你应用的色调。你会发现它有Safari提供的相同的共享方法，用户可以将网页添加到阅读列表里，或者把页面发送给朋友（看下图的F）。此外，开发者也可以添加专用按钮到表单上。
![ic-safariviewcontroller2.png](http://cc.cocimg.com/api/uploads/20160219/1455865756453171.png)
**密码、信用卡和联系卡自动填充**
SFSafariViewController允许用户安全按地自动填充iCloud钥匙串里的敏感信息和证书。这包括填充密码、信用卡和联系人信息。
**用Safari共享的Cookies**
Safari和SFSafariViewController共享Cookies，意味着两者间的用户会话和偏好依然存在。比如，如果用户已经用Safari注册或者将他们的密码存在iCloud钥匙串里，授权一个应用程序访问Twitter或者Facebook的时候，将会是一个流畅的用户体验。
**Safari阅读器**
Safari阅读器按钮将会允许用户显示一个易读版本的网站，供用户选择提供的主题和字体。
**Content Blocking(内容阻挡/内容拦截外挂程式)**
iOS9开始，任何应用程序可以写一个web内容的描述，用户浏览网页的时候Safari和SafariViewController进行拦截。此外，用户也可以在设置里添加内容拦截元素（Content blocking elements）。那些元素将被拦截，不展示给用户。这意味着应用程序用Safari或者SafariViewController展示的网页内容，将会被用户或者开发者选择的内容拦截元素（Content blocking elements）影响。内容拦截元素（Content blocking elements）可以是网页或者拦截加载的单独的元素，就像页面里的所有的图片和脚本一样。
**总结**
当在你的应用程序里展示web内容，你可以选择不同的方法，这取决于你的需求。如果你需要修改或操作web内容，iOS8中引入的WKWebView为我们提供了极大的灵活性。在应用程序里启动Safari，会给用户和Safari提供的相同体验，但是你的用户可能不会再回到你的应用程序里。SFSafariViewController像是在你的应用中嵌入了Safari，而且很容易使用。
关于作者
Eva Diaz-Santana（[@evdiasan](http://twitter.com/evdiasan)）是invasiveCode的创始人。2008年以来，她开发iOS应用，并且教授iOS开发课程。同时她在苹果公司工作，担任Cocoa架构师和用户体验设计师。
- 
更多译者翻译文章，请查看：[http://www.cocoachina.com/special/translation/](http://www.cocoachina.com/special/translation/)
- 
本文仅用于学习和交流目的，转载请注明文章译者、出处和本文链接。?
- 
感谢[博文视点](http://book.douban.com/subject/26541801/)对本期活动的支持
![博文logo-01.jpg](http://cc.cocimg.com/api/uploads/20160219/1455865961472445.jpg)
