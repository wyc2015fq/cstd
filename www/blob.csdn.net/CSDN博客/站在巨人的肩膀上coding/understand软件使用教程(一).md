# understand软件使用教程(一) - 站在巨人的肩膀上coding - CSDN博客





2018年05月22日 22:25:55[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：534








源代码阅读工具(Scientific Toolworks Understand)的特色

1、支持多语言：Ada, C, C++, C#, [Java](http://lib.csdn.net/base/java), FORTRAN, Delphi, Jovial, and PL/M ，混合语言的project也支持
2、多平台： Windows/[Linux](http://lib.csdn.net/base/linux)/Solaris/HP-UX/IRIX/MAC OS X
3、代码语法高亮、代码折迭、交叉跳转、书签等基本阅读功能。
4、可以对整个project的architecture、metrics进行分析并输出报表。
5、可以对代码生成多种图（butterfly graph、call graph、called by graph、control flow graph、UML class graph等），在图上点击节点可以跳转到对应的源代码位置。
6、提供Perl API便于扩展。作图全部是用Perl插件实现的，直接读取分析好的[数据库](http://lib.csdn.net/base/mysql)作图。
7、内置的目录和文件比较器。
8、支持project的snapshot，并能和自家的TrackBack集成便于监视project的变化。





软件说明

      功能比sourceinsight多些(比如enum的值的显示),而且#ifdef包含的比较多嵌套,sourceinsight如果太多嵌套,有bug. 流程图之类也比sourceinsight完整.sourceinsight的显示明显好多了,而且体积比较小,编辑速度快,可能是用熟的关系,还是sourceinsight比较顺手.
可惜sourceinsight太长时间没有更新了. 现在我一般用sourceinsight,需要看某些SI没有的用understand.另外好像两个软件都不太支持#if (defined(MACRO) || defined(MACRO1))这样的解析.


## 导入项目

导入项目有两种方法，一种是从菜单栏点击File–>New–>Project,另一种是点击下面界面中间的New Project



点击后，会进入到如下界面，你可以更改项目名称为你要导入的项目名称，以便于以后查找，你可以直接导入你正在开发的项目，你的代码更改后，这个项目也会自动更新，方便你快速开发，不需要每次导入。



更改名称后点击Next进入如下界面，这个界面是让你选择你要导入项目包含了哪几种语言，注意，在C/C++后面有两种模式，下面有注释，其中Strict模式包含Object-C和Object—C++，还有Web的注释，自己看看就好了，在此就不再解释，



然后点击Next进入下面界面：



在此界面点击上面的“Add a Directory”,也就是添加你要导入项目的路径，点击后会弹出如下界面,此时有个奇葩就是弹出的界面会被上图界面遮挡，此时你要移开该界面，然后会出现下面界面：



点击后面的带有三个点的按钮选择你要加入的项目文件夹，此处不用打开文件夹，只要点中文件夹点击open按钮：



此时只需要点击OK即可，界面会跳转到如下界面：



此时有两个选项，一个是立即分析代码，一个选择配置，对于我们来说只需要默认即可，然后点击OK按钮，此时软件开始分析代码，分析完成后会出现如下界面：



左侧会出你的项目结构，中间出现你项目的名称，此时你可以操作左面项目来查看相关代码，如下图所示：



这么多类和方法如何快速定位，那肯定是搜索，该软件针对不同位置，不同属性有不同的搜索方法，下面介绍搜索功能。

## 搜索功能

1.左侧项目结构中搜索：在这个搜索中你可以快速搜索你要查看的类，快捷键，鼠标点击左侧上面项目结构窗口，然后按command + F键会出现如下图所示的搜索框，在框中输入你想要的类回车即可



2.类中方法搜索：将鼠标定位到右侧代码中，点击command + F，会弹出搜索框，输入方法回车即可：



3.在文件中搜索：也就是全局搜索，快捷键F5或者去上面菜单栏中的search栏中查找，输入你想要的类或者方法，回车查找，下面会列出所有使用的地方：



4.实体类查找：软件菜单栏search中最后一项–Find Entity，点击输入你要查找的实体类，回车查找：



快速搜索是软件快速使用必备的技能，包括我们常用的idea一样，快速定位类，方法，常量等，可以快速帮助我们解决问题。

上面我介绍改软件时提到可以绘制流程图等功能，下面就针对这个功能介绍一些一些图形的绘制功能，帮助你快速分析代码。

## 项目视图

项目视图包含很多的功能，能够自动生成各种流程图结构图，帮助你快速理清代码逻辑、结构等，以便快速理解项目流程，快速开发，视图查看方式有两种，一种是鼠标点击你要查看的类或者方法等上面，然后右键弹出菜单，鼠标移动到Graphical Views，然后弹出二级菜单，如下图所示：



另一种方式是点击要查看的类或者方法，然后找到代码上面菜单栏中的如下图标：



然后点击图标右下角的下拉箭头，弹出如下菜单，即可选择查看相关视图：



### 层级关系视图分类：

1.Butterfly：如果两个实体间存在关系，就显示这两个实体间的调用和被调用关系；如下图为Activity中的一个方法的关系图：



2.Calls：展示从你选择的这个方法开始的整个调用链条；



3.Called By：展示了这个实体被哪些代码调用，这个结构图是从底部向上看或者从右到左看；



4.Calls Relationship/Calledby Relationship:展示了两个实体之间的调用和被调用关系，操作方法：首先右键你要选择的第一个实体，然后点击另一个你要选择的实体，如果选择错误，可以再次点击其他正确即可，然后点击ok；





5.Contains:展示一个实体中的层级图，也可以是一个文件，一条连接线读作”x includes y“；



6.Extended By:展示这个类被哪些类所继承，



7.Extends:展示这个类继承自那个类：



### 结构关系视图分类：

1.Graph Architecture：展示一个框架节点的结构关系；

2.Declaration:展示一个实体的结构关系，例如：展示参数，则返回类型和被调用函数，对于类，则展示私有成员变量（谁继承这个类，谁基于这个类）

3.Parent Declaration:展示这个实体在哪里被声明了的结构关系；

4.Declaration File:展示所选的文件中所有被定义的实体（例如函数，类型，变量，常量等）；

5.Declaration Type:展示组成类型；

6.Class Declaration:展示定义类和父类的成员变量；

7.Data Members:展示类或者方法的组成，或者包含的类型；

8.Control Flow:展示一个实体的控制流程图或者类似实体类型；



9.Cluster Control Flow:展示一个实体的流程图或者类似实体类型，这个比上一个更具有交互性；

10.UML Class Diagram:展示这个项目中或者一个文件中定义的类以及与这个类关联的类



11.UML Sequence Diagram:展示两个实体之间的时序关系图；



12.Package:展示给定包名中声明的所有实体

13.Task:展示一个任务中的参数，调用，实体

14.Rename Declaration:展示实体中被重命名的所有实体

由于视图比较多，所以就一一贴上代码，主要还是需要自己去调试，查看各个功能视图的展示结构以及作用，孰能生巧，多操作几下就会了，所以不再做过多的解释。最终希望这款软件能够帮助你快速开发，快速阅读别人的或者自己的代码。






## 一、术语 Terminology
- Architecture 层级:
An architecture is a hierarchical aggregation of source code units (entities). An architecture can be user created or automatically generated. Architectures need not be complete (that is, an architecture’s flattened expansion need not reference every source entity in the database), nor unique (that is, an architecture’s flattened expansion need not maintain the set property).
层级表示代码单元（或者实体）组成的层次结构，可以由用户手动创建，也可由本 
软件自动生成。一个层级可以不完整（例如一个层级的扁平化扩展有可能不会关联[数据库](http://lib.csdn.net/base/mysql)中的所 
有代码实体），也可能不唯一（扁平化扩展的层级可能不会处理其预设属性）。
- Database 数据库
The database is where the results of the source code analysis, as well as project settings, are stored. By default, this is a project’s “.udb” file.
代码经分析后产生的中间结果，以及工程设置保存在数据库，其缺省扩展名为“.udb”。
- Entity 实体
An Understand “entity” is anything it has information about. In practice this means anything declared or used in your source code and the files that contain the project. Subroutines, variables, and source files are all examples of entities.
Understand 描述的“实体”表示任何包含信息的事物，具体来说，代码中声明或 
者使用的标识、包含工程的文件、子程序、变量、源文件都可以被称为实体。
- Project 工程
The set of source code you have analyzed and the settings and parameters chosen. A “project file” contains the list of source files and the project settings.
表示源代码的集合以及相关的配置和参数，工程文件包含源文件清单和工程设置。
- Relationship 关联
A particular way that entities relate to one another. The names of relationships come from the syntax and semantics of a programming language. For instance, subroutine entities can have “Call” relationships and “CalledBy” relationships.
互作用的实体之间的关系，关联的名称来源于编程语言的语法和语义，例如过程式实体具有“调用”和“被调用”的关联对象。
- Script 脚本
Generally a Perl script. These can be run from within Understand’s GUI, or externally via the “uperl” command. The Understand Perl API provides easy and direct access to all information stored in an Understand database.
通常指perl脚本，脚本可以通过Understand 2.5的图形用户界面或者外部的脚本命令执行。Understand Perl API提供了快捷的访问Understand数据库所有信息的接口。
- parts 部件
下面的图形展示了一些Understand 图形用户界面(GUI) 中常用的部件:
> ![](http://i.imgur.com/WKokbCY.jpg)

## 常用词汇
|单词|释义|
|----|----|
|Macros|宏|
|Variables|变量|
|Constants|常量|
|Functions|函数|
|Reference|引用|
|Metrics|参数|
|Architectures|架构|
|parameters|参数|
|Call|调用|
|Compiler|编译器|
|component|组件、部件、元件|
|portability|可移植性|
|Cluster|集群|
|Function Static Variables|静态函数变量|
|case sensitive|区分大小写|
|Whole Words Only|整词匹配|
|layout|布局|
|summary|综述、总论|
|features|特征|
|control-flow graphs|控制流程图|

> 
## 右键菜单

代码编辑窗口的右键菜单提供了很多浏览编辑操作的快捷方式，也提供了相关实体的特定信息。

以下介绍了右键菜单包含的典型的浏览操作（与点击对象有关）：
- 
View Information 信息查看（参见信息浏览器）

- 
Graphic Views 图形视图（参见）

- 
Edit Source/Definition 代码编辑（参见）

- 
User Tools 用户工具（参见）

- 
Explore 浏览（参见层级查看）

- 
Find In… 查找（参见多文件查找）

- 
Add Favorite 添加到收藏夹（参见收藏夹）

- 
Metrics Chart 度量图表（参见）



以下介绍了右键菜单包含的典型的编辑操作（与点击对象有关）：

> - 
Undo/Redo 撤销/重做

- 
Cut/Copy/Past 剪切/复制/粘贴（参见文本选择和拷贝）

- 
Select All 全选（参见文本选择和拷贝）

- 
Jump to Marching Brace 跳到块末

- 
Select Block 块选择

- 
Hide/Show Inactive Lines 行显示/隐藏

- 
Fold All 折叠所有分支

- 
Soft Wrap 软包装

- 
Comment Selection/Uncomment Selection 注释选择/代码选择

- 
Change Case 大小写切换

- 
Revert 反转

- 
Add Bookmark 添加书签活动



### Project Configuration

> 
使用菜单栏Project>Configure Project，选择C++可以打开如下窗口。

![](http://i.imgur.com/9yPL1eS.png)

C++>Option配置页面包括如下区域：
- 
Compiler： 
选择当前工程使用的编译器或者平台，编译器相关的宏定义根据选择自动设置。需要注意的是，编译器的某些特性有可能没有得到很好的支持。

- 
Compiler Include Paths： 
输入编译器使用的头文件的所在路径，如%include%。

- 
Allow nested comments：>>默认情况下，该选项禁止。打开的状态下，Understand允许[C语言](http://lib.csdn.net/base/c)的注释符（/**/）可以嵌套使用（这种风格被ANSI禁止，但是有一些编译器支持）。

- 
Prepend the names of externally linkable entities with： 
使用一个随意的字符串作为工程中其他语言编写的代码中定义的实体的前缀。

- 
Append the names of externally linkable entities with： 
使用一个随意的字符串作为工程中其他语言编写的代码中定义的实体的后缀。

- 
Create implicit special member functions： 
源代码没有类和结构实体的相关语句时，Understand数据库自动创建默认构造函数，析构函数和隐式声明引用，从而为分析时提供相关的引用实体。该选项默认禁止。

- 
Create references in inactive code： 
如果希望将条件编译控制的非激活代码排除在外，需要取消该选项，默认选中。

- 
Create references to local objects： 
默认情况下，Understand数据库包含所有局部对象，如果希望不包含函数中声明的变量需要取消该选项。Understand 2.5的主窗口可以选择是否需要在HTML报告中包含局部对象。

- 
Create references to macros during macro expansion： 
选中情况下，数据库保存宏解析时的引用关系。有时候，该选择有用。注意，该选项选中导致数据库增加很多引用关系，会变得很慢。默认关闭。

- 
Create references to parameters： 
关闭该选项取消参数的引用关系，默认开启。

- 
Save comments associated with entities： 
此选项控制代码实体前后注释与实体的关联。

- 
Save duplicate references： 
默认情况下，引用关系只在数据库保存一份，选中该选项，会记录重复的引用关系。

- 
Save macro expansion text： 
选中该选项的情况下，可以在选中宏的右键菜单下选择Expanded Macro Text来查看宏定义值。

- 
Use Include cache： 
由于头文件通常被多个源文件使用，默认情况下，分析阶段头文件都采用高速缓存，这样能够加速分析过程，但是需要更多的内存，如果分析过程碰到内存问题，可以将此选项关闭。同时需要注意的是，高速缓存的使用会影响到分析结果。



#### Entity Filter窗口

> 
![](http://i.imgur.com/eKZxFkh.png)

##### 根过滤器
- 
Root Calls： 
仅显示调用而不被调用的实体，如上层代码（mains），硬件调用的代码（中断处理函数）以及哑（不被使用）代码。

- 
Root CallBys： 
仅显示只被调用而不调用其他实体的实体，即最底层的函数。

- 
Root IncludeBys： 
仅显示被其他文件包含的文件，即基础头文件。

- 
Root Classes： 
仅显示不依赖于其他类的类，包括底层类和库类。

- 
Root Decls： 
仅显示最高层的声明过程（Ada）。

- 
Root Withs： 
仅显示被其他单元使用，而不使用其他单元的程序单元（包括包，任务，子程序等）（Ada）。



### 信息浏览器

![](http://i.imgur.com/18TKwr3.png)
>

> 
信息浏览器同步

> 
取消Sync 勾选可以打开多个信息浏览器窗口，Sync 勾选的情况下，选中一个实体或者选择View Information 则会更新当前信息浏览器。

选中File Sync 可以保证信息浏览器内容始终与激活的代码编辑器保持同步。 
![](http://i.imgur.com/Ds1TL7x.png)



### 引用查看

> 
信息浏览器的“References”区域显示了所有引用选中实体的相关信息： 
![](http://i.imgur.com/chDU8Ud.png)

## 工程浏览器

![](http://i.imgur.com/rwZZgEf.png)
>


上下文信息边栏

> 
![](http://i.imgur.com/q0iWPpp.png)

显示当前激活的编辑窗口的结构和信息，标签页包含了以下信息：
- 
Structure Browser
提供当前文件的扩展范围列表，列出了文件中函数，头文件包含，宏定义，类及其他各种结构的名称。名称旁边的图标指示了实体类型。将鼠标移动到一个条目，弹出显示实体类型和名称的活动文本框。使用Ctrl+F 可以在标签页范围内进行查找。

- 
File Information
提供当前文件的信息浏览器。

- 
Scope Information
显示Structure Browser 中选中实体的相关信息。

- 
Context Browser
在左侧显示当前实体在层级结构中的位置，右侧显示当前实体包含的其他实体。文件信息标签页右上角的图标（或者使用Ctrl+,）可以将编辑窗口和CIS 中的文件替换为同目录下同名但不同扩展名的文件。例如，使用这种方法可以在.p/.cpp 和.h 文件之间快速切换。


右键菜单也提供了多种快捷的操作选择。


### 大小写修改

> 
Understand 2.5代码编辑器支持选中文本进行快速的大小写修改，方法如下：

> - 选中代码中的一个或者多个单词。
- 通过菜单栏Edit>Change Case，或者右键菜单中选择Change Case。
- 选择需要使用的大小写修改方式，选项包括： 
![](http://i.imgur.com/doCGMEG.png)



### 层级结构浏览器

> 
层级结构节点（如文件系统目录节点和本季度修改节点）的右键菜单包含了以下一些其他对象的右键菜单没有提供的额外选项：
- 
Graphical Views>Dependency Graphs： 显示层级结构各节点的依赖关系，参见。

- 
Metrics Summary：提供选中节点各实体的度量数据，这些度量数据基于当前节点下的实体，不包括子节点中的实体，参见。

- 
Metrics Export：将度量总结导出到一个CSV 文件，参见。

- 
XML Export：将层级结构中选中节点及子节点包含的实体以XML 格式导出，参见。

- 
Edit Architecutre：为用户自己创建的层级结构打开一个层级结构构建器，Understand 
自动构建的层级结构不能被修改，参见。

- 
Rename Architecture：为用户自己创建的层级结构或者节点打开一个层级结构重命名窗口供用户对其中名称进行修改，Understand 2.5 自动构建的层级结构不能被重命名，参见。

- 
Duplicate Architecture：打开层级结构复制窗口对选中层级结构的副本进行命名，参见。

- 
Manage Architecture：打开层级结构管理窗口，参见。




[1] http://www.cnblogs.com/hackerl/p/5749169.html

[2] http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/a731062834/article/details/50988966



