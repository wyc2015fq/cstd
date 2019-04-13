
# java入门教程-10.14Java键盘事件 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:53:45[seven-soft](https://me.csdn.net/softn)阅读数：257个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



键盘事件的事件源一般丐组件相关，当一个组件处于激活状态时，按下、释放或敲击键盘上的某个键时就会发生键盘事件。键盘事件的接口是KeyListener，注册键盘事件监视器的方法是addKeyListener(监视器)。实现KeyListener接口有3个：keyPressed(KeyEvent e)：键盘上某个键被按下；
keyReleased(KeyEvent e)：键盘上某个键被按下，又释放；
keyTyped(KeyEvent e)：keyPressed和keyReleased两个方法的组合。
管理键盘事件的类是KeyEvent，该类提供方法：
public int getKeyCode(),获得按动的键码，键码表在KeyEvent类中定义，参见附录。
【例 11-20】小应用程序有一个按钮和一个文本区，按钮作为发生键盘事件的事件源，并对它实施监视。程序运行时，先点击按钮，让按钮激活。以后输入英文字母时，在正文区显示输入的字母。字母显示时，字母之间用空格符分隔，且满10个字母时，换行显示。
import java.applet.*
import java.awt.*;
import java.awt.event.*;
public class Example6_10 extends Applet implements KeyListener{
int count =0;
Button button = new Button();
TextArea text = new TextArea(5,20);
public void init(){
button.addKeyListener(this);
add(button);add(text);
}
public void keyPressed(KeyEvent e){
int t = e.getKeyCode();
if(t>=KeyEvent.VK_A&&t<=KeyEvent.VK_Z){
text.append((char)t+" ");
count++;
if(count%10==0)
text.append("\n");
}
}
public void keyTyped(KeyEvent e){}
public void keyReleased(KeyEvent e){}
}

