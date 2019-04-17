# 项目迁移：从ArcGIS Runtime WPF SDK 10.2.x到100.x - 长歌行 - CSDN博客





2018年03月08日 15:55:15[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：288
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









随着ArcGIS Runtime SDK for .NET(WPF) 10.2.x的版本更新，我们看到了较以往而言，性能更强，功能更稳定的SDK（可参考[ArcGIS Runtime SDK 100.2的新增功能](http://blog.csdn.net/uninterrupted/article/details/78855556) 等相关博客），项目迁移也是势在必行。

ArcGIS Earth的1.6版是基于10.2.x版本开发的，在之前的一段时间，ArcGIS Earth team致力于将ArcGIS Runtime SDK升级到100.x。结合当时的开发经历和官方的开发文档，在此大概说下，两者的区别和迁移事的注意事项。在此也非常感谢Earth组团队成员在写作和审阅本文时提供的帮助。

# SDK安装配置的变化
- 目前你可以在VS的`manage NuGet for your solution`中搜索“Esri”，查找到”Esri.ArcGISRuntime.WPF”并添加到自己的项目中：

![img](https://developers.arcgis.com/net/latest/wpf/guide/GUID-D30BB356-DB54-4E0A-A39A-BCEA10454D36-web.png)

当然，如果网络状况不佳的话，也可以使用VSIX来本地安装包：

![img](https://developers.arcgis.com/net/latest/wpf/guide/GUID-29A4135B-C40E-4313-BCEF-6B5F06326402-web.png)
- 
目前仅支持.NET Framework 4.5.2及以后的版本。（注意100.3将使用4.6）

- 
部署更容易，不需要`ArcGIS Runtime .NET Deployment Manifest`了，配置好NuGet包之后，会自动将runtime的必要组件给复制到你工程中，例如：


![img](https://developers.arcgis.com/net/latest/wpf/guide/GUID-FD8EB238-6B9A-4107-8AD2-66F35A0D3097-web.png)

# 功能性的变化

由于对底层设计有重大更新，目前以前版本的一些功能还在开发测试中，而且现有功能的实现方式也有一些变化，具体体现在以下方面：

## Maps

10.2.7中的`Map`和`Scene`都在control中，但是100.x为了让设计更符合MVVM的模式，将他们转移到Mapping中。下图为10.2.x的类图：

![img](https://or45huni1.bkt.clouddn.com/18-3-8/74735831.jpg)

但是就开发来说，最直观的影响还是命名空间的改变 O(∩∩)O

## Views

对于视图类来说，100.x的MapView和SceneView有一个共同的基类GeoView，注意，GeoView中是没有Scene或者Map的对象的，所以如果我们开发的时候使用GeoView而非特指二维或者三维，就可以轻易的进行二三维切换。（当然，实际运作中，还要修改交互等代码）

## Loadable

`ILoadable`接口是100.0.0开始引入。由于现在的数据分布多样化，数据种类各异，使用loadable可以更好的保证数据操作的一致性。目前需要写代码让数据加载，好处是可以自由控制数据的加载状态了，例如，我们想获取数据的详细信息，但是不想在视图窗口显示这个数据，现在，我们就可以先让它加载，但是不把这个数据放到视图中。而且使用这个接口可以判断数据加载是成功还是失败，有利于我们下一步的操作。下面是程序内部的数据加载逻辑：

![img](https://developers.arcgis.com/net/latest/wpf/guide/GUID-239313DE-D183-409E-BB29-23942256EABE-web.png)

相关代码示例：

```
// get a portal item (using its ID)
var webMapItem = await PortalItem.CreateAsync(myPortal, portalItemID);


// create a new map from the item, load the map
var myMap = new Map(webMapItem);
await myMap.LoadAsync();


// make sure the map loaded
if (myMap.LoadStatus != LoadStatus.FailedToLoad)
{
    // add the map to the map view (dispatch to the UI thread)
    await Dispatcher.InvokeAsync(() => MyMapView.Map = myMap);
}
```

一般情况下，我们都是在数据加载完毕之后，才设置数据的属性，但是对于这种loadable的数据，我们可以在loading结束之前赋值。例如：

```
// create a new layer, set min/max scale
var censusLayer = new ArcGISMapImageLayer(new Uri("http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer"));
censusLayer.MinScale = 5000;
censusLayer.MaxScale = 1000000;


// handle the Loaded event
censusLayer.Loaded += (s, e) =>
{
    // after loading, min/max scale should have values provided above
    Debug.Assert(censusLayer.MinScale == 5000);
    Debug.Assert(censusLayer.MaxScale == 1000000);
};


// load the layer
await censusLayer.LoadAsync();
```

上面的例子中，loading结束之前的赋值是成功的。当然，如果你想获取一个featureLayer的feature的话，还是需要等待这个featureLayer加载结束的。

## Graphics overlays and graphics

一般GIS系统中绘制的点线面，如图钉，量测线都属于Graphics并且要求在视图的顶端显示。在10.2.x版本中，我使用`GraphicsLayer`来存放这些图形元素，但是新的版本中已经取消了这个类，取而代之的是`GraphicsOverlay`。这个变化估计对开发者来说有一定的影响，毕竟新旧两个类还有有一定的差距的。不过为了以后能支持更多的符号和渲染方式，目前就忍忍吧。下面的代码简单说下`GraphicsOverlay`如何使用：

```
// create a new graphics overlay
var graphicsOverlay = new GraphicsOverlay();

// add the overlay to the map view's graphics overlay collection
myMapView.GraphicsOverlays.Add(graphicsOverlay);
```

## Feature tables

主要是ServiceFeatureTable中有改变，请求和缓存数据的方式更加清晰，具体的还是在开发中看接口文档吧。

## Identify features and graphics

这部分从代码层面和实际应用的交互表现方面都有变化。首先是之前在FeatureLayer和GraphicsOverlayer中的`HitTestAsync`方法被和谐了，得在GeoView中使用下面的方式来进行identify：
- IdentifyGraphicsOverlayAsync - Identify graphics from a single GraphicsOverlay in the view.
- IdentifyGraphicsOverlaysAsync - Identify graphics from all GraphicsOverlay objects in the view.
- IdentifyLayerAsync - Identify features from a single Layer in the view.
- IdentifyLayersAsync - Identify features from all Layer objects in the view.

我之前的这篇博客中也略加使用：[ArcGIS Runtime WPF SDK （4）绘制，编辑Graphic](http://blog.csdn.net/uninterrupted/article/details/78143020)

## Geoprocessing

100.x中使用`GeoprocessingTask`来做地理数据处理。使用Task，job等进行操作。

## Hydrography

去这儿获取更多信息吧：[Esri.ArcGISRuntime.Hydrography](https://developers.arcgis.com/net/latest/wpf/api-reference?N_Esri_ArcGISRuntime_Hydrography.htm)

## Offline

作为一个本地的应用程序，对于离线功能的支持是不可或缺的。这是是100.x的一个重要内容。 使用ArcGIS Pro创建的mmpk（移动平台的地图包）和`MobileMapPackage`就可以进行离线的各种操作了。当然，和之前的版本一样，当前版本也支持使用task，job的方式来离线操作feature layer和tiled layer。需要使用`GeodatabaseSyncTask`。具体的例子还是看官方教程吧。 [Edit features](https://developers.arcgis.com/net/latest/wpf/guide/edit-features.htm)

## Authentication

目前使用`AuthenticationManager`，功能基本没变。

## Protal

目前使用`PortalItem`或者`Uri`来表示web map。不使用webmap，webmapviewmodel来加载portal中的webmap了。

## Error handling

现在错误代码更全面，更规范，而且会指明错误发生在哪个模块或者系统中。具体见： [Platform error codes](https://developers.arcgis.com/net/latest/wpf/guide/platform-error-codes.htm)

## 其他的一些变化

KML目前还不支持，仍在继续开发中。

其实更多的变化体现在名称和命名空间的改变，我在这儿摘录下:

#### 命名空间的改变：
- Esri.ArcGISRuntime.Layers no longer exists. The classes found here at 10.2.x are now in Esri.ArcGISRuntime.Mapping.
- Esri.ArcGISRuntime.Controls no longer exists. Classes like MapView and SceneView are now in Esri.ArcGISRuntime.UI.Controls. Others, such as GraphicsOverlay and map grids are now in Esri.ArcGISRuntime.UI. Things like Map and Camera have been moved to the new Esri.ArcGISRuntime.Mapping namespace.
- Esri.ArcGISRuntime.Symbology.SceneSymbology no longer exists. The corresponding 100.x classes are in Esri.ArcGISRuntime.Symbology.
- Esri.ArcGISRuntime.WebMap no longer exists. Many of the classes found here at 10.2.x have been moved to Esri.ArcGISRuntime.Mapping and Esri.ArcGISRuntime.Mapping.Popups.
- At 10.2.x, the classes for working with Electronic Navigational Charts (ENC) are in the Esri.ArcGISRuntime.Hydrographic namespace. In 100.2.0, you’ll find them in Esri.ArcGISRuntime.Hydrographic (as well as an EncLayer in Esri.Mapping).

#### 类的改变：
|10.2.x| |
|----|----|
|| |
|ArcGISTiledMapServiceLayer|ArcGISTiledLayer|
|ArcGISLocalTiledLayer|ArcGISTiledLayer|
|SceneLayer|ArcGISSceneLayer|
|IdentityManager|AuthenticationManager|
|Layer.ID|Layer.Id|
|Layer.DisplayName|Layer.Name|
|ArcGISTiledMapServiceLayer.ServiceUri(string)|ArcGISTiledLayer.Source (Uri)|
|ArcGISLocalTiledLayer.Path (string)|ArcGISTiledLayer.Source (Uri)|
|| |
|Events: MapViewTapped, MapViewDoubleTapped, and so on.|These events are now defined on the GeoView base class: GeoViewTapped, GeoViewDoubleTapped.|
|MapView.ExtentChanged|GeoView.ViewPointChanged|
|Extent (Envelope)|VisibleArea (Polygon)|
|Editor|SketchEditor|
|DrawShape enum (defines shape type for Editor to draw).|SketchCreationMode|
|MinScale, MaxScale|These properties are now defined on the Map: Map.MinScale, Map.MaxScale|
|WrapAround (bool)|WrapAroundMode (WrapAroundMode enum)|
|LayerLoaded|LayerViewStateChanged|
|| |
|ArcGISPortalItem|PortalItem|
|ArcGISPortalUser|PortalUser|
|ArcGISPortalGroup|PortalGroup|
|QueryParameters       SearchParameters|PortalQueryParameters|
|SearchResultInfo|PortalQueryResultSet|
|ArcGISPortal.CurrentUser|ArcGISPortal.User|
|ArcGISPortal.SearchItemsAsync|ArcGISPortal.FindItemsAsync|
|| |
|3D Marker symbols, such as BoxMarkerSymbol, SphereMarkerSymbol, and so on.|Use SimpleMarkerSceneSymbol and set the Style using a value from the SimpleMarkerSceneSymbolStyle enum.|
|ClassBreaksRenderer.Field|ClassBreaksRenderer.FieldName|
|ClassBreaksRenderer.Infos (ClassBreakInfocollection)|ClassBreaksRenderer.ClassBreaks (ClassBreak collection)|
|| |
|Scene.Surface|Scene.BaseSurface|
|ServiceElevationSource|ArcGISTiledElevationSource|
|FileElevationSource|RasterDataSource|
|SceneView.ElevationExaggeration|Surface.ElevationExaggeration|

参考： 

- [Migrate to 100.x from 10.2.x](https://developers.arcgis.com/net/latest/wpf/guide/migrate-to-100-x-from-10-2-x.htm)

- [10.2.7 Class digrams](https://developers.arcgis.com/net/10-2/desktop/guide/class-diagrams.htm)



