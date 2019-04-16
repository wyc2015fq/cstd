# JAVA Swing自定义JScorllPanel - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年01月26日 15:30:50[gloomyfish](https://me.csdn.net/jia20003)阅读数：5042








JAVA Swing中JScrollPanle自定义带标尺的JScrollPanel

Swing中JScrollPanel的结构与组成如下图：

![](https://img-blog.csdn.net/20140126151809234)


分为四个角与四个边，中间蓝色矩形为View窗口，可以通过调用JScrollPanel的

getViewport().setView(view)完成对现实内容的设置。

最常见的自定义JScrollPanel是改写默认的Column  Header View与Row Header View

下面是我做的一个关于瓷砖排列显示的UI，实现了一个标尺化的JScrollPanel，可以很好

的度量与说明空间大小与瓷砖需求的关系，实现效果如下：

![](https://img-blog.csdn.net/20140126152439187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


自定义的列与行标尺View的代码如下：



```java
package com.gloomyfish.swing.custom.ui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JComponent;

public class RuleView extends JComponent {
    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int HORIZONTAL = 0;
    public static final int VERTICAL = 1;
    private int SIZE = 600;
    public int orientation;
    public boolean isMetric;
    private double increment;
    private double units;
    private int width;
    private int height;
    
    public RuleView(int o, boolean m) {
        orientation = o;
        isMetric = m;
        setIncrementAndUnits();
    	if(orientation == HORIZONTAL)
    	{
    		width = SIZE;
    	}
    	else
    	{
    		height = SIZE;
    	}
    }

	private void setIncrementAndUnits() {
		
	}
	
    public void setPreferredHeight(int ph) {
    	this.height = ph;
        setPreferredSize(new Dimension(SIZE, ph));
    }

    public void setPreferredWidth(int pw) {
    	this.width = pw;
        setPreferredSize(new Dimension(pw, SIZE));
    }

    protected void paintComponent(Graphics g) {
    	Graphics2D g2d = (Graphics2D) g;
    	System.out.println("width = " + width);
    	System.out.println("Height = " + height);
    	
    	// background color
    	g2d.setColor(new Color(102, 255, 255));
    	Rectangle2D rect2d = new Rectangle2D.Double(0, 0, width, height);
    	g2d.fill(rect2d);
    	
    	
    	int count = 0;
    	increment = 0;
    	
    	// unit and text symbol
    	g2d.setPaint(new Color(0,0,204));
    	if(orientation == HORIZONTAL)
    	{
    		units = width / 10.0;
    		for(int i=0; i<=units; i++)
    		{
    			double rowOff = i * 10.0;
    			Line2D line = new Line2D.Double(rowOff, height*0.7, rowOff, height);
    			g2d.draw(line);
    			if(count == 10)
    			{
    				count = 0;
    				increment += 1;
    				Line2D bigLine = new Line2D.Double(rowOff, height*0.4, rowOff, height);
        			g2d.draw(bigLine);
        			g2d.drawString(increment + "", (int)rowOff-3, (int)(height/3.0));
    			}
    			count++;
    		}
    	}
    	else
    	{
    		units = height / 10.0;
    		for(int i=0; i<=units; i++)
    		{
    			double colOff = i * 10.0;
    			Line2D line = new Line2D.Double(width*0.7, colOff, width, colOff);
    			g2d.draw(line);
    			if(count == 10)
    			{
    				count = 0;
    				increment += 1;
    				Line2D bigLine = new Line2D.Double(width*0.5, colOff, width, colOff);
        			g2d.draw(bigLine);
        			g2d.drawString(increment + "", 0, (int)colOff);
    			}
    			count++;
    		}
    	}
    	g2d.setPaint(Color.RED);
    	
    }
}
```
自定义的JScrollPanel的代码如下：


```java
package com.gloomyfish.swing.custom.ui;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.LineBorder;

public class JCustomScrollPane extends JScrollPane {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private RuleView columnView;
	private RuleView rowView;
	private JLabel cornerLabel;
	public JCustomScrollPane(int rowSize, int columnSize)
	{
		super();
		JPanel buttonCorner = new JPanel(); // use FlowLayout
		cornerLabel = new JLabel("单位:米");
		cornerLabel.setForeground(Color.BLUE);
		cornerLabel.setFont(new Font("Serif", Font.PLAIN, 8));
		buttonCorner.setLayout(new FlowLayout(FlowLayout.LEFT));
		buttonCorner.add(cornerLabel);
		setCorner(JScrollPane.UPPER_LEFT_CORNER, buttonCorner);
		columnView = new RuleView(RuleView.HORIZONTAL, true);
		columnView.setPreferredHeight(30);
		rowView = new RuleView(RuleView.VERTICAL, true);
		rowView.setPreferredWidth(30);
		setColumnHeaderView(columnView);
		setRowHeaderView(rowView);
		setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
	    setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
	    setViewportBorder(new LineBorder(Color.RED));
	}
	
	public void setImagePanel(JComponent view)
	{
		this.getViewport().setView(view);
	}
	
}
```
最后说一下，Swing 是非常灵活的UI组件，提供了非常多的自定义接口
怎么合理运用就看个人啦！



