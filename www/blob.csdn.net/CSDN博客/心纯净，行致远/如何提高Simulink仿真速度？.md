# 如何提高Simulink仿真速度？ - 心纯净，行致远 - CSDN博客





2018年05月03日 15:20:02[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：6094标签：[simulink																[运行效率																[计算速度](https://so.csdn.net/so/search/s.do?q=计算速度&t=blog)
个人分类：[【MatLab/Simulink】																[【总结】																[【思考】](https://blog.csdn.net/zhanshen112/article/category/7184880)




        在基本模型具备的情况下，模型仿真的速度越快，算法的开发速度和效率就越高。在机器性能（CPU/内存/显卡等）一定的情况下，了解如何提高模型仿真速度，很有必要！

        现将网上整理的提高Simulink仿真速度的资料整理如下，有效性还未验证。根据仿真过程将其整理为以下几部分：模型仿真参数的设置（步长、求解器、仿真时间等）、模型模块的搭建、模型显示和输出设置。

#### 模型仿真参数的设置

1、较小的步长或采样时间(或者彼此间不是倍数关系的采样时间)：为了在仿真期间捕获重要事件，有时必须设置最够小的步长;反过来，步长太小会导致产生不必要的输出点，从而减慢仿真速度。 


2、最大步长太小：如果您更改了最大步长，请尝试用默认值(设置为自动)来运行仿真。


3、您要求的精度可能过高：默认相对容差(0.1%精度)通常就足够了。对于状态趋于零的模型，如果绝对容差参数太小，则仿真可能在近零状态值附近采用过多步长请记住，所有要输入的容差值都是绝对值。所以，默认相对容差值设置为 1e-3 时，意味着相对容差是 0.001，或者0.1%(采用百分比形式)。


4、 时间尺度可能太长：减少时间间隔。

##### 求解器选择

5、这可能是个刚性(stiff)问题，而您使用的是非刚性(non-stiff)求解器：尝试使用 ode15s。


总结：在离散系统仿真中，在保证精确度的条件下如果能够增大步长则增大步长，仿真总时间以满足仿真要求为准，尽可能缩短。

#### 模型模块的搭建

1、模型中有一个 MATLAB 函数块 ：当存在 MATLAB Fcn 模块时，在每个采样时间都会调用 MATLAB解释器。这会大大降低仿真速度。所以，应尽可能使用内置Simulink模块。

2、MATLAB S函数(S-Function)：在每个时间步长都会计算S-Function。把MATLAB代码转为MEX文件，这样性能可以得到显著提高。此外，如果可能，应尽量使用内置模块建模。


3、您的模型中包含一个 Memory 模块：使用 Memory 模块会导致在每个步长上变阶求解器(ode15s 和 ode113)被重置回阶数1。


4、Extras 库中有包含以上三项之一(即Graph Scope、Autoscaling Scopes、Spectrumanalyzer等)的Mask模块：Unmask模块来看看它们是否调用 S-Function。


5、代数环：为了解代数环，会在每个步长上都执行迭代计算。因此，它大大降低了速度。


6、不要在积分函数中引入白噪声模块：对于连续系统，使用 Extras/Sources 库中的带限白噪声模块。 


7、您可能碰到了连续过零，导致仿真逐渐“停滞”，时间很长(并且可能是无限长时间)：要解决此问题，可以禁用过零检测。这可以通过在“Simulation参数”(Simulation Parameters) 对话框的“高级”(Advanced) 窗格中选择“禁用过零检测”(Disable zerocrossing detection) 选项来实现。在 R11 中，可通过转到模型的“仿真”(Simulation) ->“参数”(Parameters)->“诊断”(Diagnostics) 部分来实现此目的。 


8、您也可能想要尝试设置“模型参数配置”(Model Parameter Configuration) 对话框以便您的 Simulink模型使用“内联参数”(Inline Parameters) 选项：选择此选项可使 Simulink 能够将指定参数视为常量，从而加快仿真速度。


9、Simulink Accelerator 仿真模式可以加快仿真时间。Profiler可以分析模型性能瓶颈来提高仿真速度。

首先是模型的搭建问题。在Simulink环境中建模时，以s域的表达式进行建模时问题要少一些，增大误差限、取消过零检测等可以提高仿真速度，减少警告信息。而z域的情况下要注意的问题会多一点。第一点是把模型中的代数环(Algebraic Loop)要消除掉;因为z域处理的数据都是一拍一拍按照采样周期处理的，所以如果一个数据既作为输入，同时又无延时地作为输出，就产生了数值处理的问题，造成仿真速度显著下降，处理的方法一般也不难，把反馈加一个延时环节就好了。另外就是仿真步长的问题，在能保证仿真精度的情况下，尽量使用大的步长可以显著提高仿真速度;这个最大的仿真步长自然可以按照香农采样定律来确定，但是一般情况下选择仿真步长为4-10倍的最大采样频率是足够了。

　　其次是可以改变仿真的模式。在老版本Simulink中，这个选择很少，但是新版本有了Normal、Accelerator和Rapid Accelerator、HIL等模式。硬件在回路HIL显然速度最快，但是好多时候并不适用，因为首先要有相应的硬件，其次是仿真对象要支持代码生成。因为Simulink用的是一种解释性的语言，normal模式就可以理解为Matlab解释一句，操作系统执行一句，速度自然不会太快;Accelerator则是把一部分共享模块编译为库文件，例如dll进行调用，相当于混合模式，既有解释-执行，也有直接调用，速度;Rapid Accelerator则是把整个模型编译为操作系统下独立运行的程序，少了Simulink解释给操作系统的工作，自然运行速度快，代价则是需要一定的时间来编译模型;这种模式对PC的硬件配置要求是相对比较高的，内存少于3GB时容易出错。



总结：建模尽可能少使用matlab function、m或者c的S-FUNCTION，而尽量使用内置代数运算模块；避免出现代数环；几分钟避免使用白噪声


#### 模型显示和输出设置

1、使用了Scope模块：尽管它们的影响很可能不明显。

2、使用了动态刷新模块。比如在plot图形上动态刷新显示的点。


　- 如果在仿真过程中有打开的scope或其他可视化输出设备，这些都会降低性

　- 性能与 CPU 速度成正比


　- 如果在仿真过程中存储的变量大于系统上的 RAM 量，则性能会很差。解决办法是运行较小的仿真或增加内存

        再者就是有一些模块会显著拖慢仿真速度，它们相当于“木桶理论”中的那块“短板”了。例如Simulink中的X-Y图这样实时刷新绘图的模块。如果模式中有这个模块，则仿真的时候它会缓慢地刷新X-Y图，仿真速度肯定快不了。如果有别的方法替代则可以加快仿真速度，例如先把数据保存到工作空间里，等仿真结束之后再绘制X-Y图等。一些非线性的模块，例如一个非线性的MOSFET模型，自然也比理想开关所需要的仿真时间长。

        第四种方法是并行执行和分布式执行，相当于多个人一起完成一项工作，前提是要有并行执行的许可和分布式执行的许可。并行执行就是在多核CPU的计算机上，打开多个Matlab，然后自动或者手动分配进行并行处理;分布式执行则是多台计算机使用高速网络互联之后分别处理。这种仿真方法其速度提高非常显著，特别是在处理大量迭代计算的时候，不过不是一般的开发者所能具备的。

#### 性能测试和运行时间比较



### 测量性能
您可以使用 `tic`、`toc` 和 `sim` 命令将 Accelerator 或 Rapid Accelerator 模式的执行时间与 Normal 模式进行比较。
- 
打开您的模型。

- 
从 **Simulation** > **Mode** 菜单中选择 **Normal**。

- 
在命令提示符下使用 `tic`、`toc` 和 `sim` 命令，测量模型在 Normal 模式下进行仿真需要的时间：

tic,[t,x,y]=sim('myModel',10000);toc

将 `tic` 和 `toc` 一起使用可以记录和返回已用时间并显示如下消息：

Elapsed time is 17.789364 seconds.

- 
从 **Simulation** > **Mode** 菜单中选择 **Accelerator** 或 **Rapid Accelerator**，然后点击 **Run** 按钮为模型生成可执行文件。只要模型没有发生结构性改变，加速模式就会在后续的仿真中使用此可执行文件。[Code Regeneration in Accelerated Models](https://ww2.mathworks.cn/help/simulink/ug/code-regeneration-in-accelerated-models.html)中讨论了可能导致模型重新构建的情况。

- 
在命令提示符下重新运行编译的模型：

tic,[t,x,y]=sim('myModel',10000);toc

- 
显示的已用时间展示了加速模型的运行时间。例如：

Elapsed time is 12.419914 seconds.






两个已用时间的差（在此示例中为 5.369450 秒）显示了通过加速模型获得的速度提升。


资料来源：


[http://www.360doc.com/content/18/0503/15/55172235_750792155.shtml](http://www.360doc.com/content/18/0503/15/55172235_750792155.shtml)


[https://zhidao.baidu.com/question/811590541818007132.html](https://zhidao.baidu.com/question/811590541818007132.html)

[https://ww2.mathworks.cn/help/simulink/ug/comparing-performance.htm](https://ww2.mathworks.cn/help/simulink/ug/comparing-performance.html)](https://blog.csdn.net/zhanshen112/article/category/7192776)](https://blog.csdn.net/zhanshen112/article/category/7184373)](https://so.csdn.net/so/search/s.do?q=运行效率&t=blog)](https://so.csdn.net/so/search/s.do?q=simulink&t=blog)




