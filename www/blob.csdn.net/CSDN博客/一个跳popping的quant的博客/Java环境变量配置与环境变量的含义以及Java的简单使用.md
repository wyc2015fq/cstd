# Java环境变量配置与环境变量的含义以及Java的简单使用 - 一个跳popping的quant的博客 - CSDN博客





2018年01月26日 20:35:03[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1237








这学期开了java programming这门课程，老师每段时间都会布置一些java的实验，之前就有学了一段时间的java了，然后很久没有写过java代码了，正好趁这个机会再熟悉一下。从大一到现在，从来没有怎么写过实验报告，一直都是copy别人的，然后修改个名字就交上去了，现在java的实验报告我要好好写了~

这个是老师布置的实验要求。




1.下载安装JDK并配置环境变量，百度上有很多教程。

2. 用记事本编写welcome程序并编译运行，相关内容在课本的P9~12。         

3.练习JOptionPane类的showMessageDialog方法，相关内容在课本P13
。     

4.写一个算法近似求pai  在PPT上会有算法参考。

一、JDK的安装以及环境变量

JDK现在更新到了9，每次更新都带来了一些新的特性。

因为电脑上安装的是JDK8，就以JDK8的安装说明一下。

1、先说一下环境变量的意义（百度百科）：是在操作系统中一个具有特定名字的对象，它包含了一个或者多个应用程序所将使用到的信息。以path这个环境变量为例，当要求系统运行一个程序而没有告诉它程序的完整路径时，系统除了在当前的目录下面寻找此程序外，还应该到path中指定的路径中去找。通过设置环境变量，来更好的运行进程。

所以运行java程序时，一些java中的指令就是通过Path的路径中去找的。而一般我们运行java程序的时候都是基于窗口的，这容易使我们对这个概念混淆。其实，窗口实际上是对命令行（小黑窗口）的封装。当我们用命令行的形式来运行java程序的时候，就容易理解了，例如javac这个指令在哪？除了在当前目录中找，还会去环境变量中去找。

2、安装JDK网上有很多JDK的下载链接，直接下载安装即可。链接：http://www.oracle.com/technetwork/java/javase/downloads/index.html


3、安装成功之后就开始配置环境变量了。路径：电脑属性->系统高级设置->环境变量。要配置的环境变量有三个，分别是：

JAVA_HOME、PATH和CLASSPATH。


4、JAVA_HOME：

就是指java的安装目录。就是刚才在安装jdk时的安装目录。

![](https://img-blog.csdn.net/20180126191619670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面在配置PATH的时候，用到的%JAVA_HOME%就是引用了这个这个变量，内容就是变量的值也就是jdk目录。


5、PATH：

配置PATH环境变量%JAVA_HOME%\bin，当然也可以是全路径，像图中那样。如果是追加的话还要在前面加上一个;符号来分隔。

![](https://img-blog.csdn.net/20180126192052781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


PATH环境变量的作用就是指定java命令要搜索的路径。因为我们jdk安装的一些可执行文件都放在了bin目录下，如javac、java、javaw等。这样就可以在任何目录下执行javac/java等工具了。如果不进行设置，那你就只能先进入到这个目录下，才能使用那些命令了

6、CLASSPATH：

指明java的执行环境。就是在哪些目录下可以找到要执行的java程序所需要的类或者包。就是当我们写下import  **package时，编译器要找这个package，默认是在当前的目录下找，如果配置了CLASSPATH，它就知道去CLASSPATH中去找了。

其中，dt.jar是关于运行环境的类库，tools.jar是工具类库,编译和运行需要。

![](https://img-blog.csdn.net/20180126192935082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


7、配置完之后，一路确定。可以win+R打开控制台窗口运行cmd，输入java -version来检验下是否安装成功。

![](https://img-blog.csdn.net/20180126193457878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


二、 用记事本编写welcome程序并编译运行

1、在D:/路径或者其他路径下新建一个文本文档，打开输入



```java
class Welcome{
 public static void main(String args[]){
    System.out.println("Welcome!");
 }
}
```


然后保存命名为Welcome.java。

2、然后同样cmd，输入以下的命令。

![](https://img-blog.csdn.net/20180126194812377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后我们再来到d盘目录下，会发现多了一个Welcome.class的文件，也就是生成的字节码文件。

在输入以下命令：

![](https://img-blog.csdn.net/20180126195024390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这样就在控制台上输出了Welcome!









后面的两个代码就比较简单了，只贴上代码为了交实验报告好copy~



```java
public class CalPi {

	public static void main(String[] args) {
		double pi = 0;
		for(double i = 1; i <=10000; i++){
			pi = i % 2 == 0? pi-1/(2*i-1):pi+1/(2*i-1);
		}
		System.out.println(4*pi);
	}
}
```
![](https://img-blog.csdn.net/20180126202101179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




```java
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class TestJOptionPanel {

	public static void main(String[] args) {
		final JFrame frame = new JFrame();
		JButton btn = new JButton("show message");
		frame.add(btn);
		btn.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(frame, "Hi!");				
			}
		});
		frame.setVisible(true);
		frame.setSize(200,200);
	}
	
}
```

![](https://img-blog.csdn.net/20180126203303557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







部分内容借鉴：https://www.cnblogs.com/huangbingzhi/p/4277160.html


























