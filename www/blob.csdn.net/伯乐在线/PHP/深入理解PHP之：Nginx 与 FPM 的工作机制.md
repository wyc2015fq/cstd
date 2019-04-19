# 深入理解PHP之：Nginx 与 FPM 的工作机制 - 文章 - 伯乐在线
原文出处： [Hujun（@仁风H）](http://zhuanlan.zhihu.com/p/20694204)
网络上有很多关于如何配置 Nginx + FPM 的文章，但它们更多从操作的角度出发，告诉我们怎么做，但却没有告诉我们为什么要这么做，本文从 Nginx 与 FPM 的工作机制出发，探讨配置背后的原理，让我们真正理解 Nginx 与 PHP 是如何协同工作的。
要说 Nginx 与 PHP 是如何协同工作的，首先得说 CGI (Common Gateway Interface) 和 FastCGI 这两个协议。
CGI 是 Web Server 与后台语言交互的协议，有了这个协议，开发者可以使用任何语言处理 Web Server 发来的请求，动态的生成内容。但 CGI 有一个致命的缺点，那就是每处理一个请求都需要 fork 一个全新的进程，随着 Web 的兴起，高并发越来越成为常态，这样低效的方式明显不能满足需求。就这样，FastCGI 诞生了，CGI 很快就退出了历史的舞台。FastCGI，顾名思义为更快的 CGI，它允许在一个进程内处理多个请求，而不是一个请求处理完毕就直接结束进程，性能上有了很大的提高。
至于 FPM (FastCGI Process Manager)，它是 FastCGI 的实现，任何实现了 FastCGI 协议的 Web Server 都能够与之通信。FPM 之于标准的 FastCGI，也提供了一些增强功能，具体可以参考官方文档：PHP: FPM Installation。
FPM 是一个 PHP 进程管理器，包含 master 进程和 worker 进程两种进程：master 进程只有一个，负责监听端口，接收来自 Web Server 的请求，而 worker 进程则一般有多个 (具体数量根据实际需要配置)，每个进程内部都嵌入了一个 PHP 解释器，是 PHP 代码真正执行的地方，下图是我本机上 fpm 的进程情况，1一个 master 进程，3个 worker 进程：
![ff986d4481f8f809713e98bb2b91a685_b](http://jbcdn2.b0.upaiyun.com/2016/04/4f50b441599fa39f76c6bbd6786182a9.png)
从 FPM 接收到请求，到处理完毕，其具体的流程如下：
- FPM 的 master 进程接收到请求
- master 进程根据配置指派特定的 worker 进程进行请求处理，如果没有可用进程，返回错误，这也是我们配合 Nginx 遇到502错误比较多的原因。
- worker 进程处理请求，如果超时，返回504错误
- 请求处理结束，返回结果
FPM 从接收到处理请求的流程就是这样了，那么 Nginx 又是如何发送请求给 fpm 的呢？这就需要从 Nginx 层面来说明了。
我们知道，Nginx 不仅仅是一个 Web 服务器，也是一个功能强大的 Proxy 服务器，除了进行 http 请求的代理，也可以进行许多其他协议请求的代理，包括本文与 fpm 相关的 fastcgi 协议。为了能够使 Nginx 理解 fastcgi 协议，Nginx 提供了 fastcgi 模块来将 http 请求映射为对应的 fastcgi 请求。
Nginx 的 fastcgi 模块提供了 fastcgi_param 指令来主要处理这些映射关系，下面 Ubuntu 下 Nginx 的一个配置文件，其主要完成的工作是将 Nginx 中的变量翻译成 PHP 中能够理解的变量。
![64519c5be72799e4eb8853b549f912ee_b](http://jbcdn2.b0.upaiyun.com/2016/04/1a7b25fc6585fff2800aa65102237f6e.png)
除此之外，非常重要的就是 fastcgi_pass 指令了，这个指令用于指定 fpm 进程监听的地址，Nginx 会把所有的 php 请求翻译成 fastcgi 请求之后再发送到这个地址。下面一个简单的可以工作的 Nginx 配置文件：
![644b6dae3878fa8caa202531971e768c_b](http://jbcdn2.b0.upaiyun.com/2016/04/9e4110b9c262c489b16a7f1ff52670d5.png)
在这个配置文件中，我们新建了一个虚拟主机，监听在 80 端口，Web 根目录为 /home/rf/projects/wordpress。然后我们通过 location 指令，将所有的以 .php 结尾的请求都交给 fastcgi 模块处理，从而把所有的 php 请求都交给了 fpm 处理，从而完成 Nginx 到 fpm 的闭环。
如此以来，Nginx 与 FPM 通信的整个流程应该比较清晰了吧。
