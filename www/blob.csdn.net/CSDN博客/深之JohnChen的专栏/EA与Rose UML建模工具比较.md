# EA与Rose UML建模工具比较 - 深之JohnChen的专栏 - CSDN博客

2006年03月15日 10:06:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2113标签：[uml																[工具																[vb.net																[manager																[设计模式																[sql server](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=vb.net&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=uml&t=blog)
个人分类：[软件工程](https://blog.csdn.net/byxdaz/article/category/184372)


**                                                                           EA与Rose UML建模工具比较**

作者：不详　来源：软考联盟 　  http://www.csai.cn  2005年08月08日

　　自从1997年正式发布UML以后，大量商用UML建模CASE工具粉墨登场。这样为我们提供了许多的选择，同时也要求我们在选择正确的UML建模工具以更好地适应我们业务和软件应用程序开发需求，达到最好的投资回报率（ROI）方面做大量的调查。在这篇文章中，我们将比较两款CASE工具的UML建模能力、双向工程特性和项目生命周期支持：Sparx Systems的Enterprise Architect(EA)专业版V.3.51和IBM Rational的Rational Rose企业版V.2002。 

**　　为什么我们需要UML建模CASE工具 **

　　今天，系统的构建变得越来越复杂，UML建模CASE工具为项目相关人员(如，项目经理，分析员，设计者，构架师，开发者等)提供了许多的好处。UML建模CASE工具允许我们应用规范的面向对象分析和设计的方法与理论，远离纠缠不清的源代码，达到构建和设计变得更直观，更容易地理解与修改的层次。在大型项目中，使用CASE工具更重要。通过使用CASE工具： 

　　* 通过用例模型，业务/系统分析可以捕获到业务/系统需求。 

　　* 设计者/构架师所作的设计模型能在不同层次的同一层内清晰表达对象或子系统之间的交互（典型的UML图如类图和交互图）。 

　　* 开发者能快速地将模型转变为一个可运行的应用程序，寻找类和方法的子集，以及理解它们如何交互。 

　　模型被看作是蓝图和构建系统的最终手册。同样，建模也就是一种从高层并以适当的形式来考虑一个设计的表述和理解它怎样运行的能力。 

　　出于这些动机，UML CASE工具以及对应的方法论为我们提供了一种因系统太复杂而不能理解下层源代码的描述系统的方法，同时允许我们更快更便宜地开发正确的软件解决方案。 

　　 当然，要考虑CASE工具在UML建模能力，项目生命周期支持，双向工程，数据建模，性能，价格，可支持性，易使用性等方面的不同。这篇文章将探索Rose与EA在UML建模，项目生命周期支持以及双向工程领域的相同点和不同点，希望能帮助你在你的项目中选择正确的工具。 

**　　UML建模特性 **

　　UML标准由三部分组成，即：构造块（如对象，类，消息），构造块间的关系（如关联，泛化）和图（如，活动图）。UML profile使用UML可扩展性机制扩展标准UML符号，即，构造型，标注值和约束。EA专业版V.3.51和Rational Rose V.2002.05都支持UML 1.4　九种图中的八种标准UML图 - 用例图，类图，序列图，协作图，活动图，状态图，实现图（组件）图，部署图，和几种UML Profiles. 如果需要，对象图可以使用协作图来创建。不同点仅仅存在于创建UML图（表1）和扩展UML profiles时所支持的一些特性。 

|UML图|功能|EA|Rose|
|----|----|----|----|
|用例图|建立描述领域的边界|Yes|No.但是，一些工作使用文本或包。|
|序列 协作|序列图与协作图之间的相互转化|No|Yes|
|序列|更改消息的范围|Yes|No|
|序列|显示消息层次号码|Yes|No|
|序列 协作|在浏览器中创建对象|Yes|No|
|序列|管理控件的焦点|容易|困难|
|所有|图的属性|Yes|No|

表1. EA和Rose的UML图建模比较 

　　Enterprise Architect 有一个通用的UML profile机制用来加载和运行不同的Profiles。Enterprise Architect为UML profiles指定一个特定格式的XML文件。而在Rational Rose中却需要生成一个附加项。 

　　表2展示了在EA和Rose中UML profiles的可用性。 

|UML Profiles|EA|Rose|
|----|----|----|
|业务流程建模|支持Eriksson-Penker业务流程建模扩展|使用UML活动图|
|业务建模|No|Yes|
|数据建模|Yes|Yes|
|用户体验建模|Yes|No|
|Web建模|Yes|Yes|
|Yes|No|Yes|
|XML DTD|No| |

表2. EA和Rose的UML Profile比较 

**双向工程 **
　　双向工程包括正向工程 －　从模型到代码 和反向工程　－　从代码到模型。一旦设计完成后，使用模型（设计模型和数据模型）信息能够生成特定编程语言的源代码或者数据库的DDL脚本。当开发人员添加/更改代码或数据库实现时，设计和数据模型能够通过双向工程同步代码或DDL脚本以保持一致。表3显示了EA和Rose双向工程的特征。 

|语言|EA|Rose|
|----|----|----|
|ANSI C++|Yes|Yes|
|Visual C++|No|Yes|
|VB6|Yes|Yes|
|Java|Yes|Yes|
|C#|Yes|No|
|VB.NET|Yes|No|
|Delphi|Yes|No. 第三方附加项.|
|J2EE/EJB|No|Yes|
|CORBA|No|Yes|
|Ada83, Ada95|No|Yes|
|Database|Yes. 从数据模型到DDL脚本的正向工程。ODBC数据源的反向工程|Yes.DB2，Oracle, SQL 92, SQL Server, Sybase|
|COM|No|Yes. 只是反向工程|
|Web应用程序|No|Yes|

表3. EA和Rose的双向工程 

　　EA为类生成类的源代码文件放在同一个包里。Rational Rose在VC++或VB中更多的涉及到具体的项目。 Rational Rose也可以通过向导和提供代码模板来创建类，这样可以大大增加源代码生成的数量。另外，EA和Rose都可以应用设计模式。当使用EA时，用户必须自己创建模式，而Rose则提供了Java的20种GOF设计模式。 

**　　项目生命周期的支持 **

　　CASE工具应该为团队中的所有队员完成他们的任务提供支持。关于项目生命周期的支持，EA将大量的功能合成一体,而Rose则主要是一个建模工具，它可以与其他的Rational或第三方工具集成，如RequisitePro, Test Manager, Soda, MS Word, MS Project 以达到同样的目标。 表4比较了EA和Rose在不同科目的功能支持。 

|项目科目|EA|Rose|
|----|----|----|
|业务建模|Yes. 使用UML Profile为业务流程建模|Yes.使用业务用例模型|
|需求管理|Yes. 功能和非功能需求；需求跟踪矩阵|合并RequisitePro|
|分析和设计|Yes；UML类图和交互图，如果需要可以添加一些原型，如<<层>>，<<用例实现>>|Yes；UML类图和交互图 框架向导提供了一系列的模板来构造模型|
|实现|参见表3；适合C++，VB，C#和VB.NET项目|参见表3；支持大多数语言，除了.NET外|
|测试|Yes|No. Quality Architect提供了单元测试，但是它需要其他的Rational工具，如Test Manager,Robot|
|版本控制|不直接支持。使用控制单元，为将来发布计划。|集成SCC相应版本控制应用程序|
|项目管理|风险管理、资源分配、项目预算|No|
|Web发布|Yes|Yes|
|生成报表|Yes|No. 使用SoDA.|
|多用户协作|Yes|Yes|

表4 EA和Rose对项目生命周期的支持 

**　　结论 **

　　大体上，EA和Rose在UML建模能力上有相似的功能。 EA和Rational Rose都支持UML九种图中的八种。 从表1中可以看出EA在用户友好性的灵活性中比Rose更胜一筹，特别是序列图。 在双向工程中，Rose比EA支持更多的语言，除C#和VB.NET 外(事实上，Rational 开发了另外的工具 – Rational XDE for .NET 就是针对.NET环境的)。 表4阐明，在项目生命周期的支持方面，EA相对于Rose来说，是更好的选择。尽管你可以购买其他的Rational工具来协助它，但是绝大部分公司在考虑成本问题时却不认为这是一个可接受的方案。当然，你需要工具上没有或第三方工具不支持的一些其他的特定功能时，这也是要考虑的重要因素。在这一点上，Rational Rose得到了更广的支持。 

　　最后，经过一系列同类型的比较，是不是费用也不同？ 是的――非常大的不同！单是Rose花费就是EA的28倍。如果你要比较项目生命周期的支持，假设你是一个Rose用户，你将必须去购买Rational捆绑的一整套产品 ，如Requisite Pro, SoDA, Test Manager等。虽然附加的工具比同类的EA提供了更丰富的功能，但是在大部分时间里EA的基本功能已经够用了。在写本文时，EA企业版（最高版本支持SQL后台）的费用是$179.00(￡111.58) 而Rational Rose企业版的费用是$5024(￡3140),一天整套开发包（Rational Developer Suite）的费用是$8976(￡5610)。 


