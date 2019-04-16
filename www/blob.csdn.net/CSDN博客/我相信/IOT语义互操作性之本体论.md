# IOT语义互操作性之本体论 - 我相信...... - CSDN博客





2018年02月21日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：874
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










这个系列文章描述了一个单一的语义数据模型来支持物联网和建筑、企业和消费者的数据转换。 这种模型必须简单可扩展, 以便能够在各行业领域之间实现插件化和互操作性。 对于一个目前从事智能硬件的老码农，觉得这些文字具有积极的参考意义。在这第二部分, 尝试确定上层本体在解决元数据挑战中的作用, 以及如何能够提高可伸缩性。
`“There are two words for everything.” – E.V. Lucas`
## 什么是本体论？

本体论作为科学的一部分, 有很多面孔。 起初, 本体论是关于"存在"哲学的一部分, 也是描述世界的物体、现象和规律的通用知识体系。

近年来, 本体论的发展已经从人工智能领域转向语义网。 网络上的本体论, 包括从对一般网页内容(如 schema.org)进行分类, 到销售产品及其功能(如亚马逊网站)。

作为语义互操作性的促进者, 本体提供了与特定领域(例如医疗保健)元数据标识符相关的概念，提供了一个标准化的分类。 本体虽然包含了分类学和词库的特性, 但在术语和属性之间使用严格的语义关系, 目标是以机器可读形式来表示知识(图15)。 [7]

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPUiahsictBd1iaSSKT1nbYVB9vD0Ficzzichb2liahGaZJ4tqncS3nMDGQ0KA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

[图15 | 语义层次]

用于开发本体的方法对于促进可伸缩性至关重要, 必须考虑所有相关的应用程序。 本系列文章考虑的应用包括五个相互关联行业的商业和物联网使用案例——房屋、能源、零售、医疗和运输及物流。

虽然句法语言(例如 OWL、 RDF 和 RDFS)可以用来构建本体论, 而本节将关注与任何特定建模语言无关的方法论。

## 一致性的受控词表

受控词表是经过精心挑选的词汇和短语(即术语)的集合, 这些词汇和短语在不同的语境中都被赋予了明确的含义。 一个词汇表可以用来保持本体发展的一致性, 它定义了术语背后的上下文关系。

登记中心控制的词汇表中的所有术语都有一个毫不含糊的、非冗余的定义。 如果多个术语可以用来表示同一事物, 则应将其中一个术语定义为受控词表中的首选术语, 其他术语被列为同义词或别名(见图16和[IOT语义交互性之词汇定义](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658972622&idx=1&sn=bcc140cb8c8d9a1b454b008d637f5ecc&chksm=80d32ee8b7a4a7fef9053fdace92bf5a3528bbafa56363d54626c902d065c29326611c21e233&scene=21#wechat_redirect))。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP6Huo0Hr7XLLtQy5NewJyXgbuaOQYedwIgKZDlhOfeWib7ICkCu0vANw/640?wx_fmt=jpeg)

[图16 | 別名和翻译的受管制术语]

受控词表应该为全球应用提供各国家语言支持。 代表知识领域内术语的标准词汇可以从各种组织(例如 lov.okfn.org)免费获得。

## 对每件事物都有一个对象类

本体可以通过一组类来提供域概念的标准化分类。 每个类(概念)都可以表示可以唯一标识的类似事物或对象的类别。 类被定义为反映其对象(实例)所独有的属性、限制和关系。 类可以表示物理对象, 例如传感器和人员以及信息对象, 例如商业交易[ ISO 11179]。 一个本体, 连同一组个别的类实例, 构成一个知识库。 [8]

### 一个类的层次结构

像分类法一样, 本体可以在分层结构中定义其类, 这种结构可以根据需要进行深度定义(图17)。 类(如传感器或执行器)可以是另一类(设备)的子类(类型)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP7S6gY8WW7FBchm4P6VyGC1RMJwexicX9yQpMqu8c2GF2DYDwOtAL3nA/640?wx_fmt=jpeg)

[图17 | 本体的层次结构]

所有子类都继承类的属性。 例如, 如果 Power 状态是设备类的一个属性, 那么所有传感器和执行器对象都会有这个属性。

属性在适用于其所有对象(包括子类)的最普通类附加一个属性。 由于所有类都是对象的类型, 所以类层次结构有一个根类, 即对象, 它包含所有对象继承的标识符(一个 O-DEF 分类属性)(见图19)。

虽然这种方法与面向对象程序设计相似, 但它代表了从编程中抽象出来的元数据。 代表本体的元数据可以在一个存储库(ISO 11179)中从任何编程环境中抽象出来。

## 跨行业领域互动的最高层

顶级对象类(例如 ODEF 核心索引)可以促进不同领域(如建筑物、零售、医疗保健)的数据交换和互操作性, 因为它们确保以统一和语义兼容的方式使用基本术语。

本系列文章中确定的各组织机构语义数据模型包括了支持其目标行业和使用情况的最高级别类别(图18)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPv1tpwvIibHiaxArECT7EbZFsw99rfPlpUqYhiaSTp9cOupyt1uHG9PWjw/640?wx_fmt=jpeg)

[图18 | 各联盟的顶级对象类]

虽然术语可能不同, 但各联盟集团有许多基本概念(类)。 这些概念的"混合"可以形成一个能够支持行业的特定用例和跨行业互操作性的上层本体(图19)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPAf67Ja4Pec9icKzO0M97BQm2zjLzvxs5kaz99mZU77iaUic2YJlfibuRrA/640?wx_fmt=jpeg)

[图19 | 混合、跨行业顶级等级]

根对象类的名称和描述属性可以描述这些顶级类, 并包含在术语表中:
- 
Information Model 资讯模型

- 
Asset 资产

- 
Product 产品

- 
Location 位置

- 
Party 派对

- 
Transaction 交易

- 
System 系统

- 
Process 过程

- 
Rule 规则

- 
Event 活动

- 
Relationship 关系


虽然个人和组织被一些财团模型(O-DEF, schema.org)认为是顶级的类, 但它们都是商业模式中一个Party概念的子类(GS1 EDI, ARTS ODM)。 缔约方类包括人员和组织共有的属性, 并允许一个类与业务事务和其他关系关联。 [9]缔约方能够拥有法律所有权, 并且可以与根对象类的所有人属性有关。 缔约方实例可以拥有有形(车辆)和无形(销售订单)对象。

尽管在这些联合体中没有明确定义, 但在这种混合方法中包含了一个顶级关系类, 可以从定义多对多关系的任何特定本体语言中抽象本体。

### 信息模型的类

信息模型作为一个知识领域, 可以有自己的本体, 可以建立一个多层次的本体论。 信息模型顶级对象类(ODEF Information-set)可用于包含定义信息模型的子类(图20)。 其中包括:
- 
Data Type 数据类型

- 
Measurement Unit 测量组

- 
Attribute 属性

- 
Vocabulary Term 词汇词汇

- 
Role 角色





![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPhib2UJtb1smvgdROk5wzM71bLiao35ibDTGJ3JBkYnOfcJC5dxqhUicOcw/640?wx_fmt=jpeg)

[图20 | 一个信息模型类层次结构]

### 数据类型的本体

数据类型和测量单位(如 QUDT.org)的本体论可以为商业和技术提供基础语义互操作性。

数据类型类可以作为信息模型的子类来建模。 基于数字电子学的所有数据都表示为最低级别的比特(0和1) , 数据类型类的比特属性可以由所有子类继承。 数字和字符串是原子数据类型(数据类型类的直接子类) , 因为它们的值不能用较小的部分来描述。 Integer 和 Float原语可以定义为数类的子类(如 schema.org)。 标准组织已经定义了原子和原始数据类型(例如, ISO.org 11404, W3.org XML Schema) , 但是它们之间的不一致是难以管理的。

具有唯一属性的附加数据类型(如数量)可以从原始数据类型中派生出来, 并定义为它们的子类。 然而, 在编程语言和组织数据服务中, 具体的原始数据类型和派生数据类型的使用情况各不相同, 限制了语义互操作性(图21)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPUKbX0TKx9EM5ljSYibfVia8SicYQdHaoKQBJPnJXMFgpfOH7aO3nSXmOw/640?wx_fmt=jpeg)

[图21 | 联盟组织的数据类型]

### 一个术语的数据类型

一个术语数据类型(类似于 Haystack 的"marker")可以被属性(类似于 Haystack 的“tag”)来分别与对象类层次结合使用。

与受控词表一起使用时, 术语属性的值可以表示一个 Term 对象。 例如, 在图19中, 根对象类的 Name 属性分配给术语数据类型。根对象类Name 属性的值与受控词表中的"对象"项有关(图16)。

术语的概念也可以作为信息模型的一个子类(图20)。

### 关系的数据类型

关系数据类型(类似于 Haystack 的"Ref")可以分配给一个属性, 以表示与同一个或不同类对象的关系。 例如, 根对象类的类属性分配给关系数据类型(图19)。 属性类的"内部类"属性也分配给关系数据类型(图20)。 在这种情况下, 关系表示在类对象中对属性对象的限制。

分配给关系数据类型的属性应该仅限于单个类中的对象, 这应该是适当反映关系的最严格子类。

### 用于测量的数量数据类型

商业和技术依赖于经过测量的数字, 其中大多数都有单位。 数据类型本体可以定义一个测量的数据类型(scheman.org中的数值)来作为数据类型的子类。 数据类型也可以定义为量数据类型的每个类型或"维度"的测量。 例如, 图21中的一个温度数据类型(UN/CEFACT Temp-Measure类型)可以定义为数量子类的实例。

通过将货币量建模是另一种测量类型, 包括值转换在内的过程可以在所有测量类型中归一化。 一种机制(类似于 xe.com)可以用来检索与货币单位相关的转换因素(货币兑换率)的动态价值变化。

### 一个测量单位的类

最广泛使用的单位系统是国际单位制, 即 SI。 ISO 80000-1进一步定义了 SI 和国际数量体系(ISQ)的数量和单位。

单元类可以作为信息模型的子类来建模。 图22显示了数据集 中每个对象继承的对象类的属性(标识符、名称、类)。 该图还包括由对象的类属性标识的类(单元)属性。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPqwribJVnib65StwJ4lPX0ibdXOcww46KQ5gRXjdh5FMRa4e8uXeVI7ooA/640?wx_fmt=jpeg)

[图22 具有对象和单位属性的单元类示例实例]

一个单位标识符(如 华氏度)与数据集中的数量值(例如 Haystack 标记数据)配对, 可以在所确定的单位对象内解析一个数量数据类型(如温度)。单元对象的属性也可以支持单元转换过程(图23)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP0QQVRKlsROCbCU2dbuS2oHHySEYb34nphvRryuu8ybfSMDXMjxnuIw/640?wx_fmt=jpeg)

[图23 | 使用带转换属性的单元实例进行温度值转换]

### 对象的角色

角色的概念(如 O-DEF 中的概念)描述了一个可以由特定上下文中对象执行的函数。 角色类可以作为信息模型的子类来建模, 并且可以包含适用于不同对象类的实例(图24)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPW3MMJibjcBsbicYCzRmxCicqN4IszoqJT5Syee8DN4suZt9kIpmG2eDwA/640?wx_fmt=jpeg)

[图24 | 具有对象和角色属性的角色类示例实例]

关系类中的一个实例可以将角色的实例分配给对象。 一个对象可以有多个角色。 例如, 一个人的实例可以有员工、父母和乘客的角色。 一种设备的实例可以是传感器和通信器。 许多设备的目的是承担与人相同的角色。 因此, 角色可以分配给对象类。

一些角色(客户)有一个相应的反向角色(供应商)。 当一个客户角色被分配给一个缔约方(以 ARTS ODM 为模型)时, 将相应的供应商角色分配给另一缔约方以形成贸易关系。

## References

7 Harpring, Patricia, “Introduction to Controlled Vocabularies”, 2010 J. Paul Getty Trust.

8 Noy, Natalya F. and McGuinness, Deborah L. “Ontology Development 101: A Guide to Creating Your First Ontology”, protégé.Stanford.edu, 2001.

9 Hay, David C., Data Model Patterns: Conventions of Thought, Dorset House Publishers, Inc. (New York: 1996)







