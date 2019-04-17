# VisualGDB：使用VS创建CMake Linux项目 - DoubleLi - 博客园






转载地址：[点击打开链接](http://www.cnblogs.com/robinex/p/7868893.html)



根据VisualGDB官网（https://visualgdb.com）的帮助文档大致翻译而成。主要是作为个人学习记录。有错误的地方，Robin欢迎大家指正。





         本文介绍如何使用VS来创建、构建、调试一个基于CMake的项目。当编辑该项目时，会使用高级CMake项目子系统来自动更新CMakeLists.txt文件。

         注意：开始本文如下步骤之前，确保所使用的VisualGDB为5.3或更新版本。



# 1 选择Linux项目向导

         启动VS并打开”New Project”对话框。”VisualGDB”下选择”Linux Project Wizard”：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120231805321-698910659.png)



# 2 选择项目类型

         在”New Linux Project”页面，”Create a new project”->”Application(executable file)”->”Use CMake”，并勾选”Use the advanced CMake Project Subsystem”复选框：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120231839493-1794558096.png)



# 3 选择Linux电脑

         在下一页选择你的目标Linux电脑并点击”Next”。如果你之前没有使用VisualGDB配置过到这台电脑的连接，参考《使用VS来开发Linux程序》来建立连接。

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120231902946-1790111661.png)



# 4 设置源码访问方式

         下一页设定Linux机器如何访问源码。最简单的方式是采用默认设置：将修改的源码更新到Linux机器上。

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120231928540-1523483960.png)

         你也可以配置VisualGDB将源码直接存储在Linux机器上，并通过SSH方式访问。具体参考《配置VS直接通过SSH方式访问Linux项目》。



# 5 构建项目

         项目创建好后，点击Ctrl+Shift+B来build你的solution：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120231959633-2026589383.png)



# 6 断点调试

         在main()函数中设置一个断点，按F5确保项目能够调试：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232034930-42555887.png)



# 7 GUI方式管理项目target及修改各种配置

         接下来介绍如何使用GUI来管理项目targets及修改各种settings。

## 7.1 再创建一个可执行程序

         首先我们在创建一个可执行程序。Solution Explorer中，在”.vgdbcmake”节点上右键点击，选择”Add”->”New Item”，选择”Executable”，并在location中添加子目录”/subdir”：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232120852-1701173709.png)

         VisualGDB会自动创建”subdir”文件夹，并在该文件夹下创建一个CMakeLists.txt文件，然后使主程序的CMakeLists文件引用该CMakeLists.txt文件。

         然后将main.cpp文件的内存拷贝到新程序的文件中（只是需要替换hello信息），并构建该项目：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232209758-360976312.png)



## 7.2 在新程序中再添加一个源文件

         在新项目AnotherExecutable中再添加一个源文件：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232249211-315527768.png)

         在location中添加文件夹”subdir2”，并将新的源文件保存到该文件夹下：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232324665-1778434028.png)



## 7.3 配置属性/参数

         在VisualGDB CMake Project中的每个CMake target都拥有各自独立的properties set。

         调试相关的properties，比如命令行参数，都存到各自的项目文件中。

         构建(build)相关的properties，比如include目录，将自动保存到CMakeLists.txt文件中。

         对于主应用程序，打开VS Project Properties，并设置命令行参数为非空值：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232359102-953586805.png)



## 7.4 调试观察

         在主程序上右键，选择”Debug”->”Start”，启动一个新的程序实例并开始调试它：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232433383-221738755.png)

         现在可以观察一下VisualGDB如何启动选择的目标，以及命令行参数如何匹配target settings所设置的参数：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232514493-1087460821.png)



# 8 对源文件和目标进行分组管理

         打开项目的VS Properties(通过节点.vgdbcmake)，可以使用”Sorting/Grouping”下的配置来控制VisualGDB的分组策略。比如，设置”Group Source By Types”为”False”：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232601758-1751799441.png)

         设置后，可以看到”Source Files”节点消失了，然后所有的源文件显示在目标节点下：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232638024-566673891.png)



# 9 CMake如何处理子项目

         下面我们将展示CMake如何处理子项目。从子目录下打开文件CMakeLists.txt并添加如下代码：

project(subproj)

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232715336-1608100292.png)

          保存CMakeLists文件后观察Solution Explorer，可以看到两个项目节点，每个项目都有一个声明：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232810680-1333133066.png)



# 10 冗余目标过滤隐藏

         如果一个CMake项目包括其他项目，CMake将会两次展示被包含项目（inner project，内部项目）：一次是作为外部项目(including project/outer project)的部分，一次是作为内部项目(included project/inner project)的部分。

         这显然很不方便，VisualGDB能够自动隐藏冗余目标实例。可以使用VS settings中的”Hide Redundant Targets” settings来对.vgdbcmake项目进行控制过滤：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232857008-620264296.png)

         如果选择了”HideInnerProjectTargets”，所有的targets作为外部项目来展示（就像没有内嵌的项目一样）。

         如果选择了” HideOuterProjectTargets”，VisualGDB将会显示内部项目节点中的所有targets(targets inside the inner project nodes)。



# 11 构建时进行项目过期检测

         和MSBuild项目不同的是，除非你要对项目进行构建(build)，否则CMake项目不会自动检测项目是否过期。所以，当你尝试要调试一个项目时，将会弹出提示对该项目进行构建：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120232932461-1905400436.png)

         如果想改变这种行为，需要打开”Rely on CMake up-to-date Check”，并重新加载该项目：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233013258-239974941.png)

         请注意，上面这种机制并不是一直有效：CMake经常错误地判断一些大型的项目过期了，而实际上它们并没有过期。所有，要使用View->Output->VisualGDB CMake Output面板来观察CMake的诊断输出。



# 12 包含多目录多目标的CMake项目的构建

         如果一个CMake项目在多个目录下包含多个目标(targets)，可以选择性的构建选中的目录。在目标节点上右键，然后选择”Build Target”：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233043836-453945716.png)

         注意，如果相同的目录下包含多个targets，这个”Build Target”命令将会build该目录下的所有targets。



# 13 如何编辑target properties

         下面介绍如何编辑目标的properties。选择其中一个target，打开VS Project Properties，并设置Warning Level为Pedantic：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233126649-1888512993.png)

         然后观察VisualGDB如何自动将target_compile_options声明添加到对应的CMakeLists文件中：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233159461-309436165.png)



# 14 如何为CMake项目设置全局settings

         最后介绍如何为VisualGDB CMake项目设置全局的settings。在项目节点上右键选择”VisualGDB Project Properties”：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233247508-395266486.png)

         在VisualGDB Project Properties窗口中，可以配置各种settings，比如Intellisense，static code analysis，debug/deployment settings：

![](https://images2017.cnblogs.com/blog/1251615/201711/1251615-20171120233335493-876129138.png)









