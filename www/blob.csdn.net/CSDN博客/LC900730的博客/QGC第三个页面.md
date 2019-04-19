# QGC第三个页面 - LC900730的博客 - CSDN博客
2017年09月14日 18:11:38[lc900730](https://me.csdn.net/LC900730)阅读数：197
点击第三个图标，实际上进入的第四个页面
## PlanView.qml
```
QGCView{
    id:_qgcView
    viewPanel:panel;
    z:QGroundControl.zOrderTopMost  
}
```
```
ToolStrip{
    id:toolStrip
    ...
    ...
    model:[
    {
        name:"Waypoint",
        iconSource:"/qmlimages/MapAddMission.svg",
        toggle:true 
    },
    {
        name:"In",
        iconSource:"/qmlimages/ZoomMinus.svg",  
    },
    ]
onClicked:{
    switch(index){
        case 0:
            _addWayPointOnClick=checked
            break
        case 1:
            if(_singleComplexItem){
            addComplexItem(_missionController.complex)
            break
            }
        case 4:
            editorMap.zoomLevel+=0.5
            break
        case 5:
            editorMap.zoomLevel-=0.5
            break
            }       
    }
}
}
```
//点击survey 
出现MissionItem
在MissionSettingsEditor.qml中
```
Componnet{
    id:valueColumn
    anchors.left:   parent?parent.left:undefined
    anchors.rightt: parent?parent.right:undefined
    anchors.top:parent?parent.top:undefined
    spacing: _margin
    Column{
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: _margin
        GridLayout{
            ...
            QGCLabel{
                text:qsTr("Waypoint alt")
            }
        FactTextField{
            fact:...
            Layout.fillWidth: true
        }
        QGCCheckBox{
            onClicked:missionItem.speedSection.specifyFlightSpeed=checked;
        }
        QGC
        }
    }
}
```
//当点击+的waypoint的时候 
地图变成了有mousrArea的。点击地图上的点，都是从1开始编号，missionItem
当点击+号之后，出现H标志的home位置。 
点击一次出现一个航点，第一个是takeoff，依次是waypoint
这个Mission里面，Mission Start里面点击有Vehicle Info 
Planned Home Positopn
```
SectionHeader {
                id:         plannedHomePositionSection
                text:       qsTr("Planned Home Position")
                visible:    !_vehicleHasHomePosition
                checked:    false
            }
QGCLabel {
            text: qsTr("Altitude")
 }
 FactTextField {
               fact:              
               missionItem.plannedHomePositionAltitude
               Layout.fillWidth:   true
 }
```
