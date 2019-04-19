# QML组件之Component - LC900730的博客 - CSDN博客
2017年08月31日 16:36:38[lc900730](https://me.csdn.net/LC900730)阅读数：635
### 使用Component类型
除了使用单独的QML文件，还可以使用Component类型在一个 
QML文档中定义一个组件。 
如：
```
import QtQuick 2.2
Item {
    width:100;height:100
//定义的Component必须包含一个唯一的根对象
//如这里是Rectangle
    Component {
        id:redSquare;
        Rectangle{
            color:"red";
            width:10;
            height:10
        }
    }
    Loader{
        sourceComponent:redSquare;
    }
    Loader{
        sourceComponent:redSquare;x:20
    }
}
```
Component类型一般是为视图提供图形组件，如ListView::delegate属性需要一个Component指定它的每一个列表项需要怎么样显示。 
还可以使用Qt.createComponent()来创建Component。Component创建上下文(context)对应于Component声明处的上下文。当一个组件被ListView或Loader这样对象实例化之后，这个上下文就是父对象的上下文。
