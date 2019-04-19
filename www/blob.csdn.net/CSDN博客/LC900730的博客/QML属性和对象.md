# QML属性和对象 - LC900730的博客 - CSDN博客
2017年08月28日 23:47:55[lc900730](https://me.csdn.net/LC900730)阅读数：1173
### QML对象和属性
QML文档就是一个QML对象树。
```
import QtQuick 2.2
    Rctangle{
        width:400
        height:400
        color:'blue'
        Image{
            source:'pics/logo.png'
            anchors.centerIn:parent
        }
    }
```
在上述代码中导入了QtQuick模块，它包含各种QML类型。如果不使用这个import语句，那么无法使用Rectangle和Image类型。
### QML对象和属性
QML文档就是一个QML对象树。QML对象由它们的类型(type)指定，大写字母开头，后面跟随一对大括号。括号中包含了对象的特性定义，比如这个对象的属性值或者的其子对象。一旦使用了Rectangle，那么代码中的Rectangle便称为对象，它是对象类型的一个实例，一般对象都会指定具体的属性值。
#### 布局
Image中的anchors.centerIn起到了布局的作用。
#### 表达式
```
Item{
    width:300
    height:400
    Rectangle{
    //在表达式中包含了其他对象或属性的引用，这样做便创建一个绑定:当表达式的值改变时，以该表达式为值的属性会自动更新为新的值。
    //父属性的值改变，那么Rectangle的width就会自动改变。
        width:parent.width-50
        height:100
        color:'yellow'
    }
}
```
#### import导入语句
导入语句可以告诉引擎在QML文档中使用了哪些模块、js资源和组件。QML文档必须导入必要的模块或者类型的命名空间，以便QML引擎加载文档中使用到的对象类型。默认情况下，QML文档可以访问到与该.qml文件同目录下的对象类型。除此之外，如果QML文档还需要其他对象类型，就必须在import部分导入该类型的命名空间。
#### 目录导入
QML文档支持直接导入包含有QML文档的目录，即可以通过文件系统的目录将QML类型划分为可重复使用的分组。 
1.导入本地目录 
2.导入远程目录
```
如果需要导入远程目录，那么该目录需要包含一个qmldir文件(无后缀)。qml文件中罗列了目录中的文件列表
如myapp目录在远程的 http://www.my-example-server.com上，而在mycomponents目录中包含一个qmldir文件，内容如下
CheckBox   CheckBox.qml
DialoBox  DialogBox.qml
Slider   Slider.qml
这样使用URL来导入远程的mycomponents目录，如下面所示
```
```
import "http://www.my-example-server.com/myapp/mycomponnets" 
    DialogBox {
        //...
    }
    Slider{
    //...
    }
```
#### 目录清单qmldir文件
除了远程目录，本地目录页可以包含一个qldir文件，这样可以只暴露qmldir中指定的类型给导入该目录的客户端。另外，如果目录中的JavaScript资源没有声明在一个qmldir文件中，那么他们就不能暴露给客户端。
### JS类型
QML引擎直接支持Js对象和数组。任何标准的Js类型都可以在QML中使用var类型进行创建和存储。 
如：
```
import QtQuick 2.2
    Item{
        property var theArray: new Array()
        property var theDate: new Date()
        Component.onCompleted:{
            for(var i=0;i<10;i++){
                theArray.push("Item"+i)
            }
            console.log("Thers are ",theArray.length,"items in the array")
            console.log("the time is",theDate.toUTCString())
        }
    }
```
### 对象类型
QML对象类型用于QML对象的实例化。对象类型与基本类型最大区别是：基本类型不能声明一个对象，而对象类型可以通过指定类型名称并在其后的一组大括号里面包含相应属性的方式来声明一个对象。如Rectangle是一个QML对象类型，它可以用来创建Rectangle类型的对象。
```
1.创建.qml文件类定义类型；
    2.通过C++定义QML类型，然后在QML引擎中注册
```
### 对象特性
每一个QML对象类型都包含一组已经定义的特性。每个对象类型的实例在创建时都会包含一组特性，这些特性是在该对象类型中定义的。一个QML文档中的对象声明定义了一个新的类型，其中可以包含如下特性：id特性，属性特性，信号特性，信号处理器特性，方法特性，附加属性和附加信号处理器特性。
#### id特性
每一个对象都可以指定一个唯一的id，这样便可以在其他对象中识别并引用该对象。id在一个组件的作用域中必须是唯一的。
```
id是一个特殊的值，不是一个普通的对象属性。我们无法像普通属性那样使用text.id来获得这个值。一旦对象被创建，它的id值就无法被改变。id不是字符串，而是语音提供的一种数据类型。必须小写字母或者下划线开头，不能使用字母、数字和下划线以外的字符。
```
### 属性特性
属性是对象的一个特性，可以分配一个静态的值，也可以绑定一个动态表达式。 
1.定义属性特性。Q_PROPERTY宏，在注册到QML类型系统进行定义。此外还可以在QML文档中使用下面的语法自定义一个属性：
```
[default]property<propertyTYpe><propertyName>
使用这种机制可以很容易地将属性值暴露给外部对象或者维护对象的内部状态。
```
属性名字必须小写字母开始，Js保留字不能使用，第一个default修饰符是可选的，如果添加了default修饰符，说明我们正在定义的是一个默认属性。
```
声明一个自定义属性，会隐式为该属性创建一个值改变信号，以及一个相应的信号处理器。
```
如
```
import QtQuick 2.2
Rectangle {
    prototype color previousColor
    property color nextColor
    onNextColorChanged:
        console.log('...'); 
}
```
var类型：var类型是一种通用的占位符类型，可以包含任意类型的 值，包括列表和对象。
```
property var someNumber:1.5
property var someString:'abc'
property var someBool:true
property vat someList:[1,2,"three","four"]
```
### 信号和信号处理器特性
将信号与动态创建的对象关联起来，此时就不得不使用connect()函数进行连接。
QML中this可以在属性绑定中用于消除歧义。当使用Js创建一个属性时候，QML允许使用this关键字引用该属性绑定将要分配给的对象。
```java
Item{
    width:500
    height:500
    Rectangle{
        id:rect
        width:100
        color:'yellow'
    }
    Component.onCompleted:{
    //可以使用rect.width代替this.width
        rect.height=Qt.binding(function(){
            return this.width*2;
    })
        console.log('Rect.height='+rect.height)        //200
    }
}
```
### 导入外部的js文件
较为复杂的逻辑一般会分离到外部单独的Js文件中，这些文件可以使用import语句导入到QML文件中，就像导入QML模块一样。
```
import QtQuick 2.2
import "factorial.js" as MathFunctions
Item{
    MouseArea{
        nachors.fill:parent
        onClicked:console.log(MathFunctions.factorial(10))
    }
}
```
有时候需要从一个对象发射一个信号来触发另一个对象中的定义的函数，这时候就需要使用connect()函数。
```
import QtQuick 2.2
import "script.js" as MyScript
Item{
    id:item
    width:200;
    height:200;
    MouseArea{
        id:mouseArea
        anachors.fill:parent
    }
    Component.onCompleted:{
                 mouseArea.clicked.connect(MyScript.jsFunction)
    }
}
```
#### Component属性
每一个QML对象都包含一个附加的Component属性，它可以引用对象被实例化的组件。每一个Componnet都会发射一个onCompleted信号，其对应的onCompletted()处理器会在QML环境完全建立以后执行。在onCompleted()中的脚本代码就可以实现在启动时候运行。
```java
import QtQuick 2.0
    Rectangle{
        function startupFunction(){
        //...startup code
        }
        Component.onCompleted :startupFunction();
    }
```
