# HTTP协议详解 - 星爷2401 - CSDN博客
2016年12月01日 08:55:39[星爷2401](https://me.csdn.net/leifukes)阅读数：3632标签：[http协议																[前端																[html																[css																[Cookie](https://so.csdn.net/so/search/s.do?q=Cookie&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=前端&t=blog)](https://so.csdn.net/so/search/s.do?q=http协议&t=blog)
个人分类：[http协议](https://blog.csdn.net/leifukes/article/category/6550157)
凡是搞WEB开发的每天都会和HTTP打交道，学过html，js，css基本上都能够完成大多数的网页的开发，并且有点编程基础的人一学就会。然后你就可以去应聘前端工程师了，但是要想真正做好web开发，只会界面的构建是不够的，最基本的http协议还是要知道的，否则你的技术水平不会提高的太多，当然你的待遇也就那样，所以有必要对http协议有一个深入的了解，从而进一步提升自己。
下面我将从几个方面总结一下http的一些内容。
1、http协议报头字段总结。
2、http状态码及解决办法
一、http协议报头字段总结。
常见的请求消息头
1、Cache_Control  网页的缓存是有http消息头中的Cache_Control字段控制的，一般常用的取值有：默认值private ，public，no-cache，max-age，must-revalidation，no-store等，下面详细介绍一下每个字段的作用：
（1）打开浏览器，新建一个窗口。
public ： 浏览器不会发送请求，呈现来自缓存的页面。
private：浏览器重新发送请求。
no-cache/no-store ： 浏览器重新发送请求。
max-age ： max-age=xxx，xxx秒后浏览器重新发送请求。
must-revalidation：浏览器重新发送请求。
（2）在地址栏输入网址然后回车：
private，no-cache/no-store，must-revalidation，浏览器重新发送请求
max-age ： max-age=xxx，xxx秒后浏览器重新发送请求。
（3）界面刷新的时候：
所有的都会重新发送请求。
（4）点击后退：
public，private，must-revalidation，会呈现出来自缓存的页面。
no-cache/no-store ，重新加载信息。
max-age： max-age=xxx，xxx秒后浏览器重新发送请求。
2、Cookie  ：Cookie是服务器传给客户端的一些数据，客户端访问服务器的时候会传给服务器，服务端用于区分当前账号信息等等操作。存在浏览器的一段文本文件，不包含任何可执行的代码，通过服务器set-Cookie可以设置相应的cookie，它主要包含以下几个字段：
chrome浏览器 cookies字段截图
![](https://img-blog.csdn.net/20161205114544054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（1）Name Cookie的名称
（2）Value Cookie的值
（3）Domain 可以访问此Cookie的Host名称。
（4）Path Cookie的有效范围，如果path = / 则代表在整个Host当中，Cookie都是有效的
（5）Express/Max-Age 过期时间，如果设置一个值为过期时间，超过这个时间，Cookie是失效了，不会传个服务器，如果没有设置时间，那么cookie将和session，关闭浏览器以后（整个浏览器退出），Cookie失效。
（6）Size
 Cookie的大小
（7）HTTP
 如果HTTP设置为true，满足Cookie的httponly属性，那么js将不会读取到cookie的内容，能有效防止XSS攻击。
（8）Secure
 该选项只是一个标记，没有其他的值。


