# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之42---BUIW 介绍 - 我相信...... - CSDN博客





2010年02月04日 11:12:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1450标签：[brew																[decorator																[mvc																[设计模式																[forms																[程序开发](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)







在BUIW中采用了MVC的设计模式（图10-3），MVC(Model
/View/Controller)是程序开发中经常使用的一种模式,它很好地实现了数据层与表示层的分离。MVC把应用程序分为三种对象类型：模型,维护数据并提供数据访问方法；视图,绘制模型的部分数据或所有数据的可视图；控制器处理事件。视图事先会在模型中登记,当模型数据发生改变时,马上通知已向此模型登记的每个视图。视图从模型取得最新的数据并刷新自己。 

Widgets在互联网上是一种时髦的技术，而在这里，Widgets是一个插件式可扩展的UI组件框架，是窗口化的应用框架，并引入了容器的概念，实际上，容器是一类特殊的widget。

Model 提供了驱动widget内容的实际数据，管理Listener的注册行为，而并不关注数据的可视化表达。有点像STL，不同的model支持不同的数据类型例如链表，向量组等，也支持特定控件或接口的数据类型。当数据发生变化时，Model会向关注该数据的发起者发送通知。IModel是一个基础接口，其他的Model接口都派生于IModel接口，有IValueModel，IArrayModel，IVectorModel，IInterfaceModel，IMenuModel和ITextModel等。

Widget是窗口上的小的控件，如文本框，图片框等，来完成一个Model中的数据表达，而不需要知道显示的位置，只需要知道内容和显示的区域。Widget 监听Model中数据的变化，按照开发者的需要来显示数据，可以不做显示处理，或者部分显示，甚至做到后台改变。Widget同时管理焦点和按键事件，IWidget是一个基础接口，其他的Widget接口都派生于IWidget接口，有ICheckWidget，I
RadioWidget，IButtonWidget，IProgressWidget，ITextWidget，ISliderWidget和IWaitCursorWidget 等。

Container来管理一组widgets的位置和布局，控制Widgets在屏幕上显示的次序，通常一个widget
自身就可以成为一个Container。根据布局的原则，Container像堆栈一样管理Widget。Widget在其中由底向上存放，所以Widget被叠加的时候，在堆栈底层的Widget可能被隐蔽，屏幕中只显示堆栈顶层的Widget。也就是说，Container先画最底层的Widget，然后逐层向上实现屏幕显示。IContainer是一个基础接口，其他的Widget接口都派生于IWidget接口，有IPropContainert，IConstraintContainer，IXYContainer，ICardContainer和IDecorator等。一个decorator是一个特殊的container用来管理和修饰指定的widget，通IDecorator可以实现BorderWidget，BlendWidget，ImageWidget，TabWidget，ScrollbarWidget，ScrollIndicatorWidget，ViewportWidget，TitleWidget，ListWidget，GridWidget和PickWidget等等。 

Form是针对页面显示的，有两类：RootForm和一般的Form。RootForm 预先实现了一般通用的屏幕布局，负责管理Form堆栈，为Forms定义了可视化元素，包括标题、背景和软健包括标题，软键按钮和背景，设定标题的区域，软键按钮的显示区域，一组form的客户区域，区别弹出对话框和手机的全屏显示，管理未处理的按键事件并分发给当前的form，而且，RootForm中的元素是可以被替换或删除。

Form同样包含标题，软键按钮和背景等显示元素，当显示元素的内容发生变化是发出相应的通知，获取当前Widget没有处理的事件。Form负责应用的逻辑，构建和放置Widgets，设置Model中的数据，处理应用的执行流程。IForm是一个基础接口，其他的Form接口都派生于IWidget接口，主要有ListForm，StaticForm和PopupForm。](https://so.csdn.net/so/search/s.do?q=forms&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=decorator&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




