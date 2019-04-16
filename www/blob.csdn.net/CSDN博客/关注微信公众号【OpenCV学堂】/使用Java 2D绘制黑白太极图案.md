# 使用Java 2D绘制黑白太极图案 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年12月27日 13:34:05[gloomyfish](https://me.csdn.net/jia20003)阅读数：5420








**一：基本原理**

利用Java 2D的Area对象对绘制形状几何操作的支持，完成太极图案的绘制，使用Paint来

完成对不同颜色的填充。Java 2D图形API Area对Shape支持四种几何操作：

-         Add （加）- 保留两个几何形状及其重叠部分

-         Subtract (减) – 从第一个几何形状上减去和第二个重叠的部分，保留减去之后的第一个几

       何形状

-         Intersect (与) – 只保留两个几何形状重叠的部分。

-         ExclusiveOr(或) – 保留他们相互不重叠的部分。

参看下图：

![](http://hi.csdn.net/attachment/201112/27/0_1324963757zIy9.gif)




**二：程序运行效果**
![](http://hi.csdn.net/attachment/201112/27/0_1324963803HYYv.gif)



程序基于JDK6 API完成。

背景图片我是在网上找来的，可以自己替换的。




**程序源代码如下：**

```java
package com.gloomyfish.swing;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.geom.Area;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.JComponent;
import javax.swing.JFrame;

public class YingYangGraphics extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 8812325148970066491L;
	private BufferedImage image = null;
	public YingYangGraphics() {
		super();
		this.setOpaque(false);
	}
	protected void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D)g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2.drawImage(getImage(), 0, 0, image.getWidth(), image.getHeight(), null);
		Shape lefthalfCirle = new Ellipse2D.Double(10,10, 300,300); // R = 150
		Shape righthalfCircle = new Ellipse2D.Double(10,10, 300,300); // R = 150
		Shape innerCircle1 = new Ellipse2D.Double(85,10, 150,150); // R/2 = 75
		Shape innerCircle2 = new Ellipse2D.Double(85,160, 150,150); // R = 150
	
		Shape rectangel1 = new Rectangle2D.Double(160, 10, 150, 300);
		Shape rectangel2 = new Rectangle2D.Double(10, 10, 150, 300);
		
		Area left = new Area(lefthalfCirle);
		Area right = new Area(righthalfCircle);
		
		Area area11 = new Area(rectangel1);
		Area area22 = new Area(rectangel2);
		
		left.subtract(area11);
		right.subtract(area22);
		
		Area inner1 = new Area(innerCircle1);
		Area inner2 = new Area(innerCircle2);
		
		left.add(inner1);
		//left.subtract(inner2);
		right.add(inner2);
		right.subtract(inner1); // trick is here !!!
		
		
		// create minor circle here!!!
		Shape minorWhiteCircle = new Ellipse2D.Double(150,70, 20,20); // ++ 60
		Shape innerBlackCircle = new Ellipse2D.Double(150,230, 20,20); // R = 150
		
		// draw two big frame shape here...
		g2.setPaint(Color.WHITE);
		g2.fill(left);
		g2.setPaint(Color.BLACK);
		g2.fill(right);
		
		// draw minor circle here!!!
		g2.fill(minorWhiteCircle);
		g2.setPaint(Color.WHITE);
		g2.fill(innerBlackCircle);
	}
	
	private BufferedImage getImage() {
		if(image != null) {
			return image;
		}
		try {
			File file = new File("D:\\android\\blue_flower.jpg");
			image = ImageIO.read(file);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return image;
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("Test Panel");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BorderLayout());
		
		// Display the window.
		frame.getContentPane().add(new YingYangGraphics(), BorderLayout.CENTER);
		frame.setPreferredSize(new Dimension(330,350));
		frame.pack();
		frame.setVisible(true);
	}
}
```




