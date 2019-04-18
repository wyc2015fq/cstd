# Scene Viewer新功能(2017.9) - 长歌行 - CSDN博客





2017年09月27日 13:45:59[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：740
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/09/21/whats-new-in-scene-viewer-september-2017/)

作者：[Russell Roberts](https://blogs.esri.com/esri/arcgis/author/Russ-2/)

ArcGIS Online 2017年9月发布新版本的同时，我们迎来了Scene Viewer的另一个令人兴奋的更新。我们为点云层（也是一种新的OGC层类型）增加了智能制图支持，并支持垂直坐标系。

**点云场景图层的智能制图**

现在，在Scene Viewer中您可以使用直观的用户界面和交互式绘图选项（如点颜色和点大小），来绘制点云场景图层。您可以直接在网络上将您的点云数据以True Color，Classes，Elevation或Intensity的风格进行可视化和分析。这些风格通过LAS数据集中包含的数据和属性，可以创建出引人注目的可视化效果。使用强度调制选项，以增加点的深度感知和对比度，并检查不同的颜色斜坡以获得更有吸引力的外观。要了解有关这些点云风格的更多信息，请在[此处](http://doc.arcgis.com/en/arcgis-online/create-maps/scene-style-pcl.htm)查看文档页面。下图显示了使用由Merrick提供的数据使用高程和强度调制来设计图层的点云场景图层。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/PointCloud_SmartMapping1-1024x557.jpg)

**3D对象和集成网格场景层的高程偏移**

使用新的高程偏移选项调整建筑物（3D对象），集成网格和点云场景图层在地面上方的垂直位置。在此版本之前，您只能将偏移应用于要素图层和点图层。现在使用这些图层，您可以在配置面板中输入正值或负值来应用偏移量。如果您的图层与地形完全不一致，改功能可能会有所帮助。

**支持垂直坐标系**

使用Scene Viewer可以创建精确的各种三维数据的混合场景。当您具有垂直坐标系（VCS）的高程或场景层时，创建具有精确垂直测量的场景会比较困难。 Scene Viewer现在可以兼容各种不同的VCS，就像我们目前对多个水平坐标系时所做的那样。在从Scene Viewer和ArcGIS Pro创建的网站场景中支持这一点。

**WMS层支持**

我们一直在支持OGC各种图层，目前增加了对Web Map Service（WMS）层的支持。在ArcGIS Online中，可以从组织的图库或我的内容将WMS图层添加到场景查看器。 Scene Viewer支持为此新图层类型配置的弹出窗口和图例。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/WMS-1024x554.jpg)

**访问最新的Scene Viewer**

您现在也可以轻松地访问您一直在使用的网络场景，就像在地图查看器中一样。在“新建场景”按钮下，您将看到创建新的全局或本地场景以及您最近保存的一些网络场景的选项，以便您再次开始工作。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/NewScene-277x300.jpg)

**图层项配置增强**

还有一些额外的增强功能和对AGOL功能的支持，您可以在Scene Viewer中查看此版本。
- 支持弹出框的间断式刷新：在Scene Viewer中支持已配置为刷新媒体的弹出窗口。您可以在地图查看器或这些图层的项目详细信息中进行配置。[在这里](http://doc.arcgis.com/en/arcgis-online/create-maps/configure-pop-ups.htm#ESRI_SECTION1_B16A9E11A48D48CBAFA9DB964FCEB805)看看如何配置.
- 弹出窗口和渲染器引入[Arcade](https://developers.arcgis.com/arcade/)。
- 标签表达式中引入Arcade：在6月版本中，我们添加了对点特征属性的标签支持。现在，您可以对点要素图层使用标签Arcade表达式。您可以在地图查看器中使用Arcade创建这些标签，或者在功能图层的项目明细页面中创建这些标签，并将更改保存到图层。当您将图层添加到Scene Viewer时，您将看到使用该表达式或使用单个属性的选项。
- 改进了要素简化功能。那些具有高水平细节的线和多边形，如电源线或轮廓线，现在将变得更平滑。
- 支持WMTS数据的自定义参数。



