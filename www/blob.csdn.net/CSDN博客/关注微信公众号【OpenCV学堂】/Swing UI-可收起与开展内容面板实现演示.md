# Swing UI - 可收起与开展内容面板实现演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年09月30日 12:56:30[gloomyfish](https://me.csdn.net/jia20003)阅读数：8008








**基于JAVA Swing实现的自定义组件可折叠的JPanel组件**

基本思想：

可折叠面板，分为两个部分-头部面板与内容面板

头部面板– 显示标题，以及对应的icon图标，监听鼠标事件决定内容面板隐藏或者显示

内容面板– 普通的JPanel组件。

![](https://img-blog.csdn.net/20130930124542343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**实现：**

头部面板：



```java
package com.gloomyfish.ui.demo;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class HeaderPanel extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int height = 50;
	private Color bgColor;
	private boolean isShow;
	private String title;
	public void setShow(boolean isShow) {
		this.isShow = isShow;
	}
	
	public void setTitle(String title)
	{
		this.title = title;
	}

	@Override
	protected void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g.create();
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);
		BufferedImage panelImage = createPanelImage();
		g2d.drawImage(panelImage, null, 0, 0);
	}

	private BufferedImage createPanelImage() {
		BufferedImage panelImage = new BufferedImage(getWidth(), height, BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = panelImage.createGraphics();
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.setRenderingHint(java.awt.RenderingHints.KEY_TEXT_ANTIALIASING,java.awt.RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
		int width = getWidth();
		g2d.setPaint(bgColor);
        g2d.fillRect(0, 0, width, height);
        GradientPaint gradientPaint = new GradientPaint(0, height/2, Color.LIGHT_GRAY, 0, height, Color.DARK_GRAY);
        g2d.setPaint(gradientPaint);
        g2d.fillRect(0, height/2, width, height/2);
        java.net.URL imageURL = null;
        if(this.isShow)
        {
        	imageURL = this.getClass().getResource("arrow-up-icon.png");
        }
        else
        {
        	imageURL = this.getClass().getResource("arrow-down-icon.png");
        }
        java.net.URL titleIconURL = this.getClass().getResource("user.png"); //gallery_5.png
        g2d.setFont(new Font("Serif", Font.BOLD, 24));
        g2d.setPaint(Color.WHITE);
        g2d.drawString(this.title, width/2-40, height-5);
		try {
			g2d.fillArc(width - 42, this.height/2, this.height/2, this.height/2, 0, 360);
			g2d.drawImage(ImageIO.read(imageURL), null, width - 42, this.height/2+2);
			g2d.drawImage(ImageIO.read(titleIconURL), null, 20, 5);
		} catch (IOException e) {
			System.err.println("An error occured when loading the image icon...");
		}
		
		return panelImage;
	}

	public void setHeight(int height) {
		this.height = height;
	}

	public void setBgColor(Color bgColor) {
		this.bgColor = bgColor;
	}

	@Override
	public Dimension getPreferredSize() {
		return new Dimension(this.getWidth(), height);
	}

	@Override
	public Dimension getSize() {
		return new Dimension(this.getWidth(), height);
	}

	public HeaderPanel(Color bgColor) {
		this.bgColor = bgColor;
		this.isShow = true;
		
	}
	
	public HeaderPanel(Color bgColor, String title) {
		this(bgColor);
		this.title = title;
	}
	
	public static void main(String[] args)
	{
        JFrame mainFrame = new JFrame("UI Demo - Gloomyfish");
        mainFrame.getContentPane().setLayout(new BorderLayout());
        mainFrame.getContentPane().add(new HeaderPanel(Color.BLACK), BorderLayout.CENTER);
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        mainFrame.pack();  
        mainFrame.setVisible(true);  
	}
}
```

内容面板：





```java
package com.gloomyfish.ui.demo;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ContentPanel extends JPanel {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public ContentPanel(Color theme)
	{
		this.setBorder(BorderFactory.createLineBorder(theme, 5));
	}
	
	public void createContent()
	{
		JPanel userPanel = new JPanel(new GridLayout(2,2));
		userPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
		userPanel.add(new JLabel("用户名："));
		JTextField txtField = new JTextField("", 10);
		userPanel.add(txtField);
		userPanel.add(new JLabel("密码："));
		JTextField pwdField = new JTextField("", 10);
		userPanel.add(pwdField);
		JButton okeBtn = new JButton("OK");
		JButton cancelBtn = new JButton("Cancel");
		JPanel btnPanel = new JPanel();
		btnPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		btnPanel.add(okeBtn);
		btnPanel.add(cancelBtn);
		this.setLayout(new BorderLayout());
		this.add(userPanel, BorderLayout.CENTER);
		this.add(btnPanel, BorderLayout.SOUTH);
	}

}
```

可折叠面板：





```java
package com.gloomyfish.ui.demo;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class JExpandablePanel extends JPanel{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private HeaderPanel headerPanel;
	private ContentPanel contentPanel;
	
	public JExpandablePanel()
	{
		super();
		initComponents();
	}

	private void initComponents() {
		this.setLayout(new BorderLayout());
		Color theme = Color.BLACK;
		headerPanel = new HeaderPanel(theme, "欢迎登录");
		headerPanel.addMouseListener(new PanelAction());
		contentPanel = new ContentPanel(theme);
		contentPanel.createContent();
		this.add(headerPanel, BorderLayout.NORTH);
		this.add(contentPanel, BorderLayout.CENTER);
		setOpaque(false);
	}
	
	class PanelAction extends MouseAdapter
	{
	    public void mousePressed(MouseEvent e)  
	    {  
	    	HeaderPanel hp = (HeaderPanel)e.getSource();  
	    	if(contentPanel.isShowing())
	    	{
	    		contentPanel.setVisible(false);
	    		hp.setShow(false);
	    	}
	    	else 
	    	{
	    		contentPanel.setVisible(true);
	    		hp.setShow(true);
	    	}
	    	hp.getParent().validate();
	    	hp.getParent().repaint();
	    }  
	}
	
	
	public static void main(String[] args)
	{
        JFrame mainFrame = new JFrame("UI Demo - Gloomyfish");
        mainFrame.getContentPane().setLayout(new BorderLayout());
        JPanel myPanel = new JPanel();
        myPanel.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();  
        JPanel[] panels = new JPanel[4]; // 
        gbc.insets = new Insets(1,3,0,3);  
        gbc.weightx = 1.0;  
        gbc.fill = GridBagConstraints.HORIZONTAL;  
        gbc.gridwidth = GridBagConstraints.REMAINDER;  
        for(int j = 0; j < panels.length; j++)  
        {  
        	panels[j] = new JExpandablePanel();
        	myPanel.add(panels[j], gbc);  
        }  
        mainFrame.getContentPane().add(myPanel, BorderLayout.NORTH);
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
        mainFrame.pack();
        mainFrame.setVisible(true);  
	}

}
```

效果图：



![](https://img-blog.csdn.net/20130930125035265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**顶级技巧：**
为了保证内容面板展开与收起时候，UI自动pack与刷新。尽量不要
设置UI的固定大小。使用JExpandablePanel时候，推荐使用GridBagLayout



