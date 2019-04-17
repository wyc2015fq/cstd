# 在ArcGIS Pro 2.1中发布点云场景图层 - 长歌行 - CSDN博客





2018年04月04日 08:34:54[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：307
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









原作者：Sean William Morrish 

原文地址： [Publishing Point Cloud Scene Layers in ArcGIS Pro 2.1](https://blogs.esri.com/esri/arcgis/2018/04/02/publishing-point-cloud-scene-layers-in-arcgis-pro-2-1/)

翻译：iuhsihsow 

译文地址：[在ArcGIS Pro 2.1中发布点云场景图层](https://blog.csdn.net/uninterrupted/article/details/79811935)
自ArcGIS Pro 1.4发布时，将各种格式的三维数据从ArcGIS Pro中发布为i3s格式成为可能。（I3S scene layer格式已经成为OGC的社区格式标准之一）最早的时候，只能在ArcGIS企业版发布三维对象mutipatch格式的要素数据，后来支持的格式也扩充到三维对象（multipatch），点，集成格网（Drone2Map， Pix4D， ContextCapture， VRICON），再后来，ArcGIS Pro 2.0中支持将LAS， ZLAS和LAZ格式的点云数据发布为场景图层。

本博客将重点介绍将点云数据转为I3S点云场景图层的组织，发布，管理过程。从最初收集LiDAR点云数据，然后生成供ArcGIS Pro本地使用的point cloud scene layer package（SLPK）或是作为一个场景图层发布到ArcGIS Online上。

如果你没有自己的点云数据（LiDAR，LAS，ZLAS或是LAZ格式）也关系。政府和非盈利性组织也会提供大量免费的LiDAR点云数据。

基于[加州大学圣地亚哥分校](http://www.ucsd.edu/) 的 [圣地亚哥超级计算机中心](http://www.sdsc.edu/) 的[OpenTopography](http://opentopo.sdsc.edu/datasets) 提供了大量高分辨率地球科学类的LiDAR数据。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/OpenTopo.png)

美国[Interagency Elevation Inventory](https://coast.noaa.gov/inventory/) 为给用户提供了美国许多地区的各种LiDAR，IfSAR和Bathymetry点云数据的访问权限。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/InterAgency_LiDAR.png)

数据集：[2014 USGS CMGP Lidar: Post Sandy (New York City)](https://inport.nmfs.noaa.gov/inport/item/49891) 也是Interagency Elevation Inventory提供的免费数据集之一。该数据也是USGC飓风Sandy在大西洋海岸沿岸帮助评估风暴损害和当地环境侵蚀资料的一部分。 我们也将使用这个数据集来处理和发布点云数据。

打开Interagency elevation inventory的网址（上面已提供链接），我们将会被转向到美国国家海洋和大气管理局数据访问视图。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/NOAA_DATA_Viewer.png)

选择纽约附近区域(New York vicinity)， 并下载相关的LiDAR数据与元数据。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/USGS_PostSandy_AOI.png)

数据下载完成之后，你将拥有一个包含510个LAZ数据文件的24GB文件夹。你现在可以将其直接转换为点云场景图层，或这如果你想进行进一步的过滤、分类或者分析的话，可以将它们转换为LAS格式，以便在ArcGIS Pro中进行处理和分析。

要将LAS， ZLAS或者LAZ点云数据转换为 PCSL或者是SLPK的话。先打开ArcGIS Pro，在geoprocessing选项卡中找到“Create Scene Layer Package”，并将包含LAZ文件的文件夹拖入input layer对话框，选择一个output folder并定义需要缓存的属性字段。最后选择输出坐标系或者使用默认的GCSWGS1984。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/LAZ2SLPK.png)

除了可以自定义点渲染类型（Elevation，Classification，Intensity或者RGB Color）外，这个过程和从LAS数据集输出为PCSL SLPK类似。将LASD数据集加入地图中，然后作为输入图层添加到“Create Scene Layer Package”中。在输入所有参数后，点击运行，数据将被发布为单个SLPK。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/LASD2SLPK.png)

在生成SLPK之后，可以使用“Map”选项卡上的添加数据按钮，将其添加到场景中，以便在ArcGIS Pro中查看。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/SLPK_ADDDATA.png)

如果想将SLPK发布到ArcGIS Enterprise或者ArcGIS Online，可以使用 Share Scene layer package tool。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/SHARE_PACKAGE.png)

填写摘要说明和标签。这些标签将用户帮助用户搜索对象以及包含了数据来源，标识的描述信息。

一旦功效到ArcGIS Online，即可使用Scene Layer Package properties页面的发布按钮来进行发布。发布的场景图层包可以在你的组织内部或者公众中共享。

在发布场景图层时，切记添加数据详细的描述信息以及源许可。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/PUBLISHED_SLPK.png)

现在，你可以将该点云数据连同其它二三维在线数据加入到Web Scene中了。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/PCSL_WSV1.png)

或者在ArcGIS Pro中使用这类场景图层服务和本地数据。

![img](https://blogs.esri.com/esri/arcgis/files/2018/04/PCSL_PRO.png)

上述方法只是分享和发布超大型LiDAR点云数据集的所有方法中的一部分。如想查看点云场景图层的其他示例，请查看ArcGIS Online上的3D Scene Layer Group。




