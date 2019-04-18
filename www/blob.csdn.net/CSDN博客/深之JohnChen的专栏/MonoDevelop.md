# MonoDevelop - 深之JohnChen的专栏 - CSDN博客

2011年01月12日 16:15:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6567


MonoDevelop

MonoDevelop支持使用C#和其他.NET语言进行开发，它使得开发者可以在Linux和Mac OS X上非常迅速的开发出桌面软件和ASP.NET Web应用。除此之外，MonoDevelop还允许开发者非常简单的将Visual Studio开发的.NET应用程序移植到Linux和Mac OS X下，这样开发者只需要维护一套代码即可──因为GTK#是跨平台的。 

　　或许有人对于Microsoft的.NET环境有些抵触，而开放的桌面环境：GNOME早已将开源实现的.NET运行环境Mono纳入了默认支持当中。

GNOME系统的“Tomboy便笺”即是用C#编写，Novell出品的照片管理工具：F-spot也是如此，同样还有著名的索引搜索工具Beagle。

　　通过Mono，能吸引更多的开发者，这何尝不是一件好事？

Miguel de Icaza在其[博客](http://tirania.org/blog/archive/2009/Sep-09.html)上宣布了MonoDevelop的最新版本——2.2 beta 1。这是Novell官方正式支持的第一个跨平台版本，除了支持原本的Linux，现在还支持Windows和Mac OS X。

Miguel de Icaza说道：

一直以来，人们都渴望得到一款跨平台的.NET IDE。 ……直到今天，2009年9月9日，世人可以来尝试这样的工具了。

这个版本在带来Windows和Mac OS X安装包的同时，还和这两个平台也进行了紧密的集成，并支持在每个平台上的原生调试。除此之外，MonoDevelop的一大特色是具有丰富的插件，这次带来的新插件有：ASP.NET MVC开发插件、Silverlight开发插件和iPhone开发插件（利用了[MonoTouch](http://www.mono-project.com/MonoTouch)）。

MonoDevelop 2.2 beta 1包含的[完整特性](http://monodevelop.com/index.php?title=Download/What' rel=)如下：
- Windows支持：官方支持，并提供安装包
- Mac支持：官方支持，并提供安装包
- 项目管理：- 多目标运行时
- 针对每种文件类型提供不同编辑和格式化策略
- 自定义的执行模式
- 全局程序集文件夹
- 删除项目的自定义对话框
- 可以从Mac的Nautilus或Windows Explorer中拖文件到解决方案树上
- 加强了项目重载
- 开始支持.NET 4.0 

- 文本编辑器：- 自动保存
- 代码模板
- 代码块选取
- 提升了编辑大文件的性能
- 提供了新的代码格式化功能
- 即时代码格式化
- 代码自动完成支持首字母匹配
- XML文档支持代码自动完成
- 对vi模式进行了加强
- 可自动生成某些代码片段

- 重构- 可解析命名空间
- 具备预览功能的重命名
- 抽取方法
- 声明局部变量
- 综合临时变量
- 创建常量
- 为类型创建单独的文件
- 删除无用的Usings 
- 对Usings进行排序
- 为属性创建对应的字段，或删除字段
- 支持多种键盘命令
- 内联（inline）重命名

- 调试器- 立即窗口
- 在Windows上利用Win32调试器
- 在Linux上开始支持ASP.NET调试

- ASP.NET MVC插件
- IPhone插件
- Moonlight插件
- 极大加强了Python语言的支持
- 版本控制- 提供显示注解（Show Annotations）的命令
- 加强了审阅更改视图（Review Changes View）的功能
- 新增了创建补丁（Create Patch）的命令

- 其他- 在搜索结果中提供语法高亮
- 加强了数据库插件
- “Go to File”对话框现在支持多选
- 可生成Makefile 
- Vala语言支持的加强
- C/C++插件的代码自动完成现在更加稳定
- C#代码自动完成的加强

随着Mono的逐步成熟，让.NET应用程序跨平台地运行已经成为现实，而随着MonoDevelop支持跨平台并逐步成熟，相信跨平台地开发.NET应用程序也将成为可能。 如果你使用C#的话，还能使用集成GTK#的可视化设计。这是目前为止GNOME环境下唯一的集成可视化设计器的IDE，Anjuta也不支持。

