# Java雷达扫描图 - youfangyuan - CSDN博客
置顶2016年01月16日 14:16:43[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：2841
上图，上代码
![](https://img-blog.csdn.net/20160116141609912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
package demo;
import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import javax.swing.JComponent;
import javax.swing.JFrame;
import blade.paint.FanShapedGradientPaint;
/**
 * 
 * @author Administrator
 * 
 */
public class RadarDemo extends JComponent {
	public static void main(String argv[]) {
		JFrame frame = new JFrame();
		frame.setContentPane(new RadarDemo());
		frame.setSize(800, 600);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		paintBackground(g);
		paintRadarRing(g);
		paintFanShapedGradient(g);
	}
	protected void paintBackground(Graphics g) {
		g.setColor(Color.black);
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
	}
	protected void paintRadarRing(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g.setColor(Color.green);
		Dimension d = this.getSize();
		double radius = Math.min(d.width, d.height) / 2;
		for (int r = 1; r <= 5; r++) {
			double R = r * radius / 5;
			double x = d.width / 2 - R;
			double y = d.height / 2 - R;
			Ellipse2D ellipse = new Ellipse2D.Double(x, y, R * 2, R * 2);
			g2d.draw(ellipse);
		}
	}
	protected void paintFanShapedGradient(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		Composite oldComp = g2d.getComposite();
		AlphaComposite comp = AlphaComposite.getInstance(3, 0.5f);
		g2d.setComposite(comp);
		Dimension d = this.getSize();
		double radius = Math.min(d.width, d.height) / 2;
		double x = (d.width - radius * 2) / 2;
		double y = (d.height - radius * 2) / 2;
		FanShapedGradientPaint paint = new FanShapedGradientPaint(x, y, Math.PI / 2, Math.PI / 2, radius, Color.green,
				Color.black);
		g2d.setPaint(paint);
		Arc2D arc = new Arc2D.Double(x, y, radius * 2, radius * 2, 90, 90, Arc2D.PIE);
		g2d.fill(arc);
		g2d.setComposite(oldComp);
	}
}
```
