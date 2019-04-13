
# 论Visual Studio和.NET Framework - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:46:27[Jlins](https://me.csdn.net/dyllove98)阅读数：1560


今天在工作的时候听到一席谈话感觉有点不可思议，微软真的是把开发人员惯的有点傻了，微软流水线式的产品让很多开发者认定了“唯一”，这当然也说明了微软的成功，不扯太多题外话，今天只是简单的谈一谈而不是深入的去研究，老鸟绕路哈。
对话内容如下：
X1:你封装的那个类库发给我一下。
X2：可以啊，我是VS2010开发的你可以用吗？
X1：你直接生成下给我不就好了。
X2：我很忙没空啊，我把项目给你。
X1:你的项目是VS2010开发的？
X2：是啊。
X1：那你不是要.net 4.5。
X2：不用啊，4.0就可以，Win7自带。X1:那使用起来客户端不是要装.net 4？
X1：不用啊，你把那用到的类文件拷到VS2008里面就可以了。
## .NET Frameowork和Visua Studio
Visual Studio最早于1995-04发布，而.net最早于2002-02-13发布，也就是说Visual Studio要早于.net。
其实这两个是完全不同的产品和定位，只不过微软把他们整合的太过于紧密及微软在Visual Studio上取得的巨大的成功，导致了.net开发工具的“唯一”性。
简单的来说，.net是编程平台而Visual Studio是一个用于快速编程的工具。
## 在高版本的Visual Studio中使用低版本的.NET Framework
这个应该是这篇博文的重点了。
其实在Visual Studio中开发不同版本的.NET Framework应用是很简单的一件事情。
**1.在创建项目时选择.NET Framework版本**
![QQ截图20130807123243](https://img-blog.csdn.net/20130807184618593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
VS会自动筛选出所选.net版本支持的项目模板，比如：WPF是3.5之后才推出的，那么选择.net 2.0 WPF应用程序是不可见的。
**2.更改现有项目的.NET Framework版本**
在项目属性-应用程序选项卡中可以看到目标框架的选项。
![2](https://img-blog.csdn.net/20130807184620500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 其他IDE
既然VS是开发.NET Framework应用的一个工具那么是否存在其他类似VS的工具呢？
答案是肯定有的，当然没有VS那么强大。
比如：SharpDevelop（安装包差不多15MB而且是开源的）
## 结尾
此篇文章为我的第一篇文章，文笔、润色等可能不太好，但继后我会坚持写博文，以提高自己的文笔及沟通能力，此篇文章主要针对于新手，新手大多数会这么理解，当初我也不例外。


