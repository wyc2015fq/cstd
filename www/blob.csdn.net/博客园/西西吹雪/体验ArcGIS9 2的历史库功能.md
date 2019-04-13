
# 体验ArcGIS9.2的历史库功能 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[体验ArcGIS9.2的历史库功能](https://www.cnblogs.com/watsonyin/archive/2007/02/09/646066.html)
ESRI公司于2006年11月9日全球同步发布了历史上重要的软件版本ArcGIS9.2，在该版本中，主要新增了以下四大功能（ESRI田昌莲）：第一大新功能：非版本编辑功能。多用户编辑可以在没有版本的情况下进行；第二大新功能：版本数据复制功能。为分布式数据同步复制提供了便利；第三大新功能：Oracle空间的SQL功能。对Oracle实现的geodatabase的空间sql接口完全支持；第四大新功能：增强的Geodatabase。包括以下几点：
（1）历史数据库功能
（2）对大量的表面数据集有了很在的改变和提高
（3）File-Based Geodatabase，突破了Access文件大小2G的限制，相当于增强的Personal Geodatabase。
上面这些新功能，说实话我都很喜欢，有时间真要一个个都体验一下啊。废话少说，我们现在就开始验一下ArcGIS9.2带来的历史库功能吧。
1、先装ArcGIS9.2 Desktop。
注意，要安装9.2，必须先全部卸载掉以前版本的所有软件，包括Desktop，ArcGIS Engine，ArcGIS Server等。
2、安装SDE9.2。
之前的版本的SDE是不支持历史库功能的。
如果你有安装包，没有授权文件，请到这里:[http://www.cnblogs.com/watsonyin/archive/2007/02/09/646031.html](http://www.cnblogs.com/watsonyin/archive/2007/02/09/646031.html)
3、准备数据。这里我导入一个很小的宗地层，用于测试，图层名称是sde.sde.zd。
4、注册版本。打开ArcCatalog，新建一个到默认版本的连接，然后选择该图层，点击右键，在弹出菜单中点击Register As Versioned，这一点与9.1版本中一样的，只有注册版本，才能进行下一步创“建历史库”
5、创建历史库。
在ArcCatalog中选择该图层，点击右键，发现其中比9.1多了一项Archiving，包含两个子菜单，Enable Archiving和Disable Archiving。点击Enable Archiving，这时系统会自动为该图层创建一个对应的历史图层，历史图层的名称为SDE.SDE.ZD_H。我们在ArcCatalog中并不能看到该图层，这是因为它作了过滤的缘故。如果用ArcGIS9.1软件其它机器连接该数据，可以看到该图层其实与普通图层没什么两样。
创建好的历史图层的数据是原图层的拷贝，但可以看到多了三个字段：GDB_FROM_DATE，GDB_TO_DATE，GDB_ARCHIVE_OID。看到这三个字段，马上就明白了其大致思路，前面两个字段际是指明了图形对象的存活时间，GDB_FROM_DATE即图形的创建时间，GDB_TO_DATE是图形的删除或修改时间。这与我以前用MO做历史库的思路如出一辙啊。图形的操作可分为增、删、改三种类型，修改又可以看成是先删除再新增一个图形对象，所以所有的操作又可归为增、删两种操作，这就是那两个字段的由来。至于GDB_ARCHIVE_OID，就不有解释了吧。
6、启用历史编辑选项
在ArcCatalog中，点击Tools菜单下的Options，在“选项”窗口中，切换到Geoprocessing页，选中Log geoprocessing operations to as historical model，如下图所示，这样我们在编辑图形时，编辑过程中产生的历史数据会自动记录到历史图层中。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9201.jpg)
7、编辑图形
下面我们可以进行编辑测试了。打开ArcMap，选择图层，断断续续的作一些编辑操作，以方便后面的体验。
8、新建一个到历史数据库的连接
在ArcCatalog或ArcMap的选择数据对话框中都可以完成。点击Add Spatail Database Connection，在新建连接窗口中点击Change按钮，会弹出如下窗口：![](https://images.cnblogs.com/cnblogs_com/watsonyin/9201-2.jpg)
我们选择The connection refers to a historical version，并选择是连接到历史标记还是指定一个时间点。
9、历史数据浏览
这是我们体验过程的关键方旅~
连接建好了之后，在ArcMap我们用该连接打开数据sde.sde.zd。在ArcMap工具栏上点击右键，点击Geodabase History，会出现历史库工具条，如图![](https://images.cnblogs.com/cnblogs_com/watsonyin/9208.jpg)
该工具条有三个按钮，分别是：
（1）Geodatabase History Viewer：即历史数据浏览工具。
（2）Add Historical Archive：即添加历史图层到地图上。
（3）Historical Marker Manager：即历史标记管理器。
下面分别介绍这三个工具：
9.1 Add Historical Archive
先在TOC中选中一个图层，再点击该工具按钮，会将选中图层对应的历史图层添加到地图窗口中。可以看到，历史图层包括了编辑过程中产生的所有数据。查看属性，可以看到它的数据源为sde.SDE.ZD_H，还可以到字段列表中查看新增的字段情况。这人工具的作用并不是很大，因为我们浏览历史数据，并不需要添加历史图层，直接对现状图层进行操作就可以了。当然，通过编程扩展，历史图层可以发挥很大的作用，例如，可以通过属性过滤，查看某个历史时段，而不是某个时点内的数据情况。
9.2 Historical Marker Manager
所谓历史标记，其实就是对某一历史时点的文字描述，这样比单纯的看历史时间，更加容易理解，从而更加方便历史数据的浏览。在例子里，我创建了三个历史标签，分别是“初始状态”，“新建AB”，“删除A移动B”，这样就会一目了然。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9207.jpg)

9.3 Geodatabase History Viewer
点击它，会出现以下界面。如果你是用是历史连接添加数据，这个界面会处于不可用的状态。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9205.jpg)
在Historical Date and Time中，可以选择一个历史标记，也可以选择一个具体的时间，点击Apply按钮，就可以查看该历史时点的数据了。其它的我想不用解释，看一下在不同历史时点的图形状况吧。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9202.jpg)
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/9202.jpg)（时间：16:20:09 初始状态）
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9203.jpg)
（时间：16:22:50 创建图形A、B）
![](https://images.cnblogs.com/cnblogs_com/watsonyin/9204.jpg)
（时间：16:26:06 删除图形A、移动B）
通过这种方式，就可以轻松浏览任一历史时间的数据了。通过编程，我们还可以进行扩展，例如不同历史时刻的数据对比浏览等。


终于在放假之前完成了这篇随笔，否则，我可能不会写它了，就像之前一直想写多线程的文章一样，到现在也没写呀。祝大家开开心心过个好年~





posted on2007-02-09 17:36[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=646066)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
[
](https://images.cnblogs.com/cnblogs_com/watsonyin/9204.jpg)