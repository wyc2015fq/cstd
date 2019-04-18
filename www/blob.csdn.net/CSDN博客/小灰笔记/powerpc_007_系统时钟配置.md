# powerpc_007_系统时钟配置 - 小灰笔记 - CSDN博客





2018年09月24日 10:35:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：110








       这并不是一个重复的小结课题！

从这一次小结开始，打算切换一个新的工程。少用汇编，多用C语言，毕竟C语言在工程实践中应用更加广泛。同时，切换一个ST的IDE，因为我发现这里面拥有类似ARM的SDK驱动代码生成方式。SDK的驱动代码生成并不是很新鲜的东西，但是ST的SPC5Studio是免费的，这就在学习上值得去尝试了。正好，我手头的MPC5634正好有ST与之对应兼容的芯片，SPC563M。

       这样，主要的学习应该大致脱离了寄存器的细致了解，而是转向了对接口功能的理解。

       配置具体的驱动之前，首先要选对MCU的型号：

![](https://img-blog.csdn.net/20180924103525560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       软件的大致环境准备描述直接略过，这部分可以通过帮助手册来进行相应操作的了解。环境准备完之后，着手配置驱动。

       首先，配置一下系统时钟。没有什么特别的要求，我想把我的MCU的性能配置到最高，也就是80M的主频。

![](https://img-blog.csdn.net/20180924103525538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       正好默认的配置跟手头的板子相匹配，直接就可以OK完成了。这样，我的第一个驱动实际上已经完成了。如果有外部晶振的修改，可以修改其他的配置参数，我在一个12M晶振的开发板上做过测试，实现起来倒也简单，其实只需要修改一个参数。就拿12M晶振来说，只需要把下面截图中的2修改为3即可。

![](https://img-blog.csdn.net/20180924103525571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       关于这部分的配置，倒是真离不开芯片手册了。接下来，简单修改软件，看看这个工程是否能够正常启动运行。

![](https://img-blog.csdn.net/20180924103525580?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       软件编辑上，把emacs也切换成了VS Code，做一个临时的与时俱进吧！按照我们的期待，上面的main_counter应该能够持续累加。volatile的增加主要是为了防止这个没有实际作用的操作被编译器优化。

       编译、烧写、测试：

       值得一说的是ST的这个IDE还带有资源统计的功能，十分人性化。

![](https://img-blog.csdn.net/20180924103525579?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       执行的时候抓取的执行状态截屏：

![](https://img-blog.csdn.net/20180924103525616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       数值在不断增加，从上面的结果看，至少MCU是在运行了。时钟配置的验证，则可以到后面的任务调度阶段看任务调度的准确性了。



