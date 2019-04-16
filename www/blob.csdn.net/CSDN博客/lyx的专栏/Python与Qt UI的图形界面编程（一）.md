# Python与Qt UI的图形界面编程（一） - lyx的专栏 - CSDN博客





2016年02月03日 22:34:54[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：8851









本实例实现一个"Hello Kitty!"例子，简单介绍Qt编程的基本流程，以及Qt程序的编绎运行方式,实例效果图如图所示。

![](http://www.linuxidc.com/upload/2012_06/120624071596002.png)


这是一个简单的例子，整个对话框只有一个按钮，单击该按钮，对话框关闭，退出程序。


实现代码如下：
- **from** PyQt4.QtGui **import** *  
- **from** PyQt4.QtCore **import** *  
- **import** sys  
- 
- app=QApplication(sys.argv)  
- b=QPushButton("Hello Kitty!")  
- b.show()  
- app.connect(b,SIGNAL("clicked()"),app,SLOT("quit()"))  
- app.exec_()  




第1行导入PyQt4.QtGui的所有类及模块，包括QApplication，所有Qt图形化应用程序都必须包含此文件，它包含了Qt图形化应用程序的各种资源，基本设置，控制流以及事件处理等。


第5行新创建了一个QApplication对象，每个Qt应用程序都必须有且只有一个QApplication对象，采用sys.argv作为参数，便于程序处理命令行参数。


第6行创建了一个QPushButton对象，并设置它的显示文本为“Hello Kitty！”，由于此处并没有指定按钮的父窗体，因此以自己作为主窗口。


第7行调用show()方法，显示此按钮。控件被创建时，默认是不显示的，必须调用show()函数来显示它。


第8行的connect方法是Qt最重要的特征，即信号与槽的机制。当按钮被按下则触发clicked信号，与之相连的QApplication对象的槽quit()响应按钮单击信号，执行退出应用程序的操作。关于信号与槽机制在本实例最后将进行详细的分析。


最后调用QApplication的exec_()方法，程序进入消息循环，等待可能输入进行响应。Qt完成事件处理及显示的工作，并在应用程序退出时返回exec_()的值。


最后执行程序即可出现上图所示的对话框，一个简单的Hello Kitty！例子完成。


信号与槽机制作为Qt最重要的特性，提供了任意两个Qt对象之间的通信机制。其中，信号会在某个特定情况或动作下被触发，槽是用于接收并处理信号的函数。例如，要将一个窗口中的变化情况通知给另一个窗口，则一个窗口发送信号，另一个窗口的槽接收此信号并进行相应的操作，即可实现两个窗口之间的通信。这比传统的图形化程序采用回调函数的方式实现对象间通信要简单灵活得多。每个Qt对象都包含预定的信号和槽，当一某一特定事件发生时，一个信号被发射，与信号相关联的槽则会响应信号完成相应的处理。


信号与槽机制常用的连接方式为：


connect(Object1,SIGNAL(signal),Object2,SLOT(slot))


signal为对象Object1的信号，slot为对象Object2的槽，Qt的窗口部件都包含若干个预定义的信号和若干个预定义的槽。当一个类被继承时，该类的信号和槽也同时被继承。开始人员也可以根据需要定义自己的信号和槽。


信号与槽机制可以有多种连接方式，下图描述了信号与槽的多种可能连接方式。

![](http://www.linuxidc.com/upload/2012_06/120624071596001.jpg)


1.一个信号可以与另一个信号相连


connect(Object1,SIGNAL(signal1),Object2,SIGNAL(signal1))


即表示Object1的信号1发射可以触发Object2的信号1发射。


2.表示一个信号可以与多个槽相连


connect(Object1,SIGNAL(signal2),Object2,SLOT(slot2))


connect(Object1,SIGNAL(signal2),Object3,SLOT(slot1))


3.表示同一个槽可以响应多个信号


connect(Object1,SIGNAL(signal2),Object2,SLOT(slot2))


connect(Object3,SIGNAL(signal2),Object2,SLOT(slot2)) 



