# QGC的FlightDisplayView - LC900730的博客 - CSDN博客
2017年09月20日 15:22:42[lc900730](https://me.csdn.net/LC900730)阅读数：416
## 主要是指南针表盘
1.在MainWindowInner.qml中
```
//MainWindowInner.qml
FlightDisplayView.visible = true
```
2.显示FlightDisplayView
```
//FlightDisplayView.qml
QGCView{
    property var _planMasterController:masterController
    property var _missionController:_planMasterController.missionController
    ...
    ... 
    ...
    PlanMasterController{
        id:masterController
        Component.onCompleted:start(false);
    }
}
```
3.左侧的ToolStrip
```
ToolStrip{
    visible:_activeVehicle?_activeVehicle.guideModeSupported:true
    id:toolStrip;
    title:qsTr("Fly")
    //4个按钮可见
    //takeoff与land指令不能同时可见
    buttonVisible:
        [_guideController.showTakeoff||_guideController.showLand,_guideController.showLand&&!_guideController.showPause,true,true,true,_guidedController.smartShotsAvailable]
    //按钮是否可以点击
    buttonEnabled:
        [_guideController.showTakeoff,_guideController.showLand,_guideController.showRTL,_guideController.showPause,_anyActionAvailable,_anySmartAvailable]
}
//数据通过代理设置,代理在GuideActionsController.qml
_guideController.startMissionTitle
```
```
//GuideActionsController.qml
readonly property string startMissionTitle:qsTr("Start Mission")
readonly property string emergencyStopTitle:qsTr("Emergency Stop")
```
//大表盘
```
//FlightDisplayView.qml
FlightDisplayViewWidgets{
    id:flightDisplayViewWidgets
    z:_panel.z+4
    ...
    ...
    visible: singleVehicleView.checked
}
//在FlightDisplayViewWidgets.qml
Item{
    id:_root
    property var qgcView
    property bool useLightColors
    function _SetInstrumentWidget(){
        if(QGroundControl.corePlugin.options.instrumentWidget){
                if(QGroundControl.corePlugin.options.instrumentWidget.
source.toString().length){
    instrumentLoader.source=QGroundControl.corePlugin.options.instrumentWidgets.source
switch(QGroundControl.corePlugin.options.instrumentWidget.widgetPosition){
    case CustomInstrumentWidget.POS_TOP_RIGHT:
        instrumentsLoader.state="topMode"
        break;
    case CustomInstrumentWidget.POS_BOTTOM_RIGHT:
        instrumentsLoader.state="bottomMode"
        break;
    case CustomInstrumentWidget.POS_CENTER_RIGHT:
    default:
        instrumentsLoader.state="centerMode"
        break;
    }
}else{
    var useAlternateInstrument=QGroundControl.settingsManager.appSettings.virtualJoystick.value||
    ...
}
}
}
...
Component.onCompleted:{
    _setInstrumentWidget()
}
}
```
## 点击仪表盘的齿轮
出现Value Widget setup 
![这里写图片描述](https://img-blog.csdn.net/20170920172803510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面InstrumentSwipeView组件 
在FlightMap/InstrumentSwipe.qml中
```
//InstrumentSwipeView.qml
Item{
    id:_root
    clip:true
    height:column.height
    property var qgcView
    function showPicker(){
        _valuesPage.showPicker()
    }
    function showPage(pageIndex){
        pageRow.x=-(pageIndex*_pageWidth)
        _currentPage=pageIndex
    }
    ...
    Column{
        id：column
        Row{
            ...
            ...
        }
    }
}
```
//主界面的qWarning()<<”No Internet Access”
```
void QGCCacheWorker::_testInternet(){
    QTcpSocket socket;
    socket.connectToHost("www.github.com", 80);
    if (socket.waitForConnected(2500)) {
        qCDebug(QGCTileCacheLog) << "Yes Internet Access";
        emit internetStatus(true);
        return;
    }
    qWarning() << "No Internet Access";
    emit internetStatus(false);
}
```
