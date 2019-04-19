# Nginx 缓存引发的跨域惨案 - 文章 - 伯乐在线
原文出处： [网易乐得 - 张捷](http://tech.lede.com/2017/07/21/rd/server/crossdomain/)
### 1. 前言
贵金属wap版直播间上线后，偶尔有用户反馈，在进入wap直播间的时候，出现空白页面，但是重新刷新又可以正常显示了。我们曾一度认为是网络请求异常或兼容问题，直到开发PC版直播间，在进行调试中，同样遇到了“白屏”问题，才引起了足够重视，并进行了问题跟踪与分析。现在跟大家分享一下，这种偶然现象出现的原因。
我们的直播间落地页在fa.163.com 系统，而直播间内容，是通过 向直播间系统 qz.fa.163.com 发起Ajax请求获取的。在出现“白屏”的时候，可以通过浏览器的调试窗口，可以看到出现下面的报错
![p1](http://jbcdn2.b0.upaiyun.com/2017/08/a12fbf3c168315a7d1b5b5a2ca4b6129.png)
### 2. 问题分析
从上述错误提示文案中可以看到，问题首先和 **跨域** 有关。
> 
**何为跨域**
从字面上理解为“跨域名”，
浏览器不能执行其他网站的脚本，然而，跨域不仅仅局限于域名这一项。只要协议、域名、端口有任何一个不同，都被当作是不同的域。 这是由于>同源策略的限制，从一个域上加载的脚本不允许访问另外一个域的文档属性。虽然在浏览器中，<script>、<img>、<iframe>、<link>等标签都>可>以加载跨域资源，而不受同源限制，但浏览器会限制脚本中发起的跨域请求。比如，使用 XMLHttpRequest 对象和Fetch发起 HTTP 请求就必须遵守同源策略。
**同源策略/SOP（Same origin policy）**是一种约定，它是浏览器最核心也最基本的安全功能，如果缺少了同源策略，浏览器很容易受到XSS、CSFR等攻击。SOP要求两个通讯地址的协议、域名、端口号必须相同，否则两个地址的通讯将被浏览器视为不安全的，并被block下来。
举个例子：从贵金属主站 [http://fa.163.com](http://fa.163.com) 发起请求访问以下url：
![p6](http://jbcdn2.b0.upaiyun.com/2017/08/b26b0c19bf035ea217c4df11feb23328.png)
**解决跨域**
在实际应用中有多种方式来解决跨域问题，相信在实践中都会用到其中的某些方案：
1.JSONP （无状态连接，不能获悉连接状态和错误事件，而且只能走GET的形式）
2.iframe形式
3.服务器代理
页面直接向同域的服务端发请求，服务端进行跨域处理或爬虫后，再把数据返回给客户端页。
4.CORS
CORS(Cross-Origin Resource Sharing)跨域资源共享，定义了必须在访问跨域资源时，浏览器与服务器应该如何沟通。CORS背后的基本思想就>是使用自定义的HTTP头部让浏览器与服务器进行沟通，从而决定请求或响应是应该成功还是失败。目前，所有浏览器都支持该功能，IE浏览器不能低>于IE10。整个CORS通信过程，都是浏览器自动完成，不需要用户参与。对于开发者来说，CORS通信与同源的AJAX通信没有差别，代码完全一样。浏>览器一旦发现AJAX请求跨源，就会自动添加一些附加的头信息，有时还会多出一次附加的请求，但用户不会有感觉。
CORS方式实现：
浏览器在发出CORS请求时会在头信息之中增加一个Origin字段；
后端返回代码中增加三个字段


```
header(“Access-Control-Allow-Origin”:“”);           // 必选 允许所有来源访问
header(“Access-Control-Allow-Credentials”:“true”);  //可选 是否允许发送cookie
header(“Access-Control-Allow-Method”:“POST,GET”);   //可选 允许访问的方式
```
nginx是一个高性能的web服务器，常用作反向代理服务器。nginx作为反向代理服务器，就是把http请求转发到另一个或者一些服务器上。通过把本地一个url前缀映射到要跨域访问的web服务器上，就可以。
为了解决跨域问题，我们选择方案d ， 在直播间的过滤器中，统一添加了如下代码：


```
<a href="http://www.jobbole.com/members/wx610506454">@Override</a>
   public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
    // 加入响应头
    String origin = request.getHeader("Origin");
    if("http://fa.163.com".equals(origin) || "https://fa.163.com".equals(origin) ) {
        response.addHeader("Access-Control-Allow-Origin", origin);
        response.addHeader("Access-Control-Allow-Credentials", "true");
    }
    return true;
}
```
从错误提示文案中，我们还可以看到错误提示的关键点 “[http://fa.163.com”](http://fa.163.com%E2%80%9D) that is not equal to the supplied origin. Origin ‘[https://fa.163.com](https://fa.163.com)‘ is therefore not allowed access.
目前我们的系统同时支持http访问和https访问，但是为什么使用 http访问 ，返回的header中却是 https 协议呢？
通过多次模拟，确认出现问题的请求中，Request URL使用的协议和 response返回的headers中的 Access-Control-Allow-Origin 中的 协议确实不一致，且还有一个特性，X-Cached 为 HIT，如下图：
![p2](http://jbcdn2.b0.upaiyun.com/2017/08/c7581ea997ecc2922a2e162ed57aef52.jpg)
命中了缓存的请求，出现了协议不一致？
突然想到，这个接口，我们配置了nginx 缓存，那必然和nginx缓存有关了。
> 
**Nginx 缓存**
Nginx (engine x) 是一个高性能的HTTP和反向代理服务器。
首先从源服务器（内部网络上的web服务器）上获取内容，然后把内容返回给用户，同时，也会把内容保存到代理服务器上一份，这样日后再接收同样的信息请求时，他会把本地缓存里的内容直接发给用户，以此减少后端web服务器的压力，提高响应速度。这其实就是缓存服务器所实现的功能。如下图所示。
![p3](http://jbcdn2.b0.upaiyun.com/2017/08/d93182a8d7f7fe0c1129b5c36ab6aad8.png)
进入直播间后，首先需要查询直播内容是否有更新，而这个接口客户端会以5s间隔轮询，为了减少tomcat的压力，我们配置了nginx缓存。配置如下
![p4](http://jbcdn2.b0.upaiyun.com/2017/08/ecf29cf6d154c4ad3cfe0f5d4acfa2a4.png)
其中：


```
proxy_cache_methods： 用来设置HTTP哪些方法会被缓存，直播间接口配置了GET、HEAD、POST；
proxy_cache_valid:    用来设置对不同HTTP状态码的不同缓存时间。直播间接口配置了对于 返回值为200的状态码，缓存5秒；
proxy_cache_min_uses: 用来设置多少次访问后，应答值会被缓存，配置为3次；
proxy_cache_key:      设置Web缓存的key
proxy_cache:          用来设置哪个缓存区将被使用，并定义缓存区的名称
```
通过上述配置，我们可以看到 proxy_cache_key 配置中，只配置了host + uri + 参数，但没有配置协议，所以无论用http访问，还是https访问，只要被缓存后，返回的内容都是一样的，而不会区分http或https。从而引起了跨域问题。
至此，问题分析完毕。
### 3. 问题解决
跟运维同学沟通后，通过修改nginx配置，将协议类型scheme加入到缓存查找的判断参数中，配置如下。
![p5](http://jbcdn2.b0.upaiyun.com/2017/08/ce493ab2cec7b730fd5801c8cd15fa69.png)
问题得到了解决。
# 4. 总结
上述“惨案” ，是 跨域、nginx缓存、http/https协议 这三种条件同时出现引发的。
如果不涉及跨域，混用 http/https协议 + nginx缓存，其实也是没有问题的。但是一旦出现了跨域使用，必须 在nginx 缓存配置中，配置 scheme + host + uri +　参数。
