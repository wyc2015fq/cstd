# UML学习个人总结——UML基础知识 - 深之JohnChen的专栏 - CSDN博客

2009年08月16日 22:44:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：11520


**UML****基础知识**

**UML****简介**

在80年代末至90年代中，对面向对象分析与设计方法的研究发展到一个高潮。但是，诸多流派在思想和术语上有很多不同的提法，在术语、概念上的运用也各不相同，需要一种统一的符号来描述面向对象的分析和设计活动。UML应运而生。它不仅统一了Booch、Rumbaugh和Jacobson的表示方法，而且有进一步的发展，最终成为大众所共同接受的标准建模语言。统一建模语言（UML）是一个通用的可视化建模语言，用于对软件进行描述、可视化处理、构造和建立软件系统制品的文档。它记录了对必须构造的系统的决定和理解，可用于对系统的理解、设计、浏览、配置、维护和信息控制。UML适用于各种软件开发方法、软件生命周期的各个阶段、各种应用领域以及各种开发工具，UML 是一种总结了以往建模技术的经验并吸收当今优秀成果的标准建模方法。它融入了软件工程领域的新思想、新方法和新技术。不仅支持面向对象的分析与设计，还支持从需求分析开始的软件开发全过程。

**UML****模型、视图、图**

UML的概念和模型可以分成以下几个概念域：静态结构、动态行为、实现构造、模型组织、扩展机制

UML视图和图

|*主要的域*|*视图*|*图*|*主要概念*|
|----|----|----|----|
|静态结构|静态视图|类图|类、关联、泛化、依赖关系、实现、接口|
|用例视图|用例图|用例、参与者、关联、扩展、包括、用例泛化| |
|实现视图|构件图|构件、接口、依赖关系、实现| |
|部署视图|部署图|节点、构件、依赖关系、位置| |
|动态|状态视图|状态图|状态、事件、转换、动作、|
|行|活动视图|活动图|状态、活动、完成转换、分叉、结合|
|为|交互视图|顺序图|交互、对象、消息、激活|
|||协作图|协作、交互、协作角色、消息|
|模型管理|模型管理视图|类图|包、子系统、模型|
|扩展机制|所有|所有|约束、构造型、标记值|

**静态视图**

1、类元

类元是模型中的离散概念，拥有身份、状态、行为和关系。有几种类元包括类、接口和数据类型。其他几种类元是行为概念、环境事物、执行结构的具体化。这些类元中包括用例、参与者、构件、节点和子系统。图列出了几种类元和它们的功能。元模型术语类元中包括了所有这些概念。

|*类元*|*功能*|*表示法*|
|----|----|----|
|参与者|系统的外部用户|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/1.JPG)|
|类|类代表了被建模的应用领域中的离散概念。最重要的特性是多重性|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/2.JPG)|
|状态类|局限于某个给定状态的类|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/3.JPG)|
|类元角色|在合作中局限于某个使用的类元|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/4.JPG)|
|构件|系统的一个物理组成单元|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/5.JPG)|
||||
|接口|刻划行为特征的操作命名集.|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/6.JPG)|
|节点|计算资源|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/7.JPG)|
|信号|对象间的异步通信|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/8.JPG)|
|子系统|作为且有规范、实现和身份的单元的包|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/9.JPG)|
|用例|与外界代理交互中的实体行为说明|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/10633860602200045000.JPG)|

2、类元之间关系

类元之间的关系有关联、泛化、各种形式的依赖关系，包括实现关系和使用关系。

关联：对象通常要和其他对象发生关联，关联可以具有多层形式。多重性问题（一对一、一对多）。在UML中关联用一条直线来表示。

泛化：一个类继承了其他类的属性和操作。在UML中泛化用“从之类画一条带空心三角形箭头的连线指向父类”来表示。

依赖：一个类使用了另一个类。在UML中依赖用“从依赖类到被依赖的带箭头的虚线”表示。

聚集是关联的一种，聚集对象由部分对象组成。也就是整体与部分关联。在UML中用“整体和部分之间用带空心菱形箭头的连线连接”来表示。

组合是一种特殊的聚集，在一个组合对象中，部分对象只能作为组合对象的一部分与组合对象同时存在。在UML中用“整体和部分之间用带实心菱形箭头的连线连接”来表示。

实现：类和接口之间的关系被称为实现。在UML中实现关系用一个带空心三角形箭头加虚线来表示，箭头指向接口。

关系的种类
|关系|功能|表示法|
|----|----|----|
|关联|类实例之间连接的描述|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/11.JPG)|
|依赖|两个模型元素间的关系|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/12.JPG)|
|泛化|更概括的描述和更具体的种类间的关系，适用于继承|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/13.JPG)|
|实现|说明和实现间的关系|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/14.JPG)|
|聚集|聚集对象由部分对象组成。也就是整体与部分关联。|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/15.JPG)|
|组合|一种特殊的聚集.|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/16.JPG)|

图举例：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/guanlian.JPG)

关联

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/yilai.JPG)

依赖

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/xiandingyilai.JPG)

限定关联


![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/juji.JPG)

聚集和组成

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/fanhua.JPG)

泛化

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/shixian.JPG)

实现关系 

**用例视图**

当用例视图在外部用户前出现时，它捕获到系统、子系统或类的行为。它将系统功能划分成对参与者(即系统的理想用户)有用的需求。而交互功能部分被称作用例。用例使用系统与一个或多个参与者之间的一系列消息来描述系统中的交互作用。参与者可以是人,也可以是外部计算机系统和外部进程。

用例之间的关系：关联、扩展、泛化、包含。
|关系|功能|表示法|
|----|----|----|
|关联|参与者与其参与执行的用例之间的通信途径|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/11.JPG)|
|扩展|在基础用例上插入基础用例不能说明的扩展部分|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/17.JPG)|
|泛化|用例之间的一般和特殊关系，其中特殊用例继承了一般用例的特性并增加了新的特性|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/13.JPG)|
|包含|在基础用例上插入附加的行为，并且具有明确的描述|![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/18.JPG)|

图举例：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/19.JPG)

用例图

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/20.JPG)用例关系图

**交互视图**

交互视图描述了执行系统功能的各个角色之间相互传递消息的顺序关系。类元是对在系统内交互关系中起特定作用的一个对象的描述，这使它区别于同类的其他对象。交互视图显示了跨越多个对象的系统控制流程。交互视图可用两种图来表示：顺序图和协作图，它们各有不同的侧重点。协作图也展示对象之间的交互关系，强调交互的语境和参与交互的对象的整体组织。协作图按照空间组织布图，而顺序图按照时间顺序布图。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/21.JPG)

顺序图

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/22.JPG)

协作图

**状态视图**

状态视图是一个类对象所可能经历的所有历程的模型图。状态图由对象的各个状态和连接这些状态的转换组成。状态图是对单个对象的“放大”，它说明对象所经历的状态变化。强调单个对象内状态的变化。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/23.JPG)

状态图

**活动视图**

活动图是状态图的一个变体，用来描述执行算法的工作流程中涉及的活动。活动状态代表了一个活动：一个工作流步骤或一个操作的执行。活动图描述了一组顺序的或并发的活动。活动视图用活动图来体现。活动图很像流程图，它显示出工作步骤，判定点和分支。可用于表达一个对象的操作和一个业务过程。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/24.JPG)

活动图

**物理视图**

物理视图对应用自身的实现结构建模，例如系统的构件组织和建立在运行节点上的配置。这类视图提供了将系统中的类映射成物理构件和节点的机制。物理视图有两种：构件图和部署视图。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/25.JPG)

构件图

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/26.JPG)

部署图

**模型管理视图**

模型管理视图对模型自身组织建模。一系列由模型元素（如类、状态机和用例）构成的包组成了模型。一个包(package)可能包含其他的包，因此，整个模型实际上可看成一个根包，它间接包含了模型中的所有内容。包是操作模型内容、存取控制和配置控制的基本单元。每一个模型元素包含于包中或包含于其他模型元素中。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/27.JPG)

包

**扩展机制**

UML提供了几种扩展机制，允许建模者在不用改变基本建模语言的情况下做一些通用的扩展。这些扩展机制已经被设计好，以便于在不需理解全部语义的情况下就可以存储和使用。由于这个原因，扩展可以作为字符串存储和使用。对不支持扩展机制的工具来说，扩展只是一个字符串，它可以作为模型的一部分被导入、存储，还可以被传递到其他工具。我们期望后端工具设计成能够处理各种扩展，这些工具会为它们需要理解的扩展定义特定的语法和语义。扩展机制包括约束、标记值和构造型。

约束是用文字表达式表示的语义限制。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/28.JPG)

约束

标记值是一对字符串—一个标记字符串和一个值字符串—存储着有关元素的一些信息。标记值可以与任何独立元素相关，包括模型元素和表达元素。标记是建模者想要记录的一些特性的名字，而值是给定元素的特性的值。例如，标记可以是author，而值是对元素负责的人的名字，如Charles Babbage。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/29.JPG)

标记值

构造型是在一个已定义的模型元素的基础上构造的一种新的模型元素。构造型的信息内容和形式与已存在的基本模型元素相同，但是含义和使用不同。例如，商业建模领域的建模者希望将商业对象和商业过程作为特殊的建模元素区别开来，这些元素的使用在特定的开发过程中是不同的。它们可以被看作特殊的类—它们有属性和操作，但是在它们与其他元素的关系上和它们的使用上有特殊的约束。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/30.JPG)

构造型

**各种图汇总**

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090816/31.JPG)

