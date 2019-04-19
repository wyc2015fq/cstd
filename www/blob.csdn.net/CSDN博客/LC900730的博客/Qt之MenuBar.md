# Qt之MenuBar - LC900730的博客 - CSDN博客
2017年09月15日 22:35:46[lc900730](https://me.csdn.net/LC900730)阅读数：2115
```
import QtQuick 2.6
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
ApplicationWindow {
//    id: name
    width: 640
    height: 480
    visible: true
    title:qsTr("hello World");
    /*
//    Text {
//        width: 100;
//        height: 100;
////        text:msg.author
//    }
//    Loader{
//        anchors.fill: parent;
//        sourceComponent: rect;
//    }
//    Component{
//        id:rect;
//        Rectangle{
//            width: 50;
//            height: 50;
//            color:'red'
//        }
//    }
*/
    Action{
        id:quitAction
        text:qsTr("Quit");
        shortcut: "ctrl+Q";
        onTriggered: Qt.quit()
    }
    Action{
        id:cutAction;text:qsTr("Cut");
        shortcut: "ctrl+X";
        onTriggered: Qt.quit()
    }
    Action{
        id:copyAction;text:qsTr("Copy");
        shortcut: "Ctrl+C";
        onTriggered: Qt.quit()
    }
    Action{
        id:pasteAction;
        text:qsTr("Paste");
        shortcut: "Ctrl+P";
        onTriggered: Qt.quit()
    }
    menuBar:
        MenuBar{
            Menu{
                title: qsTr("File")
                MenuItem{
                    text:"open"
                    action: quitAction
                }
            }
            Menu{
                title: qsTr("Edit")
                MenuItem {
                    text:"剪切"
                    action:cutAction
                }
                MenuItem {
                    text:"复制"
                    action:copyAction
                }
                MenuItem {
                    text:"粘贴"
                    action:pasteAction
                }
            }
}
    toolBar: ToolBar{
        id:mainToolBar
        width:parent.width
        Row{
            anchors.fill: parent
            ToolButton{action:cutAction}
            ToolButton{action:copyAction}
            ToolButton{action:pasteAction}
        }
    }
    Button{
        text:qsTr("Hello World");
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.verticalCenter: parent.verticalCenter
    }
}
```
与很多文章不同的是，如果Menu组件没有title，那么该menu不会显示；即使有title，如果没有MenuItem，或者有MenuItem没有text，那么依然不会显示。 
快捷键的显示：shortCut：“ctrl+C”，“ctrl+V”都已经提供
图片如下： 
![这里写图片描述](https://img-blog.csdn.net/20170915224807215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
