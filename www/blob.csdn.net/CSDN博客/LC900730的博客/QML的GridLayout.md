# QML的GridLayout - LC900730的博客 - CSDN博客
2017年09月24日 15:28:40[lc900730](https://me.csdn.net/LC900730)阅读数：311
布局方式之一；
```
GridLayout{
    id:gridLayout3
    anchors.rightMargin:0;
    anchors.bottomMargin:0;
    anchors.leftMargin:0;
    anchors.topMargin:0;
    rows:1
    column:2
    anchors.fill:parent;
    Exclusive {id:languageGroup}
    RadioButton{
        id:enRadioButton
        text:qsTr("en")
        exclusiveGroup:languageGroup
        Layout.columnSpan:2
    }
    RadioButton{
        id:frRadioButton
        text:qsTr("fr")
        exclusiveGroup:languageGroup
        Layout.columnSpan:2
    }
    RadioButton{
        id:enRadioButton
        text:qsTr("Other")
        exclusiveGroup:languageGroup
    }
    TextField{
        id:language
        Layout.fillWidth:true
        placeholderText:qsTr("")
    }
 RowLayout {
                id: rowLayout1
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight
                Button {
                    id: goButton
                    text: qsTr("Proceed")
                }
                Button {
                    id: clearButton
                    text: qsTr("Clear")
                }
                Button {
                    id: cancelButton
                    text: qsTr("Cancel")
                }
            }
}
```
![这里写图片描述](https://img-blog.csdn.net/20170924152952757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在GridLayout中指定了rows属性，columns属性。 
如rows：1 
columns：2 
那么其他组件Layout.columnSpan:2则表示单独占满了一行。
