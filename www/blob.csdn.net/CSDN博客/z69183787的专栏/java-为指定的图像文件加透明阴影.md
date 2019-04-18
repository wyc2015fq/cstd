# java-为指定的图像文件加透明阴影 - z69183787的专栏 - CSDN博客
2017年08月17日 17:56:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1580
```java
import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
public class AddShadow {
	/**
	 * @param srcFile
	 *            源图像文件
	 * @param x
	 *            阴影宽度
	 * @param y
	 *            阴影高度
	 * @param color
	 *            阴影颜色
	 * @param alpha
	 *            透明度，0－1之间取值
	 * @throws Exception
	 *             为指定的文件加上透明阴影
	 */
	public final static void add(File srcFile, int x, int y, Color color,
			float alpha) throws Exception {
		// 读取源文件
		BufferedImage srcImg = ImageIO.read(srcFile);
		// 在新建一个图像缓冲区，大小包括阴影部分
		BufferedImage bufImg = new BufferedImage(srcImg.getWidth() + x,
				srcImg.getHeight() + y, BufferedImage.TYPE_INT_RGB);
		Graphics2D g = bufImg.createGraphics();
		// 创建一个支持有透明度的图像缓冲区
		bufImg = g.getDeviceConfiguration().createCompatibleImage(
				srcImg.getWidth() + x, srcImg.getHeight() + y,
				Transparency.TRANSLUCENT);
		g.dispose();
		// 画阴影部分
		g = bufImg.createGraphics();
		g.setComposite(AlphaComposite.getInstance(AlphaComposite.XOR, alpha));
		g.setColor(Color.black);
		g.setBackground(Color.red);
		g.fillRect(x, y, srcImg.getWidth(), srcImg.getHeight());
		g.dispose();
		// 画源图像
		g = bufImg.createGraphics();
		g.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC, 1));
		g.drawImage(srcImg, 0, 0, null);
		g.dispose();
		String oldName = srcFile.getName();
		// 目标文件必须是PNG文件
		String fileName = "d:/ccc/"
				+ oldName.substring(0, oldName.length() - 4) + ".png";
		ImageIO.write(bufImg, "PNG", new File(fileName));
	}
	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		File f = new File("d:/ccc");
		File imgFiles[] = f.listFiles(new ImageFilter());
		for (File x : imgFiles) {
			System.out.println(x.getName() + "正在加阴影...");
			add(x, 20, 20, Color.red, 0.3f);
		}
		System.out.println("阴影添加成功，阴影文件扩展名为.png");
	}
}
```
