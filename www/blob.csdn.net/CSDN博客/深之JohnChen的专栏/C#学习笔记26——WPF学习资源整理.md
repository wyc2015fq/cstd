# C#学习笔记26——WPF学习资源整理 - 深之JohnChen的专栏 - CSDN博客

2011年09月03日 23:29:18[byxdaz](https://me.csdn.net/byxdaz)阅读数：2992


WPF（WindowsPresentation Foundation）是微软推出的基于Windows Vista的用户界面框架，属于.NET Framework 3.0的一部分。它提供了统一的编程模型、语言和框架，真正做到了分离界面设计人员与开发人员的工作；同时它提供了全新的多媒体交互用户图形界面。

在WPF的框架下，可以使用C#和xmal语言来设计界面。两种设计方法各自的好处，xmal语言是一种标记性语言，使用比较简单和灵活。C#下设计和一般的设计差别不大，但是要注意WPF下的类和接口等。具体的可以参考相关的教程。WPF浏览器应用程序不同于Silverlight。Silverlight定义了一个WPF子集它不需要把.NET Framework安装在客户机系统中，但需要把一个插件安装在浏览器上，且支持不同的浏览器和不同的操作系统。

WPF学习资源整理：

首推刘铁猛的《深入浅出WPF》系列博文

1.深入浅出WPF（1）——什么是WPF
[http://blog.csdn.net/FantasiaX/archive/2008/05/15/2449387.aspx](http://blog.csdn.net/FantasiaX/archive/2008/05/15/2449387.aspx)

2.深入浅出WPF——什么是XAML
[http://blog.csdn.net/FantasiaX/archive/2008/12/25/3599807.aspx](http://blog.csdn.net/FantasiaX/archive/2008/12/25/3599807.aspx)

3.深入浅出WPF——XAML的优点
[http://blog.csdn.net/FantasiaX/archive/2008/12/25/3607897.aspx](http://blog.csdn.net/FantasiaX/archive/2008/12/25/3607897.aspx)

4.杂七杂八——关于XAML与逻辑代码 
[http://blog.csdn.net/FantasiaX/archive/2008/12/27/3621029.aspx](http://blog.csdn.net/FantasiaX/archive/2008/12/27/3621029.aspx)

5.杂七杂八——Name与x:Name的关系
[http://blog.csdn.net/FantasiaX/archive/2008/12/11/3499767.aspx](http://blog.csdn.net/FantasiaX/archive/2008/12/11/3499767.aspx)

6.深入浅出WPF（7）——数据的绿色通道，Binding（上）
[http://blog.csdn.net/FantasiaX/archive/2008/06/23/2577239.aspx](http://blog.csdn.net/FantasiaX/archive/2008/06/23/2577239.aspx)

7.深入浅出WPF（8）——数据的绿色通道，Binding（中）
[http://blog.csdn.net/FantasiaX/archive/2008/06/27/2590458.aspx](http://blog.csdn.net/FantasiaX/archive/2008/06/27/2590458.aspx)

8.深入浅出WPF（9）——数据的绿色通道，Binding（下）

[http://blog.csdn.net/FantasiaX/archive/2008/07/31/2749292.aspx](http://blog.csdn.net/FantasiaX/archive/2008/07/31/2749292.aspx)

9.深入浅出WPF（10）——“脚踩N条船”的多路Binding
[http://blog.csdn.net/FantasiaX/archive/2008/08/07/2779433.aspx](http://blog.csdn.net/FantasiaX/archive/2008/08/07/2779433.aspx)

10.WPF实例秀——不用属性也Binding
[http://blog.csdn.net/FantasiaX/archive/2008/12/16/3525985.aspx](http://blog.csdn.net/FantasiaX/archive/2008/12/16/3525985.aspx)

11.深入浅出WPF——附加事件（Attached Event）
[http://blog.csdn.net/fantasiax/archive/2009/09/21/4575968.aspx](http://blog.csdn.net/fantasiax/archive/2009/09/21/4575968.aspx)

《深入浅出WPF》系列的相关视频

12.《深入浅出WPF》系列视频（1）——剖析最简单的WPF程序
[http://www.cnblogs.com/prism/articles/1781026.html](http://www.cnblogs.com/prism/articles/1781026.html)

13.《深入浅出WPF》系列视频（2）——WPF用户界面的树形结构

[http://www.cnblogs.com/prism/archive/2010/07/22/1782661.html](http://www.cnblogs.com/prism/archive/2010/07/22/1782661.html)

14.《深入浅出WPF》系列视频（3）——XAML中使用Attribut=Value的形式赋值和TypeConverter
[http://www.cnblogs.com/prism/archive/2010/07/23/1783408.html](http://www.cnblogs.com/prism/archive/2010/07/23/1783408.html)

15.《深入浅出WPF》系列视频（4）——属性标签与标签扩展 
[http://www.cnblogs.com/prism/archive/2010/07/26/1785086.html](http://www.cnblogs.com/prism/archive/2010/07/26/1785086.html)

WPF界面布局

16.WPF中的控件布局
[http://www.cnblogs.com/zhouyinhui/archive/2007/03/18/678892.html](http://www.cnblogs.com/zhouyinhui/archive/2007/03/18/678892.html)

MSDN杂志中WPF数据绑定博文（这两篇堪称经典！！）

17.WPF中的数据绑定
[http://msdn.microsoft.com/zh-cn/magazine/cc163299.aspx](http://msdn.microsoft.com/zh-cn/magazine/cc163299.aspx)

18.使用数据绑定和 WPF 自定义数据显示

[http://msdn.microsoft.com/zh-cn/magazine/cc700358.aspx](http://msdn.microsoft.com/zh-cn/magazine/cc700358.aspx)

其它内容
19.WPF中的事件（Event)
[http://www.cnblogs.com/jyz/archive/2008/11/24/1339911.html](http://www.cnblogs.com/jyz/archive/2008/11/24/1339911.html)

20.WPF中的命令与命令绑定
[http://www.qqread.com/dotnet/j394132.html](http://www.qqread.com/dotnet/j394132.html)

------------------------------2011年4月24日更新---------------------------------------------------

21.WPF and Silverlight 学习笔记：索引页（作者：龙腾于海）

[http://www.cnblogs.com/DragonInSea/archive/2009/04/17/1437927.html](http://www.cnblogs.com/DragonInSea/archive/2009/04/17/1437927.html)

22.WPF 基础到企业应用系列索引（作者：圣殿骑士）

[http://www.cnblogs.com/knightswarrior/archive/2010/07/09/1774141.html](http://www.cnblogs.com/knightswarrior/archive/2010/07/09/1774141.html)

