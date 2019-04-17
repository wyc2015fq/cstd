# 【译】ArcGIS Pro 2.1中场景图层的新功能 - 长歌行 - CSDN博客





2018年01月18日 13:21:40[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：530
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









原文地址: [What’s new with Scene Layers in ArcGIS Pro 2.1](https://blogs.esri.com/esri/arcgis/2018/01/16/whats-new-with-scene-layers-in-arcgis-pro-2-1/)

by Andrew Johnson on January 16, 2018

场景图层团队一直致力于为各平台上的场景图层添加新的功能。本博客将重点介绍一些添加到ArcGIS Pro2.1中的新功能。

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/apps.jpg)

上图为支持场景图层的应用平台
如果你对场景图层还不太熟悉，也无需担心，查看下面的帮助文档，你将对场景图层有一个全方位的认识，并知道它们的应用场景。文档链接：[场景图层是什么？](http://pro.arcgis.com/en/pro-app/help/mapping/layer-properties/what-is-a-scene-layer-.htm)

场景图层仍然是一个比较新的图层类型，每次发布新版本的Pro的时候，都会在场景图层的创建和应用方面加入很多新的功能。下面是一个添加的功能的列表：

## 地理处理工具

### [场景图层创建打包工具](http://pro.arcgis.com/en/pro-app/tool-reference/data-management/create-scene-layer-package.htm)
- 支持LAZ， LAS和ZLAS的文件或者文件夹
- 新的坐标系参数支持您定义缺失坐标信息的LAZ数据。在检测到缺失坐标信息的数据之前，参数被隐藏

### [场景图层包验证工具](http://pro.arcgis.com/en/pro-app/tool-reference/data-management/validate-scene-layer-package.htm)
- 支持所有的场景图层类型（3D Object, Integrated Mesh, Point, Point Cloud)

## ArcGIS Pro中的应用方式
- 用户可以选择场景图层对象，然后在Data选择卡>exclusion group中选择’hide selection’和’show selection’
- 用户可以在场景图层上定义查询
- 用户可以在应用场景图层包或者服务时，使用’set data source’
- 用户可以调整点云场景图层的点的显示限制和点密度
- 已经为点云场景图层添加了Popup的支持

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/SL_Ribbon.png)

## 符号
- 点和三维对象场景图层的Unique Value symbology的支持
- 点和三维对象场景图层的Unclassed colors symbology的支持
- 三维对象图层的符号透明度的属性驱动的支持
- 点图层的符号透明度，选择和尺寸的属性驱动的支持

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/SL_Symbology1.png)

图中x表示全部支持，i表示依赖于在slpk中存储的统计信息
## 其他

场景图层标准(i3s)已经被采纳为OGC社区标准 
[http://www.opengeospatial.org/standards/i3s](http://www.opengeospatial.org/standards/i3s)





