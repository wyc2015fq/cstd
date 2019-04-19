# Qt之createObject - LC900730的博客 - CSDN博客
2017年08月30日 19:14:04[lc900730](https://me.csdn.net/LC900730)阅读数：615
## 动态创建对象
1:Qt.createComponent(); 
2:Qt.createQmlObject()函数，从一个QML字符串创建一个对象。 
第一种方法适用于已经有一个QML文档定义的组件，而且希望动态创建该组件的一个实例； 
如果QML本身是在运行时候产生的，那么可以使用第二种方法。
### 动态创建一个组件
要动态加载定义在一个QML文件中的组件，可以调用Qt全局对象中的Qt.createComponnet()函数。需要将QML文件的URL作为参数，然后从这个URL上创建一个Component对象。一旦有了一个Component，就可以调用它的createObject()方法来创建该组件的一个实例 
原型为：
```
object createObject(Item parent, object properties);
component=Qt.createComponent("Sprite.qml");
if(component.status==Component.Ready)
{
    //...
    在create
}
```
### 属性别名
//属性别名在整个组件初始化完成之后才可用 
//报错，因为代码执行到这里时候，buttonText还是一个未定义的值 
property alias buttonText2: root.buttonText 
正确的初始化方法是 
Component.onCompleted: buttonText=”some text”
```
Rectangle{
    //buttonText属性别名指向其Text子对象的text属性。
    //对buttonText的修改都会直接反应到textIetm.text
    property alias buttonText :textItem.text
    width:100;
    height:30;
    color:"yellow"
    Text{
        id:textItem 
    }
}
```
