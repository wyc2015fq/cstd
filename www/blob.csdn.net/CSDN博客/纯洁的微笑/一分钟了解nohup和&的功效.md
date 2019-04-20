# 一分钟了解nohup和&的功效 - 纯洁的微笑 - CSDN博客
2018年07月19日 08:00:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：1387
nohup和&究竟有啥区别？不少同学进行了回复，但并不是所有同学都理解得全对，今天把自己挖的坑自己填了。
测试代码如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76fb9vsibTBIuQwnBoavsGLyO635pr5yRrCDF3ic3A5D5nTvhHEaugaJ8g/640?wx_fmt=png)
是一个输出hello与循环轮数的死循环程序，每输出一行就休眠1秒。
**使用 ./a.out 前台运行程序，会是什么效果呢？**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76oQ0UU4AHryictIwyvicNsPwMnCRkwTLurTicCHvMLowFIm4eKzgytXghg/640?wx_fmt=png)
程序每隔一秒会在终端输出一个字符串。
此时如果键入Ctrl+C ，程序会收到一个SIGINT信号，如果不做特殊处理，程序的默认行为是终止（如上图）。
**使用 ./a.out& 后台运行程序，会是什么效果呢？**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO767dxz21mFhdkbPFCgDcRMLz7YiaoWxniaJ91QdLA3H733gUsOHhyTeBrA/640?wx_fmt=png)
如上图：
- 
首先会在终端显示进程号是32389
- 
键入Ctrl + C，发出SIGINT信号，程序会继续运行
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76WK0ibLlA36lPialGXaicqufkTrtvb8iaaBpqqVpSQpblyj9qNo6Wc6ZdQQ/640?wx_fmt=png)
ps确认一下，确认进程依然在运行，进程号是32389。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76yRtTFxuhkD2SJv1m7WI2AU2B8S7aF4Y1k6Riaf2e4M3yFYT63Lmjq7w/640?wx_fmt=png)
此时如果关掉session，程序会收到一个SIGHUP信号，此时会怎么样呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76QKUB2MAOibiaY1mhCAg5lBzJsqiaheR4s4bDgGeiasveV7GuCicrlnsPqEA/640?wx_fmt=png)
ps再次确认，可以看到关闭session之后，进程号是32389的a.out进程也关闭了。
**使用nohup ./a.out 又会是什么效果呢？**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76o6ztZicic8YfOffeT88kMT27Rb7so8yQYZIia9TkAQwjxukrDxX5Ab4xg/640?wx_fmt=png)
使用nohup 运行程序a.out，会发现：
- 
前台没有出现进程号
- 
有一个“忽略输入，输出至nohup.out”的提示
- 
hello的输出也没有出现在前台
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76IxFNRAmJyMZKHv2PGnQV9fqRRU73oG6WHjE4fb1pFKgibt0MvWaKxxw/640?wx_fmt=png)
手动ps看进程号，这次a.out的进程号是32437。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76JYC9AzsQB7ica6qYnNaqxdJFQMuuvmtheOtLC0FSMAQwLIG4Jzo5yHg/640?wx_fmt=png)
此时如果关掉session，程序会收到一个SIGHUP信号，程序会不会关闭呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76JftXKRrNV8rsvZ3qdhdCYkGlHZciajyfdaBoFqJGbmgze9qIlF9gPAw/640?wx_fmt=png)
关掉session后，再次ps看一下，ID为32437的a.out进程还在。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76u3DL6TyTIOUfvFFr3GYRS55ZMywo0SmicQzkuqtNgPFFJQAicOBahmqA/640?wx_fmt=png)
这些只能通过kill把程序干掉了，killall之后，ps查看进程已经关闭。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76PE4tLh0PX3FR6aAT3iawSSYqyvwMfFTT3qqPYMUaibFNetfvVkiab49iaA/640?wx_fmt=png)
killall之后，查看发现多了一个nohup.out文件，不过这个文件的大小是0，有点奇怪，启动程序的时候，明明提示了“appending output to nohup.out”呀，先把问题遗留在这，测试一下Ctrl +C。
仍如上图，使用nohup启动a.out，如果键入Ctrl+C ，程序收到SIGINT信号后，直接关闭了。
**最后测试一下nohup和&同时使用，即用nohup./a.out &运行程序，又会是什么效果呢？**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76ygqVLic9gZsjcFVqL1KsEXJpVs2mXNCdxHzSlgwmoVpItxraic9Lz8tA/640?wx_fmt=png)
使用nohup ./a.out &运行程序后，可以看到：
- 
会在终端显示进程号是32524
- 
也会有一个“忽略输入，输出至nohup.out”的提示
键入Ctrl + C，发送SIGINT信号，似乎没反应。
关闭session，发送SIGHUP信号，再来看看。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YrezxckhYOzzYpIzc7s0cwRyof3PBO76MPbot9NL3ug1RibCict8pCHENVfDFP0wsIwNZfKc0PfyNV4OnnhQQrOQ/640?wx_fmt=png)
ID为32524的进程依然存在，后续也只能用kill来关闭它。
**结论**
使用**&**后台运行程序：
- 
结果会输出到终端
- 
使用Ctrl + C发送SIGINT信号，程序免疫
- 
关闭session发送SIGHUP信号，程序关闭
使用**nohup**运行程序：
- 
结果默认会输出到nohup.out
- 
使用Ctrl + C发送SIGINT信号，程序关闭
- 
关闭session发送SIGHUP信号，程序免疫
**平日线上经常使用nohup和&配合来启动程序**：
- 
同时免疫SIGINT和SIGHUP信号
同时，还有一个最佳实践：
- 
不要将信息输出到终端标准输出，标准错误输出，而要用日志组件将信息记录到日志里
如果觉得治学还算严谨，帮忙转一下。
**尾巴**
nohup启动a.out，程序终止后，为啥nohup.out的大小是0？“hello”的字符串哪去了？
