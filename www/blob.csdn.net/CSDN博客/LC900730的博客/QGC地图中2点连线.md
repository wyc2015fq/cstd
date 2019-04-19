# QGC地图中2点连线 - LC900730的博客 - CSDN博客
2017年09月17日 22:35:29[lc900730](https://me.csdn.net/LC900730)阅读数：345
## 画线
```
/qml/FlightMqp/MissionLineView.qml
//mission items之间连线
//定义了组件
MapItemView{
    id:_root
    property bool homePositionValid:true
    delegate:MapPolyline{
        line.width:3
        line.color:"#be781c"
        z:QGroundControl.zOrderWaypointLines
        path:[
            object.coordinate1,
            object.coordinate2,
        ]
    }
}
```
