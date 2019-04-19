# nodejs入门开发与常用模块 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月30日 15:24:34[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：262
- npm：NodeJs包管理器
- express：服务器端比较流行的MVC框架，处理服务请求，路由转发，逻辑处理
- [http://socket.io](https://link.zhihu.com/?target=http%3A//socket.io)：实现服务端和客户端socket通信解决方案
[https://www.zhihu.com/question/19793473](https://www.zhihu.com/question/19793473)
[https://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/001434501245426ad4b91f2b880464ba876a8e3043fc8ef000](https://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/001434501245426ad4b91f2b880464ba876a8e3043fc8ef000)
正式开启JavaScript的后端开发之旅。
由于Node.js平台是在后端运行JavaScript代码，所以，必须首先在本机安装Node环境。输入node
 -v 验证
输入node，此刻你将进入Node.js的交互环境。
在交互环境下，你可以输入任意JavaScript语句，
要退出Node.js环境，连按两次Ctrl+C。
为啥我们需要一个包管理工具呢？因为我们在Node.js上开发时，会用到很多别人写的JavaScript代码。如果我们要使用别人写的某个包，每次都根据名称搜索一下官方网站，下载代码，解压，再使用，非常繁琐。于是一个集中管理的工具应运而生：大家都把自己开发的模块打包后放到npm官网上，如果要使用，直接通过npm安装就可以直接用，不用管代码存在哪，应该从哪下载。
更重要的是，如果我们要使用模块A，而模块A又依赖于模块B，模块B又依赖于模块X和模块Y，npm可以根据依赖关系，把所有依赖的包都下载下来并管理起来。
npm已经在Node.js安装的时候顺带装好了。我们在命令提示符或者终端输入npm
 -v
编辑hello.js
'use strict';console.log('Hello, world.');
第一行总是写上'use
 strict';是因为我们总是以严格模式运行JavaScript代码，避免各种潜在陷阱。
运行
node hello.js
每个文件都写上'use
 strict';很麻烦。我们可以给Nodejs传递一个参数，让Node直接为所有js文件开启严格模式：
node
*--use_strict calc.js*
在编写JavaScript代码的时候，完全可以一边在文本编辑器里写代码，一边开一个Node交互式命令窗口，在写代码的过程中，把部分代码粘到命令行去验证，事半功倍！前提是得有个27'的超大显示器！
IDE开发调试工具Visual Studio Code可以跨！平！台！
模块！
一个.js文件就称之为一个模块
相同名字的函数和变量完全可以分别存在不同的模块中，因此，我们自己在编写模块时，不必考虑名字会与其他模块冲突。
模块的名字就是文件名
module.exports = greet;
把函数greet作为模块的输出暴露出去，这样其他模块就可以使用greet函数了。
'use strict';*// 引入hello模块:***var**
 greet = **require**('./hello');**var**
 s = 'Michael';greet(s);
*// Hello, Michael!*
引入的模块作为变量保存在greet变量中，
那greet变量到底是什么东西？
其实变量greet就是用module.exports
 = greet;输出的greet函数。
所以，main.js就成功地引用了hello.js模块中定义的greet()函数，接下来就可以直接使用它了。
一个模块想要对外暴露变量（函数也是变量），可以用module.exports
 = variable;，一个模块要引用其他模块暴露的变量，用var ref = require('module_name');就拿到了引用模块的变量。
引入的对象具体是什么，取决于引入模块输出的对象。
输出多个
module.exports = {
greet: greet,
hi: hi,
goodbye: goodbye
};
使用
consthello=require('./hello');
hello.greet(s);
hello.goodbye(s);
JavaScript有且仅有一个全局对象，在浏览器中，叫window对象。而在Node.js环境中，也有唯一的全局对象，但不叫window，而叫global
这个对象的属性和方法也和浏览器环境的window不同。
进入Node.js交互环境，可以直接输入：
>
global**.console**
**试试**
JavaScript程序是由事件驱动执行的单线程模型，Node.js也不例外。Node.js不断执行响应事件的JavaScript函数，直到没有任何响应事件的函数可以执行时，Node.js就退出了。
如果我们响应exit事件，就可以在程序即将退出时执行某个回调函数：
*// 程序即将退出时的回调函数:*process.on('exit',
**function** (code)
 { console.log('about to exit with code: '
 + code);});

判断JavaScript执行环境
**if**
 (**typeof**(window)
 === 'undefined') { console.log('node.js');}
**else** { console.log('browser');}
文件系统
异步读文件
**var**
 fs = **require**('fs');fs.readFile('sample.txt',
'utf-8',
**function** (err,
 data) { **if**
 (err) { console.log(err); } **else**
 { console.log(data); }});
当正常读取时，err参数为null，data参数为读取到的String。
当读取发生错误时，err参数代表一个错误对象，data为undefined。
这也是Node.js标准的回调函数：第一个参数代表错误信息，第二个参数代表结果。后面我们还会经常编写这种回调函数。
读取一个图片文件
fs.readFile('sample.png',
**function** (err,
 data) { **if**
 (err) { console.log(err); } **else**
 { console.log(data); console.log(data.length + ' bytes');
 }});
当读取二进制文件时，不传入文件编码时，回调函数的data参数将返回一个Buffer对象。在Node.js中，Buffer对象就是一个包含零个或任意个字节的数组（注意和Array不同）。
Buffer对象可以和String作转换，例如，把一个Buffer对象转换成String：
*// Buffer -> String***var**
 text = data.toString('utf-8');console.log(text);
或者把一个String转换成Buffer：
*// String -> Buffer***var**
 buf = Buffer.from(text, 'utf-8');console.log(buf);
同步读文件
同步读取的函数和异步函数相比，多了一个Sync后缀，并且不接收回调函数，函数直接返回结果。
**var**
 data = fs.readFileSync('sample.txt',
'utf-8');console.log(data);
如果同步读取文件发生错误，则需要用try...catch捕获该错误：
**try**
 { **var** data
 = fs.readFileSync('sample.txt',
'utf-8'); console.log(data);}
**catch** (err)
 { *// 出错了*}
写文件
fs.writeFile
fs.stat(filepath,
**function** (err,
 stats) { **if**
 (!err && stats.isFile()) { *// 没有出错并且文件存在:* console.log('200
 ' + request.url);
*// 发送200响应:* response.writeHead(200);
*// 将文件流导向response:*
 fs.createReadStream(filepath).pipe(response);
 } **else** {
*// 出错了或者文件不存在:*
 console.log('404 ' + request.url);
*// 发送404响应:* response.writeHead(404);
 response.end('404 Not Found');
 } });
由于response对象本身是一个Writable
 Stream，直接用pipe()方法就实现了自动读取文件内容并输出到HTTP响应。
stream流
在Node.js中，流也是一个对象，我们只需要响应流的事件就可以了：data事件表示流的数据已经可以读取了，end事件表示这个流已经到末尾了，没有数据可以读取了，error事件表示出错了。
*// 打开一个流:***var**
 rs = fs.createReadStream('sample.txt',
'utf-8');rs.on('data',
**function** (chunk)
 { console.log('DATA:') console.log(chunk);});rs.on('end',
**function** ()
 { console.log('END');});rs.on('error',
**function** (err)
 { console.log('ERROR: ' +
 err);});
要注意，data事件可能会有多次，每次传递的chunk是流的一部分数据。
就像可以把两个水管串成一个更长的水管一样，两个流也可以串起来。一个Readable流和一个Writable流串起来后，所有的数据自动从Readable流进入Writable流，这种操作叫pipe。
Node.js中，Readable流有一个pipe()方法，就是用来干这件事的。
HTTP模块
Node.js开发的目的就是为了用JavaScript编写Web服务器程序。
request对象封装了HTTP请求，我们调用request对象的属性和方法就可以拿到所有HTTP请求的信息；
response对象封装了HTTP响应，我们操作response对象的方法，就可以把HTTP响应返回给浏览器。
用Node.js实现一个HTTP服务器程序非常简单。
'use strict';*// 导入http模块:***var**
 http = **require**('http');*// 创建http server，并传入回调函数:***var**
 server = http.createServer(**function**
 (request, response) { *// 回调函数接收request和response对象,**// 获得HTTP请求的method和url:*
 console.log(request.method + ': '
 + request.url); *// 将HTTP响应200写入response, 同时设置Content-Type: text/html:*
 response.writeHead(200,
 {'Content-Type':
'text/html'});
*// 将HTTP响应的HTML内容写入response:*
 response.end('<h1>Hello world!</h1>');});*// 让服务器监听8080端口:*server.listen(8080);console.log('Server
 is running at http://127.0.0.1:8080/');
使用node ***.js执行即可
解析URL：
**var**
 url = **require**('url');console.log(url.parse('http://user:pass@host.com:8080/path/to/file?query=string#hash'));
结果如下：
Url
 { protocol:
'http:', slashes: true, auth:
'user:pass', host:
'host.com:8080', port:
'8080', hostname:
'host.com', hash:
'#hash', search:
'?query=string', query:
'query=string', pathname:
'/path/to/file',
 path: '/path/to/file?query=string',
 href: 'http://user:pass@host.com:8080/path/to/file?query=string#hash'
 }
处理本地文件目录需要使用Node.js提供的path模块
**var**
 path = **require**('path');*// 解析当前目录:***var**
 workDir = path.resolve('.');
*// '/Users/michael'**// 组合完整的文件路径:当前目录+'pub'+'index.html':***var**
 filePath = path.join(workDir, 'pub',
'index.html');*// '/Users/michael/pub/index.html'*
加密和哈希
Nodejs用C/C++实现这些算法后，通过cypto这个模块暴露为JavaScript接口，这样用起来方便，运行速度也快。
MD5是一种常用的哈希算法，用于给任意数据一个“签名”。这个签名通常用一个十六进制的字符串表示：
**onst**
 crypto = **require**('crypto');**const**
 hash = crypto.createHash('md5');//也可以用sha1
*// 可任意多次调用update():*hash.update('Hello,
 world!');hash.update('Hello,
 nodejs!');console.log(hash.digest('hex'));
*// 7e1977739c748beac0c0fd14fd26a544*
Hmac算法也是一种哈希算法
**const**
 crypto = **require**('crypto');**const**
 hmac = crypto.createHmac('sha256',
'secret-key');hmac.update('Hello,
 world!');hmac.update('Hello,
 nodejs!');console.log(hmac.digest('hex'));
*// 80f7e22570...*
AES是一种常用的对称加密算法，加解密都用同一个密钥。
加密结果通常有两种表示方法：hex和base64
