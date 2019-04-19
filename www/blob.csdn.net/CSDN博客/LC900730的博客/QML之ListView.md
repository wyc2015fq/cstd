# QML之ListView - LC900730的博客 - CSDN博客
2017年09月08日 21:54:48[lc900730](https://me.csdn.net/LC900730)阅读数：465
### ListView
QML中的ListView、GridView、Repeater等元素需要数据模型来提供显示的数据，这些元素需要一个位模型中的每一项数据生成一个实例的代理组件(delegate component)。模型可以是静态的，也可以对其动态修改，插入，删除，移动。
给代理提供的数据，通常是叫做角色的数据绑定到代理，如下面的ListModel有2个角色，type和age，ListView带有一个代理，并绑定这些角色以显示他们的值。
```
Item{
    width:200;height:200;
    ListModel{
        id:myModel
        ListElement{type:"Dog";age:8}
        ListElement{type:"Cat";age:5}
    }
    Component{
        id:myDelegate
        Text{
            text:
                {type+","+age}
        }
    }
    ListView{
        anchor.fill:parent;
        model:myModel;
        delegate:myDelegate
    }
}
```
还可以直接在ListView中直接写上代理 
//delagate中每一行作为数据的代理
```
ListView{
    anchors.fill:parent
    model:fruitModel
    delegate:Row{
        Text{
            text:"fruit:"+type;
        }
        Text{
            text:"fruit:"+age;
        }
    }
}
```
ListModel提供了可以直接在Js中调用的方法。这时候第一次插入的项决定使用模型的视图可用的角色。 
如创建了一个空的ListModel，并在JS中填充，第一次插入时指定的角色将显示在视图中：
