# 图像处理之基于NCC模板匹配识别 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年10月02日 00:48:06[gloomyfish](https://me.csdn.net/jia20003)阅读数：18754标签：[java																[图像处理																[算法																[模板匹配](https://so.csdn.net/so/search/s.do?q=模板匹配&t=blog)
个人分类：[图像处理](https://blog.csdn.net/jia20003/article/category/879324)





**一：基本原理**

NCC是一种基于统计学计算两组样本数据相关性的算法，其取值范围为[-1, 1]之间，而对图像来说，每个像素点都可以看出是RGB数值，这样整幅图像就可以看成是一个样本数据的集合，如果它有一个子集与另外一个样本数据相互匹配则它的ncc值为1，表示相关性很高，如果是-1则表示完全不相关，基于这个原理，实现图像基于模板匹配识别算法，其中第一步就是要归一化数据，数学公式如下：

![](https://img-blog.csdn.net/20151002003931895)




**二：实现步骤**

（1）      获取模板像素并计算均值与标准方差、像素与均值diff数据样本

（2）      根据模板大小，在目标图像上从左到右，从上到下移动窗口，计

算每移动一个像素之后窗口内像素与模板像素的ncc值，与阈值比较，大于

阈值则记录位置

（3）      根据得到位置信息，使用红色矩形标记出模板匹配识别结果。

（4）      UI显示结果



**三：编程实现**

基于JAVA语言完成了整个算法编程实现与演示，其中第一步的代码如下：



```java
int tw = template.getWidth();
		int th = template.getHeight();
		int[] tpixels = new int[tw * th];
		getRGB(template, 0, 0, tw, th, tpixels);
		for(int i=0; i<tpixels.length; i++)
		{
			tpixels[i] = (tpixels[i] >> 16) & 0xff;
		}
		double[] meansdev = getPixelsMeansAndDev(tpixels);
		double[] tDiff = calculateDiff(tpixels, meansdev[0]);
		int raidus_width = tw / 2;
		int raidus_height = th / 2;
```
第二步的实现代码如下：




```java
int[] windowPixels = new int[tw * th];
		Arrays.fill(windowPixels, 0);
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				// calculate the means and dev for each window
				if(row <  raidus_height || (row + raidus_height) >= height)
					continue;
				if(col < raidus_width || (col + raidus_width) >= width) 
					continue;
				int wrow = 0;
				Arrays.fill(windowPixels, 0);
				for(int subrow = -raidus_height; subrow <= raidus_height; subrow++ )
				{
					int wcol = 0;
					for(int subcol = -raidus_width; subcol <= raidus_width; subcol++ )
					{
						if(wrow >= th || wcol >= tw)
						{
							continue;
						}
						windowPixels[wrow * tw + wcol] = getPixelValue(width, col + subcol, row + subrow, inPixels);
						wcol++;
					}
					wrow++;
				}
				// calculate the ncc
				double[] _meansDev = getPixelsMeansAndDev(windowPixels);
				double[] diff = calculateDiff(windowPixels, _meansDev[0]);
				double ncc = calculateNcc(tDiff, diff, _meansDev[1], meansdev[1]);
				if(ncc > threhold) {
					Point mpoint = new Point();
					mpoint.x = col;
					mpoint.y  = row;
					points.add(mpoint);
				}
			}
		}
```
第三步的实现代码如下：




```java
// draw matched template on target image according position
		setRGB( dest, 0, 0, width, height, inPixels );
		Graphics2D g2d = dest.createGraphics();
		g2d.setPaint(Color.RED);
		g2d.setStroke(new BasicStroke(4));
		for(Point p : points)
		{
			g2d.drawRect(p.x - raidus_width, p.y - raidus_height, tw, th);
		}
```
其中第二步用到的计算NCC的方法实现如下：




```java
private double calculateNcc(double[] tDiff, double[] diff, double dev1, double dev2) {
		// TODO Auto-generated method stub
		double sum = 0.0d;
		double count = diff.length;
		for(int i=0; i<diff.length; i++)
		{
			sum += ((tDiff[i] * diff[i])/(dev1 * dev2));
		}
		return (sum / count);
	}
```
UI部分完整源代码如下：




```java
package com.gloomyfish.image.templae.match;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class DemoUI extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private BufferedImage targetImage;
	private BufferedImage template;
	
	public DemoUI()
	{
		super();
		java.net.URL imageURL = this.getClass().getResource("words.png");
		java.net.URL templateURL = this.getClass().getResource("template.png");
		
		try {
			template = ImageIO.read(templateURL);
			targetImage = ImageIO.read(imageURL);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void setTarget(BufferedImage target) {
		this.targetImage = target;
	}

	@Override
	protected void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D) g;
		if(targetImage != null) {
			g2.drawImage(targetImage, 10, 10, targetImage.getWidth(), targetImage.getHeight(), null);
		}
		if(template != null) {
			g2.drawImage(template, 20+targetImage.getWidth(), 10, template.getWidth(), template.getHeight(), null);
		}
	}
	
	public static void main(String[] args) {
		JFrame f = new JFrame("模板匹配与识别");
		JButton okBtn = new JButton("匹配");
		final DemoUI ui = new DemoUI();
		okBtn.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				
				ui.process();
			}
		});
		
		JPanel btnPanel = new JPanel();
		btnPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		btnPanel.add(okBtn);
		
		f.getContentPane().add(btnPanel, BorderLayout.SOUTH);
		f.getContentPane().add(ui, BorderLayout.CENTER);
		f.setSize(500, 500);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setVisible(true);
	}

	protected void process() {
		NccTemplateMatchAlg algo = new NccTemplateMatchAlg(template);
		targetImage = algo.filter(targetImage, null);
		this.repaint();
	}

}
```
**四：程序运行效果如下**


![](https://img-blog.csdn.net/20151002004426914)
**其中左边是目标图像、右边为模板图像**

**PS:博客从10月份开始每月都有多篇相关图像处理文章更新**

**欢迎大家继续关注**](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




