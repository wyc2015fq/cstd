# ArcGIS Earth Automation API is Here - 长歌行 - CSDN博客





2017年09月15日 14:09:30[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：400
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









# ArcGIS Earth Automation API来了

本文主要介绍了ArcGIS Earth的‘自动’API，[原文地址](https://geonet.esri.com/groups/arcgis-earth/blog/2017/09/12/arcgis-earth-automation-api-is-here)

随着ArcGIS Earth 1.6的发布，我们同时也可以很高兴的告诉大家，ArcGIS Earth的Automation API已经可用了。更多信息请见[ArcGIS for Developers](https://geonet.esri.com/external-link.jspa?url=https%3A%2F%2Fmaster-stage.developers.arcgis.com%2Fdocumentation%2F) 或者是 [ArcGIS Earth homepage](https://geonet.esri.com/external-link.jspa?url=http%3A%2F%2Fdocdev.arcgis.com%2Fen%2Farcgis-earth%2Fautomation-api%2Fget-started.htm)

在差不多两年前，我们发布ArcGIS Earth的第一个版本的时候，就听到有关于API的需求。使用这些API能够控制ArcGIS Earth以及与二三维的数据内容进行交互。因此，我们设计了这个API，主要用于app to app（应用程序对应用程序）的通讯。

## 它是什么？

ArcGIS Earth Automation API 提供了基于WCF的服务契约（Service Contract），这样第三方的基于.Net的应用程序就可以通过使用API提供的服务契约与ArcGIS Earth进行通讯。ArcGIS Earth提供的通讯方式使用[NetNamedPipeBinding](https://geonet.esri.com/external-link.jspa?url=https%3A%2F%2Fmsdn.microsoft.com%2Fen-us%2Flibrary%2Fsystem.servicemodel.netnamedpipebinding%28v%3Dvs.110%29.aspx)，可以提供快速，安全的单机绑定。

## 它能做什么？

通过ArcGIS EarthAPI，用户可以在他们自己的应用程序基础上创建.Net的扩展，并使用这个扩展来访问ArcGIS Earth的资源，或者是控制3D浏览。

ArcGIS Earth Automation API 提供了以下的功能： 

1、 获取和设置相机信息 

2、 控制相机的飞行 

3、 从本地文件夹或者网络路径添加图层 

4、 管理workspace内容（目前仅提供添加图层和删除所有图层） 

5、 提供当前视图的截图
目前的ArcGIS Earth Automation API为用户的痛点提供的解决方案。它能给你场景和工作区的控制能力。Esri将会添加更多功能。

## 现在就试试！

我们来看看能用它干什么吧。下面的一个例子展示了，如何在ArcGIS Pro 2.0中关键一个扩展，并使用这个扩展来同步ArcGIS Pro和ArcGIS Earth的三维场景的相机视角。如你所见，在切换ArcGIS Pro的书签的同时，我们会发现ArcGIS Earth的相机视角也会进行相应的改变。 
![](https://geonet.esri.com/servlet/JiveServlet/showImage/38-57809-370813/api3.gif)

想试试这个add-in吗，去这个页面[View Synchronization Sample](https://geonet.esri.com/external-link.jspa?url=https%3A%2F%2Fwww.arcgis.com%2Fhome%2Fitem.html%3Fid%3D3598be1751934fedafdadb4616063e1d)，然后下载用例代码到你本机。请注意，你需要在你本地的开发环境中编程生成该插件。如果你想获取add layer是否成功等信息，你需要修改这部分的代码以完善双工通讯部分。
## 想知道更多吗？

从[ArcGIS Earth product homepage](https://geonet.esri.com/external-link.jspa?url=http%3A%2F%2Fwww.esri.com%2Fsoftware%2Farcgis-earth)中获取更多信息，在GeoNet上并上传你的问题或者与我们分享你的想法。
- [Using the Automation API](https://geonet.esri.com/external-link.jspa?url=http%3A%2F%2Fdoc.arcgis.com%2Fen%2Farcgis-earth%2Fautomation-api%2Fuse-api.htm)
- [ArcGIS API samples](https://geonet.esri.com/external-link.jspa?url=http%3A%2F%2Fdoc.arcgis.com%2Fen%2Farcgis-earth%2Fautomation-api%2Fsamples.htm)
- [ArcGIS Earth Automation API GitHub](https://geonet.esri.com/external-link.jspa?url=https%3A%2F%2Fgithub.com%2Fesri%2Farcgisearth-automation-api)

最后特别感谢Yongwei Kang和Mandy Li为本博客提供的素材。





