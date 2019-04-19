# Nginx 教程 （1）：基本概念 - 文章 - 伯乐在线
原文出处： [netguru](https://www.netguru.co/codestories/nginx-tutorial-basics-concepts)   译文出处：[开源中国](https://www.oschina.net/translate/nginx-tutorial-basics-concepts)
![](http://jbcdn2.b0.upaiyun.com/2018/01/e4745b46a3495eb4c48754bd08a62b75.jpg)
## 简介
嗨！分享就是关心！所以，我们愿意再跟你分享一点点知识。我们准备了这个划分为三节的《Nginx教程》。如果你对 Nginx 已经有所了解，或者你希望了解更多，这个教程将会对你非常有帮助。
我们会告诉你 Nginx 是如何工作的，其背后的概念有哪些，以及如何优化它以提升应用程序的性能。还会告诉你如何安装，如何启动、运行。
这个教程包括三节：
- **基础概念**——你可以了解命令（directive）与环境（context）的区别、继承模式，以及 Nginx 选择服务器区块的顺序，还有安装位置。
- **性能管理**——提升速度的诀窍。我们将会讲解 gzip、缓存、缓冲区以及超时设置。
- **SSL 设置**——讲解用 HTTPS 来提供内容的设置步骤。
我们的目标是创建一系列教程，让你可以轻松找到特定主题的正确配置，如 gzip、SSL 等，或者直接浏览一下那些配置。为了获得最佳的学习体验，我们建议你在自己的机器上安装 Nginx，并亲手实战一下。
## 什么是 Nginx？
Nginx 最初是作为一个 Web 服务器创建的，用于解决 [C10k](https://en.wikipedia.org/wiki/C10k_problem) 的问题。作为一个 Web 服务器，它可以以惊人的速度为您的数据服务。但 Nginx 不仅仅是一个 Web 服务器，你还可以将其用作反向代理，与较慢的上游服务器（如：Unicorn 或 Puma）轻松集成。你可以适当地分配流量（负载均衡器）、流媒体、动态调整图像大小、缓存内容等等。
基本的 nginx 体系结构由 master 进程和其 worker 进程组成。master 读取配置文件，并维护 worker 进程，而 worker 则会对请求进行实际处理。
## 基本命令
要启动 nginx，只需输入：


```
[sudo] nginx
```
当你的 nginx 实例运行时，你可以通过发送相应的信号来管理它：


```
[sudo] nginx -s signal
```
可用的信号：
- stop – 快速关闭
- quit – 优雅关闭 (等待 worker 线程完成处理)
- reload – 重载配置文件
- reopen – 重新打开日志文件
## 指令和上下文
nginx 的配置文件，默认的位置包括：
- `/etc/nginx/nginx.conf`,
- `/usr/local/etc/nginx/nginx.conf`，或
- `/usr/local/nginx/conf/nginx.conf`
配置文件的由下面的部分构成:
- 指令 – 可选项，包含名称和参数，以分号结尾


```
gzip on;
```
- 上下文 – 分块，你可以声明指令 – 类似于编程语言中的作用域


```
worker_processes 2; # 全局上下文指令
http {              # http 上下文
    gzip on;        # http 上下文中的指令
  server {          # server 上下文
    listen 80;      # server 上下文中的指令
  }
}
```
## 指令类型
在多个上下文中使用相同的指令时，必须要小心，因为继承模型不同时有着不同的指令。有三种类型的指令，每种都有自己的继承模型。
### 普通指令
在每个上下文仅有唯一值。而且，它只能在当前上下文中定义一次。子级上下文可以覆盖父级中的值，并且这个覆盖值只在当前的子级上下文中有效。


```
gzip on;
gzip off; # 非法，不能在同一个上下文中指定同一普通指令2次
server {
  location /downloads {
    gzip off;
  }
  location /assets {
    # gzip is on here
  }
}
```
### 数组指令
在同一上下文中添加多条指令，将添加多个值，而不是完全覆盖。在子级上下文中定义指令将覆盖给父级上下文中的值。


```
error_log /var/log/nginx/error.log;
error_log /var/log/nginx/error_notive.log notice;
error_log /var/log/nginx/error_debug.log debug;
server {
  location /downloads {
    # 下面的配置会覆盖父级上下文中的指令
    error_log /var/log/nginx/error_downloads.log;
  }
}
```
### 行动指令
行动是改变事情的指令。根据模块的需要，它继承的行为可能会有所不同。
例如 rewrite 指令，只要是匹配的都会执行：


```
server {
  rewrite ^ /foobar;
  location /foobar {
    rewrite ^ /foo;
    rewrite ^ /bar;
  }
}
```
如果用户想尝试获取 /sample：
- server的rewrite将会执行，从 /sample rewrite 到 /foobar
- location /foobar 会被匹配
- location的第一个rewrite执行，从/foobar rewrite到/foo
- location的第二个rewrite执行，从/foo rewrite到/bar
return 指令提供的是不同的行为：


```
server {
  location / {
    return 200;
    return 404;
  }
}
```
在上述的情况下，立即返回200。
## 处理请求
在 Nginx 内部，你可以指定多个虚拟服务器，每个虚拟服务器用 server{} 上下文描述。


```
server {
  listen      *:80 default_server;
  server_name netguru.co;
  return 200 "Hello from netguru.co";
}
server {
  listen      *:80;
  server_name foo.co;
  return 200 "Hello from foo.co";
}
server {
  listen      *:81;
  server_name bar.co;
  return 200 "Hello from bar.co";
}
```
这将告诉 Nginx 如何处理到来的请求。Nginx 将会首先通过检查 listen 指令来测试哪一个虚拟主机在监听给定的 IP 端口组合。
然后，server_name 指令的值将检测 Host 头(存储着主机域名)。
Nginx 将会按照下列顺序选择虚拟主机：
- 匹配sever_name指令的IP-端口主机
- 拥有default_server标记的IP-端口主机
- 首先定义的IP-端口主机
- 如果没有匹配，拒绝连接。
例如下面的例子：


```
Request to foo.co:80     => "Hello from foo.co"
Request to www.foo.co:80 => "Hello from netguru.co"
Request to bar.co:80     => "Hello from netguru.co"
Request to bar.co:81     => "Hello from bar.co"
Request to foo.co:81     => "Hello from bar.co"
```
### `server_name` 指令
server_name指令接受多个值。它还处理通配符匹配和正则表达式。


```
server_name netguru.co www.netguru.co; # exact match
server_name *.netguru.co;              # wildcard matching
server_name netguru.*;                 # wildcard matching
server_name  ~^[0-9]*\.netguru\.co$;   # regexp matching
```
当有歧义时，nginx 将使用下面的命令：
- 确切的名字
- 最长的通配符名称以星号开始，例如“* .example.org”。
- 最长的通配符名称以星号结尾，例如“mail.**”
- 首先匹配正则表达式（按照配置文件中的顺序）
Nginx 会存储 3 个哈希表：确切的名字，以星号开始的通配符，和以星号结尾的通配符。如果结果不在任何表中，则将按顺序进行正则表达式测试。
值得谨记的是


```
server_name .netguru.co;
```
是一个来自下面的缩写


```
server_name  netguru.co  www.netguru.co  *.netguru.co;
```
有一点不同，`.netguru.co` 存储在第二张表，这意味着它比显式声明的慢一点。
### `listen` 指令
在很多情况下，能够找到 listen 指令，接受IP:端口值


```
listen 127.0.0.1:80;
listen 127.0.0.1;    # by default port :80 is used
listen *:81;
listen 81;           # by default all ips are used
listen [::]:80;      # IPv6 addresses
listen [::1];        # IPv6 addresses
```
然而，还可以指定 UNIX-domain 套接字。


```
listen unix:/var/run/nginx.sock;
```
你甚至可以使用主机名


```
listen localhost:80;
listen netguru.co:80;
```
但请慎用，由于主机可能无法启动 nginx，导致无法绑定在特定的 TCP Socket。
最后，如果指令不存在，则使用 `*:80`。
## 最小化配置
有了这些知识 – 我们应该能够创建并理解运行 nginx 所需的最低配置。


```
# /etc/nginx/nginx.conf
events {}                   # events context needs to be defined to consider config valid
http {
 server {
    listen 80;
    server_name  netguru.co  www.netguru.co  *.netguru.co;
    return 200 "Hello";
  }
}
```
## root, location, 和 try_files 指令
### root 指令
root 指令设置请求的根目录，允许 nginx 将传入请求映射到文件系统。


```
server {
  listen 80;
  server_name netguru.co;
  root /var/www/netguru.co;
}
```
根据给定的请求，指定 nginx 服务器允许的内容


```
netguru.co:80/index.html     # returns /var/www/netguru.co/index.html
netguru.co:80/foo/index.html # returns /var/www/netguru.co/foo/index.html
```
### `location` 指令
location指令根据请求的 URI 来设置配置。
`location [modifier] path`


```
location /foo/ {
  # ...
}
```
如果没有指定修饰符，则路径被视为前缀，其后可以跟随任何东西。
以上例子将匹配


```
/foo
/fooo
/foo123
/foo/bar/index.html
...
```
此外，在给定的上下文中可以使用多个 location 指令。


```
server {
  listen 80;
  server_name netguru.co;
  root /var/www/netguru.co;
  location / {
    return 200 "root";
  }
  location /foo/ {
    return 200 "foo";
  }
}
```


```
netguru.co:80   /       # => "root"
netguru.co:80   /foo    # => "foo"
netguru.co:80   /foo123 # => "foo"
netguru.co:80   /bar    # => "root"
```
Nginx 也提供了一些修饰符，可用于连接 location。这些修饰符将影响 location 模块使用的地方，因为每个修饰符都分配了优先级。


```
=           - Exact match
^~          - Preferential match
~ && ~*     - Regex match
no modifier - Prefix match
```
Nginx 会先检查精确匹配。如果找不到，我们会找优先级最高的。如果这个匹配依然失败，正则表达式匹配将按照出现的顺序进行测试。至少，最后一个前缀匹配将被使用。


```
location /match {
  return 200 'Prefix match: matches everything that starting with /match';
}
location ~* /match[0-9] {
  return 200 'Case insensitive regex match';
}
location ~ /MATCH[0-9] {
  return 200 'Case sensitive regex match';
}
location ^~ /match0 {
  return 200 'Preferential match';
}
location = /match {
  return 200 'Exact match';
}
```


```
/match/    # => 'Exact match'
/match0    # => 'Preferential match'
/match1    # => 'Case insensitive regex match'
/MATCH1    # => 'Case sensitive regex match'
/match-abc # => 'Prefix match: matches everything that starting with /match'
```
### `try_files` 指令
尝试不同的路径，找到一个路径就返回。


```
try_files $uri index.html =404;
```
所以对于 `/foo.html` 请求，它将尝试按以下顺序返回文件：
- $uri ( /foo.html )
- index.html
- 如果什么都没找到则返回 404
有趣的是，如果我们在服务器上下文中定义 try_files，然后定义匹配的所有请求的 location —— try_files 将不会执行。
这是因为在服务器上下文中定义的 try_files 是它的 pseudo-location，这是最不可能的位置。因此，定义 location/ 将比 pseudo-location 更具体。


```
server {
  try_files $uri /index.html =404;
  location / {
  }
}
```
因此，你应该避免在 server 上下文中出现 try_files:


```
server {
  location / {
    try_files $uri /index.html =404;
  }
}
```
## 总结
感谢您的阅读。如果没有大量的资源，这个系列是不可能完成的。在这一系列的写作中，我们发现了一些特别有用的网站：
- [nginx 文档](https://nginx.org/en/docs/)
- [nginx 博客](https://www.nginx.com/blog/)
- [udemy（在线教育网站 ）的 nginx 原理教程](https://www.udemy.com/nginx-fundamentals/)
- [Ilya Grigorik 的博客](https://www.igvita.com/)，和他的令人惊奇的书：[《高性能浏览器网络》](https://hpbn.co/)
- [Martin Fjordvald 的博客](https://blog.martinfjordvald.com/)
我们会很感激你的反馈和评价，请随意讨论。你喜欢这系列吗？你有什么关于下一步应该解决什么问题的建议吗？或你发现了一个错误？告诉我们，下期再见。
