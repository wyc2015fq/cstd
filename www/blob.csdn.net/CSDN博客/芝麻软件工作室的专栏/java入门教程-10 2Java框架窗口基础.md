
# java入门教程-10.2Java框架窗口基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:36:54[seven-soft](https://me.csdn.net/softn)阅读数：572


窗口是GUI编程的基础，小应用程序或图形界面的应用程序的可视组件都放在窗口中，在GUI中，窗口是用户屏幕的一部分，起着在屏幕中一个小屏幕的作用。有以下三种窗口：Applet窗口：Applet类管理这个窗口，当应用程序程序启动时，由系统创建和处理；
框架窗口(JFrame)：这是通常意义上的窗口，它支持窗口周边的框架、标题栏，以及最小化、最大化和关闭按钮；
一种无边框窗口(JWindow)：没有标题栏，没有框架，只是一个空的矩形。
用Swing中的JFrame类或它的子类创建的对象就是JFrame窗口。
JFrame类的主要构造方法：JFrame()：创建无标题的窗口对象；
JFrame(String s)：创建一个标题名是字符串s的窗口对象。
JFrame类的其他常用方法：setBounds(int x,int y,int width,int height)：参数x,y指定窗口出现在屏幕的位置；参数width,height指定窗口的宽度和高度。单位是像素。
setSize(int width,int height)：设置窗口的大小，参数width和height指定窗口的宽度和高度，单位是像素。
setBackground(Color c)：以参数 c设置窗口的背景颜色。
setVisible(boolean b)：参数b设置窗口是可见或不可见。JFrame默认是不可见的。
pack()：用紧凑方式显示窗口。如果不使用该方法，窗口初始出现时可能看不到窗口中的组件，当用户调整窗口的大小时，可能才能看到这些组件。
setTitle(String name)：以参数name设置窗口的名字。
getTitle()：获取窗口的名字。
setResiable(boolean m)：设置当前窗口是否可调整大小(默认可调整大小)。
Swing里的容器都可以添加组件，除了JPanel及其子类(JApplet)之外，其他的Swing容器不允许把组件直接加入。其他容器添加组件有两种方法：一种是用getContentPane()方法获得内容面板，再将组件加入。例如，例5.1程序中的代码：
mw.getContentPane().add(button);
该代码的意义是获得容器的内容面板，并将按钮button添加到这个内容面板中。
另一种是建立一个JPanel对象的中间容器，把组件添加到这个容器中，再用setContentPane()把这个容器置为内容面板。例如，代码：
JPanel contentPane = new JPanel();
…
mw.setContentPane(contentPane);
以上代码把contentPane置成内容面板。
【例 11-1】一个用JFrame类创建窗口的Java应用程序。窗口只有一个按钮。
import javax.swing.*;
public class Example5_1{
public static void main(String args[]){
JFrame mw = new JFrame(“我的第一个窗口”);
mw.setSize(250,200);
JButton button = new JButton(“我是一个按钮”);
mw.getContentPane().add(button);
mw.setVisible(true);
}
}
用Swing 编写GUI程序时，通常不直接用JFrame创建窗口对象，而用JFrame派生的子类创建窗口对象，在子类中可以加入窗口的特定要求和特别的内容等。
【例 11-2】定义JFrame派生的子类MyWindowDemo创建JFrame窗口。类MyWindowDemo的构造方法有五个参数：窗口的标题名，加放窗口的组件，窗口的背景颜色以及窗口的高度和宽度。在主方法中，利用类MyWindowDemo创建两个类似的窗口。
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
public class Example5_2{
public static MyWindowDemo mw1;
public static MyWindowDemo mw2;
public static void main(String args[]){
JButton static butt1 = new JButton(“我是一个按钮”);
String name1 = “我的第一个窗口”;
String name2 = “我的第二个窗口”;
mw1 = new MyWindowDemo(name1,butt1,Color.blue,350,450);
mw1.setVisible(true);
JButton butt2 = new JButton(“我是另一个按钮”);
mw2 = new MyWindowDemo(name2,butt2,Color.magenta,300,400);
mw2.setVisible(true);
}
}
class MyWindowDemo extends JFrame{
public MyWindowDemo(String name,JButton button,Color c,int w,int h){
super();
setTitle(name);
setSize(w,h);
Container con = getContentPane();
con.add(button);
con.setBackground(c);
}
}
显示颜色由java.awt包的Color类管理，在Color类中预定了一些常用的颜色，参见表11-3。JFrame类的部分常用方法参见表11-4。
表11-3 Color 类中定义的常用颜色|字段摘要|颜色|
|static Color black/BLACK|黑色|
|static Color blue/BLUE|蓝色|
|static Color cyan/CYAN|青色|
|static Color darkGray/DARK_GRAY|深灰色|
|static Color gray/GRAY|灰色|
|static Color green/GREEN|绿色|
|static Color lightGray/LIGHTGRAY|浅灰色|
|static Color magenta/MAGENTA|洋红色|
|static Color orange/ORANGE|桔黄色|
|static Color pink/PINK|粉红色|
|static Color red/RED|红色|
|static Color white/WHITE|白色|
|static Color yellow/YELLOW|黄色|
表 11-4 JFrame 类的部分常用方法|方法|意义|
|JFrame()|构造方法，创建一个 JFrame 对象|
|JFrame(String title)|创建一个以 title 为标题的 JFrame 对象|
|add()|从父类继承的方法，向窗口添加窗口元素|
|void addWindowListener(WindowListener ear)|注册监视器，监听由 JFrame 对象击发的事件|
|Container getContentPane()|返回 JFrame 对象的内容面板|
|void setBackground(Color c)|设置背景色为 c|
|void setForeground(Color c)|设置前景色为 c|
|void setSize(int w,int h)|设置窗口的宽为 w,高为 h|
|vid setTitle(String title)|设置窗口中的标题|
|void setVisible(boolean b)|设置窗口的可见性，true 可见，false 不可见
|

