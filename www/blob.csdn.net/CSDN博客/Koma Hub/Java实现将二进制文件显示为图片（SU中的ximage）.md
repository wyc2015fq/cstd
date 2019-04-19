# Java实现将二进制文件显示为图片（SU中的ximage） - Koma Hub - CSDN博客
2018年01月30日 12:30:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：194
直接上代码：
```java
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JButton;
import java.util.Random;
import javax.swing.BorderFactory;
import java.io.*;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.GridBagLayout;
import java.awt.Toolkit;
import java.awt.Font;
import javax.swing.border.EmptyBorder;
import java.awt.Insets;
import javax.swing.Box;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BoxLayout;
import javax.swing.JTextField;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.GridBagConstraints;
import javax.swing.JPopupMenu;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import java.awt.FileDialog;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseWheelListener;
import java.awt.event.MouseWheelEvent;
import java.awt.RenderingHints;
import java.awt.BasicStroke;
import java.awt.Point;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.JOptionPane;
//a#################################
//a##
//a##  myXimageJPanel for myXimageJFrame
//a##
//a#################################
class myXimageJPanel extends JPanel {
        private int nx,xbeg,nz;
        private int width,height;
        private String filename;
        private float[] Itemp;
        private float[] Image;
        private float imax, imin;
        /*small paint*/
        private int minX ;
        private int minZ ;
        private int maxX ;
        private int maxZ ;
 
        myXimageJPanel(int nx,int xbeg, int nz,int width,int height, String filename
                        ,int minX, int minZ, int maxX, int maxZ){
                this.nx = nx; 
                this.xbeg = xbeg; 
                this.nz = nz;
                this.width = width;
                this.height = height;
                this.filename = filename;
                this.minX = minX;
                this.minZ = minZ;
                this.maxX = maxX;
                this.maxZ = maxZ;
                Itemp = new float[nx*nz];
                Image = new float[nx*nz];
                //setSize(width,height);
                setPreferredSize(new Dimension(width-20,height-20));
                readFile(xbeg);
                getMaxMin();
          }
        /* get min & max */
        public void getMaxMin(){
                imax = imin = Image[0];
                for(int i=0;i<nx*nz;i++){
                        if(imin>Image[i])imin=Image[i];
                        if(imax<Image[i])imax=Image[i];
                    }
          }
        /* read file */
        public void readFile(int nskip){
                DataInputStream fp = null;
                try{    
                        if(!new File(filename).exists()){  
                                System.out.println("The "+filename+" dont't exists");
                                return;  
                              } 
                        fp = new DataInputStream(new FileInputStream(new File(filename)));
                        int i = 0;
                        fp.skip(301*4*nskip);
                        while(fp.available()>0&&i<nx*nz){   
                                Itemp[i++] = fp.readFloat();  
                                Image[i-1]=swap(Itemp[i-1]);
                              } 
                }catch(Exception e){  
                        e.printStackTrace();  
                    }
          }
        /* swap */
        public static float swap (float value){
                int intValue = Float.floatToRawIntBits (value);
                intValue = swap (intValue);
                return Float.intBitsToFloat (intValue);
          }
        public static int swap (int value){
                int b1 = (value >>  0) & 0xff;
                int b2 = (value >>  8) & 0xff;
                int b3 = (value >> 16) & 0xff;
                int b4 = (value >> 24) & 0xff;
                return b1 << 24 | b2 << 16 | b3 << 8 | b4 << 0;
          }
        /* paint */
        public void paintComponent(Graphics g) {
                super.paintComponent(g);
                Graphics2D g2d = (Graphics2D) g;
                g2d.setColor(Color.blue);
                drawRuler(g2d);
                Dimension size = getSize();
                Insets insets = getInsets();
                int[] rgb = new int[3];
                for (int ix = minX; ix<maxX; ix ++)
                for (int iz = minZ; iz<maxZ; iz ++){
                        int i = ix*nz+iz;
                        /* gray */
                        rgb[0] = rgb[1] = rgb[2] = (int)( 1.0f*(255-(Image[i]-imin)*255.0f/(imax-imin)) );
                        /* red blue */
                        //rgb[0] = (int)( 1.0f*(255-(Image[i]-imin)*255.0f/(imax-imin)) );
                        //rgb[1] = (int)( Image[i]>=0?(255-Image[i]/imax*255.0f):(255-Image[i]/imin*255.0f) );
                        //rgb[2] = (int)( 1.0f*(255-(Image[i]-imin)*255.0f/(imax-imin)) );
                        int R = (int)( rgb[0] );
                        int G = (int)( rgb[1] );
                        int B = (int)( rgb[2] );
                        g2d.setColor(new Color(R, G, B) );
                        float radiox = (float)(getWidth()-20)/(float)(maxX-minX);
                        float radioz = (float)(getHeight()-20)/(float)(maxZ-minZ);
                        int drawx = (int)((float)(ix-minX)*radiox);
                        int drawz = (int)((float)(iz-minZ)*radioz);
                        for(int j=0;j<=(int)(radiox);j++)
                                for(int k=0;k<=(int)(radioz);k++)
                                        g2d.drawLine(drawx+j+20, drawz+k+20, drawx+j+20, drawz+k+20);
                    }
                //repaint();
          }
        /* ruler */
        public void drawRuler(Graphics2D g) {
                int height = getSize().height;
                int width = getSize().width;
                for (int iz=height; iz > 0; iz--) {
                        int pos = (iz-height);
                        if (pos % 50 == 0) {          
                                if(iz>=20)g.drawLine(0, iz, 20, iz);
                        }else if (pos % 10 == 0) {     
                                if(iz>=20)g.drawLine(10, iz, 20, iz);
                        }else if (pos % 2 == 0) {      
                                if(iz>=20)g.drawLine(15, iz, 20, iz);
                              }
                    }
                for (int ix=0; ix < width; ix++) {
                        if (ix % 50 == 0) {
                                if(ix>=20)g.drawLine(ix, 0, ix, 20);
                        }else if (ix % 10 == 0) {
                                if(ix>=20)g.drawLine(ix, 10, ix, 20);
                        }else if (ix % 2 == 0) {
                                if(ix>=20)g.drawLine(ix, 15, ix, 20);
                              }
                    }
          }
}
//a#################################
//a##
//a##  myXimageJFrame
//a##
//a#################################
class myXimageJFrame extends JFrame{
        private Toolkit toolkit;
        private myXimageJPanel myximageJPanel;
        private File FileSave;
        myXimageJFrame(int nx,int xbeg, int ny, String filename, String title
                        ,int minX, int minZ, int maxX, int maxZ){
                final FileDialog FileDialogSave;
                setTitle(filename);
                setSize(500, 600);
                setLocationRelativeTo(null);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2);
                
                /* menu: save, close */
                JMenuBar JMenuBarSave;
                JMenu JMenuSave;
                JMenuItem JMenuItemSaveClose,JMenuItemSaveSave;
                JMenuBarSave=new JMenuBar();
                JMenuSave=new JMenu("File");
                JMenuItemSaveSave=new JMenuItem("Save");
                JMenuItemSaveClose=new JMenuItem("Close");
                JMenuSave.add(JMenuItemSaveSave);
                JMenuSave.add(JMenuItemSaveClose);
                JMenuBarSave.add(JMenuSave);
                setJMenuBar(JMenuBarSave);
                JMenuItemSaveClose.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                dispose();
                              }
                    });
                FileDialogSave = new FileDialog(this, "Save", FileDialog.SAVE);
                JMenuItemSaveSave.addActionListener(new ActionListener() {
			
                        @Override
                        public void actionPerformed(ActionEvent e) {
                                if(FileSave==null){
                                        FileDialogSave.setVisible(true);
                                        String dirPath=FileDialogSave.getDirectory();
                                        String fileName=FileDialogSave.getFile();
                                        System.out.println("Dir: "+dirPath+fileName);
                                        if(dirPath==null && fileName==null){
                                                return;
                                                  }
 
                                        FileSave=new File(dirPath,fileName);
                                        }
                            }
                   });
                /* set Label Title */
                JLabel labelTitle = new JLabel(title,JLabel.CENTER);
                labelTitle.setFont(new Font("Georgia", Font.BOLD, 30));
                labelTitle.setForeground(new Color(50, 50, 25));
                add(labelTitle, BorderLayout.NORTH);
                /* ximage show panel in ximageJFrame */
                myximageJPanel = new myXimageJPanel(nx,xbeg,ny,getWidth()-40,getHeight()-40,filename
                                                        ,minX, minZ, maxX, maxZ);
                add(myximageJPanel);
                setVisible(true);
          }
}
//a#################################
//a##
//a##  myXimageParasJFrame for myXimageJFrame
//a##
//a#################################
class myXimageParasJFrame extends JFrame{
        private Toolkit toolkit;
        private String filename;
        private String title;
        private int xbeg;
        private int nx;
        private int nz;
        private int minX,minZ,maxX,maxZ;
        public myXimageParasJFrame() {
                setTitle("ToaImage");
                setSize(450, 300);
                //setMaximumSize(new Dimension(350,300));
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation(size.width/4 - getWidth()/2, (size.height -getHeight())/2);
/***************** set Label Title *****************/
                JLabel labelTitle = new JLabel("Toa_Image",JLabel.CENTER);
                labelTitle.setFont(new Font("Georgia", Font.BOLD, 20));
                labelTitle.setForeground(new Color(50, 50, 25));
                add(labelTitle, BorderLayout.NORTH);
/***************** parameter panel *****************/
                final JPanel parasPanel = new JPanel();
                parasPanel.setLayout(new GridBagLayout());
                getContentPane().add(parasPanel);
                /* title */
                JLabel labeltitle = new JLabel("Title: ");
                labeltitle.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField textfieldtitle = new JTextField(20);
                textfieldtitle.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldtitle.setText("change title...");
                JButton buttontitle = new JButton(">>>");
                buttontitle.setAlignmentX(1f);
                buttontitle.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("title: "+textfieldtitle.getText());
                              }
                    });
                GridBagConstraints gbc = makeGbc(0, 0);
                parasPanel.add(labeltitle,gbc);
                JPanel p0 =new JPanel();
                p0.add(textfieldtitle);
                p0.add(buttontitle);
                gbc = makeGbc(1, 0);
                parasPanel.add(p0, gbc);
                /* filename */
                JLabel labelfilename = new JLabel(" File: ");
                labelfilename.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField textfieldfilename = new JTextField(20);
                textfieldfilename.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldfilename.setText("select file...");
                filename = "select file...";
                JButton buttonfilename = new JButton("<<<");
                buttonfilename.setAlignmentX(1f);
                buttonfilename.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                JFileChooser fileopen = new JFileChooser();
                                FileFilter filterdat = new FileNameExtensionFilter("*.dat", "dat");
                                FileFilter filterbin = new FileNameExtensionFilter("*.bin", "bin");
                                FileFilter filterbinary = new FileNameExtensionFilter("*.binary", "binary");
                                fileopen.addChoosableFileFilter(filterdat);
                                fileopen.addChoosableFileFilter(filterbin);
                                fileopen.addChoosableFileFilter(filterbinary);
                                int ret = fileopen.showDialog(parasPanel, "Select");
                                if (ret == JFileChooser.APPROVE_OPTION) {
                                        File file = fileopen.getSelectedFile();
                                        textfieldfilename.setText(file.toString());
                                        filename=file.toString();
                                        System.out.println("FileName: "+filename);
                                        }
                              }
                    });
                gbc = makeGbc(0, 1);
                parasPanel.add(labelfilename,gbc);
                JPanel p1 =new JPanel();
                p1.add(textfieldfilename);
                p1.add(buttonfilename);
                gbc = makeGbc(1, 1);
                parasPanel.add(p1, gbc);
                /* xbeg */
                JLabel labelxbeg = new JLabel(" xbeg: ");
                labelxbeg.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField textfieldxbeg = new JTextField(20);
                textfieldxbeg.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldxbeg.setText("0");
                JButton buttonxbeg = new JButton(">>>");
                buttonxbeg.setAlignmentX(1f);
                buttonxbeg.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("xbeg: "+textfieldxbeg.getText());
                              }
                    });
                gbc = makeGbc(0, 2);
                parasPanel.add(labelxbeg,gbc);
                JPanel p2 =new JPanel();
                p2.add(textfieldxbeg);
                p2.add(buttonxbeg);
                gbc = makeGbc(1, 2);
                parasPanel.add(p2, gbc);
                /* for -> minX. minZ, maxX, maxZ  */
                final JTextField textfieldminX = new JTextField(9);
                final JTextField textfieldminZ = new JTextField(9);
                final JTextField textfieldmaxX = new JTextField(9);
                final JTextField textfieldmaxZ = new JTextField(9);
                /* nx */
                JLabel labelnx = new JLabel("   nx: ");
                labelnx.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField textfieldnx = new JTextField(20);
                textfieldnx.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldnx.setText("200");
                textfieldnx.getDocument().addDocumentListener(new DocumentListener() {
                        @Override
                        public void insertUpdate(DocumentEvent de) {
                                textfieldmaxX.setText(textfieldnx.getText());
                                }
                        @Override
                        public void removeUpdate(DocumentEvent de) {
                                textfieldmaxX.setText(textfieldnx.getText());
                                }
                        @Override
                        public void changedUpdate(DocumentEvent de) {
                                //Plain text components don't fire these events.
                                }
                    });
                JButton buttonnx = new JButton(">>>");
                buttonnx.setAlignmentX(1f);
                buttonnx.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("nx: "+textfieldnx.getText());
                              }
                    });
                gbc = makeGbc(0, 3);
                parasPanel.add(labelnx,gbc);
                JPanel p3 =new JPanel();
                p3.add(textfieldnx);
                p3.add(buttonnx);
                gbc = makeGbc(1, 3);
                parasPanel.add(p3, gbc);
                /* nz */
                JLabel labelnz = new JLabel("   nz: ");
                labelnz.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField textfieldnz = new JTextField(20);
                textfieldnz.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldnz.setText("301");
                textfieldnz.getDocument().addDocumentListener(new DocumentListener() {
                        @Override
                        public void insertUpdate(DocumentEvent de) {
                                textfieldmaxZ.setText(textfieldnz.getText());
                                }
                        @Override
                        public void removeUpdate(DocumentEvent de) {
                                textfieldmaxZ.setText(textfieldnz.getText());
                                }
                        @Override
                        public void changedUpdate(DocumentEvent de) {
                                //Plain text components don't fire these events.
                                }
                    });
                JButton buttonnz = new JButton(">>>");
                buttonnz.setAlignmentX(1f);
                buttonnz.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("nz: "+textfieldnz.getText());
                              }
                    });
                gbc = makeGbc(0, 4);
                parasPanel.add(labelnz,gbc);
                JPanel p4 =new JPanel();
                p4.add(textfieldnz);
                p4.add(buttonnz);
                gbc = makeGbc(1, 4);
                parasPanel.add(p4, gbc);
                /* minX. minZ */
                JLabel labelXZ0 = new JLabel("beg:(x,z)");
                labelXZ0.setFont(new Font("Georgia", Font.BOLD, 15));
                /*minX*/
                textfieldminX.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldminX.setText("0");
                /*minZ*/
                textfieldminZ.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldminZ.setText("0");
                JButton buttonXZ0 = new JButton(">>>");
                buttonXZ0.setAlignmentX(1f);
                buttonXZ0.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("minX: "+textfieldminX.getText()
                                                  +", minZ: "+textfieldminZ.getText());
                              }
                    });
                gbc = makeGbc(0, 5);
                parasPanel.add(labelXZ0,gbc);
                JPanel p5 =new JPanel();
                p5.add(textfieldminX);
                p5.add(textfieldminZ);
                p5.add(buttonXZ0);
                gbc = makeGbc(1, 5);
                parasPanel.add(p5, gbc);
                /*  maxX, maxZ */
                JLabel labelXZ1 = new JLabel("end:(x,z)");
                labelXZ1.setFont(new Font("Georgia", Font.BOLD, 15));
                /*maxX*/
                textfieldmaxX.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldmaxX.setText(textfieldnx.getText());
                /*maxZ*/
                textfieldmaxZ.setFont(new Font("Georgia", Font.PLAIN, 15));
                textfieldmaxZ.setText(textfieldnz.getText());
                JButton buttonXZ1 = new JButton(">>>");
                buttonXZ1.setAlignmentX(1f);
                buttonXZ1.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.out.println("maxX: "+textfieldmaxX.getText()
                                                  +", maxZ: "+textfieldmaxZ.getText());
                              }
                    });
                gbc = makeGbc(0, 6);
                parasPanel.add(labelXZ1,gbc);
                JPanel p6 =new JPanel();
                p6.add(textfieldmaxX);
                p6.add(textfieldmaxZ);
                p6.add(buttonXZ1);
                gbc = makeGbc(1, 6);
                parasPanel.add(p6, gbc);
/***************** panel for two buttons *****************/
                final JPanel twobuttonPanel = new JPanel();
                twobuttonPanel.setAlignmentX(1f);
                twobuttonPanel.setLayout(new BoxLayout(twobuttonPanel, BoxLayout.X_AXIS));
                twobuttonPanel.setBorder(new EmptyBorder(new Insets(10, 10, 10, 10)));
                /* OK run button */
                JButton ok = new JButton(" OK  ");
                ok.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                xbeg = Integer.parseInt(textfieldxbeg.getText());
                                nx = Integer.parseInt(textfieldnx.getText()); 
                                nz = Integer.parseInt(textfieldnz.getText());
                                minX = Integer.parseInt(textfieldminX.getText());
                                minZ = Integer.parseInt(textfieldminZ.getText());
                                maxX = Integer.parseInt(textfieldmaxX.getText());
                                maxZ = Integer.parseInt(textfieldmaxZ.getText());
                                title = textfieldtitle.getText();
                                if(title.equals("change title...")){
                                        JOptionPane.showMessageDialog(twobuttonPanel, "Please Input Title.",
                                                                        "Warning", JOptionPane.WARNING_MESSAGE);
                                        title = " ";
                                }if(filename.equals("select file...")){
                                        JOptionPane.showMessageDialog(twobuttonPanel, "Please Select File.",
                                                                        "Error", JOptionPane.ERROR_MESSAGE);
                                }else if(minX<0||minZ<0||maxX>nx||maxZ>nz){
                                        JOptionPane.showMessageDialog(twobuttonPanel, "Dimension Beyond model Range.",
                                                                        "Error", JOptionPane.ERROR_MESSAGE);
                                }else
                                        new myXimageJFrame(nx,xbeg,nz,filename,title,minX, minZ, maxX, maxZ);
                              }
                    });
                /* close button */
                JButton close = new JButton("Close");
                close.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                dispose();
                              }
                    });
                twobuttonPanel.add(Box.createRigidArea(new Dimension(getWidth()/2, 0)));
                twobuttonPanel.add(ok);
                twobuttonPanel.add(Box.createRigidArea(new Dimension(10, 0)));
                twobuttonPanel.add(close);
                twobuttonPanel.add(Box.createRigidArea(new Dimension(10, 0)));
                add(twobuttonPanel, BorderLayout.SOUTH);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                setVisible(true);
                setResizable(false);
          }
        private GridBagConstraints makeGbc(int ix, int iz) {
/*Copyright:https://stackoverflow.com/questions/4699892/how-to-set-the-component-size-with-gridlayout-is-there-a-better-way*/
                GridBagConstraints gbc = new GridBagConstraints();
                gbc.gridwidth = 1;
                gbc.gridheight = 1;
                gbc.gridx = ix;
                gbc.gridy = iz;
                gbc.weightx = ix;
                gbc.weighty = 1.0;
                gbc.insets = new Insets(5, 5, 5, 5);
                gbc.anchor = (ix == 0) ? GridBagConstraints.LINE_START : GridBagConstraints.LINE_END;
                gbc.fill = GridBagConstraints.HORIZONTAL;
                return gbc;
          }
        public static void main(String[] args) {
                myXimageParasJFrame myXimageParasJFrame = new myXimageParasJFrame();
          }
}
```
编译与运行：
```
[Toa@AMAX swingSeismic-tmp]$ javac Demo_47_ximage_09_parasWarning.java
[Toa@AMAX swingSeismic-tmp]$ java myXimageParasJFrame
```
主界面：
![](https://img-blog.csdn.net/20180130122923815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
更改beg和end参数可以显示部分内容：
![](https://img-blog.csdn.net/20180130122955545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同时，你可以把其保存为图片：
![](https://img-blog.csdn.net/20180130123021442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
