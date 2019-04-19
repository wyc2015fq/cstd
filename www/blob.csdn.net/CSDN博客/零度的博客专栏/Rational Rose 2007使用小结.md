# Rational Rose 2007使用小结 - 零度的博客专栏 - CSDN博客
2016年08月04日 10:38:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：659
                
1.Rose如何隐藏类的属性和操作？
右击类，选Options->Suppress Attributes/Suppress Operations
2.Rose如何表示类的约束？
在工具箱中选ABC图标（Text Box），输入用大括号括起来的自由文本。
3.Rose如何表示抽象类？
打开类规格窗口。选择Detail页签，复选Abstract
4.Rose中接口的三种表示方法
右击接口，点Options->Steretype Display->None/Label/Decoration/Icon
5.Rose如何表示关联类？
关联类是关联和类的结合，在工具箱中选Association Class，连接类和关联
6.Rose如何表示模板类？
打开类规格窗口，在General页签中的Type下拉框中选择ParameterizeClass
7.Rose如何表示嵌套类？
打开类规格窗口，在Nested 页签中insert一个类，即新建了一个嵌套类的内部类
8.Rose中如何表示不同的依赖？
在依赖的旁边加自由文本，在工具箱中选ABC图标（Text Box），输入用书名号括起来的自由文本。
9.Rose中如何表示类与接口的实现关系？
在工具箱中选Realize，连接类与接口（从类到接口），改变接口的显示方式。
10.Rose中如何表示双向关联？
打开关联规格窗口，在Role A Detail或Role B Detail中取消对Navigable的选取。
11.Rose中如何表示关联的角色名和多重值？
右击关联的一端，点Role name增加角色名；右击关联的一端，点Multiplicity->选取值
12.Rose中如何表示聚合和组合关系？
打开关联规格窗口，在Role A Detail或Role B Detail中只选取Aggregate，即为聚合关系；在Role A Detail或Role B Detail中选取Aggregate和By Value，即为组合关系。
Rose中的组合关系也可以没有箭头，打开关联规格窗口，在Role A Detail或Role B Detail中取消对Navigable的选取
13.Rose中如何表示关联的限定符？
右击关联，点New Key/Qualifier
14.Rose中如何画对象图？
Rose不支持对象图，但可以用协作图表达对象之间的关系
15.Rose图如何输出到图片文件？
方法一 ：针对每一张绘制好的uml图形，使用ctrl+A  ctrl+C   最后打开word, 执行ctrl+V就可以了，一般这种情况下是可以将每张图都粘贴到文档中然后就可以发送给他人共享了。
如果中文乱码 ctrl+A全部选中，右键Format->Font把字体改为宋体在word里面就不会乱码
              copy到windows的画图如果乱码的法，一般是设置仿宋gb2312楷书gb2312 就不会乱码
方法二 ：针对每一张绘制好的uml图形，右击UML图，选择“Print Diagram”可以导出为Microsoft Document Imaging(MDI)文件。
方法三 ：上两个方法可以有效地将每张图片保存，但是一旦模型中有多个图的话就不是很方便，建议使用rational rose中的publish web功能，这个功能位于rational rose的菜单tools->web publisher，点击进去后修改导出的文件路径即可把弄好的uml模型都导出到web页面中，其中既有模型的图片还有各个图之间的关系，模型之间的联系。单个图可以在相应的目录中找到。
16.如何修改Rose图形中的字体？
改变缺省字体的方法是，点Tools->Options->General，修改Default font。
改变图形中已有文本的字体的方法是，右击图形中的元素，然后点“Format->Font”；或者在图形中用ctrl+A全选，然后右击图形，点“Format->Font”；
17.Rose中如何对消息进行多级编号？
选择Tools-> Options； 单击Diagram标签；同时选中Sequence Numbering和Hierarchical Messages复选框。
18.Rose协作图中消息（link message）的画法，Data Token的画法
先画连接（object link），然后选择工具箱中的消息图标，点连接。选择工具箱中的Data Token图标，点消息。
19. Rose协作图中多对象的画法
            右击对象，点Open Specification，选择复选框“Multiple instance”
20.Rose新建模型时，没有选择模板的对话框，如何设置才能出现选择模板的对话框？
            Add-Ins->Add-In Manager，选择Framework Wizard
 21.顺序图与协作图的转换
      从顺序图转换为协作图：打开顺序图，选择Browse->Create collaboration diagram或按F5
      从协作图转换为顺序图：打开协作图，选择Browse->Create sequence diagram或按F5
