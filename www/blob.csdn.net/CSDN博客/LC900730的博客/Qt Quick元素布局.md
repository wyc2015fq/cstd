# Qt Quick元素布局 - LC900730的博客 - CSDN博客
2017年08月22日 16:46:14[lc900730](https://me.csdn.net/LC900730)阅读数：170
## 元素布局
- Item Positioner(定位器)
- Item Layout(布局)
- 锚布局
### 定位器
定位器是一种容器元素，专门用来管理界面中的其他的元素。
##### ROW
##### Column
##### Grid
##### Flow
### ExclusiveGroup
ExclusiveGroup(互斥分组)本身是不可见元素，用于将若干个元素组合在一起，供用户选择其中的一个选项。 
你可以在ExclusiveGroup对象中定义RadioButton、CheckBox、Action等元素，此时不需要设置他们的 exclusiveGroup属性；也可以定义一个只设置了id属性 的ExclusiveGroup对象，在别处定义的RadioButton、CheckButton、CheckBox、Action等元素时通过id初始化这些元素的exclusiveGroup属性。 
current属性指向互斥分组中第一个选择的元素。
#### RadioButton
要使用RadioButton，需要导入Controls模块。 
RadioButton用于多选一的场景，使用时需要通过exclusiveGroup属性为其指定一个分组。也可以和GroupBox结合使用。 
- text属性存储单选按钮文本：单选按钮还有一个指示选中与否的小图标，一般显示在文本前面
- checked属性指示RadioButton是否被选中，也可以设置它选中或取消选中。
- hovered是只读属性，指示鼠标是否悬停在RadioButton上。
- pressed属性在按钮被按下时候为true；当单选按钮被按下时，activeFocusOnPress属性为true，按钮获得焦点
如果点击了一个单选按钮，则会触发clicked()信号。
```
Rectangle{
        id:resultHolder;
        color:"#a0a0a0";
        width:200;
        visible=false;
        z:2;
        opacity:0.8;
        Text{
            id:result;
            anchors.centerIn:parent;
            ...
            ...
        }
    }
    ExclusiveGroup{
        id:mos;
    }
    Component{
        id:radioStyle;
        RadioButtonStyle{
            indicator:Rectangle{
                implicitWidth:16;
                implicitHeight:12;
            }
            Rectangle{
                anchors.fill:parent;
                visible:control.checked;
            }
        }
    }
    RadioButton{
            id:ios;
            text:"ios";
            exclusiveGroup:mos;
            checked:true;
            onClicked:resultHolder.visible=false;
    }
```
### CheckBox
Checkbox是复选框,可以在一组选项中选择一个或多个选项，这些选项之间互不影响。相比RadioButton，CheckBox多了2个属性：partiallyCheckedEnabled属性指示是否允许部分选中状态，默认为false。checkedState记录选中状态，它的值可能是Qt.UnChecked、Qt.Checked或者Qt.PartiallyChecked。
### ComboBox
组合框，由一个列表和一个标签控件(编辑控件)组成。列表框部分可以是一直显示的，也可以是隐藏的，在用户点击编辑控件边上的按钮(下拉箭头)时显示下拉列表框。列表框中当前选中的项(如果有的话)显示在标签控件或编辑控件中。 
Qt Quick提供的是ComboBox实际上是一个下拉列表框，列表默认是隐藏的。 
ComboBox的下拉列表是使用Menu实现的，列表内的每个条目对应一个MenuItem。 
editable属性决定下拉列表框的编辑控件是否可以编辑，默认为false，为true时可以编辑，此时editText保存编辑框的内容，同时可以设置validator属性来限制用户可以输入的文本。当编辑完成之后，ComboBox会发射accepted信号。
