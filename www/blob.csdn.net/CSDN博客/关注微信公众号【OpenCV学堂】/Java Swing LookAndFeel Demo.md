# Java Swing LookAndFeel Demo - 关注微信公众号【OpenCV学堂】 - CSDN博客





2010年12月16日 16:30:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：3557








不说什么了，直接贴代码吧！呀的也太直接了点这博客写的



import java.awt.BorderLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.UIManager.LookAndFeelInfo;
public class GridBagLayoutDemoPanel extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 566006540006934384L;
	private JTextField mTxtFieldA;
	private JTextField mTxtFieldB;
	private JTextField mTxtFieldC;
	private JButton mButtonA;
	private JButton mButtonB;
	private JButton mButtonC;
	private JButton mButtonD;
	private JButton mButtonE;
	public GridBagLayoutDemoPanel() {
		super(new GridBagLayout(), true);
		initComponents();
		setUpListeners();
	}
	private void initComponents() {
		// text field
		mTxtFieldA = new JTextField();
		mTxtFieldB = new JTextField();
		mTxtFieldC = new JTextField();
		// Button
		mButtonA = new JButton("Button A");
		mButtonB = new JButton("Button B");
		mButtonC = new JButton("Button C");
		mButtonD = new JButton("Button D");
		mButtonE = new JButton("Button E");
		// align all the labels
		GridBagConstraints c = new GridBagConstraints();
		c.weightx = 0.5; // default is zero, all components will
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.CENTER;
		c.gridx = 0;
		c.gridy = 0;
		c.insets = new Insets(0, 0, 0, 0);
		add(mButtonA, c);
		c.gridx = 1;
		c.gridy = 0;
		add(mButtonB, c);
		c.gridx = 2;
		c.gridy = 0;
		add(mButtonC, c);
		c.gridx = 3;
		c.gridy = 0;
		add(mButtonD, c);
		c.gridx = 4;
		c.gridy = 0;
		add(mButtonE, c);
		// align the text field from right edge
		GridBagConstraints txtCons = new GridBagConstraints();
		txtCons.weightx = 0.5; // default is zero, all components will
		txtCons.fill = GridBagConstraints.HORIZONTAL;
		txtCons.anchor = GridBagConstraints.PAGE_END;
		txtCons.weighty = 1.0; // request some extra vertical space, default
								// zero
		txtCons.gridx = 4;
		txtCons.gridy = 1;
		add(mTxtFieldA, txtCons);
		txtCons.gridx = 3;
		txtCons.gridy = 1;
		add(mTxtFieldB, txtCons);
		txtCons.gridx = 2;
		txtCons.gridy = 1;
		add(mTxtFieldC, txtCons);
		// align the label
	}
	private void setUpListeners() {
	}
	public static void main(String[] args) {
		// UIManager settings
		/*
		 * javax.swing.plaf.metal.MetalLookAndFeel : Metal
		 * com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel : Nimbus
		 * com.sun.java.swing.plaf.motif.MotifLookAndFeel : CDE/Motif
		 * com.sun.java.swing.plaf.windows.WindowsLookAndFeel : Windows
		 * com.sun.java.swing.plaf.windows.WindowsClassicLookAndFeel : Windows Classic
		*/
		try {
			LookAndFeelInfo[] mList = UIManager.getInstalledLookAndFeels();
			for(LookAndFeelInfo info: mList) {
				System.out.println(info.getClassName() + " : " + info.getName());
			}
			System.out.println("Java Default LookAndFeel is " + UIManager.getLookAndFeel().getName());
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsClassicLookAndFeel");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}
		
		// Create and set up the window.
		JFrame frame = new JFrame("GridBagLayout Demo");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BorderLayout());
		
		// custom icon iamge
		GridBagLayoutDemoPanel gridPanel = new GridBagLayoutDemoPanel();
		java.net.URL imageURL = gridPanel.getClass().getResource("gridbag.jpg");
		try {
			frame.setIconImage(ImageIO.read(imageURL));
		} catch (IOException e) {
			System.err.println("An error occured when loading the image icon...");
		}
		// Create and set up the content pane.
		JPanel subPanel = new JPanel();
		subPanel.setLayout(new BorderLayout());
		subPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		subPanel.add(gridPanel, BorderLayout.CENTER);
		frame.getContentPane().add(subPanel, BorderLayout.CENTER);
		// Display the window.
		frame.pack();
		frame.setVisible(true);
	}
}




