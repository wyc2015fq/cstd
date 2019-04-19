# QGC之qml - LC900730的博客 - CSDN博客
2017年08月20日 15:01:00[lc900730](https://me.csdn.net/LC900730)阅读数：748
## 主要的qml
### LogDownloadPage.qml
resource/qgroundcontrol.qrc/qml/QGroundControl/LogDownloadPage.qml
主要定义了一个AnalyzePage对象
```
AnalyzePage{
    id:logDownloadPage
    pageComponent:pageComponent
    pageName:qsTr("log Download")
}
```
/resource/qgroundcontrol.qrc/qml/QGroundControl/PlanView.qml
这是地图的主界面， 
左侧的Plan 
QGroundControl提供了一套用于构建用户界面的基础控件，是基于Qt中QML Controls控件的轻量封装。
```
import QGroundControl.Controls 1.0
```
以下 QGC 控件基于标准 Qt QML Controls 开发，与 Qt 标准控件功能相同，只是其颜色由 QGC palette 提供。
QGCButton 
QGCCheckBox 
QGCColoredImage 
QGCComboBox 
QGCFlickable 
QGCLabel 
QGCMovableItem 
QGCRadioButton 
QGCSlider 
QGCTextField 
QGC Controls
这些自定义的控件只用于 QGC ，是其用户界面中的基本元素。
DropButton - 圆形按钮，单击时会弹出选项。（Plan view -> Sync button）
ExclusiveGroupItem - 作为自定义控件的 base Item ，提供 Qml ExclusiveGroup concept。
QGCView - 系统中顶层视图的基本控制，提供 FactPanels 支持和 QGCViewDialogs 、 QGCViewMessages 显示.
QGCViewDialog - 在 QGCView 中从右侧弹出的对话框。可以定制对话框元素（接收/拒绝）。（eg. 参数界面的参数修改对话框）
QGCViewMessage - QGCViewDialog 的简化版，可以定制按钮和短文本。
QGCViewPanel - QGCView 中的主视图。
RoundButton - 圆形按钮，圆形内显示图标。
SetupPage - 飞行器配置页，提供标题（title）、描述（description）、内容区域（component page contents area）。
## 图层问题
```
ToolStrip{
    id:toolStrip
    title:qsTr("Plan")
    z:QGroundControl.zOrderWidgets
}
//地图的图层要低一级
MapTtem
```
