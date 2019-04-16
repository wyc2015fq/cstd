# 转载和积累系列 - Nodejs HTTP多线程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月19日 13:30:07[initphp](https://me.csdn.net/initphp)阅读数：2483









译自：[http://developer.yahoo.net/blog/archives/2010/07/multicore_http_server_with_nodejs.html](http://developer.yahoo.net/blog/archives/2010/07/multicore_http_server_with_nodejs.html)（稍有修改），
 转载请指明原文链接：[http://www.grati.org/?p=307](http://www.grati.org/?p=307)


简单地说，NodeJS是一个使用了Google高性能[V8引擎](http://code.google.com/apis/v8/) 的服务器端JavaScript实现。它提供了一个（几乎）完全非阻塞I/O栈，与JavaScript提供的闭包和匿名函数相结合，使之成为编写高吞吐量网络服务程序的优秀平台。在我们内部，雅虎邮件队正调研能否使用NodeJS开发一些我们即将推出的新服务。我们认为分享我们的劳动成果是一件十分有意义的事情。

**目录**
- [在多处理器系统上使用NodeJS的情况](http://www.grati.org/?p=307#1)
- [充分利用多处理器的优势](http://www.grati.org/?p=307#2)- [使用软件负载均衡](http://www.grati.org/?p=307#3)
- [使用操作系统内核做负载均衡](http://www.grati.org/?p=307#4)
- [使用NodeJS转发请求](http://www.grati.org/?p=307#5)

- [结论](http://www.grati.org/?p=307#6)




### 在多处理器系统上使用NodeJS的情况


NodeJS中并不是完美无缺的。虽然单进程的性能表现相当不错，但一个CPU最终还是不够用（由于JS引擎自身的运行原理，NodeJS使用单线程执行JS代码，详见“JS和多线程”）。Node本身并没有扩展能力来充分利用多CPU系统的计算能力。实际上当前版本的NodeJS程序只能在一个上CPU执行，在2.5GHz的英特尔至强处理器下运行HTTP代理服务的性能大约为2100 reqs/s。


虽然Node相对稳定，但它仍然会偶尔崩溃。如果你实用一个单独的NodeJS进程作为服务，崩溃会对可用性造成不良影响。例如段错误、内存越界等错误在用C++编写的程序上相当普遍。如果有多个进程同事处理请求，当一个进程出错退出，传入的请求可以被导向给其他进程。



### 充分利用多处理器的优势


有如下几种方法可以使NodeJS利用多处理器，每个方法都有自己的优缺点。

**使用软件负载均衡**


直到`node-v0.1.98` ，充分利用多处理器的最佳做法是为每个处理器单独启动一个NodeJS进程，每个进程都运行HTTP服务并绑定到不同的端口。这样需要一个负载均衡软件，将客户端请求转发到各进程，这个软件知道每个服务进程的端口。这样处理性能也不错，但配置管理多进程比较复杂，因此不是最佳方案。


当然，这种架构也有好处，它允许负载均衡软件按照指定的策略将请求路由到不同进程上。（例如，通过IP，通过cookie等）。

**使用操作系统内核做负载均衡**


在`node-v0.1.98`中
 ，雅虎贡献了一个用于传递和重用文件描述符的核心补丁，允许如[Connect](http://www.google.com/url?q=http%3A%2F%2Fgithub.com%2Fsenchalabs%2Fconnect&sa=D&sntz=1&usg=AFQjCNFQhaEXpgJIihsSxcVCW3aompqZ3g)和[multi-node](http://www.google.com/url?q=http%3A%2F%2Fgithub.com%2Fkriszyp%2Fmulti-node&sa=D&sntz=1&usg=AFQjCNFH9asviQbFiaTKILvjxFKmFOW6qw)等HTTP框架使用多个进程同时提供HTTP服务，而且不需要修改原有的程序代码和配置。


概括地讲，这些框架使用的方法是创建一个进程监听端口（比如说监听80端口）。然而，这个进程不是接受Socket连接，而是使用net.Stream.write()将其传递给了其他子进程（其内部是使用sendmsg(2)发送，并使用recvmsg来获取文件表描述符）。每个子进程排队将收到的文件描述符插入自己的事件循环中并在空闲时处理客户端的连接。OS内核本身负责进程间的负载平衡。


重要的是，这实际上是一个高效但没有策略的L4负载平衡器，每个客户端的请求可能被任意一个进程处理。任何处理请求所需的应用程序的状态，都不能像单进程时那样简单的保存在一个NodeJS实例当中。

**使用NodeJS转发请求**


某些情况下，你可能可能无法使用或者不想使用上述两种方法。例如，负载均衡程序无法按照应用程序所需的路由规则转发请求（如，有复杂应用逻辑的路由规则或者需要SELinux连接信息的路由规则）。在这种情况下，可以使用单个进程接受连接，检查并传递给其他进程处理。


下面的例子需要`node-v0.1.100`或更高版本以及[node-webworker](http://www.google.com/url?q=http%3A%2F%2Fgithub.com%2Fpgriess%2Fnode-webworker&sa=D&sntz=1&usg=AFQjCNHz3AB3SbiFG3DzfUeJq5BHgnjmfw) 。node-webworker是新兴的HTML5
 Web Workers标准的NodeJS实现，这个标准允许并行执行JavaScript代码。您可以使用npm安装node-webworker，命令如下 npm install webworker@stable。


详细介绍Web Workers的原理超出了这篇文章的范围，你可以认为Web Worker是一个独立的执行上下文（类似进程），它可以由JavaScript代码生成并来回传递数据。node-webworker允许使用如下消息传递机制传递文件描述符：


首先，主进程的源代码master.js：



[view
 plain](http://www.grati.org/?p=307#)[copy
 to clipboard](http://www.grati.org/?p=307#)[print](http://www.grati.org/?p=307#)[?](http://www.grati.org/?p=307#)

- var net = require('net');  
- var path = require('path');  
- var sys = require('sys');  
- var Worker = require('webworker/webworker').Worker;  
- 
- var NUM_WORKERS = 5;  
- 
- var workers = [];  
- var numReqs = 0;  
- 
- for (var i = 0; i < NUM_WORKERS; i++) {  
-  workers[i] = new Worker(path.join(__dirname, 'worker.js'));  
-  }  
- 
- net.createServer(function(s) {  
-  s.pause();  
- 
- var hv = 0;  
-  s.remoteAddress.split('.').forEach(function(v) {  
-  hv += parseInt(v);  
-  });  
- 
- var wid = hv % NUM_WORKERS;  
- 
-  sys.debug('Request from ' + s.remoteAddress + ' going to worker ' + wid);  
- 
-  workers[wid].postMessage(++numReqs, s.fd);  
-  }).listen(80);  



主进程将执行如下操作：
- 
主进程将建立net.Server实例并在80端口上侦听连接请求。
- 
当请求到来时，主进程
- 
根据请求端的IP地址决定将请求发送至哪一个wroker。
- 
调用请求流对象的`net.``Stream.pause()`方法。这可以防止主进程从读取套接字中读取数据
 — wroker进程应该看到远程端发送的所有数据。
- 
使用`postMessage()`方法将（递增后的）全局请求计数器和刚刚收到

套接字描述符发送到指定的worker



然后，worker进程的源代码worker.js：



[view
 plain](http://www.grati.org/?p=307#)[copy
 to clipboard](http://www.grati.org/?p=307#)[print](http://www.grati.org/?p=307#)[?](http://www.grati.org/?p=307#)

- var http = require('http');  
- var net = require('net');  
- var sys = require('sys');  
- 
- process.setuid('nobody');  
- 
- var srv = http.createServer(function(req, resp) {  
-  resp.writeHead(200, {'Content-Type' : 'text/plain'});  
-  resp.write(  
- 'process=' + process.pid +  
- '; reqno=' + req.connection.reqNo + '\n'
-  );  
-  resp.end();  
-  });  
- 
- onmessage = function(msg) {  
- var s = new net.Stream(msg.fd);  
-  s.type = srv.type;  
-  s.server = srv;  
-  s.resume();  
- 
-  s.reqNo = msg.data;  
- 
-  srv.emit('connection', s);  
-  };  



worker执行如下操作：
- 
将自己的权限降为`nobody`用户。
- 
创建一个HTTP服务器实例但并不调用任何`listen()`方法。我们将通过主进程收到的描述符来传递请求。
- 
等待从主进程接收套接字描述符和相关信息
- 
将从主进程收到的请求计数保存进流对象(stream.object)中，代码有些乱，但让我们可以使用HTTP相关的类来处理这些数据。
- 
将net.Stream实例和收到的TCP链接接合，然后通过手动触发事件将其融入HTTP请求的处理流程中。
- 
现在，我们如上建立的请求处理程序可以正常运行了： HTTP服务实例完全拥有连接并将像平常一样解理客户端的请求。注意一个小技巧，请求处理程序访问流对象的reqNo属性，并根据主进程中的计数变量(既用于记录请求数的全局变量numReqs)将其设置为实际的请求数。


最后，一定要使用超级用户执行`master.js`，因为我们希望程序监听特权端口(80)。然后使用`curl` 发出一些请求，并看看是那个进程处理这些请求。

```
% sudo node ./master.js
% curl 'http://localhost:80'
process=13049; reqno=2
```


当然，前面例子用到的基于IP的哈希算法是玩具级的，任何一个合格的HTTP负载均衡器能可以实现。在现实中，你可能想根据客户端的请求，将连接分派到运行在正确的SELinux上下文中的worker。（参见，[node-selinux](http://www.google.com/url?q=http%3A%2F%2Fgithub.com%2Ftedx%2Fnode-selinux&sa=D&sntz=1&usg=AFQjCNGWH7YJzXccL0SV41CHWMUumeskyQ)）根据HTTP请求本身的的信息（如：path，vhost）作出路由决策稍微复杂些，且使用类似的技术也可行。



### 结论


最 后，我希望本文能够说明当前NodeJS利用多处理器的情况：一些现有的HTTP框架可以给各种NodeJS应用提供多处理器支持；node- webworkers为管理NodeJS中的并行机制提供了一个好方法（基于chlid_proess）；怎样实用NodeJS自身实现L7 HTTP路由器



