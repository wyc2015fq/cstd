# Macaca(一) - 环境配置 - zhusongziye的博客 - CSDN博客





2018年04月10日 20:16:55[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：209








Macaca是阿里提供的一套自动化测试框架，目前已开源。

花了两三个小时研究了一下Macaca的实现原理。因为很好奇它与appium、selenium有啥区别。

实现原理本质上与selenium的webdriver是一样的。appium也是使用的webdriver,只不过添加了一些app特别的属性和操作。

都是使用W3C标准协议，通过启动服务与客户端代码之间进行http通信，执行相应的命令，并返回执行结果。python客户端中使用requests来发送请求。

只不过Macaca将app和web的自动化合并到一起了。web浏览器、IOS、Android端都支持、web application也支持。

要运行哪个终端的自动化，都需要事先安装一个客户端环境。

比如要执行安卓设备app自动化，需要安装好安卓自动化运行环境。

web端 - chrome浏览器，也需要事先安装好chrome自动化环境 ，即chrome驱动和chrome浏览器。

由Macaca自己来启动服务端，与appium的机制一样。

客户端支持：js、java、python.  客户端与服务器进行连接，并发送命令。Server-Client模式。

安装步骤 ：

1、安装 Node.js请安装 Node.js v6.0 或者更高版本，装好 Node.js 后命令行里就已经集成了 npm 工具，为了提高安装模块的速度，请使用国内的 cnpm。Macaca安装。必须先安装Node.js。

2、命令行工具全局安装

$ npm i -g macaca-cli

如果看到如下可爱的



