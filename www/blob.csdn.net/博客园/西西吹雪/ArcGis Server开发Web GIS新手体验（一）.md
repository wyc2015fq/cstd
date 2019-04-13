
# ArcGis Server开发Web GIS新手体验（一） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGis Server开发Web GIS新手体验（一）](https://www.cnblogs.com/watsonyin/archive/2005/12/23/303374.html)
（以下针对于windows操作系统.net开发环境)
ArcGIS Server是ESRI公司最新推出的服务器端品，主要可以实现两大功能：
１、强大的Web GIS系统的开发；
２、分布式GIS系统的开发；
ArcGIS Server其内核与ArcGIS Desktop和ArcGIS Engine一样，都是ArcObjects库。其所谓的Web GIS，其实无非就是用Web技术来封装ArcObjects而已；而分布式的开发则是通过DCOM来实现的。
小弟前几个月搞了一下ArcIMS，发觉开发起来确实痛苦啊，整天就是折腾javascript+arcxml，郁闷死了，听闻有ArcGIS Server这种好东西，稍稍了解一下，便向老板建议转向ArcGIS Server，老板居然同意了。于是便开发学习了。网上四下里找了一下，发现可用的资源非常少，只好自己琢磨。从零开始真不容易啊，这不，刚上手就碰到了两个问题，好在都已经解决，现在写下来，希望对和我一样的新手有所帮助。
ArcGIS Server的安装非常简单，先安装ArcGIS Server，然后安装DotNet ADF，最后用ArcCatelog添加一个Server，然后再添加一个ServerObject，这样就可以进行开发了。这里的ServerObject与ArcIMS中的Service很像，其实应该是一个概念上的东西吧。添加一个ServerObject需要一个mxd或pmf文件，用来保存和管理需要发布的数据。
好了，现在可以开始开发了。
打开Visual Studio 2003，打开新建项目对话框，发现ESRI公司已经为我们准备好了一些ArcGIS Server Projects的模板，包括Map Viewer Web Application，Page Layout Web Applicaiton等等，我们就建一个Map Viewer Web Application吧，选中它，输入项目名称，然后点击“确定”按钮，我们发现一个已经包含了基本数据浏览和查询功能的Web GIS程序已经建好了。
好，打开Default.aspx文件，发现网页包含一个Map组件，一个Toolbar组件，一个TOC组件，一个OverviewMap组件等等。看来与C/S开发模式下很像嘛，爽啊。其中Map组件和OverviewMap组件有两个必填属性：Host和ServerObject，就分别对应着我们在ArcCatelog中添加的的Server和ServerObject，输入它。然后设置TOC组件和Toolbar组件的Buddy属性，ＯＫ，大功告成！
运行！咦，发生错误，定睛一看，原来是访问拒绝，这是怎么回事呢？记得以前看到一篇文章，说在Asp.net中使用COM组件，经常由于权限原因，产生访问拒绝，而ArcGIS Server本身就是对ArcObjects这些COM组件的再封装，看来就是这么回事了。找出这篇文章来，原来要在Web.config中加入一行就可以了：
<identity impersonate="true" userName="Administrator" password="123456"/>
再运行，哇塞，酷毙了，一个漂亮的WebGIS展现在眼前。可以浏览，查询，图层管理，鹰眼导航，还有指北针...
(后来发现页面上还一个组件叫impersonation没用上，看了一下它的属性，只有一个identity，点击它，你会发现...呵呵，原来这个组件就是用来干这个事的，晕，又浪费了我两个小时的宝贵时间...)
一行代码都不用写，爽。可是又很失落，心里没有底，这个Web GIS究竟是怎样实现的呢？
待续




posted on2005-12-23 14:34[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=303374)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
