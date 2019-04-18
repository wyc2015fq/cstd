# AJAX POST&跨域 解决方案 - CORS - z69183787的专栏 - CSDN博客
2016年11月09日 17:49:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6280
好了，下面进入实战，直接说说使用方法。
```
1 var xhr = new XMLHttpRequest();  
2 xhr.open("POST", "http://www.b.com", true);  
3 xhr.send();
```
没有看错！就是这么简单！
和Ajax的调用方法是毫无差异的，需要的只是服务器端的配合。
服务器端如何配置？
PHP：只需要使用如下的代码设置即可。
```
1 <?php  
2  header("Access-Control-Allow-Origin:*");
```
以上的配置的含义是允许任何域发起的请求都可以获取当前服务器的数据。
当然，这样有很大的危险性，恶意站点可能通过XSS攻击我们的服务器。
如果仅支持www.a.com这个站跨域访问，那就：
```
1 <?php  
2  header("Access-Control-Allow-Origin: http://www.a.com");
```
这样就基本配置ok了~
前端的部分，如果需要跨域向服务器发cookies
还需要设置一个属性：withCredentials
```
1 var xhr = new XMLHttpRequest();  
2 xhr.open("POST", "http://www.b.com", true);  
3 xhr.withCredentials = true; //支持跨域发送cookies
4 xhr.send();
```
 当然，服务器端也要设置
```
1 <?php
2 header("Access-Control-Allow-Credentials: true");
```
好吧，下面说说需要注意的地方。
如果不使用第三方库，用原生javascript来写，还是有些地方需要注意的。
1、不要设置X-Requested-With头
因为跨域访问，如需带header，服务器端必须要allow，不然报错。
setRequestHeader("X-Requested-With", "XMLHttpRequest")主要用在Ajax调用资源时，服务器能判断该次请求是Ajax的。
2、INVALID_STATE_ERR: DOM Exception 11
这是一个神奇的错误，网上找了一下，也没什么较为明确的答复。但我却在手机端遇到了这个问题！
测试了一下：在IOS4和IOS5的UC浏览器、Safari、Chrome，进行CORS访问均会报这个错，
Android4.0原生浏览器，也无法正常CORS（没有测试2.3和2.2）
估计也是报这个错，但没控制台，所以无法查bug。
Android4.0下的Chrome和UC都可以顺利CORS。
本以为是浏览器兼容的问题，经过蛋疼的排查之后，发现...
我们可以看一下，下面两段代码有啥差异？
```
1 var xhr = new XMLHttpRequest();  
2 xhr.withCredentials = true; //支持跨域发送cookies
3 xhr.open("POST", "http://weibo.com/demo/b/index.php", true);  
4 xhr.send();
```
```
1 var xhr = new XMLHttpRequest();  
2 xhr.open("POST", "http://weibo.com/demo/b/index.php", true);  
3 xhr.withCredentials = true; //支持跨域发送cookies
4 xhr.send();
```
唯一的差异就是xhr.withCredentials = true; 的位置。
但就是这个差别，导致第一段代码无法顺利在手机端运行，并报INVALID_STATE_ERR: DOM Exception 11这个错！
而在桌面版浏览器下，两段代码都可以顺利运行！
所以，以后设置withCredentials属性时，一定要在open方法之后！
- CORS能做什么：
     正常使用AJAX会需要正常考虑跨域问题，所以伟大的程序员们又折腾出了一系列跨域问题的解决方案，如JSONP、flash、ifame、xhr2等等。
     本文介绍的CORS就是一套AJAX跨域问题的解决方案。
-  CORS的原理：
     CORS定义一种跨域访问的机制，可以让AJAX实现跨域访问。CORS 允许一个域上的网络应用向另一个域提交跨域 AJAX 请求。实现此功能非常简单，只需由服务器发送一个响应标头即可。
- CORS浏览器支持情况如下图：
![](http://images.cnitblog.com/blog/159097/201401/06082033-9233ced009a644f9af909baca57a72b7.png)
![](http://images.cnitblog.com/blog/159097/201401/06082103-f46e697e71884240b7200f8713697fbc.png)
　　喜闻乐见、普大喜奔的支持情况，尤其是在**移动终端**上，除了opera Mini；
　　PC上的现代浏览器都能友好的支持，除了IE9-，不过前端工程师对这种情况早应该习惯了...
![](http://images.cnitblog.com/blog/159097/201401/06082134-b13e1416aa0b453f89f71ad953807361.jpg)
# CORS启航
　　假设我们页面或者应用已在 http://www.test1.com 上了，而我们打算从 http://www.test2.com 请求提取数据。一般情况下，如果我们直接使用 AJAX 来请求将会失败，浏览器也会返回“源不匹配”的错误，"**跨域**"也就以此由来。
　　利用 CORS，http://www.test2.com 只需添加一个标头，就可以允许来自 http://www.test1.com 的请求，下图是我在PHP中的 hander() 设置，**“*”号表示允许任何域向我们的服务端提交请求**：
![](http://images.cnitblog.com/blog/159097/201401/06082159-b1a102a3ce0e49e7841f76675236e408.png)
也可以设置指定的域名，如域名 http://www.test2.com ，那么就允许来自这个域名的请求：
![](http://images.cnitblog.com/blog/159097/201401/06082227-10f22c4f75094faeb78255509199aed5.png)
　　当前我设置的header为“*”，任意一个请求过来之后服务端我们都可以进行处理&响应，那么在调试工具中可以看到其头信息设置，其中见红框中有一项信息是“**Access-Control-Allow-Origin：*** ”，表示我们已经启用CORS，如下图。
PS：由于demo都在我厂的两台测试机间完成，外网也不能访问，所以在这就不提供demo了，见谅![](http://images.cnitblog.com/blog/159097/201401/06082301-706148cc7ee142898b8eb25fcd62c406.jpg)
![](http://images.cnitblog.com/blog/159097/201401/06082242-c70a37f237ed48c4a60d33fccfd467fb.png)
 　　简单的一个header设置，一个支持跨域&POST请求的server就完成了:)
　　当然，如果没有开启CORS必定失败的啦，如下图：
![](http://images.cnitblog.com/blog/159097/201401/06110611-953e174761e545ad94da63fceee9afb4.jpg)
# 问题&小结
- 刚刚说到的兼容性。CORS是W3C中一项较新的方案，所以部分浏览器还没有对其进行支持或者完美支持，详情可移至 [http://www.w3.org/TR/cors/](http://www.w3.org/TR/cors/)
- 安全问题。CORS提供了一种跨域请求方案，但没有为安全访问提供足够的保障机制，如果你需要信息的绝对安全，不要依赖CORS当中的权限制度，应当使用更多其它的措施来保障，比如OAuth2。
　　自认为的cors使用场景：
- cors在移动终端支持的不错，可以考虑在移动端全面尝试；PC上有不兼容和没有完美支持，所以小心踩坑。当然浏览器兼容就是个伪命题，说不准某个浏览器的某个版本就完美兼容了，说不准就有点小坑，尼玛伤不起！～
- jsonp是get形式，承载的信息量有限，所以信息量较大时CORS是不二选择；
- 配合新的JSAPI(fileapi、xhr2等)一起使用，实现强大的新体验功能。
