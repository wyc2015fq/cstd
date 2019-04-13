
# 使用Qt Designer -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:25:07[seven-soft](https://me.csdn.net/softn)阅读数：596


上节我们创建了一个简单的`Qt
 Widgets Application`项目，并使用C++代码设计出了简单的界面。这是传统的GUI程序开发方式，只使用C++代码，C++既负责设计界面，也负责处理业务逻辑。
界面是用户能够直接看到的，称为“前端”；业务逻辑的处理用户看不到，在背后默默进行，称为“后台”。
传统的GUI程序开发不区分前端和后台，统一使用C++代码管理，源文件中既有创建和设置控件的代码，又有处理业务逻辑的代码，非常杂乱。使用C++代码直接创建控件，不管是从语法上讲还是从代码量上讲，看起来都比较“麻烦”。如果你有Web开发经验，这种体会就更加深刻，网页就是将前端和后台无缝集成的经典例子。
> 在网页中，我们能看到的各种文字、颜色、图片、布局、按钮、菜单、列表等界面元素都是使用HTML+CSS（专门用来设计界面的声明式语言）创建的，而后台逻辑才需要JavaScript、Python、PHP、Java等编程语言来处理。
现代GUI程序的设计，或许也借鉴了Web开发的思想，可以使用 XML 来设计界面，使用C++来处理后台逻辑，在 Qt 中可以轻而易举的做到前端和后台分离，从而让代码逻辑更加清晰，开发更加高效。
## 使用Qt Designer
Qt Designer 是与 Qt 配套的界面设计工具，使用它可以轻而易举的拖拽出简单的界面，这节我们就来学习一下。
上节中讲到，创建 Qt Widgets Application 项目的第四步是填写类信息，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120FS354563.png)
“创建界面”默认是被勾选的，为了简单起见，上节我们取消了该选项，目的是向大家展示如何使用纯C++代码来创建界面。
这节我们需要重新创建一个新的 Qt Widgets Application 项目，命名为 WidgetsDemo-UI，并将“创建界面”选项勾选，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120GA433C1.png)
项目创建完成后可以看到，勾选了“创建界面”选项，IDE 会多生成一个`mainwindow.ui`文件，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120GATQK.png)
双击 mainwindow.ui，就可以进入设计模式，启动 Qt Designer，如下图所示（点击图片查看大图）：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120GG532D3.png)
再切换回编辑模式，就可以看到 mainwindow.ui 的源代码，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120GH1012J.png)
`.ui`文件是
 Qt Designer 的界面设计文件，由 XML 代码构成。Qt Creator 不允许在编辑模式下修改 .ui 文件，只能切换到设计模式由 Qt Designer 自动生成。
XML是一种标记语言（也称声明式语言），由一个一个地节点组成，每个节点还可以包含多个属性，HTML 就是 XML 的一种具体化。
> XML 是程序员必备的技能，也很简单，开发中会经常用到，可以不熟练，但要有所了解。该教程不打算细讲 XML，请读者自行脑补。
在编辑区看到的界面，仅包含了程序的客户区，没有包含标题栏，所以不会看到最大化、最小化、关闭等常见按钮。新创建的程序，默认包含了菜单栏、工具栏和状态栏。如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151208/1-15120Q349344G.png)
本节暂时不会涉及这些，可以在菜单栏、工具栏和状态栏单击鼠标右键，在弹出菜单中删除即可。如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151208/1-15120Q401521F.png)
大家可以从左侧的控件箱中拖拽几个控件到编辑区的窗口中，亲自测试一下效果。如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151208/1-15120Q6403E61.png)
左侧控件箱中的控件都可以用鼠标拖到编辑区域，上图是笔者拖拽出的一个简单的登陆框。运行结果为：![](http://c.biancheng.net/cpp/uploads/allimg/151208/1-15120Q64301M1.png)

