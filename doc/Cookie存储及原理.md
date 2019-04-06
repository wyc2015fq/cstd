# Cookie存储及原理

 																				2018年08月26日 15:23:22 					[chao09_01](https://me.csdn.net/chao821) 						阅读数：642 						 																															

 									

​                   					 							 					                   					 					版权声明：本文为博主原创文章，未经博主允许不得转载。					https://blog.csdn.net/chao821/article/details/82080732				

​       Cookie是一小段文本信息，伴随着用户请求在 Web  服务器和浏览器之间传递，其大小限制在4k左右（不同浏览器略有不同）。它是「浏览器」提供的一种机制，它将document 对象的 cookie  属性提供给 JavaScript。可以使用JavaScript来创建和取回 cookie 的值，因此我们可以通过document.cookie访问它。

## 一、Cookie的格式

​       因为浏览器提供了 `cookie 属性`给 JavaScript，所以可以通过 `document.cookie` 来访问这个页面中的cookie。

![img](https://img-blog.csdn.net/20180826150552978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoYW84MjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​      这是一串字符串，仔细观察，我们可以发现规律。每个 cookie 都以名/值对的形式，即 `name=value`，名称和值都必须是`URL`编码的，且两对`cookie`间以`分号和空格`隔开。（ps：千万不要忘了空格，特别是在获取某个 cookie 时）

 

## 二、Cookie的存储位置

内存cookie，是指没有设在cookie的Expires的属性，此时cookie将停留在客户端的内存中。

硬盘cookie，是指在你设置了cookie的Expires属性，此时cookie将保存到你的硬盘上。

 

## 三、Cookie的设置（可通过浏览器或者服务器端进行设置）

服务器端设置

​         服务器通过发送一个名为 Set-Cookie 的HTTP头来创建一个cookie，作为 Response Headers  的一部分。如下图所示，每个Set-Cookie 表示一个  cookie（如果有多个cookie,需写多个Set-Cookie），每个属性也是以名/值对的形式（除了secure），属性间以分号加空格隔开。格式如下：

```html
Set-Cookie: name=value[; expires=GMTDate][; domain=domain][; path=path][; secure]
```

![img](https://img-blog.csdn.net/20180826151340299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoYW84MjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​        注意，通过 Set-Cookie 指定的可选项(域、路径、失效时间、secure标志)只会在「浏览器端」使用，它们都是服务器给浏览器的指示，以指定何时应该发送cookie。这些参数不会被发送至服务器端，只有name和value才会被发送。

 浏览器端设置

​        浏览器端设置cookie的格式和Set-Cookie头中使用的格式一样。如下：

```html
document.cookie = "name=value[; expires=GMTDate][; domain=domain][; path=path][; secure]"
```

 

## 四、Cookie用途

​      Cookie 可以作为全局变量，这是它最大的一个优点。它最根本的用途是 Cookie 能够帮助 Web 站点保存有关访问者的信息，以下列举cookie的几种小用途。

1）保存用户登录信息。这应该是最常用的了。当您访问一个需要登录的界面，例如微博、百度及一些论坛，在登录过后一般都会有类似"下次自动登录"的选项，勾选过后下次就不需要重复验证。这种就可以通过cookie保存用户的id。

2）创建购物车。购物网站通常把已选物品保存在cookie中，这样可以实现不同页面之间数据的同步(同一个域名下是可以共享cookie的)，同时在提交订单的时候又会把这些cookie传到后台。

3）跟踪用户行为。例如百度联盟会通过cookie记录用户的偏好信息，然后向用户推荐个性化推广信息，所以浏览其他网页的时候经常会发现旁边的小广告都是自己最近百度搜过的东西。这是可以禁用的。