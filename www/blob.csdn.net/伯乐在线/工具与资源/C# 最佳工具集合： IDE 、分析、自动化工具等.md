# C# 最佳工具集合： IDE 、分析、自动化工具等 - 文章 - 伯乐在线
原文出处： [Sweet-Tang](http://www.cnblogs.com/tdfblog/p/best-csharp-tools.html)
C#是企业中广泛使用的编程语言，特别是那些依赖微软的程序语言。如果您使用C#构建应用程序，则最有可能使用Visual Studio，并且已经寻找了一些扩展来对您的开发进行管理。但是，这个工具列表可能会改变您编写C#代码的方式。
# C#编程的最佳工具有以下几类：
- [IDE](#IDEs)
- [VS扩展](#Extensions)
- [编译器、编辑器和序列化](#Compilers)
- [反编译和代码转换工具](#Decompilers)
- [构建自动化和合并工具](#Automation)
- [版本控制](#VersionControl)
- [测试工具和VS扩展](#Testing)
- [性能分析](#Profilers)
- [APM](#APM)
- [部署自动化](#Deployment)
- [容器](#Containerization)
使用上面的链接直接跳转到特定工具，或继续阅读以浏览完整列表。
# IDE
## 1. [Visual Studio Code](https://code.visualstudio.com/)
## ![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/ab7cd9d2dc2e1643611ad6d6dc17a94b.jpg)
Visual Studio Code是C#开发人员最受欢迎的代码编辑器。您可以使用Visual Studio Code与C#扩展功能实现强大的编辑功能，并完全支持C# IntelliSense和调试。
**主要特征：**
- 优秀的自动完成与上下文感知提示
- 内置Git集成，实现更好的版本控制
- 大量的扩展进一步增强平台
## 2. [MonoDevelop](http://www.monodevelop.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/cdbf06f5a31bed35d6e8e99501364c76.jpg)
由Xamarin开发，MonoDevelop具有许多与VS Code相似的功能，也是跨平台的。使用MonoDevelop，您可以在Windows，Linux和Mac OS X上快速编写桌面和Web应用程序。
**主要特征：**
- 标准功能，如代码提示和调试
- 还配有[Unity](https://unity3d.com/)，一个游戏引擎，可以为视频游戏等应用程序实现高级C#编程
- 支持C#代码提示
- 代码模板和代码折叠
- 多语言支持
## 3. [SharpDevelop](http://www.icsharpcode.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/56e59653b98d56cb5a410d98cb7bedf7.jpg)
一个用于C#、VB.net和Boo项目的免费开源IDE，#develop（SharpDevelop）是VS Code的轻量级替代品。
**主要特征：**
- Visual Studio提供的通常功能，包括代码编辑，编译和调试
- 一些高级功能包括上下文操作和后台语法检查
## 4. [Rider](https://www.jetbrains.com/rider/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/9d3f920744876bd8d1a24134838ba714.jpg)
Jet Brains产品套件中一个成员，Rider是基于IntelliJ平台和ReSharper的跨平台.NET IDE。
**主要特征：**
- 与其他JetBrains产品无缝集成
- 2000多种代码检查
- 上下文操作
- 支持Unity游戏引擎
- 支持.NET Framework和.NET Core
# Visual Studio 扩展
## 5. [Productivity Power Tools 2017](https://marketplace.visualstudio.com/items?itemName=VisualStudioProductTeam.ProductivityPowerPack2017)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/b6ea07eb5999a3e0136e07a09f122f49.jpg)
由Microsoft提供，这是一个很好的扩展，以加快Visual Studio的开发。
**主要特征：**
- 中间点击滚动
- 快速查找
- 解决方案错误可视化
- 直观的界面
- 其它版本可用于旧版Visual Studio版本
## 6. [jQuery Code Snippets](https://github.com/kspearrin/Visual-Studio-jQuery-Code-Snippets)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/3e8c0688d1b43afcb054ae41872c6ed4.jpg)
简化jQuery框架编码，jQuery Code Snippets为Visual Studio 2012，2013，2015和2017提供了超过130个代码段。
**主要特征：**
- 130多种jQuery代码片段
- 支持所有版本的Visual Studio
- 对于那些刚入门jQuery开发人员来说非常棒，可以使用这个扩展来学习jQuery的门道
## 7. [CodeMaid](http://www.codemaid.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/2864e73047c391392c93c64b6f0e91cd.jpg)
CodeMaid是一个开源的VS扩展，可以帮助您清理和简化除C#之外各种支持语言的代码。
**主要特征：**
- 从代码中删除不必要的字符，如空格
- 代码整理
- 使用树型结构视图轻松浏览代码
- 支持多种语言
- 注释格式
## 8. [Refactoring Essentials](http://vsrefactoringessentials.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/9c0f0877240c4e938221ed3b681237e4.jpg)
使用Refactoring Essentials，一个具有内置的C#和VB代码转换器的VS扩展，使您的C#代码更容易阅读和修复常见错误。
**主要特征：**
- 修复常见的错字和编码错误
- 在代码库中找到每种不匹配的样式
- 开源代码，您可以扩展或改进它
# 编译器、编辑器和序列化
### 9. [SlickEdit](https://www.slickedit.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/a1333c638778be745f8795863f76f2b9.jpg)
SlickEdit是一个代码编辑工具，可以提高生产力并提高代码质量。它是一个跨平台的代码编辑器，可在九个平台上支持超过60种语言（包括C#）。
**主要特征：**
- 编译并调试您的代码，显示所有错误以便行快速访问
- 可以处理非常大的代码文件，大到2TB
- 语法扩展和缩进
- 按键模拟
- 备份历史记录
- 多个光标和选择
- 标准和专业版可用于满足您的需求
## 10. [Rextester](http://rextester.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/aa72d6d1d2d6d850b8bc04e757da51dc.jpg)
Rextester（代表正则表达式测试器）是C#的简单在线编译器。
**主要特征：**
- 每段代码运行CPU时间最多为5秒
- 每段代码最多可以占用150MB内存
- 代码在应用程序域中运行，具有基本的执行、反射、线程控制和web权限
## 11. [C# Pad](https://csharppad.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/4fac2d2f11390ea2e6dd24c61717f926.jpg)
C# Pad是C#的一个方便的REPL工具。使用起来很简单：输入代码，点击“Go”执行。
**主要特征：**
- 在线快速测试C#代码
- 包括代码自动完成
- 可嵌入其它网站
## 12. [Jdoodle](https://www.jdoodle.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/085e042ba82da71329d3f2acc3a697b8.jpg)
另一个轻量级代码编译器，Jdoodle支持63种语言（包括C#），并允许您保存程序并与其他人共享URL。您可以将Jdoodle中创建的代码嵌入到您的网站，用于协作，在线访问等。
**主要特征：**
- 在线快速测试一小段代码，而无需打开IDE
- 轻松地与他人分享您的代码
- API即将推出
## 13. [.NET Fiddle](https://dotnetfiddle.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/c1c4f3caf8dc79a543e7fc07eb0431b9.jpg)
C#的一个流行的调试工具，.NET Fiddle的灵感来自于JSFiddle.net，旨在快速，简单的代码测试，而无需打开Visual Studio并创建一个新的项目。
**主要特征：**
- 让您编写，保存和协作C#代码
- 支持4种项目类型：控制台，Script，MVC，Nancy
- 还支持VB.NET和F＃
## 14. [Telerik Fiddler](http://www.telerik.com/fiddler)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/1ba1c74d48a3197be5ea9a54663bf102.jpg)
Telerik Fiddler允许您在线执行代码。它提供了一系列功能，包括Web调试、Web会话操作、性能测试、安全测试等。
**主要特征：**
- 运行一系列测试，如性能测试、Web调试和安全测试
- FiddlerCore可让您将Fiddler应用程序嵌入到您自己的应用程序中，以便您可以避免整个Fiddler UI，还可以获得Fiddler的所有功能
## 15. [Json.NET](http://www.newtonsoft.com/json)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/00105e5bd3b3bcdbfe502aaacf9526dc.jpg)
Json.NET是一个流行的JSON框架。使用它，您可以在任何地方（Windows，Windows Store，Windows Phone，Mono和Xamarin）运行它时，创建、解析、查询和修改JSON，在XML和JSON之间进行转换。
**主要特征：**
- 对.NET对象进行序列化和反序列化
- 查询JSON数据库
- 使用JObject，JArray和JValue对象创建、解析、查询和修改JSON
- 开源
## 16. [Scriptcs](http://scriptcs.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/fb23c062ff12d10ad60fb4e3e65407be.jpg)
一个简单的在线编辑器，编写和执行C#代码，Scriptcs被设计为非常简单，以避免使用过于复杂的工具和解决方案阻碍您的工作效率。
**主要特征：**
- 避免完整IDE的臃肿
- 为您提供简单的C#编码任务的灵活性和速度
- 管理与NuGet的依赖关系
- 优雅的C#脚本语法
# 反编译和代码转换工具
## 17. [dotPeek](https://www.jetbrains.com/decompiler/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/5d1169da0b608f78a20702bff1909a77.jpg)
dotPeek允许您将.NET程序集反编译为C#。dotBeek是JetBrains提供的免费独立工具，基于ReSharper捆绑的反编译器。
**主要特征：**
- 强大的搜索和导航功能
- 将任何.NET程序集解码为等效的C#或IL代码
- 支持多种格式（.dll、.exe、.winmd）
- 将反编译的程序集保存为Visual Studio项目
## 18. [ILspy](http://ilspy.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/b14c6254b53a1eef405a965f7e857a35.jpg)
基于Red Gate的.NET Reflector开源反编译器，在2011年停止维护，ILspy需要.NET Framework 4.0。
**主要特征：**
- 浏览程序集并将其反编译为C#
- 将程序集保存为C#项目
- 直观的搜索和导航功能
- 导航历史
- 支持C#5.0 “async”
- 反编译为VB和C#
## 19. [JustDecompile](http://www.telerik.com/products/decompiler.aspx)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/d93095d5cb7fe7a91e8e683a757068d8.jpg)
一个来自Telerik的反编译器，JustDecompile是一个免费的开源反编译引擎。
**主要特征：**
- 可扩展的开放API
- 快速代码导航
- 轻松地在标签页中切换方法和程序集
- 在加载程序集中使用书签
- 从反编译的程序集创建一个Visual Studio项目
- Visual Studio扩展在VS项目中进行反编译
## 20. [Telerik Code Converter](http://converter.telerik.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/55f1c5759c01a823a8cd152e67e9fa28.jpg)
Telerik代码转换器将VB代码转换为C#，反之亦然。一个简单而强大的工具，Telerik代码转换器易于使用，具有直观的界面。
**主要特征：**
- 基于云的转换，快速和轻松的
- 使用NRefactory库进行复杂的代码转换
- 使用ASP.NET AJAX的Telerik UI
# 构建自动化和合并工具
### 21. [Cake Build](http://cakebuild.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/710b109b667b0a2a4c754815d85c73b8.jpg)
Cake（C#Make）是C#版的开源Make Build自动化工具。一个跨平台构建自动化系统，Cake使用C# DSL来编写代码，运行单元测试，复制文件和文件夹，压缩文件和构建NuGet软件包等任务。
**主要特征：**
- 在Windows，Linux和OS X上可用
- 建立与其他CI系统相同的方式
- 支持构建工具，包括MSBuild，MSTest，xUnit，NUnit，NuGet，ILMerge，WiX和SignTool
- 高可扩展
## 22. [MSBuild](https://www.visualstudio.com/en-us/docs/build/steps/build/msbuild)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/dc18c33a85da75cdb5e9e497dfc15020.jpg)
MSBuild是由Microsoft于2008年推出的。它不是Visual Studio平台的一部分，但它是您的C#开发工具包的一个很好的资源，例如在您构建代码项目时，除了解决方案之外。
**主要特征：**
- 来自微软，它被许多MS商店广泛使用
- 微软把它放到了Visual Studio中确保它不会成单独存
- 构建多个项目
- 构建Win32，x86，x64或任何cpu平台
- 为多个平台构建多个配置
## 23. [FinalBuilder](https://www.finalbuilder.com/finalbuilder)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/76cc136d56bf28d5f660a00e8dd0f20c.jpg)
FinalBuilder是构建自动化工具的一个很好的商业选择。使用FinalBuilder，您不需要编辑XML或编写脚本来自动完成构建过程。
**主要特征：**
- 通过包括可视化调试，使构建变得容易
- 可视地定义和调试构建脚本
- 使用Windows调度程序安排脚本
- 与Continua CI，Jenkins或任何其它CI服务器集成
## 24. [SemanticMerge](http://www.semanticmerge.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/27c5f88a28199621439e8d5f121cf978.jpg)
SemanticMerge分析您的代码，旨在处理已被移动或修改的代码，使其成为团队协作的强大工具。
**主要特征：**
- 实现协作和分布式开发
- 从结构合并开始，然后在发生冲突时移动到更具体的基于文本的合并
- 基于结构而不是位置合并
- 处理特定于语言的冲突
- 避免错误地识别自动合并
- 保持清洁的代码并分析更改
# 版本控制
## 25. [Git](https://git-scm.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/2ebd63d78a68b858493da22210f0a2bb.jpg)
Git就不需要介绍了。这是一个免费的开源版本控制系统，也是当今最流行的版本控制系统。
**主要特征：**
- 现代版本控制工具，适用于当今地理位置分布的团队
- 包括强大的默认值，用于解决冲突，自动恢复等
- 便宜的本地分支
- 方便的暂存区
- 多个工作流程
## 26. [Team Foundation Version Control](https://www.visualstudio.com/team-services/tfvc/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/328f6f58124bcf89625bc7bc2aa43639.jpg)
TFS版本控制是Microsoft替代Git的一种，它集成到Visual Studio中。它是企业准备的，所以对于使用VS项目的任何规模的团队来说，这是一个不错的选择。
**主要特征：**
- 利用Visual Studio功能提供本地体验
- 跟踪分支机构的代码更改
- 在代码签入之前提供反馈
- 集成最流行的CI和自动化构建工具
## 27. [BitBucket](https://bitbucket.org/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/83bb4073ba4ba43ec8f09f7221898316.jpg)
BitBucket是Jira的Git替代品，也是一个有能力的版本控制工具——“唯一一个大规模的协作的Git解决方案。”
**主要特征：**
- 使用内联注释选项更快地协作
- 由于代码是开源的，还是让Jira为您管理代码
- 拉请求允许更有效的代码审查批准
- 使用BitBucket数据中心进行不间断的访问
- 无限私有存储库
## 28. [SourceTree](https://www.sourcetreeapp.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/6dbc13b3a314cec4fe0c7366d37abb2c.jpg)
SourceTree是另一个Jira产品，一个用于Windows和Mac的免费Git客户端，具有简单的Git GUI来管理您的仓库。
**主要特征：**
- 支持Windows和Mac
- 与您的存储库进行视觉交互
- 简化分布式版本控制
- 对初学者来说足够简单 足够强大的专家
# 测试工具和VS扩展
### 29. [LINQPad](http://www.linqpad.net/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/8b84ef5c25005a19e0d25f7e19706ac6.jpg)
一个用于即时测试C#，F#和VB中的代码片段的工具，LINQPad可以让您以交互方式查询数据库。
**主要特征：**
- 有自己的查询语言 – LINQ，这是一个强大的查询数据库的方法
- 非常适合学习C#，因为它为您提供了IDE的许多功能，而不臃肿
- 在LINQ（或SQL） – SQL/CE/Azure、Oracle、SQLite和MySQL中查询数据库
- 丰富的输出格式
- 脚本并以您首选的.NET语言自动化
## 30. [Selenium](http://www.seleniumhq.org/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/30d2148445af4f7ae6568772fc5ccae8.jpg)
Selenium是目前最流行的测试工具之一。它可以自动化Web浏览器，以便您可以在规模和精度上运行测试。
**主要特征：**
- 自动化的基于Web的管理任务
- 创建强大的回归自动化套件和测试
- 跨环境和脚本分发
- 构建您自己的Selenium服务器或使用供应商提供的托管选项
## 31. [ReSharper](https://www.jetbrains.com/resharper/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/e0d310155483608ebc407ab9a28c6fda.jpg)
由JetBrains创建的，ReSharper是.NET开发人员的Visual Studio扩展。使用它来即时分析代码质量，消除错误，安全地更改代码库等。
**主要特征：**
- 提供即时代码分析
- 快速修复错误
- 帮助复杂重构
- 多个代码编辑助手
## 32. [OzCode](https://oz-code.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/5aad95b7d82740b30d92429af2aa9bcb.jpg)
OzCode是C#的Visual Studio扩展，提供强大的调试工具，因此您可以花更多的时间开发和更少的时间调试。
**主要特征：**
- 使Visual Studio中更容易找到并修复错误
- 与ReSharper合作
- 用Magic Glance来绘制出复杂的表达式
- 强大的搜索，显示和比较功能
## 33. [CodeRush](https://www.devexpress.com/products/coderush/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/afb8749eb07de32dd5886f8121b86baf.jpg)
CodeRush可以帮助您轻松找到并修复问题，像本节中的其它测试工具一样，是一个VS扩展。
**主要特征：**
- 视觉突出显示代码库中的问题，并帮助您快速发现问题
- 快速创建新的测试和测试套件与快捷方式
- 容易重构
# 性能分析
### 34. [Prefix](https://stackify.com/prefix/)
![162090-20170910211007866-1263253556](http://jbcdn2.b0.upaiyun.com/2017/09/03b06f5d1cb589306d5795789ad8d54d.png)
一个用于.NET和Java的轻量级开发工具，Prefix会实时显示日志、错误、查询等。它可以在您现有的工作站内使用 – 它是免费的。
**主要特征：**
- 提供每个Web请求的详细跟踪
- 没有凌乱的配置或代码更改
- 立即下载; 立即获得代码分析结果
- 按要求查看日志
- 查找隐藏的异常
- 查看SQL查询
- 快速找出可疑行为
## 35. [dotTrace](https://www.jetbrains.com/profiler/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/d305e83fd3ff97da47bbcbd688abb102.jpg)
来自JetBrains，dotTrace是一个分析器，可帮助排除性能问题并加快应用程序的速度。
**主要特征：**
- 了解SQL查询和HTTP请求如何影响应用程序性能
- 查看逐行剖析数据并获取特定的绩效指标
- 获取调用执行时间的数据
- 配置文件SQL查询和HTTP请求
- 简介静态方法和单元测试
- 在Visual Studio中配置和可视化结果
## 36. [ANTS Performance Profiler](https://www.red-gate.com/products/dotnet-development/ants-performance-profiler/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/421d2c22e18956d4c7d759d2189ddb2a.jpg)
ANTS性能分析器是.NET代码的分析工具，包括提供逐行分析统计信息。
**主要特征：**
- 查看SQL查询和HTTP请求的性能数据
- 提供逐行结果
- 显示历史性的表现
- 深入了解长期分析结果
## 37.[N Profiler](https://www.nprofiler.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/193d29df37f2c0a6de25bb5f83b8c30c.jpg)
一个分析工具声称比任何其培训分析器更快，N Profiler提供丰富的.NET性能数据，可以节省90%的优化时间。
**主要特征：**
- 了解性能问题的真实原因 – 数据库查询或I/O时间
- 性能指标组织在调用树中，可以在概述或细节级别查看
# APM
## 38. [Retrace](https://stackify.com/retrace/)
![162090-20170910211007866-1263253556](http://jbcdn2.b0.upaiyun.com/2017/09/54b4463f786af4d51a64b9624ce703e3.png)
Retrace是唯一将APM、日志、错误、指标和监控结合在一个平台上的开发者工具，可帮助您更好地构建。它专为生产和预生产服务器而设计，每月只需10美元。
**主要特征：**
- 在代码级收集性能数据
- 应用程序和服务器记录
- 各种来源的应用指标
- 错误跟踪和监控
- 与现有堆栈一起工作
- 免费试用
# 部署自动化
## 39. [Octopus](https://octopus.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/1143dc56d334c82c08d0e6e076190eb0.jpg)
C#主要部署自动化工具，这使得自动化复杂的应用程序部署成为可能。
**主要特征：**
- 提供和配置新机器来部署您的应用程序
- 与CI服务器集成，将构建自动化和CI进程扩展到部署中
- 精简各环境之间的版本
- 可重复，可靠的部署
## 40. [Appveyor](https://www.appveyor.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/541566723eb5308d235dcfc63e057987.jpg)
一个CI构建服务器，不仅可以处理构建，而且还可以进行部署，Appveyor非常强大，安全，并且无需设置即可开始使用。
**主要特征：**
- 用于构建，测试和部署端到端解决方案
- 易于使用，无需设置
- 隔离构建环境
- 使用UI或YAML的开发人员友好的构建配置
- 内置部署和NuGet服务器
- 多分支构建
## 41.[AppHarbor](https://appharbor.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/38343a06fc098841a183e68f8dc5c71e.jpg)
AppHarbor不仅仅是部署：它是一个完全托管的.NET 平台服务。它用于托管从个人博客到高流量网络应用程序的所有内容。
**主要特征：**
- 从源代码存储库获取代码，并将其部署在自己的托管平台上
- 使用您最喜欢的版本控制工具即时部署和扩展.NET应用程序
- 不用担心扩展您的基础架构来跟上负载，AppHarbor会为您处理这一切
## 42. [Jenkins](https://jenkins.io/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/74f7d0aa59c2d8ac0aad93e8ef7cfe4a.jpg)
Jenkins不仅仅是部署自动化工具，也是最受欢迎的CI服务器，它将其覆盖面扩展到部署和交付阶段。
**主要特征：**
- 丰富的收集扩展，以适应每个堆栈
- 从构建到部署的端到端解决方案
- 数以百计的插件来支持构建，部署和自动化
# 容器
## 43. [Docker.DotNet](https://www.nuget.org/packages/Docker.DotNet/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/15f470b4e14bc30557d58f71c8372b3f.jpg)
Docker.DotNet是Docker API的C#客户端库。通过它，您可以通过编程方式与Docker Remote API进行交互。
**主要特征：**
- 可以通过Visual Studio的NuGet库管理器进行安装
- 在.NET中启用完全异步，非阻塞，面向对象的代码
## 44. [Distelli](https://www.distelli.com/)
![162090-20170910205052272-559220882](http://jbcdn2.b0.upaiyun.com/2017/09/34cc483c247ca99f52ba7cb37d0c2f07.jpg)
一个强大的工具来集中化C#应用程序，Distelli帮助您从头到尾自动化您的容器工作流程。有关如何使用Distelli构建和部署C#应用程序，请参阅[此有用的教程](https://www.distelli.com/docs/tutorials/build-and-deploy-c-sharp-application/) 。本教程提供了构建和部署Docker C#应用程序的分步说明。
**主要特征：**
- 使用Distelli自动化您的容器工作流端到端
- 部署到Kubernetes，用于最先进的容器编排
C#和.NET Core 将在2018年及以后的得到巨大的发展，磨练C#技能将在未来几年内为您服务。我们希望您已经找到了一两种可以使用的工具，并提高了C#开发的质量和生产力。如果有什么工具我们遗漏了，请告诉我们，这个工具应该在这个工具包中有一席之地。
