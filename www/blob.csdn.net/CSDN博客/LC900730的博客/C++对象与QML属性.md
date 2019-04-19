# C++对象与QML属性 - LC900730的博客 - CSDN博客
2017年08月31日 15:44:03[lc900730](https://me.csdn.net/LC900730)阅读数：205
### 导出
//QQuickView
```
QQuickview viewer；
viewer.rootContext()-> setContextProperty("colorMaker",new ColorMaker);
viewer.serSource(QUrl("qrc:///main.qml"));
```
从堆上分配了一个ColorMaker对象，然后注册为QML上下文是属性，名字为colorMaker。 
viewer.rootContext()返回的是QQmlContext对象。QQmlContext类代表一个QML上下文，它的setContextProperty()方法可以为该上下文设置一个全局可见的属性。
## QQmlApplicationEngine+Window
```
QQmlApplicationEngine engine;
engine.rootContext()-> setContextProperty("colorMaker",new ColorMaker);
engine.load(QUrl(QStringLiteral("qrc:///main.qml")))
```
在一个qml文件中：
```
Window {
    objectName: "rootObject"
    visible: true
    width: 360
    height: 360
    title: qsTr("Hello World")
    Text{
        objectName: "textLabel";
        text:"hello world"
        anchors.centerIn: parent
        font.pixelSize: 26
    }
    Button{
        anchors.right:parent.right
        anchors.rightMargin: 4;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 4;
        text:"quit"
        objectName: "quitButton"
    }
}
```
在cpp文件中可以这样使用：
```
QList<QObject *>rootObjects=engine.rootObjects();
qDebug()<<rootObjects;
//QQuickWindowQmlImpl(0x7f80xxxx,name="rootObject")
//遍历rootOject的子对象
int count=rootObjects.size();
for(int i=0;i<count;i++){
    if(rootObjects.at(i)->objectName()=="rootObject"){
        root=rootObjects.at(i);
        break;
    }
}
new ChangeQmlColor(root);
QObject *quitbutton=root->findChild<QObject *>("quitButton");
QObject *textLabel=root->findChild<QObject *>("textLabel");
```
```
bool bRet=QMetaObject::invokeMethod(textLabel,"setText",Q_ARG(QString,"world hello"));
qDebug()<<"call setText return "<<bRet;
textLabel->setProperty("color"QColor::fromRgb(255,0,0));
bRet=QMeatObject::invokeMethod(textLAbel,"doLayout");
qDebug()<<"call dolayout return "<<bRet;
```
上述通过textLabel找到了textLabel对象，使用invokeMEthod()调用setText()方法来改变textLabel的文本，这个注定失败；因为QML中Text对象对应着C++的QQuickText类，而QQuickText没有名为“setText”的槽或者可调用方法。
