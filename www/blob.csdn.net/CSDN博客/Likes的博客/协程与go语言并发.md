# 协程与go语言并发 - Likes的博客 - CSDN博客
2018年11月14日 10:11:44[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：27
[https://www.cnblogs.com/liang1101/p/7285955.html](https://www.cnblogs.com/liang1101/p/7285955.html)
[https://studygolang.com/articles/10112](https://studygolang.com/articles/10112)
简而言之，协程与线程主要区别是它将不再被内核调度，而是交给了程序自己调度（意味着程序员需要自己编写调度代码），而线程是将自己交给内核调度。
而go语言封装了这种调度，使协程的调度对用户透明，让用户感觉自己是在使用线程一样。
协程的运用：由于系统能产生的线程数有限，我们一般不使用每个线程维护一个用户连接，而使用IO复用的异步模型，但是IO复用代码的编写远远复杂于每个线程维护一个连接。而协程可以让我们使用一个线程维护一个连接的方式进行编写，而不用担心协程数有限。
