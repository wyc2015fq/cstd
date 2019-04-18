# ArcGIS Online上场景图层的性能提升 - 长歌行 - CSDN博客





2018年02月28日 15:34:30[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：314
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









原文地址: [Scene Layer Performanc on ArcGIS Online](https://blogs.esri.com/esri/arcgis/2018/02/13/scene-layer-performance-optimizations/)

by Simon Reinhard on February 13, 2018

您可以使用基于OGC®I3S社区标准的3D对象图层和集成网格场景图层，直接在浏览器中制造引人注目的可视化效果。我们正在使用各种技术在这个受限的平台上让用户的获得更平滑的视觉和操作体验。 在本文中，我将介绍我们在2017年12月发布的ArcGIS Online和ArcGIS API for JavaScript版本4.6中引入的三项改进：
- IndexedDB缓存：一旦3D数据被下载并处理，我们将其缓存在IndexedDB中，以便我们可以更快地重新加载它。
- 流式传输：我们优化了i3s节点和数据的并行流式传输，使场景层以更高的优先顺序加载。
- 渐进式加载：现在集成的网格场景图层首先下载一个中等细节层次（LOD），然后细化为更详细的LOD。

![img](https://blogs.esri.com/esri/arcgis/files/2020/12/Helsinki-1024x588.jpg)

## IndexedDB缓存

场景层已经有很多缓存级别：数据缓存在CDNs和客户端，在那里Web浏览器会自动缓存所有请求。但是，场景图层的几何数据通常在地理坐标系中，我们需要将其转换为以地球为中心的全球场景坐标系(译注：客户端展示的场景图层按坐标系、投影方式分为 global scene 和local scene两类）。此外，为了获得最佳的渲染性能，我们希望将数据转换为具有交错位置，纹理坐标，颜色等的GPU友好格式。

理想情况下，我们将保留GPU上的所有几何数据并仅渲染可见部分。但是，GPU内存非常有限，我们必须删除不可见的节点。为了避免在显示我们以前看过的场景图层的部分时重复这些转换，我们将几何数据以我们可以直接上传到GPU的格式存储在一个额外的缓存级别中。该缓存使用IndexedDB API来存储临时数据，因此我们将其称为IndexedDB缓存。下图说明了客户端上的缓存层次结构：

![img](https://blogs.esri.com/esri/arcgis/files/2020/12/IndexedDBcache.png)

该流程图的末端是我们向服务器发送请求的位置。 如果您对接下来发生的事情有更多的兴趣，请查看Paul关于ArcGIS Online缓存的优秀博客。([Paul’s excellent blog on caching in ArcGIS Online](https://blogs.esri.com/esri/arcgis/2017/12/12/scalable-hosted-feature-layers-in-arcgis-online-tile-queries-and-response-caching/))

## 流式传输

要创建最佳用户体验，在正确的时间加载正确的数据非常重要。 位于屏幕中心或查看器前方的数据应在位于屏幕边界或背景的数据之前加载。 但是，网络延迟使得使用连接的全部带宽变得困难，并且得保持严格的请求顺序。 下图显示请求的串行排序由于请求的延迟而导致一些带宽浪费（以绿色显示）。 另一方面，并行请求使用全部可用带宽，但不保留请求的顺序：

![img](https://blogs.esri.com/esri/arcgis/files/2020/12/Requests.png)

并行请求的最佳数量大致取决于三个参数：网络延迟，网络带宽和请求数据的大小。 在i3s服务中有两个主要资源：
- 索引节点：空间索引，它告诉我们服务数据的位置。 这包括许多小的请求。
- 几何数据：我们想要在屏幕上显示的数据。 这些通常很少但非常大的请求。

对于这两种不同类型的资源，需要不同的策略。 对于小型索引节点，并行请求多个节点以隐藏网络延迟并优化使用带宽非常重要。 另一方面，对于大型几何数据，连接带宽容易饱和，但并行请求很多会使单个节点加载速度变慢。

在这个版本中，我们单独调整了这两种请求的参数，以便最大限度地利用您的带宽，同时仍然保持我们绘制数据的顺序接近最佳顺序。 作为这些改进的结果，您可以看到在以下视频中加载顺序非常接近最佳顺序（从前到后）：



译注：原视频在YouTube上[https://youtu.be/rlyiHcb0Gew](https://youtu.be/rlyiHcb0Gew)，由于部分读者可能不方便观看，我就截图说明下。 
![img](https://or45huni1.bkt.clouddn.com/18-2-28/81937737.jpg)
加载数据之后，第五秒，“After”已经加载好所有数据，等到第十六秒的时候，“Before”才加载完所有数据，并且加载的时候，一会儿加载远处数据，一会儿加载近处数据。

## 渐进式加载

有时候，尽快显示稍微更多的数据，但要快速地显示较不详细的数据版本要比等待细节加载更好。我们的详细程度系统基于屏幕分辨率，因此我们可以模拟较低的分辨率，并首先下载我们在此情况下显示的所有节点。这使我们能够快速浏览该图层，然后我们可以开始使用更详细的数据来优化显示。

这两个细节层次之间的加载顺序是另一个有趣的话题。一般来说，我们希望首先下载低细节几何图形，但是，我们希望在显示器中心开始细化数据，同时仍然下载低详细背景节点。要做到这一点，我们根据当前视图的比例向节点排序优先级添加一个基于到感兴趣点的距离的罚分。

我们在ArcGIS Online上通过各种场景图层对其进行了测试，并得出结论，这对于集成的网格场景图层来说是一个重大改进，可以在以下视频中看到：



译注：[https://youtu.be/n4RoEs2ucTA](https://youtu.be/n4RoEs2ucTA)依旧截图，从图中可以看到，前后差异很大，后者数据的加载和渲染更加平滑和迅速。

![img](https://or45huni1.bkt.clouddn.com/18-2-28/66544481.jpg)

在此视频的右上角，您可以看到，即使当前视图中我们尚未拥有缓存，渐进式加载也会非常快速地下载场景内容的概览。虽然总加载时间稍长于左侧的非渐进加载，但只能在背景中注意到差异，总体而言，逐渐加载感觉快得多。(译注：划重点：感觉）

## 下一步

我希望您喜欢ArcGIS Online中的3D场景图层和ArcGIS API for JavaScript的性能。我期待着能够真正展示您的数据的许多web场景和应用程序。我们仍然有许多机会可以做得更好，这是我们将继续努力的一个令人激动的话题。 请继续关注下一个版本，以了解这方面的更多改进。

截图或视频：
- [3D model of Helsinki](http://www.arcgis.com/home/item.html?id%3D5ecba5273b2d41ff9f6f1eb33f238d18) by [City of Helsinki](http://www.hri.fi/en/dataset/helsingin-3d-kaupunkimalli) | [Terms of use](http://creativecommons.org/licenses/by/4.0/deed.fi)
- [NYC 3-D Building Model](http://www.arcgis.com/home/item.html?id%3Dc4054b475e60472f8d24585287ceebca) by [NYC DoITT](http://www1.nyc.gov/site/doitt/initiatives/3d-building.page) | [Terms of use](http://www1.nyc.gov/site/doitt/initiatives/3d-building.page)
- [Yosemite Valley](http://www.arcgis.com/home/item.html?id%3D1f97ba887fd4436c8b17a14d83584611) by [VRICON](https://www.vricon.com/) | [Terms of use](http://www.arcgis.com/home/item.html?id%3D5618776e04be41a68b349b8751a9cdb6)




