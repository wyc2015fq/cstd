# java程序占用CPU过高问题排查 - Orisun - 博客园







# [java程序占用CPU过高问题排查](https://www.cnblogs.com/zhangchaoyang/articles/5988061.html)




- 查看进程占多少CPU
#top -p 25484
![](https://images2015.cnblogs.com/blog/103496/201610/103496-20161022173522107-713169063.png)
- 查看进程里各个线程占多少CPU
#top -p 25484 -H
![](https://images2015.cnblogs.com/blog/103496/201610/103496-20161022173719607-1824140505.png)
- 查看线程的执行栈信息
先把线程号转换为16进制。
#printf '%0x\n' 28616
6fc8
再通过jstack命令看看这个线程在干嘛
$jstack 25484 | grep -A 10 6fc8
连续执行上述jstack命令多次，不出意料你会发现代码总是停留在某一行或某一个循环块内，如果是停留在某一行说明该行代码执行起来非常耗CPU，如果是停留在某一个循环块内说明这是个死循环。












