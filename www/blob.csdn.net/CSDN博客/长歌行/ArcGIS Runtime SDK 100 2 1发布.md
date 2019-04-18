# ArcGIS Runtime SDK 100.2.1发布 - 长歌行 - CSDN博客





2018年02月28日 14:37:44[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：531
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









原文地址： [ArcGIS Runtime SDK 100.2.1 released](https://blogs.esri.com/esri/arcgis/2018/02/20/arcgis-runtime-sdk-100-2-1-released/)

by Eric Bader on February 20, 2018

我们很高兴地宣布ArcGIS Runtime SDK 100.2.1已经发布！

这是基于去年12月发布的100.2主要版本([link](https://blogs.esri.com/esri/arcgis/2017/12/18/whats-new-in-arcgis-runtime-sdk-100-2-0/))的一次重点更新。它包含对12月份发布的功能的一些增强功能，并且还包含一些重要的错误修复。

增强功能包括以下内容：

## WMS
- 此发行版支持没有“GetCapabilities”查询字符串的网址。
- 运行时现在支持WMS版本1.1.0,1.1.1和1.3.0。以前，只支持1.3.0。
- 您现在可以设置WMS子层的可见性。

## 移动地图包

此版本支持包含栅格数据集和tile包的移动地图包（mmpk）。这些mmpks可以使用ArcGIS Pro 2.1版本创建。如果您加载mmpk的地图（显式地或通过将其传递给地图视图），将分别创建一个RasterLayer或一个ArcGISTiledLayer来显示栅格数据集和tile包。要访问mmpk的栅格数据集，您需要先解压缩它。这在访问移动地图包中有更详细的解释([link](https://developers.arcgis.com/qt/latest/qml/guide/create-an-offline-map.htm#ESRI_SECTION2_745BCF9870EA497B8D77853A0F8FE754))。

## 3D场景支持WGS84中的瓦片图层(tiled layer)

3D场景现在支持使用基于WGS84的瓦片图层作为底图或图层。 ArcGIS Runtime 100.2.0中的场景仅支持Web Mercator投影中的瓦片图层。(译注：简单理解就是两种投影方式，切片规则不同的栅格数据）

## 关于此版本的更多注意事项
- 此版本与100.2具有相同的产品生命周期。
- 它不会覆盖或安装在当前的SDK安装主页上。它将与开发机器上的任何其他运行版本并行生存。

要获取100.2.1，请转至ArcGIS for Developers网站([link](https://developers.arcgis.com/arcgis-runtime/))，在该网站中，您可以浏览至所选ArcGIS Runtime SDK主页并下载SDK。您也可以在某些平台上通过Nuget，Gradle或CocoaPods获取它。如果您刚开始在ArcGIS平台上开发并且尚未拥有ArcGIS Developer订阅，请注册一个免费帐户，并且您将能够访问开发应用程序所需的所有内容。

译注：即将发布的新版ArcGIS Earth也是基于ArcGIS Runtime SDK 100.2.1开发的，在性能和程序架构上都有很多提升。



