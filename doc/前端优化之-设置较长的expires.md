# 前端优化之-设置较长的expires 

 

前端优化中很重要的一项是设置一个较长的过期时间，例如yahoo的expires设的是10年，page  speed推荐一个月以上。设expires的目的就是让没有更新的资源不应该产生http请求，如果强制产生请求则返回304，减少服务器压力和降低带宽。exprires属于服务器优化范畴，需要修改服务器配置并重启。

**http header相关知识**

expires：指定缓存到期GMT的绝对时间，如果设了max-age，max-age就会覆盖expires。如果expires到期需要重新请求。

last-modified：WEB  服务器认为对象的最后修改时间，比如文件的最后修改时间，动态页面的最后产生时间。

ETag：就是一个对象（比如URL）的标志值，就一个对象而言，比如一个 html 文件，如果被修改了，其 Etag  也会别修改， 所以，ETag 的作用跟 Last-Modified 的作用差不多，主要供 WEB 服务器  判断一个对象是否改变了。如果有多个服务器需要考虑Etag同步问题。
比如前一次请求某个 html  文件时，获得了其 ETag，当这次又请求这个文件时， 浏览器就会把先前获得的 ETag 值发送给  WEB 服务器，然后 WEB 服务器会把这个 ETag  跟该文件的当前 ETag 进行对比，然后就知道这个文件有没有改变了。

Cache-Control:
这个是http  1.1中为了弥补 Expires 缺陷新加入的，现在不支持http 1.1的浏览器已经很少了。
max-age: 指定缓存过期的相对时间秒数，max-ag=0或者是负值，浏览器会在对应的缓存中把Expires设置为1970-01-01  08:00:00 。
s-maxage: 类似于max-age，只用在共享缓存上，比如proxy.
public: 通常情况下需要http身份验证的情况，响应是不可cahce的，加上public可以使它被cache。
no-cache: 强制浏览器在使用cache拷贝之前先提交一个http请求到源服务器进行确认。这对身份验证来说是非常有用的,能比较好的遵守  (可以结合public进行考虑)。它对维持一个资源总是最新的也很有用，与此同时还不完全丧失cache带来的好处)，因为它在本地是有拷贝的，但是在用之前都进行了确认，这样http请求并未减少，但可能会减少一个响应体。
no-store:  告诉浏览器在任何情况下都不要进行cache，不在本地保留拷贝。
must-revalidate: 强制浏览器严格遵守你设置的cache规则。
proxy-revalidate: 强制proxy严格遵守你设置的cache规则。
用法举例:  Cache-Control: max-age=3600, must-revalidate

cache:使用本地缓存，不发生请求。

304:通过If-Modified-Since  If-Match判断资源是否修改，如未修改则返回304，发生了一次请求，但请求内容长度为0，节省了带宽。

如果有多台负载均衡的服务器，不同服务器计算出的Etag可能不同，这样就会造成资源的重复加载，我看的yahoo首页就没设Etag，估计有这么方面的考虑。

 

下面我的一些分析，不敢说完全正确，只测试了IE6和FF3.5，感兴趣的朋友一块讨论，毕竟浏览器对我来说是个黑洞。

**下面主要说说expires，主要有这四个疑问**

1.设置expires后，请求状态应该是什么样的？是304还是cache？

2.如果资源被浏览器cache了，怎么才能保证资源正确更新，尤其是页面不出错。

3.浏览器是否有差异？

4.过期时间设为多少比较好。

**为了解决上面的疑惑，我用apche做了测试，apache的配置如下：**

**#加载expires模块  LoadModule expires_module modules/mod_expires.so  #取消Etag，去掉Etag带来的干扰  FileETag none  ExpiresActive On  ExpiresDefault "access plus 10 years"  ExpiresByType image/gif "access plus 10 years"  ExpiresByType image/jpeg "access plus 10 years"  ExpiresByType image/png "access plus 10 years"  ExpiresByType text/css "access plus 10 years"  ExpiresByType text/html "access plus 1 seconds"  ExpiresByType text/javascript "access plus 10 years"  ExpiresByType application/x-unknown-content-type "access plus 10 years"  ExpiresByType application/x-javascript "access plus 10 years"**   

 



**测试还需要考虑访问页面的方式，方式不同请求状态也会有差异。打开浏览器清除缓存，然后打开页面进行测试，多次测试结果如下：**

| 打开页面方式              | IE6（httpwatch）                      | FF3.5（httpfox）                      |
| ------------------------- | ------------------------------------- | ------------------------------------- |
| 1. 第一次打开页面         | 200                                   | 200                                   |
| 2. 重启浏览器打开页面     | cache，即时发生资源修改也不会重新请求 | cache，即时发生资源修改也不会重新请求 |
| 3. F5刷新                 | 304，发生修改的资源状态为200          | 304，发生修改的资源状态为200          |
| 4. Ctrl+F5刷新            | 200，强制全新请求                     | 200                                   |
| 5. 后退                   | cache,简单直接地从缓存加载            | cache,简单直接地从缓存加载            |
| 6. 在已访问页面地址栏回车 | cache                                 | cache                                 |

**总结：**

用yahoo首页验证了上面的结论，除yahoo F5刷新页面时IE6下有两个请求状态为cache外（目前没分析到原因），二者基本一致。

把expires设为30天、14天结果页一样。有了上面的测试结果做expires优化心理就有底了，疑问1和3已解决。

第二种方式打开页面，有资源修改也走本地缓存，极有可能出错，问题2最稳妥的办法是修改的资源启用新名称，yahoo的作法是文件名后加版本号，这回增加前端的工作量。目前每次更新脚本库，资源地址都不变，主要为避免重命名后相关应用都需要更新的问题，节省开发的时间。

 expires设2，3天，有点短，稳定的应用的js css  img更新不是很频繁，像yahoo设10年可以理解为永不过期，十年中网站不知道改版多少次了。最低设30天还比较合适。



**参考资料：**

<http://www.360doc.com/content/08/0501/14/41237_1227846.shtml>
<http://www.w3.org/Protocols/rfc2616/rfc2616-sec13.html#sec13.3.4>

<http://www.mnot.net/cache_docs/>
杨建：网站加速--Cache为王篇  <http://www.itlearner.com/article/4421>

Caching in HTTP <http://www.w3.org/Protocols/rfc2616/rfc2616-sec13.html>
Header  Field Definitions  <http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html>

age <http://labs.apache.org/webarch/http/draft-fielding-http/p6-cache.html#header.age>

Age Calculations  <http://www.w3.org/Protocols/rfc2616/rfc2616-sec13.html>

 

 