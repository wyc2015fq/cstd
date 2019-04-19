# QML小结1-（一个简单的qml程序） - xqhrs232的专栏 - CSDN博客
2017年07月26日 10:21:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：221
原文地址::[http://blog.csdn.net/qq_31793791/article/details/53009314](http://blog.csdn.net/qq_31793791/article/details/53009314)
相关文章
1、我的第一个QML小程序----[http://download.csdn.net/detail/z609932088/9802022](http://download.csdn.net/detail/z609932088/9802022)
2、[QML基础——在C++程序中使用QML](http://blog.csdn.net/playstudy/article/details/7835882)----[http://blog.csdn.net/playStudy/article/details/7835882](http://blog.csdn.net/playStudy/article/details/7835882)
qml又称qt quick。Qt
 Quick是一种高级用户界面技术。使用它可轻松地创建供移动和嵌入式设备使用的动态触摸式几面和轻量级应用程序。三种全新技术共同构成了Qt Quick用户界面：一个改进的Qt
 Creator IDE、一种新增的简便易学的语言（QML）和一个新加入Qt库名为QtDeclarative的模块，这些使得Qt更加便于不熟悉C++的开发人员和设计人员使用。
1.scene情景工具：
是qt内置的查看单个qml文件运行结果的exe程序，可以在qt的bin目录下找到，名为qmlscene.exe；打开后会直接进入文件选择，此时选择定位到相应qml文件，既可以查看运行效果。
2.一个简单的Hello程序
<1>新建Qt选择qt
 quick application项目
<2>如果是qt5.4及以上，会提示是否建立对面的界面文件，可以选择，也可以不选择。
<3>如果选择了界面文件，会默认生成如下的main.qml文件
importQtQuick2.4importQtQuick.Window2.2Window{visible:truewidth:640height:480title:**qsTr**("HelloWorld")MainForm{anchors.fill:parentmouseArea.onClicked:{**Qt**.quit();}}}
运行会发现屏幕的正中显示了hello
 world字样，但是很显然这不是我们想要的hello。此时如果修改title中的hello world，会发现运行的程序只是标题变了，而内容不变。仔细观察，发现下面应用了一个MainForm对象，可以尝试打开它，会发现有文本编辑模式和界面模式两种，两种状态都是可以修改界面的。
但是如果要在单个qml，也就是我们的main.qml中生成打印hello的
程序，此时，就需要删掉MainForm部分，修改为如下代码：
Text{text:"hello";anchors.centerIn:parentcolor:"cyan"font.pointSize:30}此时运行就发现正确了，但是我们界面文件在此时是没有调用的。。。。（个人觉得依照个人的习惯选择两种方式就行）
