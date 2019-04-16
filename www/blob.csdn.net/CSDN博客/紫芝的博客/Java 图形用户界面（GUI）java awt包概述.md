# Java 图形用户界面（GUI）java.awt包概述 - 紫芝的博客 - CSDN博客





2018年11月11日 11:09:16[紫芝](https://me.csdn.net/qq_40507857)阅读数：346








AWT是用于创建用户图形界面的一个工具包，提供了一系列用于实现图形界面的组件，如窗口、按钮、文本框、对话框等，在JDK中针对每个组件都提供对应的Java类。

这些类的继承关系：

![](https://img-blog.csdnimg.cn/2018111110251168.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

### 1.Window

Window类是不依赖于其他容器而独立存在的容器，有两个子类，分别是Frame类和Dialog类

**Frame类用于创建一个具有标题栏的框架窗口，作为程序的主界面**

**Dialog用于创建一个对话框 **

```java
import java.awt.*;

public class Example {
	public static void main(String[] args) {
		//建立新窗体对象
		Frame f=new Frame("My Frame!");
		
		//设置窗体的宽和高
		f.setSize(400,300);

		//设置窗体在屏幕中所处的位置（参数是左上角坐标）
		//f.setLocation(300, 200);
		
		Point p=new Point(300,200);
		//p.x=300;p.y=200;
		f.setLocation(p);
	
		//设置窗体可见
		f.setVisible(true);
		
		//建立一个对话框
		Dialog d=new Dialog(f,"My Dialog!");
		
		d.setLocation(100,350);
		d.setSize(200,100);
		d.setVisible(true);
	}
}
```



![](https://img-blog.csdnimg.cn/2018111110411858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

### 2.Panel

Panel不能单独存在，只能存在于其他容器（Window或其子类），一个panel对象代表一个长方形区域，可以容纳其他组件



