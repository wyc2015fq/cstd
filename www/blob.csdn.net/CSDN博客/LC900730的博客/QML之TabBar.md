# QML之TabBar - LC900730的博客 - CSDN博客
2017年08月31日 22:28:57[lc900730](https://me.csdn.net/LC900730)阅读数：1661
### TabBar的切换效果
```
import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
Window{
    id:window
    visible:true
    width:400
    height:400
    TabBar{
        id:bar;
        width:parent.width;
        TabButton{
            text:qsTr("Home");
        }
        TabButton{
            text:qsTr("Discover");
        }
        TabButton{
            text:qsTr("Activity");
        }
    }
    StackLayout{
        width:parent.width;
        currentIndex:bar.currentIndex;
        Item{
            id:homeTab
        }
        Item{
            id:discoverTab
        }
        Item{
            id:activityTab
        }
    }
}
```
### 如果TabBar的宽度超过可用宽度，会自动形成flickable
```
TabBar{
    id:bar
    width:parent.width
    Repeater{
        model:["First","Second","Third","Fourth","Fifth"]
        TabButton{
            text:modelData;
            width:Math.max(100,bar.width/5);
        }
    }
}
```
flickable形成的效果是：当点击第4个按钮的时候，第1个按钮隐藏一半，第5个按钮露出一半，当点击第5个按钮的时候，第1个按钮隐藏了。
