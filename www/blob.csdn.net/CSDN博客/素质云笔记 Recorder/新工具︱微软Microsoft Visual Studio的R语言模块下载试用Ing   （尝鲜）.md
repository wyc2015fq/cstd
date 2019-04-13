
# 新工具︱微软Microsoft Visual Studio的R语言模块下载试用Ing...（尝鲜） - 素质云笔记-Recorder... - CSDN博客

2016年03月16日 20:07:48[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4760



**笔者：**前几天看到了以下的图片，着实一惊。作为R语言入门小菜鸟，还是觉得很好看，于是花了一点时间下载下来试用了一下，觉得还是挺高大上的。
就是英文不好是硬伤。下面贴给小白，我当时的下载步骤与遇见的问题。
大神请绕道。
欢迎会用的人，能够指导一下俺，俺的确不太熟悉这个界面与应用方式。

![](https://img-blog.csdn.net/20160316194232727)

官方解释：
Visual Stuido R工具预览版主要特性：
•编辑器：为R脚本和功能带来完整的编辑体验，包括分离/合并窗口、语法高亮等；
•智能感知（即自动补全）：在编辑器和交互R窗口中均可用；
•R交互窗口：在Visual Studio中直接与R控制台协同工作；
• 历史窗口：查看、搜索、选择历史指令，并发送到交互窗口中；
•变量资源管理器：深入R数据结构，检查变量值；
• 绘图：在Visual Studio工具窗口中查看所有R绘图；
•调试：断点、单步调试、监视窗口、堆栈调用等；
•R Markdown：支持R Markdown/knitr导出到Word和HTML；
•Git：支持Git和GitHub源代码版本控制；
除此之外，还有其他一些开发者要求的新功能将在后续的更新中带来。


下载流程：
1、打开：[https://www.visualstudio.com/en-us/features/rtvs-vs.aspx](https://www.visualstudio.com/en-us/features/rtvs-vs.aspx)
![](https://img-blog.csdn.net/20160316195001768)
主要就是按照图中的step1、step2、step3来进行操作。

**2、第一步-step 1**
点击“Dowload Community 2015”，
点击后会自动弹出下载页面，下载至自己想要的工作目录之中。
![](https://img-blog.csdn.net/20160316195232849)
这个会安装稍微久一些，估计在30min的样子（笔者电脑i7-6700,16G内存）。

**3、点击主界面的“step 2:download R tools for VS”**
会自动弹出下载，然后傻瓜式安装即可。
速度较快。

**4、第三步，点击“step 3 Download Microsoft R Open”**
会进入如下界面：
不同的电脑系统，需要选择不同的“Platform”，我选择了windows。
硬性需要下载“Microsoft R Open”,
模型库也最好也下载了“MKL**”.
![](https://img-blog.csdn.net/20160316195902075)


差不多已经该下载的东西，都已经下载好了。下面来看一下桌面有啥：
![](https://img-blog.csdn.net/20160316200217945)
这个是普通的Rgui界面，并不是刚开始看到的界面。
![](https://img-blog.csdn.net/20160316200330196)
实际**应该打开“Visual Studio 2015”**才是最后要打开的。（笔者是win10界面）

最后，打开，如果，
可以看到这个界面的上面有一个“R Tool”
那就大功告成了。
![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)

————————————————————分割线————————————————————————
**汉化方法：**

登陆网页：
https://www.microsoft.com/en-us/download/details.aspx?id=48157

下载简体中文汉化包，然后加载

最后在tools-option-选择最左边的环境（environment）下的国际设置（international setting）

设置即可

————————————————————分割线————————————————————————
**Microsoft Visual Studio 2015 试用期已过**

过了一段时间发现studio试用期已过，登陆微软账号也不行。
于是乎，操作了以下两步又可以了：
1、修复。从控制面板那里，重新“修复”了一下；
2、登陆微软账号又可以重新登陆了。


