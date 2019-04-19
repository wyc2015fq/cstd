# QML之CheckBox与ComboBox - LC900730的博客 - CSDN博客
2017年09月15日 23:38:34[lc900730](https://me.csdn.net/LC900730)阅读数：872
checkbox启用部分选中 
partiallyCheckedEnabled：true 
CheckBox的exclusiveGroup属性可以将几个CheckBox对象组成一个互斥的组，当其中之一被选中时候，其余会自动取消选择。
### ComboBox
```
ApplicationWindow{
    visible:true;
    width:480;
    height:480;
    ComboBox{
        width:200;
        currentIndex:2;
        model:ListModel{
            id:cbItems
        ListElement{
            text:"Banana";
            color:"Yellow"
        }
        ListElement{
            text:"Apple";
            color:"Green"
        }
        ListElement{
            text:"Coconut";
            color:"Brown"
        }
    }
        onCurrentIndexChanged:{
        console.log(
            cbItems.get(currentIndex).text
         +","+
         cbItems.get(currentIndex).color)
        }
    }
}
```
创建一组ListModel对象，ListModel由一组ListElement对象构成。每一个ListElemnet都可以设置一个text属性，作为显示的文本，另外还可以添加额外的用户数据，用于对这个数据的描述，如我们这里添加ListElement对象增加了一个color属性。
- editable:设置这个ComboBox是不是可以编辑，将其设置为true的时候，下拉框允许用户编辑数据。
在用户输入时候，ComboBox会自动显示下拉框中匹配的数据，还可以通过accepted信号确定是否可以将用户输入的数据添加到下来框中。不过在添加新数据的时候，需要确定下拉框中没有重复项。
当输入完一个字符串并按下回车键，会执行accepted信号。 
如
```
ComboBox{
    id:editableCombo;
    editable: true
    model: ListModel{
        id:model;
        ListElement{
            text:"banana";color:"Yellow"
        }
        ListElement{
            text:"apple";color:"Green"
        }
        ListElement{
            text:"Coconut";color:"Brown"
        }
    }
    onAccepted: {
        if(editableCombo.find(currentText)===-1){
            model.append({text:editText})
            currentIndex=editableCombo.find(editText)
        }
    }
}
```
