# 028_AUTOSAR RTE学习笔记-1 - 小灰笔记 - CSDN博客





2018年12月09日 22:59:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：590








         对AUTOSAR的了解确实不多，一直觉得AUTOSAR不应该是买买买，更不该是各种工具来一统天下。我觉得任何软件架构都应该升华为一种哲学，但是在掌握的过程中我们可能得从支离破碎的零星判断中逐渐还原哲学的内核本质。

         今天尝试理解一份我自己从网上找的几页PPT资料，整理出这份学习笔记。

第1点，

![](https://img-blog.csdnimg.cn/20181209225905277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

截图中已经做了很多解释，不过这种解释我一般只会保留性的吸收。我自己再整理一遍：

RET的主要功能分为如下4点：
- VFB的接口实现；
- 一共基础设施服务，支持如下功能：
	- AUTOSAR模块（我理解应该是应用层模块）之间的通信；
- AUTOSAR模块（依然是ASW）访问基础软件，比如OS以及各种服务；

- 实现软件模块（应该是ASW）与ECU的映射以让RTE服务标准化；
- 针对专门的ECU生成而不是通用的，从而保证最优。

**个人思考：**
- **从上面的描述看，直接的变量赋值或者传递的方式肯定是不行的；**
- **2.a****中可以看得出，ASW****不同模块之间的通信应该是通过RTE****，这样ASW****模块之间似乎模块不该用全局变量合理一点；**
- **变量与ECU****的映射以及标准化，需要再进一步了解；**
- **一个疑问：这个东西是否会导致软件执行效率的降低？**

![](https://img-blog.csdnimg.cn/20181209225905683.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

**关于这个图，先提出自己的几个疑问点：**
- **这是整个RTE****的描述还是RTE****可以拆分为若干个这样的小单元？**
- **端口是怎样一个概念？如何使用？是哪一个英文描述的翻译？**
- **ECU****中的并发是一个什么样的概念？**
- **RTE****的事件是一个什么概念？**
- **通信收发中，明确与不明确又是一个什么概念？**
- **运行提应该是ASW****的软件逻辑？**

![](https://img-blog.csdnimg.cn/20181209225905729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

**这个图应该可以理解清楚，其实比较好的方式就是看箭头的走向。可能呢分为几种路线，如果把两个构件（ASW****模块）左边叫做A****，右边叫做B****。路线如下：**

**路线1****： A****经过端口到通信的收发器，再从收发器出去到B****的端口，传递给B****。可以理解为A****的修改通知给B****，或者B****有时候需要查询A****的信息。这个路线比较简单，因为是单向的。**

**路线2****：A****经过端口到客户端，客户端再到BSW****端口，之后访问底层。这种可以理解为底层的输出驱动类操作。**

**路线3,****：其实主要是路线2****的双向箭头反过来，BSW****的信息通过端口到客户端，之后再传递到A****的端口，之后被A****读取。这样的场景可以理解为对底层状态的读取，诸如AD****采集。**

![](https://img-blog.csdnimg.cn/20181209225905581.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         我个人理解上，上面的图应该是错误的或者是个例，不然的话构件Call右边的箭头应该还是一个双向的箭头，不然没法解释通信与BSW之间的Call的箭头的双向。

         先按照我自己的理解方式顺着箭头理顺一遍：

         路线1：这一个理解方式不一定对，就是ASW直接到通信，之后访问BSW。或许，上面图中没有这个意思。

         路线2：ASW调用端口通信的时候需要等待RTE事件的发生。或者描述为：RTE事件的发生会出发ASW调用端口产生通信，从而访问BSW。或许，这是它想表达的意思。

![](https://img-blog.csdnimg.cn/20181209225905723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)
- **ASW****不能够直接访问OS****，所以没有task****的概念与ASW****关联到一块。这样，前后台程序模式下，我会考虑，没有task****，ASW****会如何运行？**
- **针对我在1****中的问题，AUTOSAR****的回答或许是，通过Runnable****来执行。可是，Runable****又是通过什么来激活？**
- **AUTOSAR****或许回答我2****中问题的方式：Runable****由RTE****统一管理，通过RTE****的事件来触发。RTE****可以被OS****调度，这样的话要满足不同的调度顺序，RTE****应该执行的十分高频？RTE****的事件又是一个什么概念呢？这个资料中看不到问题的答案。**
- **另外，我自己疑问之外的基础性知识有一部分。**- **Runable****有几种，以满足不同的设备。**
- **RTE****的事件可能会尝试把所有的触发任务的事件都包含进来，这样理解的话，应该是各种执行条件的判断？**
- **Runable****是可以由RTE****调用管理的一段指令序列。**


文档剩下的部分是一个卖工具的介绍，暂且不去理解了。



