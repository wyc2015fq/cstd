
# java入门教程-10.4Java面板基础：JPanel和JScrollPane -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:41:56[seven-soft](https://me.csdn.net/softn)阅读数：707


面板有两种，一种是普通面板(JPanel)，另一种是滚动面板(JScrollPane)。
## JPanel
面板是一种通用容器，JPanel的作用是实现界面的层次结构，在它上面放入一些组件，也可以在上面绘画，将放有组件和有画的JPanel再放入另一个容器里。JPanel的默认布局为FlowLayout。
面板处理程序的基本内容有以下几个方面：通过继承声明JPanel类的子类，子类中有一些组件，并在构造方法中将组件加入面板。
声明JPanel子类对象。
创建JPanel子类对象。
将JPanel子类对象加入到某个容器。
JPanel类的常用构造方法有：JPanel(),创建一个JPanel对象。
JPanel(LayoutManager layout),创建JPanel对象时指定布局layout。
JPanel对象添加组件的方法：add(组件)，添加组件。
add(字符串，组件)，当面板采用GardLayout布局时，字符串是引用添加组件的代号。
【例 11-4】小应用程序有两个JPanel子类对象和一个按钮。每个JPanel子类对象又有两个按钮和一个标签。
import java.applet.*;
import javax.swing.*;
class MyPanel extends JPanel{
JButton button1,button2;
JLabel Label;
MyPanel(String s1,String s2,String s3){
//Panel对象被初始化为有两个按钮和一个文本框
button1=new JButton(s1);button2=new JButton(s2);
Label=new JLabel(s3);
add(button1);add(button2);add(Label);
}
}
public class J504 extends Applet{
MyPanel panel1,panel2;
JButton Button;
public void init(){
panel1=new MyPanel("确定","取消","标签，我们在面板1中");
panel2=new MyPanel("确定","取消","标签，我们在面板2中");
Button=new JButton("我是不在面板中的按钮");
add(panel1);add(panel2);add(Button);
setSize(300,200);
}
}
## JScrollPane
当一个容器内放置了许多组件，而容器的显示区域不足以同时显示所有组件时，如果让容器带滚动条，通过移动滚动条的滑块，容器中位置上的组件就能看到。滚动面板JScrollPane能实现这样的要求，JScrollPane是带有滚动条的面板。JScrollPane是Container类的子类，也是一种容器，但是只能添加一个组件。JScrollPane的一般用法是先将一些组件添加到一个JPanel中，然后再把这个JPanel添加到JScrollPane中。这样，从界面上看，在滚动面板上，好像也有多个组件。在Swing中，像JTextArea、JList、JTable等组件都没有自带滚动条，都需要将它们放置于滚动面板，利用滚动面板的滚动条，浏览组件中的内容。
JScrollPane类的构造方法有：JScrollPane()，先创建JScrollPane对象，然后再用方法setViewportView(Component com)为滚动面板对象放置组件对象。
JScrollPane(Component com),创建JScrollPane对象，参数com是要放置于JScrollPane对象的组件对象。为JScrollPane对象指定了显示对象之后，再用add()方法将JScrollPane对象放置于窗口中。
JScrollPane对象设置滚动条的方法是：setHorizontalScrollBarPolicy(int policy),policy 取以下列3个值之一：
JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS
JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEED
JScrollPane.HORIZONTAL_SCROLLBAR_NEVER
setVerticalScrollBarPolicy(int policy), policy 取以下列3个值之一：
JScrollPane.VERTICAL_SCROLLBAR_ALWAYS
JScrollPane.VERTICAL_SCROLLBAR_AS_NEED
JScrollPane.VERTICAL_SCROLLBAR_NEVER
以下代码将文本区放置于滚动面板，滑动面板的滚动条能浏览文本区
JTextArea textA = new JTextArea(20,30);
JScrollPane jsp = new JScrollPane(TextA);
getContentPane().add(jsp);//将含文本区的滚动面板加入到当前窗口中。

