# QML之ScrollView滚动条 - LC900730的博客 - CSDN博客
2017年09月01日 12:13:51[lc900730](https://me.csdn.net/LC900730)阅读数：3162
### ScrollView滚动条
```
ApplicationWindow{
    visible: true
    height: 300;
    width: 530;
    ScrollView{
        id:scroll
        width: 530
        height: 300
        Rectangle{
            id: rect
            width: 500
            height: 900;
            Rectangle{
                id:rect1;
                anchors.top: rect.top
                width: 500
                height: 300
                color: "red"
                border.color: "blue"
            }
            Rectangle{
                id: rect2
                anchors.top: rect1.bottom
                width: 500
                height: 300
                color: "green"
                border.color: "blue"
            }
            Rectangle{
                id: rect3
                anchors.top: rect2.bottom
                width: 500
                height: 900
                color: "orange"
                border.color: "blue"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        scroll.flickableItem.contentY = rect2.y
                    }
                }
            }
        }
    }
}
```
图片预览如下 
![scrollew](https://img-blog.csdn.net/20170901112934519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再点击第三个矩形块的时候，会自动将滚动条置于第2个矩形。
