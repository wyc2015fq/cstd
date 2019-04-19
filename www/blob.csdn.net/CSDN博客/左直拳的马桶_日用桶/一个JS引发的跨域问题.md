# 一个JS引发的跨域问题 - 左直拳的马桶_日用桶 - CSDN博客
2016年11月16日 19:50:44[左直拳](https://me.csdn.net/leftfist)阅读数：678标签：[javascript																[跨域请求](https://so.csdn.net/so/search/s.do?q=跨域请求&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
忽然遇上跨域错误。
我们有张页面，使用了EXT.js，在本地运行正常，部署到服务器上，出不来数据。F12调试，提示有跨域错误？
```
XMLHttpRequest cannot load http://192.168.0.22:8080/cas/login?service=http%3a%2f%2f192.168.0.22%2fGJ.W…El%3d0%26numberfield-1024-inputEl%3d20%26page%3d1%26start%3d0%26limit%3d20. No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'http://192.168.0.22' is therefore not allowed access.
```
这应该就是传说中的跨域错误了吧。怎么回事呢？
[http://192.168.0.22:8080/cas](http://192.168.0.22:8080/cas)，这个网址一望就知，是个单点登录网址（以下简称为SSO站点）。而我们这个页面是部署在站点A下。难道要在SSO下进行配置？想到这里，我感到一阵恐惧。因为SSO是用java弄的，搞死人。
但很快就否定了这个原因。因为类似的页面部署在站点B下，也是用SSO来登录，却不见有啥问题。
难道问题出在站点A下？翻遍了各种配置，没发现有啥问题。
后来想起，这个页面用到了EXT.JS，而这个EXT.JS的各种JS文件，都部署在一个虚拟目录里，而这个虚拟目录，则属于站点B。应该就是这个原因吧，删除这个虚拟目录，重新部署到网站根目录下，与站点A、B并列，如图，问题解决。
![这里写图片描述](https://img-blog.csdn.net/20161116194716916)
![这里写图片描述](https://img-blog.csdn.net/20161116194733026)
我猜测是EXT.JS引起了跨域错误。对于B来讲，它在自己域下，没问题；而对于A来讲，不同域（严格来说，是不同子域），请求不同域下的JS，引发了跨域问题。
