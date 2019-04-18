# Qt开发：Qt Widgets模块——简介 - 一世豁然的专栏 - CSDN博客





2018年05月15日 08:29:35[一世豁然](https://me.csdn.net/Explorer_day)阅读数：4311








Qt Widgets模块提供了一组UI元素来创建经典的桌面风格的用户界面。 请参阅用户界面概述以获取有关使用Widgets 的更多信息。




一、概述

要包含模块类的定义，请使用以下指令：


```cpp
#include <QtWidgets>
```

要链接模块，请将以下行添加到您的qmake .pro文件中：
`  QT += widgets`



二、Widgets

Widgets是在Qt中创建用户界面的主要元素。 Widgets可以显示数据和状态信息，接收用户输入，并为应该组合在一起的其他小部件提供容器。 未嵌入到父窗口小部件中的窗口小部件称为窗口。


![](https://img-blog.csdn.net/20180515082652578)


QWidget类提供了渲染到屏幕以及处理用户输入事件的基本类。 Qt提供的所有UI元素都是QWidget的子类，或者与QWidget子类一起使用。 通过继承QWidget或合适的子类并重新实现虚拟事件处理程序来完成创建自定义窗口小部件。





三、Styles

样式代表小部件绘制并封装GUI的外观和感觉。 Qt的内置小部件使用QStyle类来执行几乎所有的绘图，确保它们看起来与等效的本地小部件完全相同。


![](https://img-blog.csdn.net/20180515082718748)


Qt样式表是一种强大的机制，除了通过子类化QStyle已可实现的功能之外，还允许您自定义小部件的外观。





四、Layouts

布局是一种优雅而灵活的方式，可以在其容器中自动安排子部件。 每个小部件通过sizeHint和sizePolicy属性将其大小要求报告给布局，并且该布局相应地分配可用空间。


![](https://img-blog.csdn.net/20180515082752747)


Qt Designer是一个强大的工具，用于在布局中交互式创建和排列小部件。





五、Model/View类

模型/视图体系结构提供了管理数据呈现给用户的方式。 使用列表和表格的数据驱动的应用程序被构造为使用模型，视图和委托来分离数据和视图。




![](https://img-blog.csdn.net/20180515082817184)





六、Graphics View

Graphics View Framework用于管理和交互大量定制的2D图形项目，以及用于可视化项目的视图窗口小部件，支持缩放和旋转。


![](https://img-blog.csdn.net/20180515082853354)





