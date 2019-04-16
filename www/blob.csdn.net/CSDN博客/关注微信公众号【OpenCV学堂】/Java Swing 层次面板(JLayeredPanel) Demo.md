# Java Swing 层次面板(JLayeredPanel) Demo - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年06月28日 17:17:02[gloomyfish](https://me.csdn.net/jia20003)阅读数：15183








**概述：**

JLayeredPanel 作为Java层次面板是创建多编辑窗口与实现某些特殊显示层次效果的最好方法

当前已经内置支持JLayeredPanel的组件包括JFrame,JDialog,JDesktopPanel本身就是个

JLayeredPanel对象，JLayeredPanel中可以包含多个JComponent组件对象，并且可以相互切换

到编辑状态。



**Demo程序：**

演示JLayeredPanel如何添加JComponent对象，利用JLayeredPanel的特性实现相框效果，主要思路

如下：

1.       创建一个JLayeredPanel对象实例，声明大小

2.       创建一个背景面板(JComponent)对象，贴上你要显示的图像

3.       创建一个前景面板(JComponent)对象，选择一种相框，我是网上找的素材

4.       对前景相框面板进行透明度设置，设置相框内矩形为透明alpha=0

5.       继承JComponent对象的paintComponent()方法把图像绘制到各自面板上

完成测试UI编码，效果显示如下：

![](https://img-my.csdn.net/uploads/201206/28/1340874984_5847.png)


两张源图像分别为：

![](https://img-my.csdn.net/uploads/201206/28/1340875025_1608.png)


相框图：

![](https://img-my.csdn.net/uploads/201206/28/1340875049_7546.jpg)




**特别提示：**由于JLayeredPanel没有layoutManager所以必须完成设置setBounds()函数对每个添加到JLayeredPanel的对象，否则会导致无法显示该组件。



程序源代码：

背景面板类- JBackGroundPanel



```java
package com.gloomyfish.layer.demo;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;
import javax.swing.JComponent;

public class JBackGroundPanel extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 4639168655243672491L;
	private Dimension size;
	private int startx;
	private int starty;
	private BufferedImage image;
	public JBackGroundPanel() {
		super();
		setOpaque(false);
		startx = 30;
		starty = 30;
		if(size == null) {
			size = new Dimension(400, 400);
		}
		this.setBounds(startx, starty, size.width / 2, size.height / 2);
	}
	
	public JBackGroundPanel(int xp, int yp, int width, int height) {
		super();
		setOpaque(false);
		startx = xp;
		starty = yp;
		size = new Dimension(width, height);
		this.setBounds(startx, starty, size.width, size.height);
	}
	
	private void loadImage() {
		try {
			InputStream stream = this.getClass().getResourceAsStream("frame.jpg");
			image = tunningAlpha(ImageIO.read(stream));
			
		} catch (IOException e) {
			System.out.println("Could not load the image resource...");
		}
	}

	private BufferedImage tunningAlpha(BufferedImage src) {
		int width = src.getWidth();
        int height = src.getHeight();

        int[] inPixels = new int[width*height];
        int[] outPixels = new int[width*height];
        getRGB(src, 0, 0, width, height, inPixels );
        System.arraycopy(inPixels, 0, outPixels, 0, width*height);
        int index = 0;
        for(int row=90; row<height-90; row++) {
        	int ta = 0, tr = 0, tg = 0, tb = 0;
        	for(int col=90; col<width-90; col++) {
        		index = row * width + col;
        		ta = (inPixels[index] >> 24) & 0xff;
                tr = (inPixels[index] >> 16) & 0xff;
                tg = (inPixels[index] >> 8) & 0xff;
                tb = inPixels[index] & 0xff;
                ta = 0;
                outPixels[index] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
                
        	}
        }
        BufferedImage dest = new BufferedImage(width, height, BufferedImage.TYPE_4BYTE_ABGR);
        setRGB(dest, 0, 0, width, height, outPixels);
        return dest;
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// do something here.....
		if(image == null) {
			loadImage();
		}
		g2.drawImage(image, startx, starty, size.width, size.height, null);
	}
	
	@Override
	public Dimension getPreferredSize() {
		return size;
	} 
	
	public Dimension getMinimumSize() {
		return size;
	}
	
	/**
	 * A convenience method for getting ARGB pixels from an image. This tries to avoid the performance
	 * penalty of BufferedImage.getRGB unmanaging the image.
	 */
	public int[] getRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) {
		int type = image.getType();
		if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
			return (int [])image.getRaster().getDataElements( x, y, width, height, pixels );
		return image.getRGB( x, y, width, height, pixels, 0, width );
    }

	/**
	 * A convenience method for setting ARGB pixels in an image. This tries to avoid the performance
	 * penalty of BufferedImage.setRGB unmanaging the image.
	 */
	public void setRGB( BufferedImage image, int x, int y, int width, int height, int[] pixels ) {
		int type = image.getType();
		if ( type == BufferedImage.TYPE_INT_ARGB || type == BufferedImage.TYPE_INT_RGB )
			image.getRaster().setDataElements( x, y, width, height, pixels );
		else
			image.setRGB( x, y, width, height, pixels, 0, width );
    }
	
}
```
**JMainPanel 类的源代码:**



```java
package com.gloomyfish.layer.demo;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;
import javax.swing.JComponent;

public class JMainPanel extends JComponent {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4973612988991524751L;
	private Dimension size;
	private int startx;
	private int starty;
	private BufferedImage image;
	public JMainPanel() {
		super();
		setOpaque(false);
		startx = 60;
		starty = 60;
		if(size == null) {
			size = new Dimension(400, 400);
		}
		this.setBounds(startx, starty, size.width / 2, size.height / 2);
	}
	
	public JMainPanel(int xp, int yp, int width, int height) {
		super();
		setOpaque(false);
		startx = xp;
		starty = yp;
		size = new Dimension(width, height);
		this.setBounds(startx, starty, size.width, size.height);
	}
	
	/**
	 * 
	 */
	private void loadImage() {
		try {
			InputStream stream = this.getClass().getResourceAsStream("kitty.png");
			image = ImageIO.read(stream);
		} catch (IOException e) {
			System.out.println("Could not load the image resource...");
		}
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// do something here.....
		if(image == null) {
			loadImage();
		}
		g2.drawImage(image, startx, starty, size.width, size.height, null);
	}
	
	@Override
	public Dimension getPreferredSize() {
		return size;
	} 
	
	public Dimension getMinimumSize() {
		return size;
	}
	
}
```
**测试程序DemoUI类源代码：**


```java
package com.gloomyfish.layer.demo;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;

public class DemoUI extends JFrame implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2851755643049352576L;
	private JButton changeButton;
	private JButton exitButton;
	public DemoUI() {
		super("Layer Panel Demo - GloomyFish");
		getContentPane().setLayout(new BorderLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		initComponent();
	}
	private void initComponent() {
		JPanel btnPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		changeButton = new JButton("Change");
		exitButton = new JButton("Exit");
		btnPanel.add(changeButton);
		btnPanel.add(exitButton);
		changeButton.addActionListener(this);
		exitButton.addActionListener(this);
		
		// create layer panels
		JLayeredPane layerPanel = new JLayeredPane();
		layerPanel.setPreferredSize(new Dimension(400,400));
		
		// background layer
		JBackGroundPanel bgPanel = new JBackGroundPanel(0,0,400,400);
		layerPanel.add(bgPanel, new Integer(2));
		
		JMainPanel imagePanel = new JMainPanel(0,0,400,400);
		layerPanel.add(imagePanel, new Integer(1));
		
		// add to main UI
		getContentPane().add(btnPanel, BorderLayout.SOUTH);
		getContentPane().add(layerPanel, BorderLayout.CENTER);
		
		this.pack();
		this.setVisible(true);
	}
	
	public static void main(String[] args) {
		new DemoUI();
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("Change")) {
			
		} else if(e.getActionCommand().equals("Exit")) {
			this.setVisible(false);
			this.dispose();
		}
	}

}
```





