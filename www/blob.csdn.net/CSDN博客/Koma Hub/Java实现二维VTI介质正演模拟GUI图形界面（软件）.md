# Java实现二维VTI介质正演模拟GUI图形界面（软件） - Koma Hub - CSDN博客
2017年09月16日 19:57:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：305
个人分类：[Java																[Geophysics																[Java AWT/Swing](https://blog.csdn.net/Rong_Toa/article/category/7157478)](https://blog.csdn.net/Rong_Toa/article/category/7221460)](https://blog.csdn.net/Rong_Toa/article/category/7156200)
```java
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JToolBar;
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
import javax.swing.JComboBox;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
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
import javax.swing.Timer;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JCheckBox;
//a#################################
//a##
//a##  myXmovieJFrame
//a##
//a#################################
class myXmovieJFrame extends JFrame{
        private Toolkit toolkit;
        private myXmovieJPanel myxmovieJPanel;
        private File FileSave;
        Timer timer;
        myXmovieJFrame(float velocity, float epsilon, float delta, 
                       int SX, int SZ, int NX, int NZ, float DX,float DZ,
                       int NT, float DT, float FM, int MM, int npd){
                final FileDialog FileDialogSave;
                setTitle("Toa_Xmovie");
                setSize(300, 300);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                addWindowListener(new WindowAdapter() {
                        public void windowClosing(WindowEvent windowEvent){
                                timer.stop();
                              }
                    });
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2);
                /* ximage show panel in ximageJFrame */
                myxmovieJPanel = new myXmovieJPanel(velocity,epsilon,delta,
                                                    SX, SZ, NX, NZ, DX,DZ,NT, DT, FM, MM,npd);
                add(myxmovieJPanel);
                setVisible(true);
          }
        //a#################################
        //a##
        //a##  myXmovieJPanel for myXmovieJFrame
        //a##
        //a#################################
        class myXmovieJPanel extends JPanel {
                private int NX,NZ;
                private int SX,SZ;
                private float DX,DZ;
                private int NT;
                private float DT;
                private float FM;
                private int MM;
                private float[] Image;
                private float imax, imin;
                private int npd;
                ActionListener go;
                int it = -1;
                myXmovieJPanel(float velocity, float epsilon, float delta,
                               int SX, int SZ, int NX, int NZ, float DX,float DZ,
                               int NT, float DT, float FM, int MM, int npd){
                        this.NX = NX;
                        this.NZ = NZ;
                        this.SX = SX;
                        this.SZ = SZ;
                        this.DX = DX;   
                        this.DZ = DZ;
                        this.NT = NT;
                        this.DT = DT;
                        this.FM = FM;
                        this.MM = MM;
                        this.npd = npd;
                        final int nx = NX; 
                        final int nz = NZ;
                        final int sx = SX; 
                        final int sz = SZ;
                        final float dx = DX;
                        final float dz = DZ;
                        final int nt = NT;
                        final float dt = DT;
                        final float fm = FM;
                        final int mm = MM;
                        Image = new float[nx*nz];
                        /* initial the wave(P0,P1,Q0,Q1) */
                        final Wave wave = new Wave(nx, nz);
                        wave.Init_Wave();
           
                        /* get the wavelet<Gaussian or Ricker> */
                        Wavelet wavelet = new Wavelet(nt,dt, fm); 
                        wavelet.getWavelet(2);/* 1:Ricker;2:Gaussian */
                        /* Model (int nx,int nz,float dx,float dz); */
                        vtiModel model0 = new vtiModel(nx,nz,dx,dz);
                        model0.InitModel(velocity, epsilon, delta);
                        model0.ReadModel("vel.dat","epsilon.dat","delta.dat");
                        final vtiFD vtifd = new vtiFD(model0, wave, wavelet);
                        go = new ActionListener() {
                                public void actionPerformed(ActionEvent actionEvent) {
                                        it++;
                                        Image = vtifd.FD(sx,sz,mm,it); 
                                        getMaxMin();
                                        repaint();
                                        if(it>=nt-1)timer.stop();
                                      }
                            };
                        timer = new Timer(10, go);
                        timer.start();
                    }
                /* get min & max */
                public void getMaxMin(){
                        imax = imin = Image[0];
                        for(int i=0;i<NX*NZ;i++){
                                if(imin>Image[i])imin=Image[i];
                                if(imax<Image[i])imax=Image[i];
                            }
                    }
                /* paint */
                public void paintComponent(Graphics g) {
                        super.paintComponent(g);
                        Graphics2D g2d = (Graphics2D) g;
                        g2d.setColor(Color.blue);
                        Dimension size = getSize();
                        Insets insets = getInsets();
                        int[] rgb = new int[3];
                        float radiox = (float)(getWidth())/(float)(NX);
                        float radioz = (float)(getHeight())/(float)(NZ);
                        for (int ix = 0; ix<NX; ix ++)
                        for (int iz = 0; iz<NZ; iz ++){
                                int i = ix*NZ+iz;
                                /* gray */
                                rgb[0] = rgb[1] = rgb[2] 
                                       = (int)(( 1.0f*(255-(Image[i]-imin)*255.0f/(imax-imin)) )%255);
                                int R = (int)( rgb[0] );
                                int G = (int)( rgb[1] );
                                int B = (int)( rgb[2] );
                                g2d.setColor(new Color(R, G, B) );
                                int drawx = (int)((float)(ix)*radiox);
                                int drawz = (int)((float)(iz)*radioz);
                                for(int j=0;j<=(int)(radiox);j++)
                                        for(int k=0;k<=(int)(radioz);k++)
                                                g2d.drawLine(drawx+j, drawz+k, drawx+j, drawz+k);
                            }
                        g2d.setColor(Color.RED);
                        g2d.drawLine((int)(npd*radiox),(int)(npd*radioz),
                                     (int)((NX-npd)*radiox),(int)(npd*radioz));
                        g2d.drawLine((int)(npd*radiox),(int)((NZ-npd)*radioz),
                                     (int)((NX-npd)*radiox),(int)((NZ-npd)*radioz));
                        g2d.drawLine((int)(npd*radiox),(int)(npd*radioz),
                                     (int)(npd*radiox),(int)((NZ-npd)*radioz));
                        g2d.drawLine((int)((NX-npd)*radiox),(int)(npd*radioz),
                                     (int)((NX-npd)*radiox),(int)((NZ-npd)*radioz));
                    }
                //a#################################
                //a##
                //a##  inner class Wavelet for fd
                //a##
                //a#################################
                class Wavelet{
                        private float pi = 3.141592653f;
                        public int nt;
                        public float dt, fm;
                        public float[] wlt;
                        public Wavelet(int nt, float dt, float fm){
                                this.nt = nt;
                                this.dt = dt;
                                this.fm = fm;
                        }//end
                        /* ricker wavelet */
                        public void getWavelet(int wtype){
                                wlt = new float[nt];
                                for(int i = 0; i<nt;i++)wlt[i]=0.0f;
                                int it;
                                float tdelay, t, ts, x, xx;
                                for(t = 0.0f, it = 0; it < nt ; t = (++it)*dt){
                                        tdelay = 1.0f/fm;
                                        ts = t - tdelay;
                                        if(wtype==1&&t <= 2*tdelay){
                                                x = fm * ts;
                                                xx = x * x;
                                                wlt[it] = (float)((1-2*pi*pi*xx)*Math.exp(-(pi*pi*xx)));
                                        }//end if
                                        if(wtype==2&&t <= 2*tdelay){
		                                x = (float)((-4.0f)*(fm*fm*pi*pi)/Math.log(0.1));
		                                wlt[it] = (float)((-2.0f)*(pi*pi*ts)*Math.exp(-x*ts*ts));
                                        }//end if
                                }//end for
                        }//end
                    }
                //a#################################
                //a##
                //a##  inner class Wave for fd
                //a##
                //a#################################
                class Wave{
                        public int nx,nz;
                        public float[] P0;
                        public float[] P1;
                        public float[] Q0;
                        public float[] Q1;
                        public Wave(int nx,int nz){
                                this.nx = nx;
                                this.nz = nz;
                            }
                        public void Init_Wave(){
                                P0 = new float[(nx)*(nz)];
                                P1 = new float[(nx)*(nz)];
                                Q0 = new float[(nx)*(nz)];
                                Q1 = new float[(nx)*(nz)];
                                for(int i = 0; i < (nx)*(nz); i++){
                                        P0[i] = 0.0f;
                                        P1[i] = 0.0f;
                                        Q0[i] = 0.0f;
                                        Q1[i] = 0.0f;
                                }//end
                        }//initial Wave
                    }
                //a#################################
                //a##
                //a##  inner class vtiFD 
                //a##
                //a#################################
                class vtiFD {
                        private float[] stencil = {-205.0f/72.0f,8.0f/5.0f,-1.0f/5.0f,8.0f/315.0f,-1.0f/560.0f};
                        private float pi = 3.141592653f;
                        vtiModel model;
                        Wave wave;
                        Wavelet wavelet;
                        public vtiFD(vtiModel model, Wave wave, Wavelet wavelet){
                                this.model = model;
                                this.wave = wave;
                                this.wavelet = wavelet;
                        }//end struct function
                        public float[] FD(int sx, int sz, int mm, int it){
                                float dt2x2 = wavelet.dt*wavelet.dt/(model.dx*model.dx);
                                float dt2z2 = wavelet.dt*wavelet.dt/(model.dz*model.dz);
                                int i, j, id, im;
                                float xx,zz,flag;
                                        if(it%50==0)System.out.println("Time: "+(it*wavelet.dt)+" (s)");
                                        addSource(wave, wavelet, sx, sz, it);
                                        for(i=mm; i<(model.nx-mm); i++) {
                                                for(j=mm;j<(model.nz-mm); j++) { 
                                                        id = i * model.nz + j;
                                                        xx = stencil[0] * wave.P1[id];
                                                        zz = stencil[0] * wave.Q1[id];
                                                        for(im=1;im<=mm;im++) {
                               xx += stencil[im]*(wave.P1[id+im * model.nz]+wave.P1[id-im * model.nz]);
                               zz += stencil[im]*(wave.Q1[id+im]+wave.Q1[id-im]);
                                                        }//end
                                                        xx *= dt2x2;
                                                        zz *= dt2z2;
                               wave.P0[id] =( 2.0f*wave.P1[id] - wave.P0[id] 
                                                        + xx*model.velocity[id]
                                                            *model.velocity[id]*(1.0f+2.0f*model.epsilon[id]) 
                                                        + zz*model.velocity[id]*model.velocity[id] );
                               wave.Q0[id] =( 2.0f*wave.Q1[id] - wave.Q0[id] 
                                                        + xx*model.velocity[id]
                                                            *model.velocity[id]*(1.0f+2.0f* model.delta [id]) 
                                                        + zz*model.velocity[id]*model.velocity[id] );
                                                }//end j
                                        }//end i
                                buffer(wave);
                                absorbBndr(wave,-0.25f);
                                return wave.P0;
                        }//end FD
                        public void buffer(Wave wave){
                                float tmp;                
                                for(int l =0 ;l<model.nx*model.nz; l++){
                                        tmp = wave.P1[l]; wave.P1[l] = wave.P0[l]; wave.P0[l] = tmp;
                                        tmp = wave.Q1[l]; wave.Q1[l] = wave.Q0[l]; wave.Q0[l] = tmp;
                                }//end for
                        }//end
                        public void addSource(Wave wave, Wavelet wavelet, int sx, int sz, int it){
                                wave.P0[(sx-1)*(wave.nz)+sz-1] += wavelet.wlt[it];
                                wave.Q0[(sx-1)*(wave.nz)+sz-1] += wavelet.wlt[it];
                        }//end
                        public void absorbBndr(Wave wave,float qp){
                                int ix,iz,id;
                                for(ix=0;ix<wave.nx;ix++){
                                        for(iz=0;iz<wave.nz;iz++){
                                                id = ix*(wave.nz) + iz;
                                                if(ix<npd){
                               wave.P0[id]=(float)( qp*Math.pow((npd-ix)/(1.0f*npd),2.0f) + 1.0f )*wave.P0[id];
                               wave.P1[id]=(float)( qp*Math.pow((npd-ix)/(1.0f*npd),2.0f) + 1.0f )*wave.P1[id];
                               wave.Q0[id]=(float)( qp*Math.pow((npd-ix)/(1.0f*npd),2.0f) + 1.0f )*wave.Q0[id];
                               wave.Q1[id]=(float)( qp*Math.pow((npd-ix)/(1.0f*npd),2.0f) + 1.0f )*wave.Q1[id];
                                                }else if(ix>=wave.nx-npd){
                               wave.P0[id]=(float)( qp*Math.pow((ix+npd-wave.nx)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.P0[id];
                               wave.P1[id]=(float)( qp*Math.pow((ix+npd-wave.nx)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.P1[id];
                               wave.Q0[id]=(float)( qp*Math.pow((ix+npd-wave.nx)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.Q0[id];
                               wave.Q1[id]=(float)( qp*Math.pow((ix+npd-wave.nx)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.Q1[id];
                                                }if(iz<npd){
                               wave.P0[id]=(float)( qp*Math.pow((npd-iz)/(1.0f*npd),2.0f) + 1.0f )*wave.P0[id];
                               wave.P1[id]=(float)( qp*Math.pow((npd-iz)/(1.0f*npd),2.0f) + 1.0f )*wave.P1[id];
                               wave.Q0[id]=(float)( qp*Math.pow((npd-iz)/(1.0f*npd),2.0f) + 1.0f )*wave.Q0[id];
                               wave.Q1[id]=(float)( qp*Math.pow((npd-iz)/(1.0f*npd),2.0f) + 1.0f )*wave.Q1[id];
                                                }else if(iz>=wave.nz-npd){
                               wave.P0[id]=(float)( qp*Math.pow((iz+npd-wave.nz)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.P0[id];
                               wave.P1[id]=(float)( qp*Math.pow((iz+npd-wave.nz)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.P1[id];
                               wave.Q0[id]=(float)( qp*Math.pow((iz+npd-wave.nz)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.Q0[id];
                               wave.Q1[id]=(float)( qp*Math.pow((iz+npd-wave.nz)
                                                        /(1.0f*npd),2.0f) + 1.0f )*wave.Q1[id];
                                                }//if
                                        }//iz
                                }//ix
                        }//end
                    }
                //a#################################
                //a##
                //a##  inner class vtiModel for fd
                //a##
                //a#################################
                /* vtiModel */
                class vtiModel{
                        public int nx;
                        public int nz;
                        public float dx;
                        public float dz;
                        public float[] velocity, epsilon, delta;
                        public vtiModel(int nx,int nz,float dx,float dz){
                                this.nx=nx;
                                this.nz=nz;
                                this.dx=dx;
                                this.dz=dz;
                        }//struct function ending
                        public void InitModel(float v, float e, float d){
                                velocity = new float[nx*nz];
                                 epsilon = new float[nx*nz];
                                   delta = new float[nx*nz];
                                for(int i = 0; i < nx*nz; i++) {
                                        velocity[i] = v;
                                         epsilon[i] = e;
                                           delta[i] = d;
                                    }
                        }//initial the vtiModel
                        public void ReadModel(String FileNameOfVelocity, 
                                              String FileNameOfEpsilon, 
                                              String FileNameOfDelta){
                                DataInputStream fpVelocity = null, fpEpsilon = null, fpDelta = null;
                                try{    
                                        if(!new File(FileNameOfVelocity).exists()){  
                                                System.out.println("The "+FileNameOfVelocity+" file dont't exists");
                                                return;  
                                              } 
                                        if(!new File(FileNameOfEpsilon).exists()){  
                                                System.out.println("The "+FileNameOfEpsilon+" file dont't exists");
                                                return;  
                                              } 
                                        if(!new File(FileNameOfDelta).exists()){  
                                                System.out.println("The "+FileNameOfDelta+" file dont't exists");
                                                return;  
                                              } 
                                        fpVelocity = new DataInputStream(
                                                        new FileInputStream(new File(FileNameOfVelocity)));
                                         fpEpsilon = new DataInputStream(
                                                        new FileInputStream(new File(FileNameOfEpsilon)));
                                           fpDelta = new DataInputStream(
                                                        new FileInputStream(new File(FileNameOfDelta))); 
                                        int i = 0, j =0 , k =0 ;
                                        while(fpVelocity.available()>0&&fpEpsilon.available()>0&&fpDelta.available()>0
                                                &&i<nx*nz&&j<nx*nz&&k<nx*nz){   
                                                velocity[i++] = fpVelocity.readFloat(); 
                                                 epsilon[j++] =  fpEpsilon.readFloat(); 
                                                   delta[k++] =    fpDelta.readFloat();  
                                                velocity[i-1] = swap(velocity[i-1]);
                                                 epsilon[j-1] = swap( epsilon[j-1]);
                                                   delta[k-1] = swap(   delta[k-1]);
                                              } 
                                }catch(Exception e){  
                                        e.printStackTrace();  
                                    }
                        }//read vtiModel
                        public float swap (float value){
                                int intValue = Float.floatToRawIntBits (value);
                                intValue = swap (intValue);
                                return Float.intBitsToFloat (intValue);
                              }
                        public int swap (int value){
                                int b1 = (value >>  0) & 0xff;
                                int b2 = (value >>  8) & 0xff;
                                int b3 = (value >> 16) & 0xff;
                                int b4 = (value >> 24) & 0xff;
                                return b1 << 24 | b2 << 16 | b3 << 8 | b4 << 0;
                            }
                    }
          }
}
//a#################################
//a##
//a##  myXmovieParasJFrame
//a##
//a#################################
class Demo extends JFrame{
//
        private Toolkit toolkit;
        private int NX,NZ,SX,SZ,NT,MM,npd;
        private float velocity,epsilon,delta,DX,DZ,DT,FM;
        private boolean flag_SelectFile = false;
        Demo(){
                setTitle("Toa VTI Snapshot Movie");
                setSize(500, 190);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2);
                /* menu: save, close */
                JMenuBar menubar = new JMenuBar();
                JMenu menu=new JMenu("File");;
                JMenuItem close=new JMenuItem("Close");
                menu.add(close);
                close.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                dispose();
                              }
                    });
                menubar.add(menu);
                setJMenuBar(menubar);
                /* include toolbars */
                JPanel panel = new JPanel();
                panel.setLayout(new GridLayout(4,1));
                /* toolbar0 */
                final JToolBar toolbar0 = new JToolBar();
                JCheckBox checkbox = new JCheckBox("Select File[Y/N]", false);
                checkbox.setFocusable(false);
                final JLabel selectYN = new JLabel(" > NO");
                checkbox.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                flag_SelectFile = !flag_SelectFile;
                                if (flag_SelectFile) {
                                        selectYN.setText(" > YES");
                                } else {
                                        selectYN.setText(" > NO");
                                        }
                              }
                    });
                toolbar0.add(checkbox);
                toolbar0.add(selectYN);
                /* toolbar1 */
                final JToolBar toolbar1 = new JToolBar();
                /*nx*/
                toolbar1.add(new JLabel(" nx:"));
                final JTextField JFnx = new JTextField(5);
                JFnx.setFont(new Font("Georgia", Font.BOLD, 15));
                JFnx.setText("200");
                final JTextField JFsx = new JTextField(5);
                JFsx.setFont(new Font("Georgia", Font.BOLD, 15));
                JFsx.setText("100");
                JFnx.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFnx.getText()))
                                        JOptionPane.showMessageDialog(JFnx, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                JFsx.setText( Integer.toString(Integer.parseInt( JFnx.getText() )/2) );
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFnx);
                /*nz*/
                toolbar1.add(new JLabel(" nz:"));
                final JTextField JFnz = new JTextField(5);
                JFnz.setFont(new Font("Georgia", Font.BOLD, 15));
                JFnz.setText("200");
                final JTextField JFsz = new JTextField(5);
                JFsz.setFont(new Font("Georgia", Font.BOLD, 15));
                JFsz.setText("100");
                JFnz.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFnz.getText()))
                                        JOptionPane.showMessageDialog(JFnz, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                JFsz.setText( Integer.toString(Integer.parseInt( JFnz.getText() )/2) );
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFnz);
                /*dx*/
                toolbar1.add(new JLabel(" dx[m]:"));
                final JTextField JFdx = new JTextField(5);
                JFdx.setFont(new Font("Georgia", Font.BOLD, 15));
                JFdx.setText("5");
                JFdx.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFdx.getText()))
                                        JOptionPane.showMessageDialog(JFdx, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFdx);
                /*dz*/
                toolbar1.add(new JLabel(" dz[m]:"));
                final JTextField JFdz = new JTextField(5);
                JFdz.setFont(new Font("Georgia", Font.BOLD, 15));
                JFdz.setText("5");
                JFdz.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFdz.getText()))
                                        JOptionPane.showMessageDialog(JFdz, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFdz);
                /*sx*/
                toolbar1.add(new JLabel(" sx:"));
                JFsx.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFsx.getText())){
                                        JOptionPane.showMessageDialog(JFsx, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                        return ;
                                        }
                                if(Integer.parseInt( JFnx.getText() )-4<Integer.parseInt( JFsx.getText() ))
                                        JOptionPane.showMessageDialog(JFsx, 
                                                   "You must set: sx <= nx-4",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFsx);
                /*sz*/
                toolbar1.add(new JLabel(" sz:"));
                JFsz.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFsz.getText())){
                                        JOptionPane.showMessageDialog(JFsz, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                        return ;
                                        }
                                if(Integer.parseInt( JFnz.getText() )-4<Integer.parseInt( JFsz.getText() ))
                                        JOptionPane.showMessageDialog(JFsz, 
                                                   "You must set: sz <= nz-4",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar1.add(JFsz);
                /* toolbar2 */
                JToolBar toolbar2 = new JToolBar();
                /*nt*/
                toolbar2.add(new JLabel(" nt:"));
                final JTextField JFnt = new JTextField(5);
                JFnt.setFont(new Font("Georgia", Font.BOLD, 15));
                JFnt.setText("300");
                JFnt.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFnt.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar2.add(JFnt);
                /*dt*/
                toolbar2.add(new JLabel("dt(us):"));
                final JTextField JFdt = new JTextField(5);
                JFdt.setFont(new Font("Georgia", Font.BOLD, 15));
                JFdt.setText("1000");
                JFdt.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFdt.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar2.add(JFdt);
                /*fm*/
                toolbar2.add(new JLabel("f(Hz):"));
                final JTextField JFfm = new JTextField(5);
                JFfm.setFont(new Font("Georgia", Font.BOLD, 15));
                JFfm.setText("25");
                JFfm.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFfm.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar2.add(JFfm);
                /*npd*/
                toolbar2.add(new JLabel(" npd:"));
                final JTextField JFnpd = new JTextField(5);
                JFnpd.setFont(new Font("Georgia", Font.BOLD, 15));
                JFnpd.setText("20");
                JFnpd.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFnpd.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar2.add(JFnpd);
                /*mm*/
                toolbar2.add(new JLabel(" mm:"));
                String[] m = {" 1 "," 2 "," 3 "," 4 "};
                JComboBox comboxM = new JComboBox<String>(m);
                comboxM.setSelectedIndex(3);
                MM = 4;
                comboxM.addItemListener(new ItemListener(){
                        public void itemStateChanged(ItemEvent e) {
                                if (e.getStateChange() == ItemEvent.SELECTED) {
                                        JComboBox combo = (JComboBox) e.getSource();
                                        int index = combo.getSelectedIndex();
                                        MM = index+1;
                                        }
                              }
                    });
                toolbar2.add(comboxM);
                /* toolbar3 */
                JToolBar toolbar3 = new JToolBar();
                /*velocity*/
                toolbar3.add(new JLabel(" vel[m/s]:"));
                final JTextField JFvelocity = new JTextField(5);
                JFvelocity.setFont(new Font("Georgia", Font.BOLD, 15));
                JFvelocity.setText("2000");
                JFvelocity.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFvelocity.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar3.add(JFvelocity);
                /*epsilon*/
                toolbar3.add(new JLabel(" eps[E-2]:"));
                final JTextField JFepsilon = new JTextField(5);
                JFepsilon.setFont(new Font("Georgia", Font.BOLD, 15));
                JFepsilon.setText("40");
                JFepsilon.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFepsilon.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar3.add(JFepsilon);
                /*delta*/
                toolbar3.add(new JLabel(" del[E-2]:"));
                final JTextField JFdelta = new JTextField(5);
                JFdelta.setFont(new Font("Georgia", Font.BOLD, 15));
                JFdelta.setText("20");
                JFdelta.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFdelta.getText()))
                                        JOptionPane.showMessageDialog(JFnt, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                toolbar3.add(JFdelta);
                /*OK button*/
                JButton buttonOK = new JButton(" >OK< ");
                buttonOK.setAlignmentX(1f);
                buttonOK.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                NX = Integer.parseInt( JFnx.getText() );
                                NZ = Integer.parseInt( JFnz.getText() );
                                DX = (float)Integer.parseInt( JFdx.getText() );
                                DZ = (float)Integer.parseInt( JFdz.getText() );
                                SX = Integer.parseInt( JFsx.getText() );
                                SZ = Integer.parseInt( JFsz.getText() );
                                NT = Integer.parseInt( JFnt.getText() );
                                DT = (float)(Integer.parseInt( JFdt.getText() )/1000000.0f);
                                FM = (float)Integer.parseInt( JFfm.getText() );
                                npd = Integer.parseInt( JFnpd.getText() );
                                velocity = (float)Integer.parseInt( JFvelocity.getText() );
                                epsilon = (float)(Integer.parseInt( JFepsilon.getText() )/100.0f);
                                delta = (float)(Integer.parseInt( JFdelta.getText() )/100.0f);
                                System.out.println(NX);
                                System.out.println(NZ);
                                System.out.println(DX);
                                System.out.println(DZ);
                                System.out.println(SX);
                                System.out.println(SZ);
                                System.out.println(NT);
                                System.out.println(DT);
                                System.out.println(FM);
                                System.out.println(npd);
                                System.out.println(MM);
                                System.out.println(velocity);
                                System.out.println(epsilon);
                                System.out.println(delta);
                                new myXmovieJFrame(velocity,epsilon,delta,SX,SZ,NX,NZ,DX,DZ,NT,DT,FM,MM,npd);
                              }
                    });
                toolbar3.add(buttonOK);
                panel.add(toolbar0);
                panel.add(toolbar1);
                panel.add(toolbar2);
                panel.add(toolbar3);
                add(panel, BorderLayout.NORTH);
                JLabel labelTitle = new JLabel("Click The >OK< Button to GO!",JLabel.CENTER);
                labelTitle.setFont(new Font("Georgia", Font.BOLD, 25));
                labelTitle.setForeground(new Color(50, 50, 25));
                add(labelTitle);
                setVisible(true);
          }
        public static boolean isNumeric(String str){
        /*Copyright: http://javapub.iteye.com/blog/666544*/
                for (int i = 0; i < str.length(); i++){
                        if (!Character.isDigit(str.charAt(i))){
                                return false;
                              }
                      }
                return true;
          }
        public static void main(String[]args){
                Demo demo = new Demo();
          }
}
```
效果如下：
GUI主界面：
![](https://img-blog.csdn.net/20170916195800036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
运行效果（图为动图，显示原因）：
![](https://img-blog.csdn.net/20170916195846606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
整体效果：
![](https://img-blog.csdn.net/20170916195902601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
