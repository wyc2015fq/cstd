# Qt Quick学习笔记(六) - LC900730的博客 - CSDN博客
2017年08月21日 11:48:32[lc900730](https://me.csdn.net/LC900730)阅读数：177
## 第10章
### Canvas
QML中的canvas，俗称画布，它用来定义一个绘图区域，你可以使用ECMAScript代码来绘制直线、矩形、贝塞尔曲线、弧线、图片文字等图元，还可以为图元应用填充颜色和边框颜色，甚至还可以进行低阶像素级操作。
### Context2D
var ctx=getContext(“2d”);
### 画笔
笔有颜色，是为strokeStyle属性；笔有粗细，是为lineWidth属性。
### 画刷
在Context2D，fillStyle属性就是描述画刷的。
### 图元
基本图元有线、弧、矩形、曲线、文本、图片等。
## 基本绘图模式
Canvas是Item的派生类，通过设置width和height属性，就可以设定一个绘图区域，然后在onPaint()信号处理器内使用Context2D对象来绘图。
### PropertyAnimation对象
```
//定义一个PropertyAnimation对象。默认不启动。当你用鼠标左键点击蓝色矩形，通过设置running属性为true来启动动画，把蓝色矩形宽度设置为150，矩形变成正方形。
    Rectangle{
        id:rect;
        width:50;
        height:150;
        anchors.centerLn:parent;
        color:"blue";
        PropertyAnimation{
            id:animation;
            //设置target属性指定要操作的目标对象
            //即id=“rect”的矩形
            target:rect;
            //使用property属性指定要改变目标对象的那个属性
            property:"width";
            //使用to属性指定目标属性的目标值
            to:150;
            //设置duration为1000ms
            duration:1000;
        }
        MouseArea{
            anchors.fill:parent;
            onClicked:animation.running=true;
        }
    }
    //PropertyAnimation还有from属性，指定动画启动时候要改变的目标属性的初始值。如果不指定，动画对象会使用动画开始时候目标对象的目标属性当前值。
```
#### 改变多个属性
如果想改变多个属性，可以使用PropertyAnimation的properties属性来指定，属性名以英文逗号分隔，而这些属性共用to指定的目标值。所以目标属性最好是同一个类型property 。
PropertyAnimation{ 
    id:animation; 
    target:rect; 
    properties:”width,height”; 
    to:220； 
}
#### 同时改变多个目标对象
PropertyAnimation{ 
    id：animation; 
    targets:[rectA,rectB]; 
    properties: “width”; 
    to :150; 
    duration:1000; 
}
### NumberAnimation
是PropertyAnimation的派生类，专门处理数字类型的property，它重写了from和to2个属性，将其类型设置为real。
