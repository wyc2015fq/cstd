# Qt之Action - LC900730的博客 - CSDN博客
2017年09月15日 22:49:50[lc900730](https://me.csdn.net/LC900730)阅读数：276
## Action
Action控件封装应用程序的 动作命令； 
Action可以添加到菜单、工具条中，由用户触发或通过快捷键调用。 
使用Action最大好处是出现在多个位置的 Action可以自动同步，如可以将一个Action添加到菜单栏和工具栏，当修改了Action的某些属性时候，菜单栏和工具栏的对应的按钮会同步变化。
### Action具有triggered()信号
通过连接这个信号，可以响应Action的动作。为了达到这个目的，需要在相应的onTriggered()信号处理器中编写代码。
### checkable
Action类型包含一个默认的checkable属性，可以设置Action是否被选中或者被切换状态，默认是false；如果设置为true，那么使用通过checked属性来获取或设置状态。状态改变时候会发射toggled()信号，而且可以使用exclusiveGroup属性将几个Action设置为互斥的一组，同组中只能有一个Action被选中。
```
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
ApplicationWindow{
    visible: true;
    width: 480;
    height: 480
ExclusiveGroup{
    id:radioInputGroup
    Action{
        id:dabRadioInput;
        text:"DAB"
        checkable: true
        checked: true
    }
    Action{
        id:fmRadioInput;
        text:"FM"
        checkable: true
    }
    Action{
        id:amRadioInput;
        text:"AM"
        checkable: true
    }
}
toolBar: ToolBar{
    id:mainToolBar;
    width: parent.width
    Row{
        anchors.fill: parent
        ToolButton{
            action: dabRadioInput
        }
        ToolButton{
            action: fmRadioInput
        }
        ToolButton{
            action: amRadioInput
        }
    }
}
}
```
checked:true表示默认选中哪个。 
ExclusiveGroup能保证互斥
### ExclusiveGroup
ExclusiveGroup仅能将Action对象作为自己的子元素，当我们需要使用其他控件的时候，只能显式将ExclusiveGroup对象赋值给这些元素的ExclusiveGroup属性。
```
ApplicationWindow{
ExclusiveGroup{id:radioInputGroup;}
toolBar:ToolBar{
    id:mainToolBar;width:parent.width
    Row{
        anchors.fill:parent
        ToolButton{
            id:dabRadioInput;
            text:"DAB"
            checkable:true
            exclusiveGroup:radioInputGroup; 
        }
        ToolButton{
            id:amRadioInput;
            text:"AM"
            checkable:true
            exclusiveGroup:radioInputGroup; 
        }
        ToolButton{
            id:fmRadioInput;
            text:"FM"
            checkable:true
            exclusiveGroup:radioInputGroup; 
        }
    }
}
}
```
