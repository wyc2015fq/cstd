# JAVA实现自动打开URL对应的网页并保存为图片-不借助第三方API - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年04月14日 22:17:41[gloomyfish](https://me.csdn.net/jia20003)阅读数：6640








基本原理：

JDK6u10版本以上提供了DeskTop的API接口支持，可以根据URI参数自动打开

操作系统默认的浏览器来加载对应的网页，同时借助JDK的Robot类实现自动截屏

这样就完成了对任意一个URL对应网页保存为图片。程序如下：



```java
package com.gloomyfish.image.util;

import java.awt.AWTException;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URI;

import javax.imageio.ImageIO;

public class DeskTopTest {
	public static void main(String[] args) {
		if (!java.awt.Desktop.isDesktopSupported()) {
			System.err.println("Desktop is not supported (fatal)");
			System.exit(1);
		}

		java.awt.Desktop desktop = java.awt.Desktop.getDesktop();
		if (!desktop.isSupported(java.awt.Desktop.Action.BROWSE)) {
			System.err.println("Desktop doesn't support the browse action (fatal)");
			System.exit(1);
		}
		
		try {
			URI uri = URI.create("http://www.csdn.net");
			desktop.browse(uri);
			Thread.sleep(8000); // 8 seconds is enough to load the any page.
			Robot robot = new Robot();
			// Rectangle rectangle = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize() );
			Rectangle rectangle = new Rectangle(300, 90, 1000, 720);
			BufferedImage image = robot.createScreenCapture(rectangle);
			File outputfile = new File("D:\\agriculture\\test.jpg");
			ImageIO.write(image, "jpg", outputfile);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (AWTException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} 
	}
}
```
**缺点：**


程序运行时用户不能有其它操作，否则可能保存错误截屏。 这里

假设加载一个网页时间最长为8秒.

**转载请注明出自gloomyfish**






