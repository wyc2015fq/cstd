# QGC的PlanToolBar - LC900730的博客 - CSDN博客
2017年09月17日 23:06:14[lc900730](https://me.csdn.net/LC900730)阅读数：235
## MainWindowInner.qml中当点击时候执行showPlanView()
```php
//点击主窗口第三个图标，显示这个planView
function showPlanView(){
    rootLoader.sourceComponent=null;
    if(currentPopUp){
        currentPopUp.close()
    }
    if(planViewLoader.source!=_planViewSource){
            planViewLoader.source!=_planViewSource
    }
    ScreentTools.availableHeight=parent.height-toolbar.height
    hideAllViews();
    planViewLoader.visible=true;
    planToolBar.visible=true;
}
```
![这里写图片描述](https://img-blog.csdn.net/20170917230309731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## QGC的PlanToolBar
```
//qml/QGroundControl/Controls/PlanToolBar
Rectangle{
    id:_root
    height:ScreenTools.toolbarHeight;   
    anchors.left:parent.left;
    anchors.right:parent.right;
    anchors.top:parent.top;
    signal showFlyView;
    property var planMasterController
    property var currentMissionItem
    property var missionItems:       ..
    property var missionDistance:    ...
    ...
    ...
    function getMissionTime(){
        if(isNaN(_missionTime)){
            return "00:00:00"
        }
        var t=new Date(0,0,0,0,0,Number(_missionTime))
        return  Qt.formatTime(t,"hh:mm:ss")
    }
    //工具栏阻止鼠标事件传递
    MouseArea{
        anchors.fill:parent;
        onWheel:{wheel.accepted=true;}
        onPressed:{mouse.accepted=true;}
        onReleased:{mouse.accepted=true;}
    }
...
...
    GridLayout{
        QGCLable{
            text:qsTr("Selected Waypoint");
            Layout.columnSpan:8
            font.pointSize:_dataFontSize;
        }
        QGCLable{
            text:qsTr("Alt diff");
            font.pointSize:_dataFontSize;
        }
    ...
        QGCLable{
            text:qsTr("Azimuth");
            font.pointSize:_dataFontSize;
        }
        QGCLable{
            text:_azimuthText;
            font.pointSize:_dataFontSize;
        }
        QGCLable{
            text:qsTr("Distance");
            font.pointSize:_dataFontSize;
        }
        QGCLable{
            text:_distanceText;
            font.pointSize:_dataFontSize;
        }
    }
    //电池信息不显示
}
```
