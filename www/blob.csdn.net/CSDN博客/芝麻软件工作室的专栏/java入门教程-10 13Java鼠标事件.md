
# java入门教程-10.13Java鼠标事件 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:52:54[seven-soft](https://me.csdn.net/softn)阅读数：366


鼠标事件的事件源往往与容器相关，当鼠标进入容器、离开容器，或者在容器中单击鼠标、拖动鼠标时都会发生鼠标事件。java语言为处理鼠标事件提供两个接口：MouseListener，MouseMotionListener接口。
## MouseListener接口
MouseListener接口能处理5种鼠标事件：按下鼠标，释放鼠标，点击鼠标、鼠标进入、鼠标退出。相应的方法有：
(1) getX()：鼠标的X坐标
(2) getY()：鼠标的Y坐标
(3) getModifiers()：获取鼠标的左键或右键。
(4) getClickCount()：鼠标被点击的次数。
(5) getSource()：获取发生鼠标的事件源。
(6) addMouseListener(监视器)：加放监视器。
(7) removeMouseListener(监视器)：移去监视器。
要实现的MouseListener接口的方法有：
(1) mousePressed(MouseEvent e);
(2) mouseReleased(MouseEvent e);
(3) mouseEntered(MouseEvent e);
(4) mouseExited(MouseEvent e);
(5) mouseClicked(MouseEvent e);
【例 11-18】小应用程序设置了一个文本区，用于记录一系列鼠标事件。当鼠标进入小应用程序窗口时，文本区显示“鼠标进来”；当鼠标离开 窗口时，文本区显示“鼠标走开”；当鼠标被按下时，文本区显示“鼠标按下”，当鼠标被双击时，文本区显示“鼠标双击”；并显示鼠标的坐标。程序还显示一个红色的圆，当点击鼠标时，圆的半径会不断地变大（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-18.txt)）。
任何组件上都可以发生鼠标事件：鼠标进入、鼠标退出、按下鼠标等。例如，在上述程序中添加一个按钮，并给按钮对象添加鼠标监视器，将上述程序中的init()方法修改成如下形式，即能示意按钮上的所有鼠标事件。
JButton button;
public void init(){
button = new JButton(“按钮也能发生鼠标事件”);
r = 10;
text = new JTextArea(15,20);
add(button);
add(text);
button.addMouseListener(this);
}
如果程序希望进一步知道按下或点击的是鼠标左键或右键，鼠标的左键或右键可用InputEvent类中的常量BUTTON1_MASK和BUTTON3_MASK来判定。例如，以下表达式判断是否按下或点击了鼠标右键：
e.getModifiers()==InputEvent. BUTTON3_MASK
## MouseMotionListener接口
MouseMotionListener接口处理拖动鼠标和鼠标移动两种事件。
注册监视器的方法是：
addMouseMotionListener(监视器)
要实现的的接口方法有两个：
(1) mouseDragged(MouseEvent e)
(2) mouseMoved(MouseEvent e)
【例 11-19】一个滚动条与显示窗口同步变化的应用程序。窗口有一个方块，用鼠标拖运方块，或用鼠标点击窗口，方块改变显示位置，相应水平和垂直滚动条的滑块也会改变它们在滚动条中的位置。反之，移动滚动条的滑块，方块在窗口中的显示位置也会改变（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-19.txt)）。
上述例子中，如果只要求通过滑动滑块，改变内容的显示位置，可以简单地使用滚动面板JScrollPane。如果是这样，关于滚动条的创建和控制都可以免去，直接由JScrollPane内部实现。参见以下修改后的MyWindow的定义：
class MyWindow extends JFrame{
public MyWindow(String s){
super(s);
Container con = this.getContentPane();
con.setLayout(new BorderLayout());
this.setLocaltion(100,100);
MyListener listener = new MyListener();
listener.setPreferredSize(new Dimension(700,700));
JScrollPane scrolledCanvas = new JScrollPane(listener);
this.add(scrolledCanvas,BorderLayout.CENTER);
this.setVisible(true);
this.pack();
}
public Dimension getPreferredSize(){
return new Dimension(400,400);
}
}
鼠标指针形状也能由程序控制 ，setCursor()方法能设置鼠标指针形状。例如，代码setCursor(Cursor.getPredefinedCursor(cursor.WAIT_CURSOR))。

