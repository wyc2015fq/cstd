# 基于Swing 实现放大镜效果 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年10月15日 23:27:30[gloomyfish](https://me.csdn.net/jia20003)阅读数：4365








**一：概述**

本文代码在JDK6u20版本调试通过，通过对像素的抓取处理和利用TexturePaint对BufferedImage对象的支持

从而实现对任何JComponent组件的放大镜效果。




**二：前期准备工作**



1.      将传入的JComponent对象转换为BufferedImage对象做为**源图像**，有下面代码完成，关

键步骤在于从BufferedImage对象创建自己的图形设备对象



orginalImage = **new** BufferedImage(**this**.srcComponent.getSize().width,

**this**.srcComponent.getSize().height, BufferedImage.*TYPE_INT_ARGB*);

Graphics g = orginalImage.createGraphics();

**this**.srcComponent.paintAll(g);


2. 从源BufferedImage对象中读取所有的像素数据，这个在后面将会用到。代码如下



originalPixels = **newint**[orginalImage.getWidth()*orginalImage.getHeight()];

orginalImage.getRaster().getDataElements(0,0, orginalImage.getWidth(), 

orginalImage.getHeight(), originalPixels)




3. 创建放大镜组件，使用一个自定义的JFrame对象，去掉边框，使用形状为圆形，实现代码如下：



zoomFrame = **new** JFrame("MagnifyGlass");

zoomFrame.setUndecorated(**true**);

Shape shape = **new** Ellipse2D.Double(0, 0,180, 180);

AWTUtilitiesWrapper.*setWindowShape*(zoomFrame,shape);




**三：主要思路及关键代码**



1. 捕获鼠标在源图像上的位置移动，这个需要完成MouseMotionListener接口，实现对位置捕获的代码如下：

**double** x = e.getPoint().getX();

**double** y = e.getPoint().getY();




2.  根据鼠标在源图像上的位置，抓取源图像上像素，抓取像素的范围取决于放缩的比例因子

// get garb area rectangle

****int**** grabCols = (**int**)((****double****)mySize.width /zoomFactor);

****int**** grabRows = (****int****)((****double****)mySize.height /zoomFactor);




3. 从源图像对象一维像素数组中获取要放大的三维像素数组，本实现的关键点之一，实现代码如下：

首先定义三维像素数组对象：

****int****[][][] data = **new**int****[grabRows][grabCols][4];

转换为三维ARGB的像素数组：

// Alpha data

data[row][col][0] = (aRow[col] >> 24) & 0xFF;

// Red data

data[row][col][1] = (aRow[col] >> 16) & 0xFF;

// Green data

data[row][col][2] = (aRow[col] >> 8) & 0xFF;

// Blue data

data[row][col][3]= (aRow[col]) & 0xFF;




4. 调用方法convertToOneDim()将抓取到的像素转换为一维数组，放入到创建的BufferedImage对象中，实现代码如下：

grabImage.getRaster().setDataElements( 0, 0,grabWidth, grabHeight, oneDimPixelData );




5.利用TexturePaint来使用grabImage对象来填充放大镜组件



TexturePaint tp = **new** TexturePaint((BufferedImage)grabImage,rect);

g2.setPaint(tp);

g2.fill(circle1);

6. 获得screen坐标来实现放大镜对鼠标的跟随效果



获取鼠标的screen坐标

// get screen point

Point offsetPoint = srcComponent.getLocationOnScreen();

e.translatePoint(offsetPoint.x, offsetPoint.y);

screenPoint = e.getPoint();


实现放大镜跟随鼠标效果：



zoomFrame.setLocation(screenPoint);

zoomFrame.setAlwaysOnTop(**true**);


7.处理边缘像素放大问题，空白部分用白色填充

data[row][col][0] = 0xFF; // alpha


data[row][col][1] = 0xFF; // red


data[row][col][2] = 0xFF; // green


data[row][col][3] = 0xFF; // blue





8. 最终效果如下图：

![](http://hi.csdn.net/attachment/201110/17/0_1318853736es01.gif)





全部源代码及测试程序如下：



```java
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Paint;
import java.awt.Point;
import java.awt.RadialGradientPaint;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.TexturePaint;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;

import javax.swing.JComponent;
import javax.swing.JFrame;

public class PixelMagnifyGlass extends JComponent implements MouseMotionListener {
	
	/**
	 * magnify the image based on pixel data.
	 */
	
	private static final long serialVersionUID = -1632282149543956832L;
	private double zoomFactor;
	private JComponent srcComponent;
	private Point imgPoint;
	private Point screenPoint;
	private JFrame zoomFrame;
	private Dimension mySize;
	private BufferedImage orginalImage;
	private int[] originalPixels;
	
	public PixelMagnifyGlass(JComponent comp, Dimension size, double zoom) {
		 this.srcComponent = comp;
		 screenPoint = new Point(-1, -1);
		 comp.addMouseMotionListener(this);
		 this.mySize = size;
		 this.zoomFactor = zoom;
		 
		 // convert the JComponent to image
		 orginalImage = new BufferedImage(this.srcComponent.getSize().width, 
				 		this.srcComponent.getSize().height, BufferedImage.TYPE_INT_ARGB);
		 Graphics g = orginalImage.createGraphics();
		 this.srcComponent.paintAll(g);
		 g.dispose();
		 
		 // get all pixel arrays
		 originalPixels = new int[orginalImage.getWidth()*orginalImage.getHeight()];
		 orginalImage.getRaster().getDataElements( 0, 0, orginalImage.getWidth(), orginalImage.getHeight(), originalPixels);

		 
		 // start the glass component
		zoomFrame = new JFrame("MagnifyGlass");
		zoomFrame.setUndecorated(true);
		Shape shape = new Ellipse2D.Double(0, 0,mySize.getWidth(), mySize.getHeight());
		AWTUtilitiesWrapper.setWindowShape(zoomFrame, shape);
		 
	}
	
	public void paint(Graphics g) {
		if(imgPoint.x == -1) {
			g.setColor(Color.BLUE);
			g.fillRect(0,0, mySize.width, mySize.height);
			return;
		}
		
		// draw zoom in/out image here
		Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		int grabWidth = (int)((double)mySize.width / zoomFactor);
		int grabHeight = (int)((double)mySize.height / zoomFactor);
		int[][][] threeDimPixelData = clipImageToThreeDim(originalPixels, (int)imgPoint.getX(), (int)imgPoint.getY());
		int[] oneDimPixelData = convertToOneDim(threeDimPixelData, grabWidth, grabHeight);
		ColorModel cMD = orginalImage.getColorModel();
		BufferedImage grabImage = new BufferedImage(orginalImage.getColorModel(), cMD.createCompatibleWritableRaster(grabWidth, grabHeight), cMD.isAlphaPremultiplied(), null);
		grabImage.getRaster().setDataElements( 0, 0, grabWidth, grabHeight, oneDimPixelData );
		
		Image scaleImg = grabImage.getScaledInstance(mySize.width, mySize.height, Image.SCALE_FAST);
		// g2.drawImage(scaleImg, 0,0,null);
		
		// add glass edge with black
		Shape innerCircle = new Ellipse2D.Float(15.5f, 15.5f, mySize.width - 30, mySize.height - 30);
		Ellipse2D outterCircle = new Ellipse2D.Float(0, 0, mySize.width, mySize.height);
		Paint gp = new RadialGradientPaint((float)outterCircle.getCenterX(), (float)outterCircle.getCenterY(),
				 							(mySize.width)/2,
											new float[]{0f,0.8f,1f},new Color[]{Color.WHITE,Color.GRAY,Color.BLACK});
		g2.setPaint(gp);
		g2.fill(outterCircle);
		Rectangle rect = new Rectangle(0,0,scaleImg.getWidth(null), scaleImg.getHeight(null));
		TexturePaint tp = new TexturePaint((BufferedImage)grabImage, rect);
		g2.setPaint(tp);
		g2.fill(innerCircle);
	}
	
	public Dimension getPreferredSize() {
		return mySize;
	}
	
	public Dimension getMinimumSize() {
		return mySize;
	}
	
	public Dimension getMaximumSize() {
		return mySize;
	}
	
	
	@Override
	public void mouseDragged(MouseEvent e) {
		mouseMoved(e);
		
	}
	
	public int[][][] clipImageToThreeDim(int[] oneDPix, int startCols, int startRows) {
		
		// get garb area rectangle
		int grabCols = (int)((double)mySize.width / zoomFactor);
		int grabRows = (int)((double)mySize.height / zoomFactor);
		int[][][] data = new int[grabRows][grabCols][4];
		
		for (int row = 0; row < grabRows; row++) {
			int[] aRow = new int[grabCols];
			for (int col = 0; col < grabCols; col++) {
				int element = (row + startRows) * orginalImage.getWidth() + (startCols + col);
				
				// image edge detection and assign it as white color.
				if(element >= oneDPix.length || (startCols + col) >= orginalImage.getWidth()) {
					aRow[col] = 0;
				} else {
					aRow[col] = oneDPix[element];
				}
			}
			
			for (int col = 0; col < grabCols; col++) {
				if(aRow[col] == 0) {
					// Alpha data
					data[row][col][0] = 0xFF;
					// Red data
					data[row][col][1] = 0xFF;
					// Green data
					data[row][col][2] = 0xFF;
					// Blue data
					data[row][col][3] = 0xFF;
				} else {
					// Alpha data
					data[row][col][0] = (aRow[col] >> 24) & 0xFF;
					// Red data
					data[row][col][1] = (aRow[col] >> 16) & 0xFF;
					// Green data
					data[row][col][2] = (aRow[col] >> 8) & 0xFF;
					// Blue data
					data[row][col][3] = (aRow[col]) & 0xFF;
				}
			} // end for loop on column

		}
		return data;
	}
	
	/* <p> The purpose of this method is to convert the data in the 3D array of ints back into </p>
	 * <p> the 1d array of type int. </p>
	 * 
	 */
	public int[] convertToOneDim(int[][][] data, int imgCols, int imgRows) {
		// Create the 1D array of type int to be populated with pixel data
		int[] oneDPix = new int[imgCols * imgRows * 4];

		// Move the data into the 1D array. Note the
		// use of the bitwise OR operator and the
		// bitwise left-shift operators to put the
		// four 8-bit bytes into each int.
		for (int row = 0, cnt = 0; row < imgRows; row++) {
			for (int col = 0; col < imgCols; col++) {
				oneDPix[cnt] = ((data[row][col][0] << 24) & 0xFF000000)
						| ((data[row][col][1] << 16) & 0x00FF0000)
						| ((data[row][col][2] << 8) & 0x0000FF00)
						| ((data[row][col][3]) & 0x000000FF);
				cnt++;
			}// end for loop on col

		}// end for loop on row

		return oneDPix;
	}// end convertToOneDim

	@Override
	public void mouseMoved(MouseEvent e) {
		double x = e.getPoint().getX();
		double y = e.getPoint().getY();
		
		//  we did not need to handle edge pixel like this again.
		//	if((x + mySize.getWidth()) > this.orginalImage.getWidth()) {
		//		x = x - ((x + mySize.getWidth()) - this.orginalImage.getWidth());
		//	}
		//	if((y + mySize.getHeight()) > this.orginalImage.getHeight()) {
		//		y = y - ((y + mySize.getHeight()) - this.orginalImage.getHeight());
		//	}
		//  end comment by gloomy fish on 15-October-2011
		
		imgPoint = new Point((int)x, (int)y);
		
		// get screen point
		Point offsetPoint = srcComponent.getLocationOnScreen();
		e.translatePoint(offsetPoint.x, offsetPoint.y);
		screenPoint = e.getPoint();
		repaint();
		invokeGlass();
	}
	
	/**
	 * 
	 */
	public void invokeGlass() {
		if(imgPoint.x == -1) {
			return;
		}
		
		if(zoomFrame.isVisible()) {
			zoomFrame.getContentPane().remove(this);
			zoomFrame.getContentPane().add(this);
			zoomFrame.setLocation(screenPoint);
		} else {
			zoomFrame.getContentPane().remove(this);
			zoomFrame.getContentPane().add(this);
			zoomFrame.pack();
			zoomFrame.setLocation(screenPoint);
			zoomFrame.setVisible(true);
			zoomFrame.setAlwaysOnTop(true);
		}
	}

}
```



测试程序：





```java
import java.awt.Dimension;
import java.io.File;

import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class TestDetachedMagnifyingGlass {
	
	public TestDetachedMagnifyingGlass(File f) {
		ImageIcon i = new ImageIcon(f.getPath());
		JLabel label = new JLabel(i);
		JFrame imageFrame = new JFrame("IMage");
		imageFrame.getContentPane().add(label);
		imageFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		imageFrame.pack();
		imageFrame.setVisible(true);
		
		new PixelMagnifyGlass(label, new Dimension(180,180),2.0);
		// mag.invokeGlass();
	}
	
	public static void main(String[] args) {
		JFileChooser chooser = new JFileChooser();
		chooser.showOpenDialog(null);
		File f = chooser.getSelectedFile();
		new TestDetachedMagnifyingGlass(f);
	}

}
```







