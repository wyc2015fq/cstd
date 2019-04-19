# QML之Button - LC900730的博客 - CSDN博客
2017年09月15日 23:32:10[lc900730](https://me.csdn.net/LC900730)阅读数：170
个人分类：[qml](https://blog.csdn.net/LC900730/article/category/7141083)
## 初始化Button
既可以使用Action初始化，也可以使用自己的属性； 
如：
```
Button{
    x:10;
    y:10;
    text:qsTr("Button");
}
```
Button类型由一个menu属性，可以为这个按钮添加菜单
```
Button{
    x:10;
    y:10
    text:qsTr("Button with menu")
    menu:Menu{
        MenuItem{
            text:qsTr("Item1");
        }
        MenuItem{
            text:qsTr("Item2");
        }
    }
}
```
Button也提供checkable和checked属性。 
Button的exclusiveGroup属性可以将结果Button对象组成一个互斥的组，当其中之一选择时候，其余自动取消选择。
