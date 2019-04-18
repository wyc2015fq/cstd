# （Software Engineering Overview）軟件工程基礎 - 無名黑洞 - CSDN博客
2012年03月24日 00:22:17[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1381标签：[prototype																[cmm](https://so.csdn.net/so/search/s.do?q=cmm&t=blog)](https://so.csdn.net/so/search/s.do?q=prototype&t=blog)
个人分类：[Engineer](https://blog.csdn.net/wangxiaojun911/article/category/5904817)
SE的目標是發展和保持高水平的軟件，使軟件系統具有可管理性，可重用性，魯棒性以及更加容易理解。
Prototype Process模型
【需求——快速設計——原型設計——評測】——最終設計
Spiral Process Model
分為RiskAnalysis、Engineering、Customer Evaluation、Planning四個象限。
CMM = Capability Maturity Model
1、 initial
2、 Repeatable
3、 Defined
4、 Managed
5、 Optimizing
一個軟件工程選用這些模型是要化簡難度，軟工的難度主要體現在溝通、工程本身的複雜性上、個人能力的局限性、資源、管理等方面。終極目標在提高軟件質量和降低成本/風險上。
總而言之，軟件的Lifecycle由如下部分組成：
Definition (What)
Development (How)
Maintenance (Change)
在不同的時期也可以選用不同的模型，比如初期使用原型設計，風險評估使用螺旋模型，維護時用瀑布模型。
**System Design（系统设计）**
系统设计的目标：高层次战略，子系统划分，并行任务等。
Design Patterns (设计模式)
软件工程的设计是消耗巨大的工作，当计算机硬件的成本不断降低时，软件的设计和实现成本并没有降低。人们希望找到一个可重用的软件设计模式。它应该是灵活的、模块化的和抽象化的。也就是说，会反复出现的问题才有设计模式的意义（Recur属性）。
注意“设计”的含义，它不是“构架”，也不是“计算”（相对于算法）。
Pattern：是“问题”和“解决方案”的集合。分为四个主要部分：Pattern name，Problem，Solution和Consequences（Alternative, 花销和Benefits）。具体描述(Descriptions)如下：
1、 名字和分类；
2、 目的（问题描述
3、 别名；
4、 动机（适用此模式的Scenarios）；
5、 应用；
6、 结构（Class and interaction diagram）；
7、 参与者（Class/Object）；
8、 Collaborations(How participantscollaborate)；
9、 结果Consequence；
10、             实现（程序语言等问题）；
11、             Sample Code；
12、             已知的实际系统；
13、             相关的其它模式。
设计模式空间：
Creational Patterns： Factory Method, Abstract Factory,Builder, Prototype, Singleton…进一步可分为 Class Creational和Object Creational两种方式。
Structural Patterns: Adapter, Bridge,Composite, Decorator, Façade, Flyweight, Proxy…
Behavioral Patterns: Chain ofResponsibility, Command, Iterator, Interpreter, Strategy…
Factory Method：工厂方法模式 是一种面向对象的设计模式。通过调用不同的方法返回需要的类，而不是去实例化具体的类。对实例创建进行了包装。 工厂方法是一组方法， 他们针对不同条件返回不同的类实例，这些类一般有共同的父类。（Wiki原文）
Adapter: 在[设计模式 (计算机)](http://zh.wikipedia.org/wiki/%E8%A8%AD%E8%A8%88%E6%A8%A1%E5%BC%8F)中，**适配器模式**（英语：adapter
 pattern）有时候也称包装样式或者包装。将一个[类](http://zh.wikipedia.org/wiki/%E7%B1%BB_(%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6))的接口转接成用户所期待的。一个适配使得因接口不兼容而不能在一起工作的类工作在一起，做法是将类别自己的接口包裹在一个已存在的类中。（Wiki原文）
Proxy: 相当于其它对象的代用品（surrogate）。
Strategy: 策略模式作为一种软件设计模式，指对象有某个行为，但是在不同的场景中，该行为有不同的实现算法。比如每个人都要“交个人所得税”，但是“在美国交个人所得税”和“在中国交个人所得税”就有不同的算税方法。(Wiki原文）
相关概念：
Signature (of an operation)：一个Operation的函数。
Interface（of an object）
再介绍个具体模式：
Observer Pattern: 观察家模式是基于某个特定的观察对象的模式，并在对象改变时主动发出通知。是一对多的Dependency。
总而言之，设计模式的好处是：提高了可重用性，提高了专业化，帮助开发人员交流，促进了面向对象的开发模式。不足之处有：不能直接生成代码；表面过于简单（实际很复杂）；加重开发人员负担；在人员交流中有用，但无法自动处理等。
**Requirement工程需求**
怎么根据需求确立模型？（Howto wrap up?）首先要建立ClassDiagram和对应的Operation、Invariants等。
Waterfall模型
(收集需求——撰写需求文档)——(建立设计——撰写设计文档)——（写代码——实现——测试）——维护
**需求分析**
包括功能，数据interface，API，User interface，硬件Interface，error handling和潜在修改。
一些原则包括优先级，忽略细节，准备好随时变更需求。
一些需求分析产品：
文档
Prototype
Preliminary用户说明书
用户和开发人员评测
需求分析的步骤：
1、 取得问题说明
2、 写Use Case
3、 建立Object model 和 data dictionary
4、 建立动态模型
5、 Refine模型
6、 撰写分析文档
还可以撰写IssueList。 Meeting。或者Interview等方式。
**Non-Functional需求 （NFR）**
这种需求也叫软件质量，它们不能用Use case, sequence diagram等方式描述。满足NFR最重要的是Finness，它直接描述了软件质量：是不是有必要？是不是满足用户需求？够稳定吗？够快吗？安全吗？可负担吗？能按期完成吗？当需要的时候可以修改吗？可知NFR不仅和软件本身相关，与使用软件的环境也有关系。
从用户角度出发的NFR叫做Quality Factor，从开发人员的专业角度出发的NFR叫做Design Criteria。
方法又分订量与质量的。质量的研究就是基于软件是否能够实现目标。而NFR本身是难以量化的，但是可以设计出诸如“每小时出错次数”来量化衡量是否稳定，用“平均上手时间”来衡量软件对用户的友好程度。
**Goalmodeling**
Goal就是目标，Goal modeling就是一个需求相关的目标hierarchy, 通常可以由需求分析文档中提取。
在UML建立之前就可以建立Goal模型，这有助于明确目标以选取合适的设计和实现。它的好处在于能表达更清晰和稳定的信息。
一个Goal模型一般是分层的树形结构，也就是由一个大目标下面有子目标。每个目标用空心圆型表示，每个子目标都有箭头指向更大一级的目标。在箭头连线上可以表明And或者Or来表示是不是全部子目标都要满足才可以实现更大的目标。
一种Soft Goal模型不设主要目标，多个目标平级，互相也不是必须有关联,而且常常是NPR的目标，使用像云一样的空心符号表示。除了上述两种标识外，还有“+”和”++”表促进，“-”和“--”表反作用和阻碍。比如在某模型中自动化技术会促使冲突(Conflicts)，手册会阻碍冲突。
![](https://img-my.csdn.net/uploads/201204/30/1335764937_6991.jpg)
下面介绍两种最有特色Goal建模方法：KAOS(读作Chaos)和I*(读作I star)
KAOS模型
特点是吧Goal转化为Requirement，把Responsibility指定给Agent。
分为五个部分：
Agent：活跃的系统组件。
Object：不活跃的系统组件。
Operation：Agent实现Goal的Action。
Expectation：就是Goal。
Requirement：某个自动组件的需求。![](https://img-my.csdn.net/uploads/201204/30/1335765225_4781.jpg)
KAOS以确立Agent的responsibility为目标，另外明确Operation作为Agent达成目标的手段。
I*模型
涵盖了Actor-oriented和Goal Modeling的特色。
分为两类：
Strategic Dependency Model(SD)：Actor之间的网络。每个Actor相当于黑盒。
Strategic Rationale Model(SR)：对每个actor建模，可包含SD。么个Actor相当于白盒。
I*图表由一些白色外框和有半圆键头的连线组成。与一般的箭头不同，半圆箭头要划在连线的中央。主要的外框如下:
方形外框，表示ResourceDependency；
六边形外框，表示TaskDependency；
椭圆外框，表示GoalDependency；
凹陷的四边形外框，表示SoftgoalDependency；
圆型表示开放(Open)；
叉子表示重要(Critical)。
以上可作为理解和画图的依据。优势是组织结构非常清楚。可以用来生成Class和Use Case。
![](https://img-my.csdn.net/uploads/201204/30/1335764944_7752.jpg)
### [Domain Specific Design Patterns](http://blog.csdn.net/wangxiaojun911/article/details/7397316)
通用的设计模式由於太过抽象，对某个具体问题常常很底效。若使用Domain Specific Design Patterns (DSDP)
，可以针对某一个特殊的设计模式进行优化。DSDP是由objects和component组成的，有特定边界的，适用于某一种领域的设计模式。关于Domain举例如下：
Real-time Systems(RS)
一个RS是一个有multitask和prioritizing功能的系统。
Network ManagementInterface (NMI)
NMI是在分布式系统中用于控制和监控系统的接口。它具有一个中心控制器。
Graphical UserInterfaces (GUI)
包括键入设备，显示设备，窗口，目录，图标等。
### [AspectOriented Language (面向侧面的程序设计AOP)](http://blog.csdn.net/wangxiaojun911/article/details/7389131)
解决了OOP中的“Tangled Code”问题。
横切关注点（crosscuttingconcerns）：在传统的程序设计语言中，除了主关注点可以被支配性分解方式捕捉以外，还有许多没有被支配性分解方式捕捉到的关注点，这些关注点的实现会弥散在整个软件内部，这时这些关注点同主关注点是横切的。（wiki原文）
支配性分解（dominantdecomposition）：将软件分解成模块的主要方式。传统的程序设计语言是以一种线性的文本来描述软件的，只采用一种方式（比如：类）将软件分解成模块；这导致某些关注点比较好的被捕捉，容易进一步组合、扩展；但还有一些关注点没有被捕捉，弥散在整个软件内部。支配性分解一般是按主关注点进行模块分解的。（wiki原文）
侧面（aspect）：在支配性分解的基础上，提供的一种辅助的模块化机制，这种新的模块化机制可以捕捉横切关注点。
An aspect weaveris a metaprogramming utility for aspect-oriented languages designed to takeinstructions specified by aspects (isolated representations of a significantconcepts in a program) and generate the final implementation
 code. （wiki原文）
Join point: execution Point.
Pointcuts: a collection of join point.
Advice: method-like constructs additionalbehavior at join points.它连接了pointcut和一段额外的代码，有三种情况：before, after和around。
Aspects: pointcuts, advice, regular code
In aspect-orientedcomputer programming, a pointcut is a setof join points. Whenever the program executionreaches one of the join points described in the pointcut, a
 piece of codeassociated with the pointcut (called advice) isexecuted. This allows a programmer to describe where and when additional codeshould be executed in addition to an already defined behavior. This permits theaddition
 of aspects to existing software, or the design of software with aclear separation of concerns, wherein theprogrammer weaves (merges) different aspects into a complete application.（wiki原文）
下面是在AspectJ中调用一个Pointcuts:
Pointcut call(void Point.setX(int))
可以把一组Pointcuts用一个统一的名字来命名。
最后总结AOP和OOP的关系：AOP无法取代OOP；AOP使OOP更强大；AOP也会带来额外的开销，在不需要AOP的部分尽量都用OOP。
**參考資料**
Betty Cheng, MichiganState University, softwareengineering lectures
