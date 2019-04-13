
# java入门教程-10.10Java菜单（菜单条、菜单和菜单项） -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:49:46[seven-soft](https://me.csdn.net/softn)阅读数：403


有两种类型的菜单：下拉式菜单和弹出式菜单。本章只讨论下拉式菜单编程方法。菜单与JComboBox和JCheckBox不同，它们在界面中是一直可见的。菜单与JComboBox的相同之处是每次只可选择一个项目。
在下拉式菜单或弹出式菜单中选择一个选项就产生一个ActionEvent事件。该事件被发送给那个选项的监视器，事件的意义由监视器解释。
## 菜单条、菜单和菜单项
下拉式菜单通过出现在菜单条上的名字可视化表示，菜单条(JMenuBar)通常出现在JFrame的顶部，一个菜单条显示多个下拉式菜单的名字。可以用两种方式来激活下拉式菜单。一种是按下鼠标的按钮，并保持按下状态，移动鼠标，直至释放鼠标完成选择，高亮度显示的菜单项即为所选择的。另一种方式是当光标位于菜单条中的菜单名上时，点击鼠标，在这种情况下，菜单会展开，且高亮度显示菜单项。
一个菜单条可以放多个菜单(JMenu)，每个菜单又可以有许多菜单项(JMenuItem)。例如，Eclipse环境的菜单条有File、Edit、Source、Refactor等菜单，每个菜单又有许多菜单项。例如，File菜单有New、Open
 File、Close 、Close All等菜单项。
向窗口增设菜单的方法是：先创建一个菜单条对象，然后再创建若干菜单对象，把这些菜单对象放在菜单条里，再按要求为每个菜单对象添加菜单项。
菜单中的菜单项也可以是一个完整的菜单。由于菜单项又可以是另一个完整菜单，因此可以构造一个层次状菜单结构。
1.菜单条
类JMenuBar的实例就是菜单条。例如，以下代码创建菜单条对象menubar：
JMenuBar menubar = new JMenuBar();
在窗口中增设菜单条，必须使用JFrame类中的setJMenuBar()方法。例如，代码：
setJMenuBar(menubar);
类JMenuBar的常用方法有：add(JMenu m)：将菜单m加入到菜单条中。
countJMenus()：获得菜单条中菜单条数。
getJMenu(int p)：取得菜单条中的菜单。
remove(JMenu m)：删除菜单条中的菜单m。
2. 菜单
由类JMenu创建的对象就是菜单。类JMenu的常用方法如下：JMenu()：建立一个空标题的菜单。
JMenu(String s)：建立一个标题为s的菜单。
add(JMenuItem item)：向菜单增加由参数item指定的菜单选项。
add(JMenu menu)：向菜单增加由参数menu指定的菜单。实现在菜单嵌入子菜单。
addSeparator()：在菜单选项之间画一条分隔线。
getItem(int n)：得到指定索引处的菜单项。
getItemCount()：得到菜单项数目。
insert(JMenuItem item,int n)：在菜单的位置n插入菜单项item.
remove(int n)：删除菜单位置n的菜单项
removeAll()：删除菜单的所有菜单项。
3.菜单项
类JMenuItem 的实例就是菜单项。类JMenuItem的常用方法如下：JMenuItem()：构造无标题的菜单项。
JMenuItem(String s)：构造有标题的菜单项。
setEnabled(boolean b)：设置当前单项是否可被选择。
isEnabled()：返回当前菜单项是否可被用户选择。
getLabel()：得到菜单项的名称。
setLabel()：设置菜单选项的名称。
addActionListener(ActionListener e)：为菜单项设置监视器。监视器接受点击某个菜单的动作事件。
4. 处理菜单事件
菜单的事件源是用鼠标点击某个菜单项。处理该事件的接口是ActionListener,要实现的接口方法是actionPerformed(ActionEvent e),获得事件源的方法getSource().
【例 11-15】小应用程序示意窗口有菜单条的实现方法。设有一个按钮，当按钮处于打开窗口状态时，单击按钮将打开一个窗口，窗口设有一个菜单条，有两个菜单，每个菜单又各有三个菜单项。当一个菜单项被选中时，菜单项监视方法在文本框中显示相应菜单项被选中字样（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-15.txt)）。
5. 嵌入子菜单
创建了一个菜单，并创建多个菜单项，其中某个菜单项又是一个(含其他菜单项的)菜单，这就构成菜单嵌套。例如，将上述程序中的有关代码改成如下：
Menu menu1,menu2,item4;
MenuItem item3,item5,item6,item41,item42;
另插入以下代码创建item41和item42菜单项，并把它们加入到item4菜单中：
item41= new MenuItem(“东方红”);
item42 = new MenuItem(“牡丹”);
item4.add(item41);
item4.add(item42);
则点击item4菜单时，又会打开两个菜单项供选择。
6. 向菜单增加退出项
增设一个新的菜单项，对该菜单项加入监视，对应的监视方法中使用System.exit()方法，就能实现单击该菜单项时退出Java运行环境。例如，以下示意代码：
…
item7 = new MenuItem(“退出”);
item7.addActionListener(this);
…
public void actionPerformed(ActionEvent e){
if(e.getSource()==item7){
System.exit(0);
}
}
7.设置菜单项的快捷键
用MenuShortcut类为菜单项设置快捷键。构造方法是MenuShortcut(int key)。其中key可以取值KeyEvent.VK_A至KenEvent.VK_Z,也可以取 ‘a’到 ‘z’键码值。菜单项使用setShortcut(MenuShortcut
 k)方法来设置快捷键。例如，以下代码设置字母e为快捷键。
class Herwindow extends Frame implements ActionListener{
MenuBar menbar;
Menu menu;
MenuItem item;
MenuShortcut shortcut = new MenuShortcut(KeyEvent.VK_E);
…
item.setShortcut(shortcut);
…
}
## 选择框菜单项
菜单也可以包含具有持久的选择状态的选项，这种特殊的菜单可由JCheckBoxMenuItem类来定义。JCheckBoxMenuItem对象像选择框一样，能表示一个选项被选中与否，也可以作为一个菜单项加到下拉菜单中。点击JCheckBoxMenuItem菜单时，就会在它的左边出现打勾符号或清除打勾符号。例如，在例6.5程序的类MenuWindow中，将代码
addItem(menu1,“跑步”,this);addItem(menu1,”跳绳”,this);
改写成以下代码，就将两个普通菜单项“跑步“和“跳绳”改成两个选择框菜单项：
JCheckBoxMenuItem item1 = new JCheckBoxMenuItem(“跑步”)；
JCheckBoxMenuItem item2 = new JCheckBoxMenuItem(“跳绳”);
item1.setActionCommand(“跑步”);
item1.addActionListener(this);
menu1.add(item1);
item2.setActionCommand(“跳绳”);
item2.addActionListener(this);
menu1.add(item2);

