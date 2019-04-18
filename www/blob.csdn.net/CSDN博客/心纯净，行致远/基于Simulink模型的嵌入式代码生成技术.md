# 基于Simulink模型的嵌入式代码生成技术 - 心纯净，行致远 - CSDN博客





2018年05月22日 16:29:20[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：3516








自动生成代码技术具有**生成的代码效率高、一致性好、开发便捷**等诸多优点，也正是因为这些优点使得这项技术已经成为汽车、航空航天、工业控制等领域嵌入式软件开发的主流方式。本次研讨会将主要讨论如何通过Embedded Coder从Simulink/Stateflow模型生成嵌入式C代码，如何通过数据对象定义C代码中的变量和参数，如何定制C代码的函数原型，如何控制C文件的生成，如何生成可以重用的C代码，如何对被引用的模型进行代码生成，如何进行代码和模型的等效性测试等话题。


simulink的c代码生成工具有3种：

Embedded coder：自动生成嵌入式系统的C/C++代码

Simulink coder：自动生成Simulink/Stateflow的C/C++代码

Matlab coder：自动生成Matlab的C/C++代码

下面是一个比较好的自动代码生成教学视频：[simulink自动代码生成](https://ww2.mathworks.cn/videos/embedded-code-generation-from-simulink-82571.html?elqsid=1526972922872&potential_use=Student)

视频主要内容如下：

1、使用simulink coder生成代码；

2、使用embedded coder生成代码；

然后对以上两种方式生成的C/C++代码进行管理，主要有以下应用场景：

1.使用数据对象进行数据管理；

2.代码生成的优化配置；

3.模型机算法函数原型的控制；

4.子系统算法函数以及文件的控制；

5.从自定义模块生成可重用代码；

6.平台化模型的代码生成；

7.代码的等效性测试（SIL: software in the loop）。

使用数据对象进行数据管理的过程如下：simulink代码生成通过数据对象进行数据管理。具体而言，就是先设置数据对象，然后将数据对象和模型参数关联，就可以通过配置数据对象影响代码生成。

这里的对象和平时所说的面向对象编程是同一个概念，类（class）有一个包，包里面又会包括几个类，而类里面会定义方法（method）和属性（property）。我们可以通过类去创建对象或者说派生对象，然后可以通过类里面的属性值去设置这些类派生对象的属性。

simulink里面有两个包：simulink package、mpt package(专门用于嵌入式代码生成)。

###### 简单的基于simulink的嵌入式代码生成



这篇文章生成的是通用行的C代码，也就是将生成的代码直接加入工程中就可以使用，不存在控制器的差别

模型如下

![](https://img-blog.csdn.net/20180316183855426?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180316183913501?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


第一步：设置解算器

固定步长，离散，0.01s运行一次算法

![](https://img-blog.csdn.net/20180316183932965?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码生成选项，选择ert.tlc，这个生成的嵌入式代码

![](https://img-blog.csdn.net/20180316183950308?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180316183955847?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


代码生成页面

![](https://img-blog.csdn.net/20180316184008876?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看见我们生成的代码中的变量是用结构体进行计算的，那么这么简单的算法，我们能否用平时习惯的变量方式去表示呢，答案是肯定的，这就需要我们进行数据的管理

二、数据管理

首先，双击信号线为信号线添加名称，那么我们就简单的对信号线命名为x，y，z。之后点击2选项

![](https://img-blog.csdn.net/20180316184031437?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180316184036959?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


![](https://img-blog.csdn.net/2018031618410898?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里存储类型一定不能是auto.


![](https://img-blog.csdn.net/20180316184127951?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R0eTMwNjAzNDgzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


之后代码生成crtl+b




