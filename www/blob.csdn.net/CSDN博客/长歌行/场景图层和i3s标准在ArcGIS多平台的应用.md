# 场景图层和i3s标准在ArcGIS多平台的应用 - 长歌行 - CSDN博客





2017年09月18日 13:17:01[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：851
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2016/09/18/i3s-scene-layers/)

作者：Chris Andrews, Pascal Müller, Johannes Schmid, Tamrat Belayneh, Dragan Petrovic, Sud Menon

## 译者注
- 
Chris是Esri的3D产品的总方向的经理，Pascal是Esri苏伊士研发中心负责人，John是Esri苏伊士研发中心高级开发工程师，Tam和Dragan都是Esri本部的服务端与客户端的资深开发工程师，Sud是Esri的CTO，所以本博客作者阵容强大。

- 
译者本人也参与了i3s标准的设计，服务端编写和客户端展示等一系列工作。所以对该标准还是有一定的了解的吗，当然在此只是个人兴趣使然，翻译一下，并不是官方文档或者宣传，算是对个人工作的一个小结。（不知道CSDN平台对GIS的关注度如何，如果关注度高，有可能出一期i3s技术解析方面的文章）

- 
虽然是一年以前的文章，但还是有借鉴意义，而且我会在文章中更新这一年的变化。翻译这篇文章的契机是，i3s标准已经被正式收入OGC了。详见[本文](http://www.opengeospatial.org/pressroom/pressreleases/2639)。

- 
希望在引用的时候能保留作者信息，算是尊重原创，业余时间完成，而且有很多信息需要考据搜集，实属不易；希望保留原始链接，用以追溯历史和在这儿看到最新更新。注明：非技术文档，所以很多地方只是意译。


文章比较长，对于没有时间阅读全文的小伙伴，我在此给一个大致的索引，这样就能在读文章之前了解到，本文是否有你感兴趣的内容。本文大致涉及到的内容如下：
- Esri的3D产品简介，例如ArcGIS Pro， Runtime， SceneViewer。
- i3s数据标准简介，包括数据格式，包含的数据类型，应用场景。

后转至 [知乎](https://zhuanlan.zhihu.com/p/30461274)

**以下是正文。已完成翻译**

2016年Esri用户大会对于我们的用户和合作伙伴来说最大的惊喜之一就是看到了ArcGIS产品在3D跨平台方面的支持。在用户大会上，用户和合作伙伴可以看到，我们推出了一个新数据格式integrated meshes（集成格网？）并推出了相关的发布，共享与可视化功能。使得用户可以在网页端，Runtime系列产品，经典桌面端和企业端浏览和交互操作这些三维格网。Integrated Meshes提供了更多利用无人机，航空器，卫星的数据的方式。

*译者注： 参加用户大会的时候，很多中国用户会问到，Esri的3D产品体系是怎么样的？一般说来，目前的主流三维产品是，Web端，使用由Zurich团队开发的SceneViewer，移动端（或者轻量级的多平台桌面端ios， Android，UWP，WPF，linux，iMac）使用ArcGIS Runtime，桌面端使用ArcGIS Pro*。而i3s主要解决的问题是上述多个平台的**通用**，**高效**的数据格式。

随着I3S（Indexed scene layer，译者注，这种场景图层最为显著的特定是，已经将空间索引固化在数据中，数据的组织方式类似于空间树型的结构）标准的发布。合作伙伴和用户可以自由使用该规范创建出数据或者编写客户端程序来使用该数据。我们希望I3S能够成为一种新的开放标准，其他第三方的数据提供商也能够使用该标准创建或者转换他们的数据以集成到ArcGIS产品体系中。（译者注，一年后的今天，愿望成真。Esri的很多合作伙伴已经使用改标准了）

### 触手可及的I3S

现在，只需要鼠标轻轻一点，就能够揭开场景图层的神秘面纱。使用ArcGIS Earth桌面程序或者其他产品都能轻松访问SceneLayer。在ArcGIS Online上，搜索scene layer就会找到很多示例，并能直接使用SceneViewer打开，当然，在移动端也可以如此操作。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_1a_netherlands_wsv_40.png)

*Figure 1. ArcGIS.com上已经有很多可用场景服务图层了，图中为鹿特丹的建筑群，根据属性渲染出不同颜色*

如果使用ArcGIS Earth，用户可以通过 [Earth’s Add Data interface](https://youtu.be/ou5SN6hDUhg)功能轻松的从ArcGIS Online中浏览场景图层。Online中集成的SceneViewer和ArcGIS Earth都可以用于快速浏览场景图层，讲一段故事，或者开始一系列的分析。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_2_netherlands_earth_40.png)
*Figure 2. R鹿特丹建筑群在ArcGIS Earth中的显示效果*
### WebGIS的重中之重

ArcGIS平台是一个万能系统，能在企业或者组织之间对任意大小，任意复杂度的数据进行创建，修改，部署。改平台能让用户使用web map，web scene， web layer和web apps来浏览数据的各方各面，也能让用户使用桌面端和移动端以适应各种情况。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/2016-October-UpdatedWebGISSlide.png)
*Figure 3. ArcGIS平台的WebGIS体系*
I3S的构想来源是由于三维内容会比二维内容更大，会占用更多的内存，显存以及硬盘空 间。例如，三维建筑物模型就比二维的建筑物底图（footprints）要复杂得多。

在ArcGIS 的Web GIS体系中，基于I3S的场景图层会将场景中的特征（要素feature），几何体，以及属性信息预先构建空间索引并缓存在服务器中，这样在web环境或者本地设备就能快速访问海量的三维数据。场景图层包含了几何体信息，纹理信息，属性信息等，也包含了LOD信息（会根据屏幕分辨率自动切换显示等级），同时也包括了空间坐标系和空间参考。（如球面坐标系，空间直角坐标系和高程坐标系）

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_4_Berlin_WSV_40.png)
*Figure 4. I3S格式的柏林建筑物数据； data credit to City of Berlin, ESRI Germany*
![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_5_vricon_marseille_40.png)
*Figure 5. Integrated mesh scene layer from Vricon shown streamed as a service into ArcGIS Pro*
网络场景和场景图层首次推出是在2014年末，ArcGIS Pro 1.0发布的时候，并同时更新了ArcGIS Online相关内容，所以是Esri生态系统的新成员。

### 探究场景图层

场景图层基于i3s标准。i3s的网络服务包括了几何体，熟悉，纹理等各方面，从而应用程序可以快速显示和分析。从Portal上我们可以看到场景图层对象的很多信息，并可以用于浏览和查询。

i3s是为快速流传输和大缓存的三维内容所设计。在i3s的缓存内部，数据是按照空间树的节点组织的，空间树的形态根据数据的分布特性而有所区别。这种分层次的节点同时也反应了数据从粗糙到精细的细节层次。最上层是粗略的格网和图片，但是到了下层就是很精细的格网模型和纹理了。

每一个i3s节点都包括了对于孩子节点，几何体，属性和纹理的相对（引用）位置。这些数据可能是JSON格式也可以是二进制的形式。他们都构成了负载信息，对于不同能力的设备很有帮助。（例如，移动设备可以在距离目标物体很近的时候，再加载更精细的格网或者纹理，而格网和纹理精细程度的描述就存在于上述的负载信息中，而对于window pc设备，支持纹理压缩的话，可以直接使用DDS的纹理。）

### 多种数据类型

I3S使用不同的配置文件（profile）来区分不同的数据，目前提供了三种数据（目前有四种，发布博客时只有三种）
- 三维物体图层，可以展示大量的三维模型例如建筑物或者车辆。
- 点图层，可以展示二三维的矢量点，支持复合化，可以用图片，模型来作为各个点的符号。
- 集成格网图层，用于表达带纹理的三角格网的数据，一般用于表示地表（注：包括地面附属物，如地面上的道路，房屋，树木，类似于DSM 。或者google earth的photorealistic数据）这种图层一般是从无人机，航空器，卫星影像通过倾斜摄影技术生成的。展示尺度从单个建筑物到整个洲。
- 点云图层，（Chris在写这篇博客的时候，点云图层还在研发过程中，并没有发布。点云图层同样支持LoD，使用Stream方式传输，同样也是预先构建索引。效率很高，目前可以使用SceneViewer和ArcGIS Pro来浏览）

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_13_Tijuana_Estuary_WSV_40.png)

*Figure 6. 在这个场景中，数以万计的植被通过流数据显示出来*

### 灵活的坐标系统

目前i3s支持两种类型的坐标系，一种是平的，一种是球。对于球来说，我们一般使用的是WGS84的球面坐标系。我们也使用一种高程的表现方式，能够反映高程单位和海平面。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_6_portland_local.jpg)
*Figure 7. SceneViewer中展示的一个投影之后的本地建筑物数据*
Usable everywhere through REST interfaces or as Scene Layer Packages

### 随处可用的REST接口或者SLPK

场景层的另一个重要特性是可用使用多种模式交付到ArcGIS平台上的Web，移动和桌面客户端。

大多数用户将使用ArcGIS Online或ArcGIS Server与场景层交互。在这些情况下，场景层的高速缓存位于服务器上，并通过I3S RESTful接口返回给客户端，包括节点及其相关联的资源（几何，属性，纹理）。

一些用户还将场景图层打包为一个可本地存储和访问的场景图层包（SLPK）。目前，ArcGIS Earth和Esri的新版Drone2Map能够查看本地场景层包，ArcGIS Earth可以支持和显示包含数百GB信息的场景图层包。

我们一直在改进场景层包以存储新的层类型，并满足不同移动设备上优化显示和使用的需求。我们期待收到来自我们的合作伙伴和用户社区的反馈。

### 作为典型的GIS用户如何发布场景图层

任何当前使用ArcGIS桌面的许可用户都可以根据I3S规范创建和共享场景图层。所需的最低配置是ArcGIS Pro的当前版本和激活的ArcGIS Online订阅。例如，ArcGIS Pro的用户可以先获取建筑物的底图，然后使用拉伸功能，将底图转换成为三维模型，存储为multipatch格式，最后创建一个SLPK。SLPK实际上就是把场景图层缓存的所有内容，包括节点层次结构都打包为一个单一文件。场景层包可以上传到ArcGIS Online并发布为一个场景层项目，然后就可以在WebSceneViewer，Web AppBuilder，Story Maps和ArcGIS Earth中立即访问。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_7_SF_about_40.png)

*Figure 8. Web AppBuilder中显示的旧金山建筑*

Beyond this basic pattern of using ArcGIS Pro and ArcGIS Online via upfront creation of scene layer packages in ArcGIS Pro, ArcGIS users can also publish 3D information directly to their ArcGIS Server with cache generation occurring on the server.  Desktop users can also use Python to script and automate I3S publishing processes.  Expect to see additional ways to create scene layers within the ArcGIS platform based on the different patterns for creating and using 3D content.  For example, CityEngine recently released the ability to create scene layer packages and we are also exploring the ability to create scene layers from existing GIS feature layers already hosted in ArcGIS Online.

除了上述方式（在ArcGIS Pro中创建SLPK并上传到ArcGIS Online或ArcGIS Pro本地使用）外，ArcGIS用户还可以将3D信息直接发布到ArcGIS Server，并在服务器上生成高速缓存。桌面用户也可以使用Python来编写和自动化I3S发布过程。例如，CityEngine最近发布了创建场景层包的能力，我们也正在探索从已经在ArcGIS Online中托管的现有GIS特征层创建场景层的能力。

### 跨平台使用I3S

场景层设计为由Web，移动和桌面客户端使用。 Esri将基于I3S的场景层的不同应用方式建立了三个不同的实现。这些实现允许客户根据他们的专业知识和期望在其组织中的任何地方使用I3S内容。 ArcGIS Pro除了作为创建3D内容的主要创作工具外，也能使用这些数据。 ArcGIS Pro用户可以连接到ArcGIS Server或ArcGIS Online，查找组织中的私有或公开分享的场景图层，并使用这些图层创建场景，布局甚至动画。场景层可用于丰富的桌面工作流程，例如ArcGIS Pro中高质量的视频创建： 
[YouTube视频地址](https://youtu.be/5YMHjvp0-sA)

The [ArcGIS API for JavaScript](https://developers.arcgis.com/javascript/)也包括从场景层访问和显示3D内容的功能。 JavaScript API随ArcGIS Server和ArcGIS Online一起提供。场景图层可以与Web场景查看器，Story Maps和Web AppBuilder一起使用，所有这些都基于相同的JavaScript API。目前JavaScript访问是只读的，但我们计划实现对功能服务和其他图层类型的读写访问。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_9_custom_js_40.png)
*Figure 9. 场景层与其他GIS信息一起使用在用于城市规划的自定义JavaScript界面​​中*
我们还实现了在ArcGIS Runtime库中使用场景图层的功能。 目前，ArcGIS Earth和Drone2Map是基于[ArcGIS Runtime SDK for .NET](https://developers.arcgis.com/net/)10.2.7版构建，并且是ArcGIS Runtime关于I3S构建的3D应用程序的好例子。开发人员可以探索ArcGIS Runtime，了解如何将场景图层添加到自己的自定义3D应用程序中（包括iOS和Android应用程序）。

ArcGIS Earth和Drone2Map是完全支持使用I3S内容工作的应用程序。具有访问Drone2Map的权限的任何ArcGIS用户都可以处理无人机收集的图像，并生成可以上传并发布到ArcGIS Online的集成网格SPK。 ArcGIS Earth可用于在ArcGIS Online中查看生成的场景图层，或者场景图层包可以作为本地文件直接放入ArcGIS Earth。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_10_D2M_Earth_40.png)
*Figure 10. Drone2Map数据可以作为本地场景层包或作为服务加载到ArcGIS Earth中*
### 内容提供方与I3S

我们将i3s发布为一个开发的标准的原因之一，就是希望其他的数据提供方能够生成ArcGIS平台通用的格式的数据。三维数据格式和数据获取方式层出不穷。提供数据转换软件或者数据标准是GIS行业的刚需。目前我们已经和三个不同的数据提供方进行了合作，Vricon，Bentley和Pix4D。

**Vricon**

[Vricon](http://bit.ly/VriconLink) 使用卫星影像和机器学习技术创建高精度，大面积的带纹理的地球表面的格网数据。一个数据块就有1000平方公里，数十GB大小。Vicon同时也需要一种数据格式，能够轻松的将他们的数据应用到GIS市场中。在I3S的帮助下，Vricon可以将它的数据转换为集成格网数据包并部署于ArcGIS Online上。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_11_Earth_Vricon_40.png)
*Figure 11. 使用ArcGIS Earth展示的1000平方公里的优胜美地数据*
**Pix4D**

[Pix4D](https://pix4d.com/) 创建或者授权将图片转换为点云或者带纹理格网的软件。

**Bentley Systems**

[Bentley Systems’ContextCapture](Bentley%20Systems%E2%80%99%20ContextCapture) 在今年也引用了我们的标准。 ContextCapture是一个能将海量多传感器说去到的影像数据转换为细节纹理格网的软件。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2016/09/i3s_12_bentley_orlando2_40.png)
*Figure 12. Bentley System’s ContextCapture puts the power of creating meshes from imagery into the hands of the user at very high volumes; shown here in a web scene*
希望有更多的合作方来了解并在他们的产品中支持i3s。

### 更多资源

对于用户来说，了解i3s的最佳途径是从ArcGIS Online上搜索相关数据，或者是下载ArcGIS Earth并添加场景图层。我们在这个[博客](http://j.mp/EarthBeta3DContent2)中分享了很多数据集。
- 
[One million 3D buildings](http://www.arcgis.com/home/webscene/viewer.html?webscene=85b82116f43b41609163facfaf9420db) from New York City

- 
[Phototextured San Francisco](http://www.arcgis.com/home/webscene/viewer.html?layers=4dac5135a4ff402cb98224c8011b139b) 3D buildings from [Pictometry](http://www.eagleview.com/) and [PLW Modelworks](http://plwmodelworks.com/)

- 
[Vricon integrated mesh](http://www.arcgis.com/home/webscene/viewer.html?layers=1f97ba887fd4436c8b17a14d83584611) example

- 
[Bentley Systems integrated mesh](http://www.arcgis.com/home/webscene/viewer.html?layers=423ead7f6deb48e9a74535e130e0624e) example

- 
[Geodesign story map](http://esrigeodesign.maps.arcgis.com/apps/MapJournal/index.html?appid=093e239a59714106b505646f139e85cc)

- 
[South Boston Zoning code story map](http://citygov.maps.arcgis.com/apps/MapJournal/index.html?appid=544f6a16efbc4e73a398a4102512d95a)

- 
[Manhattan Air Rights viewer](http://nyc3d-portal-ms.esri.com/NYCAirRights/) – custom JavaScript app


对于数据提供方或者是对技术细节感兴趣的用户，我们同样上传了[i3s标准的文档](http://j.mp/I3S-spec)。希望你们能给予意见和建议，这样我们就会进一步了解用户的需求并作出改进。

### 下一步的计划

虽然我们目前着重于i3s跨平台的显示以及部署能力，但将来，我们计划引入更多新的数据类型，并在现有的场景图层上逐步加入更多的分析，管理和可视化功能，三维GIS市场正在飞速发展，我们会让i3s支持更多新技术，更完善的流程，更新的交互方式，例如虚拟现实，增强现实和三维打印。

原博客最早发布与，2016年9月14日。












