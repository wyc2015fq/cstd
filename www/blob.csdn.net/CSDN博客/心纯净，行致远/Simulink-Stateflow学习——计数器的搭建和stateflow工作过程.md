# Simulink-Stateflow学习——计数器的搭建和stateflow工作过程 - 心纯净，行致远 - CSDN博客





2018年03月26日 15:14:09[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：4687标签：[matlab																[simulink																[stateflow](https://so.csdn.net/so/search/s.do?q=stateflow&t=blog)
个人分类：[【MatLab/Simulink】](https://blog.csdn.net/zhanshen112/article/category/7184373)





        Stateflow 是一个基于有限状态机和流程图来构建组合和时序逻辑决策模型并进行仿真的环境。它可以将图形表示和表格表示（包括状态转换图、流程图、状态转换表和真值表）结合在一起，针对系统对事件、基于时间的条件以及外部输入信号的反应方式进行建模。

        Stateflow 可用于设计有关监控、任务调度以及故障管理应用程序的逻辑。Stateflow 包括状态图动画及静态和运行时检查，可以在实施前测试设计的一致性和完整性。

        Stateflow可以直接嵌入到Simulink模型中，并且在仿真的初始化阶段，Simulink会把Stateflow绘制的逻辑图形通过编译程序转换成C语言，使二者有机地结合在一起。 Stateflow可以在Simulink Extra模块库中找到。


下面是一个简单易学的stateflow实例：
**题目：在simulink中搭建一个简单的stateflow，实现在输入的上升沿进行计数，然后输出。**

![](https://img-blog.csdn.net/2018032614454255)

    如上图是搭建的stateflow，输入为周期为1的脉冲信号，设定最大值maxtime为18。仿真时间为10s，由于只在上升沿触发，故最后结果只有9。

    下图为stateflow的内部：



![](https://img-blog.csdn.net/20180326144853850)

    输出结果为：

![](https://img-blog.csdn.net/20180326144953968)

    下面详细讲解stateflow内部工作的过程：

        首先讲解stateflow内部的工作过程。stateflow的内部有两个state，reset和count。当chart模块在第一个上升沿的条件下（注意是1s位置的上升沿，0s时没有上升沿）触发时，chart内部会首先进入到reset状态。此时output=0。state进入节点，节点在sitch，即上升沿触发时，两个state都可以传送至节点，而节点传输至哪个状态，则由输出转换线生的条件决定。由于maxtime设定为18，所以后面再2s，3s……一直到10s计数才达到9，不会进入reset。

    哈哈，在刚才继续写的时候突然发现我之前如果调整仿真时间远远大于10s，也就是让计数可以达到18，突然想到为什么结果不是18呢？如下图：

![](https://img-blog.csdn.net/20180326150717253)

        然后才突然意识到output>maxtime出现了问题。。。

![](https://img-blog.csdn.net/20180326150948121)

    然后我把output>maxtime改成output>=maxtime，出来的结果就符合预期了！666

![](https://img-blog.csdn.net/20180326151207964)



注：stateflow的详细搭建过程可参考博客：[http://blog.renren.com/share/407154693/16355969210/1](http://blog.renren.com/share/407154693/16355969210/1)

**增添内容，修改日期2018.07.31**

上述内容stateflow中变量设置和数据管理的问题。下面为增加的内容。

从上面的simulink模型可以看出，stateflow中的输入输出为maxtime和output，如何定义呢？

1、点击model explore，如下图圈中标注区域。

![](https://img-blog.csdn.net/20180731141119359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、依次点击下图中的1,2,3，设置变量，并将scope改为input和output。

![](https://img-blog.csdn.net/2018073114143679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时stateflow就会出现相应的输入输出。即可。](https://so.csdn.net/so/search/s.do?q=simulink&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




