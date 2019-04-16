# Java Swing模拟水波纹扩散效果动画 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年11月03日 22:31:23[gloomyfish](https://me.csdn.net/jia20003)阅读数：7485








基于Java语言模拟水波纹运动效果，分为两种方法，一种采用简单的叠加计算

不使用sine函数模拟水波纹，好处是计算量小，另外一种采用sine函数来计算

水波纹扩展，计算量大，但是效果比较真实。




第一种简单的叠加效果水波模拟，是很多简单的2D游戏中会用的，关键是计

算水波的迁移，然后剩以能量衰减因子。Java实现的代码如下：



```java
for(y=1; y<waveWidth-1; y++)
{
	for(x= 1; x<waveHeight-1; x++)
	{
		int n = (waveMapData[current][y-1][x] + 
				waveMapData[current][y+1][x] + 
				waveMapData[current][y][x+1] +
				waveMapData[current][y][x-1])/2 -
				waveMapData[before][y][x];
		// energy lost
		n = n - n / damp;
		/*if(Math.abs(n) >= Math.abs(-100))
		{
			n = n % Math.abs(-100);
		}*/
		waveMapData[next][y][x] = n;
		// System.out.print(" " + n);
	}
	// System.out.println();
}
```
解释：
主要是利用前两个水波的位置，计算出下一个将要出现的水波的位置，因此非常

重要的是保存前两个水波的迁移信息，然后实现波的叠加计算，在加上能量损失

因子，即可获得下个波的位置。

这种方法模拟出来的水波不是很圆，如果想得到那种很圆的水波效果，建议看

这里：[http://blog.csdn.net/jia20003/article/details/13159535](http://blog.csdn.net/jia20003/article/details/13159535)

解释完水波模拟的原理部分，下面来说说JAVA Swing的动画原理

主要是利用一个线程来计算水波的新位置，然后重新绘制图片，图片绘制好

以后调用repaint（）方法，自动触发Swing重绘机制，实现刷新显示效果。

水波计算线程代码如下：



```java
Thread animationThread = new Thread () {
    @Override
    public void run() {
    	int k = 0;
       while (true) {
    	  startWaterWave();
    	  // sinWaterRipple();
    	  updateText("水波开展：" + k);
          repaint();  // Refresh the display
          try {
             Thread.sleep(1000 / 30); // delay and yield to other threads
          } catch (InterruptedException ex) { }
          k++;
          if(k>=100){
        	  break;
          }
       }
    }
 };
 animationThread.start();
```
最后说一下Java的一个数据Copy的函数System.arraycopy，如果在多维数据中交


换数据你可能发现由于它是指针交换，交换完以后对新的数组赋值会同时跑到旧的

里面去。这个是我在debug程序的时候才发现。惭愧啊！程序效果如下：

![](https://img-blog.csdn.net/20131103222717406)
**UI加水波计算的完全源代码如下：**


```java
package com.gloomyfish.water.ripple.study;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import com.gloomyfish.filter.study.WaterFilter;

public class MyDemoUI extends JFrame implements ActionListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3702707300863726479L;
	private BufferedImage textureImg = null;
	private BufferedImage resultImg = null;
	private int waveWidth = 40;
	private int waveHeight = 40;
	private int damp = 16;
	private int[][][] waveMapData;
	private int before = 0, current = 1, next = 2;
	private int x = 1, y = 1 /*n = 0*/;
	private WaterPixelRender imageRender;
	private WaterFilter filter;
	private JButton runBtn;
	private JLabel txtLabel;
	private boolean text = false;
	public MyDemoUI()
	{
		super("Water Ripple");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        getContentPane().setLayout(new BorderLayout());  
          
        // Display the window.  
        getContentPane().add(createWaterPanel(), BorderLayout.CENTER);  
        runBtn = new JButton("Run it");
        runBtn.addActionListener(this);
        
        txtLabel = new JLabel();
        txtLabel.setText("水纹：0");
        getContentPane().add(runBtn, BorderLayout.SOUTH);
        getContentPane().add(txtLabel, BorderLayout.NORTH);
        setPreferredSize(new Dimension(300 + 25,500));  
        pack();  
        setVisible(true);  
	}
	
	public void updateText(String textContent)
	{
		txtLabel.setText(textContent);
		this.invalidate();
	}

	private JPanel createWaterPanel() {
		try {
			File file = new File("D:\\resource\\flower_001.png");
			textureImg = ImageIO.read(file);
			resultImg = textureImg;
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		imageRender = new WaterPixelRender();
		filter = new WaterFilter();
		JPanel wPanel = new JPanel()
		{
			protected void paintComponent(Graphics g) {
				Graphics2D g2 = (Graphics2D) g;
				g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
				if (resultImg != null) {
					g2.drawImage(resultImg, 0, 0, resultImg.getWidth(), resultImg.getHeight(), null);
				}
			}
		};
		return wPanel;
	}
	
	public void swapData()
	{
		for(int i=0; i<waveWidth; i++)
		{
			for(int j=0; j<waveHeight; j++)
			{
				waveMapData[before][i][j] = waveMapData[current][i][j];
				waveMapData[current][i][j] = waveMapData[next][i][j];
			}
		}
	}
	
	public void simulateWaterRipple()
	{
		x = 1;
		y = 1;
		//n = 0;
		waveMapData = new int[3][textureImg.getHeight()][textureImg.getWidth()];
		waveWidth = textureImg.getHeight();
		waveHeight = textureImg.getWidth();
		for(int i=0; i<waveWidth; i++)
		{
			for(int j=0; j<waveHeight; j++)
			{
				waveMapData[next][i][j] = 0;
				waveMapData[current][i][j] = 0;
				waveMapData[before][i][j] = 0;
			}
		}
		waveMapData[current][waveWidth/2][waveHeight/2] = -300;
		Thread animationThread = new Thread () {
		    @Override
		    public void run() {
		    	int k = 0;
		       while (true) {
		    	  startWaterWave();
		    	  // sinWaterRipple();
		    	  updateText("水波开展：" + k);
		          repaint();  // Refresh the display
		          try {
		             Thread.sleep(1000 / 30); // delay and yield to other threads
		          } catch (InterruptedException ex) { }
		          k++;
		          if(k>=100){
		        	  break;
		          }
		       }
		    }
		 };
		 animationThread.start();
	}
    
	protected void sinWaterRipple() {
		resultImg = filter.filter(textureImg, null);
		filter.setRadius(filter.getRadius() + 5);
		if(filter.getRadius() > 150)
		{
			filter.setRadius(50);
		}
		float wl = filter.getWavelength();
		if(wl > 4)
		{
			wl = wl / 2;
		}
		else
		{
			wl = 32;
		}
		filter.setWavelength(wl);
	}

	protected void startWaterWave(/*int sx, int sy, Graphics2D g2*/) 
	{
		for(y=1; y<waveWidth-1; y++)
		{
			for(x= 1; x<waveHeight-1; x++)
			{
				int n = (waveMapData[current][y-1][x] + 
						waveMapData[current][y+1][x] + 
						waveMapData[current][y][x+1] +
						waveMapData[current][y][x-1])/2 -
						waveMapData[before][y][x];
				// energy lost
				n = n - n / damp;
//				if(Math.abs(n) >= Math.abs(-100))
//				{
//					n = n % Math.abs(-100);
//				}
				waveMapData[next][y][x] = n;
				// System.out.print(" " + n);
			}
			// System.out.println();
		}
		// render image pixel and display water ripple at here
		imageRender.setWaveData(waveMapData[next]);
		resultImg = imageRender.filter(textureImg, null);
		// prepare for the next water ripple
		// Java System.arraycopy give very bad result due to reference issue!!!!
		// System.arraycopy(waveMapData[next], 0, waveMapData[current], 0, waveMapData[next].length);
		swapData();
	}

	public static void main(String[] args)
	{
		new MyDemoUI();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run(){
				simulateWaterRipple();		
			}
		});		
	}
}
```
**绘制水波像素的生产新图片的代码如下：**




```java
package com.gloomyfish.water.ripple.study;

import java.awt.image.BufferedImage;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;


public class WaterPixelRender extends AbstractBufferedImageOp {

	private int[][] waveData;
	private double rIndex = 2.0;
	private int counter = 0;
	public WaterPixelRender()
	{
		
	}
	
	public int[][] getWaveData() {
		return waveData;
	}
	public void setWaveData(int[][] waveData) {
		this.waveData = waveData;
	}
	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		counter++;
		int width = src.getWidth();
        int height = src.getHeight();

        if ( dest == null )
        	dest = createCompatibleDestImage( src, null );

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB( src, 0, 0, width, height, inPixels );
        int index = 0;
        int nrow = 0, ncol = 0;
        for(int row=0; row<height-1; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=0; col<width-1; col++) {
        		
                int xDiff = waveData[row+1][col] - waveData[row][col];
            	int yDiff = waveData[row][col+1] - waveData[row][col];
            	double xAngle = Math.atan(xDiff);
            	double xRefraction = Math.asin( Math.sin( xAngle ) / rIndex );
            	double xDisplace = Math.tan( xRefraction ) * xDiff ;

            	double yAngle = Math.atan( yDiff );
            	double yRefraction = Math.asin( Math.sin( yAngle ) / rIndex );
            	double yDisplace =Math.tan(yRefraction ) * yDiff;

            	if(xDiff < 0) 
            	{
            		if(yDiff < 0)
            		{
            			nrow = (int)(row - xDisplace);
            			ncol = (int)(col - yDisplace);
            		}
            		else
            		{
            			nrow = (int)(row - xDisplace);
            			ncol = (int)(col + yDisplace);
            		}
            	}
            	else
            	{
            		if(yDiff < 0)
            		{
            			nrow = (int)(row + xDisplace);
            			ncol = (int)(col - yDisplace);
            		}
            		else
            		{
            			nrow = (int)(row + xDisplace);
            			ncol = (int)(col + yDisplace);
            		}
            	}
            	if(nrow < 0 || nrow >= height)
            	{
            		nrow = 0;
            	}
            	if(ncol < 0 || ncol >= width)
            	{
            		ncol = 0;
            	}
            	
            	
        		index = nrow * width + ncol;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                index = row * width + col;
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
        	}
        }
        setRGB( dest, 0, 0, width, height, outPixels );
        return dest;
	}
}
```
**复制代码以后替换图片路径即可运行，转载请注明出处**




