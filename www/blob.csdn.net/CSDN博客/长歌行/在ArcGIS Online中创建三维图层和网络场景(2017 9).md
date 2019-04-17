# 在ArcGIS Online中创建三维图层和网络场景(2017.9) - 长歌行 - CSDN博客





2017年10月02日 21:35:22[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1742
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/09/25/authoring-3d-layers-and-creating-web-scenes-in-arcgis-online/)

作者： Russell Roberts

*因为内容比较简略的原因，本文主要使用google机翻，人工校对*

### 为ArcGIS Online创建3D内容

在本博客中，我将采用此ArcGIS Pro教程中提供的数据，重点介绍ArcGIS Enterprise中的3D内容，并向您展示如何为ArcGIS Online创建3D内容。我们将介绍如何在Scene Viewer中创建场景图层，高程图层，设置场景图层和要素图层的样式，并最终使用我们撰写的网络场景创建一个网络应用程序。

### 我在这个博客中使用什么来创建3D图层和场景？

在本文中中我正在使用：
- 提供的数据可以在[这里](http://www.arcgis.com/home/item.html?id=fea39c57674c43c38521ee18fa9a997c)下载。
- ArcGIS Pro 2.0（您也可以使用1.4）
- ArcGIS Online组织，具有发布商角色的2级帐户
- 托管场景图层，托管高程图层，托管功能图层
- Scene Viewer
- Web AppBuilder

### 创建托管高程图层

对于我们将在Pro中创建的本地场景，我们将要使用托管的高程图层，因此当您将本地场景共享到ArcGIS Online以创建一个Web场景时，它将使用相同的托管高程图层和正确的范围。创建托管高程图层时，您将需要确保使用与底图相同的坐标系和切片规则。在提供的数据中，有一个切片规则，因此我们不必为高程层生成一个。在Pro中，我们将使用Manage Tile Cache工具。在这里，您将使用Portland_DTM3ft数据作为下载的文件地理数据库中的数据源，并在输入切片方式 中选择导入方案的选项，并选择Portland_DTM3ft xml文件并运行该工具。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Manage.png)

接下来，我们将使用“Export Tile Cache ”工具来生成包含“Manage Tile Cache”工具的输出的图块包。确保将导出缓存设置为**tile package**。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Export.png)

在此之后，您可以转到ArcGIS Online组织，然后单击添加项目并转到我的内容，然后选择生成的图块包。这将开始发布托管高程图层。托管高程层完成发布后，我们将其作为Pro中的高程层添加。在大于1GB的软件包上，您可以使用Pro中的**Share Package**工具。

### 创建一个剪贴到特定区域的本地场景

打开ArcGIS Pro并启动一个新的本地场景项目。一旦您的新项目加载完成，我们将要将该file geodatabase添加到此项目中。要在“Catalog ”窗格中执行此操作，请右键单击“Databases  ”并单击“ Add Database”。浏览并选择需要解压缩的下载文件，并添加地理数据库3D_Portland.gdb。

现在要将数据添加到本地场景，请转到“Map ”选项卡的“Map ”选项卡，在“Layer ”组中，单击“Add Data”。浏览到3D_Portland文件地理数据库并添加以下图层：

Clip 

Portland_Topo 

场景自动切换到数据的投影坐标系：NAD 1983 HARN StatePlane Oregon North FIPS 3601 Feet International。右键单击“地形底图”图层，然后单击“删除”。我们将把Portland_Topo 层取代它。
现在删除Terrain3D高程图层，因为我们将在此场景中使用自定义高程图层。您可以通过右键单击“Contents ”窗格，右键单击“场景”，然后单击“属性”，然后转到“海拔”选项卡，您将在“海拔”源下删除Terrain3D。要引入作为托管海拔图层发布的波特兰海拔数据，请单击添加高程源。浏览到我的内容下的波特兰海拔图层，该内容显示了您的ArcGIS Online 组织帐户下的内容。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Elevation1.jpg)

接下来，我们将要将该场景剪辑到底图的范围，因为我们将要在本地场景中使用我们要从Pro发布到ArcGIS Online的准确范围。 在扩展选项卡上，单击自定义范围。 单击Calculate from drop-down菜单，然后单击Clip。

现在，您可以从场景中删除Clip layer，我们将将此本地场景共享给ArcGIS Online以供以后使用。 转到共享选项卡，然后单击Web Scene。 在“Share as a Web Scene”对话框中单击“内容”，然后右键单击“图层”图层，然后单击该选项以配置Web图层。 我们现在要使用下载的zip文件中包含的Portland_Aerial瓦片方案文件。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Customtiling-260x300.jpg)

在配置Web层面板下，单击配置并在下拉菜单中选择平铺方案文件选项，然后选择Portland_Aerial文件。 设置完后，返回“共享”面板，开始发布过程。 在使用托管标高层时，这将仅发布拓扑图层和剪辑区域局部场景。 您可以在Pro中“共享”面板底部提供的链接中查看网络场景。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/WebScene_Elevation-1024x555.jpg)

### 创建3D建筑 - 发布托管的3D对象场景图层

我们场景图层的基础层已经完成，让我们开始为这个网络场景创建操作层。将建筑物的足迹添加到Pro中的场景使用在“地图”选项卡上的“图层”组中，单击“添加数据”。在目录中，将“建筑物”图层从“2D图层”组拖到“3D图层”组中，以便您可以定义建筑物的拉伸。右键单击建筑图层，然后单击属性。在“高程”选项卡上，单击“功能属性”下拉菜单，然后单击“地面”。

建筑物现在处于零的高度，在地形表面下方。在零高度启动建筑物将确保建筑物被拉伸到正确的高度，不会受到地形坡度的影响。单击“确定”关闭“属性”对话框。单击内容窗格中的建筑图层，然后单击外观上下文选项卡。单击要素拉伸类型，然后单击基础高度。单击“Extrusion Expression”按钮。从屋顶高程中减去调整的曲面高程[ROOF_ELEV] - [SURF_ADJ]。单击单位下拉菜单，然后单击英尺以匹配数据的投影。建筑物现在被拉伸到正确的高度，但尚未与表面对齐。

![这里写链接内容](https://blogs.esri.com/esri/arcgis/files/2017/09/ExpressionBuilder-195x300.png)

在你拉伸了建筑物的足迹之后，将三维图层转换为要素类，以便从拉伸的多边形中创建multipatch要素类。 对于分组字段，单击构建ID，并接受所有其他默认值。 单击运行。当您选择输入功能的分组字段时，会将多个输入要素合并到相同的输出功能中。 所得到的剩余属性将从其中一个输入记录继承。 这将仅限于分组属性来限制输出multipatch体中的属性。 我将多边形图层连接到多面体数据集，并在多面体要素类导出，以创建一个新的要素类，其中将包含一组属性以用于场景查看器中的弹出窗口或智能制图。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Multipatch-1024x756.png)

要将此多面体要素类共享到ArcGIS Online以创建托管的要素图层，将图层向下拖动到“内容”窗格的“2D”部分，然后右键单击并选择“作为Web层共享”。 给图层标题和标签后，点击发布。

图层完成发布后，转到ArcGIS Online中的要素图层项目。 在要素图层项目上，您将要从图层发布场景图层。 要执行此操作，请转到项目右侧的“发布”按钮，然后选择“场景层”选项。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/PublishSceneLayer-300x128.png)

在发布对话框上填写标签和摘要，并在全局或本地选项上选择“本地”选项。 我们选择此选项，因为它将为本地投影创建场景层，因为高程和平铺图层都在同一个局部坐标系中。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Publish.jpg)
场景层完成发布后，可以将其添加到网络场景中。 打开我们从Pro创作的本地网络场景，然后单击添加图层，并将场景图层添加到网络场景中。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/AddLayer-1024x554.jpg)

我们可以通过点击内容窗格中的图层来更改建筑物的颜色。 对于此层，我使用维护代理属性使用Types智能制图渲染器对建筑物场景层进行样式，该渲染器将根据该属性值唯一地对建筑物进行着色。 我将使用此层的默认颜色，但您可以单击选项按钮更改使用的颜色，透明度和高程偏移。 您也可以设置隐藏图例或禁用弹出窗口。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/SmartMapping-1024x555.jpg)

### 使用Web样式来设置点的样式

接下来我们要做的是发布提供的地理数据库中的树层。如果您打开此功能类的属性表，您将看到要素类中的功能少于2000个。这意味着我们不必将其作为点场景层发布。点场景层可用于发布具有2000多个特征的点图层，并且是以大量点数流式传输时使用的推荐图层。在之前的一篇博客中，我使用了一个点场景图层，在网站上流下了五十万分。

在Pro中将此点要素类添加到内容窗格的2D部分。就像使用多面体要素类，右键单击该图层，然后单击“共享为Web层”。这将创建一个托管点要素图层。图层完成发布后，将其添加到您配置的建筑物的Web场景中。添加图层后，单击“内容”窗格中的图层启动“配置层”面板，您将看到使用2D或3D样式显示树的选项。您还可以在下拉菜单中使用属性为“计数”和“数量”或“类型”（唯一渲染器）智能映射样式显示2D和3D选项。单击3D对象并单击选项。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/3DPoints-1024x554.jpg)

在这个面板上，我们可以从Esri中提供Web样式，并配置类似的选项，如显示弹出窗口，图例和偏移量。 单击符号，然后选择专题植被，然后选择要用于对图层进行样式化的树。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/ThematicVeg-264x300.jpg)

接下来，我们要使用属性TREE_HEIGHT来驱动场景中树的高度。 树的高度以ft记录，在所选属性旁边的下拉菜单中选择该测量单位。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/TreeHeight-300x66.jpg)
在树层完全配置之后，这是Web场景的外观。 
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/SceneComplete-1024x554.jpg)
### 使用滑块和日光

网络场景中的图层现已完全配置，我们可以继续创建滑块并配置也存储在幻灯片中的日光设置。 要更改日光点击场景查看器右侧的太阳符号，并移动滑块以更改太阳位置。 这将移动太阳的位置，并且您将看到建筑物和树木移动的阴影。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/Daylight-1024x553.jpg)

现在打开“幻灯片”选项，然后单击“创建幻灯片”以捕获幻灯片中的Web场景的此配置。 您可以在创建更多幻灯片时选择不同的位置和不同的时间。 创建幻灯片后，您需要单击“保存”将这些更改应用于网络场景。

随着网络场景准备就绪，您只需点击几下即可创建一个可配置的网络应用程序。 要了解如何从Scene Viewer创建一个网络应用程序，请查看我在几周前发布的[这个博客](https://blogs.esri.com/esri/arcgis/2017/09/11/creating-3d-web-apps-in-arcgis-online-tracking-hurricane-irma/)。

在下面留下任何意见或问题。







