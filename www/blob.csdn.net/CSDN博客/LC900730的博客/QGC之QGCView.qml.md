# QGC之QGCView.qml - LC900730的博客 - CSDN博客
2017年09月18日 13:35:44[lc900730](https://me.csdn.net/LC900730)阅读数：422
### QGCView.qml
组件根元素引入FactPanel
```
FactPanel{
    id:_rootItem;
    signal completed
}
```
## FactPanel.qml
```
FocusScope{
    property alias color:rectangle.color;
    property string __missingParams:""
    property string __errorMsg
    function showMissingParameterOverlay(missingParamName){
    if(__missigParams.length!=0){
            __missingParams=__missingParams.concat(missingParamName)
        __missingParamsOverlay.visible=true;
    }
}
    function showError(erroeMsg){
        __errorMsg=errorMsg;
        __missionParamsOverlay.visible=true;
    }
}
```
注意：Rectangle有宽高会自己显示。
### Connections对象
一个Connections对象创建一个到QML信号的连接。
```
Connections{
    target:area;
    on{Signal}:function or code block
}
```
- 需要将多个对象连接到同一个QML信号上
- 你需要在发出信号的作用域外来建立连接
- 发射信号的对象没有在QML中
```
Text{
    id:text1;
    color:"blue"
    text:"Text one"
}
Text{
    id:text2;
    color:"blue"
    text:"Text two"
}
Button{
    id:changeButton;
    text:"change"
}
//target是通过id找到的这个按钮
//点击信号
Connections{
    target:changeButton
    onClicked:{
        text1.color:Qt.rgba(xxx);
        text2.color:Qt.rgba(xxx);
    }
}
```
