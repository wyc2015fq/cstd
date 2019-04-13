
# java入门教程-11.2Java绘图之设置字型和颜色 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:55:59[seven-soft](https://me.csdn.net/softn)阅读数：418


Java绘图中，显示文字的方法主要有三种：
（1）drawString(String str,int x,int y)：在指定的位置显示字符串。
（2）drawChars(char data[],int offset,int length, int x, int y)：在指定的位置显示字符数组中的文字，从字符数组的offset位置开始，最多显示length个字符。
（3）drawBytes(byte data[],int offset,int length,int x,int y), 在指定的位置显示字符数组中的文字，从字符数组的offset位置开始，最多显示length个字符。
这里给出的显示位置(x,y)为文字的基线的开始坐标，不是文字显示的矩形区域的左上角坐标。
文字字型有三个要素：字体：常用的字体有Times New Roman、Symbol、宋体、楷体等。
风格：常用的风格有三种：正常、粗体和斜体；分别用三个常量表示：Font.PLAIN(正常)、Font.BOLD(粗体)和Font.ITALIC（斜体）。风格可以组合使用，例如 ，Font.BOLD+Font.ITALIC。
字号：字号是字的大小，单位是磅。
在Java语言中，用类Font对象字型。Font类构造方法有：
Font(String fontName,int style,int size),3个参数分别表示字体、风格和字号。例如，代码：
Font fnA = new Font(“细明本”,Font.PLAIN,12)
设置的字型的是：细明体、正常风格， 12磅字号。
Font类的其他常用方法：getStyle()，返回字体风格。
getSize()，返回字体大小。
getName()，返回字体名称。
isPlain()，测试字体是否是正常字体。
isBold(),测试字体是否是粗体。
isItalic()，测试字体是否是斜体。
【例 12-1】小应用程序用6种字型字符串，显示内容说明本身的字型。
import java.applet.*;
import java.awt.*;
public class Example7_1 extends Applet{
Font f1 = new Font("Helvetica",Font.PLAIN,18);
Font f2 = new Font("Helvetica", Font.BOLD,10);
Font f3 = new Font("Helvetica",Font.ITALIC,12);
Font f4 = new Font("Courier",Font.PLAIN,12);
Font f5 = new Font("TimesRoman", Font.BOLD+Font.ITALIC,14);
Font f6 = new Font("Dialog",Font.ITALIC,14);
public void paint(Graphics g){
setSize(250,200);
g.setFont(f1);drawString("18pt plain Helvetica",5,20);
g.setFont(f2);drawString("10pt bold Helvetica",5,43);
g.setFont(f3);drawString("12pt italic Helvetica",5,58);
g.setFont(f4);drawString("12pt plain courier",5,75);
g.setFont(f5);drawString("14pt bold & italic times Roman",5,92);
g.setFont(f6);drawString("14pt italic dialog",5,111);
}
}
用类Color的对象设置颜色，有两种方法生成各种颜色：用类Color预定议的颜色：black，red, white, yellow ……；
通过红绿蓝（RGB）的值合成颜色。
与颜色有关的常用方法：
（1）用类Color的构造方法Color（int R, int G,int B）创建一个颜色对象，参数R,G,B分别表示红色、绿色和蓝色，它们的取值是从0到255。
（2）用类Graphics的方法setColor(Color c)，参数c的取值参见表12-1。
（3）用类Component的方法setBackground(Color c)设置背景颜色。因为小程序是组件类的子类，直接可用setBackground()方法改变背景色。
（4）用类Graphics的方法getColor()获取颜色。
表12-1 Color 类预定义颜色常量|black|blue|cyan|
|darkGray|gray|green|
|lightGray|magenta|orange|
|pink|red|white|
|yellow|
【例 12-2】小应用程序设置颜色并涂方块，其中绘制方块的方法将在后续小节中讲到。
import java.applet.*;
import java.awt.*;
public class Example7_2 extends Applet{
public void paint(Graphics g){
setSize(380,200);
for(int i=0;i<=10;i++){
Color myredcolor = new Color(i*25+5,0,0);
g.setColor(myredcolor);
g.fillRect(i*32+5,2,28,28);
}
for(int i=0;i<=10;i++){
Color mygreencolor = new Color(0,i*25+5,0);
g.setColor(mygreencolor);
g.fillRect(i*32+5,32,28,28);
}
for(int i=0;i<=10;i++){
Color mybluecolor = new Color(0,0,i*25+5);
g.setColor(mybluecolor);
g.fillRect(i*32+5,62,28,28);
}
}
}

