# Visual Studio 11预览： 新的编程语言功能 - 文章 - 伯乐在线
每个版本的Visual Studio开发环境通常都会在其IDE及其支持的编程语言中引入一些新功能。微软开发部副总裁S. Somasegar最近[展示](http://blogs.msdn.com/b/somasegar/archive/2011/09/30/visual-studio-11-programming-language-advances.aspx)了一些即将出现在Visual Studio 11中的新的编程语言功能。
***JavaScript***
虽然在Visual Studio的早期版本中已包含对于JavaScript的支持，但是Visual Studio 11打算进一步增强这种支持，即给予该语言全面认可，就像给予同类的顶级Visual Studio语言（C#、Visual Basic、以及C++）的待遇一样。针对JavaScript预期的改进包括：
- 支持ECMAScript 5语言标准
- 智能感知（IntelliSense）得到“显著改善”，对于更大的文件会具有更高的精确度和更好的性能
- 对JavaScript调试的支持现在可与为其他Visual Studio语言所提供的调试功能相匹敌，包括若干支持可与被调试应用程序进行交互的调试窗口
- DOM资源管理器（DOM Explorer）使得开发者可以检查并操作HTML页面中的文档对象模型（Document Object Model）
或许最有趣的新增功能莫过于REPL（Read-Evaluate-Print Loop，即“读取——求值——打印”循环）。此功能为开发者提供了一种既可编写代码又可与运行中的应用程序进行互动的交互环境。
***Visual Basic / C#***
异步编程支持将被集成到C# 5和Visual Basic 11中，这些语言版本已引入Visual Studio 11。而在Visual Studio的早期版本中使用异步代码对于开发者的实现和调试而言在逻辑上都是很困难的。通过提供新的语言功能，使得Visual Studio 11在如下两方面都得以增强：一、允许对异步代码使用传统控制流（循环/条件/以及其他）；二、提供了可在集成调试器中调试这些语句的能力。
***C++***
计划在Visual Studio 11中纳入几个有助于C++开发的显著改善。这些改善包括：增强IDE支持（例如，引用突出显示和语义着色）、以及构建本机Windows Metro风格应用程序的能力。并且更加注重对于使用C++ AMP（Accelerated Massive Parallelism，即加速大规模并行处理）技术编写并行代码的支持。C++ AMP技术是在2011年6月AMD Fusion开发者峰会上[宣布的](http://www.infoq.com/cn/news/2011/06/CPP-AMP-Massive-Parallelism)。Visual Studio 11将提供使用C++ AMP直接调试应用程序的能力。
通过C++编译器可进一步增强并行代码开发，C++编译器将有能力检测出适合速度提升的代码，进而实现自动向量化（auto-vectorization）和自动并行化（auto-parallelization）。
***F#***
Visual Studio 11将引入F# 3.0和[Type Provider](http://msdn.microsoft.com/en-us/library/hh156509%28v=VS.110%29.aspx)功能。此机制是在响应将静态类型语言（F#）桥接至网络上找到的日益增长的动态数据的需要过程中逐渐形成的。类型提供程序（Type providers）简化了对不同数据访问库的引入，而无需采用代码生成器。F# 3.0将支持OData[1]、WSDL[2]、以及SQL三类数据开箱即用，不过自定义类型提供程序还需自行编写。由于F#是.NET Framework一部分，因此对于类型提供程序的支持并不仅限于F#。所有托管语言，包括C#和Visual Basic，将都能使用它们。
**译注**
[1] **OData**，Open Data Protocol，即开放数据协议，是用来查询和更新数据的一种Web协议，其提供了把存在于应用程序中的数据暴露出来的方式。关于OData的详细内容，请参阅[其官网](http://www.odata.org/)。
[2] **WSDL**，Web Services Description Language，即Web服务描述语言，是为描述Web服务发布的XML格式。关于WSDL的详细内容，请参阅[维基百科](http://zh.wikipedia.org/wiki/WSDL)。
**查看英文原文：**[Preview of Visual Studio 11: New Programming Language Features](http://www.infoq.com/news/2011/11/vs11_programming_features)
