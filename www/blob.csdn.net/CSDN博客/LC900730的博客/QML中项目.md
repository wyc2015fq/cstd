# QML中项目 - LC900730的博客 - CSDN博客
2017年08月29日 14:40:08[lc900730](https://me.csdn.net/LC900730)阅读数：481
### 1.Item
Qt quick的所有可视项目都继承自Item。虽然单独的Item对象没有可视化外观，但是它定义了可视化项目所有通用的特性，如x，y，width，height等属性、关于布局的anchors相关属性、和关于按键处理的Keys属性等。
#### 1.1作为容器
Item常用于对项目进行分组。在一个根项目下，使用Item对象组织其他项目。
```bash
Item{
    Image{
        x:80
        width:100
        height:100
        source:'tile.png'
    }
    Rectangle{
        x:190
        width:100
        height:100
    }
}
```
#### 1.2默认属性
Item有一个children属性和一个resource属性，前者一般包含可见的孩子列表，后者包含了不可见的资源。
```
Item{
    children:[
        Text{},
        Rectangle{}
    ]
    resource:[
        Timer {}
    ]
}
```
Item还有一个data默认属性，允许在一个项目中将可见的孩子和不可见的资源进行自由混合。也就是说向data列表中添加一个可视项目，那么该项目将作为一个孩子进行添加。如果添加任何其他的对象类型，那么该项目则会作为资源进行添加。因为data是默认属性，所以可以省略data标签。
```
Item{
    Text{}
    Rectange{}
    Timer{}
}
```
简单来说，就是在实际编程中不需要考虑children和resource属性，可以直接向一个项目中添加任何子项目或者资源即可。
#### 1.3不透明度
Item有个pacity属性，可以用来设置不透明度。 
opacity是一个继承属性，父项目的透明度会应用到子项目中。
#### 1.4可见与启用
Item的visible属性用来设置项目是否可见，默认值是true。 
设置一个项目的visible属性会直接影响其子项目的可见性，除非单独设置子项目的visible属性。如果将该属性设为false，那么项目将不再接收鼠标事件，但是可以继续接收键盘事件。 
Item还有一个enabled属性，它可以设置项目是否接收鼠标和键盘事件，其值默认是true。
#### 1.5堆叠顺序
默认的堆叠顺序拥有较大z值的项目会出现在z值较小的兄弟项目上。
#### 1.6定位子项目和坐标映射
Item中提供了childAt(real x,real y)函数来返回点(x,y)处的子项目，如果没有则返回NULL。Item的mapFromItem(Item item,real x,real y)函数会将item坐标系统中点(x,y)映射到该项目的坐标系统上，该函数会返回一个包含映射后的x，y属性对象。如果item被指定为null，那么会从根QML视图的坐标系统上的点进行映射。对应的还有一个mapToItem(Item item,real x,real y)函数，它与mapFromItem()类似，只不过是从当前项目坐标系统的(x,y)点映射到item的坐标系统。
### 2.Rectangle
Rectangle项目继承自Item，用来使用纯色或者渐变填充一个矩形区域，并提供一个边框。每一个Rectangle项目可以使用color属性指定一个纯色填充，或者使用gradient属性指定一个Gradient类型定义的渐变来填充。如果既设置了color又设置了gradient，那么最终会使用gradient。 
boder.color:颜色 
border.width:宽度
还可以使用radius属性来产生一个圆角矩形。
### 3.Text
Text项目可以显示纯文本或者富文本。
```
import QtQuick 2.2
Column{
    Text{
        text:"hello world";
        font.family:"Helvetica";
        font.pointSize:24;
        color:"red"
    }
    Text{
        text:"<b>Hello</b><i>world</i>"
    }
}
```
Text项目通过text属性设置要显示的文本，并且可以自动判断是否以富文本的形式进行显示。如果没有使用wrapMode属性设置换行，那么所有的文本都会被放置于单行上。对于设置了宽度并且只想在单行中显示纯文本，使用elide属性。它可以为超出宽度的 文本提供自动省略显示(即使用。。。来表示省略)。 
实际编程中使用Qt Quick Controls模块中的Label项目代替Text。
### 超链接信号
```
Item{
    width:400
    height:100
    Text{
        textFormat:Text.RichTexgt;
        font.pointSize:24;
        text:"欢迎访问<a href=\"http://qter.org\">开源社区</a>"
        onLinkActivated:console.log(link+"link activated");
        函数中link字符串提供了被单击的特定链接。
    }
}
```
### 4.TextInput
TextInput项目用来显示单行可编辑的纯文本。TextInput与Qt中的QLineEdit相似，可以接收单行文本输入。在一个TextInput项目上可以使用输入限制，如使用验证器validator或者掩码inputMask。通过设置echoMode，可以将TextInput应用于输入密码。
### 4.1信号处理器
TextInput提供了2种信号处理器：onAccepted()和onEditingFinished(),它们会在回车键按下时被调用，区别是后者在TextInput失去焦点时候也会调用，而前者不会。
## 5.布局管理
### 5.1定位器
定位器是一个容器，可以管理其中子项目中的布局。定位器包含Column、Row、Grid和Flow。
### 5.2Column
Column可以将其子项目排成一列。 
spacing属性用来为这几个Rectangle添加间距。
```
import Qtquick2.2
Column{
    spacing:2
    Rectangle{color:'red';width:50;height:50}
    Rectangle{color:'green';width:20;height:50}
    Rectangle{color:'blue';width:50;height:20;
}
```
### 5.3Row
Row可以将其子项目排成一行。 
spacing属性用来添加间距。
### 5.4Grid
Grid项目可以将其子项目排列在一个网格中。Grid会计算一个足够大的矩形网格来容纳所有的子项目。向网格中添加项目，会按照从左到右、从上到下的顺序进行排列。 
一个Grid默认有4列，可以有无限多的行容纳所有的子项目。行数和列数也可以通过rows和columns属性指定。Grid也可以通过spacing属性设置子项目之间的间距。水平和垂直方向使用相同间距。如果需要分别设置，则使用rowSpacing和columnSpacing属性。
### 5.5Flow
Flow项目可以从前向后，像流一样布局其子项目。
### 5.6Repeater
用来创建大量相似的项目。一个Repeater包含一个模型model属性和一个委托delegate属性。委托用来将模型中的每一个条目进行可视化显示。 一个Repeater通常包含在一个定位器中，用于直观对Repeater产生的众多委托项目中进行布局。 
如：
```
import QtQuick 2.2
Rectangle{
    width:400;height:240;color:"black"
    Grid{
        x:5;y:5
        rows:5;columns:5;spacing:10
        Repeater{
            model:12
            Rectangle{
                width:70;height:70;color:"lightgreen"
                Text{
                    text:index;font.pointSize:30
                    anchors.centerIn: parent
                }
            }
        }
    }
}
```
### 6.基于锚的布局
每个项目都有一组无线的“锚线”：left、horizontalCenter、right、top、verticalCenter、baseline和bottom.
### 7.事件处理
#### 7.1MouseArea
MouseArea是一个不可见的项目，通常用来和一个可见的项目配合使用，为可视项目提供鼠标处理。鼠标处理的逻辑完全包含在这个MouseArea项目中。
- enabled:用来设置是否启用鼠标处理，默认为true。如果设置为false，MouseArea对于鼠标事件会变得透明，也就是不在处理任何鼠标事件。
- 只读的pressed属性表明：用户是否在MouseArea上按住了鼠标按钮。
- 只读的containsMouse属性表示，当前鼠标光标是否在MouseArea上。
- onClicked()
- onDoubleClicked()
- onPressed()
- onPressAndHold()
- onWheel():则可以用来处理滚轮事件
默认情况下，MouseArea项目只报告鼠标单击而不报告鼠标光标的位置改变。这可以通过设置hoverEnabled属性为true来进行更改。这样设置之后，onPositionChanged()、onEntered()和onExited()等函数才可以使用，而且这时候containsMouse属性也可以在没有鼠标按钮按下的情况下检查光标。
#### 7.2MouseArea重叠的情况
如蓝色的矩形绘制在黄色矩形之上，而蓝色矩形的MouseArea设置了propagateComposedEvents为true，并且为clicked和doubleClicked事件的MouseEvent.accepted设置为false，所以蓝色矩形的鼠标单击、双击事件都会传播到黄色矩形。
#### 7.3MouseEvent和滚轮事件WheelEvent
Qt Quick的可视项目结合MouseArea获取鼠标的相关事件，并且通过信号和处理器与鼠标进行交互。大多数MouseArea信号都包含了一个mouse参数，它是MouseEvent类型 的，如mouse.accepted。
- 在MouseEvent对象中，可以设置accepted属性为true来防止鼠标事件传播到下一个项目中。
- 通过x和y属性可以获取鼠标的位置；
- 通过button或buttons属性可以获取按下的按键。有Qt.LeftButton;Qt.RightButton和Qt.MiddleButton
- 通过modifiers属性可以获取按下的键盘修饰符，而modifiers的值由多个按键进行组合而成，在使用时候需要将modifiers与这些特殊的按键进行按位与来判断按键，常用按键有： 
Qt.NoModifiers:没有修饰符被按下； 
Qt.ShiftModifiers:Shift被按下； 
Qt.ControlModifiers:Ctrl被按下； 
Qt.AltModifiers:Alt被按下； 
Qt.MetaModifiers:Meta被按下； 
Qt.KeypadModifiers:小键盘按钮被按下； 
#### WheelEvent
获取鼠标滚轮事件。 
MouseArea的onWheel处理器就有一个wheel参数，就是WheelEvent类型的。  WheelEvent最重要的一个属性就是angleDelta，可以用来获取滚轮滚动的距离，它的x，y坐标分别保存了水平和垂直方向的增量。滚轮向上或向右返回正值，向下或者向左滚动返回负值。
### 拖放事件(DragEvent)
MouseArea中的drag分组。
```
Recrangle{
    id:container
    width:600;height:200
    Rectangle {
        id:rect
        width:50;height:50
        MouseArea{
            anchors.fill:parent;
            drag.target=rect
            drag.axis:Drag.XAxis
            drag.minimunX:0
            drag.maximumX:container.width-rect.width
        }
    }
}
```
在实际编程中启动拖动并不是直接操作DragEvent,而是使用Drag附加属性和DropArea。当一个项目的Drag附加属性的active属性设置为true时候，该项目的任何位置变化都会产生一个拖动事件，并且发送给与这些项目新位置相互交互的DropArea。其他实现了拖放事件处理器的项目也可以接收这些事件。
DropArea是一个不可见的项目。
