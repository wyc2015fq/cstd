# 使用Pspice进行电路仿真 - xiahouzuoxin - CSDN博客





2014年08月13日 22:07:37[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：32225








Pspice现在是集成到Cadence中的一个电路仿真工具，能通过Pspice模型的原理图仿真电路的输出结果。其大致步骤是：

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#1-%E5%88%9B%E5%BB%BA%E4%BB%BF%E7%9C%9F%E5%B7%A5%E7%A8%8B)1. 创建仿真工程

执行菜单命令`File->New->Project`，选择`Analog or Mixed A/D`，然后选择一个模板（这个看自己需要，也可以是空工程）

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#2-%E6%B7%BB%E5%8A%A0%E4%BB%BF%E7%9C%9F%E5%85%83%E4%BB%B6%E5%BA%93)2. 添加仿真元件库

仿真所用的元件必须要有Pspice模型，Cadence安装目录下\tools\capture\library\pspice中所有的元件库都含有Pspice仿真模型。

\tools\capture\library\pspice下的库很多，比如：
- Analog：包含无源元件（R、L、C），互感器，传输线，以及电压和电流非独立的源（电压控制的调用源E、电流控制的电流源F、电压控制的电流源G 和电流控制的电压源H）
- Source：给出不同类型的独立电压和电流源，例如：Vdc（直流电压），Idc（直流电流），Vac（交流电压），Iac（交流电流），Vsin（正弦电压），Vexp（指数电压），脉冲，分段线性，等
- Eval：提供二极管（D…），双极型晶体管（Q…），MOS 晶体管，结型场效应晶体管（J…），真实运算放大器；如u741，开关（SW_tClose, SW_tOpen），各种数字门和元件
- Abm：包含一个可以应用于信号的数学运算符选择，例如：乘法（MULT），求和（SUM），平方根（SWRT），拉普拉斯（LAPLACE），反正切（ARCTAN），等
- Special：包含多种其他元件，像参数、节点组，等

若不知道使用的元件，可以在Place part框中搜索，如下图：

![search](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F/search.png)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#3-%E7%BB%98%E5%88%B6%E5%8E%9F%E7%90%86%E5%9B%BE)3. 绘制原理图

就以上面找到的LM393电压比较器为例：

![sch](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F/sch.png)

其中的元件来自不同的Pspice库（\tools\capture\library\pspice目录下）：
- 电阻：Analog
- 电压比较器LM393：opamp
- 直流电源V1（Vdc）：Source
- 正弦电源V2（VSIN)：Source

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#4-%E8%AE%BE%E7%BD%AE%E4%BB%BF%E7%9C%9F%E5%8F%82%E6%95%B0%E5%B9%B6%E6%89%A7%E8%A1%8C%E4%BB%BF%E7%9C%9F)4. 设置仿真参数并执行仿真

这步是最重要的，也是做仿真分析的目的——你要仿真什么？

通过菜单命令`PSpice->New Simulation Profile`设置仿真参数，如下图，可进行共4种类型的仿真：

![Profile](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F/Profile.png)
- 时域仿真：
- 直流扫描：
- 交流扫描：一般用于频域及相位分析
- 偏置点分析：

这里就以一个上面电路的时域仿真为例，如上图，设置仿真时间10ms，仿真最大步长0.1ms。

设置好了仿真类型参数，接着添加探针，观测我们感兴趣的值（电压、电流），在工具栏中点击RUN开始仿真，

![run](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F/run.png)

仿真结果默认会在电路网络上显示偏置电压值（可通过工具栏按钮选择显示与不显示），同时会弹出上面设置的时域仿真分析结果波形：

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F/result.png)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#5-%E5%88%86%E6%9E%90%E4%BB%BF%E7%9C%9F%E7%BB%93%E6%9E%9C)5. 分析仿真结果

还是那句话，仿真是为了帮助分析，因此做仿真后的分析是必不可少，相对于前面的软件傻瓜操作，分析更为重要。

我们来分析仿真与电路预期的效果是否一致？

原电路时一个LM393的电压比较电路，反相输入端电压通过等值的1K电阻分压（运放输入阻抗近似无穷大），因此理论上为2.5V。所以当正相输入端电压< 2.5V时，输出为低电平（0V），否则为高电平（LM393的输出电平电压通过上拉电阻的电位确定，这里使用10K电阻上拉到5V）。

我们来看看仿真结果：输入为正弦波（峰峰值5V，直流分量2.5V），当正弦波（绿线）>2.5V时，输出（红线）为高电平；当正弦波（绿线）< 2.5V时，输出（红线）为50mV->0V为低电平，这50mV是由于运放本身造成的，对输出电平判别而言值可以忽略。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E4%BD%BF%E7%94%A8Pspice%E8%BF%9B%E8%A1%8C%E7%94%B5%E8%B7%AF%E4%BB%BF%E7%9C%9F.md#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)参考资料
- 
PSPICE简明教程. 宾西法尼亚大学电气与系统工程系. 编译：陈拓, 2009年8月4日.

这是一份学习操作Pspice分析的很好的资料，文档逻辑清晰，语言简明，非常适合初学者。里面有不少的例子关于4种仿真类型的具体例子，可谓比较全面。

- 
Pspice教程(基础篇+进阶篇). 上海银利电子有限公司（下载的文档上是这样标的，如有侵权请联系）.

大致浏览了一下，和上面的参考资料[1]差不多，参数分析部分更为详细（包括蒙特卡洛分析、傅里叶分析等），但关于入门的介绍感觉没[1]那么直观，作为进阶使用PSpice参考还是不错的。

- 
在PSPICE 中使用德州仪器 (TI) SPICE 模型. [http://www.ti.com](http://www.ti.com/)

文档的TI网站链接是[http://www.ti.com.cn/cn/lit/an/zhca088/zhca088.pdf](http://www.ti.com.cn/cn/lit/an/zhca088/zhca088.pdf)，从题目就可以看到，不用说了。有些PSpice模型在Cadence默认的PSpice目录中是没有的，但器件方可能会提供该模型，TI官网上就可以下载许多PSpice元器件模型，这些模型可用于电路设计前的仿真，提高电路设计的正确率。

- 
LMV791: Application Note 1516 Pspice Universal Test Circuits. [http://www.ti.com](http://www.ti.com/)

好吧，也是TI的文档，是TI的关于运放的PSpice测试电路设计，当然比较偏应用了。




