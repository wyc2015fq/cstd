# 039_AUTOSAR学习笔记_EB的编译环境修改 - 小灰笔记 - CSDN博客





2018年12月15日 12:55:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：187








         使用EB的Demo，自己加了一点代码发现不能够自动识别。看起来，还是不能够自适应、自识别的编译环境。找了一下各个命令中调用的程序以及使用的文件，最终找到了如何扩充增加自己的软件文件。

![](https://img-blog.csdnimg.cn/20181215125539141.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         文件是左边标注的文件，修改如右边的管理配置。再次编译，我自己增加的文件已经被编译进去了。

         在EB的配置工具中，我做了简单的修改。

![](https://img-blog.csdnimg.cn/20181215125539139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         增加了一个回调函数，这个放到了我自己的代码中，暂时是一个空函数。最终编译成功，从输出的map文件中可以查看如下：

![](https://img-blog.csdnimg.cn/20181215125539143.png)

         看起来，这个小demo还是可以继续玩玩的。



