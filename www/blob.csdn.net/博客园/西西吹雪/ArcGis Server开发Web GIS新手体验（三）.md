
# ArcGis Server开发Web GIS新手体验（三）  - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGis Server开发Web GIS新手体验（三） ](https://www.cnblogs.com/watsonyin/archive/2006/01/16/318470.html)
接（二）
在（二）中我们分析了HTML代码，也就基本弄清了.net adf怎样封装客户端的交互，以及客户端怎样与服务器端进行交互。下面我们再看一看服务器端的开发方式是怎样的，实际是就是进入了比较简单的实战。用其它工作开发Web GIS的朋友如果能够了解一下AGS的实现，一定会有很的大启发，你会在其中学到一些封装客户端、服务器端以及两者交互的技巧，怎样做到既封装一些常用的功能，又不妨碍我们进行功能的扩充。
好了，废话少说，开始我们新的体验吧。
在（一）中我们已经说了，.net adf提供了一系列的模板，这些模板又根据不同的需要，封装了一些常用的功能。我们的开发一般是从这些模板开始的。如果我们对这些模板生成的程序已经很熟了，当然也可以不需要这些模板，但如果你是新手，请从模板开始，这样可以避免走很多弯路。
首先，我们启动Visual Studio 2003，新建一个项目，在新建对话框中，在“项目类型”中可以看到ArcGIS Server Projects这一项，选择语言Visual C\#，在右边的模板列表中可以看到有七种模板可供选择，每种模板可用于新建不同用途的应用，如下图所示：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/ags_03_01.jpg)
我们先新建一个Map Viewer Web Application，也就是地图浏览程序。我们待会通过分析可以看到这个地图浏览程序是怎样实现的，以及怎样扩充自己的功能。
新建了一个Map Viewer Web Application之后，界面如下图所示。可以看到，.net为我们新建了一个asp.net　web工程。工程包括三个页面，default.aspx,errorpage.aspx和identify.aspx。其中，default.aspx是主界面，errorpage.aspx用于出错信息的显示，identify用于点击查询操作的结果的显示。下面我们主要分析default.aspx这个网页。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/ags_03_02.jpg)
这个网页的的上部分是一个banner，不用管。下部分由七个asp.net组件构成。这七个组件分别是：
１、Map组件：地图组件。有两个关键属性：Host和ServerObject，不用我说，也知道这是什么意思。
２、Toc组件：图层控件组件(table of contents)。有一个关键属性BuddyControl，即与哪个Map组件相关联。
３、Toolbar组件：工具条组件。有一个关键属性BuddyControls，表示可以绑定多个Map组件或PageLayout组件。
４、OverviewMap组件：导航图组件。有三个关键属性Host，ServerObject和BuddyControl。
５、Impersonation组件：身份验证组件。有一个关键属性identity，用于设计怎么登录到服务器，拥有操作的权限。
６、NorthArrow组件：指北针
７、ScaleBar组件：比例尺组件，有一个关键属性BuddyControl。BarFont属性好像有Bug，怎么设置也不起效，字体总是很小，不知为什么，哪位朋友也帮俺一个忙，看看怎么回事？
这七个组件中，显然Map组件最为重要，因为它负责地图显示并与用户交互。我们看一下Map组件有的事件，只有MapChanged的事件包含代码，仔细看一下，是与视图（地图范围）保存相关的代码。那Map组件与用户交互以及服务器端的处理代码，如缩放地图，在哪里呢？
再看一看Toolbar组件，共包含七个按钮，分别是放大，缩小，平移，全图，上一视图，下一视图和点选。点开它的ToolbarItems属性看一下，原来工具条按钮的定义都在这里面。
工具条按钮有两种类型：Tool和Command
Tool：工具按钮。表示点击后并不立即执行某项功能，而是需要用户与地图进行交互再执行。所以需要保存它的状态。并且同时需要有相应的客户端代码（javascript)和服务器端代码。放大、缩小、平移和点选属于这种类型。
Command：命令按钮。立即执行某项操作，不需要有户交互，所以不需要何存状态。全图、上一视图、下一视图属于这种类型。
我们在ToolbarItems的定制界面中，选中identify，看一下它的属性，包括五部份：
１、Appearance Images组：定义按钮显示的图片。这些设置会转化成HTML代码。
２、Appearance Text组：有两个子项。Text表示在按钮中显示的文字，也就是说按钮可以由图片和文字两部份组成。ToolTip，即鼠标放在按钮上面时显示的提示信息。
３、Client-side Action组：这组只有一个属性，就是ClientToolAction，这里就是与客户端交互的javascript代码了。内容是ID()。我们打开defaut.aspx，查看HTML，可以看到ID函数的内容。这里交互比较简单的情况，交互比较复杂，如放大时需要拉框，怎么封装？原来，Cient-side Action实际上只是javascript中onmousedown的执行函数，拉框所需的onmousemove和omnouseup去哪里找？其实只是一个小技巧而已，在onmousedown中动态地给onmousemove和onmouseup赋值不就可以了吗？事实上，.net adf封装zoomin等操作就是这要样做的。
Client-side Action属性有一个下拉选项，包含的是DragImage，DragRectangle,Point,Line,PolyLine,Polygon,Circle,Oval,none,Custom。不用解释，相信大家也明白了，是Toolbar组件封装了各种不同类型的客户端操作。这些与客户端交互的javascript在哪里呢？（二）中已经介绍了，这里就省略啦。
４、General组。包括Disabled,Name,ShowLoading三个属性。
５、Server-side Action组：有两个属性，ServerToolActionAssembly和ServerToolActionClass，用于定义服务器端要执行的功能。在.net adf中，每个被封闭的服务端操作，都是由一个类来实现的，ServerToolActionAssembly表示这个类在哪个程序集中，ServerToolActionClass就是这个类的名称。我们在ServerToolActionAssembly中选择ESRI.ArcGIS.Server.WebControls，可以清楚的看到该程序集中定义了四个这种类：MapCenterAt,MapPan,MapZoomin,MapZoomout。事件上，我们也可以定义自己的处理类，只要实现ESRI.ArcGIS.Server.WebControls.Tools.IMapServerToolAction接口就可以了。
关于扩展：
１、客户端交互：即Client-side Action组，adf封装了DragImage，DragRectangle,Point,Line,PolyLine,Polygon,Circle,Oval这几种操作，相信已经能够应付我们绝大部份的应用了。如果还不行，别忘了最后还有一项custom，选中它，自己写javascript就可以了。记得是onmousedown的javascript哦。在交互完毕后的代码中，如果需要返回到服务器端的话，记得执行_doPostback函数。
２、服务器端功能实现：即Server-side Action组。Map组件针对客户端封装的每种行为，都提供了一个事件，表示客户端交互完毕后，立即执行该事件中的代码。这些事件有Oval,Line,Point,Polygon等。如果客户端操作是自己写的javascript，并最后提交了操作到服务器的话，可以在Page_Load中处理你自己的操作。
上面介始的是Tool类型的工具条按钮。至于Command类型的就简单多了，我们可以选中FullExtent看一下，它的属性少了Client-side Action组和Server-side Action组。它的实现代码就直接到Toolbar组件的CommandClick事件中。自己去看吧。
其它组件就不介绍了。
说了这么多，相信您对用.net adf开发Web GIS已经有一个大致的了解，也基本明白了.net adf的封装方式，您已经可以开始偿试修改已有的功能，或者进行一些简单的扩展了。下一节我就再介绍一下怎样进行简单的功能扩展。
待续




posted on2006-01-16 19:20[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=318470)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
