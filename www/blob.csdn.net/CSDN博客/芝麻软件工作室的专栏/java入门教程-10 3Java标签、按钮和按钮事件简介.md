
# java入门教程-10.3Java标签、按钮和按钮事件简介 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:40:39[seven-soft](https://me.csdn.net/softn)阅读数：573


标签和按钮也许是图形界面中最常见的两种组件，按钮又总是与激发动作事件有关。
## 标签
标签(JLabel)是最简单的Swing组件。标签对象的作用是对位于其后的界面组件作说明。可以设置标签的属性，即前景色，背景色、字体等，但不能动态地编辑标签中的文本。
程序关于标签的基本内容有以下几个方面：声明一个标签名；
创建一个标签对象；
将标签对象加入到某个容器。
JLabel类的主要构造方法是：JLabel ()：构造一个无显示文字的标签；
JLabel (String s)：构造一个显示文字为s的标签；
JLabel(String s, int align)：构造一个显示文字为s的标签。align为显示文字的水平方式，对齐方式有三种：左对齐：JLabel.LEFT
中心对齐：JLabel.CENTER
右对齐：JLabel.RIGHT

JLabel类的其他常用方法是：setText(String s)：设置标签显示文字；
getText()：获取标签显示文字；
setBackground(Color c)：设置标签的背景颜色，默认背景颜色是容器的背景颜色；
setForeground(Color c)：设置标签上的文字的颜色，默认颜色是黑色。
## 按钮
按钮(JButton)在界面设计中用于激发动作事件。按钮可显示文本，当按钮被激活时，能激发动作事件。
JButton常用构造方法有：JButton()：创建一个没有标题的按钮对象；
JButton(String s)：创建一个标题为s的按钮对象。
JButton类的其他常用方法有：setLabel(String s)：设置按钮的标题文字。
getLabel()：获取按钮的标题文字。
setMnemonic(char mnemonic)：设置热键
setToolTipText(String s)：设置提示文字。
setEnabled(boolean b)：设置是否响应事件
setRolloverEnabled(boolean b)：设置是否可滚动。
addActionListener(ActionListener aL)：向按钮添加动作监视器。
removeActionListener(ActionListener aL)：移动按钮的监视器。
按钮处理动作事件的基本内容有以下几个方面：与按钮动作事件相关的接口是ActionListener，给出实现该接口的类的定义；
声明一个按钮名；
创建一个按钮对象；
将按钮对象加入到某个容器；
为需要控制的按钮对象注册监视器，对在这个按钮上产生的事件实施监听。如果是按钮对象所在的类实现监视接口，注册监视器的代码形式是
addActionListener(this);
参见【例 11-3】，如果是别的类A的对象a作为监视器，类A必须实现ActionListener接口，完成监视器注册需用以下形式的两行代码：
A a = new A();  //创建类A的实例a
addActionListener(a);  //用对象a作为监视器对事件进行监视。
在实现接口ActionListener的类中，给出处理事件的方法的定义：
public void actionPerformed(ActionEvent e);
在处理事件的方法中，用获取事件源信息的方法获得事件源信息，并判断和完成相应处理。获得事件源的方法有：方法
 getSource()获得事件源对象；方法getActionCommand()获得事件源按钮的文字信息。
【例 11-3】处理按钮事件实例，应用程序定义了一个窗口，窗口内设置两个按钮，当点击Red按钮时，窗口的背景色置成红色；点击Green按钮时，窗口的背景色置成绿色（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/11-3.txt)）。
用鼠标点击按钮产生事件对象，将事件送达对象，这个过程称为激发事件。当一个事件被送到监视器对象时，监视器对象实现的接口方法被调用，调用时系统会提供事件对象的参数。程序中虽然没有调用监视器方法的的代码，但是程序做了两件事：第一，指定哪一个对象是监视器，它将响应由按钮的激发的事件，这个步骤称为监视器注册。第二，必须定义一个方法，当事件送到监视器时，这个方法将被调用。程序中没有调用这个方法的代码，这个调用是系统执行的。
在上面的程序中，代码
redBut.addActionListener(this);
注册this作为redBut按钮的监视器，随后的代码也注册this作为greenBut按钮的监视器。在上述的程序中，this就是当前的ButtonDemo对象myButtonGUI。这样，ButtonDemo类就是监视器对象的类，对象MyButtonGUI作为两个按钮的监视器。在类ButtonDemo中有监视器方法的实现。当一个按钮被点击时，系统以事件的激发者为参数，自动调用方法actionPerformed
 ()。
组件不同，激发的事件种类也不同，监视器类的种类也不同。按钮激发的事件称为action事件，相应的监视器称为action监视器。一个action监视器对象的类型为ActionListener，类要实现ActionListener接口。程序体现这些内容需要做到两点：在类定义的首行接上代码implements ActionListener；
类内定义方法actionPerformed ()。
前面程序中的类ButtonDemo正确地做到了这两点。
每个界面元素当激发事件时，都有一个字符串与这个事件相对应，这个字符串称为action命令。用代码 e.getActionCommand()就能获取action事件参数e的命令字符串，据此，方法actionPerformed()就能知道是哪一个按钮激发的事件。在默认情况下，按钮的命令字符串就是按钮上的文字。如有必要可以用方法
 setActionCommand()为界面组件设置命令字符串。

