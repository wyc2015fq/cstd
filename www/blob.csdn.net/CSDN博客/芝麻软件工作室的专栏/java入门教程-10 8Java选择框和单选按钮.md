
# java入门教程-10.8Java选择框和单选按钮 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:47:21[seven-soft](https://me.csdn.net/softn)阅读数：422


选择框、单选框和单选按钮都是选择组件，选择组件有两种状态，一种是选中(on)，另一种是未选中(off)，它们提供一种简单的
 “on/off”选择功能，让用户在一组选择项目中作选择。
## 选择框
选择框(JCheckBox)的选中与否开状是一个小方框，被选中则在框中打勾。当在一个容器中有多个选择框，同时可以有多个选择框被选中，这样的选择框也称复选框。与选择框相关的接口是ItemListener，事件类是ItemEvent。
JCheckBox类常用的构造方法有以下3个：JCheckBox()：用空标题构造选择框。
JCheckBox(String s)：用给定的标题s构造选择框。
JCheckBox(String s, boolean b)：用给定的标题s构造选择框，参数b设置选中与否的初始状态。
JCheckBox类的其他常用方法如下：getState()：获取选择框的状态。
setState(boolean b)：设置选择框的状态
getLabel()：获取选择框的标题。
setLabel(String s)：设置选择框的标题。
isSelected()：获取选择框是否被选中的状态。
itemStateChanged(ItemEvent e)：处理选择框事件的接口方法。
getItemSelectable()：获取可选项，获取事件源。
addItemListener(ItemListener l)：为选择框设定监视器。
removeItemListener(ItemListener l)：移去选择框的监视器。
【例 11-11】声明一个面板子类，面板子类对象有3个选择框。
class Panel1 extends JPanel{
JCheckBox box1,box2,box3;
Panel1(){
box1 = new JCheckBox(“足球”);
box2 = new JCheckBox(“排球”);
box2 = new JCheckBox(“篮球”);
}
}
## 单选框
当在一个容器中放入多个选择框，且没有ButtonGroup对象将它们分组，则可以同时选中多个选择框。如果使用ButtonGroup对象将选择框分组，同一时刻组内的多个选择框只允许有一个被选中，称同一组内的选择框为单选框。单选框分组的方法是先创建ButtonGroup对象，然后将希望为同组的选择框添加到同一个ButtonGroup对象中。参见例6.2程序的面板子类Panel2的声明，组内有3个单选框。
## 单选按钮
单选按钮(JRadioButton)的功能与单选框相似。使用单选按钮的方法是将一些单选按钮用ButtonGroup对象分组，使同一组的单选按钮只允许有一个被选中。单选按钮与单选框的差异是显示的样式不同，单选按钮是一个圆形的按钮，单选框是一个小方框。
JRadioButton类的常用构造方法有以下几个：JRadioButton()：用空标题构造单选按钮。
JRadioButton(String s)：用给定的标题s构造单选按钮。
JRadioButton(String s,boolean b)：用给定的标题s构造单选按钮,参数b设置选中与否的初始状态。
单选按钮使用时需要使用ButtonGroup将单选按钮分组，单选按钮的分组方法是先创建对象，然后将同组的单选按钮添加到同一个ButtonGroup对象中。参见例6.2程序的子类panel1的声明，组内有3个单选按钮。
## 选择项目事件处理
用户对选择框或单选按钮做出选择后，程序应对这个选择作出必要的响应，程序为此要处理选择项目事件。选择项目处理程序的基本内容有：监视选择项目对象的类要实现接口ItemListener,
程序要声明和建立选择对象，
为选择对象注册监视器，
编写处理选择项目事件的接口方法itemStateChanged(ItemEvent e),在该方法内用getItemSelectable()方法获取事件源，并作相应处理。
【例 11-12】处理选择项目事件的小应用程序。一个由3个单选按钮组成的产品选择组，当选中某个产品时，文本区将显示该产品的信息。一个由3个选择框组成的购买产品数量选择框组，当选择了购买数量后，在另一个文本框显示每台价格（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-12.txt)）。

