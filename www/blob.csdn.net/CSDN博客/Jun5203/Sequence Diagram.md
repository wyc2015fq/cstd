# Sequence Diagram - Jun5203 - CSDN博客
2018年10月05日 21:05:20[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：154
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 前言
动态图是进行分析和设计系统的结构行为图，动态模型描述了系统随时间变化的行为，这些行为是从静态视图中抽取的系统的瞬间值的变化来描述的。在UML的表现上，动态模型主要是建立系统的交互图以及活动图和状态图。
### 芝士
- 什么是时序图？
时序图描述了对象之阿金传递消息的时间顺序，它用来表示用力中的行为顺序，是强调消息时间顺序的交互图。
- 时序图组成元素
- 对象（Object）
- 生命线（Lifeline）
- 激活（Activation）
- 消息（Message）
![在这里插入图片描述](https://img-blog.csdn.net/20181005205743169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 举例——汽车租赁系统中客户取车
![在这里插入图片描述](https://img-blog.csdn.net/20181005213458841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
时序图涉及了5个对象：客户、预定请求、工作人员、工作记录和汽车。取车的动作从客户向工作人员提出取车要求并出示清单开始，工作人员检查客户的预定审定，确认后客户可以付款。工作人员填写工作记录，同时登记汽车的状态，最后客户取车。
- 时序图建模步骤
- 设置交互的语境，这些语境可以是系统、子系统、操作、类、用例和协作的一个脚本。
- 通过对象在交互中扮演的角色，根据其出场顺序，将其按从左向右的方向放在时序图中。
- 设置每个对象的生命线。一般情况下，对象存在于交互的整个过程，但它也可以在交互过程中被创建和撤销。
- 根据时间先后，在生命线之间按从上向下的顺序画出随后的消息。
。。。。。。
### 附加——机房收费系统
- 一般用户——上机
![在这里插入图片描述](https://img-blog.csdn.net/20181005213642541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 操作员——添加用户
![在这里插入图片描述](https://img-blog.csdn.net/20181005213648420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结合机房收费系统来看，我认为时序图就是对先做什么后做什么做一个整体的规划，单个功能体现不太出它的功能，但旨在把所有事件的顺序给理清的话，那肯定是非时序图莫属`
### 小结
时序图强调事件的顺序，简单粗暴，理解的很到位~
