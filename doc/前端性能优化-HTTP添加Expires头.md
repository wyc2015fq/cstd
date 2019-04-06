# 前端性能优化-HTTP添加Expires头

#  

# 简介

# HTTP中的Cache-Control和Expires

Web页面设计中，建少HTTP请求可以提高页面响应速度。浏览器在第一次访问页面时下载的资源会缓存起来，第二次访问时会判断在缓存中是否已有该资源并且有没有更新过，如果已有该资源且没有更新过，则去缓存去取，这样减少了下载资源的时间。原理上是通过HTTP  Rquest Header中的 if-modified-since 和Response  Headers中的last-modified来实现（还有一对组合If-None-Match和Etag，类似，但有细微差别，不属于本文重点讨论范围，如有兴趣可参考链接Google  Developers 优化性能指导），HTTP请求把if-modified-sincede  时间传给服务端，服务端把last-modified时间与之对比，如果相同，则意味着文件没有改动，则返回304，浏览器则从缓存中获取资源，无需下载。

虽然这种方法减少了已缓存资源的下载时间，但是仍然发起了一次http请求。仍然还有更激进的做法来节省也页面加载的时间，就是免去该资源的http请求。即本文的主角，HTTP的Expires和Cache-Control。



![img](https://upload-images.jianshu.io/upload_images/3020614-92c2f1a1b34d9079.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/871)

tmall home page

## Cache-Control的使用机制

Cache-Control属性值是在server端配置的，不同的服务器有不同的配置，web服务器 apache、nginx、IIS,应用服务器tomcat等配置都不尽相同；以Apache为例，在http.conf中做如下配置：



![img](https://upload-images.jianshu.io/upload_images/3020614-a78af7255ef94edf.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/746)

第一段代码是指jgp/jepg/png/gif/ico等类型的文件缓存1年。单位是秒，你可以自己算算。第二段代码是指css和js这种文件类型缓存1个月。通常来说一些长期不变的图片比如logo，背景图片，字体，icon小图标等变化一般不会频繁，可以设的久一点。可以设个一年半载，具体设多久见仁见智了。新闻，广告等频繁更新的图片，不用缓存。css，js文件更新的周期会短一点，可以设置一周或者一个月。

如果用户浏览器缓存了页面的资源，你又想让用户更新怎么办呢？你可以通过修改该资源的名称来实现。名字改了，浏览器会当做不同的资源，这样就可以实现了。在大型项目中不可能频繁的更新文件名，要通过全局的配置文件版本的方法。你可以想想有没有更好的方式。详细的Cache-Control可以参考这篇文章How  to use Cache-Control

## Expires的使用机制

expires也是需要在服务端配置（具体配置也根据服务器而定），expires添加的是该资源过期的日期，浏览器会根据该过期日期与客户端时间对比，如果过期时间还没到，则会去缓存中读取该资源，如果已经到期了，则浏览器判断为该资源已经不新鲜要重新从服务端获取。通过这种方式，可以实现直接从浏览器缓存中读取，而不需要去服务端判断是否已经缓存，避免了这次http请求。值得注意的是expires时间可能存在客户端时间跟服务端时间不一致的问题。所以，建议expires结合Cache-Control一起使用，大型网站中一起使用的情况比较多见。

## 实际测试效果

未使用expires和cache-control的情况（已经访问过，资源已经缓存）：

我使用chrome的network来查看页面加载的情况，利用Live HTTP Headers这个google浏览器插件来监听http发起请求的状况。

打开网址http://stevesouders.com/hpws/expiresoff.php（一个yahoo！工程师的测试demo，拿来用下），注意是打开；（特别注意，刷新的话无论你是否缓存浏览器都会重新发起HTTP请求来判断是否更新）



![img](https://upload-images.jianshu.io/upload_images/3020614-3221f616b68252c3.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/913)

监听器插件的图片



![img](https://upload-images.jianshu.io/upload_images/3020614-8b30ba114d173aca.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/980)

可以看出虽然是304，但是实际上发起了HTTP请求。

使用Expires和Cahce-Control的情况（已经访问过，资源已经缓存）

打开网址http://stevesouders.com/hpws/expireson.php（一个yahoo！工程师的测试demo，拿来用下）。



![img](https://upload-images.jianshu.io/upload_images/3020614-dd86287fec34cd2f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/949)



可以看出图片都是来自缓存，time为0也可以看出没有发起http请求。



![img](https://upload-images.jianshu.io/upload_images/3020614-ed6ef31c9448e50f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/726)



上图可以看出，没有发起图片的http请求。这样在图片非常多的网站中，大大减少了http的请求。从而大幅提高了网站的性能。这种方法不仅仅针对图片，对css，js，font，等等都适用。现在在阿里巴巴的比较前沿的产品中都可以看到使用expires和cache-control的身影。使用此方法，可以提高20%-30%的网站性能。