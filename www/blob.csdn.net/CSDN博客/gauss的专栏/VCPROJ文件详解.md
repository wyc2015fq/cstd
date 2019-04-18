# VCPROJ文件详解 - gauss的专栏 - CSDN博客
2013年01月04日 00:22:33[gauss](https://me.csdn.net/mathlmx)阅读数：387
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
[http://blog.csdn.net/n567656756765676567/article/details/7333502](http://blog.csdn.net/n567656756765676567/article/details/7333502)
vcproj是一个标准的xml文件。因此以下就以节点顺序描述。
####    大概是分为以下几部分：
<VisualStudioProject>    工程属性
<Platforms>                       平台名称
<ToolFiles>                        工具文件，好像经常是没内容，不清楚
<Configurations>             编译和链接的配置信息，了解项目的编译链接一般在这
                     里
<References>                    在实际中，比较少看到，不清楚
<Files>                                目录文件
<Globals>                          全局配置，一般是和全局有关的，比如qt下用的
                                             编译版本4.7.1等, 一般工程移植后出问题都会在这里
一、首先是VisualStudioProject：
属性中包含了工程的全局信息，常见的信息有：
|> > **属性**|> > **含义**|> > **说明**|
|----|----|----|
|> > ProjectType|> > 工程类型|> > **默认值是**Visual C++|
|> > Version|> > 版本|> > **默认值是**7.10|
|> > Name|> > 工程名称|> >|
|> > ProjectGUID|> > 工程的GUID|> >|
|> > Keyword|> > 工程关键字|> > **默认值是**Win32Proj|
|> > SccProjectNameSccAuxPathSccLocalPathSccProvider|> > SourceSafe信息|> > **默认值是**SAK|
后面的Scc**属性，标志了此工程在SourceSafe中。手动将一个工程从SourceSafe中删除时，只要删除这四个属性就行。当它们的默认值是SAK时，可以在工程文件的同一个目录下找到一个文件mssccprj.scc，这里面包含了工程在SourceSafe中的信息，比如：
SCC = This is a Source Code Control file
[PS.vcproj]
SCC_Aux_Path = "\\code-server\code$\"
SCC_Project_Name = "$/project/PS", IQIBAAAA
Platforms很简单，表示平台内容，通常就是“Win32”；
二、Configurations是编译和链接的配置信息；
Files下包括的是工程中的文件信息。以下主要谈谈Configurations和Files。
Configurations包含了工程编译和链接等配置信息，其子节点是Configuration，由用户设定的编译类型决定，默认有Debug和Release两个子节点。Configuration的属性如下：
|**属性**|**含义**|**说明**|
|----|----|----|
|Name|编译选项名称|Debug下通常为Debug|Win32|
|OutputDirectory|目标文件输出路径|默认为Debug|
|IntermediateDirectory|编译信息输出路径|默认为Debug|
|ConfigurationType|工程类型|1表示exe程序文件，2表示dll动态库文件，3表示lib静态库文件|
|UseOfMFC|表示是否使用MFC|0表示不使用MFC，1表示静态链接MFC，2表示动态链接MFC|
|CharacterSet|表示编码类型|1表示Unicode，2表示Ansi|
Configuration的子节点全是Tool，每个子节点都有一个属性Name表示节点含义。这里面有两个子节点比较有用，一个是VCCLCompilerTool，表示编译信息，一个是VCLinkerTool，表示链接信息，其它的子节点用的不多。
VCCLCompilerTool的常用属性如下：
|**属性**|**含义**|**说明**|
|----|----|----|
|Optimization|优化选项|可以为0、1、2、3、4|
|PreprocessorDefinitions|预定义标记|通常都是WIN32 _WINDOWS _DEBUG|
|MinimalRebuild|是否使用最小编译|设置为TRUE能节约编译时间|
|BasicRuntimeChecks|运行时检测，包括栈和未初始化变量等|默认为3|
|RuntimeLibrary|程序运行时|选择多（单）线程，（非）调试，DLL（EXE）类型|
|TreatWChar_tAsBuiltInType|是否将wchar_t当作内置类型|如果为FALSE，wchar_t被认为是unsigned short类型|
|ForceConformanceInForLoopScope|if和for循环中声明的变量的作用范围是否在循环内|7.1默认为FALSE，8.0默认为TRUE|
|UsePrecompiledHeader|预定义头文件设置|0表示不使用，1表示创建预定义头文件，2表示自动创建，3表示使用预定义头文件|
|PrecompiledHeaderThrough|预定义头文件名|通常都是stdafx.h，可以随意指定|
|PrecompiledHeaderFile|预编译信息文件名|默认为$(IntDir)/$(TargetName).pch|
|WarningLevel|警告级别|4|
|Detect64BitPortabilityProblems|检测是否兼容64位程序|FALSE|
|DebugInformationFormat|调试信息格式|Debug下通常设置为4，Release下可以设置为3|
VCLinkerTool的常用属性如下：
|**属性**|**含义**|**说明**|
|----|----|----|
|AdditionalDependencies|依赖lib文件||
|OutputFile|输出的目标文件|默认$(OutDir)/$(ProjectName).exe|
|LinkIncremental|增量编译/span>|2|
|AdditionalLibraryDirectories|依赖lib的位置||
|GenerateDebugInformation|是否生成调试信息|通常都为TRUE|
|ProgramDatabaseFile|调试信息文件名称|$(IntDir)/$(ProjectName).pdb|
|SubSystem|子系统|1为控制台，2为Windows程序|
|ImportLibrary|导入的lib文件|默认$(IntDir)/$(ProjectName).lib|
三、Files
Files下包括的是工程中的文件信息，由Filter和File组成，Filter表示目录，File表示文件。每个cpp还可以包含一个子节点FileConfiguration，这个子节点表示此cpp文件编译时(与全局编译选项不一致）（个人理解去掉)的内容，通常情况下除了预编译头文件外这不是必须的，预编译头则必须指定它的UsePrecompiledHeader信息为1，也就是由它来创建预定义头文件。
