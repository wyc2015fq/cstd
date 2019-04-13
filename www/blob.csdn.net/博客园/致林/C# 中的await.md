
# C\# 中的await - 致林 - 博客园






# [C\# 中的await](https://www.cnblogs.com/bincoding/p/8065437.html)
今天看代码遇到了很多await，梳理一下await的用法。
await只针对awaitable对象，经常和Task一起使用，主要用法如下：
vartask = RunAsync();//开始执行异步操作DoSomeThing();//主线程开始执行其他操作awaittask;//主线程阻塞，等待task异步执行完成DoOtherThing()//再执行其他操作
**参考用法：**
[创建自己的awaitable类型](http://www.cnblogs.com/TianFang/archive/2012/09/21/2696769.html)
[async和await用法](http://www.cnblogs.com/lonelyxmas/p/3579985.html)
**其他语言中也有相关语法：**
[体验异步的终极解决方案-ES7的Async/Await](http://cnodejs.org/topic/5640b80d3a6aa72c5e0030b6)
另外.net中使用Task，BackgroundWorker，ThreadPool也能很方便的实现异步。





