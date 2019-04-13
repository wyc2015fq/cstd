
# java入门教程-10.6Java文本框和文本区 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:44:09[seven-soft](https://me.csdn.net/softn)阅读数：537


在图形界面中，文本框和文本区是用于信息输入输出的组件。
## 文本框
文本框(JTextField)是界面中用于输入和输出一行文本的框。JTextField类用来建立文本框。与文本框相关的接口是ActionListener。
文本框处理程序的基本内容有以下几个方面：声明一个文本框名。
建立一个文本框对象。
将文本框对象加入到某个容器。
对需要控制的文本框对象注册监视器，监听文本框的输入结束(即输入回车键)事件。
一个处理文本框事件的方法，完成对截获事件进行判断和处理。
JTextField类的主要构造方法：JTextField()，文本框的字符长度为1。
JTextField(int columns)，文本框初始值为空字符串，文本框的字符长度设为columns.
JTextField(String text),文本框初始值为text的字符串。
JTextField(String text,int columns);文本框初始值为text，文本框的字符长度为columns。
JTextField类的其他方法：setFont(Font f),设置字体
setText(String text)，在文本框中设置文本
getText()，获取文本框中的文本。
setEditable(boolean)，指定文本框的可编辑性，默认为true，可编辑。
setHorizontalAlignment(int alignment)设置文本对齐方式。对齐方式有：JTextField.LEFT, JTextField.CENTER, JTextField.RIGHT。
requestFocus()，设置焦点。
addActionListener(ActionListener )，为文本框设置动作监视器，指定ActionListener对象接收该文本框上发生的输入结束动作事件。
removeActionListener(ActionListener)移去文本框监视器。
getColumns(),返回文本框的列数。
getMinimumSize(),返回文本框所需的最小尺寸。
getMinimumSize(int), 返回文本框在指定的字符数情况下，所需的最小尺寸。
getPreferredSize(),返回文本框希望具有的尺寸。
getPreferredSize(int),返回文本框在指定字符数情况下，希望具有的尺寸。
【例 11-8】小应用程序有两个文本框。一个文本用于输入一个整数，另一个文本框显示这个整数的平方值。程序用字符串转基本类型的方法Long.parseLong(text1.getText())，读取文本框text1中的字符串，并将它转换成整数。程序用Sqr类的实例作为监视器，但为了让监视器能访问主类的变量，主类中的变量被声明为类变量，并且不设置访问权限（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-8.txt)）。
密码框(JPasswordField)是一个单行的输入组件，与JTextField基本类似。密码框多一个屏蔽功能，就是在输入时，都会以一个别的指定的字符(一般是*字符)输出。除了前面介绍的文本框的方法外，另有一些密码框常用的方法：getEchoChar(),返回密码的回显字符。
setEchoChar(char)，设置密码框的回显字符。
## 文本区
文本区(JTextArea)是窗体中一个放置文本的区域。文本区与文本框的主要区别是文本区可存放多行文本。javax.swing包中的JTextArea类用来建立文本区。JTextArea组件没有事件。
文本区处理程序的基本内容有以下几个方面：声明一个文本区名。
建立一个文本区对象。
将文本区对象加入到某个容器。
JTextArea类的主要构造方法：JTextArea()，以默认的列数和行数，创建一个文本区对象。
JTextArea(String s),以s为初始值，创建一个文本区对象。
JTextArea(Strings ,int x,int y)，以s为初始值，行数为x,列数为y,创建一个文本区对象。
JTextArea(int x,int y)以行数为x,以列数为y，创建一个文本区对象。
JTextArea类的其他常用方法：setText(String s),设置显示文本，同时清除原有文本。
getText()，获取文本区的文本。
insert(String s,int x),在指定的位置插入指定的文本。
replace(String s,int x,int y),用给定的一替换从x位置开始到y位置结束的文本。
append(String s)，在文本区追加文本。
getCarePosition(),获取文本区中活动光标的位置。
setCarePosition(int n)，设置活动光标的位置。
setLineWrap(boolean b)，设置自动换行，缺省情况，不自动换行。
以下代码创建一个文本区，并设置能自动换行。
JTextArea textA = new JTextArea(“我是一个文本区”,10,15);
textA.setLineWrap(true);//设置自动换行
当文本区中的内容较多，不能在文本区全部显示时，可给文本区配上滚动条。给文本区设置滚动条可用以下代码：
JTextArea ta = new JTextArea();
JScrollPane jsp = new JScrollPane(ta);//给文本区添加滚动条

