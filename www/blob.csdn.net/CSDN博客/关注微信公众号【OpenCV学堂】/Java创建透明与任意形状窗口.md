# Java创建透明与任意形状窗口 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年08月05日 17:16:01[gloomyfish](https://me.csdn.net/jia20003)阅读数：20388








Java创建透明与任意形状窗口

一： Java 2D图形基础知识

自从Java Swing发布以来，Java的图形能力得到了很大的提升，JDK6的高级版本开始支持创

建自定义形状的JFrame，结合Java 2D图形的相关API，可以实现任意形状JFrame的创建。当

前JAVA 2提供可以创建的基本形状如下：



当前Java提供的对2D图形的主要操作有裁剪和路径覆盖，适当的运用Java 2D与Swing的其

他相关API，可以创建出任意形状的JFrame。

![](http://hi.csdn.net/attachment/201108/5/0_1312535616b6sz.gif)

二：API支持透明和自定义形状

从JDK6的release 10开始支持设置透明和自定义形状，支持的对象有Swing的JFrame, JDialog, 

AWT的Frame.以及所以继承java.awt.Window的子类。



Windows的透明效果又分为两类，简单的透明效果和基于像素位的透明效果。简单透明效果使

用alpha来设置所有的象素位，值越小越透明，最小值是完全透明，最大值代表完全不透明

（opaque）windows, 基于像素为的透明效果需要通过以下API来启用：
AWTUtilities.setWindowOpaque (frame, false);


对于透明度和形状的设定都是通过Java的反射机制完成调用，在JDK的官方网站有一个很好的Demo里面有个AWTUtilitiesWrappe类是一个很好的示例.



三：一个可以改变自身形状与透明效果的JFrame

程序的主要功能是选择你想要的形状，然后点击【OK】按钮，应用程序的窗口随之改变

点击【Cancel】按钮将推出程序，移动鼠标到窗口上面，可以拖动窗口程序。



涉及到Swing组件有JFrame, JPanel, JButton, JComboBox

组件的位置放置使用了绝对定位的方式，需要调用setLayout(null)来声明绝对定位方式

程序启动的运行效果如下：

![](http://hi.csdn.net/attachment/201108/5/0_13125356419te9.gif)

选择 Circle并点击【OK】按钮以后的效果如下：

![](http://hi.csdn.net/attachment/201108/5/0_13125356644Z53.gif)


选择 Area并点击【OK】按钮以后的效果如下：

![](http://hi.csdn.net/attachment/201108/5/0_13125356846jr2.gif)


主要的代码为JCustomFrame



```java
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Shape;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.geom.Area;
import java.awt.geom.Ellipse2D;
import java.awt.geom.RoundRectangle2D;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

public class JCustomFrame extends JFrame {
	
	/**
	 * gloomy fish
	 */
	private static final long serialVersionUID = -523336873755438297L;
    private Shape shape;
    private float alpha = 1f;
    private Dimension arcSize = new Dimension(50, 50);
    protected static final int CIRCLE_TYPE = 1;
    protected static final int RECTANGEL_TYPE = 0;
    protected static final int AREA_TYPE = 2;
    
	public JCustomFrame() {
        setUndecorated(true);
        setVisible(true);
        setListenersForEffects();
	}
	
	public JCustomFrame(int width, int height) {
        this();
        setSize(width, height);
    }

    public JCustomFrame(Shape shape, int width, int height) {
        this(width, height);
        setShape(shape);
    }

    public void setShape(Shape shape) {
		this.shape = shape;
	}

	public JCustomFrame(float alpha, Shape shape, int width, int height) {
        this(shape, width, height);
        setAlpha(alpha);
    } 
	
	public void setAlpha(float alpha) {
		this.alpha = alpha;
	}

	private void setListenersForEffects() {
        //It is important to upadate visual effect on form resize.
        addComponentListener(new ComponentAdapter() {

            @Override
            public void componentResized(ComponentEvent evt) {
                updateFrameEffects();
            }
        });
    }

    /**
     * This updates visual effects like SHAPE form and transparency. You have to
     * update also <b>shape</b> property or it paints old shape ( if you resize
     * frame without resize shape .. )
     */
    public void updateFrameEffects() {
        updateShape();
        try {
            AWTUtilitiesWrapper.setWindowShape(this, shape);
            if (shape != null) {
                AWTUtilitiesWrapper.setWindowOpacity(this, alpha);
            }
        } catch (Exception ex) {
            Logger.getLogger(JCustomFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
	public void updateShape() {
		if(shape == null) {
			shape = new RoundRectangle2D.Double(0d, 0d, getWidth(), getHeight(), arcSize.width, arcSize.height);
		}
		
	}
	
	public void updateShape(int type) {
		if(type == RECTANGEL_TYPE) {
			shape = new RoundRectangle2D.Double(0d, 0d, getWidth(), getHeight(), arcSize.width, arcSize.height);
		} else if(type == CIRCLE_TYPE) {
			shape = new Ellipse2D.Double(0, 0,400, 400);
		} else if(type == AREA_TYPE) {		
			Shape circle1 = new Ellipse2D.Double(0, 0,400, 400);
			Shape circle2 = new Ellipse2D.Double(200, 100,400, 400);
			Area area1 = new Area(circle1);
			Area area2 = new Area(circle2);
			area1.subtract(area2);
			shape = area1;
		}
	}
	
	public void center() {
        Toolkit tk = Toolkit.getDefaultToolkit();
        Dimension screenSize = tk.getScreenSize();
        int screenHeight = screenSize.height;
        int screenWidth = screenSize.width;
        this.setLocation((screenWidth - this.getWidth()) / 2, (screenHeight - this.getHeight()) / 2);
    }
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {

            public void run() {
                try {
                	// com.sun.java.swing.plaf.windows.WindowsLookAndFeel
                	UIManager.setLookAndFeel("com.sun.java.swing.plaf.motif.MotifLookAndFeel");
                } catch (Exception e) {
                }

                /*These are simple custom panel generated with vidual editor of Netbeans
                  don't care about it, take a look only to ImagePanel inherit ( why?...)
                 */
                
                // Cool transparent Frame
                final JCustomFrame customFrame = new JCustomFrame();
                customFrame.setLayout(new BorderLayout());
                
                // create custom JPanel
                final JImagePanel panel = new JImagePanel();
                java.net.URL image1 = this.getClass().getResource("ball.jpg");
                try {
					panel.setImage(ImageIO.read(image1));
					panel.addActionListener(new ActionListener() {
						
						@Override
						public void actionPerformed(ActionEvent e) {
							if(e.getActionCommand().equals("OK")) {
								System.out.println("Transfer now......");
								customFrame.updateShape(panel.getSelectedIndex());
								if(panel.getSelectedIndex() == CIRCLE_TYPE) {
									customFrame.setSize(400, 400);
								} else if(panel.getSelectedIndex() == AREA_TYPE) {
									customFrame.setSize(400, 399); // force layout Manager re-computation
								} else {
									customFrame.setSize(400, 300);
								}
							} else if(e.getActionCommand().equals("Cancel")) {
								System.out.println("System Exit......");
								customFrame.setVisible(false);
								customFrame.dispose();
								System.exit(0);
							}
						}
					});
				} catch (IOException e) {
					e.printStackTrace();
				}

				DragBarHandler dragHandle = new DragBarHandler(customFrame);
                customFrame.add(panel, BorderLayout.CENTER);
                customFrame.add(dragHandle, BorderLayout.NORTH);
                customFrame.setTitle("Ttranslucency JFrame");
                customFrame.setSize(400, 300);
                customFrame.setAlpha(0.8f);
                customFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                customFrame.center();

            }
        });

	}

}
```
如果想要全部的源代码，留下E-Mail






