# linux下如何控制RTS？为了控制RS485发送正常，但无法接收到端发送过来的数据 - xqhrs232的专栏 - CSDN博客
2016年03月31日 17:29:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3528
原文地址::[http://blog.csdn.net/myjalo/article/details/42172143](http://blog.csdn.net/myjalo/article/details/42172143)
相关文章
1、Linux串口编程笔记-详细----[http://wenku.baidu.com/link?url=ifHCAioKJCPGyZpXgUVwLdlyRfbsdunWbmfGqRXuBaut2RdRPYT-4u5ML4Tj8uMjDU0kdGHosLr_zll7RuCInp5yfuFSnAz0BnAnA8GcRKC](http://wenku.baidu.com/link?url=ifHCAioKJCPGyZpXgUVwLdlyRfbsdunWbmfGqRXuBaut2RdRPYT-4u5ML4Tj8uMjDU0kdGHosLr_zll7RuCInp5yfuFSnAz0BnAnA8GcRKC)
2、[Linux终端tty设备驱动](http://blog.chinaunix.net/uid-9688646-id-3287609.html) ----[http://blog.chinaunix.net/uid-9688646-id-3287609.html](http://blog.chinaunix.net/uid-9688646-id-3287609.html)
3、[storysnail的Linux串口编程笔记](http://blog.csdn.net/dyllove98/article/details/8915478)----[http://blog.csdn.net/dyllove98/article/details/8915478](http://blog.csdn.net/dyllove98/article/details/8915478)
Linux 下串口编程的文章网上是满天飞，但大都是出自一篇文章，而且写的都是些基本的操作，像控制 RTS/CTS 等串口引脚状态，接收发送二进制数据等，都没有很好的说明，我在使用中遇到了些问题，写出来，希望能对大家有所帮助，少走弯路，呵呵！
       我使用的操作系统是 Redhat9 ， gcc 版本是 3.2.2
       其实在 linux 下对串口的设置主要是通过 termios 这个结构体实现的，但是这个结构体却没有提供控制 RTS 或获得 CTS 等串口引脚状态的接口，可以通过 ioctl 系统调用来获得 / 控制。
获得：
ioctl(fd, TIOCMGET, &controlbits);
if   (controlbits & TIOCM_CTS)
      printf(“ 有信号 \n”);
else
      printf(“ 无信号 \n”);
设置：
       ioctl(fd, TIOCMGET, &ctrlbits) ；
       if (flag)
           ctrlbits |= TIOCM_RTS;
       else
           ctrlbits &= ~TIOCM_RTS;
      ioctl(fd, TIOCMSET, &ctrlbits);
其实 TIOCM_RTS 有效后是把串口的 RTS 设置为有信号，但串口的电平为低时是有信号，为高时为无信号，和用 TIOCMGET 获得的状态正好相反，也就是说 TIOCMGET/TIOCMSET 只是获得 / 控制串口的相应引脚是否有信号，并不反应当前串口的真实电平高低。
网上许多流行的 linux 串口编程的版本中都没对 c_iflag （ termios 成员变量 ）这个变量进行有效的设置，这样传送 ASCII 码时没什么问题，但传送二进制数据时遇到 0x0d,0x11 和 0x13 却会被丢掉。不用说也知道，这几个肯定是特殊字符，被用作特殊控制了。关掉 ICRNL 和 IXON 选项即可解决。
       c_iflag &= ~(ICRNL | IXON);
0x0d 回车符 CR
0x11 ^Q VSTART 字符
0x13 ^S VSTOP 字符
ICRNL 将输入的 CR 转换为 NL  
IXON 使起动 / 停止输出控制流起作用  
在《 UNIX 环境高级编程 第二版》第 18 章第 11 小节看到把终端 I/O 设置为原始模式（串口通讯就是终端 I/O 的原始模式）时输入属性设置为
term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
屏蔽了许多属性，怪不得有人说如果是使用串口通讯 c_iflag 和 c_oflag 都设置为 0 就行了！
以下是我的设置的一些重要的串口属性
term.c_cflag |= CLOCAL | CREAD;
       term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
       term.c_oflag &= ~OPOST;
term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
[](http://blog.csdn.net/myjalo/article/details/42172143#)[](http://blog.csdn.net/myjalo/article/details/42172143#)[](http://blog.csdn.net/myjalo/article/details/42172143#)[](http://blog.csdn.net/myjalo/article/details/42172143#)[](http://blog.csdn.net/myjalo/article/details/42172143#)[](http://blog.csdn.net/myjalo/article/details/42172143#)
- 
顶
0- 
踩
