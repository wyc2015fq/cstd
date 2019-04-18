# JS和OC的交互（基于UIWebView） - weixin_33985507的博客 - CSDN博客
2016年04月27日 11:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
#### 前言
最近要把网页端的商城接入app，其中就要用到JS和OC的交互，我自己也根据在Google上查到的资料试了几种方式，下面稍微总结一下。
#### 过程
JS和OC的交互分为两种方式：JS调用OC以及OC调用JS，一般的原则是**如果JS想传值给OC就用JS调用OC，相反则用OC调用JS**。
> 
传统的方式：
**OC调用JS：**
webView对象通过调用stringByEvaluatingJavaScriptFromString这个方法执行一段JS代码实现交互。如：
self.title = [self.webView stringByEvaluatingJavaScriptFromString:@"document.title"];
便可获得标题，这里比较简单，就此略过......
**JS调用OC：**webView拦截链接的方法
-(BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;
实现以上webView的代理方法，当webView每次开始加载URL时会进入这个方法，我们便可以在这个方法实现JS调用OC。
JS代码如下：
![1754605-fd5189fb620afa01.png](https://upload-images.jianshu.io/upload_images/1754605-fd5189fb620afa01.png)
OC代码如下：
![1754605-b0fc100f6af57eab.png](https://upload-images.jianshu.io/upload_images/1754605-b0fc100f6af57eab.png)
如上图当JS中**window.location.href = "iOS:shareToTest"**的代码被触发，会进入OC中的这个代理方法，并且获得**"iOS:shareToTest"**这个字符串，接下进行一系列的字符串解释，得到需要被实现的方法名且调用。如果需要传值可把需要传的值拼接在字符串上，字符串解释后获取响应的值后调用一下方法:
![1754605-5063f020d7e30d85.png](https://upload-images.jianshu.io/upload_images/1754605-5063f020d7e30d85.png)
这种JS调用OC的方法的缺点十分明显，需要繁琐地**解释字符串**得到相应的方法名和传值，且最多只能有两个值，调用的方法也不能传递返回值；但是也有一个优点：**不需要等待页面加载完才触发**，**当相应的代码被运行就能调用OC的方法**，这也是下面要讲到的JavaScriptCore的一个小坑。
> 
苹果推荐的框架--JavaScriptCore
JavaScriptCore是苹果在iOS7时新推出用以实现JS和iOS代码交互的框架，十分简单高效。以下两篇在网上搜到关于JavaScriptCore用法的博文，写得十分详细，照着做基本没什么问题。
[iOS js oc相互调用（JavaScriptCore） - 
        - 博客频道 - CSDN.NET](https://link.jianshu.com?t=http://blog.csdn.net/lwjok2007/article/details/47058101)
[iOS js oc相互调用（JavaScriptCore）（二） - 
        - 博客频道 - CSDN.NET](https://link.jianshu.com?t=http://blog.csdn.net/lwjok2007/article/details/47058795)
需要特别说明的一点是：第二篇博文是介绍JS调用OC的方法，有两种做法：
**第一种是直接调用方法通过iOS的block回调**，这种做法虽然简单易懂，也可以传值，但是不能传递返回值，而我那时刚好有**返回值的需求**，所以我参照的**第二种方法，这种方法的原理就是把JS的对象和我们本地建立的对象捆绑在一起，并调用相应的方法**。这种方法虽然代码写得比较复杂，但是调用方法十分畅顺，能够**正常传值和返回值**，推荐！！！
最后关于这个框架**最需要强调的一点**是：JS调用OC时，**是需要等浏览器加载完页面后才能进行交互（坑！！！）**，而安卓是不需要的，当时我做到这里也被坑了，明明我自己写的demo是可以交互的（**用按钮点击触发交互，显然这是在页面加载后触发的**），但加载后台的页面就不行，因为我们的后台页面是**在加载的过程中就想调用OC的方法，所以不能实现正常交互**，而我们后台当时的做法就是把方法延时执行，但是这样不仅会影响用户体验也会受到网速的影响，因此不太稳定，不建议这样做。
> 
优秀的第三方框架--WebViewJavascriptBridge
先奉上这个框架的GitHub地址[GitHub - WebViewJavascriptBridge](https://link.jianshu.com?t=https://github.com/marcuswestin/WebViewJavascriptBridge)
事实上关于JS和OC的交互，上Google一查，很多人都在推荐用这个框架，我下载了demo，看了一下，确实是十分好用，同时**支持UIWeView和WKWebView，**无论是**JS调用OC**还是**OC调用JS**，都可以**正常传值**和**返回值**；而且在页面**加载时**只要JS代码被运行就可以**进行交互**，上面遇到的缺点和坑在这里都被掩埋的，真是良心之作，好有真诚！！！具体用法在GitHub上说的挺详细的，下面大概说一下吧：
JS代码：
先把下述代码复制到JS
![1754605-6dd06f337b6e9856.png](https://upload-images.jianshu.io/upload_images/1754605-6dd06f337b6e9856.png)
调用的代码：
![1754605-d6f2043c2ab8ff6d.png](https://upload-images.jianshu.io/upload_images/1754605-d6f2043c2ab8ff6d.png)
OC代码：
注册一个WebViewJavascriptBridge的对象
![1754605-8044b98739bbe186.png](https://upload-images.jianshu.io/upload_images/1754605-8044b98739bbe186.png)
![1754605-d033edeb57c46cbc.png](https://upload-images.jianshu.io/upload_images/1754605-d033edeb57c46cbc.png)
调用的代码：
![1754605-fd2e117c29fbce86.png](https://upload-images.jianshu.io/upload_images/1754605-fd2e117c29fbce86.png)
无论是OC和JS，主动调用的一方都是先调用callHandler方法，而被调用的一方则在registerHandler方法里响应，看起来很清晰明了吧。
如果真的要用到这个框架，除了iOS的开发人员外，也要**让后台的人了解这个框架**，并在**合适的位置注入上述JS代码**，虽然是比较麻烦，但是这个框架确实挺好用，推荐指数5颗星！！！
#### 结束
方法是多样的，至于到底使用哪种方法，还要看各自的产品需求，不管是黑猫还是白猫，能捉到老鼠的就是好猫，哈哈。以上就是我做这个功能时的个人总结，浅薄之见，多多包涵。最后送上座右铭：学习之路，与君共勉。
