# Linux BIOS启动代码DS与ES寄存器初始化分析 - 小灰笔记 - CSDN博客





2017年08月24日 00:40:12[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：426








       学习Linux内核的时候看到一段代码最初有点不解，具体的代码如下：

go:  mov ax,cs

       mov ds,ax

       mov es,ax

       上面的代码是对ds与es两个寄存器的初始化，但是从一般程序设计的角度考虑，总觉得第一句似乎是么有什么意义。为什么不修改成直接用cs寄存器对ds和es两个寄存器进行初始化呢？就向下面这样：

go:  mov ds,cs

       mov es,cs

       我觉得我似乎发现了一点小问题，但是毕竟这教程是中国人编辑的，随便写写也是有可能的。何况，书中我已经发现过其他的错误了。后来查看Linus的源代码发现居然也是这个样子，一下子觉得应该不会是不谨慎造成的，一定是有其他原因。我按照上面自己思考的方式修改了代码测试结果如下：

![](https://img-blog.csdn.net/20170824003902571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       启动的时候出现的居然是乱码！怎么回事儿呢？维基百科查了一下恍然大悟。原来cs寄存器被称为代码的段寄存器，用来指示处理器读取将要执行的指令地址。这样的话，这个值肯定是一直在变的，无法将ds和es进行正确的初始化。



