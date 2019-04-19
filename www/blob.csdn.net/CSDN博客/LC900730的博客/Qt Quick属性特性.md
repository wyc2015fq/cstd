# Qt Quick属性特性 - LC900730的博客 - CSDN博客
2017年09月13日 23:42:33[lc900730](https://me.csdn.net/LC900730)阅读数：198
### QQmlApplicationEngine
加载QML文件。这个类主要提供一种简易的方式，将一个QML文件加载到正在运行的程序中。因为项目中自动生成的QML文件被放到资源文件中，因此使用qrc:///前缀，从资源文件中加载。
id是个特殊对象属性，我们无法使用XX.id来获取这个值。
## 1.定义属性
1.可以在C++中通过注册一个类的Q_PROPERTY宏，注册到QML系统定义。 
2.还可以
```
[default] property<propertyType><propertyName>
//使用这种机制可以很容易将属性值暴露给外部对象或危害对象的内部状态
```
声明自定义属性，会隐式为该属性创建一个值改变信号，以及一个相应信号处理器`on<PropertyName>Changed`。
var是一种通用的占位符类型，类似于QVariant，它可以包含任意类型的值，包含列表和对象。
```
property var someNumber:1.5
property var someString:"abc"
property var someBool: true
property var someList:[1,2,"three","four"]
property var someObject:Rectangle[width:100;height:100;color:"red"}
//QML对象类型也可以作为一个属性类型
property Item someItem;
property Rectangle someRectangle
//还可以将自己定义的对象类型作为属性类型使用
```
## 初始化和赋值
### 初始化赋值
QML属性的值可以通过初始化或者赋值操作来给出。 
如：property color nextColor:”blue”
### 代码中赋值
使用赋值运算符号；
```
Rectangle{
    id:rect
    Component.onCompleted:{
        rect.color="red";
    }
}
```
### 有效属性值
静态值和绑定值。
```
Rectangle{
    width:400
    height:200
    color:"red"
    Rectangle{
        //使用绑定表达式初始化
        width:parent.width/2;
        height:parent.height;
    }
}
```
### 列表属性
可以将一个QML对象类型值列表赋值给一个list类型的属性。
```
//Item类型有一个states属性，用于保存一个State类型对象的列表。
Item{
    states:[
        State{name:"loading"},
        State{name:"running"},
        State{name:"stopped"},
    ]
//只有一个时候可以省略方括号
}
```
### 列表类型初始化
```cpp
//Item类型有一个states属性，用于保存一个State类型对象的列表。Rectangle{
    //只声明，不初始化
    property list<Recatngle> siblingRects;
    //声明并且初始化
    property list<Recatngle> childRects:[
        Rectangle{color:"red"},
        Rectangle{color:"blue"},
    ];
    MouseArea{
        anchors.fill:paren
        onClicked:{
            for(var i=0;i<childRects.length;i++){
            console.log("color",i,childRects[i].color)
            }
        }
    }
}
```
### 分组属性
如font属性
```
font.pixelSize;
font.bold:true;
font{
    pixelSize:12;
    bold:true;
}
```
## 属性别名
