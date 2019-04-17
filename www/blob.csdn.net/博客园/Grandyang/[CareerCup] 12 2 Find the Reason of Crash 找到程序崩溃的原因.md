# [CareerCup] 12.2 Find the Reason of Crash 找到程序崩溃的原因 - Grandyang - 博客园







# [[CareerCup] 12.2 Find the Reason of Crash 找到程序崩溃的原因](https://www.cnblogs.com/grandyang/p/4906561.html)







12.2 You are given the source to an application which crashes when it is run. After running it ten times in a debugger, you find it never crashes in the same place. The application is single threaded, and uses only the C standard library. What programming errors could be causing this crash? How would you test each one?



这道题说有个应用程序运行的时候崩溃了，在调试运行了十次后，发现在同一个地方不崩溃了。这个应用是单线程的，只用了C语言的标准库，问什么程序原因会导致这个崩溃，怎么样检测每一个原因。根据书上的描述，我们先来看一个会导致程序崩溃的通常的原因：

1. 随机变量。程序有时候会用一些随机数字或变量，每次运行时值都不同。

2. 未初始化的变量。程序有未初始化的变量，在有些程序语言中，这可能会赋值为任意值。

3. 内存泄露。程序有时候会用尽内存，也包括堆和栈的溢出。

4. 外部依赖：程序有时候会依赖其他的应用，机器或资源。如果有很多的外部依赖，很有可能程序会在某一点崩溃掉。

既然程序不在同一个地方崩溃，那么有可能是和某一个部件或场景相关联。例如，可能程序在打开后，什么都不动的情况下是好的，但是当读入了一个文件，或者有了其他的操作时，崩溃了。那么我们就最好用排除法来找原因，关掉所有其他的应用，仔细追踪各种资源。如果有部分程序可以禁用，那么就禁用掉，然后在不同的机子上跑程序看是否会遇到同样的问题。

另外我们也可以借助一些专业的工具来帮我们找错，比如对于问题2，未初始化的变量，就有专门的工具来替我们找到这些变量。












