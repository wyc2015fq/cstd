# QGC的第二个按钮Vehicle Setup - LC900730的博客 - CSDN博客
2017年09月14日 17:43:45[lc900730](https://me.csdn.net/LC900730)阅读数：316
点击左边栏的Summary： 
    SetupView.qml
## QGCView
```
FactPanel{
    id:_rootItem
    property var qgcView:_rootItem;
    property bool completedSignalled:false;
    property real topDialogMargin : 0;
    property var viewPanel; 
}
```
## AppSetting
第一个Summary
```
//qml/QGroundControl/SetupView.qml
Revtangle{
    id:setupView;
    color:qgcPal.window
    z:QGroundControl.zOrderTopMost
    QGCPalette{
        id:qgcPal;
        colorGroupEnabled:true
    }
    ExclusiveGroup{
        id:setupButtonGroup
    }
}
```
在Setup.qml中，如果没有连接vehicle， 
显示的Component是id为disconnectedVehicleSummaryComponent
```
Component{
    id:disconnectedVehicleSummaryComponent
    Rectangle{
        color:qgcPal.windowShade
        QGCLable{
            anchors.margins:_defaultTextWidth*2;
            anchors.fill:parent;
            verticalAlignment:Text.AlignVCenter
            horizontalAlignment:Text.AlignHCenter
            wrapMode:Text.WordWrap
            font.pointSize:ScreenTools.largeFontPointSize
            text:qsTr("Vehicle settings and info will display after ..."+(ScreentTools.isMobile?"":"Click Firmware on the left to upgrade your vehicle"))
        }
    }
}
```
