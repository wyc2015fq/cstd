# 在VS2010中配置ICE插件的步骤 - DoubleLi - 博客园






ICE为不同的IDE都提供了插件。本文介绍VS2010下的配置。

首先建立一个控制台工程，以此为例，进行介绍。

1.激活插件

选择工具-> IceConfiguration或者在工程右键选择IceConfiguration，打开插件的配置界面。

2.配置工程设置

Enable IceBuilder必须选择。

Trace Level 用来控制输出窗口中打印的信息。

Output directoryfor generated files 设置产生的文件的目录，.\generated即可。

Slice include directories设置包含路径。

Link projectwith these additional libraries 设置链接库，每个Ice程序都默认包含 Ice和IceUtil库。如果使用到了额外的库，就勾选相应的BOX。

3.添加Slice文件

工程上右键，添加项目可以创建添加Slice文件。

4,生成代码

保存时，插件自动帮你把Slice文件编译成.h,和.cpp文件并帮你添加到工程中。

5.插件如何定位你的Ice 安装位置。

C:\Users\All Users\ZeroC\Ice.props中定义的几个宏

($(IceHome),$(IceInclude),$(IceBin),$(IceLib))定义了几个路径的位置。

之后就可以编写server或者client程序，并编译了。









