# 图像处理之泛洪填充算法(Flood Fill Algorithm) - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年05月10日 01:23:34[gloomyfish](https://me.csdn.net/jia20003)阅读数：38867








泛洪填充算法(Flood Fill Algorithm)

泛洪填充算法又称洪水填充算法是在很多图形绘制软件中常用的填充算法，最熟悉不过就是

windows paint的油漆桶功能。算法的原理很简单，就是从一个点开始附近像素点，填充成新

的颜色，直到封闭区域内的所有像素点都被填充新颜色为止。泛红填充实现最常见有四邻域

像素填充法，八邻域像素填充法，基于扫描线的像素填充方法。根据实现又可以分为递归与

非递归（基于栈）。



在介绍算法的三种实现方式之前，首先来看一下测试该算法的UI实现。基本思路是选择一

张要填充的图片，鼠标点击待填充的区域内部，算法会自动填充该区域，然后UI刷新。完

整的UI代码如下：



```java
package com.gloomyfish.paint.fill;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.MediaTracker;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;

public class FloodFillUI extends JComponent implements MouseListener{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private BufferedImage rawImg;
	private MediaTracker tracker;
	private Dimension mySize;
	FloodFillAlgorithm ffa;
	public FloodFillUI(File f)
	{
		try {
			rawImg = ImageIO.read(f);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		tracker = new MediaTracker(this);
		tracker.addImage(rawImg, 1);
		
		// blocked 10 seconds to load the image data
		try {
			if (!tracker.waitForID(1, 10000)) {
				System.out.println("Load error.");
				System.exit(1);
			}// end if
		} catch (InterruptedException e) {
			e.printStackTrace();
			System.exit(1);
		}// end catch
		
		mySize = new Dimension(300, 300);
		this.addMouseListener(this);
		ffa = new FloodFillAlgorithm(rawImg);
		JFrame imageFrame = new JFrame("Flood File Algorithm Demo - Gloomyfish");
		imageFrame.getContentPane().add(this);
		imageFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		imageFrame.pack();
		imageFrame.setVisible(true);
	}

	public void paint(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;
		g2.drawImage(rawImg, 10, 10, rawImg.getWidth(), rawImg.getHeight(), null);
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
	
	public static void main(String[] args) {
		JFileChooser chooser = new JFileChooser();
		chooser.showOpenDialog(null);
		File f = chooser.getSelectedFile();
		new FloodFillUI(f);
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		System.out.println("Mouse Clicked Event!!");
		int x = (int)e.getPoint().getX();
		int y = (int)e.getPoint().getY();
		System.out.println("mouse location x = " + x); // column
		System.out.println("mouse location y = " + y); // row
		System.out.println();
		long startTime = System.nanoTime();
		// ffa.floodFill4(x, y, Color.GREEN.getRGB(), ffa.getColor(x, y));
		// ffa.floodFill8(x, y, Color.GREEN.getRGB(), ffa.getColor(x, y));
		// ffa.floodFillScanLine(x, y, Color.GREEN.getRGB(), ffa.getColor(x, y)); // 13439051
		ffa.floodFillScanLineWithStack(x, y, Color.GREEN.getRGB(), ffa.getColor(x, y)); // - 16660142
		long endTime = System.nanoTime() - startTime;
		System.out.println("run time = " + endTime);
		ffa.updateResult();
		this.repaint();
	}

	@Override
	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

}
```


首先介绍四邻域的泛洪填充算法，寻找像素点p(x, y)的上下左右四个临近像素点，如果没有

被填充，则填充它们，并且继续寻找它们的四邻域像素，直到封闭区域完全被新颜色填充。

![](https://img-blog.csdn.net/20130510011203837)蓝色方格为四个邻域像素， p(x, y)为当前像素点。



基于递归实现代码很简单：



```java
public void floodFill4(int x, int y, int newColor, int oldColor)
{
    if(x >= 0 && x < width && y >= 0 && y < height 
    		&& getColor(x, y) == oldColor && getColor(x, y) != newColor) 
    { 
    	setColor(x, y, newColor); //set color before starting recursion
        floodFill4(x + 1, y,     newColor, oldColor);
        floodFill4(x - 1, y,     newColor, oldColor);
        floodFill4(x,     y + 1, newColor, oldColor);
        floodFill4(x,     y - 1, newColor, oldColor);
    }   
}
```



八邻域的填充算法，则是在四邻域的基础上增加了左上，左下，右上，右下四个相邻像素。

并递归寻找它们的八邻域像素填充，直到区域完全被新颜色填充。

![](https://img-blog.csdn.net/20130510011440796)




蓝色方格为四个邻域像素，黄色为左上，左下，右上，右下四个像素， p(x, y)为当前像素点。

基于递归实现的代码也很简单：

```java
public void floodFill8(int x, int y, int newColor, int oldColor)
{
    if(x >= 0 && x < width && y >= 0 && y < height && 
    		getColor(x, y) == oldColor && getColor(x, y) != newColor) 
    { 
    	setColor(x, y, newColor); //set color before starting recursion
        floodFill8(x + 1, y,     newColor, oldColor);
        floodFill8(x - 1, y,     newColor, oldColor);
        floodFill8(x,     y + 1, newColor, oldColor);
        floodFill8(x,     y - 1, newColor, oldColor);
        floodFill8(x + 1, y + 1, newColor, oldColor);
        floodFill8(x - 1, y - 1, newColor, oldColor);
        floodFill8(x - 1, y + 1, newColor, oldColor);
        floodFill8(x + 1, y - 1, newColor, oldColor);
    }   
}
```

基于扫描线实现的泛洪填充算法的主要思想是根据当前输入的点p(x, y)，沿y方向分别向上

与向下扫描填充，同时向左p(x-1, y)与向右p(x+1, y)递归寻找新的扫描线，直到递归结束。

代码如下：



```java
public void floodFillScanLine(int x, int y, int newColor, int oldColor)
{
	if(oldColor == newColor) return;
    if(getColor(x, y) != oldColor) return;
      
    int y1;
    
    //draw current scanline from start position to the top
    y1 = y;
    while(y1 < height && getColor(x, y1) == oldColor)
    {
    	setColor(x, y1, newColor);
        y1++;
    }    
    
    //draw current scanline from start position to the bottom
    y1 = y - 1;
    while(y1 >= 0 && getColor(x, y1) == oldColor)
    {
    	setColor(x, y1, newColor);
        y1--;
    }
    
    //test for new scanlines to the left
    y1 = y;
    while(y1 < height && getColor(x, y1) == newColor)
    {
        if(x > 0 && getColor(x - 1, y1) == oldColor) 
        {
        	floodFillScanLine(x - 1, y1, newColor, oldColor);
        } 
        y1++;
    }
    y1 = y - 1;
    while(y1 >= 0 && getColor(x, y1) == newColor)
    {
        if(x > 0 && getColor(x - 1, y1) == oldColor) 
        {
        	floodFillScanLine(x - 1, y1, newColor, oldColor);
        }
        y1--;
    } 
    
    //test for new scanlines to the right 
    y1 = y;
    while(y1 < height && getColor(x, y1) == newColor)
    {
        if(x < width - 1 && getColor(x + 1, y1) == oldColor) 
        {           
        	floodFillScanLine(x + 1, y1, newColor, oldColor);
        } 
        y1++;
    }
    y1 = y - 1;
    while(y1 >= 0 && getColor(x, y1) == newColor)
    {
        if(x < width - 1 && getColor(x + 1, y1) == oldColor) 
        {
        	floodFillScanLine(x + 1, y1, newColor, oldColor);
        }
        y1--;
    }
}
```

基于递归实现的泛洪填充算法有个致命的缺点，就是对于大的区域填充时可能导致JAVA栈溢出

错误，对最后一种基于扫描线的算法，实现了一种非递归的泛洪填充算法。

```java
public void floodFillScanLineWithStack(int x, int y, int newColor, int oldColor)
{
	if(oldColor == newColor) {
		System.out.println("do nothing !!!, filled area!!");
		return;
	}
    emptyStack();
    
    int y1; 
    boolean spanLeft, spanRight;
    push(x, y);
    
    while(true)
    {    
    	x = popx();
    	if(x == -1) return;
    	y = popy();
        y1 = y;
        while(y1 >= 0 && getColor(x, y1) == oldColor) y1--; // go to line top/bottom
        y1++; // start from line starting point pixel
        spanLeft = spanRight = false;
        while(y1 < height && getColor(x, y1) == oldColor)
        {
        	setColor(x, y1, newColor);
            if(!spanLeft && x > 0 && getColor(x - 1, y1) == oldColor)// just keep left line once in the stack
            {
                push(x - 1, y1);
                spanLeft = true;
            }
            else if(spanLeft && x > 0 && getColor(x - 1, y1) != oldColor)
            {
                spanLeft = false;
            }
            if(!spanRight && x < width - 1 && getColor(x + 1, y1) == oldColor) // just keep right line once in the stack
            {
                push(x + 1, y1);
                spanRight = true;
            }
            else if(spanRight && x < width - 1 && getColor(x + 1, y1) != oldColor)
            {
                spanRight = false;
            } 
            y1++;
        }
    }
	
}
```
运行效果：

![](https://img-blog.csdn.net/20130510012043904)

算法类源代码如下：



```java
package com.gloomyfish.paint.fill;

import java.awt.image.BufferedImage;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;

public class FloodFillAlgorithm extends AbstractBufferedImageOp {

	private BufferedImage inputImage;
	private int[] inPixels;
	private int width;
	private int height;
	
	// 	stack data structure
	private int maxStackSize = 500; // will be increased as needed
	private int[] xstack = new int[maxStackSize];
	private int[] ystack = new int[maxStackSize];
	private int stackSize;

	public FloodFillAlgorithm(BufferedImage rawImage) {
		this.inputImage = rawImage;
		width = rawImage.getWidth();
        height = rawImage.getHeight();
        inPixels = new int[width*height];
        getRGB(rawImage, 0, 0, width, height, inPixels );
	}

	public BufferedImage getInputImage() {
		return inputImage;
	}

	public void setInputImage(BufferedImage inputImage) {
		this.inputImage = inputImage;
	}
	
	public int getColor(int x, int y)
	{
		int index = y * width + x;
		return inPixels[index];
	}
	
	public void setColor(int x, int y, int newColor)
	{
		int index = y * width + x;
		inPixels[index] = newColor;
	}
	
	public void updateResult()
	{
		setRGB( inputImage, 0, 0, width, height, inPixels );
	}
	
	/**
	 * it is very low calculation speed and cause the stack overflow issue when fill 
	 * some big area and irregular shape. performance is very bad.
	 * 
	 * @param x
	 * @param y
	 * @param newColor
	 * @param oldColor
	 */
	public void floodFill4(int x, int y, int newColor, int oldColor)
	{
	    if(x >= 0 && x < width && y >= 0 && y < height 
	    		&& getColor(x, y) == oldColor && getColor(x, y) != newColor) 
	    { 
	    	setColor(x, y, newColor); //set color before starting recursion
	        floodFill4(x + 1, y,     newColor, oldColor);
	        floodFill4(x - 1, y,     newColor, oldColor);
	        floodFill4(x,     y + 1, newColor, oldColor);
	        floodFill4(x,     y - 1, newColor, oldColor);
	    }   
	}
	/**
	 * 
	 * @param x
	 * @param y
	 * @param newColor
	 * @param oldColor
	 */
	public void floodFill8(int x, int y, int newColor, int oldColor)
	{
	    if(x >= 0 && x < width && y >= 0 && y < height && 
	    		getColor(x, y) == oldColor && getColor(x, y) != newColor) 
	    { 
	    	setColor(x, y, newColor); //set color before starting recursion
	        floodFill8(x + 1, y,     newColor, oldColor);
	        floodFill8(x - 1, y,     newColor, oldColor);
	        floodFill8(x,     y + 1, newColor, oldColor);
	        floodFill8(x,     y - 1, newColor, oldColor);
	        floodFill8(x + 1, y + 1, newColor, oldColor);
	        floodFill8(x - 1, y - 1, newColor, oldColor);
	        floodFill8(x - 1, y + 1, newColor, oldColor);
	        floodFill8(x + 1, y - 1, newColor, oldColor);
	    }   
	}
	
	/**
	 * 
	 * @param x
	 * @param y
	 * @param newColor
	 * @param oldColor
	 */
	public void floodFillScanLine(int x, int y, int newColor, int oldColor)
	{
		if(oldColor == newColor) return;
	    if(getColor(x, y) != oldColor) return;
	      
	    int y1;
	    
	    //draw current scanline from start position to the top
	    y1 = y;
	    while(y1 < height && getColor(x, y1) == oldColor)
	    {
	    	setColor(x, y1, newColor);
	        y1++;
	    }    
	    
	    //draw current scanline from start position to the bottom
	    y1 = y - 1;
	    while(y1 >= 0 && getColor(x, y1) == oldColor)
	    {
	    	setColor(x, y1, newColor);
	        y1--;
	    }
	    
	    //test for new scanlines to the left
	    y1 = y;
	    while(y1 < height && getColor(x, y1) == newColor)
	    {
	        if(x > 0 && getColor(x - 1, y1) == oldColor) 
	        {
	        	floodFillScanLine(x - 1, y1, newColor, oldColor);
	        } 
	        y1++;
	    }
	    y1 = y - 1;
	    while(y1 >= 0 && getColor(x, y1) == newColor)
	    {
	        if(x > 0 && getColor(x - 1, y1) == oldColor) 
	        {
	        	floodFillScanLine(x - 1, y1, newColor, oldColor);
	        }
	        y1--;
	    } 
	    
	    //test for new scanlines to the right 
	    y1 = y;
	    while(y1 < height && getColor(x, y1) == newColor)
	    {
	        if(x < width - 1 && getColor(x + 1, y1) == oldColor) 
	        {           
	        	floodFillScanLine(x + 1, y1, newColor, oldColor);
	        } 
	        y1++;
	    }
	    y1 = y - 1;
	    while(y1 >= 0 && getColor(x, y1) == newColor)
	    {
	        if(x < width - 1 && getColor(x + 1, y1) == oldColor) 
	        {
	        	floodFillScanLine(x + 1, y1, newColor, oldColor);
	        }
	        y1--;
	    }
	}
	
	public void floodFillScanLineWithStack(int x, int y, int newColor, int oldColor)
	{
		if(oldColor == newColor) {
			System.out.println("do nothing !!!, filled area!!");
			return;
		}
	    emptyStack();
	    
	    int y1; 
	    boolean spanLeft, spanRight;
	    push(x, y);
	    
	    while(true)
	    {    
	    	x = popx();
	    	if(x == -1) return;
	    	y = popy();
	        y1 = y;
	        while(y1 >= 0 && getColor(x, y1) == oldColor) y1--; // go to line top/bottom
	        y1++; // start from line starting point pixel
	        spanLeft = spanRight = false;
	        while(y1 < height && getColor(x, y1) == oldColor)
	        {
	        	setColor(x, y1, newColor);
	            if(!spanLeft && x > 0 && getColor(x - 1, y1) == oldColor)// just keep left line once in the stack
	            {
	                push(x - 1, y1);
	                spanLeft = true;
	            }
	            else if(spanLeft && x > 0 && getColor(x - 1, y1) != oldColor)
	            {
	                spanLeft = false;
	            }
	            if(!spanRight && x < width - 1 && getColor(x + 1, y1) == oldColor) // just keep right line once in the stack
	            {
	                push(x + 1, y1);
	                spanRight = true;
	            }
	            else if(spanRight && x < width - 1 && getColor(x + 1, y1) != oldColor)
	            {
	                spanRight = false;
	            } 
	            y1++;
	        }
	    }
		
	}
	
	private void emptyStack() {
		while(popx() != - 1) {
			popy();
		}
		stackSize = 0;
	}

	final void push(int x, int y) {
		stackSize++;
		if (stackSize==maxStackSize) {
			int[] newXStack = new int[maxStackSize*2];
			int[] newYStack = new int[maxStackSize*2];
			System.arraycopy(xstack, 0, newXStack, 0, maxStackSize);
			System.arraycopy(ystack, 0, newYStack, 0, maxStackSize);
			xstack = newXStack;
			ystack = newYStack;
			maxStackSize *= 2;
		}
		xstack[stackSize-1] = x;
		ystack[stackSize-1] = y;
	}
	
	final int popx() {
		if (stackSize==0)
			return -1;
		else
            return xstack[stackSize-1];
	}

	final int popy() {
        int value = ystack[stackSize-1];
        stackSize--;
        return value;
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		// TODO Auto-generated method stub
		return null;
	}

}
```
**转载文章请务必注明**


