# NodeJS入门(4)-网络操作 - 零度的博客专栏 - CSDN博客
2016年11月02日 14:49:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：488
### 网络编程简介：
1.NodeJS本来的用途是编写高性能的Web服务器。下面是一个使用NodeJS内置的http模块简单实现HTTP服务器的例子：
 var http = require('http');
 http.createServer(function (request, response) {
     response.writeHead(200, { 'Content-Type': 'text-plain' });
     response.end('Hello World\n');
 }).listen(8124);
 运行后，直接用浏览器访问[http://127.0.0.1:8124/](http://127.0.0.1:8124/)就能看到效果。（linux下可能需要权限）
### 网络编程API:
1.在NodeJS中，可以通过‘process’对象感知和控制NodeJS自身进程的方方面面。另外需要注意的是，‘process’不是内置模块，而是一个全局对象，因此在任何地方都可以直接使用。
     Process:
2.使用'child_process'模块可以创建和控制子进程。该模块提供的API中最核心的是'.spawn'，其余API都是针对特定使用场景对它的进一步封装，算是一种语法。
    Child Process:
3.'cluster'模块是对'child_process'模块的进一步封装，专用于解决单进程NodeJS Web服务器无法充分利用多核CPU的问题。使用该模块可以简化多进程服务器程序的开发，让每个核上运行一个工作进程，并统一通过主进程监听端口和分发请求。
### 典型应用场景:
1.如何获取命令行参数：在NodeJS中可以通过‘process.argv’获取命令行参数。但是比较意外的是，node执行程序路径和主模块文件路径固定占据了‘argv[0]’和‘argv[1]’两个位置，而第一个命令行参数从‘argv[2]’开始。为了让‘argv’使用起来更加自然，可以按照以下方式处理。
 function main(argv) {
        // ...
 }
 main(process.argv.slice(2));    
2.如何退出程序：正常退出状态码为0,如果遇到异常想手工退出，可以指定该错误码。
try {         // ...
 } catch (err) {
    // ...
    process.exit(1);
}
3.如何输入输出：NodeJS程序的标准输入流、一个标准输出流、一个标准错误流分别是：process.stdin,process.stdout,process.stderr,第一个是只读数据流，后两个是只写数据流。下面是一个例子：
 unction log() {
        process.stdout.write(
        util.format.apply(util, arguments) + '\n');
 }
4.linux下，需要root权限才能监听1024以下端口，但是完成监听后，不能一直让程序在root权限下运行，否则会存在安全陷患，因此最好能把权限降下来。下面是个例子：
 http.createServer(callback).listen(80, function () {
         var env = process.env,
         uid = parseInt(env['SUDO_UID'] || process.getuid(), 10),
         gid = parseInt(env['SUDO_GID'] || process.getgid(), 10);
         process.setgid(gid);
         process.setuid(uid);
 });
      上例中有几点需要注意：
      ◦如果是通过sudo获取root权限的，运行程序的用户的UID和GID保存在环境变量SUDO_UID和SUDO_GID里边。如果是通过chmod +s方式获取root权限的，运行程序的用户的UID和GID可直接通过process.getuid和process.getgid方法获取。
      ◦process.setuid和process.setgid方法只接受number类型的参数。
      ◦降权时必须先降GID再降UID，否则顺序反过来的话就没权限更改程序的GID了。
5.创建子进程：
var child = child_process.spawn('node', [ 'xxx.js' ]);//第一个参数是执行文件路径，可以是执行文件的相对或绝对路径，也可以是根据PATH环境变量能找到的执行文件名。第二个参数中，数组中的每个成员都按顺序对应一个命令行参数。第三个参数可选，用于配置子进程的执行环境与行为。
    child.stdout.on('data', function (data) {
        console.log('stdout: ' + data);
    });
    child.stderr.on('data', function (data) {
        console.log('stderr: ' + data);
    });
    child.on('close', function (code) {
        console.log('child process exited with code ' + code);
    });
另外，上例中虽然通过子进程对象的.stdout和.stderr访问子进程的输出，但通过options.stdio字段的不同配置，可以将子进程的输入输出重定向到任何数据流上，或者让子进程共享父进程的标准输入输出流，或者直接忽略子进程的输入输出。
1.进程间的通讯
在linux下，进程间通过信号相互通讯，例子如下：
    /* parent.js */
    var child = child_process.spawn('node', [ 'child.js' ]);
    child.kill('SIGTERM');
    /* child.js */
    process.on('SIGTERM', function () {
        cleanUp();
        process.exit(0);
    });
在上例中，父进程通过.kill方法向子进程发送SIGTERM信号，子进程监听process对象的SIGTERM事件响应信号。不要被.kill方法的名称迷惑了，该方法本质上是用来给进程发送信号的，进程收到信号后具体要做啥，完全取决于信号的种类和进程自身的代码。
在NodeJS中，如果父子进程都是NodeJS进程，就可以通过IPC(进程间通讯)双向传输数据：
    /* parent.js */
    var child = child_process.spawn('node', [ 'child.js' ], {
       stdio: [ 0, 1, 2, 'ipc' ]
    });
    child.on('message', function (msg) {
        console.log(msg);
    });
    child.send({ hello: 'hello' });
    /* child.js */
    process.on('message', function (msg) {
        msg.hello = msg.hello.toUpperCase();
        process.send(msg);
    });
可以看到，父进程在创建子进程时，在options.stdio字段中通过ipc开启了一条IPC通道，之后就可以监听子进程对象的message事件接收来自子进程的消息，并通过.send方法给子进程发送消息。在子进程这边，可以在process对象上监听message事件接收来自父进程的消息，并通过.send方法向父进程发送消息。数据在传递过程中，会先在发送端使用JSON.stringify方法序列化，再在接收端使用JSON.parse方法反序列化。
2.如何守护子进程
守护进程一般用于监控工作进程的运行状态，在工作进程不正常退出时重启工作进程，保障工作进程不间断运行。
/* daemon.js */
function spawn(mainModule) {
    var worker = child_process.spawn('node', [ mainModule ]);
    worker.on('exit', function (code) {
        if (code !== 0) {
            spawn(mainModule);
        }
    });
}
spawn('worker.js');
可以看到，工作进程非正常退出时，守护进程立即重启工作进程。
