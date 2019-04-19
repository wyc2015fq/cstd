# Ajax主要用来完成哪些工作？ - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月31日 13:57:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：237
AJAX 是与服务器交换数据并更新部分网页的艺术，在不重新加载整个页面的情况下。
它使用 JavaScript 在 web 浏览器与 web 服务器之间来发送和接收数据。
ajax主要用于在页面内容加载完成后“异步”的向服务器发送和接收请求。前端工程师不仅要完成后台传过来数据在页面上的显示，也要负责从页面向后端发送各种请求。
例子;当您在谷歌的搜索框输入关键字时，JavaScript 会把这些字符发送到服务器，然后服务器会返回一个搜索建议的列表。
AJAX请求是异步执行的，也就是说，要通过回调函数获得响应。
在现代浏览器上写AJAX主要依靠`XMLHttpRequest`对象：
可以使用jsonp跨域加载数据
html5可作cors  全称Cross-Origin Resource Sharing，是HTML5规范定义的如何跨域访问资源。
跨域能否成功，取决于对方服务器是否愿意给你设置一个正确的`Access-Control-Allow-Origin`，决定权始终在对方手中。
## 艺搜参考
[http://www.w3school.com.cn/ajax/index.asp](http://www.w3school.com.cn/ajax/index.asp)
[http://www.zhihu.com/question/19905190](http://www.zhihu.com/question/19905190)
[https://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/001434499861493e7c35be5e0864769a2c06afb4754acc6000](https://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/001434499861493e7c35be5e0864769a2c06afb4754acc6000)
