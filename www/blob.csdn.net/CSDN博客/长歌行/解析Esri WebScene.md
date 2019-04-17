# 解析Esri WebScene - 长歌行 - CSDN博客





2018年04月11日 22:34:43[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：617
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









## 前言

WebScene是符号化的3D地理空间内容，可以用于在直观的交互式3D环境中显示并分析地理信息。相对于传统的Geodatabase或者其他栅格，模型数据而言，更加利于分享和多平台显示。

示例如下：（新版CSDN好像嵌入不了网页了，大家可以自己嵌入在前端展示，或者浏览代码中的链接。）

[https://arcg.is/0bbK45](https://arcg.is/0bbK45)

```xml
<iframe width="500" height="400" frameborder="0" scrolling="no" allowfullscreen src="<https://arcg.is/0bbK45>"></iframe>
```

那么它究竟包括哪些内容呢？

![img](https://i.imgur.com/A7dLaM6.png)

从界面来看，大体上包括，Layer，Basemap，Ground，Slides(Presentation)

其实如果想知道更具体的内容，需要看WebScene的spec了。

## WebScene spec

链接：[https://developers.arcgis.com/web-scene-specification/](https://developers.arcgis.com/web-scene-specification/)

里面主要列举了WebScene可以包含的图层格式，底图的定义方法等，如果需要解析WebScene的JSON的话，有一定的参考意义。

### operationalLayers
- [CSV Layer (CSV)](https://developers.arcgis.com/web-scene-specification/objects/csvLayer/)
- [Feature Layer (ArcGISFeatureLayer)](https://developers.arcgis.com/web-scene-specification/objects/featureLayer/)
- [Group Layer](https://developers.arcgis.com/web-scene-specification/objects/groupLayer/)
- [Image Service Layer (ArcGISImageServiceLayer)](https://developers.arcgis.com/web-scene-specification/objects/imageServiceLayer/)
- [Integrated Mesh Layer (IntegratedMeshLayer)](https://developers.arcgis.com/web-scene-specification/objects/integratedMeshLayer/)
- [Map Service Layer (ArcGISMapServiceLayer)](https://developers.arcgis.com/web-scene-specification/objects/mapServiceLayer/)
- [PointCloud Layer](https://developers.arcgis.com/web-scene-specification/objects/pointCloudLayer/)
- [Scene Layer (ArcGISSceneServiceLayer)](https://developers.arcgis.com/web-scene-specification/objects/sceneLayer/)
- [Tiled Image Service Layer (ArcGISTiledImageServiceLayer)](https://developers.arcgis.com/web-scene-specification/objects/tiledImageServiceLayer/)
- [Tiled Map Service Layer (ArcGISTiledMapServiceLayer)](https://developers.arcgis.com/web-scene-specification/objects/tiledMapServiceLayer/)
- [Vector Tile Layer (VectorTileLayer)](https://developers.arcgis.com/web-scene-specification/objects/vectorTileLayer/)
- [WebTiledLayer](https://developers.arcgis.com/web-scene-specification/objects/webTiledLayer/)
- [WMS Layer (WMS)](https://developers.arcgis.com/web-scene-specification/objects/wmsLayer/)

### Basemap

关于Basemap的JSON例子：

```
{
  "baseMap": {
    "baseMapLayers": [
      {
        "id": "World_Imagery_5014",
        "title": "World Imagery",
        "layerType": "ArcGISTiledMapServiceLayer",
        "opacity": 1,
        "visibility": true,
        "url": "http://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer"
      },
      {
        "id": "World_Boundaries_and_Places_9730",
        "title": "World Boundaries and Places",
        "layerType": "ArcGISTiledMapServiceLayer",
        "isReference": true,
        "visibility": true,
        "url": "http://services.arcgisonline.com/ArcGIS/rest/services/Reference/World_Boundaries_and_Places/MapServer"
      }
    ],
    "title": "Imagery with Labels"
  }
}
```

使用ground.layers来表示elevationLayers

### Presentation

就是幻灯片和幻灯片的表现了。

举个例子：

```
{
  "presentation": {
    "slides": [
      {
        "id": "slide_1",
        "title": {
          "text": "Slide 1"
        },
        "thumbnail": {
          "url": "data:image/jpeg;base64,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"
        },
        "description": {
          "text": "Slide description"
        },
        "baseMap": {
          "id": "basemap_3",
          "title": "Light Gray Canvas",
          "baseMapLayers": [
            {
              "url": "http://services.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Light_Gray_Base/MapServer",
              "layerType": "ArcGISTiledMapServiceLayer",
              "isReference": false,
              "title": "World Light Gray Canvas Base",
              "id": "World_Light_Gray_Base_1486"
            },
            {
              "url": "http://services.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Light_Gray_Reference/MapServer",
              "layerType": "ArcGISTiledMapServiceLayer",
              "isReference": true,
              "title": "World Light Gray Reference",
              "id": "World_Light_Gray_Reference_1486"
            }
          ]
        },
        "viewpoint": {
          "scale": 294288.68889219884,
          "targetGeometry": {
            "xmin": -6699655.673599025,
            "ymin": 2095589.1288518738,
            "xmax": -6233038.83376736,
            "ymax": 2562322.7919495814,
            "spatialReference": {
              "wkid": 102100
            }
          },
          "camera": {
            "position": {
              "x": -6409183.940944876,
              "y": 1696072.3376810949,
              "z": 140179.151083461,
              "spatialReference": {
                "wkid": 102100
              }
            },
            "heading": -11.142703441074717,
            "tilt": 74.27611784201275
          }
        },
        "visibleLayers": [
          {
            "id": "layerSphere"
          }
        ],
        "environment": {
          "lighting": {
            "datetime": 1426420800000,
            "directShadows": true
          }
        }
      }
    ]
  }
}
```

## ArcGIS API for Python中的WebScene

我在这篇文章中介绍了ArcGIS API for Python： [我的Python学习笔记(5): ArcGIS API for Python](https://blog.csdn.net/uninterrupted/article/details/77884656)。以下为截图：

![img](https://i.imgur.com/OiMf8Mi.png)

在ArcGIS API for Python中，Online和Portal中大部分的对象都属于Item，所以使用通用的content.get(itemId)的方式就可以获取到这个WebScene的对象，然后就可以查询其中的属性和调用API提供的各种方法。 

Item本身从dict继承，是从POST请求返回的JSON格式WebScene而构建的:

```
self.con.post('content/items/' + itemid, self._postdata())
```

所以可以直接使用字典的方式查询id，extent等各种属性。 

下面来进行简单的代码分析： 

安装了包之后，可以从这个目录找到代码，我使用的是anaconda，env的名字是py35，所以路径如下，大家可以自行更改查找，或者使用pycharm和F12功能（设置为VS快捷键模式） 

C:\Anaconda3\envs\py35\Lib\site-packages\arcgis-1.3.0-py3.5.egg\arcgis\gis\_init__.py
```
def get(self, itemid):
    """ Returns the item object for the specified itemid.


    =======================    =============================================================
    **Argument**               **Description**
    -----------------------    -------------------------------------------------------------
    itemid                     Required string. The item identifier.
    =======================    =============================================================

    :return:
        The item object if the item is found, None if the item is not found.
    """
    try:
        item = self._portal.get_item(itemid)
    except RuntimeError as re:
        if re.args[0].__contains__("Item does not exist or is inaccessible"):
            return None
        else:
            raise re

    if item is not None:
        return Item(self._gis, itemid, item)
    return None
```

大致画一个相关类图： 
![img](https://i.imgur.com/DOS9mpY.png)
## ArcGIS API for JavaScript中的Webscene

具体可以参考：[https://developers.arcgis.com/javascript/latest/api-reference/esri-WebScene.html](https://developers.arcgis.com/javascript/latest/api-reference/esri-WebScene.html)

由于WebScene很多内容都来自于Zurich R&D Center，所以JS API里面对WebScene的描述应该是最全面的了。

使用起来也很简单：

```
var scene = new WebScene({
  // autocasts as esri/portal/PortalItem
  portalItem: {
    id: "affa021c51944b5694132b2d61fe1057"  // ID of the WebScene on arcgis.com
  }
});

var view = new SceneView({
  map: scene,  // The WebScene instance created above
  container: "viewDiv"
});

scene.when(function() {
  // All layers and the basemap have been created
});
view.when(function() {
  // All layer and basemap resources have been loaded and are ready to be displayed
});
```

更多细节大家还是慢慢啃官方API-Reference吧。

照例还是画一个简图，就是做参考用，不要在意细节。 
![img](https://i.imgur.com/yjaRvu2.png)
## WebScene的RESTful服务endpoint

### WebScene

POST如下URL来获取json:

`https://www.arcgis.com/sharing/rest/content/items/91b46c2b162c48dba264b2190e1dbcff/data?f=json`

可以看到数据主要分为，operationalLayers, baseMap, ground三类，展示效果有presentation，里面会引用上面各种图层的id。

### SceneService

说到这里，就顺便说下SceneService的内容吧，毕竟是WebScene中最炫的数据格式。

首先，还是要看i3s-spec 
[https://github.com/Esri/i3s-spec](https://github.com/Esri/i3s-spec)

以这个WebScene为例（新版CSDN好像嵌入不了网页了，大家可以自己嵌入在前端展示，或者浏览代码中的链接。）

[sf_small](http://www.arcgis.com/home/webscene/viewer.html?webscene=40b3391c9cad4beca75155e79d42366d)

```xml
<iframe width="500" height="400" frameborder="0" scrolling="no" allowfullscreen src="<http://www.arcgis.com/home/webscene/viewer.html?webscene=40b3391c9cad4beca75155e79d42366d&viewpoint=cam:-122.38172925,37.78671683,344.267;286.649,63.16&ui=min>"></iframe>
```

它的sceneLayerURL是 
[https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer](https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer)

从layers属性可以看出，这个service中只包含一个图层，id为0，然后我们就可以在后面加上layers/0来访问该图层。

从[https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0](https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0)

中我们大致可以知道数据的范围，高程参数，纹理，属性等信息，也可以从rootNode字段找到根节点是root。 

然后就可以使用 
[https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0/nodes/root](https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0/nodes/root)

来看service中根节点的信息了。 

然后可以遍历各个字段，找找这个节点是否有feature，geometry，texture等信息。 

如果需要找feature信息，就可以在后面加features/0, 

geometry的二进制信息，在后面加geometries/0, 

纹理信息:  
[https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0/nodes/1/textures/0_0](https://tiles.arcgis.com/tiles/lVkj5PBOw7tRmIPU/arcgis/rest/services/sf_small/SceneServer/layers/0/nodes/1/textures/0_0)
中间细节很多，如果感兴趣的话，可以在博客下面留言，我会看大家的反馈，对各个部分进行更新。

## 总结

本博客，从数据标准，Python，Javascript API中使用，RESTful服务endpoint等方面介绍了WebScene，时间关系，不能做一个详尽的讲解。

与本博客相关博客：
- [在ArcGIS Online中创建三维图层和网络场景(2017.9)   - CSDN博客](https://blog.csdn.net/iuhsihsow/article/details/78152696)
- [场景图层和i3s标准在ArcGIS多平台的应用   - CSDN博客](https://blog.csdn.net/iuhsihsow/article/details/78018033)
- [我的Python学习笔记(5): ArcGIS API for Python](https://blog.csdn.net/uninterrupted/article/details/77884656)

相关网站：
- WebSceneSpec: [https://developers.arcgis.com/web-scene-specification/](https://developers.arcgis.com/web-scene-specification/)
- i3s-spec: [https://github.com/Esri/i3s-spec](https://github.com/Esri/i3s-spec)
- ArcGIS API for Javascript - WebScene: [https://developers.arcgis.com/javascript/latest/api-reference/esri-WebScene.html](https://developers.arcgis.com/javascript/latest/api-reference/esri-WebScene.html)
- ArcGIS API for Python: [https://developers.arcgis.com/python/](https://developers.arcgis.com/python/)







