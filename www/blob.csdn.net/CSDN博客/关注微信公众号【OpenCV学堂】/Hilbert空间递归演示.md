# Hilbert空间递归演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年02月17日 23:14:54[gloomyfish](https://me.csdn.net/jia20003)阅读数：4199








Hilbert空间填充曲线在图像采样等方面十分有用关于什么希尔伯特

空间填充曲线看这里：http://en.wikipedia.org/wiki/Hilbert_curve

程序效果:

模拟Hilbert空间填充曲线效果，点击鼠标自动叠加！运行效果截图

![](https://img-blog.csdn.net/20140217231252375)


Hilbert源程序代码：



```java
package com.gloomyfish.image.hilbert;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class Hilbert {
	
	public static final int WHEELSIZE = 1024;
	
	// four edges
	public static final int NORTH = 0;
	public static final int EAST = 90;
	public static final int SOUTH = 180;
	public static final int WEST = 270;

	// four corners
	public static final int NE = 45;
	public static final int SE = 135;
	public static final int SW = 225;
	public static final int NW = 315;
	
	// attributes
	private Point location;
	private Color[] colorWheel;
	private int colorIdx;

	public Hilbert() {
		// build color lookup table
		this.colorWheel = new Color[1024];
		for (int i = 0; i < 128; ++i)
			this.colorWheel[i] = new Color(0, 255 - i, i);
		for (int j = 128; j < 256; ++j)
			this.colorWheel[j] = new Color(0, j, j);
		for (int k = 0; k < 256; ++k)
			this.colorWheel[(k + 256)] = new Color(0, 255 - k, 255);
		for (int l = 0; l < 128; ++l)
			this.colorWheel[(l + 512)] = new Color(0, l, 255 - l);
		for (int i1 = 0; i1 < 128; ++i1)
			this.colorWheel[(i1 + 640)] = new Color(0, 127 - i1, 127 - i1);
		for (int i2 = 0; i2 < 256; ++i2)
			this.colorWheel[(i2 + 768)] = new Color(0, i2, 0);
		this.colorIdx = 0;
	}

	public void process(Graphics graphic, int level, int width, int height) {
		this.location = null;
		if(level > 32 ) 
		{
			graphic.drawString("could get max depth is 32!", 40, 40);
			return;
		}
		hilbert(graphic, level, 0, 0, width, height, 0, 225);
	}

	public void hilbert(Graphics g, int depth, int startx, int starty, int width, int height, int startgray, int endgray) {
		int centerX = width / 2;
		int centerY = height / 2;
		if (depth == 0) {
			if (this.location != null) {
				g.setColor(this.colorWheel[this.colorIdx]);
				g.drawLine(this.location.x, this.location.y,
						startx + centerX, starty + centerY);
				if (++this.colorIdx >= 1024)
					this.colorIdx = 0;
			}
			this.location = new Point(startx + centerX, starty + centerY);
			return;
		}

		switch (startgray) {
		case 0:
			if (endgray == 225) {
				hilbert(g, depth - 1, startx, starty + centerY, centerX, centerY, 90, 225); // bottom-left
				hilbert(g, depth - 1, startx, starty, centerX, centerY, 0, 225); // upper-left
				hilbert(g, depth - 1, startx + centerX, starty, centerX, centerY, 0, 225); // upper-right
				hilbert(g, depth - 1, startx + centerX, starty + centerY, centerX, centerY, 270, 45); // bottom-right
				return;
			}

			if (endgray != 135)
				return;
			hilbert(g, depth - 1, startx + centerX, starty + centerY, centerX, centerY, 270, 135);
			hilbert(g, depth - 1, startx + centerX, starty, centerX, centerY, 0, 135);
			hilbert(g, depth - 1, startx, starty, centerX, centerY, 0, 135);
			hilbert(g, depth - 1, startx, starty + centerY, centerX, centerY, 90, 315);

			return;
		case 90:
			if (endgray == 315) {
				hilbert(g, depth - 1, startx, starty, centerX,
						centerY, 180, 315);
				hilbert(g, depth - 1, startx + centerX, starty,
						centerX, centerY, 90, 315);
				hilbert(g, depth - 1, startx + centerX, starty
						+ centerY, centerX, centerY, 90, 315);
				hilbert(g, depth - 1, startx, starty + centerY,
						centerX, centerY, 0, 135);

				return;
			}

			if (endgray != 225)
				return;
			hilbert(g, depth - 1, startx, starty + centerY, centerX,
					centerY, 0, 225);
			hilbert(g, depth - 1, startx + centerX, starty + centerY,
					centerX, centerY, 90, 225);
			hilbert(g, depth - 1, startx + centerX, starty, centerX,
					centerY, 90, 225);
			hilbert(g, depth - 1, startx, starty, centerX, centerY,
					180, 45);

			return;
		case 180:
			if (endgray == 45) {
				hilbert(g, depth - 1, startx + centerX, starty,
						centerX, centerY, 270, 45);
				hilbert(g, depth - 1, startx + centerX, starty
						+ centerY, centerX, centerY, 180, 45);
				hilbert(g, depth - 1, startx, starty + centerY,
						centerX, centerY, 180, 45);
				hilbert(g, depth - 1, startx, starty, centerX,
						centerY, 90, 225);

				return;
			}

			if (endgray != 315)
				return;
			hilbert(g, depth - 1, startx, starty, centerX, centerY,
					90, 315);
			hilbert(g, depth - 1, startx, starty + centerY, centerX,
					centerY, 180, 315);
			hilbert(g, depth - 1, startx + centerX, starty + centerY,
					centerX, centerY, 180, 315);
			hilbert(g, depth - 1, startx + centerX, starty, centerX,
					centerY, 270, 135);

			return;
		case 270:
			if (endgray == 45) {
				hilbert(g, depth - 1, startx + centerX, starty,
						centerX, centerY, 180, 45);
				hilbert(g, depth - 1, startx, starty, centerX,
						centerY, 270, 45);
				hilbert(g, depth - 1, startx, starty + centerY,
						centerX, centerY, 270, 45);
				hilbert(g, depth - 1, startx + centerX, starty
						+ centerY, centerX, centerY, 0, 225);

				return;
			}

			if (endgray != 135)
				return;
			hilbert(g, depth - 1, startx + centerX, starty + centerY,
					centerX, centerY, 0, 135);
			hilbert(g, depth - 1, startx, starty + centerY, centerX,
					centerY, 270, 135);
			hilbert(g, depth - 1, startx, starty, centerX, centerY,
					270, 135);
			hilbert(g, depth - 1, startx + centerX, starty, centerX,
					centerY, 180, 315);

			return;
		}
	}
}
```
UI部分代码：



```java
package com.gloomyfish.image.hilbert;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JComponent;
import javax.swing.JFrame;

public class HilbertUI extends JComponent implements MouseListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private int level;
	private Hilbert hilbert;
	private Dimension size;

	public HilbertUI() {
		this.level = 1;
		size = new Dimension(405, 405);
		this.hilbert = new Hilbert();
		addMouseListener(this);
	}

	@Override
	protected void paintComponent(Graphics g) {
		g.setColor(Color.BLACK);
		g.clearRect(0, 0, (int)size.getWidth(), (int)size.getHeight());
		this.hilbert.process(g, this.level, (int)size.getWidth()-5, (int)size.getHeight()-5);
	}

	@Override
	public Dimension getPreferredSize() {
		return size;
	}

	@Override
	public Dimension getMaximumSize() {
		return size;
	}

	@Override
	public Dimension getMinimumSize() {
		return size;
	}

	public void mousePressed(MouseEvent paramMouseEvent) {
	}

	public void mouseEntered(MouseEvent paramMouseEvent) {
	}

	public void mouseExited(MouseEvent paramMouseEvent) {
	}

	public void mouseReleased(MouseEvent paramMouseEvent) {
	}

	public void mouseClicked(MouseEvent event) {
		int i = event.getModifiers();
		if ((i & 0x10) > 0)
			this.level += 1;
		else if (((i & 0xC) > 0) && (this.level > 0))
			this.level -= 1;
		repaint();
	}
	
	public static void main(String[] args)
	{
        JFrame mainFrame = new JFrame("Hilbert Demo - Gloomyfish");
        mainFrame.getContentPane().setLayout(new BorderLayout());
        mainFrame.getContentPane().add(new HilbertUI(), BorderLayout.CENTER);
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        mainFrame.pack();  
        mainFrame.setVisible(true);  
	}
}
```




