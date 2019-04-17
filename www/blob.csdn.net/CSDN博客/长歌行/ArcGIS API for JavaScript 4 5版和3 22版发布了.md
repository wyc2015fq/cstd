# ArcGIS API for JavaScript 4.5版和3.22版发布了 - 长歌行 - CSDN博客





2017年10月09日 14:48:04[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1084
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/09/29/arcgis-api-for-javascript-version-4-5-released/)

> 
by Kristian Ekenes, Lloyd Heberlie, Björn Svensson, and Julie Powell on September 29, 2017


4.5版本的ArcGIS API for JavaScript增加了一些关键功能和几个更小（但甜美的）增强功能，将会派上用场。 以下是[4.5](https://developers.arcgis.com/javascript/latest/guide/release-notes/index.html)（以及[3.22](https://developers.arcgis.com/javascript/3/jshelp/whats_new.html)）的亮点： 在[发行说明](https://developers.arcgis.com/javascript/latest/guide/release-notes/index.html)中可以找到详细介绍新功能的全面概述。

### 快速的要素图层：与超过一百万个要素进行交互

现在，您可以[更快速](https://developers.arcgis.com/javascript/latest/api-reference/esri-layers-FeatureLayer.html#faster-rendering)地显示[更多的数据](https://developers.arcgis.com/javascript/latest/api-reference/esri-layers-FeatureLayer.html#fetching-improvements)和动态显示功能，例如[这个图层](https://developers.arcgis.com/javascript/latest/sample-code/visualization-vv-color-animate/index.html)，它包括超过100万的建筑物占地空间的图形。 这是通过在2D地图中启用FeatureLayers的[WebGL选项（测试版）](https://developers.arcgis.com/javascript/latest/api-reference/esri-layers-FeatureLayer.html#webgl-rendering)实现的。

![这里写图片描述](https://developers.arcgis.com/javascript/latest/assets/img/guide/whats-new/45/featurelayer-webgl-fast-updates.gif)

FeatureLayer在将来的版本中将会默认直接使用WebGL渲染。

### 绘图：在这里！

您现在可以在4.x应用程序中启用[绘图](https://developers.arcgis.com/javascript/latest/api-reference/esri-widgets-Sketch-SketchViewModel.html)。 在初始版本的草图工具中，您可以在2D地图中绘制新的几何图形。 绘图和编辑的全面支持将在以后的版本中逐步提供，其中包括：
- 编辑现有的几何体
- 创建和编辑新的几何，包括（但不限于）多点几何体。 
- 素描/编辑小部件 
- 支持在3D场景中绘图
- 完全的编辑支持，包括启用通用几何校验规则的能力，如防止自相交行。 查看这个新的草图样本，并播放当前的功能。

查看这个[新的草图示例](https://developers.arcgis.com/javascript/latest/sample-code/sketch-geometries/index.html)，并使用当前的功能。

![这里写图片描述](https://developers.arcgis.com/javascript/latest/assets/img/guide/whats-new/45/sketch-geometries.gif)

### OGC增强

现在可以在3D场景（3D Scene）中显示WMS和WMTS图层。 另外，为2D地图添加了KML支持。 将来的版本中将提供对3D场景中KML的支持。

### 垂直方向放置3D对象的新选项

可以使用字段值，z值或表达式设置建筑物和其他3D对象的垂直位置。 对将物体放置到地面下或到地表上的情况会比较有用。

![这里写图片描述](https://developers.arcgis.com/javascript/latest/assets/img/guide/whats-new/45/elevation-options-sample.png)

### 3.22版本的聚类

如果您的地图包含一个拥有大量点的图层，则配置[点聚类](point%20clustering)可以更容易地从数据中直观提取信息。 启用[聚类](https://developers.arcgis.com/javascript/3/jsapi/featurelayer-amd.html#setfeaturereduction)时，一定距离内的点要素被分组成一个符号。 您可以使用以下任一方法在JavaScript应用程序中启用群集：
- 在[ArcGIS Online的Map Viewer](https://blogs.esri.com/esri/arcgis/2017/09/20/clustering-now-available-in-arcgis-online/)中应用集群，保存您的Web地图，然后加载到JavaScript应用程序中
- 在[JavaScript代码](https://developers.arcgis.com/javascript/3/jsapi/featurelayer-amd.html#setfeaturereduction)中直接在FeatureLayers或CSVLayers上配置群集。

![这里写图片描述](https://developers.arcgis.com/javascript/3/jshelp/images/whats_new/3.22/clustering-toggle.png)

聚类功能目前只能在3.22版本中使用，但该功能将会在2018年初加入到4.x版本中。[这个博客](https://blogs.esri.com/esri/arcgis/2017/09/20/clustering-now-available-in-arcgis-online/)讨论了聚类如何实现数据挖掘。

### 更多

你可以通过浏览发行说明和新示例来了解更多更新，例如支持垂直坐标系，弹出窗口的改进，以及扩展自动播放支持等节省时间的增强功能。

[Version 4.5 release notes](https://developers.arcgis.com/javascript/latest/guide/release-notes/index.html)
[Version 4.5 samples](https://developers.arcgis.com/javascript/latest/sample-code/index.html?search=4.5)
[Version 3.22 release notes](https://developers.arcgis.com/javascript/3/jshelp/whats_new.html)
[Version 3.22 samples](https://developers.arcgis.com/javascript/3/jssamples/#latest_samples)



