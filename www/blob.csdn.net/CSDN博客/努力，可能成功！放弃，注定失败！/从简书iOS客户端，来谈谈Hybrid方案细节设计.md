# 从简书iOS客户端，来谈谈Hybrid方案细节设计 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月07日 14:33:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：206标签：[iOS 资源大全																[iOS学习资料																[Hybrid](https://so.csdn.net/so/search/s.do?q=Hybrid&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
> 
[原文地址:http://www.jianshu.com/p/9c8552a87a3e](http://www.jianshu.com/p/9c8552a87a3e)
> 
作为一位 iOS 开发人员，你应该已经敏感地发现，自己的工作涉及内容已经不止于 Native 的部分，因为 Hybrid App 和 ReactNative 等技术方案已经不仅仅是概念，越来越多的公司开始着手自己的 Hybrid 方案以及 ReactNative 本地化工作。
### 一、引言
介绍相关概念的优秀文章已经有许多，方案的实现原理你也应该已经或多或少有了一些理解。不了解也没有关系，在这篇文章里，我将用简书 iOS 客户端的有关特性，来探索一下 Hybrid 方案的技术细节。**文章的目的是抛砖引玉**，用一个具体的项目，大家很熟悉的简书客户端，来帮助大家认识 Hybrid 方案，**然后亲自实现它** 。
**从现在开始，不再着眼于某一个 feature ，你需要站在一个客户端架构师的角度来看待问题。**
### 二、我们用到的简书客户端特性
##### 1.界面构成分析
- 
本文的主角是简书 iOS 客户端的文章展示页面，这是我的一篇文章的展示页面：
![](https://img-blog.csdn.net/20170807142935550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
文章展示页面
- 如你所见，文章内容的展示是使用`webview`控件，具体是`UIWebview`还是`WKWebview`按下不表，这不是本文的关键。在我的demo中，我使用了`UIWebview`。
- 在简书`发现`tab栏的内容顶部，还有一个热门内容推荐的轮播图。与它类似是一些app内的活动推介轮播图，以及广告页面，它们的详情页内容展示多使用webview。在简书中，这个轮播图对应的下一级页面也是文章展示页面，特性基本一致。
![](https://img-blog.csdn.net/20170807142940543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
热门内容推荐轮播图
- 在 webview 的基础上，添加了符合浏览器用户习惯的导航栏按钮。包括左侧的`返回`和`关闭`按钮。以及右侧的功能列表按钮。
- 页面底部，是一个工具栏，提供了四个常用的操作。注意这里的评论按钮，它是我们下文的一个谈论点。
##### 2.界面特性分析
- 
一般各家客户端的内容页，都会有一些适于自己功能点的设计。简书也不例外。比如，在文章内容区域点击作者的头像（它本身也是网页的一部分，暂且理解为对应一个链接），跳转到了作者的个人主页，注意，容易发现它是一个客户端的原生页面，也就是一个VC。
![](https://img-blog.csdn.net/20170807142951231)
作者个人主页
- 点击底部工具栏中的`评论`按钮（原生组件），页面（web页面）会滑动到评论区域，如图
![](https://img-blog.csdn.net/20170807142957921)
点击评论按钮，页面滑动
- 对一篇文章写下自己的评论（使用了原生组件），评论列表（网页内容）进行更新。
- 简书对于展示内容作了内外站的区分。据我自己的简要测试，来自简书域名`www.jianshu.com`下的内容，在加载过程中，是没有进度条的，用户体验非常接近原生页面。而第三方的内容，则在加载过程中会出现一般浏览器中常见的加载进度条，如图：
![](https://img-blog.csdn.net/20170807143008055)
第三方内容的加载进度条
- 对于简书域名下的内容，不会出现叉号的`关闭`按钮，这也是为了营造接近原生页面的用户体验，让用户不会察觉到这是一个 web 界面。而第三方内容，则会出现符合浏览器使用习惯的`关闭`按钮，如上图。
### 三、我们需要的储备知识
##### 1.Hybrid相关
- 在Hybrid架构中，原生界面和web页面需要频繁地沟通，并且是双向的沟通。原生代码可以构建`JavaScript`语句，交由webview进行执行，从而在web页面上实现需要的效果。而在web页面的js文件中，也可以调用原生的`Objective-C`方法，从而执行一些原生方法才能完成的操作。与此相关的库有[WebViewJavascriptBridge](https://github.com/marcuswestin/WebViewJavascriptBridge)以及`JavaScriptCore`，有需要的同学可以自行了解。
##### 2.UIWebview的相关特性
- UIWebviewDelegate
webview的代理方法大家想必非常熟悉，我们可以在页面加载前、开始加载时、加载完成时以及失败时进行需要的操作。这里我们需要用到的是这一条代理方法：
`- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;`
webview根据它的返回结果来决定是否进行加载。
- 执行JS语句的方法：
`- (nullable NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script;`
我们可以自行构建一条JS语句，通过这个方法交由webview执行
- goback相关
`UIWebview`拥有布尔类型的`canGoBack`、`loading`等属性，通过监测它们的值我们可以知道当前页面是否可以进行回退，以及页面是否正在加载。
与之对应，拥有`- (void)goBack;`等方法，调用之页面会进行返回，就像我们在浏览器中常见的那样。
### 四、相关特性的模仿实现
**对于上文中提到的相关特性，我出写了一个demo，对它们进行了简要的模仿实现。当然简书官方的实现会考虑到方方面面，而我的demo仅是从Hybrid架构的思想出发，盼能够抛砖引玉。**
这是demo中对该页面的模仿实现：
![](https://img-blog.csdn.net/20170807143013813)
demo页面
##### 1.页面初始化
在demo中，使用一条web页面的URL来初始化VC：`- (instancetype)initWithURL:(NSURL *)URL;`这条URL对应文章的链接。
顶部导航栏和底部工具栏都是系统原生的`UINavigationBar`和`UIToolBar`，按钮素材使用阿里巴巴的iconfont字体。
##### 2.点击作者头像进入个人主页
关于这个特性的实现，如果按照 Hybrid 架构的思想，属于 Web 页面调用原生方法，进入一个原生的VC。点击头像，JS脚本执行相关代码，调用原生方法暴露出来的接口，执行原生方法。
我在这里用一种简要的方法实现：原生代码利用之前提到的代理方法，在用户点击头像后，拦截该URL，分析URL为头像部分，直接执行原生方法跳转到个人主页VC。
通过分析简书文章页面的网页源代码，我发现用户头像对应的`URL`中的`Query`部分，有一个参数为`utm_medium=note-author-link`。据此，在`- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType`代理方法中加以判断，若是头像链接，则跳转到个人主页VC。下面是相关代码：
```
NSURL *destinationURL = request.URL;
    NSString *URLQuery = destinationURL.query;
//    简书点击文章中头像时跳转至原生页面。此处利用头像链接中的一个参数作判断
    if ([URLQuery containsString:@"utm_medium=note-author-link"])
    {
        NSLog(@"我跳转到个人主页啦");
        AvatorViewController *avatorVC = [[AvatorViewController alloc] init];
        [self.navigationController pushViewController:avatorVC animated:YES];
        return NO;
    }
```
最后返回NO是因为若是头像链接，该web页面是不需要做跳转操作的。
> 
这里顺便讲一个小tips：如果想要在Mac端查看移动端的网页源代码，那么你只需要在Safari中输入该页面，并且在`开发`选项下的`用户代理`中，选择iOS系统下的Safari作为代理，这时再使用源代码查看，看到的就是移动端的网页源代码了。
##### 3.点击评论按钮，页面滑动到评论区域
这个特性的实现方式和上面类似，点击评论按钮，原生代码构建一条JS语句，交由`- (nullable NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script;`方法进行执行，由web页面执行滑动操作。代码如下：
```
- (void)scrollToCommentField
{
    [self stringByEvaluatingJavaScriptFromString:@"scrollTo(0,20500)"];
}
```
这里的JS语句非常简单，由于笔者的前端知识还有所欠缺，没有想到可以精确滑动的评论区域的JS语句，所以简要实现，点到为止。
##### 4.原生组件写评论，web页面更新
这里首先需要贴一下文章页面的网页源代码：
```xml
<!-- 评论列表 -->
  <div data-vcomp="comments-list" data-lazy="3">
    <script type="application/json">
      {"likedNote":true,"commentable":true,"publicCommentsCount":3,"noteId":2491941,"likesCount":43}
    </script>
  </div>
```
可以看到，页面的评论内容是异步加载的。所以这个功能的实现，我
认为比较合理的逻辑是原生组件向服务器提交一条新的评论，收到成功回调之后，原生组件和web页面进行交互，执行更新并加载评论列表的JS代码，从而看到自己发的新评论。
##### 5.内外站页面的区分
这里和点击头像的实现方法类似，通过拦截链接的URL，区分内部链接和第三方链接，从而在开始加载的时候采用不同的加载界面，或者对于第三方链接单独开启一个第三方VC。
demo中关于第三方链接的关闭按钮的显示逻辑，做出了相应的处理。
### 五、demo中的不足
看到这里大家应该就会发现，对于提到Hybrid我们就会想到的Bridge、Router等模块，我并没有做明显的限定。这样也是为了方便大家用一种更接近以往原生代码编写的思维，来理解Hybrid模式。
同时，demo中较多涉及了原生代码对web页面做出的沟通操作。而没有JS代码对原生代码的调用，这是因为一来站在一个简书客户端的用户和iOS开发的角度，对于JS端执行的操作，有些力不能及，这本是和你共同工作的前端伙伴的任务，二来对于一篇帮助大家入门Hybrid的文章来说，从这个单方面的交互来入手，管中窥豹，已是足够。
### 六、一些感悟
其实，写了这么多，我觉得收获到一些感悟是最重要的，**下面的要讲的，可能是我觉得更为重要的思想性的东西。**
##### 1.未来的趋势之一，便是大前端团队进行客户端开发。
- 看到这里你发现，如果你们的团队想要采用Hybrid模式进行产品的开发，光靠iOS或者是安卓的客户端工程师是不可能完成的。在客户端框架的开发过程中，需要和前端的工程师沟通具体的技术细节。比如怎样设计接口能够更好地兼顾客户端和前端特点，对于某个问题，如何能把握全局而不是单单从客户端的角度来看待。这些可能是普通的iOS开发工程师和大牛的差距所在之一。
- 越来越多的客户端工程师招聘要求中，出现了熟悉前端语言的要求。如果你能在精通客户端开发之余，对前端语言也游刃有余，那么在接下来的发展趋势中，就会有更多的可能性。所以，请开始你的前端学习吧~。
##### 2.在Hybrid模式下，如何进行产品技术方案的取舍
- 如前文所见，简书客户端对于内部域名的内容和第三方内容，在展示方式上是有明显不同的。在阅读简书的文章时，让用户发现不了自己是在一个浏览器上进行阅读，这在方方面面就极大改善了用户体验。为了做到这一点，我推测简书首先需要对自己的内容进行非常良好的CDN加速，以保证内容加载时不会耗时过长，同时采取一些预加载策略，二是在内容加载时，采用与原生界面部分相同的loading界面，去掉进度条，模拟原生界面的加载过程。而对于第三方的链接，采用`进度条+返回、关闭按钮`的设计，则更符合用户在浏览器中进行阅读的习惯，也可以和自己的内容进行直观区分，这也改善了用户体验。
- 对于某些原生和Web页面都可以实现的特性如何取舍，这也是需要考虑的问题。比如，点击评论按钮页面滑动，这个功能使用web页面的滑动而非原生的控制显然更为自然也更符合用户习惯。而对于撰写评论的功能，使用原生的键盘、编辑器组件，当然就比使用web页面的键入更加稳定可控了。
### 七、文章的demo
##### 对于文章内容，我写了一个demo，这是[demo地址](https://github.com/halohily/LYWebviewController)。
为了便于理解，我为代码写了详尽的注释。
如果觉得它对你有帮助，不妨在github上为我点一个star~非常感谢！
作者：halohily
链接：http://www.jianshu.com/p/9c8552a87a3e
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
