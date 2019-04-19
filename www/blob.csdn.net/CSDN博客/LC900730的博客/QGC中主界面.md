# QGC中主界面 - LC900730的博客 - CSDN博客
2017年09月19日 11:41:21[lc900730](https://me.csdn.net/LC900730)阅读数：1082
## MainWindowInner.qml
```
//--Main UI
MainToolBar{
    id:toolBar;
    height:ScreenTools.toolbarHeight
    anchors.left: parent.left
    anchors.right: parent.right
    ...
    ...
    opacity:planToolBar.visible?0:1
    z:QGroundControl.zOrderTopMost
    ...
    ...
    Component.onCompleted:ScreenTools.available=parent.height-toolBar.height;
    onShowSettingsView:
    onShowSetupView:   mainWindow.showSetupView();
    onShowShowPlanView:   mainWindow.showPlanView();
    //MainToolBar组件在MainToolBar.qml中
    MainToolBar{    
    }
}
```
默认是flyButton被选中
//MainToolBar组件在MainToolBar.qml中 
//定义如下
```
Rectangle{
    id:toolBar
    color:qgcPal.globalTheme===QGCPalette.Light?Qt.rgba(1,1,1,0.8):
                    Qt.rgba(0,0,0,0.75)
    QGCPalette{id:qgcPal;colorGroupEnabled:true}
    property var _activeVehicle:
        QGroundControl.multiVehicleManager.activeVehicle;
    signal showSettingsView;
    signal showSetupView;
    signal showPlanView;
    signal showFlyView;
    signal showAnalyzeView;
    signal armVehicle
    signal disarmVehicle
    function checkSettingsButton(){
        settingsButton.checked=true;
    }
    function checkSetupButton(){
        setupButton.checked=true;
    }
    Componnet.onCompleted{
        flyButton.checked=true
    }
    }
}
```
最小化窗口的时候会输出QQuickWidget:Attempted to render scene with no context
在MainWindowInner.qml中引入了FlightDisplayView组件 
FlightDisplayView.qml
``
在Fly主界面，最左边的面板 
![这里写图片描述](https://img-blog.csdn.net/20170919121348048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
FlightDisplayViewMap.qml中
```
onUserPannedChanged:{
    if(userPanned){
        console.log("user panned")
        userPanned=false;
        _disableVehicleTracking=true
        panRecenterTimer.restart();
    }
}
```
FlightDisplayViewMap.qml 
中的object
在QGroundControlQmlGlobal.h中
```bash
qreal zOrderTopMost(){return 1000;} 
qreal zOrderTopWidgets(){return 100;} 
qreal zOrderMapItems(){return 50;} 
qreal zOrderVehicles(){return 49;} 
qreal zOrderTrajectoryLines(){return 48;} 
qreal zOrderWaypointLines(){return 47;}
```
5个页面，主要是受MainWindowInner.qml的_viewList属性控制
```
property var _viewList:[
    settingsViewLoader,
    setupViewLoader,
    planViewLoader,
    flightView,
    analyzeViewLoader
]
```
第一个页面的Waitting for Vehicle  Connection 
在MainToolBarIndicators.qml中
```
//MainToolBarIndicators.qml
Item{
}
```
默认第4个按钮是选中的，展示地图界面原因： 
1.第4个选中 
//MainWindowInner.qml中引入了MainToolBar组件 
//在MainToolBar.qml中
```
Rectangle{
    id:toolBar;
    ...
    ...
    signal showSettingsView
    signal showSetupView
    ...
    Component.onCompleted:{
        flyButton.checked=true
    }
}
```
显示地图 
//在MainWindow.UI中：
```
FlightDisplayView{
    id:flightView
    anchors.fill:parent
    visible:true
    Loader{
        id:rootVideoLoader
        anchors.centerIn:parent
    }
}
```
在FlightDisplayView.qml中
```
//FlightDisplayView.qml
property var _mainIsMap:QGroundControl.loadBoolGlobalSettings(_mainIsMapKey,true):true
bool QGroundControlQmlGlobal::loadBoolGlobalSettings(const QString &key,bool defaultValue)
```
