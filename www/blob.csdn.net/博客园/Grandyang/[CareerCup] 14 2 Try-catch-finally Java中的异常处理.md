# [CareerCup] 14.2 Try-catch-finally Java中的异常处理 - Grandyang - 博客园







# [[CareerCup] 14.2 Try-catch-finally Java中的异常处理](https://www.cnblogs.com/grandyang/p/4946889.html)







14.2 In Java, does the finally block get executed if we insert a return statement inside the try block of a try-catch-finally?



这道题问我们Java中的finally块是否会被执行，当我们在try中加入了返回return。

答案是即便try中加入了return或者continue或者break等命令，finally块仍然会被执行。但是下列两种情况下finally里的内容不会被执行：

1. 当虚拟机Virtual Machine在try/catch模块中就退出了的时候

2. 当线程在处理try/catch模块就结束了的时候












