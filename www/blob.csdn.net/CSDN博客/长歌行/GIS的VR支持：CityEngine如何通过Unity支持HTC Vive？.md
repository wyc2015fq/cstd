# GIS的VR支持：CityEngine如何通过Unity支持HTC Vive？ - 长歌行 - CSDN博客





2017年09月28日 11:06:01[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：738
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/09/12/86150/)

作者：Markus Lipp 

时间：2017年9月12日
Esri CityEngine是将城市模型和其他3D GIS内容引入虚拟现实（VR）的好工具。它允许您为手机（见[ArcGIS 360 VR](https://blogs.esri.com/esri/arcgis/2017/07/09/cityengine-unveils-mobile-virtual-reality-solution/)）和台式PC创建VR体验。

在这篇博文中，我们将逐步描述如何快速将3D数据导入桌面VR头戴式设备，如HTC Vive。为了在VR中的3D环境中进行交互和导航，我们还将展示如何简单地设置手动控制器和传送功能。

作为3D数据集的示例，我们使用CityEngine City Wizard创建的虚拟城市，该数据可以通过欢迎屏幕或通过 *File menu – New – City*向导轻松访问。

### 导出你的城市环境

作为第一步，选择城市的中心部分，然后转到*File – Export Models*。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/0-export.jpg)

Autodesk FBX在CityEngine和Untiy中都很出色。在下一个对话框中，您可以调整一些参数。 *Simpify Terrain Meshes*用于减少平坦区域中的三角形数量，对于良好的性能来说很重要。还可以选择 *Create Texture Atlases*，这样可以减少绘制次数。（*译者注：将多个纹理打包为单一纹理，并通过纹理坐标来访问对应区域可以减少底层渲染引擎的消耗*）您可以进一步选择*Embed Textures*“以更容易地在Unity使用这些数据。您可能需要向下滚动才能找到这些选项。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/09/1-export-settings.png)

### 导入到Unity

所得到的FBX文件可以在CityEngine Navigator的’*models*‘文件夹中找到。将此文件拖放到空的Unity场景的资源文件夹中。

![拖放](https://blogs.esri.com/esri/arcgis/files/2017/09/2-dragdrop.jpg)

然后将城市拖动到“*Hierarchy* ”视图中。它也将出现在3D场景视图中。双击*Hierarchy*视图中的城市，然后放大一些。现在，我们来调整相机的位置。选择*Main Camera*并使用移动工具将其放置在街道上。小*Camera Preview*会有帮助的。

![城市拖网](https://blogs.esri.com/esri/arcgis/files/2017/09/88-city-dragdrop.png)

### 设置VR

要启用VR，请转到*Edit – Project Settings – Player*，并勾选检查器的*Virtual Reality Supported*。

![VR SDK设置](https://blogs.esri.com/esri/arcgis/files/2017/09/89-vr-sdk-setup.png)

默认情况下只添加Oculus SDK，但这不适用于HTC Vive。要添加Vive支持，请单击小的“+”符号，然后单击*OpenVR*。 OpenVR可以适用于Oculus和HTC Vive。将OpenVR拖到Oculus前面，或者简单地删除Oculus。 

按3D视图上方的*Play* 按钮。如果SteamVR和VR眼镜安装正确，将显示一个小的SteamVR弹出窗口，城市将显示在VR中。如果在SteamVR弹出窗口中出现警告，请参阅HTC Vive或Oculus Rift设置说明以寻求帮助。

![SteamVR接口](https://blogs.esri.com/esri/arcgis/files/2017/09/90-steamvr.png)

### 添加移动

我们现在有一个与初始相机定位的场景。如果你有足够大的房间，你可以自身走动一下去探索街道。但是，您如何浏览更大的区域？

传统上，3D游戏使用控制器输入实现连续运动。然而，在VR中，许多用户会引起不适，因为移动的视觉刺激与静止的感觉不符。

一个解决方案是使用传送移动。用户直接在不同的位置之间切换，而不是连续旅行。幸运的是，这在Unity中很容易实现。首先，单击*Window – Asset Store*。可能会要求您登录，然后搜索SteamVR插件，然后单击*Import*。在“*SteamVR Settings*”对话框中，单击“*Accept All*”。

![SteamVR插件](https://blogs.esri.com/esri/arcgis/files/2017/09/91-steamvr.png)

将文件夹“SteamVR / InteractonSystem / Core / Prefabs”中的“Player prefab”拖放到“*Hierarchy*”中。就像我们之前使用*Main Camera*一样，将其移动到3D场景中的一个不错的位置。然后删除主相机，因为Player prefab已经包含相机。

![拖放机](https://blogs.esri.com/esri/arcgis/files/2017/09/92-setup.png)

现在，将一些来自SteamVR / InteractonSystem / Teleport / Prefabs的TeleportPoint prefabs拖放到3D视图中，并将Teleporting预制拖放到Hierarchy*中。为了更方便地在更大的场景中传送，您可以增加传送参数中的弧距。

![设置传输点](https://blogs.esri.com/esri/arcgis/files/2017/09/93-setup.png)

当您启动场景时，您现在可以通过点击Vive控制器并传送到传送点上的绿色路径来传送到这些位置。
![万岁](https://blogs.esri.com/esri/arcgis/files/2017/09/94-vive.png)

运行起来，它与Oculus Touch一样的出色。

![Oculus Touch](https://blogs.esri.com/esri/arcgis/files/2017/09/95-oculus-touch.png)

您还可以添加传送区域：双击3D视图中的街道，然后创建副本。单击检查器中的*Add Component*，并添加传送区域以及*Mesh Collider*。

![传送设置](https://blogs.esri.com/esri/arcgis/files/2017/09/96-teleport-setup.png)

现在你可以在VR中这条街上的任何地方传送。

![传送Vive](https://blogs.esri.com/esri/arcgis/files/2017/09/97-teleport.png)

### 调整场景

你可能会注意到树木看起来有点怪异。要修复此问题，请单击*Assets/Materials*，然后选择所有绿色树材料。然后在检查器中选择Cutout作为渲染模式。这将从透明部分中删除高亮部分。

[拧](https://blogs.esri.com/esri/arcgis/files/2017/09/99-tweak.png)

### 结论

在CityEngine和Unity中使用几个简单的步骤和预置功能，您可以创建引人入胜的VR体验。通过添加额外的插件，如SteamVR运动系统，可以改进体验。

您可以添加进一步的简单调整来提高视觉质量：通过在Unity中添加屏幕空间环境遮挡（SSAO）提高对细节的深度感知。 Unity还为现实阴影提供了全球照明解决方案。

在VR中显示更大的城市时，重要的是要注意表现。 理想情况下，它应该运行90fps，以达到舒适的VR体验。 为了提高性能，可以在Unity中启用*Single-Pass*立体渲染。 此外，CityEngine的基于规则的性质可以为几何形状创建不同的细节级别，例如将较少的细节添加到远处的建筑物中。 由于Unity的多平台特性，很容易在多个平台上部署VR体验。 例如，它与HTC Vive和Oculus Rift兼容，还可以为Windows全息照相机添加Microsoft Hololens支持。 我们将进一步完善这一工作流程，使GIS2VR在未来更加容易。 你有兴趣看到为Unreal Engine等提供的指南吗？ 我们很乐意收到到您的反馈。

*译者注：Markus属于苏黎世研发中心，在I3S的设计和实现过程中，也曾和他远程合作过一段时间。*





