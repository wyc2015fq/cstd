
# java入门教程-10.1Java图形界面设计基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:49:26[seven-soft](https://me.csdn.net/softn)阅读数：4381


早先程序使用最简单的输入输出方式，用户在键盘输入数据，程序将信息输出在屏幕上。现代程序要求使用图形用户界面(Graphical User Interface,GUI)，界面中有菜单、按钮等，用户通过鼠标选择菜单中的选项和点击按钮，命令程序功能模块。本章学习如何用Java语言编写GUI科学试验，如何通过GUI实现输入和输出。
## AWT和Swing
先前用Java编写GUI程序，是使用抽象窗口工具包AWT(Abstract Window Toolkit).现在多用Swing。Swing可以看作是AWT的改良版，而不是代替AWT，是对AWT的提高和扩展。所以，在写GUI程序时，Swing和AWT都要作用。它们共存于Java基础类(Java
 Foundation Class,JFC)中。
尽管AWT和Swing都提供了构造图形界面元素的类，但它们的重要方面有所不同：AWT依赖于主平台绘制用户界面组件；而Swing有自己的机制，在主平台提供的窗口中绘制和管理界面组件。Swing与AWT之间的最明显的区别是界面组件的外观，AWT在不同平台上运行相同的程序，界面的外观和风格可能会有一些差异。然而，一个基于Swing的应用程序可能在任何平台上都会有相同的外观和风格。
Swing中的类是从AWT继承的，有些Swing类直接扩展AWT中对应的类。例如，JApplet、JDialog、JFrame和JWindow。
使用Swing设计图形界面，主要引入两个包：javax.swing包：包含Swing的基本类；
java.awt.event包：包含与处理事件相关的接口和类。
由于Swing太丰富，不可能在一本教科书中给出Swing的全面介绍，但本章所介绍的有关Swing的知识，已足以让读者编写相当精美的GUI程序。
## 组件和容器
组件(component)是图形界面的基本元素，用户可以直接操作，例如按钮。容器(Container)是图形界面的的复合元素，容器可以包含组件，例如面板。
Java语言为每种组件都预定义类，程序通过它们或它们的子类各种组件对象，如，Swing中预定义的按钮类JButton是一种类，程序创建的JButton对象，或JButton子类的对象就是按钮。Java语言也为每种容器预定义类，程序通过它们或它们的子类创建各种容器对象。例如，Swing中预定义的窗口类JFrame是一种容器类，程序创建的JFrame或JFrame子类的对象就是窗口。
为了统一管理组件和容器，为所有组件类定义超类，把组件的共有操作都定义在Component类中。同样，为所有容器类定义超类Container类，把容器的共有操作都定义在Container类中。例如，Container类中定义了add()方法，大多数容器都可以用add()方法向容器添加组件。
Component、Container和Graphics类是AWT库中的关键类。为能层次地构造复杂的图形界面，容器被当作特殊的组件，可以把容器放入另一个容器中。例如，把若干按钮和文本框分放在两个面板中，再把这两个面板和另一些按钮放入窗口中。这种层次地构造界面的方法，能以增量的方式构造复杂的用户界面。
## 事件驱动程序设计基础
1.事件、监视器和监视器注册
图形界面上的事件是指在某个组件上发生用户操作。例如，用户单击了界面上的某个按钮，就说在这个按钮上发生了事件，这个按钮对象就是事件的击发者。对事件作监视的对象称为监视器，监视器提供响应事件的处理方法。为了让监视器与事件对象关联起来，需要对事件对象作监视器注册，告诉系统事件对象的监视器。
以程序响应按钮事件为例，程序要创建按钮对象，把它添加到界面中，要为按钮作监视器注册，程序要有响应按钮事件的方法。当“单击按钮”事件发生时，系统就调用已为这个按钮注册的事件处理方法，完成处理按钮事件的工作。
2.实现事件处理的途征
java语言编写事件处理程序主要有两种方案：一个是程序重设handleEvent(Eventevt),采用这个方案的程序工作量稍大一些；另一个方案是程序实现一些系统设定的接口。java按事件类型提供多种接口，作为监视器对象的类需要实现相应的接口，即实现响应事件的方法。当事件发生时，系统内设的handleEvent(Event
 evt)方法就自动调用监视器的类实现的响应事件的方法。
java.awt.event包中用来检测并对事件做出反应的模型包括以下三个组成元素：源对象：事件“发生”这个组件上，它与一组“侦听”该事件的对象保持着联系。
监视器对象：一个实现预定义的接口的类的一个对象，该对象的类要提供对发生的事件作处理的方法。
事件对象：它包含描述当事件发生时从源传递给监视器的特定事件的信息。
一个事件驱动程序要做的工作除创建源对象和监视器对象之外，还必须安排监视器了解源对象，或向源对象注册监视器。每个源对象有一个已注册的监视器列表，提供一个方法能向该列表添加监视器对象。只有在源对象注册了监视器之后，系统才会将源对象上发生的事件通知监视器对象。
3.事件类型和监视器接口
在java语言中，为了便于系统管理事件，也为了便于程序作监视器注册，系统将事件分类，称为事件类型。系统为每个事件类型提供一个接口。要作为监视器对象的类必须实现相应的接口，提供接口规定的响应事件的方法。
再以程序响应按钮事件为例，JButton类对象button可以是一个事件的激发者。当用户点击界面中与button对应的按钮时，button对象就会产生一个ActionEvent类型的事件。如果监视器对象是obj，对象obj的类是Obj,则类Obj必须实现AWT中的ActionListener接口，实现监视按钮事件的actionPerformed方法。button对象必须用addActionListener方法注册它的监视器obj。
程序运行时，当用户点击button对象对应的按钮时，系统就将一个ActionEvent对象从事件激发对象传递到监视器。ActionEvent对象包含的信息包括事件发生在哪一个按钮，以及有关该事件的其他信息。
表 11-1 给出有一定代表性的事件类型和产生这些事件的部分Swing组件。实际事件发生时，通常会产生一系列的事件，例如，用户点击按钮，会产生ChangeEvent事件提示光标到了按钮上，接着又是一个ChangeEvent事件表示鼠标被按下，然后是ActionEvent事件表示鼠标已松开，但光标依旧在按钮上，最后是ChangeEvent事件，表示光标已离开按钮。但是应用程序通常只处理按下按钮的完整动作的单个ActionEvent事件。
表 11-1  组件和事件类型|事件类型|组件|描述|
|ActionEvent|JButton,JCheckBox
|JComboBox,JMenuItem
|JRadioButton|点击、选项或选择|
|ChangeEvent|JSlider|调整一个可移动元素的位置|
|AdjustmentEvent|JScrollBar|调整滑块位置|
|ItemEvent|JComboBox,JCheckBox
|JRadioButton
|JRadioButtonMenuItem
|JCheckBoxMenuItem|从一组可选方案中选择一个项
|目|
|ListSelectionEvent|JList|选项事件|
|KeyEvent
|MouseEvent|JComponent 及其派生类|操纵鼠标或键盘|
|CareEvent|JTextArea,JTextField|选择和编辑文本|
|WindowEvent|Window 及其派生类 JFrame|对窗口打开、关闭和图标化|
每个事件类型都有一个相应的监视器接口，表11-2列出了每个接口的方法。实现监视器接口的类必须实现所有定义在接口中的方法。
表 11-2 监视器接口方法|监视器接口|方法|获取事件源的方法|
|ActionListener|actionPerformed|getSource,getActionCommand|
|ChangeListener|stateChanged|getSource|
|AdjustmentListener|adjustmentValueChanged|getAdjustable|
|FocusListener|focusGained,
|focusLost|
|ItemListener|itemStateChanged|getItemSelectable(),getSource()|
|ListSelectionListener|valueChanged|e.getSource().getSelectedValue()|
|KeyListener|keyPressed,
|keyReleased,
|keyTyped|
|CareListener|careUpdate|
|MouseListener|mouseClicked,
|mouseEntered,
|mouseExited,
|mousePressed,
|mouseReleased|
|MouseMontionListener|mouseDragged,
|mouseMoved|
|WindowListener|windowClosed,
|windowClosing,
|windowDeactivated,
|windowDeiconified,
|windowIconified,
|windowOpened
|

