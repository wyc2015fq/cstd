# 使用exe4j制作java启动程序等候界面 - 零度的博客专栏 - CSDN博客
2016年05月09日 16:35:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：1903
代码下载 [http://download.csdn.net/detail/z69183787/7486757](http://download.csdn.net/detail/z69183787/7486757)
最近，项目中想做几个java本地应用的 开启等候界面（最好类似与photoshop那种，开机界面是一张图片，而且图片可以使不规则形状，即不是矩形等），抽出时间来网上查了一下，当中也走了很多弯路，最后发现 exe4j可以直接实现这个功能。
方案1：网上找到了一些示例代码，使用的是 jframe 和 jwindow，
目录结构如下：
![](https://img-my.csdn.net/uploads/201212/26/1356503294_1102.jpg)
代码如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- import javax.swing.*;  
- import java.awt.*;  
- import java.io.File;  
- import java.io.UnsupportedEncodingException;  
- import java.net.*;  
- 
- //Download by http://www.codefans.net
- //程序启动界面
- 
- publicclass JWindowDemo extends JWindow implements Runnable {  
-     Thread splashThread; // 进度条更新线程
-     JProgressBar progress; // 进度条
- 
- public JWindowDemo() {  
-         Container container = getContentPane(); // 得到容器
-         setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR)); // 设置光标
-         URL url = getClass().getResource("1.jpg"); // 图片的位置
-         URL url2 = getClass().getResource("测试.jpg"); // 图片的位置
-         String f = url2.getPath();  
- try {  
-             f = java.net.URLDecoder.decode(f,"utf-8");  
-         } catch (UnsupportedEncodingException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-         File file = new File(f);  
-         System.out.println(file.length());  
-         System.out.println(f);  
- if (url != null) {  
-             container.add(new JLabel(new ImageIcon(url)), BorderLayout.CENTER); // 增加图片
-         }  
-         progress = new JProgressBar(1, 100); // 实例化进度条
-         progress.setStringPainted(true); // 描绘文字
-         progress.setString("加载程序中,请稍候......"); // 设置显示文字
-         progress.setBackground(Color.white); // 设置背景色
-         container.add(progress, BorderLayout.SOUTH); // 增加进度条到容器上
- 
-         Dimension screen = getToolkit().getScreenSize(); // 得到屏幕尺寸
-         pack(); // 窗口适应组件尺寸
-         setLocation((screen.width - getSize().width) / 2,  
-                 (screen.height - getSize().height) / 2); // 设置窗口位置
-     }  
- 
- publicvoid start() {  
- this.toFront(); // 窗口前端显示
-         splashThread = new Thread(this); // 实例化线程
-         splashThread.start(); // 开始运行线程
-     }  
- 
- publicvoid run() {  
-         setVisible(true); // 显示窗口
- try {  
- for (int i = 0; i < 20; i++) {  
-                 Thread.sleep(100); // 线程休眠
-                 progress.setValue(progress.getValue() + 1); // 设置进度条值
-             }  
-         } catch (Exception ex) {  
-             ex.printStackTrace();  
-         }  
-         dispose(); // 释放窗口
-         showFrame(); // 运行主程序
-     }  
- 
- staticvoid showFrame() {  
-         JFrame frame = new JFrame("程序启动界面演示"); // 实例化JFrame对象
-         frame.setSize(300, 200); // 设置窗口尺寸
-         frame.setVisible(true); // 窗口可视
-         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 关闭窗口时退出程序
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         JWindowDemo splash = new JWindowDemo();  
-         splash.start(); // 运行启动界面
-     }  
- }  
```java
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.*;
//Download by http://www.codefans.net
//程序启动界面
public class JWindowDemo extends JWindow implements Runnable {
	Thread splashThread; // 进度条更新线程
	JProgressBar progress; // 进度条
	public JWindowDemo() {
		Container container = getContentPane(); // 得到容器
		setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR)); // 设置光标
		URL url = getClass().getResource("1.jpg"); // 图片的位置
		URL url2 = getClass().getResource("测试.jpg"); // 图片的位置
		String f = url2.getPath();
		try {
			f = java.net.URLDecoder.decode(f,"utf-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		File file = new File(f);
		System.out.println(file.length());
		System.out.println(f);
		if (url != null) {
			container.add(new JLabel(new ImageIcon(url)), BorderLayout.CENTER); // 增加图片
		}
		progress = new JProgressBar(1, 100); // 实例化进度条
		progress.setStringPainted(true); // 描绘文字
		progress.setString("加载程序中,请稍候......"); // 设置显示文字
		progress.setBackground(Color.white); // 设置背景色
		container.add(progress, BorderLayout.SOUTH); // 增加进度条到容器上
		Dimension screen = getToolkit().getScreenSize(); // 得到屏幕尺寸
		pack(); // 窗口适应组件尺寸
		setLocation((screen.width - getSize().width) / 2,
				(screen.height - getSize().height) / 2); // 设置窗口位置
	}
	public void start() {
		this.toFront(); // 窗口前端显示
		splashThread = new Thread(this); // 实例化线程
		splashThread.start(); // 开始运行线程
	}
	public void run() {
		setVisible(true); // 显示窗口
		try {
			for (int i = 0; i < 20; i++) {
				Thread.sleep(100); // 线程休眠
				progress.setValue(progress.getValue() + 1); // 设置进度条值
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		dispose(); // 释放窗口
		showFrame(); // 运行主程序
	}
	static void showFrame() {
		JFrame frame = new JFrame("程序启动界面演示"); // 实例化JFrame对象
		frame.setSize(300, 200); // 设置窗口尺寸
		frame.setVisible(true); // 窗口可视
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 关闭窗口时退出程序
	}
	public static void main(String[] args) {
		JWindowDemo splash = new JWindowDemo();
		splash.start(); // 运行启动界面
	}
}
```
运行效果如下：
![](https://img-my.csdn.net/uploads/201212/26/1356503030_7748.jpg)
之后，需求需要更改，想让该图片的效果像office启动界面一样，透明显示，查了很久发现jdk1.5不能实现，要使用jdk1.6中的一些特别类，对jwindow进行透明度设置。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- AWTUtilities.setWindowOpacity(this, 0.8f);  
```java
AWTUtilities.setWindowOpacity(this, 0.8f);
```
目录结构如下：
![](https://img-my.csdn.net/uploads/201212/26/1356503327_1782.jpg)
完整代码如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- import javax.swing.*;  
- 
- import com.sun.awt.AWTUtilities;  
- 
- import java.awt.*;  
- import java.io.File;  
- import java.net.*;  
- 
- //Download by http://www.codefans.net
- //程序启动界面
- 
- publicclass Sample extends JWindow implements Runnable {  
- 
- 
- 
- /**
-      * 
-      */
- privatestaticfinallong serialVersionUID = -983869148219123774L;  
-     Thread splashThread; // 进度条更新线程
-     JProgressBar progress; // 进度条
- 
- public Sample(String fileName) {  
- 
-         Container container = getContentPane(); // 得到容器
-         JPanel pane = new JPanel();  
-         pane.setBackground(Color.WHITE);  
-         pane.setOpaque(true);  
-         pane.setBorder(null);  
-         setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR)); // 设置光标
-         URL url = getClass().getResource(fileName); // 图片的位置
- if (url != null) {  
-             JLabel j = new JLabel(new ImageIcon(url));  
-             pane.add(j);  
-             container.add(pane, BorderLayout.CENTER); // 增加图片
-         }  
- //  container.add(new JButton("dsadsadsads"), BorderLayout.);
-         progress = new JProgressBar(1, 100); // 实例化进度条
-         progress.setStringPainted(true); // 描绘文字
-         progress.setString("加载程序中,请稍候......"); // 设置显示文字
-         progress.setBackground(Color.white); // 设置背景色
-         container.add(progress, BorderLayout.SOUTH); // 增加进度条到容器上
- 
-         Dimension screen = getToolkit().getScreenSize(); // 得到屏幕尺寸
-         pack(); // 窗口适应组件尺寸
-         setLocation((screen.width - getSize().width) / 2,  
-                 (screen.height - getSize().height) / 2); // 设置窗口位置
-         AWTUtilities.setWindowOpacity(this, 0.8f);  
- 
-     }  
- 
- publicvoid start() {  
- this.toFront(); // 窗口前端显示
-         splashThread = new Thread(this); // 实例化线程
-         splashThread.start(); // 开始运行线程
-     }  
- 
- publicvoid run() {  
-         setVisible(true); // 显示窗口
- try {  
- for (int i = 0; i < 20; i++) {  
-                 Thread.sleep(100); // 线程休眠
-                 progress.setValue(progress.getValue() + 1); // 设置进度条值
-             }  
-         } catch (Exception ex) {  
-             ex.printStackTrace();  
-         }  
-         dispose(); // 释放窗口
-         showFrame(); // 运行主程序
-     }  
- 
- staticvoid showFrame() {  
-         JFrame frame = new JFrame("程序启动界面演示"); // 实例化JFrame对象
-         frame.setSize(300, 200); // 设置窗口尺寸
-         frame.setVisible(true); // 窗口可视
-         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 关闭窗口时退出程序
-     }  
- 
- 
- }  
```java
import javax.swing.*;
import com.sun.awt.AWTUtilities;
import java.awt.*;
import java.io.File;
import java.net.*;
//Download by http://www.codefans.net
//程序启动界面
public class Sample extends JWindow implements Runnable {
	
	
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -983869148219123774L;
	Thread splashThread; // 进度条更新线程
	JProgressBar progress; // 进度条
	public Sample(String fileName) {
		
		Container container = getContentPane(); // 得到容器
		JPanel pane = new JPanel();
		pane.setBackground(Color.WHITE);
		pane.setOpaque(true);
		pane.setBorder(null);
		setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR)); // 设置光标
		URL url = getClass().getResource(fileName); // 图片的位置
		if (url != null) {
			JLabel j = new JLabel(new ImageIcon(url));
			pane.add(j);
			container.add(pane, BorderLayout.CENTER); // 增加图片
		}
	//	container.add(new JButton("dsadsadsads"), BorderLayout.);
		progress = new JProgressBar(1, 100); // 实例化进度条
		progress.setStringPainted(true); // 描绘文字
		progress.setString("加载程序中,请稍候......"); // 设置显示文字
		progress.setBackground(Color.white); // 设置背景色
		container.add(progress, BorderLayout.SOUTH); // 增加进度条到容器上
		Dimension screen = getToolkit().getScreenSize(); // 得到屏幕尺寸
		pack(); // 窗口适应组件尺寸
		setLocation((screen.width - getSize().width) / 2,
				(screen.height - getSize().height) / 2); // 设置窗口位置
		AWTUtilities.setWindowOpacity(this, 0.8f);
		
	}
	public void start() {
		this.toFront(); // 窗口前端显示
		splashThread = new Thread(this); // 实例化线程
		splashThread.start(); // 开始运行线程
	}
	public void run() {
		setVisible(true); // 显示窗口
		try {
			for (int i = 0; i < 20; i++) {
				Thread.sleep(100); // 线程休眠
				progress.setValue(progress.getValue() + 1); // 设置进度条值
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		dispose(); // 释放窗口
		showFrame(); // 运行主程序
	}
	static void showFrame() {
		JFrame frame = new JFrame("程序启动界面演示"); // 实例化JFrame对象
		frame.setSize(300, 200); // 设置窗口尺寸
		frame.setVisible(true); // 窗口可视
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 关闭窗口时退出程序
	}
	
}
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- publicclass Test {  
- publicstaticvoid main(String[] args){  
- //      Sample splash = new Sample("img/ssms.png");
- //      splash.start(); // 运行启动界面
- //      
- 
- //      
- //      Sample splash = new Sample("img/fqdb.png");
- //      splash.start(); // 运行启动界面
- //      
- //      
- //      
-         Sample splash = new Sample("img/epms.png");  
-         splash.start(); // 运行启动界面
-     }  
- }  
```java
public class Test {
	public static void main(String[] args){
//		Sample splash = new Sample("img/ssms.png");
//		splash.start(); // 运行启动界面
//		
		
//		
//		Sample splash = new Sample("img/fqdb.png");
//		splash.start(); // 运行启动界面
//		
//		
//		
		Sample splash = new Sample("img/epms.png");
		splash.start(); // 运行启动界面
	}
}
```
该程序由于需要打包成jar包，所以还需要1个MANIFEST.MF文件，放在meta-inf目录下，使用eclipse导出时可以设置，否则就是默认的文件。
**[plain]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- Manifest-Version: 1.0  
- Author: Eastsun  
- Created-By: 1.6.0-beta2 (Sun Microsystems Inc.)  
- SplashScreen-Image: SplashTest/ssms.png  
- Main-Class: SplashTest.Main  
```
Manifest-Version: 1.0
Author: Eastsun
Created-By: 1.6.0-beta2 (Sun Microsystems Inc.)
SplashScreen-Image: SplashTest/ssms.png
Main-Class: SplashTest.Main
```
效果如下，可通过 AWTUtilities.setWindowOpacity(this, 0.8f); 设置透明度，但也有一个问题，当设置window框架为透明时，其中的控件，诸如label，panel都会变成透明的。
而且你会发现，无论怎么设置，最外面总有以图片宽高为边界的边框，即使图片本身是不规则的。
![](https://img-my.csdn.net/uploads/201212/26/1356503446_7902.jpg)
第三种实现方式：使用java6 的 java.awt.SplashScreen 类实现该功能，而且可以解决透明度以及不规则图形现实的问题。但由于需要启动图片，运行只能依靠jar包运行或者命令行运行，无法在eclipse中直接运行。
目录结构：
![](https://img-my.csdn.net/uploads/201212/26/1356507423_7894.jpg)
代码如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- package SplashTest;  
- 
- 
- 
- import javax.swing.*;  
- import java.awt.*;  
- 
- publicclass Main{  
- publicstaticvoid main(String[] args){  
- new Thread(){                                                  //用于运行SplashScreen的线程
- publicvoid run(){  
- try{  
-                     SplashScreen splash =SplashScreen.getSplashScreen();  
-                     Graphics2D g =splash.createGraphics();  
- //                    g.setColor(Color.red);
- //                    g.drawString(splash.getBounds().toString(),10,30);  //SplashScreen在屏幕的位置,大小
- //                    g.drawString(splash.getSize().toString(),10,50);    //SplashScreen的大小
- //                    g.drawString(splash.getImageURL().toString(),10,70);//当前显示的图片
- //                    splash.update();                                    //刷新以上内容到屏幕
-                     Thread.sleep(1000);  
- 
- //下面的代码是在SplashScreen上显示一个3D效果的字符串
- //  g.setComposite(AlphaComposite.Clear);                                                //1     
- //  g.fillRect(0,0,(int)splash.getSize().getWidth(),(int)splash.getSize().getHeight());  //2
-                     splash.setImageURL(Main.class.getResource("/SplashTest/1.png"));      //更改显示的图片
- //                    
- //                    int x =30, y=100;
- //                    String author ="Made By Eastsun";
- //                    g.setFont(new Font("TimesRoman", Font.PLAIN, 30));
- //                    g.setPaintMode();                                                                    //3
- //                    for(int i=0;i<5;i++){
- //                        g.setColor(new Color(200, 200, 0));
- //                        g.drawString(author, x+i, y+i-1);
- //                        g.setColor(new Color(100, 100, 0));
- //                        g.drawString(author, x+i-1, y+i);
- //                    }
- //                    g.setColor(Color.yellow);
- //                    g.drawString(author, x+5, y+5);
- //                    splash.update();
-                     Thread.sleep(10000);                                //注意,这儿是sleep(10000),但实际上SplashScreen并没有显示10秒    
-                 }catch(Exception e){  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }.start();  
- 
- try{  
-             Thread.sleep(3000);                                         //这儿决定SplashScreen显示时间长短.                                                             
-         }catch(Exception e){}                                           //对于实际应用程序,这儿对应程序的初始化工作
-         JFrame frame =new JFrame("Java SE6 Splash Test");  
-         frame.setPreferredSize(new Dimension(320,240));  
-         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
-         frame.pack();  
-         frame.setVisible(true);  
-     }  
- }  
```java
package SplashTest;
import javax.swing.*;
import java.awt.*;
public class Main{
    public static void main(String[] args){
        new Thread(){                                                  //用于运行SplashScreen的线程
            public void run(){
                try{
                    SplashScreen splash =SplashScreen.getSplashScreen();
                    Graphics2D g =splash.createGraphics();
//                    g.setColor(Color.red);
//                    g.drawString(splash.getBounds().toString(),10,30);  //SplashScreen在屏幕的位置,大小
//                    g.drawString(splash.getSize().toString(),10,50);    //SplashScreen的大小
//                    g.drawString(splash.getImageURL().toString(),10,70);//当前显示的图片
//                    splash.update();                                    //刷新以上内容到屏幕
                    Thread.sleep(1000);
                    
                    //下面的代码是在SplashScreen上显示一个3D效果的字符串
                  //  g.setComposite(AlphaComposite.Clear);                                                //1     
                  //  g.fillRect(0,0,(int)splash.getSize().getWidth(),(int)splash.getSize().getHeight());  //2
                    splash.setImageURL(Main.class.getResource("/SplashTest/1.png"));      //更改显示的图片
//                    
//                    int x =30, y=100;
//                    String author ="Made By Eastsun";
//                    g.setFont(new Font("TimesRoman", Font.PLAIN, 30));
//                    g.setPaintMode();                                                                    //3
//                    for(int i=0;i<5;i++){
//                        g.setColor(new Color(200, 200, 0));
//                        g.drawString(author, x+i, y+i-1);
//                        g.setColor(new Color(100, 100, 0));
//                        g.drawString(author, x+i-1, y+i);
//                    }
//                    g.setColor(Color.yellow);
//                    g.drawString(author, x+5, y+5);
//                    splash.update();
                    Thread.sleep(10000);                                //注意,这儿是sleep(10000),但实际上SplashScreen并没有显示10秒    
                }catch(Exception e){
                    e.printStackTrace();
                }
            }
        }.start();
        
        try{
            Thread.sleep(3000);                                         //这儿决定SplashScreen显示时间长短.                                                             
        }catch(Exception e){}                                           //对于实际应用程序,这儿对应程序的初始化工作
        JFrame frame =new JFrame("Java SE6 Splash Test");
        frame.setPreferredSize(new Dimension(320,240));
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }
}
```
MANIFEST.MF文件
**[plain]**[view plain](http://blog.csdn.net/z69183787/article/details/8438864#)[copy](http://blog.csdn.net/z69183787/article/details/8438864#)[print](http://blog.csdn.net/z69183787/article/details/8438864#)[?](http://blog.csdn.net/z69183787/article/details/8438864#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/281906/fork)
- Manifest-Version: 1.0  
- Author: Eastsun  
- Created-By: 1.6.0-beta2 (Sun Microsystems Inc.)  
- SplashScreen-Image: SplashTest/ssms.png  
- Main-Class: SplashTest.Main  
```
Manifest-Version: 1.0
Author: Eastsun
Created-By: 1.6.0-beta2 (Sun Microsystems Inc.)
SplashScreen-Image: SplashTest/ssms.png
Main-Class: SplashTest.Main
```
打成jar包后（打包时需要制定manifest文件，帮助设置SplashScreen，达到启动图片的效果）：
![](https://img-my.csdn.net/uploads/201212/26/1356507217_4212.jpg)![](https://img-my.csdn.net/uploads/201212/26/1356507623_8190.jpg)![](https://img-my.csdn.net/uploads/201212/26/1356507388_5577.jpg)
以jar方式运行即可，效果如下：
![](https://img-my.csdn.net/uploads/201212/26/1356507295_2281.jpg)
线程sleep1秒 ，自动切换图片为代码中设置的 1.jpg。
到这里，已经实现了效果，但是由于执行的是jar包无法做到兼容性，所以有了下面的内容。
其实到这里，我觉得已经差不多实现了，接下来应该考虑的是如何演示。最简单的办法当然是 exe执行文件，所以我查询了如何让java 程序 以exe方式执行，结果找到了exe4j。
搜索过程中，exe4j+inno setup一直出现，但我只用到了exe4j，所以只介绍一下他的用法：
![](https://img-my.csdn.net/uploads/201212/26/1356503629_1661.jpg)
上图为 exe4j的启动界面。
在这里要强调的是，因为是java程序，执行时必须依赖于jre，所以需要首先copy一份jre到目标文件夹。
第一步：选择执行模式，我选择的是jar包执行
![](https://img-my.csdn.net/uploads/201212/26/1356503715_3426.jpg)
综上所述，需要先将程序打包成jar包才能继续使用。具体打包方式，见。。。。。
[http://blog.csdn.net/z69183787/article/details/8439178](http://blog.csdn.net/z69183787/article/details/8439178)
第三步：配置生成的应用名称，及输出目录（即上面提到的 有着jre的文件夹）
![](https://img-my.csdn.net/uploads/201212/26/1356503921_9107.jpg)
第四步：执行类型，GUI或控制台 以及exe文件名
![](https://img-my.csdn.net/uploads/201212/26/1356504034_8521.jpg)
第五步：这步比较关键，需要选择需要转换的jar包，以及选择需要执行的main函数所在的类（同时classpath中也可以添加需要关联的jar包，但切记放在同一目录下）
我使用myeclipse中的export导出jar包的，但切记选择 runnable jar包，这样会指定某一个有main方法的类，不会造成混乱。
jar包中目录结构如下：
![](https://img-my.csdn.net/uploads/201212/26/1356505341_4714.jpg)
splashTest下为 class与图片，meta-inf为上文中提到的配置文件。
![](https://img-my.csdn.net/uploads/201212/26/1356504139_8764.jpg)
第六步：配置jre
![](https://img-my.csdn.net/uploads/201212/26/1356504208_9725.jpg)
上文提到过，我们copy了一份jre，在这里就需要配置了点击
![](https://img-my.csdn.net/uploads/201212/26/1356504267_3042.jpg)
默认界面如下：读取的是电脑上的环境变量与jdk，但之前说过，需要在任意电脑上都可以运行 ，所以这里要删除2个默认配置，添加目标文件夹中的jre
![](https://img-my.csdn.net/uploads/201212/26/1356504311_2237.jpg)
修改后：
![](https://img-my.csdn.net/uploads/201212/26/1356504484_7213.jpg)
之后，选择VM类型：为客户端
![](https://img-my.csdn.net/uploads/201212/26/1356504586_9313.jpg)
最后，第一次生成的时候这个界面没有细看，直接下一步了，之后才知道其实要做的东西，这一个步骤就可以搞定。
![](https://img-my.csdn.net/uploads/201212/26/1356504684_3761.jpg)
这样默认next后，exe文件就自动生成了，效果如下
![](https://img-my.csdn.net/uploads/201212/26/1356504954_3979.jpg)
这样，大体上一个可演示的程序就完成了，最后文件夹内结构，包括了1个exe以及jre。
上面提到过，这个显示启动界面的程序用 exe4j 自带的一个功能就可以实现。
![](https://img-my.csdn.net/uploads/201212/26/1356505412_2294.jpg)
在这个步骤中，在java6的基础下，可以选择插入splash screen启动图片，相当于只要打包一个主程序，使用该软件直接可以在这个步骤插入一张启动图片，不需要像上面一样独立写一个splash启动程序，直接选取文件即可。。。。而且这样还解决了之前 使用jwindow和jframe透明性 以及 矩形边框的问题。
![](https://img-my.csdn.net/uploads/201212/26/1356505736_2437.jpg)
