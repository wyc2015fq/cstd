# 用NuGet安装NewtonSoft.json - 左直拳的马桶_日用桶 - CSDN博客
2014年08月19日 21:25:54[左直拳](https://me.csdn.net/leftfist)阅读数：22739
因为要在C#里读取JSON字符串，资料查来查去，发现只能用第三方的NewtonSoft.json。本来.net也有自带的类库可以处理json，但TM的不停要你将JSON读进类对象里面。我靠，我只不过想将一个json字符串里的东西读出来而已，而里面的内容事先完全不知，我建立哪根毛线的类对象啊，脑子真有毛病。
但想下载newtonsoft.json的时候，却不见传统的ZIP包，只有一个命令行：
`pm> install-package newtonsoft.json`
这就是NuGet。
之前我一直搞不懂NuGet是干嘛的。现在有点明白了，就是一个管理.NET的安装、更新程序集的工具，类似于android的sdk.exe，node.js的npm。
好吧，潮流兴复古，现在流行命令行了，酷！
切克闹，现在用NuGet安装Newtonsoft.json，要如何进行呢？
1、打开vs2012的工具 - NuGet程序包管理器 - 程序包管理器控制台，这时在VS的底部窗口出现命令行：pm>。注意并不是黑色的DOS窗口，我这里是灰不溜秋的窗口，不注意看很难发现
2、输入命令行：
`pm> install-package newtonsoft.json`
回车，好像没什么反应。但其实过了一会，提示newtonsoft.json已经安装成功。
问题是，所谓的安装成功，是指将DLL引用设置到了相关项目。我的解决方案有好多项目，它给我装到了一个不需引用newtonsoft.json的项目里。
按照提示，查帮助
`pm>get-help`并无发现指定项目的。后来想碰碰运气，这样写：
`PM> install-package newtonsoft.json wcf`果然，将newtonsoft.json装到了项目wcf里，成功了。
nuget的确比较好用，下载、安装、配置，都帮你完成了，还不用指定下载地址，它自己就会去找，方便得很。但是，从这个帮助的信息残缺不全来看，又搞得很不专业，不知道脑子出了什么毛病。
NueGet为何会知道到哪里下载相关软件呢？估计这些第三方软件会有个到微软什么地方登记的过程。订阅者模式。
-----------------------------------------------------------
卸载的话，是这样：
uninstall-package newtonsoft.json 项目名
