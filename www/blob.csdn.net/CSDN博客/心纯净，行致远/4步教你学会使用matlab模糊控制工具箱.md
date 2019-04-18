# 4步教你学会使用matlab模糊控制工具箱 - 心纯净，行致远 - CSDN博客





2018年08月16日 22:46:59[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：375








原帖地址：[http://blog.sina.com.cn/s/blog_408540af0100avnv.html](http://blog.sina.com.cn/s/blog_408540af0100avnv.html)



      Matlab模糊控制工具箱为模糊控制器的设计提供了一种非常便捷的途径，通过它我们不需要进行复杂的模糊化、模糊推理及反模糊化运算，只需要设定相应参数，就可以很快得到我们所需要的控制器，而且修改也非常方便。下面将根据模糊控制器设计步骤，一步步利用Matlab工具箱设计模糊控制器。

首先我们在Matlab的命令窗口（command window）中输入fuzzy，回车就会出来这样一个窗口。

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897818117w.jpg)

下面我们都是在这样一个窗口中进行模糊控制器的设计。

1．确定模糊控制器结构：即根据具体的系统确定输入、输出量。

这里我们可以选取标准的二维控制结构，即输入为误差e和误差变化ec，输出为控制量u。注意这里的变量还都是精确量。相应的模糊量为E，EC和U，我们可以选择增加输入(Add Variable)来实现双入单出控制结构。

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897822VoI2.jpg)

2．输入输出变量的模糊化：即把输入输出的精确量转化为对应语言变量的模糊集合。

首先我们要确定描述输入输出变量语言值的模糊子集，如{NB，NM，NS，ZO，PS，PM，PB}，并设置输入输出变量的论域，例如我们可以设置误差E（此时为模糊量）、误差变化EC、控制量U的论域均为{-3，-2，-1，0，1，2，3}；然后我们为模糊语言变量选取相应的隶属度函数。

在模糊控制工具箱中，我们在Member Function Edit中即可完成这些步骤。首先我们打开Member Function Edit窗口.

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897826RvkG.jpg)

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897832n03S.jpg)

然后分别对输入输出变量定义论域范围，添加隶属函数，以E为例，设置论域范围为[-3 3]，添加隶属函数的个数为7.

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897838JJ05.jpg)

然后根据设计要求分别对这些隶属函数进行修改，包括对应的语言变量，隶属函数类型。

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897843lPBL.jpg)

3．模糊推理决策算法设计：即根据模糊控制规则进行模糊推理，并决策出模糊输出量。

首先要确定模糊规则，即专家经验。对于我们这个二维控制结构以及相应的输入模糊集，我们可以制定49条模糊控制规则（一般来说，这些规则都是现成的，很多教科书上都有），如图。

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897850yylj.jpg)

制定完之后，会形成一个模糊控制规则矩阵，然后根据模糊输入量按照相应的模糊推理算法完成计算，并决策出模糊输出量。

4．对输出模糊量的解模糊：模糊控制器的输出量是一个模糊集合，通过反模糊化方法判决出一个确切的精确量，凡模糊化方法很多，我们这里选取重心法。

![4步教你学会使用matlab模糊控制工具箱](http://hi.csdn.net/attachment/201104/27/3687160_1303897851jymo.jpg)

5．然后Export to disk，即可得到一个.fis文件，这就是你所设计的模糊控制器。

 >>[http://blog.sina.com.cn/](http://blog.sina.com.cn/xianfa110)[xianfa110](http://blog.sina.com.cn/xianfa110)



======================================================================================

MATLAB功能强大，很多同学用其做控制算法的仿真 

但是其中的fuzzy logic controller刚开始用常常容易出错 

常见报错： 

1、MinMax blocks do not accept 'boolean' signals. The input signal(s) of block 'mkkzq1/Subsystem/Fuzzy Logic Controller with Ruleviewer/Fuzzy Logic Controller/FIS Wizard/Defuzzification1/Max (COA)' must be one of the MATLAB 'uint8', 'uint16', 'uint32', 'int8', 'int16', 'int32', 'single', or 'double' data types

2、An error occurred while propagating data type 'boolean' from block 'mkkzq1/Subsystem/Fuzzy Logic Controller with Ruleviewer/Fuzzy Logic Controller/FIS Wizard/Defuzzification1/Zero Strength (COA)', output port 1。 



以下是解决方案： 



1、fis编辑器（MATLAB的command window中键入fuzzy打开）中，建立好规则后，file-export-to workspace 

2、对于7.0版本，simulink模型窗口菜单中，Simulation-Configuration Parameters-Implement logic signals as boolean data前面默认的勾选去掉 

3.而对于6.5及以前版本，则是在SIMULATION -SIMULATION PARAMETERS-ADVANCED中将boolean logic signals选为off，强烈推荐用这个





用FIS（Mamdani）设计完模糊控制器之后，连接到控制系统中进行仿真，每次总出现这两个错误：

1)Block error Max(coa): MinMax blocks do not accept'boolean'signals.

2)block error Zero strength(coa):An error occurred while propagating data type'boolean'from block'....',outport 1

解决方案：

在simulink parameter中，找到advance选项卡，把第一二个选项设为off。

====================================================================================

matlab/simulink模糊控制器应用实例

前面我们已经利用模糊控制工具箱设计好了一个模糊控制器（假定我们存为fuzzy1.fis），下面我们检验一下，看看我们的控制器到底怎么样。以一个简单的电机控制为例，我们在Simulink中建立了它的模糊控制系统如下：

![](http://hi.csdn.net/attachment/201104/27/3687160_1303899610f6fl.jpg)



在用这个控制器之前，需要用readfis指令将fuzzy1.fis加载到matlab的工作空间，比如我们用这样的指令：myFLC=readfis（‘fuzzy1.fis’）；就创建了一个叫myFLC的结构体到工作空间，并在fuzzy logic controller中参数设为：myFLC。

可以看到，在模糊控制器的输入和输出均有一个比例系数，我们叫它量化因子，它反映的是模糊论域范围与实际范围之间的比例关系，例如，模糊控制器输入输出的论域范围均为[-3，3]，而实际误差的范围是[-10，10]，误差变化率范围是[-100，100]，控制量的范围是[-24，24]，那么我们就可以算出量化因子分别为0.3，0.03，8。量化因子的选取对于模糊控制器的控制效果有很大的影响，因此要根据实际情况认真选取哦。

好，现在我们可以设定仿真步长，比如定步长的10ms，就可以运行了。

运行后，产生这样一个错误：

MinMax blocks do not accept 'boolean' signals. The input signal(s) of block 'test_fuzzy/Fuzzy Logic Controller/FIS Wizard/Defuzzification1/Max (COA)' must be one of the MATLAB 'uint8', 'uint16', 'uint32', 'int8', 'int16', 'int32', 'single', or 'double' data types

我想很多朋友做模糊控制的时候都会遇到这个情况。没关系，这里提供两个解决办法：

1．直接在Defuzzification1这个模块中的那个比较环节后加入数据类型转换模块，将boolean转化为double型，或者双击那个比较模块，选中show additional parameters，将输出数据类型改为specify via dialog，然后选uint(8)即可；但是在仿真之后，又会发现很多地方都存在这个问题，因此你不得不一个一个去修改，如果你不怕累的话。

2．第二个方法是最简单的，直接在simulation parameters->advanced将boolean logic signals选为off，强烈推荐你用这个。

好了，这些都解决了，我们就可以仿真了，例如给个方波信号，可以得到仿真曲线如下

![](http://hi.csdn.net/attachment/201104/27/3687160_1303899611lxlZ.jpg)



