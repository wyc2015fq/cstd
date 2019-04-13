
# java入门教程-10.7Java文本框和文本区的输入输出 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:46:00[seven-soft](https://me.csdn.net/softn)阅读数：1517


在GUI中，常用文本框和文本区实现数据的输入和输出。如果采用文本区输入，通常另设一个数据输入完成按钮。当数据输入结束时，点击这个按钮。事件处理程序利用getText()方法从文本区中读取字符串信息。对于采用文本框作为输入的情况，最后输入的回车符可以激发输入完成事件，通常不用另设按钮。事件处理程序可以利用单词分析器分析出一个个数，再利用字符串转换数值方法，获得输入的数值。对于输出，程序先将数值转换成字符串，然后通过setText()方法将数据输出到文本框或文本区。
【例 11-9】小应用程序设置一个文本区、一个文本框和两个按钮。用户在文本区中输入整数序列，单击求和按钮，程序对文本区中的整数序列进行求和，并在文本框中输出和。单击第二个按钮，清除文本区和文本框中的内容。
import java.util.*;import java.applet.*;import java.awt.*;
import javax.swing.*;import java.awt.event.*;
public class J509 extends Applet implements ActionListener{
JTextArea textA;JTextField textF;JButton b1,b2;
public void init(){
setSize(250,150);
textA=new JTextArea("",5,10);
textA.setBackground(Color.cyan);
textF=new JTextField("",10);
textF.setBackground(Color.pink);
b1=new JButton("求 和"); b2=new JButton("重新开始");
textF.setEditable(false);
b1.addActionListener(this); b2.addActionListener(this);
add(textA); add(textF); add(b1);add(b2);
}
public void actionPerformed(ActionEvent e){
if(e.getSource()==b1){
String s=textA.getText();
StringTokenizer tokens=new StringTokenizer(s);
//使用默认的分隔符集合：空格、换行、Tab符合回车作分隔符
int n=tokens.countTokens(),sum=0,i;
for(i=0;i<=n-1;i++){
String temp=tokens.nextToken();//从文本区取下一个数据
sum+=Integer.parseInt(temp);
}
textF.setText(""+sum);
}
else if(e.getSource()==b2){
textA.setText(null);
textF.setText(null);
}
}
}
【例 11-10】小应用程序计算从起始整数到终止整数中是因子倍数的所有数。小程序容器用GridLayout布局将界面划分为3行列，第一行是标签，第二行和第三行是两个Panel。设计两个Panel容器类Panel1,Panel2,并分别用GridLayout布局划分。Panel1为1行6列，Panel2为1行4列。然后将标签和容器类Panel1,Panel2产生的组件加入到窗口的相应位置中。
import java.applet.*;import javax.swing.*;
import java.awt.*;import java.awt.event.*;
class Panel1 extends JPanel{
JTextField text1,text2,text3;
Panel1(){//构造方法。当创建Panel对象时，Panel被初始化为有三个标签
//三个文本框，布局为GridLayout(1,6)
text1=new JTextField(10);text2=new JTextField(10);
text3=new JTextField(10);setLayout(new GridLayout(1,6));
add(new JLabel("起始数",JLabel.RIGHT));add(text1);
add(new JLabel("终止数",JLabel.RIGHT));add(text2);
add(new JLabel("因子",JLabel.RIGHT));add(text3);
}
}
class Panel2 extends JPanel{//扩展Panel类
JTextArea text;JButton Button;
Panel2(){//构造方法。当创建Panel对象时，Panel被初始化为有一个标签
//一个文本框，布局为GridLayout(1,4)
text=new JTextArea(4,10);text.setLineWrap(true);
JScrollPane jsp=new JScrollPane(text);
Button=new JButton("开始计算");
setLayout(new GridLayout(1,4));
add(new JLabel("计算结果：",JLabel.RIGHT));
add(jsp);
add(new Label());add(Button);
}
}
public class J510 extends Applet implements ActionListener{
Panel1 panel1;Panel2 panel2;
public void init(){
setLayout(new GridLayout(3,1));
setSize(400,200);panel1=new Panel1();panel2=new Panel2();
add(new JLabel("计算从起始数到终止数是因子倍数的数",JLabel.CENTER));
add(panel1);add(panel2);
(panel2.Button).addActionListener(this);
}
public void actionPerformed(ActionEvent e){
if(e.getSource()==(panel2.Button)){
long n1,n2,f,count=0;
n1=Long.parseLong(panel1.text1.getText());
n2=Long.parseLong(panel1.text2.getText());
f=Long.parseLong(panel1.text3.getText());
for(long i=n1;i<=n2;i++){
if(i%f==0)
panel2.text.append(String.valueOf(i)+"");
}
}
}
}

