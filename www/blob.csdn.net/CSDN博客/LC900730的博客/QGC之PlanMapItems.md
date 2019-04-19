# QGC之PlanMapItems - LC900730的博客 - CSDN博客
2017年09月20日 18:31:25[lc900730](https://me.csdn.net/LC900730)阅读数：263
PlanMapItems.qml中定义了变量object
```
Component{
    id:missionLineViewComponent
    MapPolyline{
        line.width:3
        line.color:"#be781c"
        z:QGroundControl.ZOrderWaypointLines
        path:object?[object.coordinate1,object.coordinate2]:undefined
    property var object
    }
}
```
//planView.qml 
当点击waypoint之后，就 
_addWaypointOnClick=true 
然后在
```
if(_addWaypointOnClick){
    _insertSimpleMissionItem(coordinate,
        _missionController.visualItems.count);
}
```
## object的产生
MapItems/PlanMapItems.qml 
_missionLineViewComponent=missionLineViewComponent.createObject(map,{“object”,object})
