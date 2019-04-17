# 【译】ArcGIS Runtime SDK 100.2.0的新增功能 - 长歌行 - CSDN博客





2017年12月20日 17:25:47[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2596
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/12/18/whats-new-in-arcgis-runtime-sdk-100-2-0/)

原作者：Eric Bader on December 18, 2017

我们很高兴地宣布，ArcGIS Runtime SDK 100.2.0现已发布！

100.2.0，也被称为“Update 2”，是对100.0.0的第二次重大更新。 这是从各种意义上说都是一个重要的版本。 首先，这个版本的功能更贴近于10.2.x版本。 其次，100.2.0桥接了ArcGIS Engine和Runtime之间的大部分功能差距，允许人们将其项目迁移到ArcGIS Runtime平台。

那么，有哪些新鲜功能呢？ 本版本中包含了一些非常重要的主题，包括新的图层和数据源，附加的分析工具，2D和3D渲染改进，更好地处理标准和自定义地理转换，工作流生产力增强，错误修复等等。

进一步来看：

### 新图层

Runtime SDK中的OGC支持随着Update 2继续增长。新的WMS层可用于在地图和场景中，以显示来自符合OGC标准的WMS 1.3服务的内容。 您可以识别显示中的各要素，并为其生成图例。 未来版本将支持其他版本的WMS规范。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/wms_identify-300x201.png)

新增一个**ENC**图层。它可以用于显示S-57格式的ENC（Electronic Navigational Charts）数据的内容。 Runtime的实现遵循于S-52展示库4.0规范。 您可以查询那些显示的要素，选择那些要素以及更改视图组，文本和其他元素（如信标，轮廓，配色方案等）的各种显示设置。未来版本会支持加密的S-63格式。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/enc-300x194.png)

### 新的数据格式

现在支持直接读取Shapefile数据集。 可以将Shapefile添加为地图和场景中显示的要素图层。 您还可以通过shapefile功能表在数据集中添加和编辑要素。

此版本中还增加了对OGC GeoPackage格式的支持。 您可以将GeoPackage中的矢量和栅格数据集分别添加到地图和场景中作为要素图层和栅格图层。 您也可以在现有的GeoPackage功能表中添加和编辑功能。

### 渲染提升

现在要素图层除了可支持动态渲染外还支持静态渲染，就像图形叠加层一样。您可以通过加载设置在地图或场景级别设置渲染模式，也可以在要素图层级别设置逐层渲染模式。动态渲染改善了地图或场景导航过程中的要素的外观和交互性。包含点几何图形的要素图层现在默认为动态呈现，其符号在地图视图中保持面对屏幕，并将在场景视图中以“广告牌”的方式呈现。包含多边形或多段线几何图形的特征​​图层默认情况下仍然呈现静态，但您可以选择动态渲染它们以允许基于要素图层属性的拉伸和基于z值的曲面布局的3D行为。

在更新大量图形时，图形叠加层的显示性能也得到了改善，在某些情况下，与以前的版本相比，速度提高了2倍。

已经引入了新的多层符号类型，以更好地表示包含高级制图的要素图层。在此版本中，这些符号类型不能由开发人员创建，但可以在ArcGIS Pro中制作，并通过要素服务，Mobile Map Packages和Mobile Style文件进行部署并应用在Runtime中。在将来的版本中，这些符号类型将进一步扩展，以便开发人员可以通过组合多个符号图层来产生复杂的制图效果。

### 分析

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/analysis2-300x136.png)

使用新的场景分析API，您可以定义使用当前3D场景视图中显示的数据执行的各种分析，然后渲染动态更新的结果。 此版本包括两种类型的场景可见性分析：视域和视线。 视域分析高亮显示了您的3D场景中可以从给定观察者看到的区域。 视线显示沿着观察者和目标位置之间画出的线可以看到那部分。 对于任一类型的分析，观察者和/或目标可能是移动的或静止的。
新的统计查询API允许您获取特征表中指定字段的以下任何统计信息：总和，平均值，总数，最小值，最大值，标准偏差或方差。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/stats_query.png)
### 坐标系统和转换

现在可以在`geometry engine`类的投影函数中发现，定义和使用地理变换（或基准变换）。 在此版本之前，无论何时，投影数据都会自动使用最合适的转换。 一个新的`transformation catalog` 类允许您查找在具有不同基准的两个空间参考之间进行投影时使用的最佳转换列表。 你甚至可以通过一个特定的矩形来取得适合该特定区域的变换。 您可以通过well-known ID（WKID）定义转换，或使用WKT创建自定义转换。 您也可以更改内部使用的默认转换方法。 支持基于方程式和基于网格的转换。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/geotrans-300x156.png)
未完待续

### 离线地图

本版本增强了按需工作流程，支持从ArcGIS Online或者ArcGIS Enterprise托管的矢量瓦片(Vector tile）地图服务中到处和下载矢量瓦片包(VPK）。VPK中包含了一个模型的样式来定义如何渲染瓦片。矢量瓦片图层还能够将门户中的自定义样式作为资源引用。这些样式可以下载并作为资源应用到客户端上的矢量瓦片层。

本版本扩展了在预计划的工作流中使用离线地图的功能。离线地图功能可使您的用户即便在网络连接不良或者不存在的情况下也能保持高效工作。预计划的工作流通过提供一种允许地图作者定义和预先创建离线区域而非现场作业的方式来补充现有的按需工作流程。您的现场工作人员可以按需下载地图区域。使用这两种脱机工作流方式，现场工作人员都可以在网络恢复时将任何更新同步到操作数据中。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/offline-300x71.png)
### 本地化网络解析器返回的消息

路线任务和最近的设施任务返回的路线有10种其他语言：丹麦语，芬兰语，印地语，克罗地亚语，印度尼西亚语，挪威语，罗马尼亚语，塞尔维亚语，越南语和中文（台湾）。 如果请求的语言不可用，则路线返回到默认语言，而不是失败。而且，由Route，Service Area和Closest Facility任务返回的错误消息现在是一致的，并被翻译成所有支持的语言。 注：Java和Android将在下一个主要版本中支持本功能。

### 事务编辑

在地里数据库和地里数据库要素表中已经支持了事务性编辑。这允许您执行多个编辑，然后选择将他们全部作为一个单元提交，如果任何一个单元遇到错误，所有的提交会全部回滚。当前还不支持嵌套事务。 注：Qt将在下一个主要版本中支持这一点。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/trans_edits-300x118.png)

### 时间支持

您现在可以在地图视图和场景视图中应用时间范围，以过滤来自时间感知图层的内容的显示。时间感知图层包括要素图层，地图图像图层和栅格图层。 您可以将时间偏移应用于时间感知图层，可用于比较数据随着时间的推移。 查询特征表时，您也可以指定时间参数。 注：Qt，Java和Android将在下一个主要版本中支持这一点。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/time-aware-300x226.png)

### 其他值得关注的事情

**按类授权**

我们已经在指南中添加了一个有用的资源，它告诉API中的哪些类需要哪个级别的Runtime授权。 在开发人员指南的每个API参考部分中查找许可详细信息主题。 下面是一个例子…

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/12/license_by_API2.png)

**对迁移到100.2的支持**

本主题提供了一些非常方便的高级技巧，用于规划从以前的Runtime版本开始的100.2迁移。 您可以在开发人员指南的每个“开始”部分中找到“从10.2.x迁移到100.x”主题。

### Download and get started

想要获取100.2.0版本, 可以浏览这个页面[ArcGIS for Developers web site](https://developers.arcgis.com/arcgis-runtime/), 浏览ArcGIS Runtime SDK主页 并下载SDK. 你也可以从[Nuget](https://developers.arcgis.com/net/latest/wpf/guide/install-the-sdk.htm#ESRI_SECTION1_189FB0998B464B8CB614B745F1B33D62), [Gradle](https://developers.arcgis.com/android/latest/guide/develop-your-first-map-app.htm#ESRI_SECTION1_E1CFE3BFB2814243ADFD8CE8DB11ACC2), 或者 [CocoaPods](https://developers.arcgis.com/ios/latest/swift/guide/install.htm#ESRI_SECTION1_9448ADD25F414BA3BA0319E77C1E8FF8)中获得它. 如果你最近才接触ArcGIS平台的开发并且没有任何ArcGIS 开发者的订阅的话，你可以简单的从[sign up](http://developers.arcgis.com/sign-up/) 一个免费的账户开始，就这样，你就可以访问开发自己的App所需要的一切。

有关这个令人兴奋的发行版的更多详细信息，请参阅以下各版本的具体发行说明 [Android](https://developers.arcgis.com/android/latest/guide/release-notes.htm), [Qt](https://developers.arcgis.com/qt/latest/qml/guide/release-notes.htm), [.NET](http://developers.arcgis.com/net/latest/wpf/guide/release-notes.htm), [Java](https://developers.arcgis.com/java/latest/guide/release-notes.htm), [iOS](https://developers.arcgis.com/ios/latest/swift/guide/release-notes.htm) and [macOS](https://developers.arcgis.com/macos/latest/swift/guide/release-notes.htm).







