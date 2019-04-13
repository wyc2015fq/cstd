
# Source Insight上手教程 - Magician的博客 - CSDN博客


2017年09月05日 19:40:49[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：1456


[目录](#目录)[Project的建立和工作区域](#project的建立和工作区域)
[同步](#同步)
[查看定义](#查看定义)
[查找引用](#查找引用)
[查找调用](#查找调用)
[Source Insight常用快捷键](#source-insight常用快捷键)


# 目录
最近刚参加工作，第一个任务就是查看项目的源码，熟悉代码结构。于是乎就简单学习了Source Insight。在这里就转载别人的文章当做自己的笔记，便于自己以后查找。
为什么要用Source Insight呢？貌似是因为比完整的IDE要更快一些，比较利于查看大量的代码。
软件的安装很简单，设置好安装目录。
配置好文档路径，当然这个也可以在Options里面改，选Options->Preferences…里面的Folders标签就行。
## Project的建立和工作区域
在菜单中选择Project->New Project…可以建立新的工程，自定义工程名，选择保存路径，然后新建。
新建确定之后，在接下来的菜单中，需要选择Project Source Directory: the main location of your source files. 比如我可以选最新的Android API 19的源码目录。
选定之后会出现Add and Remove Project Files的对话框，这时候可以选择刚才目录中的文件，然后添加进来。
Load完成之后close对话框就可以了。可以加不同路径的代码进来。
完成之后，Project Window就出现在右边了，点击其中任意一个文件，内容就显示在中央编辑区域了。左边是Symbol Window，可以快速定位函数、变量定义等。下面是Context Window，显示有用的上下文相关信息。可以通过View菜单调出或者关闭各种窗口。比如Relative Window可以显示出方法中调用的方法。
![这里写图片描述](https://img-blog.csdn.net/20170905193444369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 同步
在编辑区域中，键盘按下Ctrl，用鼠标单击变量或方法时，会跳到变量或方法的定义。
如果某个变量或者方法是黑色字体表示的，说明它还没有被定义。
当然有时候也不是因为没定义，需要同步一下，Project->Synchronizing Files。
## 查看定义
可以选中一个变量，然后右键，Jump to Definition，跳转到定义。
当然，上面提过，也可以直接Ctrl+鼠标点击。
在编辑区域内选中变量或者方法，过一两秒，Context Window会自动显示定义。
## 查找引用
选择一个变量或方法后右键Lookup References…就可以进行查找。
查找结果显示在一个框里，点击每条查找结果前面的红色标记(两个红色的相背的三角形)，可以该条结果对应代码文件，进入代码编辑状态；再点击代码中的查找对应行开头的红色标记可以转回查找结果页面。
工具栏上有相应的图标进行上一条和下一条的跳转。
Lookup References弹出的对话框里可以进行查询的选项配置。比如是否大小写敏感，是不查找注释还是只在注释里查找。
另外Search Method也可以选是字符串、正则、关键字、还是引用类型。
## 查找调用
这个是针对方法的，选中方法名，然后右键Jump to Caller，如果只有一次调用，则直接跳转，如果多次调用，则显示一个列表框，选择一项就会跳转过去。
文件中的查找可以通过菜单Search。
## Source Insight常用快捷键
1.Ctrl + 鼠标单击 进入定义.
2.Alt + F12可以切换，让字符宽度变得一致，或者是大小不同地显示。
3.Shift + F8 标亮文本中光标所在的单词。
4.Ctrl + G (或者F5) 跳转到某一行。
5.Ctrl + O 搜索文件，找到回车打开，找不到ESC退出
6.Alt + G (或者F7) 打开Symbol Window.
7.Alt +, 后退；Alt+.前进.
8.Ctrl + F 查找关键字。

