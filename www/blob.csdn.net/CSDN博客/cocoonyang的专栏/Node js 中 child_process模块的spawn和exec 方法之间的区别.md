# Node.js 中 child_process模块的spawn和exec 方法之间的区别 - cocoonyang的专栏 - CSDN博客





2014年02月27日 06:43:13[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1592








 Node.js 子进程模块 (`child_process`) 有两个方法 `spawn` 和`exec`, 这两个方法的功能都是启动一个新进程。

`child_process.spawn` 和 `child_process.exec 两个方法`之间最重要的区别就在于它们的返回对象。`child_process.spawn` 方法返回的是一个带有 标准输出 `stdout` 和 `stderr` 的流对象。`child_process.exec` 返回的是子进程输出缓冲区中所有的内容。
 缓冲区默认容量是 200k，如果子进程返回的数据超过了这个容量限制，程序就会崩溃，相应的错误信息是"Error: maxBuffer exceeded"。通过调整exec 的参数中缓冲容量这一项来可以解决这个问题。但是，如果需要子进程返回大量数据Node 进程，最好使用`child_process.spawn` 方法。

`child_process.spawn` 和 `child_process.exec `两个方法`之间另外一个重要的区别在于它们的子进程异步方式有区别。``child_process.spawn` 是 "异步式异步（asynchronously asynchronous）"，就是说，当一旦子进程启动后，就会向其返回的流对象输送数据。`child_process.exec` 是"同步式异步（synchronously
 asynchronous）"，意思是说尽管子进程执行是按照异步执行的，但是exec方法会等到子进程执行完毕后才将缓冲区内的数据一并返回主进程。








