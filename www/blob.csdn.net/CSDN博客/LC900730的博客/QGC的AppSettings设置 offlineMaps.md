# QGC的AppSettings设置:offlineMaps - LC900730的博客 - CSDN博客
2017年09月19日 15:19:42[lc900730](https://me.csdn.net/LC900730)阅读数：431
## AppSettings.cc
```
const char *AppSettings::parameterDirectory ="Parameters"
const char *AppSettings::telemetryDirectory ="Telemetry"
const char *AppSettings::missionrDirectory ="Missions"
```
设置里面的offlineMap选项 
在OffineMap.qml中
```
QGCViewPanel{
}
```
点击OfflineMap出现瓦片地图
点击Add New Set 
出现如下所示
![这里写图片描述](https://img-blog.csdn.net/20170919153051793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
Item{
    id:addNewSetView
    anchors.fill:parent
    visible:parent
    Column{
        text:"Show zoom previews"
        visible:!_showPreview
        onClicked:_showPreview=!_showPreview
    }
    Map{
        id:minZoomPreview
        width:addNewSetView.width/4
    }
}
```
在FlightMapSettings.h中定义了
```
typedef enum{
}
```
```
//FlightMap.qml中
Map{
    id:_map;
    zoomLevel:QGroundControl.flightMapZoom
    plugin: Plugin{
        name:"QGroundControl"
    }
}
QGroundControl地图组件中在QGCLocationPlugin中:
有个json文件中配置的：
{
    "Keys": ["qgc-maps"],
    "Provider": "QGroundControl",
    "Version": 101,
    "Experimental": false,
    "Features": [
        "OnlineMappingFeature",
        "OnlineGeocodingFeature",
        "ReverseGeocodingFeature"
    ]
}
```
