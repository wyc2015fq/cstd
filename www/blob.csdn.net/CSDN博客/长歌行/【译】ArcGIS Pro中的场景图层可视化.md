# 【译】ArcGIS Pro中的场景图层可视化 - 长歌行 - CSDN博客





2018年01月19日 12:59:05[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：603
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









# ArcGIS Pro中的场景图层可视化

原文地址： [Visualizing Scene Layers in ArcGIS Pro](https://blogs.esri.com/esri/arcgis/2018/01/16/visualizing-scene-layers-in-arcgis-pro/)

by Andrew Johnson on January 16, 2018

在ArcGIS Pro 2.1版本中，新增了关于可视化场景图层的重要功能，让我们来看看这些新功能吧！

当您将点场景图层(point scene layer)或者三维对象场景图层(3d object scene layer)添加到ArcGIS Pro中时，你会发现新增了’Data’选项卡。在’Data’选项卡中包括了，定义查询(Definition Query), 排除(Exclusion)和选择(Selection)功能。让我们看看它们分别有什么功能：

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/SL_Ribbon1.png)

## 选择

在’Selection’功能区中，您可以选择场景图层中的要素或者清楚已选定的要素。下拉列表包括，矩形，线，圆形等标准选择模式。这种选择方式较之来回切换地图选项卡更为方便。由于该选择方式是基于当前场景已渲染的要素，所以当视角调远之后，一些要素会消失。改功能可以结合下面的“排除”功能使用。

## 排除

功能区的这两个按钮允许您隐藏或者显示之前选择的要素。这个功能可以为城市规划提供服务，你可以先隐藏建筑物，然后打开你包括想建造的建筑的新图层。另一个用途是，可以“砍到”目前的树木，并打开包含新设计的树木图层。

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/image003.gif)

注：选择功能的工作流程只适合小规模数据，如果想隐藏更多的要素，请使用定义查询功能。
## 定义查询

定义查询现在可以应用于场景图层了。您可以通过场景图层的图层属性设置定义查询。场景图层支持标准化的查询语句。’Data’选项卡中的定义查询功能区允许您查看已应用的查询以及清除查询。

注：场景图层上创建定义查询的功能取决于图层中可用的统计信息。这些统计信息在生成场景图层包(.slpk)时可以得到。定义查询的修改也只能使用该统计信息进行计算。在ArcGIS Enterprise中通过要素图层创建的场景图层将自带这些统计信息。因此，在应用定义查询时，这些统计信息也能重新计算。当对定义查询的修改需要动态统计信息时，使用已关联要素图层的场景图层。

## 点和三维对象场景图层的符号化

以下功能围绕符号化展开，并取决于您是否使用从场景图层包加载的场景图层，或者对象场景图层是否和要素图层关联。

已经为点和三维对象场景图层添加了两种新的符号化方法：Unique Values和Unclassed Colors。 此外，三维对象场景图层还支持以属性驱动计算透明度的符号化系统，点场景图层支持以属性驱动计算透明度，旋转，尺寸的符号化系统。查看一下表格，获取该功能的详细信息。

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/SL_Symbology2.png)

x表示支持全部功能，i表示需要spk中的统计信息才能支持的功能
更多信息，请看以下文档：
- [Point Scene Layer Symbology](http://pro.arcgis.com/en/pro-app/help/mapping/layer-properties/point-scene-layer-symbology.htm)
- [3D Object Scene Layer Symbology](http://pro.arcgis.com/en/pro-app/help/mapping/layer-properties/3d-object-scene-layer-symbology.htm)

## 标签

另一个你可能不知道的功能是关于点要素的标签。当在要素图层上启用标签并将其作为Web图层共享或者输入到创建场景图层包工具时，标签将与场景图层一起缓存。标签可以打开和关闭，但由于它们已经被场景图层固化缓存的缘故，不支持修改。

## 点云的抽稀和尺度变化支持

点云场景图层是可扩展的，支持搞笑的处理大规模的点云数据集。由于一部分来自于由于硬件的限制，显示海量的点数据集会非常缓慢。但是点云场景图层的显示效率很高，会按照您需要显示的屈戌以优化的点的分辨率的方式显示。下面介绍的“显示限制”和“密度”可以控制显示的点的个数。

## 显示限制

应用显示限制将会给用于显示点云场景图层的点数附加一个大小限制值。默认值是1,000,000。如果在给定的显示范围内，点云场景图层超过了指定的点数，则该图层将会抽稀。如果您在放大和缩小点云场景图层时看到瓦片形式的更新效果，请考虑增加显示限制或者调整点的密度。

## 密度

此设置可以控制点云场景图层中的点密度。您可以使用滑块进行设置，将滑块向左移动到Min可获得更粗略的点云分辨率。

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/lidar.gif)

## 点云符号化

对于点云场景图层，目前有四种符号显示方法：Stretch，Classify，Unique Values和RGB。 每种方法都会选择下面的某一种属性：

点符号化相关属性：
- 高程 Elevation
- 类别 Class
- 返回值 Return
- 彩色值 RGB
- 强度 Intensity

![img](https://blogs.esri.com/esri/arcgis/files/2018/01/image008.gif)

更全面的场景图层新功能请见：[ArcGIS Pro 2.1中场景图层的新功能](http://blog.csdn.net/uninterrupted/article/details/79094967)





