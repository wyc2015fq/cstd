# Eclipse下搭建SWT与Swing图形界面开发环境 - 深之JohnChen的专栏 - CSDN博客

2017年07月29日 14:54:42[byxdaz](https://me.csdn.net/byxdaz)阅读数：3871


一、SWT与Swing介绍

        SWT(StandardWidget Toolkit)则是由Eclipse项目组织开发的一套完整的图形界面开发包，虽然当初仅仅是IBM为了编写Eclipse的IDE环境才编写的一套底层图形界面 API，但是随着Eclipse的大范围使用，人们也开始对这套GUI开发包做了深入的研究，并且不少人都开始转向SWT的使用。也许有人会问，既然有了 Swing, 那为什么还要SWT?答案很简单，因为人们的理念不同。SWT的原理是根据运行者的操作系统不同调用JNI来完成图形界面的绘制的，如果你用的是 Windows，那么建立的窗口就是windows风格的，还可以用XP的皮肤，如果是Linux或者MacOS的话，则在代码不改变的情况下，绘制出相 应风格的界面。而Swing则是完全从底层开始自己“手绘”界面的，这也就导致了速度比较慢和工人的“风格诡异”的问题。当然，由于Sun的强大推动力和 AWT/Swing的普及，很多人仍然在使用Swing作为首选的GUI开发工具包。

Eclipse本身就自带了SWT/Swing的相关jar包，所以不用下载。如果不喜欢自己手写GUI代码的话就去下载一个可视化开发插件（WindowBuilder）通过拖动控件实现布局设计以及事件代码编码。

二、WindowBuilder插件安装

        Eclipse上安装插件很简单，主要有两种方式：第一种就是在线安装插件；第二种就是，采取离线方式，也就是先将插件压缩包下载下来。推荐使用第二种，因为到时候你可以自己随时删除这个插件。

1、找到对应版本的windowbuilder

打开这个链接：[http://www.eclipse.org/windowbuilder/download.php](http://www.eclipse.org/windowbuilder/download.php)

显示eclipse的版本号和对应的插件链接

Update Sites
|Eclipse Version|Release Version|Integration Version| | |
|----|----|----|----|----|
||Update Site|Zipped Update Site|Update Site|Zipped Update Site|
|4.7 (Oxygen)|||[link](http://download.eclipse.org/windowbuilder/WB/integration/4.7/)||
|4.6 (Neon)|[link](http://download.eclipse.org/windowbuilder/WB/release/4.6/)||[link](http://download.eclipse.org/windowbuilder/WB/integration/4.6/)||
|4.5 (Mars)|[link](http://download.eclipse.org/windowbuilder/WB/release/R201506241200-1/4.5/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201506241200-1/WB_v1.8.0_UpdateSite_for_Eclipse4.5.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201506241200-1/WB_v1.8.0_UpdateSite_for_Eclipse4.5.zip.MD5))|[link](http://download.eclipse.org/windowbuilder/WB/integration/4.5/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/integration/WB_v1.8.0_UpdateSite_for_Eclipse4.5.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/integration/WB_v1.8.0_UpdateSite_for_Eclipse4.5.zip.MD5))|
|4.4 (Luna)|[link](http://download.eclipse.org/windowbuilder/WB/release/R201506241200-1/4.4/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201506241200-1/WB_v1.8.0_UpdateSite_for_Eclipse4.4.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201506241200-1/WB_v1.8.0_UpdateSite_for_Eclipse4.4.zip.MD5))|[link](http://download.eclipse.org/windowbuilder/WB/integration/4.4/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/integration/WB_v1.8.0_UpdateSite_for_Eclipse4.4.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/integration/WB_v1.8.0_UpdateSite_for_Eclipse4.4.zip.MD5))|
|4.3 (Kepler)|[link](http://download.eclipse.org/windowbuilder/WB/release/R201406251200/4.3/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.3.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.3.zip.MD5))|||
|4.2 (Juno)|[link](http://download.eclipse.org/windowbuilder/WB/release/R201406251200/4.2/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.2.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.2.zip.MD5))|||
|3.8 (Juno)|[link](http://download.eclipse.org/windowbuilder/WB/release/R201406251200/4.2/)|[link](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.2.zip) ([MD5  Hash](http://eclipse.org/downloads/download.php?file=/windowbuilder/WB/release/R201406251200/WB_v1.7.0_UpdateSite_for_Eclipse4.2.zip.MD5))|||

根据你下载的Eclipse版本选择，应该下载那个。因为我使用的是Juno（4.2），下载了这个对应的WindowBuilder插件。

2、点击Eclipse中的Help，再次点击InstallNew Software，出现：

![](https://img-blog.csdn.net/20170729144548364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、点击“Add”按钮，弹出一个AddRepository对话框。
![](https://img-blog.csdn.net/20170729144559367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在Name那一栏填写插件的名称，如WindowBuilder。再点击Archive，选择插件所在目录。
![](https://img-blog.csdn.net/20170729144611155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4、点击Ok出现，AvailableSoftware
![](https://img-blog.csdn.net/20170729144623010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

select all勾选，同意协议。一步一步的简单点击就可以了，安装成功后会要求重启Eclipse。

三、如何使用WindowBuilder插件

新建一个工程，New-->JavaProject，工程名为GuiTest。

添加一个窗口，new-->other然后选择下图
![](https://img-blog.csdn.net/20170729144633187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择SwingDesigner，（如果确实要选择SWTDesigner也可以，只不过SWTDesigner兼容性没有SwingDesigner这么好，毕竟SwingDesigner是针对Java使用的，它里面的控件都是JXXXX形式的，比如JText，而SWT Designer确实XXXX形式的，比如Text），所以，最好选择SwingDesigner里面的组件。

 在WindowBuilder下可以直接建立工程，也可以建立单个的窗口。假设已经建立了工程，这里选择Swing Designer下的Application Window，点击Next >后，类似于新建类，在后续对话框中输入Name和Package。
![](https://img-blog.csdn.net/20170729144640816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Finish后，即可得到Test.java。打开Test.java，可见其中已经预先生成了一些代码，是一个空白的窗体。点击代码窗口左下角新出现“Design"标签，可以使用WindowBuilder Editor可视化地查看窗体，如图所示。
![](https://img-blog.csdn.net/20170729144649745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

WindowBuilder Eidtor的界面类似于VS等工具，能够可视化地对界面进行设计。
![](https://img-blog.csdn.net/20170729150438606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

添加事件，在Components中选择一个控件，右键点击，弹出窗口，可以选择add Eventhandler事件。
![](https://img-blog.csdn.net/20170729144713313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实例代码：

```java
package com.zychen.form;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JTextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Test {

	private JFrame frame;
	private JTextField textField;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Test window = new Test();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public Test() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		JLabel lblNewLabel = new JLabel("Name");
		lblNewLabel.setBounds(10, 33, 54, 15);
		frame.getContentPane().add(lblNewLabel);
		
		textField = new JTextField();
		textField.setBounds(74, 30, 66, 21);
		frame.getContentPane().add(textField);
		textField.setColumns(10);
		
		JButton btnNewButton = new JButton("submit");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				textField.setText("hello");
			}
		});
		btnNewButton.setBounds(35, 73, 93, 23);
		frame.getContentPane().add(btnNewButton);
	}
}
```

运行界面
![](https://img-blog.csdn.net/20170729144723767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

