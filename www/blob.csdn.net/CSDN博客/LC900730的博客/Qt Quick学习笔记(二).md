# Qt Quick学习笔记(二) - LC900730的博客 - CSDN博客
2017年08月20日 18:18:09[lc900730](https://me.csdn.net/LC900730)阅读数：256
## 第四章
```
#include<QGuiApplication>
#include<QQmlApplicationEngine>
int main(int argc,char * argv[]){
    QGuiApplication app(argc,argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app.exec();
}
```
定义了一个QQmlApplicationEngine实例，代表QML引擎，然后load()放在qrc的主QML文档main.qml中。
## QML文档
一个QML文档由2部分组成 
- import语句
- QML对象树
import QtQuick2.2这行代码引入了2.2版本的QtQuick模块，部分基基础的QML类型，如Text，Rectangle、Item、Row等，都包含在这个模块中。
import QtQuick.Window 2.1 导入了2.1版本的Window模块，Window模块包含的主要类型是Window，代表的是QML应用的顶层窗口，对应的C++类型是QQuickWindow。
Qt Quick App模板创建Qt Quick App，自动生成的main.qml文档，就是使用Rectangle作为根对象的。main.cpp，则使用QQuickView(QQuickWindow的派生类)来加载QML文档。
启动Qt Quick App模式 
- QQmlApplicationEngine搭配Window
- QQuickView搭配Item
两者不同之处在于:使用QQuickView显示QML文档，对窗口的控制权(比如设置窗口标题、Icon、窗口的最小尺寸)在C++代码；而使用QQmlApplicationengine加载以Qindow为根对象的QML文档，QML文档则拥有窗口的完整控制权，可以直接设置标题、窗口尺寸等属性。
#### 渐变色
QML渐变色的类型是Gradient，渐变色通过两个或者多个颜色值来指定，QML会自动在你指定的颜色之间插值，进行无缝填充。Gradient使用GradientStop来指定一个颜色值和它的位置(取值在0.0与1.0之间)
```
Rectangle{
    width:100;
    height:100;
    gradient:Gradient{
        GradientStop{
            positon:0.0;
            color:"#202020";
        }
        GradientStop{
            positon:0.33;
            color:"blue";
        }
        GradientStop{
            positon:1.0;
            color:"#FFFFFF";
        }
    }
}
```
在Qt5.0中，只有垂直方向的线性渐变可以用于Item。
### Item
Item是所有可视元素的基类，虽然自己不绘制，但是定义绘制图元所需要的大部分通用属性，如x、y、width、height、锚定(anchoring)和按键处理。 
除了x、y属性外，还有z属性，指定图元的Z序。z属性类型是real，值越小，越垫底。 
opacity指定图元的透明度。
- clip属性：目的是根据自己的位置和大小来裁切它自己以及孩子们的显示范围，如果clip为true，那么孩子们都在父的手掌心。如果为false，那么有些孩子就可能升除父item框外
虽然Item本身不可见，但是可以使用item可以分组其他的可视图元。 
如：
```
Rectangle{
    width:300;
    height:200;
    Item{
        id：gradientGroup;
        Rectangle{
            x:20;
            y:20;
            width:120;
            height:120;
            gradient:Gradient{
                GradientStop{position:0.0;color:"blue";}            GradientStop{position:0.0;color:"black";}
            }
        }
        Rectangle{
            x:160;
            y:20;
            width:120;
            height:120;
            rotation:90;
            gradient:Gradient{
                GradientStop{position:0.0;color:"blue";}            GradientStop{position:1.0;color:"black";}
        }
    }
}
Component.onCompleted:{
    console.log("visible children:",gradientGroup.visibleChildren.length);
    console.log("visible children:",gradientGroup.children[i].x);
}
```
### 锚布局
x、y、width、height属性绝对布局方式，不够灵活。 
anchors可以通过指定一个元素与其他元素的关系来确定元素在界面中的位置。
## 响应按键
所有从Item继承的元素都可以处理按键。 
Item通过附加属性Keys来处理按键。Keys对象是Qt Quick提供的。
### 使用Button
QML中Button与QPushButton类似，用户点击会触发clicked()信号。 
要使用Button,需要引入import QtQuick.Controls 1.x
### ButtonStyle
对于多个ButtonStyle，如果有多个按钮同时用到，可以使用Component在QML文档内定义一个组件，设置id属性值为btnStyle，然后在Button中设定style属性时直接使用btnStyle
```
Rectangle{
        width:300;
        height:200;
        Component{
            id:btnStyle;
            ButtonStyle{
                background:Rectangle{
                    implicitWidth:70;
                    implicitHeight:25;
                    color:"black";
                    border.width:control.pressed?2:1;
                    }
            }
        }
        Button{
            ...
            style:btnStyle;
        }
        Button{
            ...
            style:btnStyle;
        }
    }
```
### Image
只能显示静态图片，对于GIF等格式，只能显示第一帧。 
如果要显示动画，则可以使用AnimatedSprite或者AnimatedImage。
Image默认会堵塞加载图片。可以设置asynchronous属性为true来开启异步加载模式，在这种模式下Image使用一个线程来加载图片。可以放加载图标，然后当status(枚举值)的值为Image.Ready时在隐藏加载等候界面。
注意：Image支持从网络加载图片，当source属性类型是url，可以接受http、ftp等。识别是网络资源时，会自动启用异步加载模式。此时Image的progress(取值范围是0.0-1.0)、status(枚举值)都会实时更新，根据值可以判断何时加载结束。
```
//加载网络上图片
Rectangle{
    BusyIndiccator{
        id:busy;
        running:true;
        anchors.centerIn:parent;
        z:2;
    }
    Image{
        id:imageViewwr;
        //对于网络图片，默认就是异步加载，这个属性设置不生效
        asychronous:true;
        cache:false;
        onStatusChanged:{
            if(imageViewer.status===Image.Loading){
                busy.running=true;
                stateLabel.visible=false;
            }
            else if(imageViewer.status===Image.Ready){
                busy.running=false;
            }
            else if(imageViewer.status===Image.Error){
                busy.running=false;
                statusLabel.visible=true;                                     
                statusLabel.text="ERROR";
            }
            }
        }
    Component.onCompleted:{
        imageViewer.source="XXX";
    }
}
}
```
### BusyIndicator
BusyIndicator用来显示一个等待图元，在进行一些耗时操作的时候使用其来缓解用户的焦躁情绪。
BusyIndicator的running属性是布尔值，为true时显示。style属性允许你定制BusyIndicator。默认是转圈圈的动画。虽然BusyIndicator只有running和style属性，但是它的祖先有很多属性，如anchors、z都是从Item继承来的属性，可以直接使用。
