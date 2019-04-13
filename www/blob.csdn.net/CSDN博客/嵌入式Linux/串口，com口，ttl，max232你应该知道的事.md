
# 串口，com口，ttl，max232你应该知道的事 - 嵌入式Linux - CSDN博客

2019年04月08日 20:35:31[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：26


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9rxbe2TVyIY1o49wJ1ccDW451uh1Io1chWxOXDXQw6JiamfJTLYCJjZg/640?wx_fmt=jpeg)
１
今天，说几个比较基础的知识，大家在开发过程中经常会遇到但是又不是特别注意的知识点。


TTL电平：
TTL是Transistor-Transistor Logic，即晶体管-晶体管逻辑的简称，它是计算机处理器控制的设备内部各部分之间通信的标准技术。TTL电平信号应用广泛，是因为其数据表示采用二进制规定，+5V等价于逻辑"1"，0V等价于逻辑"0"。
数字电路中，由TTL电子元器件组成电路的电平是个电压范围，规定：
输出高电平>=2.4V，输出低电平<=0.4V；
输入高电平>=2.0V，输入低电平<=0.8V。
噪声容限是0.4V。

RS232电平：
RS232是串口的一个标准，它有自己的电平定义：它采用负逻辑，-15V~-3V代表逻辑"1"，+3V~+15V代表逻辑"0"。

2
串口，COM口就是电脑上的物理接口形式，COM口简称串口。而TTL，RS-232,是电平标准，就是多少电压范围代表逻辑“ 1 ”，多少电压范围代表逻辑“ 0 ”。大家都知道在电子的世界里面就是 0 和 1 而已嘛。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9ic8VoricoGF6ZOLQCq83g6gFoX9tic8sZOK5G4GXkvqEIhl1GzuNepqAQ/640?wx_fmt=jpeg)
下图是 USB 转 RS232 的转换线，我们看到很多线，只要是 9 pin 的，就可以猜测这个鬼东西是 RS232 的串口线，如果是 排针引出的，可以认为是 TTL 电平的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9UqOHGHoGP9TLl3xlVTkOFKtvPlESnqbXqL3SldHeiaA9O4xNLVIEI2A/640?wx_fmt=jpeg)

PL2303、CP2102 芯片是 USB 转成 TTL 串口的芯片，我们以前在学习 51 单片机、 STM32 经常这样用，也要注意端口的电平是多少 V 。而 MAX232 芯片是 TTL 电平 和 RS232 电平的双向转换芯片。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9dF31fzD4Br57WULncHicczicNo3FqjsWIdfySb8AbTrvOibg2lITV5IUw/640?wx_fmt=gif)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9Va6CV0AKRI4E83fkiaLgjyIUakCUdkUiaOibrgcBfcibPZKGQNWLdKfJwQ/640?wx_fmt=png)
为什么有了 TTL 电平还需要 RS232 和 RS485 两种电平呢？
TTL 电平传输距离比较近，主要用于芯片之间的传输，RS232 和 RS485 数据传输距离比较远。

3
一般从芯片里引出的UART口就是TTL电平，其引脚如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9hYicCAGy8icTSySPhm9icJeYvavlw7tnibSWcdK8uau4ySOciaQYAuMiavhg/640?wx_fmt=png)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9dF31fzD4Br57WULncHicczicNo3FqjsWIdfySb8AbTrvOibg2lITV5IUw/640?wx_fmt=gif)
学习 STC 单片机的同学应该记得，我们以前开发的时候，都是用串口线给单片机下载程序的，用到的串口就是 RS232 ，类似下面这样的，工作以后公司配的电脑都没有了这个东西，笔记本也没有了，大家用的最多的都是 USB 转串口线。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9dF31fzD4Br57WULncHicczicNo3FqjsWIdfySb8AbTrvOibg2lITV5IUw/640?wx_fmt=gif)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9wnI03uPKGIaBWwsic08Ms58ueA80dHwLS4FL7Cmwf0VOrRSYGPQL3kA/640?wx_fmt=png)

4
最后说一个我遇到串口调试比较奇葩是问题，我最近做一个 Android 设备上的串口 透传实验，前两周做好了 demo 收发数据都没有问题，我发送 0123456789ABCDEF 在接收端也能正常接收，今天过来调试的时候发现了问题，发现接收到的数据变成了 7F6E5D4C3B2A1908 （而且每次都是这样的稳定数据），我当时看到这个数据，简直是要疯了的感觉，然后我自己又用命令行 echo cat 来调试，结果还是一样的悲剧，实在没办法了，就约了小 C 去楼下喝了一杯水，回来把设备上的串口线重新插拔了下，结果，你们知道的，就那么好了。希望这里给大家一点帮助，调试之前一定要保证硬件设备是 OK 的，而且之前的调试记录和软件硬件环境要记录好，这样会减少排查问题的时间。

这样的解决办法不是正向的，N 年前，面试的时候，有个面试官问我一个很刁钻的问题，说遇到难题的时候如何正向解决，就是说知道问题了，不能倒推验证，相反，我是比较喜欢倒推验证的，「不管白猫黑猫，抓到老鼠的就是好猫」，在学习知识道路中，会遇到很多人质疑你，做自己就好，不一定要迎合别人。

共勉。

5
推荐两个东西，一个是韦东山老师的视频课程，大家可以通过我来购买，其他优惠我不敢保证，保证能跟我一起学习吧，有问题我也优先给解答，学习的路上，多我一个，不孤单，生活的路上，多我一个，会很逗逼。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9gypTI5fmR7iar85MiaYRiab7FIBaPWibt4Z4z8RW7X3HLGpDKl6FqNZRtw/640?wx_fmt=png)
还有一个就是我的知识星球了，如果大家有问题想找我提问，有技术难题需要讨论的，欢迎加我的知识星球，当然，我知道的问题不仅仅是技术问题。



![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9plAx6aqIGnVPU4zKx89uA7uCN1GDSx6wNFXvgauo87jOIa7SbTVG0A/640?wx_fmt=gif)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPc92NpKfNV0eTiaKy3IRjDic9KQuA27msqmQXtMaYiaCyr3UK56DIYqiay6AMVs7hekynnsfhMBIsV19g/640?wx_fmt=jpeg)



