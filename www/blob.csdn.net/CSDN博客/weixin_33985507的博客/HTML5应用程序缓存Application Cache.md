# HTML5应用程序缓存Application Cache - weixin_33985507的博客 - CSDN博客
2015年02月04日 15:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**什么是Application Cache**
HTML5引入了应用程序缓存技术，意味着web应用可进行缓存，并在没有网络的情况下使用，通过创建cache manifest文件，可以轻松的创建离线应用。
Application Cache带来的三个优势是：
① 离线浏览
② 提升页面载入速度
③ 降低服务器压力
而且主要浏览器皆以支持Application Cache，就算不支持也不会对程序造成什么影响
**离线存储技术**
HTML5提出了两大离线存储技术：localstorage与Application Cache，两者各有应用场景；传统还有离线存储技术为Cookie。
经过实践我们认为localstorage应该存储一些非关键性ajax数据，做锦上添花的事情；
Application Cache用于存储静态资源，仍然是干锦上添花的事情；
而cookie只能保存一小段文本（4096字节）；所以不能存储大数据，这是cookie与上述缓存技术的差异之一，而因为HTTP是无状态的，服务器为了区分请求是否来源于同一个服务器，需要一个标识字符串，而这个任务就是cookie完成的，这一段文本每次都会在服务器与浏览器之间传递，以验证用户的权限。
所以Application Cache的应用场景不一样，所以使用也不一致。
**Application Cache简介**
Application Cache的使用要做两方面的工作：
① 服务器端需要维护一个manifest清单
② 浏览器上只需要一个简单的设置即可
```
<html  manifest="demo.appcache">
```
以例子做说明：
```
CACHE MANIFEST
CACHE:
# 需要缓存的列表
style1.css
1.jpg
01.js
http://localhost/applicationcache/02.js
http://localhost/applicationcache/zepto.js
NETWORK:
# 不需要缓存的
4.jpg
FALLBACK:
# 访问缓存失败后，备用访问的资源，第一个是访问源，第二个是替换文件*.html /offline.html
2.jpg/3.jpg
```
首先我这里报了一个错：
```
Application Cache Error event: Manifest fetch failed (404)
```
这个错误的原因是：manifest 文件需要配置正确的 MIME-type，即 "text/cache-manifest"。必须在 web 服务器上进行配置，不同的服务器不一样
![](https://images0.cnblogs.com/blog/294743/201502/041311370316730.png)
```
\APPLICATIONCACHE
    01.js
    02.js
    1.jpg
    2.jpg
    3.jpg
    4.jpg
    demo.appcache
    index.html
    style1.css
    style2.css
    web.config
    zepto.js
```
这样一来便可以离线应用了，这个时候就算断网了，那些文件依旧能访问
![](https://images0.cnblogs.com/blog/294743/201502/041319200625689.png)
这里有一点值得注意，比如这里不带/index.html他会将“applicationcache/”缓存，其实这个就是index.html
```
manifest 文件可分为三个部分：
CACHE MANIFEST - 在此标题下列出的文件将在首次下载后进行缓存
NETWORK - 在此标题下列出的文件需要与服务器的连接，且不会被缓存
FALLBACK - 在此标题下列出的文件规定当页面无法访问时的回退页面（比如 404 页面）
```
![](https://images0.cnblogs.com/blog/294743/201502/041350519689411.png)
如图所示，HTML5定义了几个事件点，但是我们一般不会主动使用js去操作什么，大多数情况下，我们完全依赖浏览器的处理即可。
**尺寸限制**
Application Cache的尺寸限制统一在5M，我这里做一个测试：
![](https://images0.cnblogs.com/blog/294743/201502/041358312812766.png)
如所示，两个css文件依旧超过了5M这个时候
```
Document was loaded from Application Cache with manifest http://localhost/applicationcache/demo.appcache
index.html:1 Application Cache Checking event
index.html:6 GET http://localhost/applicationcache/style2.css net::ERR_FAILED
index.html:1 Application Cache NoUpdate event
index.html:11 GET http://localhost/applicationcache/2.jpg net::ERR_FAILED
index.html:12 GET http://localhost/applicationcache/3.jpg net::ERR_FAILED
```
如所示，style2已经不能缓存了，这个会造成什么问题呢？
比如我A频道维护了自己的Application Cache，B频道也维护了自己的，这个时候A频道如果使用达到了一个峰值，会导致B频道所有的缓存失效，所以：
```
建议Application Cache，存储公共资源，不要存储业务资源
```
**一些问题**
由更新机制来说，首次更新manifest时，因为页面加载已经开始甚至已经完成，缓存更新尚未完成，浏览器仍然会使用过期的资源；浏览器是当Application Cache有更新时，该次不会使用新资源，第二次才会使用。这个时候update事件中执行window.reload事件。
```
window.applicationCache.addEventListener("updateready", function(){
    window.location.reload()
});
```
由上例可以知道，缓存的不只是显示定义的文件，比如上例中的applicationcache/时便会默认保存index.html为映射的数据，并且包含demo.appcache文件，很多时候会遇到一次文件更新线上老是不更新，这个时候随便在manifest配置文件中做一点修改即可更新。
比如我们将这里代码做一个改变：
```
<html  manifest="demo.appcache">
=>
<html  manifest="demo1.appcache">
```
这个时候如果不做demo.appcache的更新的话，缓存将不会更新，原因是index.html被缓存了，检测的仍然是原manifest清单
各个页面统一管理自己的manifest清单，意思是a页面配置了common.js，b页面也配置了common.js，意思是a页面更新后，b页面的manifest不更改的话，b页面依旧读取的是老版本的文件，这个有一定道理却也有一定浪费，需要公共页面做处理。
**总结**
从可用性与易用性来说，Application Cache是值得使用的，但是最好是做静态资源的缓存，真正要实现离线应用还得花更多的功夫呢！
参考：http://www.w3school.com.cn/html5/html_5_app_cache.asp
文中有何错误、不足请指出。
![](http://service.t.sina.com.cn/widget/qmd/2013900244/b48b0477/1.png?rnd=1406031295210)
