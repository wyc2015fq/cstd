# 转载和积累系列 - Nodejs实现多进程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月24日 09:32:54[initphp](https://me.csdn.net/initphp)阅读数：2598








一般单独启动一个node是单进程运行的，如何实现多进程运行呢，就像Nginx一样？

实现多进程，需要关注两个node自带模块：

child_process : 子进程模块

net : 网络模块

nodejs的子进程模块提供了一个send函数，这个函数非常有用，可以将主进程的数据传递给子进程

那么也就是说，我们可以通过send函数和子进程进行通信。

那么我们就可以通过net模块，实现一个tcp服务器，然后将handle传递给子进程，子进程去监听同一个端口，就可以实现多进程啦。





主进程：



```java
var child_process = require('child_process');
				var net = require("net");
				var tcp = net.createServer();
				tcp.listen(listen , function() {
					console.log('fork process start');
					console.log('listen:' + listen);
					for (var i = 0; i < processLen; i++) {
						start_process(child_process, tcp, indexFile); //启动进程
					}
					console.log('fork process end\r\n');
					tcp.close();
				});
    /*    
     *    实现进程启动，并且监控进程，如果进程退出，自动调用该函数重新启动
     */
    var start_process = function (child_process, tcp, indexFile) {
        var cp = child_process.fork(indexFile); //生成子进程，indexFile进程文件地址
        cp.send('yes', tcp._handle); //向子进程发送数据
        console.log('pid:' + cp.pid);
        cp.on('exit', function () {
            start_process(child_process, tcp, indexFile);
        })
    }
```


子进程：



```java
//子进程接收主进程handle
				process.on("message",function(m, handle) { //通过进程message来监听主进程传递进来的数据
					http.createServer(_this.on).listen(handle);//运行HTTP服务器	
				});
```









