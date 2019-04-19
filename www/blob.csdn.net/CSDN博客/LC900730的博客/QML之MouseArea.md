# QML之MouseArea - LC900730的博客 - CSDN博客
2017年09月16日 10:47:02[lc900730](https://me.csdn.net/LC900730)阅读数：810
### MouseArea
MouseArea是一个不可见的项目，通常用来和一个可见项目配合使用。
#### enabled属性
默认为true； 
如果为false，那么MouseArea对于鼠标将会变得透明，不处理任何鼠标事件。
#### pressed属性
只读的pressed属性表明，用户是否在MouseArea上按住了鼠标按钮。这个属性经常用于属性绑定，可以实现在鼠标按下时执行一些操作。
#### containsMouse属性
只读的containsMouse属性表明，当前鼠标光标是否在MouseArea上。默认只有鼠标的一个按钮处于按下状态时，才可以被检测到。
#### hoverEnabled属性
默认MouseArea项目只报告鼠标单击而不报告鼠标光标的位置改变。可以设置hoverEnabled属性为true来更改。 
这样设置之后，onPositionChanged()和onEntered()和onExited()等函数才可以使用。 
而且这时候containsMouse属性也可以在没有鼠标按钮按下的情况下 检查光标。
#### propagateComposedEvents属性
当MouseArea与其他的MouseArea项目重叠时候，可以设置propagateComposedEvents属性为true来传播clicked、doubleClicked和pressAndHold等事件。但是只有在MouseArea没有接受这些事件的时候，它们才可以继续向下传播。
即当事件已经在一个MouseArea中进行处理，则需要在其事件处理器中设置MouseEvent.accepted为false，这样事件才能继续向下传播。
```
Rectangle {
        color: "yellow"
        width: 100;
        height: 100
        MouseArea{
            anchors.fill: parent
            onClicked: console.log("Clicked yellow")
            onDoubleClicked: {
                console.log("double clicked yellow")
                mouse.accepted=false
            }
        }
        Rectangle{
            color:"blue"
            width: 50
            height: 50
            MouseArea{
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    console.log("clicked blue")
                    mouse.accepted=false
                }
                onDoubleClicked: {
                    console.log("double clicked")
                    mouse.accepted=false
                }
            }
        }
    }
```
![这里写图片描述](https://img-blog.csdn.net/20170916110019492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为蓝色的矩形的MouseArea设置了propagateComposedEvents为true，且clicked和doubleClicked事件的MouseEvent.accept设置设置为false，所有蓝色矩形的 所有单击和双击事件都会传播到黄色矩形。
## 鼠标事件：MouseEvent
#### mouse.accepted属性
通过设置accepted为true防止鼠标事件传播到下一层项目。
#### mouse.x属性
通过x和y来获取鼠标位置。
#### mouse.button属性
通过button或buttons可以获取按下的按键； 
Qt.LeftButton、Qt.RightButton、Qt.MiddleButton、
#### modifiers
通过modifiers属性可以设置获取按下的键盘修饰符 
常用按键有： 
Qt.NoModifiers:没有修饰符被按下 
Qt.ShiftModifiers:Shift被按下 
Qt.ControlModifiers:Ctrl被按下 
Qt.AltModifiers:Alt被按下 
Qt.MetaModifiers:Meta键被按下 
Qt.KeypadModifiers:小键盘按钮被按下
```
Rectangle{
    width:!00;
    height:100;
    color:"green"
    MouseArea{
        anchors.fill:parent;
        acceptedButtons:Qt.LeftButton|Qt.rightButton
        onClicked:{
            if(mouse.button==Qt.RightButton)
                parent.color="blue"
            else
                parent.color="red"
        }
        onDoubleClicked:{
            if(mouse.button==Qt.LeftButton)
                &&(mouse.modifiers&Qt.ShiftModifiers)
                    parent.color="green"
        }
    }
}
```
## 鼠标滚动：WheelEvent
使用鼠标滚轮事件。 
onWheel处理器有一个wheel参数，就是WheelEvent类型的。 
最重要的一个属性是angleDelta，可以用来获取滚轮滚动的距离，它的x,y坐标分别保存了水平和垂直方向的增量。 
向上或者向右返回正值，向下或者向左返回负值。 
大多数鼠标，每当滚轮旋转一下是15°，此时angleDelta的值就是15*8=120。
滚轮向上滚动放大字号，向下缩小字号。
```
import QtQuick 2.2
Rectangle{
    width:360;
    height:360
    Text{
        id:myText;
        anchors.centerIn:parent;
        text:"Qt"
    }
    MouseArea{
        anchors.fill:parent;
        onWheel:{
            if(wheel.modifiers & Qt.ControlModifier){
                myText.font.pointSize+=1;
            }else{
                myText.font.pointSize-=1;
            }
        }
    }
}
```
## 拖放事件
MouseArea中的drag分组属性。
