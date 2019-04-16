# 转载和积累系列 - nodejs服务器端开发 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月26日 10:04:02[initphp](https://me.csdn.net/initphp)阅读数：3268








随着 Web 2.0 概念和 Ajax 技术的流行，JavaScript 作为 Ajax 应用开发中必不可少的一部分，已经得到了广泛的流行。开发人员也开始逐步的熟悉和掌握 JavaScript，并积累了相关的开发经验。虽然 JavaScript 目前主要用在 Web 应用中，以浏览器作为运行平台，但是已经有相关的尝试把 JavaScript 迁移到服务器端，这其中包括 Aptana 的[Jaxer](http://www.jaxer.org/)
 等。这种做法与[Google GWT](http://code.google.com/webtoolkit/) 是异曲同工的。Google GWT 允许开发人员使用 Java 语言来编写 Web 前端代码。这两种做法的目的都是为了复用开发人员已经掌握的知识和积累的经验。在这点上，node.js 类似于 Jaxer。

简单的来说，node.js 是一个允许开发人员使用 JavaScript 语言编写服务器端代码的框架。也就是说编写的 JavaScript 代码可以直接运行在本地机器上，而不仅限于浏览器。从实现的角度来说，Jaxer 和 node.js 都使用了已有的 JavaScript 执行引擎。Jaxer 用的是 Mozilla Firefox 中使用的 JavaScript 引擎，而 node.js 用的则是 Google Chrome 中用的 V8 引擎。

node.js 入门

node.js 可以运行在 Linux、Windows 和 Macintosh 等主流的操作系统上。在 Windows 平台上运行 node.js 的话，需要 Cygwin 或是 MinGW 的支持。下面以常用的 Windows 平台为例来说明。首先需要安装 Cygwin。安装的时候需要选择`gcc-g++` 、`make` 、`openssl` 和
`python` 等包。`gcc` 的版本必须是最新的。接着从[参考资料](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#resources) 中给出的地址下载 node.js 0.4.0 版本的源代码。下载解压之后，依次在 Cygwin 中运行`./configure`
 、`make` 和 `make install` 等命令进行编译和安装。安装完成之后，直接运行`node` 命令就可以启动 node.js 提供的命令行。在命令行中可以直接输入 JavaScript 代码并运行。也可以通过
`node server.js` 的方式来运行一个 JavaScript 文件 `server.js` 。

[代码清单 1](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code1) 中给出了一个简单的“Hello World”程序的示例。通过`node helloworld.js` 来运行该 JavaScript 文件之后，会在控制台输出“Hello World”。

**清单 1. 使用 node.js 的“Hello World”程序**
|process.stdout.write("Hello World");|
|----|


[代码清单 1](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code1) 中的`process` 表示的是当前运行的 node.js 进程，其属性
`stdout` 表示的是进程的标准输出流。通过`write()` 方法向给流中写入一个字符串。从 [代码清单 1](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code1) 可以看到，使用 JavaScript 就可以访问标准输出流等本地系统上的资源。这从一个侧面反映出来了 node.js 的强大。

在 node.js 可以运行的 JavaScript 代码中，可以使用一些全局的对象：包括 [代码清单 1](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code1) 中用到的 `process` 、下面会介绍的用来加载模块的`require()` 方法、表示当前正在执行的 JavaScript 文件名的
`__filename` 、表示当前正在执行的 JavaScript 文件的目录的`__dirname` 和与浏览器中相似的用来执行定时任务的
`setTimeout()` 和 `setInterval()` 方法等。

在介绍了 node.js 的基本知识之后，下面介绍 node.js 的模块化结构。



[回页首](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#ibm-pcon)

模块化结构

node.js 使用了 [CommonJS](http://commonjs.org/) 定义的模块系统。不同的功能组件被划分成不同的模块。应用可以根据自己的需要来选择使用合适的模块。每个模块都会暴露一些公共的方法或属性。模块使用者直接使用这些方法或属性即可，不需要关系模块内部的实现细节。除了系统预置的多个模块之外，应用开发团队也可以利用这个机制来将应用拆分成多个模块，以提高代码的可复用性。

使用模块

在 node.js 中使用一个模块的方式是非常简单的。使用某个模块之前需要首先声明对它的依赖。在 JavaScript 代码中可以直接使用全局函数 
```
require()
```
 来加载一个模块。如 `require("http")` 可以加载系统预置的 `http` 模块。而`require("./myModule.js")` 用来加载与当前 JavaScript 文件同一目录下的
`myModule.js` 模块。如果使用`require()` 的路径以“/”开头的话，则认为是模块 JavaScript 文件在操作系统上的绝对路径。如果不是这两种情况的话，node.js 就会尝试在当前 JavaScript 文件的父目录及其祖先目录下的`node_modules` 目录下查找。比如目录
`/usr/home/my.js` 中调用了 `require("other.js")` 的话，node.js 会依次尝试查找下列文件：`/usr/home/node_modules/other.js` 、`/usr/node_modules/other.js` 和`/node_modules/other.js` 。

`require()` 方法的返回值是该模块所暴露出来的公开 JavaScript 对象，包含了可供使用的方法和属性。[代码清单 2](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code2) 给出了模块的基本使用方式。

**清单 2. 模块的基本使用方式**
|var greetings = require("./greetings.js");  var msg = greetings.sayHello("Alex", "zh_CN");  process.stdout.write(msg);|
|----|


如 [代码清单 2](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code2) 所示，一般是直接把 `require()` 方法的返回值赋值给一个变量，在 JavaScript 代码中直接使用此变量即可。`greetings.js` 模块暴露了一个`sayHello()` 方法，当前 JavaScript 代码直接使用了该方法。

开发自己的模块

开发自己的模块的基本工作是在模块对应的 JavaScript 文件中编写模块相关的代码。这其中封装了模块的内部处理逻辑。一般来说，一个模块通常会暴露一些公开的方法或属性给其使用者。模块的内部代码需要把这些方法或属性给暴露出来。[代码清单 3](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code3)
 给出了[代码清单 2](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code2) 中所使用的
`greetings.js` 文件的内容。

**清单 3. greetings.js 模块的内容**
|var languages = {     "zh_CN" : "你好，",     "en" : "Hello, " };       exports.sayHello = function(name, language) {     return languages[language] || languages["en"] + name;  };|
|----|


如 [代码清单 3](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code3) 所示，`exports` 对象的内容就是模块的使用者调用`require()` 方法的返回值中所包含的内容。模块通过这种方式来声明其所暴露出来的公开方法和属性。在模块中定义的变量，如
`languages` ，是只对模块内部的代码可见的。

如果一个模块所包含的内容比较多，也可以用文件夹的方式来组织。可以在文件夹的根目录下面创建一个 `package.json` 文件，其内容中包含了模块的名称和入口 JavaScript 文件的路径。如果没有提供这个`package.json` 文件的话，node.js 会默认在文件夹中查找
`index.js` 文件作为模块的启动 JavaScript 文件。

在介绍完 node.js 的模块化结构之后，下面介绍其事件驱动机制。



[回页首](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#ibm-pcon)

事件驱动

开发过 Web 应用的人都熟悉浏览器中的事件处理机制。当对某个 DOM 元素上的某类事件感兴趣的时候，只需要在该 DOM 元素上面注册一个事件监听器即可。如`ele.addEventListener("click", function() {})` 就添加了一个对
`click` 事件的监听器。当事件发生的时候，事件监听器的 JavaScript 方法就会被调用。事件的处理方法是异步执行的。这种异步执行的方式非常适合于开发高性能并发网络应用。实际上，目前的高性能并发应用开发一般有两种做法：第一种是使用多线程的机制，另外一种就是采用基于事件驱动的方式。多线程的问题在于应用开发起来难度较高，很容易出现线程饥饿或是死锁等问题，对开发人员提出了更高的要求。而事件驱动的方式则更加灵活，很容易为 Web 开发人员所理解和使用，也不存在线程死锁等问题。依托于性能强大的
 Google V8 引擎和先进的事件 I/O 架构，node.js 可以成为创建高性能服务器端应用的良好基础。

基于 node.js 开发应用与开发 Web 应用有相似的编程模型。很多模块都会暴露出一些事件，使用这些模块的代码通过注册事件监听器的方式来添加相应的处理逻辑。[代码清单 4](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code4) 中给出了一个简单的 HTTP 代理服务器的实现代码。

**清单 4. HTTP 代理服务器**
|var http = require("http");  var url = require("url");  http.createServer(function (req, res) {     var urlObj = url.parse(req.url, true); // 获取被代理的 URL     var urlToProxy = urlObj.query.url;     if (!urlToProxy) {         res.statusCode = 400;         res.end("URL 是必须的。");     }     else {         console.log("处理代理请求：" + urlToProxy);         var parsedUrl = url.parse(urlToProxy);         var opt = {             host : parsedUrl.hostname,             port : parsedUrl.port || 80,             path : (parsedUrl.pathname || "") + (parsedUrl.search || "")                 + (parsedUrl.hash || "")         };         http.get(opt, function(pres) { // 请求被代理 URL 的内容            res.statusCode = pres.statusCode;             var headers = pres.headers;             for (var key in headers) {                 res.setHeader(key, headers[key]);             }             pres.on("data", function(chunk) {                 res.write(chunk); // 写回数据            });             pres.on("end", function() {                 res.end();             });         });     }  }).listen(8088, "127.0.0.1");  console.log("代理服务器已经在 8088 端口启动。");|
|----|


整个代理服务器的实现比较简单。首先通过 `http` 模块中的 `createServer()` 方法用来创建一个 HTTP 服务器，再通过`listen()` 方法就可以让该 HTTP 服务器在特定端口监听。在
`createServer()` 方法中传入的参数是 HTTP 请求的响应方法。实际上，每个 HTTP 请求都是对应于 HTTP 服务器上的一个`request` 事件。[代码清单 4](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code4)
 中的 HTTP 服务器创建部分实际上等价于[代码清单 5](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code5) 中给出的实现方式。

**清单 5. 使用事件机制的 HTTP 服务器创建方式**
|var server = http.createServer();  server.on("request", function(req, res) {  });|
|----|


在请求的处理方法里面，通过 `http.get()` 方法来获取被代理 URL 的内容。这里同样采用了基于事件的处理方式。`pres.on("data", function(chunk) {})` 在`pres` 的
`data` 事件上添加了一个处理方法。该方法的作用是当获取到被代理 URL 的内容的时候，就把获取到的内容写回到原始 HTTP 请求的响应中。对于`end` 事件的处理也是同样的。在使用 node.js 进行开发的时候，会经常遇到这种使用事件处理方法和回调方法的场景。

在介绍了 node.js 的事件驱动机制之后，下面介绍一些常用的模块。



[回页首](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#ibm-pcon)

常用模块

node.js 默认提供了很多与网络与文件系统操作相关的模块。这些模块是构建服务器端应用的基础。下面对其中一些常见的模块进行具体说明。

事件模块

前面提到过，node.js 采用的是事件驱动的架构，其中的很多模块都会产生各种不同的事件，可以由模块使用者来添加事件处理方法。所有能够产生事件的对象都是事件模块中的`EventEmitter` 类的实例。

`EventEmitter` 类中的方法都与事件的产生和处理相关，如下所示：
- `addListener(event, listener)` 和 `on(event, listener)` ：这两个方法的作用都是用来为某个事件`event` 添加事件处理方法
`listener` 。
- `once(event, listener)` ：这个方法为某个事件 `event` 添加仅执行一次的处理方法`listener` 。处理方法在执行一次之后就会被删除。
- `removeListener(event, listener)` ：该方法用来删除某个事件 `event` 上的处理方法`listener` 。
- `emit(event, [arg1], [arg2], [...])` ：该方法用来产生某个事件 `event` 。事件名称`event` 之后的参数被传递给对应的事件处理方法。

[代码清单 6](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code6) 给出了事件模块的使用示例。

**清单 6. 事件模块的使用示例**
|var events = require("events");  var emitter = new events.EventEmitter();  emitter.on("myEvent", function(msg) {     console.log(msg);  });  emitter.emit("myEvent", "Hello World.");|
|----|


在事件模块中有一个特殊的事件 `error` 。当出现错误的时候，`EventEmitter` 会产生此事件。如果此事件没有对应的处理方法的话，默认的行为是输出错误信息后，程序自动终止。因此，需要注意总是添加一个对`error` 事件的处理方法。

流

node.js 中存在各式各样不同的数据流，包括文件系统、HTTP 请求和响应、以及 TCP/UDP 连接等。这些流都是 `EventEmitter` 的实例，因此可以产生各种不同的事件。流可以分成只读、只写和读写流三种。

可读流主要会产生 4 个事件：
- `data` ：当读取到流中的数据时，产生此事件。
- `end` ：当流中没有数据可读时，产生此事件。
- `error` ：当读取数据出现错误时，产生此事件。
- `close` ：当流被关闭时，产生此事件。

除了上面的事件之外，还有一个 `pipe()` 方法可以用来把当前的可读流和另外一个可写流连接起来。可读流中的数据会被自动写入到可写流中。

可写流中最常用的是 `write()` 和 `end()` 两个方法。`write()` 方法用来向流中写入数据，而`end()` 则用来结束写入操作。

为了表示二进制数据，node.js 使用了类 `Buffer` 来表示数据缓冲区，以对二进制数据进行操作。`Buffer` 类内部是以数组的方式来存储数据的。一旦创建出来之后，`Buffer` 的大小是不能被修改的。`Buffer` 类的实例是可以与 JavaScript 中的字符串类型互相转换的。在转换的时候需要指定编码格式。通过`Buffer` 类的
`toString(encoding, start, end)` 方法可以把 `Buffer` 中的从`start` 到
`end` 的内容转换成以 `encoding` 编码的字符串。可以支持的编码格式有：`ascii` 、`utf8` 和`base64` 。通过
`new Buffer(str, encoding)` 可以用一个字符串 `str` 来初始化一个缓冲区。`write(string, offset, encoding)` 用来把一个字符串`string` 以编码格式
`encoding` 写入到缓冲区中以 `offset` 开始的位置上。

网络操作

node.js 提供了一些与网络操作相关的模块，包括 TCP、UDP 和 HTTP 等，可以实现网络服务器和客户端。

与 TCP 协议相关的实现在 `net` 模块中。通过该模块的 `createServer(connectionListener)` 方法可以创建一个 TCP 服务器。参数`connectionListener` 是当有客户端连接到该服务器上时的处理方法，等价于对
`connect` 事件的处理。一个 TCP 服务器是类`Server` 的实例。通过 `listen` 方法可以让服务器在指定端口监听。

如果想连接到一个已有的 TCP 服务器的话，可以使用 `createConnection(port, host)` 方法来连接到指定主机`host` 的端口
`port` 上。该方法的返回值是 `Socket` 类的实例，表示一个套接字连接。得到一个`Socket` 类的实例之后，就可以通过
`write()` 方法来向该连接中写入数据。如果想从该连接上获取数据的话，可以添加`data` 事件的处理方法。

[代码清单 7](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code7) 中给出了一个简单的用来进行表达式计算的 TCP 服务器，可以通过`telnet` 命令连接到此服务器来进行测试。

**清单 7. 简单的表达式计算服务器**
|var net = require("net");  var server = net.createServer(function(socket) {     socket.setEncoding("utf8");     var buffer = [], len = 0;     socket.on("data", function(data) { // 接收到客户端数据        if (data.charCodeAt(0) == 13) {             var expr = buffer.join("");             try {                 var result = eval(expr); // 进行计算                socket.write(result.toString()); // 写回结果            } catch (e) {                 socket.write("Wrong expression.");             } finally {                 socket.write("\r\n");                 buffer = [];             }         }         else {             buffer.push(data);         }     });  });  server.listen(8180, "127.0.0.1");  console.log("服务器已经在端口 8180 启动。");|
|----|


除了 TCP 服务器外，模块 `http` 和 `https` 可以分别实现 HTTP 和 HTTPS 服务器，模块`dgram` 可以实现 UDP/Datagram 套接字连接，模块
`tls` 可以实现安全的套接字连接（SSL）。这些模块的使用方式都类似于模块`tcp` 。

文件系统

node.js 中的 `fs` 模块用来对本地文件系统进行操作。`fs` 模块中提供的方法可以用来执行基本的文件操作，包括读、写、重命名、创建和删除目录以及获取文件元数据等。每个操作文件的方法都有同步和异步两个版本。异步操作的版本都会使用一个回调方法作为最后一个参数。当操作完成的时候，该回调方法会被调用。而回调方法的第一个参数总是保留为操作时可能出现的异常。如果操作正确成功，则第一个参数的值是`null` 或
`undefined` 。而同步操作的版本的方法名称则是在对应的异步方法之后加上一个 `Sync` 作为后缀。比如异步的`rename()` 方法的同步版本是
`renameSync()` 。下面列出来了 `fs` 模块中的一些常用方法，都只介绍异步操作的版本。
- `rename(path1, path2)` ：将路径 `path1` 表示的目录或文件重命名成路径 
```
path2
```
 。
- `truncate(fd, len)` ：将文件描述符 `fd` 对应的文件的长度截断为 `len` 。
- `chmod(path, mode)` ：将路径 `path` 表示的目录或文件的权限修改为 `mode` 。
- `stat(path)` ：获取路径 `path` 表示的目录或文件的元数据。元数据用 `Stats` 类来表示。
- `open(path, flags, mode)` ：打开一个路径 `path` 表示的文件。回调方法中可以得到该文件的描述符。
- `read(fd, buffer, offset, length, position)` ：读取给定文件描述符 `fd` 所表示的文件中从`position` 位置开始的长度为
`length` 字节的数据，并存放到缓冲区 `buffer` 中从`offset` 起始的位置上。回调方法中可以得到实际读取的字节数。
- `write(fd, buffer, offset, length, position)` ：将缓冲区 `buffer` 中的数据写入到文件描述符`fd` 所表示的文件中。参数的含义与
`read()` 方法一样。回调方法中可以得到实际写入的字节数。
- `readFile(filename, encoding)` ：以编码格式 `encoding` 来读取一个文件`filename` 中的内容。回调方法中可以得到文件的内容。
- `writeFile(filename, data, encoding)` ：将数据 `data` 以编码格式`encoding` 写入到文件
`filename` 中。

除了上面列出来的直接操作文件本身的方法外，还可以把文件转换成流。`createReadStream(path, options)` 和`createWriteStream(path, options)` 分别用来从文件中创建可读和可写流。参数
`path` 表示的是文件的路径，`options` 是一个表示读取或写入文件时的选项的 JavaScript 对象。

[代码清单 8](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code8) 中给出了一个简单的 HTTP 静态文件服务器的实现。

**清单 8. HTTP 静态文件服务器**
|var http = require("http"),     fs = require("fs"),     path = require("path"),     url = require("url");  var server = http.createServer(function(req, res) {     var pathname = url.parse(req.url).pathname;     var filepath = path.join("/tmp", "wwwroot", pathname);     var stream = fs.createReadStream(filepath, {flags : "r", encoding : null});     stream.on("error", function() {         res.writeHead(404);         res.end();     });     stream.pipe(res);  });  server.on("error", function(error) {     console.log(error);  });  server.listen(8088, "127.0.0.1");|
|----|


如 [代码清单 8](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code8) 所示，首先把 HTTP 请求的路径转换成服务器上文件路径，再从文件中创建可读流，最后通过`pipe()` 方法把文件的数据流传递到 HTTP 请求的响应中。

辅助模块

除了上面介绍的这些常见模块之外，node.js 还提供了一些辅助的模块。

模块 `path` 用来处理文件系统上的路径。这个模块中的 `join()` 用来把多个路径连接起来，形成一个完整的路径。如`join("/usr", "home", "test/index.html")` 的结果是路径
`/usr/home/test/index.html` 。`normalize()` 用来对路径进行归一化操作，去掉其中多余的“/”以及处理“..”和“.”。`resolve([from ...], to)` 方法用来获取给定路径`to` 的绝对路径。如果
`to` 不是绝对路径，就把它之前的参数按从右到左的顺序添加上去，直到得到了一个绝对路径。如果到最后还是无法得到绝对路径，就把当前的工作目录加上。假设当前的工作目录是`/usr/home` ，那么
`resolve("test", "index.html")` 的返回结果是 `/usr/home/test/index.html` 。`dirname()` 方法用来获取路径的目录部分。如
`dirname("/usr/home/index.html")` 的返回结果是`/usr/home` 。`basename()` 用来获取路径的最后一个部分。如
`basename("/usr/home/index.html")` 的返回结果是`index.html` 。`extname()` 用来获取一个路径的文件扩展名部分。如
`extname("/usr/home/index.html")` 的返回结果是`.html` 。

模块 `url` 用来对 URL 进行解析。`parse(urlStr, parseQueryString)` 方法用来把一个 URL 字符串`urlStr` 解析成主机名、端口和路径等几个部分。该方法的返回值是一个包含了
`protocol` 、`hostname` 、`port` 、`pathname` 和`query` 等属性的 JavaScript 对象。如果参数
`parseQueryString` 的值为 `true` 的话，URL 中包含的查询字符串部分也会被解析。`format(urlObj)` 方法的作用与
`parse()` 方法正好相反，用来从一个 JavaScript 对象中构建出 URL 字符串。

模块 `querystring` 用来处理 URL 中的查询字符串。`stringify(obj)` 方法用来把一个 JavaScript 对象`obj` 转换成查询字符串的格式。如
`stringify({a : 1, b : "good"})` 的返回结果是`a=1&b=good` 。`parse(str)` 用来把一个查询字符串解析成 JavaScript 对象。

模块 `vm` 可以用来执行 JavaScript 代码。方法 `runInThisContext(code)` 用来执行一段 JavaScript 代码`code` 并返回其结果。通过该方法运行的 JavaScript 代码不能访问当前代码的作用域。`runInNewContext(code, [sandbox])` 方法也是用来执行 JavaScript 代码的，与`runInThisContext()`
 不同的是通过该方法运行的 JavaScript 代码使用 `sandbox` 对象作为全局对象。如`runInNewContext("a + 3", {a : 4})` 的返回结果是 7。`createScript(code)` 方法用来预先编译一段 JavaScript 代码，但是并不立即执行。该方法的返回值是一个`Script` 对象。该对象同样有
`runInThisContext()` 和 `runInNewContext([sandbox])` 两个方法，含义与上面提到的两个方法类似。

模块 `os` 提供了与底层操作系统相关的一些信息。包括 `hostname()` 用来获取操作系统的主机名；`type()` 用来获取操作系统的类型；`release()` 用来获取操作系统的发行版本号；`uptime()` 用来获取以秒计算的系统运行时间；`cpus()` 用来获取 CPU 的相关信息。`freemem()` 和`totalmem()`
 分别用来获取系统的内存总数和可用内存数。

模块 `util` 提供了一些常用的辅助方法。`debug(string)` 方法用来输出信息到标准错误流。`log(string)` 方法用来输出附带时间戳的信息到标准输出流。`inspect(object, showHidden, depth)` 方法用来输出一个对象的内部结构，参数`object` 是要查看的对象，`showHidden` 表示是否查看对象的隐藏属性，`depth`
 表示查看的对象层次结构的深度，默认值是 2。`inherits(constructor, superConstructor)` 方法用来实现 JavaScript 中基于原型的继承机制。

在介绍完 node.js 提供的常用模块之后，下面通过一个完整的示例来说明 node.js 的用法。



[回页首](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#ibm-pcon)

实例分析

这个实例实现的功能是动态监测服务器的内存使用状态，即内存的占用率。获取服务器上的内存占用率比较简单，只需要使用 `os` 模块提供的方法即可，即`freemem()/totalmem()` 。为了能够实时的监测内存占有率，服务器需要实时的把数据传输给浏览器端。这里最好的实现方式是 HTML 5 中引入的 WebSocket 规范。该规范在 Firefox 4 和 Google Chrome 等新浏览器上得到了支持。同时服务器端也需要支持此规范。Socket.IO
 在 node.js 上提供了对 WebSocket 规范的支持，包括服务器端和浏览器端代码。[代码清单 9](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code9) 给出了使用 Socket.IO 的服务器端代码。

**清单 9. 监测内存占用率的服务器端代码**
|var io = require('./socket.io');  var io = io.listen(server);  io.on("connection", function(client){     setInterval(function() {         client.send(os.freemem() / os.totalmem());     }, 500);  });|
|----|


在 [代码清单 9](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code9) 中，`server` 是 node.js 中的一个 HTTP 服务器对象，用来响应一般的 HTTP 请求。Socket.IO 可以对 node.js 的 HTTP 服务器的请求进行拦截，将部分请求交给 Socket.IO 来处理。这里的处理逻辑是当有客户端连接上的时候，就每隔 500 毫秒把服务器的内存占用率发送给客户端。[代码清单
 10](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code10) 给出了浏览器端的 HTML 和 JavaScript 代码。

**清单 10. 监测内存占用率的浏览器端代码**
|<!doctype html>  <html>   <head>     <title> 服务器内存使用情况 </title>     <script src="/socket.io/socket.io.js"></script>     <style>         #usage {border : 1px dashed green;}     </style>     <script>         var canvas, width = 200, height = 200, buffer = [], max = 200;         function updateChart(data) {             if (buffer.length >= max) {                 buffer.unshift();             }             buffer.push(data);             var ctx = canvas.getContext("2d");             ctx.clearRect(0, 0, width, height);             for (var i = 1, n = buffer.length; i < n; i++) {                 ctx.strokeStyle = "red";                 ctx.beginPath();                 ctx.moveTo(i - 1 , buffer[i - 1] * height);                 ctx.lineTo(i, buffer[i] * height);                 ctx.closePath();                 ctx.stroke();             }         }             function onLoad() {             canvas = document.getElementById("usage");             var socket = new io.Socket(null, {port: 8088});             socket.connect(); // 连接到服务器            socket.on("message", function(obj){ // 接收到消息时的处理方法                updateChart(obj);             });         }     </script>   </head>   <body onload="onLoad();">     <h1> 内存使用情况 </h1>       <canvas id="usage" width="200" height="200"></canvas>   </body>  </html>|
|----|


如 [代码清单 10](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#code10) 所示，首先建立一个与服务器之间的 WebSocket 连接。通过 
```
message
```
 事件定义了当接收到服务器端的消息时，更新浏览器端的显示。浏览器端通过一个 HTML 5 提供的 canvas 来绘制内存占用率的曲线图，如[图 1](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#fig-mem-usage) 所示。

**图 1. 内存占用率的曲线图**
![内存占用率的曲线图](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/mem_usage.jpg)



[回页首](http://www.ibm.com/developerworks/cn/web/1107_chengfu_nodejs/index.html#ibm-pcon)

总结

一提到服务器端开发，开发人员一般想到的就是 Java 和 C/C++ 等语言。但是通过 node.js 提供的强大能力，熟悉 JavaScript 的 Web 开发人员也可以开发出服务器端的应用。本文详细介绍了 node.js 的事件驱动机制和模块化结构，并对其中的常用模块做了详细说明，最后通过一个完整的实例展示了 node.js 的实用性。




