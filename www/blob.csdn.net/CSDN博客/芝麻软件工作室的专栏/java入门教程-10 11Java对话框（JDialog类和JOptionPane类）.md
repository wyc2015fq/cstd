
# java入门教程-10.11Java对话框（JDialog类和JOptionPane类） -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:50:56[seven-soft](https://me.csdn.net/softn)阅读数：1029


对话框是为了人机对话过程提供交互模式的工具。应用程序通过对话框，或给用户提供信息，或从用户获得信息。对话框是一个临时窗口，可以在其中放置用于得到用户输入的控件。在Swing中，有两个对话框类，它们是JDialog类和JOptionPane类。JDialog类提供构造并管理通用对话框；JOptionPane类给一些常见的对话框提供许多便于使用的选项，例如，简单的“yes-no”对话框等。
## JDialog类
JDialog类作作对话框的基类。对话框与一般窗口不同，对话框依赖其他窗口，当它所依赖的窗口消失或最小化时，对话框也将消失；窗口还原时，对话框又会自动恢复。
对话框分为强制和非强制两种。强制型对话框不能中断对话过程，直至对话框结束，才让程序响应对话框以外的事件。非强制型对话框可以中断对话过程，去响应对话框以外的事件。强制型也称有模式对话框，非强制对话框也称非模式对话框。
JDialog对象也是一种容器，因此也可以给JDialog对话框指派布局管理器，对话框的默认布局为BoarderLayout布局。但组件不能直接加到对话框中，对话框也包含一个内容面板，应当把组件加到JDialog对象的内容面板中。由于对话框依赖窗口，因此要建立对话框，必须先要创建一个窗口。
JDialog类常用的构造方法有3个：JDialog(),构造一个初始化不可见的非强制型对话框。
JDialog(JFramef,String s),构造一个初始化不可见的非强制型对话框，参数f设置对话框所依赖的窗口，参数s用于设置标题。通常先声明一个JDialog类的子类，然后创建这个子类的一个对象，就建立了一个对话框。
JDialog(JFrame f,String s,boolean b),构造一个标题为s，初始化不可见的对话框。参数f设置对话框所依赖的窗口，参数b决定对话框是否强制或非强制型。
JDialog类的其他常用方法有以下几个：getTitle(),获取对话框的标题。
setTitle(String s),设置对话框的标题。
setModal(boolean b)，设置对话框的模式。
setSize(),设置框的大小。
setVisible(boolean b),显示或隐藏对话框。
【例 11-16】小应用程序声明一个用户窗口类和对话框类，用户窗口有两个按钮和两个文本框，当点击某个按钮时，对应的对话框被激活。在对话框中输入相应信息，按对话框的确定按钮。确定按钮的监视方法，将对话框中输入的信息传送给用户窗口，并在用户窗口的相应文本框中显示选择信息（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-16.txt)）。
上述例子创建的是强制型对话框，改为非强制型对话框就允许用户在对话过程中暂停，与程序的其他部分进行交互。这样，在界面中可以看到部分对话的效果。
将上述例子改为非强制型对话框只要作少量的改动即可。首先是将对话框构造方法中的代码“super(F,s,true);”改为 “super(F,s,false);”。
第二个变化：原来是响应确定按钮事件时，才调用方法returnName()，将对话框得到的字符串返回给程序。现在当文本框输入选择字符串结束后，就应该立即调用该方法。为此，需要对文本框的输入事件作监视，为文本注册监视器：
public void actionPerformed(ActionEvent e){
if(e.getSource()==text){
MyWindow.returnName(text.getText());
}
else if(e.getSource()==done){
MyWindow.returnName(text.getText());
setVisible(false);
dispose();//清除资源
}
}
## JOptionPane类
经常遇到非常简单的对话情况，为了简化常见对话框的编程，JOptionPane类定义了四个简单对话框类型，参见表11-4。JOptionPane类提供一组静态方法，让用户选用某种类型的对话框。下面的代码是选用确认对话框：
int result = JOptionPane.showConfirmDialog(parent,“确实要退出吗”, “退出确认“，JOptionPane.YES_NO_CANCEL_OPTION);
其中方法名的中间部分文字“Confirm”是创建对话框的类型，文字Confirm指明是选用确认对话框。将文字Confirm改为另外三种类型的某一个，就成为相应类型的对话框。上述代码的四个参数的意义是：第一个参数指定这个对话框的父窗口；第二个参数是对话框显示的文字；第三个参数是对话框的标题；最后一个参数指明对话框有三个按钮，分别为“是(Y)”，“否(N)”，和“撤销”。方法的返回结果是用户响应了这个对话框后的结果，参见表
 11-5 给出的可能答案。
输入对话框以列表或文本框形式请求用户输入选择信息，用户可以从列表中选择选项或从文本框中输入信息。以下是一个从列表中选择运行项目的输入对话框的示意代码：
String result = (String)JOptionPane.showInputDialog(parent,
“请选择一项运动项目”, “这是运动项目选择对话框”,
JOptionPane.QUESTION_MESSAGE,null,
new Object[]{“踢足球“，“打篮球”，“跑步”，“跳绳”}，“跑步”);
第四个参数是信息类型，参见11-6，第五个参数在这里没有特别的作用，总是用null;第六个参数定义了一个供选择的字符串数组，第七个参数是选择的默认值。对话框还包括“确定”和“撤销”按钮。
表11-4 JOptionPane对话框类型|输入|通过文本框、列表或其他手段输入，另有“确定”和“撤销”按钮|
|确认|提出一个问题，待用户确认，另有“是(Y)”、“否(N)”和“撤销”按钮|
|信息|显示一条简单的信息，另有“确定”和“撤销”按钮|
|选项|显示一列供用户选择的选项|
表 11-5  由 JOptionPane 对话框返回的结果|YES_OPTION|用户按了“是(Y)”按钮|
|NO_OPTION|用户按了“否(N)”按钮|
|CANCEL_OPTION|用户按了“撤销”按钮|
|OK_OPTION|用户按了“确定”按钮|
|CLOSED_OPTION|用户没按任何按钮，关闭对话框窗口|
表 11-6 JOptionPane 对话框的信息类型选项|PLAIN_MESSAGE|不包括任何图标|
|WARNING_MESSAGE|包括一个警告图标|
|QUESTION_MESSAGE|包括一个问题图标|
|INFORMATIN_MESSAGE|包括一个信息图标|
|ERROR_MESSAGE|包括一个出错图标|
有时，程序只是简单地输出一些信息，并不要求用户有反馈。这样的对话框可用以下形式的代码创建：
JOptionPane.showMessageDialog(parent, “这是一个Java程序”,
“我是输出信息对话框”，JOptionPane.PLAIN_MESSAGE);
上述代码中前三参数的意义与前面所述相同，最后参数是指定信息类型为不包括任何图标，参见表11-6。

