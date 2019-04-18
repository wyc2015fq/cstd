# PowerDesigner入门 - 深之JohnChen的专栏 - CSDN博客

2017年09月08日 14:41:01[byxdaz](https://me.csdn.net/byxdaz)阅读数：487标签：[PowerDesigner 入门](https://so.csdn.net/so/search/s.do?q=PowerDesigner 入门&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)



一、介绍

PowerDesigner是一款功能非常强大的建模工具软件，足以与Rose比肩，同样是当今最著名的建模软件之一。Rose是专攻UML对象模型的建模工具，之后才向数据库建模发展，而PowerDesigner则与其正好相反，它是以数据库建模起家，后来才发展为一款综合全面的Case工具。

PowerDesigner主要分为7种建模文件：

1. 
概念数据模型 (CDM)

对数据和信息进行建模，利用实体-关系图（E-R图）的形式组织数据，检验数据设计的有效性和合理性。

2. 
逻辑数据模型 (LDM)

PowerDesigner 15
新增的模型。逻辑模型是概念模型的延伸，表示概念之间的逻辑次序，是一个属于方法层次的模型。具体来说，逻辑模型中一方面显示了实体、实体的属性和实体之间的关系，另一方面又将继承、实体关系中的引用等在实体的属性中进行展示。逻辑模型介于概念模型和物理模型之间，具有物理模型方面的特性，在概念模型中的多对多关系，在逻辑模型中将会以增加中间实体的一对多关系的方式来实现。

逻辑模型主要是使得整个概念模型更易于理解，同时又不依赖于具体的数据库实现，使用逻辑模型可以生成针对具体数据库管理系统的物理模型。逻辑模型并不是在整个步骤中必须的，可以直接通过概念模型来生成物理模型。

3. 
物理数据模型 (PDM)

基于特定DBMS，在概念数据模型、逻辑数据模型的基础上进行设计。由物理数据模型生成数据库，或对数据库进行逆向工程得到物理数据模型。

4. 
面向对象模型 (OOM)

包含UML常见的所有的图形：类图、对象图、包图、用例图、时序图、协作图、交互图、活动图、状态图、组件图、复合结构图、部署图（配置图）。OOM
本质上是软件系统的一个静态的概念模型。

5. 
业务程序模型 (BPM)

BPM
描述业务的各种不同内在任务和内在流程，而且客户如何以这些任务和流程互相影响。 BPM
是从业务合伙人的观点来看业务逻辑和规则的概念模型，使用一个图表描述程序，流程，信息和合作协议之间的交互作用。

6. 
信息流模型（ILM）

ILM是一个高层的信息流模型，主要用于分布式数据库之间的数据复制。

7. 
企业架构模型（EAM）：

从业务层、应用层以及技术层的对企业的体系架构进行全方面的描述。包括：组织结构图、业务通信图、进程图、城市规划图、应用架构图、面向服务图、技术基础框架图。

二、PowerDesigner物理数据模型的使用

1．现在开始使用PowerDesigner创建数据库,首先运行程序,进入主界面:

![](https://img-blog.csdn.net/20170908142451353)

2.File—New Model—Physical Data Model—Physical Diagram—Model name
设置为test,DBMS属性设置为MicrosoftSQL
 Server 2010:

![](https://img-blog.csdn.net/20170908142507733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.首先用表格工具创建一个表格模板。

![](https://img-blog.csdn.net/20170908142521622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170908142535058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.
双击表格模板,设置属性,设置**该**表的属性。

![](https://img-blog.csdn.net/20170908142549445)

5.
设置好表名,点击**Columns**标签,设置字段属性。

![](https://img-blog.csdn.net/20170908142558086)

6.因为DeviceID字段我们要设置为**自动增长**,所以要设置它的高级属性,选择DeviceID字段,点击属性按钮,在General面板中勾选上Identity复选框:

![](https://img-blog.csdn.net/20170908143904738)

Comment备注里面用于书写备注信息。备注信息也会生成到sql脚本中。

7．再建立一个person表。

![](https://img-blog.csdn.net/20170908142605693)

8．设置表之间的引用关系，为表创建一个外键。

1）选择引用图标。

2）左键拖动鼠标。从需要添加外键的表开始，一直到外键主表结束。

![](https://img-blog.csdn.net/20170908142613276)

9.
如何建立视图

工具栏中单击视图（view）按钮→创建视图→双击视图打开属性窗口，其中Usage是表示视图是只读的还是可更新的。如果我们只创建一般的视图，那么选择query
 only选项即可。

![](https://img-blog.csdn.net/20170908142727659)

切换到SQL Query选项卡，在文本框中可以设置定义视图的sql查询语句，在定义视图时最好不要使用*，而应该使用各个需要的列名，这样在视图属性的Columns中才能看到每个列。单击右下角Edit
 with SQL Editor按钮，即可弹出SQL Editor编辑器，编写SQL语句。也可采用其他sql语句生成器生成sql语句。![](https://img-blog.csdn.net/20170908142743879)

10. 
如何建立索引

双击表打开属性窗口→选择Indexes选项卡→新建一索引→双击该索引打开属性窗口

![](https://img-blog.csdn.net/20170908142801066)

选择Columns选项卡→单击Add
 Columns图标按钮→选择要建立索引的字段→确定

这里Column definition就不要选了，会与主键冲突。

![](https://img-blog.csdn.net/20170908142822707)

11. 
如何建立存储过程

工具栏中单击Procedure按钮→创建存储过程→双击存储过程打开属性窗口→选择Definition选项卡，其中在下拉列表框中，有Default
 Procedure、Default Function这两个选项，前者是定义过程，后者是定语函数，系统会根据选择的类型创建SQL语句的模板→编辑存储过程脚本。

![](https://img-blog.csdn.net/20170908142831508)

12.生成数据库脚本，执行操作:**Database—Generate Database**,设置好存储过程导出目录和文件名,点击确定即可。

![](https://img-blog.csdn.net/20170908142621577)

三、Powerdesigner里PDM模型和CDM模型之间的相互转换

菜单Tool里面的Generate to Conceptual Data Models上面也有快捷键：ctrl＋shift＋c 如果cdm转pdm：ctrl＋shift＋p

四、PowerDesigner使用过程当中的问题汇总

1．PowerDesigner连接数据库的时候不用使用odbc,使用JDBC就可以，ODBC时有问题。

2．PowerDesigner不支持JDK64位的，所以需要使用JDK32位的，否则会出现couldnot
 initialize JVM的问题。

3．None
 SQL error:could not find the com.MySQL.jdbc.Driver class ,原因是PowerDesigner对JDBC支持不好，需要在环境变量当中配置classpath为mysql
 jdbc jar包的路径,而且路径当中不要出现中文。

4．PowerDesigner
检查模型出错“constraint nameuniqueness”

PowerDesigner生成的约束名字相同，只要改一下约束的名字就可以。双击约束,
在 Integrity
选项卡下面的 constraint name
修改下名字。

