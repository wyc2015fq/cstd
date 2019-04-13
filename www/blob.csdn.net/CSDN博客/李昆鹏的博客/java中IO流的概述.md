
# java中IO流的概述 - 李昆鹏的博客 - CSDN博客


2018年04月04日 11:19:23[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：45


-------------------------------------java中IO流的概述----------------------------
1.    IO的概述
IO就是input/output的组合。输入和输出，指的是某个设备或环境进行数据的输入或者输出。
例如：键盘的输入，再比如显示器就是输出的设备，输出图像。
对于java来说输入输出问题，java将它抽象成对象来解决。
以游戏程序为中心读取文件就是输入，写入文件是输出。
![](https://img-blog.csdn.net/20180404111855418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
IO流在java中从输入输出角度分类：
1.    输入流
2.    输出流
IO流在java中从数据的角度来分类：
1.    字符流
文本，我们能读的懂的都可以认为是字符流
2.    字节流
二进制的数据，这种数据我们一般读不懂。
比如：图片文件，MP3文件。

