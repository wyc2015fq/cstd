# C# 系统应用之EM安全卫士总结及源码分享 - 杨秀璋的专栏 - CSDN博客





置顶2015年04月14日 00:55:03[Eastmount](https://me.csdn.net/Eastmount)阅读数：15912
所属专栏：[C# 系统应用知识](https://blog.csdn.net/column/details/eastmount-xtyy.html)








        本文主要是总结自己“C#系统应用系列”的一篇文章，讲述以前的毕设“个人电脑使用记录清除软件设计与实现”。希望对大家有所帮助，同时建议大家下载源代码，不论是界面还是注释及应用都是非常不错的C#学习程序。

        下载地址(免费资源)： 
[http://download.csdn.net/detail/eastmount/8591789](http://download.csdn.net/detail/eastmount/8591789)
[http://pan.baidu.com/s/1o93rS](http://pan.baidu.com/s/1o93rS)
如果文章中有不足之处，还请海涵！主要是分享一些思想和源代码供大家学习，同时该程序通过VS2012开发，其它较低版本打开可能会出现错误。如果觉得该部分内容比较冗余，可以直接学习源码，代码中有非常详细的注释信息。
PS:因为自己小名东山(Eastmountain)，所以CSDN叫Eastmount，故名EM安全卫士。哈哈，仿造着360的界面也是当时觉得比较好玩。


## 一. 项目结构及功能介绍

首先介绍该项目的结构如下图所示：

![](https://img-blog.csdn.net/20150413192145096)


        从图中可以看到主要由6个功能组成，其中对应的运行效果如下图所示：

![](https://img-blog.csdn.net/20150413193807380)

其功能主要分为六个部分，具体对应如下：清除上网痕迹模块(CleanIE.cs)、办公文档记录清除模块(CleanWord.cs)、文件简单删除\清空回收站模块(CleanRecycle.cs)、USB痕迹清除模块(CleanU.cs)、最近文件使用记录清除模块(CleanRecent.cs)和文件粉碎模块(CleanFile.cs)。这些功能都是根据毕设要求完成的。

        同时OpaqueCommand.cs和MyOpaqueLayer.cs是透明罩的效果，就是当选中某部分功能时，界面就会添加透明罩效果；而通过Pancel控件实现点击不同图标显示不同的功能界面的效果。
其中该部分你可以学到的知识包括：
[C# 系统应用之透明罩MyOpaqueLayer实现360界面阴影效果](http://blog.csdn.net/eastmount/article/details/20914999)
[C# 系统应用之使用Pancel控件同一窗体切换页面](http://blog.csdn.net/eastmount/article/details/21461275)
[C# 系统应用之无标题窗体移动的两种方法](http://blog.csdn.net/eastmount/article/details/20707363)
[C# 系统应用之窗体最小化至任务栏及常用操作](http://blog.csdn.net/eastmount/article/details/18604721)


## 二. 清除IE模块


        该模块的主要功能是获取和清除Internet Explorer（简称IE）浏览器的上网痕迹。当运行软件进入主界面后，点击“清除IE”的图标，就可以进入“上网痕迹清除模块”。用户可以实现3个功能：获取IE浏览器地址栏网址并以列表形式显示、获取IE浏览器最近访问网站历史、删除IE浏览器的上网痕迹。

![](https://img-blog.csdn.net/20150413200732227)

        如上图所示是获取IE浏览器的浏览历史记录，同时点击“获取地址栏网址”可以获取浏览器输入过的网址信息，选中左边的清楚IE可以实现清楚浏览器的痕迹信息，它是多选的清除。

![](https://img-blog.csdn.net/20150413201458016)

        其中CleanIE.cs的代码布局如下图所示：

![](https://img-blog.csdn.net/20150413201820146)

其中该部分你可以学到的知识包括：
[C# 系统应用之获取IE浏览记录和IE地址栏输入网址](http://blog.csdn.net/eastmount/article/details/22830903)
[C# 系统应用之清除Cookies、IE临时文件、历史记录](http://blog.csdn.net/eastmount/article/details/18821221)


## 三. 清除U盘记录模块

        进入该模块后，点击“检索信息”按钮，将在列表中按顺序依次显示最近使用USB移动存储介质的信息，信息包括：标号、USB名称、UID标记、路径信息、时间信息；点击“删除信息”按钮，将清除USB最近使用痕迹记录，并且在此模块中有温馨提示操作。


![](https://img-blog.csdn.net/20150413202509818)

        这部分也是一个技术难点，由于微软没有开源很多内容都需要自己去学习，同时该方面的资料也比较少。从图中可以看到U盘和移动硬盘的历史记录，它是从注册表中获取的，该图中有两条信息。

        其中No.2是西部数据的移动硬盘信息My Passport Ultra，如下图所示。

![](https://img-blog.csdn.net/20150413202952475)

        其中对应的CleanU.cs代码如下图所示：(由于担心删除注册表USB信息会破坏，所以该功能被注释掉了。详见代码和博客)

![](https://img-blog.csdn.net/20150413203356768)

其中该部分你可以学到的知识包括：
[C# 系统应用之通过注册表获取USB使用记录(一)](http://blog.csdn.net/eastmount/article/details/23136821)
[C# 系统应用之注册表使用详解](http://blog.csdn.net/eastmount/article/details/25428405)


## 四. 文件简单删除及清空回收站模块

        进入该模块后，点击“添加文件”按钮，会弹出一个“打开”对话框，用户可以根据自己的需要选择要删除的文件，选中的文件会添加至列表中；当点击“back”按钮后，会清空添加的所有文件，用户需要再次选择；当点击“删除文件”时，会删除列表中添加的文件；当点击“清空回收站”时，会清空每个磁盘分区中的回收站文件，并且此模块中将有温馨提示用户操作结果。


![](https://img-blog.csdn.net/20150413204204132)

        其中添加文件按钮如下图所示，同时能点击按钮能实现清空回收站的内容：

![](https://img-blog.csdn.net/20150413204711624)

        同样该部分CleanRecyle.cs代码中对应不同按钮有不同的实现函数，此处不再截图。
其中该部分你可以学到的知识包括：
[C# 系统应用之清空回收站操作](http://blog.csdn.net/eastmount/article/details/18414935)


## 五. 清除最近使用文件痕迹模块

        进入该模块时，当用户点击“查看最近浏览文件路径”按钮时，将在列表栏中显示Windows系统最近访问过的文档记录，并在“共获取/清除文件”栏中显示获取的数量；当点击“获取地址栏最近访问文件”按钮时，将在列表栏中显示Windows系统地址栏中最近访问文件的记录，并在“共获取/清除文件”栏中显示获取文件的数量；当点击“删除最近浏览文件信息”按钮时，将清空Windows系统最近访问的文件和Windows系统地址栏中记录最近访问文档的记录。


![](https://img-blog.csdn.net/20150413205703005)

        其中对应的获取最近浏览文件是WIndows系统中Recent路径下的快捷方式，如下图所示：

![](https://img-blog.csdn.net/20150413210050657)


        其中CleanRecent.cs代码如下图所示：
![](https://img-blog.csdn.net/20150413210314392)


PS:该部分存在一个bug，当你获取最近浏览文件时打开新的文件，它会自动添加并且显示一个文件多个连接，但是再次点击"获取"按钮后可以获取正确的超链接。
        其中该部分你可以学到的知识包括：
[C# 系统应用之获取Windows最近使用记录](http://blog.csdn.net/eastmount/article/details/18474655)


## 六. 清除Word痕迹模块


        进入该模块后，点击按钮“获取office最近使用文档”可以实现获取微软offcie办公软件最近使用文档记录，其中包括word、ppt、excel等类型文档，并以列表形式分块显示；点击“清除office最近使用文档”可以实现清除office最近访问文档的记录，并弹出对话框提示删除操作。

![](https://img-blog.csdn.net/20150413211320035)

        其中在获取存储办公文件时我是通过静态目录实现的，代码如下：

string dirPath = @"C:\Users\dell\AppData\Roaming\Microsoft\Office\Recent";

        因为我的电脑名是Dell，此处建议的方法是获取Environment计算机的名字实现。
![](https://img-blog.csdn.net/20150413211634065)





## 七. 粉碎文件模块

        文件粉碎模块主要是用户将指定的文件彻底粉碎，并确保该文件无法再恢复。主要采用了树形控件，这是我当时做得比较好的一种现实效果。如下图所示：

![](https://img-blog.csdn.net/20150414003313274)

        其中该部分你可以学到的知识包括：
[C# 系统应用之TreeView控件 (一).显示树状磁盘文件目录及加载图标](http://blog.csdn.net/eastmount/article/details/19453107)
[C# 系统应用之ListView控件 (二).加载选中节点文件夹下文件信息](http://blog.csdn.net/eastmount/article/details/21241313)
[C# 系统应用之ListView控件 (三).添加ContextMenuStrip右键菜单打开删除文件](http://blog.csdn.net/eastmount/article/details/27264295)
[C# 系统应用之调用SDelete程序粉碎文件及基础原理知识](http://blog.csdn.net/eastmount/article/details/23792823)

最后希望文章对大家有所帮助。最近自己太颓废了，还是找些事情做吧！一直没有出去实习，也应该去学习下真正的应用知识了；一方面要完成关于维基百科的语义分析毕设，一方面看能不能做个智能的一些python分析图片、搜索引擎等东西吧！沉下心来做些实事吧~
（By:Eastmount 2015-4-14 夜1点   [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）





























