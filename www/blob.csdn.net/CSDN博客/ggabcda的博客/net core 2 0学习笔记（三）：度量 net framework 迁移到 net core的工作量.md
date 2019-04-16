# .net core 2.0学习笔记（三）：度量.net framework 迁移到.net core的工作量 - ggabcda的博客 - CSDN博客





2017年08月22日 11:36:11[ggabcda](https://me.csdn.net/ggabcda)阅读数：218








  把现有的.net framework程序迁移到.net core上，是一个非常复杂的工作，特别是一些API在两个平台上还不能同时支持。两个类库的差异性,通过人工很难识别全。好在微软的工程师们考虑到了我们顾虑，为我们提前设计了一个工具：.NET Portability Analyzer。通过此工具我们可以快速的确定.net framework 程序用到的API与.net core的匹配度，并给出详细的差异化报表，美中不足的时，没有给出详细的升级建议。在升级的过程中，如果要详细的了解API的差异性和详情，可以参考：https://docs.microsoft.com/en-us/dotnet/api/?view=netcore-2.0。

     下面说一下此工具的安装方法和用法。

     首先，打开VS2017 > 工具 >扩展域更新，在联机目录下，搜索：.NET Portability Analyzer，安装即可。

![Image(37)](http://images2017.cnblogs.com/blog/850196/201708/850196-20170821083015918-228569887.png)

     安装完后，重启VS，打开一个.net framework 工程。在工程或者解决方案的右键菜单中，选择：Portability Analyzer Settings。通过配置页面，设置要分析的.net core 版本即可。另外，还可以设置分析报告的展现方式。

![Image(38)](http://images2017.cnblogs.com/blog/850196/201708/850196-20170821083016730-754026654.png)

![Image(39)](http://images2017.cnblogs.com/blog/850196/201708/850196-20170821083017589-2010973975.png)

     设置完成后，在VS的工程或者解决方案右键菜单中，选择：Analyze Assembly Portability，然后静等结果即可。下面是一个分析结果的样例。第一个Sheet页中，展现了当期程序使用到API与目标平台的匹配度。第二个Sheet页面详细的说明了不支持的API信息。怎么样？非常使用的一个工具！

![Image(40)](http://images2017.cnblogs.com/blog/850196/201708/850196-20170821083018308-403450354.png)

![Image(41)](http://images2017.cnblogs.com/blog/850196/201708/850196-20170821083019480-1205255402.png)



