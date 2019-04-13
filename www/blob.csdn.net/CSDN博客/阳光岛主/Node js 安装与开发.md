
# Node.js 安装与开发 - 阳光岛主 - CSDN博客

2017年11月02日 10:24:43[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：883



**Node.js 简介**
Node.js是一个Javascript运行环境(runtime)，发布于2009年5月，由Ryan Dahl开发，实质是对Chrome V8引擎进行了封装。Node.js对一些特殊用例进行优化，提供替代的API，使得V8在非浏览器环境下运行得更好。
V8引擎执行Javascript的速度非常快，性能非常好。
Node.js是一个基于Chrome JavaScript运行时建立的平台， 用于方便地搭建响应速度快、易于扩展的网络应用。
Node.js 使用事件驱动， 非阻塞I/O 模型而得以轻量和高效，非常适合在分布式设备上运行数据密集型的实时应用。
Node.js is a JavaScript runtime built on[Chrome's V8 JavaScript engine](https://developers.google.com/v8/). Node.js uses an event-driven, non-blocking I/O model that makes it lightweight and efficient. Node.js' package ecosystem,[npm](https://www.npmjs.com/), is the largest ecosystem of open source libraries in the world.
博客原文请见米扑博客：[Node.js 安装与开发](https://blog.mimvp.com/2017/10/node-js-an-zhuang-yu-kai-fa/)
**Node.js 特性**
Node.js 是为网络而生，使用node，可以做以下开发：
具有复杂逻辑的网站；
基于社交网络的大规模 Web 应用；
Web Socket 服务器；
TCP/UDP 套接字应用程序；
命令行工具；
交互式终端程序；
带有图形用户界面的本地应用程序；
单元测试工具；
客户端 JavaScript 编译器。
此外，Node.js内建了 HTTP 服务器支持，也就是说可以轻而易举地实现一个网站和服务器的组合，这个是它的最大优点。
**Node.js 架构**
![node-js-an-zhuang-yu-kai-fa-01](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-01.png)
Node标准库就不介绍了，后面开发实例当中会来介绍，比如：http，fs，buffer等。。。
介绍以下图上面的几个英文字母：
**V8：**指的是谷歌的一款javascript引擎——Google V8引擎，具体可以查阅其相关资料，据说是目前全宇宙执行javascript最快的引擎。
**Libeio和Libev：**底层的事件驱动和异步式 I/O库，据说也是非常高效的。
**IOCP：**输入输出完成端口机制。
**Libuv：**这个主要是用来适用不同的操作系统的，Node刚开始是没有这个的，刚开始只能运行在linux底下，随着发展为了适用于windows，node团队抽象出Libuv这一层。

**1. 下载 node.js**
官网：[https://nodejs.org/en/download/](https://nodejs.org/en/download/)
下载：[node-v6.11.1.tar.gz](https://nodejs.org/dist/v6.11.1/node-v6.11.1.tar.gz)
**2. 安装 node.js**
tar zxvf node-v6.11.1.tar.gz
cd node-v6.11.1/
./configure --prefix=/usr/local/node
make && make install
**配置环境变量**
vim /etc/profile
添加如下配置：
NODE_HOME=/usr/local/node
export PATH=$NODE_HOME/bin:$PATH
export NODE_PATH=$NODE_HOME/lib/node_modules
**3. 验证安装**
|1
|2
|3
|4
|\# node -v
|v6.11.1
|\# npm -v
|3.10.10
|

**4. Eclipse 插件安装**
1）检查本地是否已安装好node.js
$ which node
/usr/local/bin/node
$ node -v
v6.11.1
$ npm -v
3.10.10
2）安装插件 nodeclipse （安装需要连接VPN）
**方式1（****推荐****，简单便捷****）**
Help —> Eclipse Marketplace —> 搜索"node.js" —> 找到"Nodeclipse xxx"  —>  安装，如下图：
![node-js-an-zhuang-yu-kai-fa-02](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-02.png)
**方式2**
Help —> Install New Software... —> Add...
Name：nodeclipse
Location：http://www.nodeclipse.org/updates
如下图：
![node-js-an-zhuang-yu-kai-fa-03](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-03.png)
OK确认后，选择 "nodeclipse"，如下图：
![node-js-an-zhuang-yu-kai-fa-04](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-04.png)
安装完毕后，重启Eclipse
Ecplise —> Preferences... —> Nodeclipse，如下图：
![node-js-an-zhuang-yu-kai-fa-05](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-05.png)
**6. 测试示例**
新建Node.js Project 项目
![node-js-an-zhuang-yu-kai-fa-06](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-06.png)
hello-world-server.js
|1
|2
|3
|4
|5
|6
|var|http = require(|'http'|);
|http.createServer(|function|handler(req, res) {
|res.writeHead(200, {|'Content-Type'|:|'text/plain'|});
|res.end(|'Hello World\n'|);
|}).listen(1337,|'127.0.0.1'|);
|console.log(|'Server running at http://127.0.0.1:1337/'|);
|

**1）命令行运行**
$node hello-world-server.js
Server running at[http://127.0.0.1:1337/](http://127.0.0.1:1337/)
**2）Eclipse 运行**
右键文件"hello-world-server.js" —> Run as  —>  Node.js Application
![node-js-an-zhuang-yu-kai-fa-07](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-07.png)
浏览器里，输入：[http://127.0.0.1:1337](http://127.0.0.1:1337/)
![node-js-an-zhuang-yu-kai-fa-08](https://blog.mimvp.com/wp-content/uploads/2017/10/node-js-an-zhuang-yu-kai-fa-08.png)
最简单的 Hello world 程序，运行成功！
**最简单的测试代码**
vim hello_mimvp.js
添加一行日志内容：
vim hello_mimvp.js
保存退出运行命令：
$/usr/local/bin/node hello_mimvp.js
hello mimvp.com
**7. 事务处理示例**
本示例意图向读者传递 Node.js 关于 HTTP 处理过程的详实概念。
在不考虑编程语言和环境的情况下，假设您已经知晓通常情况下 HTTP 请求是如何工作的，并且对 Node.js 的 EventEmitters 和 Streams 也已知晓。如果您对他们并不熟悉，通过 API 文档可以快速查阅。
1）创建服务
Node 的网络应用都需要先创建一个网络服务对象，这里我们通过 createServer 来实现。
|1
|2
|3
|4
|var|http = require(|'http'|);
|var|server = http.createServer(|function|(request, response) {
|// handle your request
|});
|

传入 createServer 的 function 在每次 HTTP 请求时都将被调用执行，因此这个 function 也被称为请求的处理者。
事实上，通过 createServer 返回的 Server 对象是一个 EventEmitter，我们需要做的仅仅是在这里保存这个 server 对象，并在之后对其添加监听器。
|1
|2
|3
|4
|5
|var|http = require(|'http'|);
|var|server = http.createServer();
|server.on(|'request'|,|function|(request, response) {
|// handle your request
|});
|

当 HTTP 请求这个服务时，node 调用请求处理者 function 并传入一些用于处理事务相关的对象：request 和 response。
我们可以非常方便的获得这两个对象。
|1
|2
|3
|4
|5
|var|http = require(|'http'|);
|var|server = http.createServer();
|server.on(|'request'|,|function|(request, response) {
|// handle your request
|}).listen(8080);
|

为了对实际的请求提供服务，在 server 对象上需要调用listen方法。
绝大多数情况你需要传入 listen 你想要服务监听的端口号，这里也存在很多其他的可选方案，参见 API reference。



