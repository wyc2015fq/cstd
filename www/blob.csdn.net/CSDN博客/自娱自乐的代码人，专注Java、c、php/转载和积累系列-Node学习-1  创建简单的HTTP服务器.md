# 转载和积累系列 - Node学习 - 1. 创建简单的HTTP服务器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月25日 13:48:11[initphp](https://me.csdn.net/initphp)阅读数：892







- NodeJS下载：[http://nodejs.org](http://nodejs.org)
- 在windows下安装完毕之后，直接在cmd中直接运行node进入命令行模式

![](https://img-my.csdn.net/uploads/201207/25/1343195528_2102.jpg)


- 输出一行Hello World

![](https://img-my.csdn.net/uploads/201207/25/1343195599_5945.jpg)


- 创建一个简单的Node Http服务器，hello.js

```java
var http = require("http");
var httpInfo = function (req, res) {
	res.writeHead(200, {'Content-Type' : 'text/html'});
	res.write("Hello");
	res.end("Hello");
}
http.createServer(httpInfo).listen(8080);
console.log("Http Server is Listening at port 8080");
```
- 运行node

![](https://img-my.csdn.net/uploads/201207/25/1343195746_6693.jpg)


- 通过浏览器 [http://127.0.0.1:8080](http://127.0.0.1:8080)访问一下

![](https://img-my.csdn.net/uploads/201207/25/1343195811_8469.jpg)






