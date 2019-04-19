# UML建模详解（9）—Rose将C++代码自动生成UML类图详解 - fanyun的博客 - CSDN博客
2016年09月18日 12:29:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2890
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一.类图****( class diagram )****即**** c++ ****中的**** class:**
**聚合( Aggregation )即我们c++中的引用**：表现为
 class 头文件中的一个或多个指针成员。
**组合( Composition )即 c++中的成员对象**：表现为class头文件中的一个或多个对象成员。（也可以表现为指针成员，但需要在类中实例化此对象）。
**泛化( Generalization )即继承**。
**双向关联(Bidirectional Associations) 　轻量级的聚合与组合**：即c++中的交叉引用。
**依赖（ Dependency )从另一个维度表述组合和聚合**：即在class
 A 中使用 class B的方法或操作 class B
的成员。
用例视图( user case view )、逻辑视图( logical
 view )、组件视图( component view )、部署视图( deployment view )。
rose 提供了 reserver engineer（逆向设计）的功能，用于将
 c++类文件转化为类图，并描述类与类之间的关系。
**用 rose逆向一组 c++ class的一般步骤是：**
(1).新建一个空白工程
(2).添加一个组件，用于容纳一组类文件。
(3).设置组件属性。指定解析类文件使用的插件，以及逆向出的类使用的包名称。
(4).解析，在逻辑视图中的指定包( package )中生成相应的类图。
(5).将关注的类拖到指定的视图中，观察其结构以及与其它成员的关系。
1、新建一个空白工程。完成后工程信息如下:
图中 untitled表示这是一个未命名的文档。四个大文件夹分别表示用例视图、逻辑视图、组件视图、部署视图。每个大文件夹下面有个
 main结点。这是用来显示该视图内容的“桌布”。双击此结点，便会在主窗口中打开此桌布。结点可以添加、重命名。
![](https://img-blog.csdn.net/20160615100655361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、新建组件，指定其相关属性。
       在工程的 compnent view (组件视图)结点上单击，选择
 new -> component。便创建了一个新的组件。右键此组件，选择"openspecifition"(属性设置)。在弹出的
 component （组件属性）窗口的 general选项卡上，更改其name(名字)属性和
 language 属性。name任意， language选择
 ANSI c++。（默认为 analysys）。点“确定”。这里，我们将其名称命名为
 "constol"
3、指定相关的类文件。
      右键 constol组件，选择“open ANSI c++ specifition ”，在出现的属性窗口中
 file选项卡中，更改 Reverse engineering rootpackage项，（默认为 c++ reverse Engineered。）名称随意。这里我们改为constol。指定 project
 files（项目文件）。把要解析的 c++源文件都包含进来。点“确定”。
4、回到工程视图。
      在 component中的 constol上，右键，“ANSI
 c++”-> “reverseengineer”，点“确定”。这时会在
 logical view 下生成一个新的文件夹结点，结点名称为我们在第3步中　Reverse
 engineering rootpackage 项中添的内容。结点下面包含了由源代码中解析出的类。
5、在 logical view中，双击
 main结点。便打开了一个 main逻辑视图。挨个把 logical view -> constol结点下的所有类结点拖到
 main 视图中。则类的成员、类的关系会在视图中显示出来。
