# QGC的MissionItem的Camera - LC900730的博客 - CSDN博客
2017年09月20日 12:33:10[lc900730](https://me.csdn.net/LC900730)阅读数：242
![这里写图片描述](https://img-blog.csdn.net/20170920123245390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击之后出现一个面板，复用了SectionHead.qml
```
//CameraSection.qml
Column{
    anchors.left:parent.left
    anchors.right:parent.right
    spacing:_margin
    property alias exclusiveGroup:  
         cameraSectionHeader.exclusiveGroup;
    ...
    ...
    SectionHeader{
        id:cameraSectionHeader
        text:"qsTr(Camera)"
        checked:false
    }
}
```
