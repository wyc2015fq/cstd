# Java实现二维VTI介质一阶拟声波方程正演模拟GUI图形用户界面（加强版） - Koma Hub - CSDN博客
2018年01月30日 12:45:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：678
依据的拟声波公式如下：
```
du/dt=1/rho*dp/dx , 
dw/dt=1/rho*dq/dz ,  
dp/dt=rho*vpx^2*du/dx+rho*vp*vpn*dw/dz ,
dq/dt=rho*vp*vpn*du/dx+rho*vp^2*dw/dz ,
                vpx^2=vp^2*(1+2*epsilon);
                vpn^2=vp^2*(1+2*delta);
```
还是直接上程序：
```java
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseAdapter;
import java.awt.event.WindowAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.Toolkit;
import java.awt.Dimension;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Color;
import java.awt.FileDialog;
import java.awt.Color;
import java.awt.GraphicsEnvironment;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.BasicStroke;
import java.awt.Image;
import java.awt.image.BufferedImage;
import javax.swing.JColorChooser;
import javax.swing.JComponent;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextPane;
import javax.swing.JEditorPane;
import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JPopupMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JProgressBar;
import javax.swing.JToolBar;
import javax.swing.JList;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JDialog;
import javax.swing.JComboBox;
import javax.swing.JCheckBox;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.UIManager;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.Timer;
import javax.swing.JOptionPane;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.MutableAttributeSet;
import javax.swing.text.AttributeSet;
import javax.swing.border.EmptyBorder;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.Writer;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.nio.file.Path;
import javax.imageio.ImageIO;
import net.java.dev.designgridlayout.DesignGridLayout;
//a#############################################################################################
//a##
//a##                myXmovie1OrderPMLParasJFrame -> the tmovie parameters basic frame
//a##
//a##                    myXmovie1OrderPMLJFrame -> the frame include a movie panel
//a##                           innerClass: myXmovie1OrderPMLJPanel -> the panel to paint
//a##                                          innerClass: Wavelet,Wave,vtiFD,vtiModel
//a##
//a##                myXmovie1OrderPMLAboutDialog -> show the about of the Tmovie
//a##
//a############################################################################################# 
//a#################################
//a##
//a##  myXmovie1OrderPMLJFrame
//a##
//a#################################
class myXmovie1OrderPMLJFrame extends JFrame{
        private Toolkit toolkit;
        private myXmovie1OrderPMLJPanel myxmovie1OrderPMLJPanel;
        private File FileSave;
        private int it = -1;
        private JProgressBar progressBar;
        private ActionListener updateProBar;
        Timer timer;
        Timer ntimer;
        myXmovie1OrderPMLJFrame(float velocity, float epsilon, float delta, 
                       int SX, int SZ, int NX, int NZ, float DX,float DZ,
                       int NT, float DT, float FM, int MM, int npd,
                        String filev, String filee, String filed,
                        boolean flag_SelectFile, boolean flag_VTI,
                        boolean Sp, boolean Sq, boolean SV,int Snap){
                final FileDialog FileDialogSave;
                setTitle("Toa_Xmovie");
                setSize(300, 360);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                addWindowListener(new WindowAdapter() {
                        public void windowClosing(WindowEvent windowEvent){
                                timer.stop();
                                ntimer.stop();
                              }
                    });
                /* menu: save, close */
                JMenuBar JMenuBarSave;
                final JMenu JMenuSave;
                JMenuItem JMenuItemSaveClose,JMenuItemSaveSave;
                JMenuBarSave=new JMenuBar();
                JMenuSave=new JMenu("File");
                JMenuSave.setFont(new Font("Georgia", Font.BOLD, 20));
                JMenuItemSaveSave=new JMenuItem("Save as PNG Image");
                JMenuItemSaveSave.setFont(new Font("Georgia", Font.BOLD, 15));
                JMenuItemSaveClose=new JMenuItem("Close");
                JMenuItemSaveClose.setFont(new Font("Georgia", Font.BOLD, 15));
                JMenuSave.add(JMenuItemSaveSave);
                JMenuSave.add(JMenuItemSaveClose);
                JMenuBarSave.add(JMenuSave);
                setJMenuBar(JMenuBarSave);
                /* toolbar */
                final JPanel panel = new JPanel();
                panel.setLayout(new GridLayout(1,1));
                final JToolBar toolbar0 = new JToolBar();
                final JLabel ntime = new JLabel("0");
                ntime.setFont(new Font("Georgia", Font.BOLD, 15));
                final int ntit = NT;
                progressBar = new JProgressBar();
                progressBar.setStringPainted(true);
                updateProBar = new ActionListener() {
                        public void actionPerformed(ActionEvent actionEvent) {
                                int val = progressBar.getValue();
                                if (it >= 0) {
                                        ntime.setText(Integer.toString(it+1)+"/"+Integer.toString(ntit));
                                        progressBar.setValue((int)(it*1f/ntit*100f)+1);
                                        }
                                if(it == ntit-1){
                                        JOptionPane.showMessageDialog(panel, "Complete!",
                                                "Info VTI snapshot", JOptionPane.INFORMATION_MESSAGE);
                                        ntimer.stop();
                                        }
                              }
                    };
                ntimer = new Timer(100, updateProBar);
                ntimer.start();
                JLabel JLit = new JLabel("it:");
                JLit.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar0.add(JLit);
                toolbar0.add(progressBar);
                toolbar0.add(ntime);
                panel.add(toolbar0);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2);
                /* xmovie show panel in xmovieJFrame */
                myxmovie1OrderPMLJPanel = new myXmovie1OrderPMLJPanel(velocity,epsilon,delta,
                                                    SX, SZ, NX, NZ, DX,DZ,NT, DT, FM, MM,npd,
                                                    filev,filee,filed,flag_SelectFile,flag_VTI,
                                                    Sp, Sq, SV, Snap);
                JMenuItemSaveClose.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                dispose();
                              }
                    });
                JMenuItemSaveSave.addActionListener(new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                                saveJPanel.saveImage(myxmovie1OrderPMLJPanel, JMenuSave);
                              }
                   });
                add(panel, BorderLayout.NORTH);
                add(myxmovie1OrderPMLJPanel);
                setVisible(true);
          }
        //a#################################
        //a##
        //a##  myXmovie1OrderPMLJPanel for myXmovie1OrderPMLJFrame
        //a##
        //a#################################
        class myXmovie1OrderPMLJPanel extends JPanel {
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
                private boolean Sp, Sq, SV;
                private int Snap;
                ActionListener go;
                myXmovie1OrderPMLJPanel(float velocity, float epsilon, float delta,
                               int SX, int SZ, int NX, int NZ, float DX,float DZ,
                               int NT, float DT, float FM, int MM, int npd,
                                String filev, String filee, String filed,
                                boolean flag_SelectFile, boolean flag_VTI,
                                boolean Sp, boolean Sq, boolean SV,int Snap){
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
                        this.Sp = Sp;
                        this.Sq = Sq;
                        this.SV = SV;
                        this.Snap = Snap;
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
                        if(flag_VTI)
                                model0.InitModel(velocity, epsilon, delta);
                        else
                                model0.InitModel(velocity, 0f, 0f);
                        if(flag_SelectFile){
                                if(flag_VTI)
                                        model0.ReadVTIModel(filev,filee,filed);
                                else    model0.ReadISOModel(filev);
                              }
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
                                                wlt[it] = (float)((1-2*pi*pi*xx)
                                                                *Math.exp(-(pi*pi*xx)));
                                        }//end if
                                        if(wtype==2&&t <= 2*tdelay){
		                                x = (float)((-4.0f)*(fm*fm*pi*pi)
                                                                /Math.log(0.1));
		                                wlt[it] = (float)((-2.0f)*(pi*pi*ts)
                                                               *Math.exp(-x*ts*ts));
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
                        public float[] P, Px, Pz;
                        public float[] Q, Qx, Qz;
                        public float[] U, W;
                        public Wave(int nx,int nz){
                                this.nx = nx;
                                this.nz = nz;
                            }
                        public void Init_Wave(){
                                P = new float[(nx)*(nz)];
                                Px = new float[(nx)*(nz)];
                                Pz = new float[(nx)*(nz)];
                                Q = new float[(nx)*(nz)];
                                Qx = new float[(nx)*(nz)];
                                Qz = new float[(nx)*(nz)];
                                U = new float[(nx)*(nz)];
                                W = new float[(nx)*(nz)];
                                for(int i = 0; i < (nx)*(nz); i++){
                                        P[i] = 0.0f;Px[i] = 0.0f;Pz[i] = 0.0f;U[i] = 0.0f;
                                        Q[i] = 0.0f;Qx[i] = 0.0f;Qz[i] = 0.0f;W[i] = 0.0f;
                                }//end
                        }//initial Wave
                    }
                //a#################################
                //a##
                //a##  inner class vtiFD 
                //a##
                //a#################################
                class vtiFD {
                        private float[][]
                           c = {{1.125f,-0.04166667f,0f,0f},//mm==2
                                {1.1718750f,-0.065104167f,0.0046875f,0f},//mm==3
                                {1.196289f,-0.0797526f,0.009570313f,-0.0006975447f}};//mm==4
                        private float[] coffx1, coffx2, coffz1, coffz2,
                                        acoffx1, acoffx2, acoffz1, acoffz2;
                        private float pi = 3.141592653f;
                        private float d0;
                        private float eps,del;
                        vtiModel model;
                        Wave wave;
                        Wavelet wavelet;
                        public vtiFD(vtiModel model, Wave wave, Wavelet wavelet){
                                this.model = model;
                                this.wave = wave;
                                this.wavelet = wavelet;
                                d0 = (float)(6.0f*model.velocity[0]
                                        *Math.log(100000.0f)/(2.0f*npd*model.dx) );
                                initial_coffe();
                        }//end struct function
                        public float[] FD(int sx, int sz, int mm, int it){
                                float dt2x2 = wavelet.dt*wavelet.dt/(model.dx*model.dx);
                                float dt2z2 = wavelet.dt*wavelet.dt/(model.dz*model.dz);
                                int i, j, id, im;
                                float xx,zz,flag;
                                        if(it%50==0)System.out.println("Time: "+(it*wavelet.dt)+" (s)");
                                        addSource(wave, wavelet, sx, sz, it);
                                        update_vel(mm);
                                        update_stress(mm,sx,sz);
                                if(Snap==1)return wave.P;
                                if(Snap==2)return wave.Q;
                                if(Snap==3)return wave.U;
                                if(Snap==4)return wave.W;
                                if(Snap==5)return wave.Px;
                                if(Snap==6)return wave.Pz;
                                if(Snap==7)return wave.Qx;
                                else return wave.Qz;
                                //return wave.U;
                        }//end FD
                        public void update_vel(int mm){
                                float dtx = wavelet.dt/model.dx;
                                float dtz = wavelet.dt/model.dz;
                                for(int iz=mm;iz<model.nz-mm;iz++) { 
                                        for(int ix=mm;ix<model.nx-mm;ix++) {
                                                float xx = 0.0f;
                                                float zz = 0.0f;
                                                for(int im=0;im<mm;im++) {
                                                        xx += c[mm-2][im]
                                                              *(wave.P[(ix+im+1)*model.nz + iz]
                                                               -wave.P[(ix-im)*model.nz + iz]);
                                                        zz += c[mm-2][im]
                                                              *(wave.Q[ix*model.nz + iz+im+1]
                                                               -wave.Q[ix*model.nz + iz-im]);
                                                            }
                                                wave.U[ix*model.nz + iz] = coffx2[ix]*wave.U[ix*model.nz + iz]
                                                                        -coffx1[ix]*dtx*xx;
                                                wave.W[ix*model.nz + iz] = coffz2[iz]*wave.W[ix*model.nz + iz]
                                                                        -coffz1[iz]*dtz*zz;
                                                  }
                                        }
                              }
                        public void update_stress(int mm,int sx, int sz){
                                float dtx = wavelet.dt/model.dx;
                                float dtz = wavelet.dt/model.dz;
                                float r1 = 4, r2 = 15;
                                for(int iz=mm;iz<model.nz-mm;iz++) { 
                                        for(int ix=mm;ix<model.nx-mm;ix++) {
                                                float xx = 0.0f;
                                                float zz = 0.0f;
                                                for(int im=0;im<mm;im++) {
                                                        xx += c[mm-2][im]
                                                              *(wave.U[(ix+im)*model.nz + iz]
                                                               -wave.U[(ix-im-1)*model.nz + iz]);
                                                        zz += c[mm-2][im]
                                                              *(wave.W[ix*model.nz + iz+im]
                                                               -wave.W[ix*model.nz + iz-im-1]);
                                                            }
                                                int id = ix*model.nz + iz;
                                                float ir = (float)Math.sqrt(
                                                               Math.pow(sx-ix,2)
                                                              +Math.pow(sz-iz,2));
                                                float sin = (float)Math.sin((ir-r1)/(r2-r1)*pi/2f);
                                                float sv = SV?(ir<r1?0f:(ir<r2?sin:1f)):1f; 
                                                eps = sv*model.epsilon[id];
                                                del = sv*model.delta[id];
                                                wave.Px[id] = acoffx2[ix]*wave.Px[id]
                                                             -acoffx1[ix]
                                                                *model.velocity[id]*model.velocity[id]
                                                                *(1+2*eps)
                                                                *dtx*xx;
                                                wave.Pz[id] = acoffz2[iz]*wave.Pz[id]
                                                             -acoffz1[iz]
                                                                *model.velocity[id]*model.velocity[id]
                                                                *(float)Math.pow((1+2*del),0.5)
                                                                *dtz*zz;
                                                wave.Qx[id] = acoffx2[ix]*wave.Qx[id]
                                                             -acoffx1[ix]
                                                                *model.velocity[id]*model.velocity[id]
                                                                *(float)Math.pow((1+2*del),0.5)
                                                                *dtx*xx;
                                                wave.Qz[id] = acoffz2[iz]*wave.Qz[id]
                                                             -acoffz1[iz]
                                                                *model.velocity[id]*model.velocity[id]
                                                                *dtz*zz;
                                                wave.P[id] = wave.Px[id] + wave.Pz[id];
                                                wave.Q[id] = wave.Qx[id] + wave.Qz[id];
                                                  }
                                        }
                              }
                        public void initial_coffe(){
                                coffx1 = new float[model.nx];
                                coffx2 = new float[model.nx];
                                coffz1 = new float[model.nx];
                                coffz2 = new float[model.nx];
                                acoffx1 = new float[model.nx];
                                acoffx2 = new float[model.nx];
                                acoffz1 = new float[model.nx];
                                acoffz2 = new float[model.nx];
                                for(int i=0;i<npd;i++) {   
                                        coffx1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow((npd-0.5f-i)/npd,2f))/2f));
                                        coffx2[i] = (float)(coffx1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow((npd-0.5f-i)/npd,2f))/2f));
                                        coffz1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow((npd-0.5f-i)/npd,2f))/2f));
                                        coffz2[i] = (float)(coffz1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow((npd-0.5f-i)/npd,2f))/2f));
                                        //System.out.println(coffx1[i]);
                                        acoffx1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow(((npd-i)*1.0f)/npd,2f))/2f));
                                        acoffx2[i] = (float)(acoffx1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow(((npd-i)*1.0f)/npd,2f))/2f));
                                        acoffz1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow(((npd-i)*1.0f)/npd,2f))/2f));
                                        acoffz2[i] = (float)(acoffz1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow(((npd-i)*1.0f)/npd,2f))/2f));
		                         }
                                for(int i=npd;i<model.nx-npd;i++)  {
                                        coffx1[i] = 1.0f;
                                        coffx2[i] = 1.0f;
                                        acoffx1[i] = 1.0f;
                                        acoffx2[i] = 1.0f;
                                        }
                                for(int i=npd;i<model.nz-npd;i++)  {
                                        coffz1[i] = 1.0f;
                                        coffz2[i] = 1.0f;
                                        acoffz1[i] = 1.0f;
                                        acoffz2[i] = 1.0f;
                                        }
                                for(int i=model.nx-npd;i<model.nx;i++) {
                                        coffx1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow((1f+i-model.nx+npd)/npd,2f))/2f));
                                        coffx2[i] = (float)(coffx1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow((1f+i-model.nx+npd)/npd,2f))/2f));
                                        //System.out.println(coffx1[i]);
                                        acoffx1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow(((1f+i-model.nx+npd)*1.0f)/npd,2f))/2f));
                                        acoffx2[i] = (float)(acoffx1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow(((1f+i-model.nx+npd)*1.0f)/npd,2f))/2f));
                                        }
                                for(int i=model.nz-npd;i<model.nz;i++) {
                                        coffz1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow((1f+i-model.nz+npd)/npd,2f))/2f));
                                        coffz2[i] = (float)(coffz1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow((1f+i-model.nz+npd)/npd,2f))/2f));
                                        //System.out.println(coffz1[i]);
                                        acoffz1[i] = (float)(1f/(1f+
                                                (wavelet.dt*d0*Math.pow(((1f+i-model.nz+npd)*1.0f)/npd,2f))/2f));
                                        acoffz2[i] = (float)(acoffz1[i]*(1f-
                                                (wavelet.dt*d0*Math.pow(((1f+i-model.nz+npd)*1.0f)/npd,2f))/2f));
                                        }
                              }
                        public void addSource(Wave wave, Wavelet wavelet, int sx, int sz, int it){
                                if(Sp)wave.P[(sx-1)*(wave.nz)+sz-1] += wavelet.wlt[it];
                                if(Sq)wave.Q[(sx-1)*(wave.nz)+sz-1] += wavelet.wlt[it];
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
                        public void ReadVTIModel(String FileNameOfVelocity, 
                                              String FileNameOfEpsilon, 
                                              String FileNameOfDelta){
                                DataInputStream fpVelocity = null, fpEpsilon = null, fpDelta = null;
                                try{    
                                        if(!new File(FileNameOfVelocity).exists()){  
                                                System.out.println("The "+FileNameOfVelocity
                                                                  +" file dont't exists");
                                                return;  
                                                  } 
                                        if(!new File(FileNameOfEpsilon).exists()){  
                                                System.out.println("The "+FileNameOfEpsilon
                                                                  +" file dont't exists");
                                                return;  
                                                  } 
                                        if(!new File(FileNameOfDelta).exists()){  
                                                System.out.println("The "+FileNameOfDelta
                                                                  +" file dont't exists");
                                                return;  
                                                  } 
                                        fpVelocity = new DataInputStream(
                                                        new FileInputStream(
                                                          new File(FileNameOfVelocity)));
                                         fpEpsilon = new DataInputStream(
                                                        new FileInputStream(
                                                          new File(FileNameOfEpsilon)));
                                           fpDelta = new DataInputStream(
                                                        new FileInputStream(
                                                          new File(FileNameOfDelta))); 
                                        int i = 0, j =0 , k =0 ;
                                        while(fpVelocity.available()>0
                                                &&fpEpsilon.available()>0
                                                &&fpDelta.available()>0
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
                        public void ReadISOModel(String FileNameOfVelocity){
                                DataInputStream fpVelocity = null;
                                try{    
                                        if(!new File(FileNameOfVelocity).exists()){  
                                                System.out.println("The "+FileNameOfVelocity
                                                                  +" file dont't exists");
                                                return;  
                                                  } 
                                        fpVelocity = new DataInputStream(
                                                        new FileInputStream(
                                                          new File(FileNameOfVelocity)));
                                        int i = 0;
                                        while(fpVelocity.available()>0
                                                &&i<nx*nz){   
                                                velocity[i++] = fpVelocity.readFloat();  
                                                velocity[i-1] = swap(velocity[i-1]);
                                              } 
                                }catch(Exception e){  
                                        e.printStackTrace();  
                                    }
                        }//read isoModel
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
//a##  myXmovie1OrderPMLAboutDialog to myJMenuBar
//a##
//a#################################
class myXmovie1OrderPMLAboutDialog extends JDialog {
        private Toolkit toolkit;
        private int itext =0 ;
        public myXmovie1OrderPMLAboutDialog() {
                setTitle("About Information ");
                setSize(300, 350);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height -getHeight())/2);
                JPanel basic = new JPanel();
                basic.setLayout(new BoxLayout(basic, BoxLayout.Y_AXIS));
                add(basic);
                JPanel topPanel = new JPanel(new BorderLayout(0, 0));
                topPanel.setMaximumSize(new Dimension(450, 0));
                JLabel hint = new JLabel("About VTIsnapshot Tmovie");
                hint.setBorder(BorderFactory.createEmptyBorder(0, 25, 0, 0));
                topPanel.add(hint);
                ImageIcon icon = new ImageIcon("picture/ButtonImg/tmovie32.png");
                JLabel label = new JLabel(icon);
                label.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
                topPanel.add(label, BorderLayout.EAST);
                JSeparator separator = new JSeparator();
                separator.setForeground(Color.gray);
                topPanel.add(separator, BorderLayout.SOUTH);
                basic.add(topPanel);
                JPanel textPanel = new JPanel(new BorderLayout());
                textPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
                final JTextPane pane = new JTextPane();
                pane.setContentType("text/html");
                final String[] text ={ "<p><b>About The Tmovie1order</b></p>" +
                                "<p>. Author: Rong Tao</p>" +
                                "<p>. Time: 2017.8.11-8.12 </p>"+
                                "<p>. @Copyright Rong Tao </p>"+
                                "<p>. Location: @UPC </p>",
                                "<p><b>Function</b></p>" +
                                "<p>.du/dt=dp/dx ,</p>" +
                                "<p>.dw/dt=dq/dz ,</p>"+
                                "<p>.dp/dt=vpx^2*du/dx+vp*vpn*dw/dz ,</p>"+
                                "<p>.dq/dt=vp*vpn*du/dx+vp^2*dw/dz ,</p>"+
                                "<p>.-----------vpx^2=vp^2*(1+2*epsilon)</p>"+
                                "<p>.-----------vpn^2=vp^2*(1+2*delta)</p>",
                                "<p><b></b></p>" +
                                "<p>.   You just change the velocity,</p>" +
                                "<p>. epsilon and delta to the VTI media.  </p>"+
                                "<p>.   This is a constant Paras. </p>"+
                                "<p>.   </p>"+
                                "<p>.   </p>",
                                "<p><b>Use It Stydy</b></p>" +
                                "<p>.   You can see the wavefield propagate.</p>" +
                                "<p>. after click OK button.</p>"+
                                "<p>.   You can change the shot location.</p>"+
                                "<p>.  </p>",
                                "<p><b>Hope you enjoy it.</b></p>" +
                                "<p>. Learning makes me happy.</p>" +
                                "<p>. I like fitness.</p>"+
                                "<p>. Wish me Luck.</p>"+
                                "<p>. Good Luck! </p>"};
                pane.setText(text[itext]);
                pane.setEditable(false);
                textPanel.add(pane);
                basic.add(textPanel);
                JPanel boxPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 0));
                basic.add(boxPanel);
                JPanel bottom = new JPanel(new FlowLayout(FlowLayout.RIGHT));
                JButton previous = new JButton("<<-Previous");
                previous.setMnemonic(KeyEvent.VK_N);
                previous.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                if(itext>0&&itext<=4)pane.setText(text[--itext]);
                              }
                    });
                JButton next = new JButton("Next->>");
                next.setMnemonic(KeyEvent.VK_N);
                next.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                if(itext>=0&&itext<4)pane.setText(text[++itext]);
                              }
                    });
                JButton close = new JButton("Close");
                close.setMnemonic(KeyEvent.VK_C);
                close.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                dispose();
                              }
                    });
                bottom.add(previous);
                bottom.add(next);
                bottom.add(close);
                basic.add(bottom);
                bottom.setMaximumSize(new Dimension(300, 0));
                setResizable(false);
                setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
                setVisible(true);
          }
}
//a#################################
//a##
//a##  myXmovie1OrderPMLParasJFrame
//a##
//a#################################
class myXmovie1OrderPMLParasJFrame extends JFrame{
        private Toolkit toolkit;
        private int NX,NZ,SX,SZ,NT,MM,npd;
        private float velocity,epsilon,delta,DX,DZ,DT,FM;
        private boolean flag_SelectFile = false;
        private boolean flag_VTI = true;
        private String filev, filee, filed;
        private JLabel tmplabel;
        private DesignGridLayout layout1,layout2,layout3;
        private JSeparator js1 = new JSeparator();
        private JSeparator js2 = new JSeparator();
        private boolean Sp = true, Sq = true, SV = true;
        private int Snap = 1;
        myXmovie1OrderPMLParasJFrame(){
                setTitle("Toa VTI Snapshot Movie");
                setSize(700, 460);
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2);
                /* menu: About, close */
                JMenuBar menubar = new JMenuBar();
                JMenu menu=new JMenu("File");
                menu.setFont(new Font("Georgia", Font.BOLD, 20));
                JMenuItem about=new JMenuItem("About");
                about.setFont(new Font("Georgia", Font.BOLD, 15));
                menu.add(about);
                about.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                javax.swing.SwingUtilities.invokeLater(new Runnable() {
                                        public void run() {
                                                new myXmovie1OrderPMLAboutDialog();
                                                  }
                                        });
                              }
                    });
                JMenuItem close =new JMenuItem("Close");
                close.setFont(new Font("Georgia", Font.BOLD, 15));
                menu.add(close);
                close.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                dispose();
                              }
                    });
                menubar.add(menu);
                setJMenuBar(menubar);
                /* include 2 toolbars */
                final JPanel panel = new JPanel();
                panel.setLayout(new GridLayout(5,1));
                final JPanel panel1 = new JPanel();
                final JPanel panel2 = new JPanel();
                final JPanel panel3 = new JPanel();
                layout1 = new DesignGridLayout(panel1);
                layout2 = new DesignGridLayout(panel2);
                layout3 = new DesignGridLayout(panel3);
                /* toolbar0 */
                final JToolBar toolbar0 = new JToolBar();
                final JCheckBox checkboxVTI = new JCheckBox("vti", true);
                checkboxVTI.setFocusable(false);
                checkboxVTI.setFont(new Font("Georgia", Font.BOLD, 15));
                final JCheckBox checkboxISO = new JCheckBox("iso", false);
                checkboxISO.setFocusable(false);
                checkboxISO.setFont(new Font("Georgia", Font.BOLD, 15));
                JCheckBox checkboxSelectFile = new JCheckBox("Select File[Y/N]", false);
                checkboxSelectFile.setFocusable(false);
                checkboxSelectFile.setFont(new Font("Georgia", Font.BOLD, 15));
                final JLabel selectYN = new JLabel(">NO    ");
                selectYN.setFont(new Font("Georgia", Font.BOLD, 15));
                final JLabel labelvtiiso = new JLabel(">VTI   ");
                labelvtiiso.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar0.add(checkboxVTI);
                toolbar0.add(checkboxISO);
                toolbar0.add(labelvtiiso);
                toolbar0.add(checkboxSelectFile);
                toolbar0.add(selectYN);
                ImageIcon folder = new ImageIcon("picture/ButtonImg/folder32.png");
                /* toolbar01 */
                final JToolBar toolbar01 = new JToolBar();
                tmplabel = new JLabel("Velocity:");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar01.add(tmplabel);
                final JTextField JFselectvelocity = new JTextField(30);
                JFselectvelocity.setFont(new Font("Georgia", Font.BOLD, 15));
                JFselectvelocity.setText("/home/");
                filev = JFselectvelocity.getText();
                toolbar01.add(JFselectvelocity);
                final JButton buttonvelocity = new JButton(folder);
                buttonvelocity.setAlignmentX(1f);
                buttonvelocity.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                JFileChooser fileopen = new JFileChooser();
                                FileFilter filtertxt = new FileNameExtensionFilter("*.dat", "dat");
                                FileFilter filterpar = new FileNameExtensionFilter("*.bin", "bin");
                                fileopen.addChoosableFileFilter(filtertxt);
                                fileopen.addChoosableFileFilter(filterpar);
                                int ret = fileopen.showDialog(panel, "Select");
                                if (ret == JFileChooser.APPROVE_OPTION) {
                                        File file = fileopen.getSelectedFile();
                                        JFselectvelocity.setText(file.toString());
                                        filev = file.toString();
                                        System.out.println("Velocity file name : "+filev);
                                        }
                              }
                    });
                toolbar01.add(buttonvelocity);
                /* toolbar02 */
                final JToolBar toolbar02 = new JToolBar();
                tmplabel = new JLabel(" Epsilon:");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar02.add(tmplabel);
                final JTextField JFselectepsilon = new JTextField(30);
                JFselectepsilon.setFont(new Font("Georgia", Font.BOLD, 15));
                JFselectepsilon.setText("/home/");
                filee = JFselectepsilon.getText();
                toolbar02.add(JFselectepsilon);
                final JButton buttonepsilon = new JButton(folder);
                buttonepsilon.setAlignmentX(1f);
                buttonepsilon.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                JFileChooser fileopen = new JFileChooser();
                                FileFilter filtertxt = new FileNameExtensionFilter("*.dat", "dat");
                                FileFilter filterpar = new FileNameExtensionFilter("*.bin", "bin");
                                fileopen.addChoosableFileFilter(filtertxt);
                                fileopen.addChoosableFileFilter(filterpar);
                                int ret = fileopen.showDialog(panel, "Select");
                                if (ret == JFileChooser.APPROVE_OPTION) {
                                        File file = fileopen.getSelectedFile();
                                        JFselectepsilon.setText(file.toString());
                                        filee = file.toString();
                                        System.out.println("epsilon file name : "+filee);
                                        }
                              }
                    });
                toolbar02.add(buttonepsilon);
                /* toolbar03 */
                final JToolBar toolbar03 = new JToolBar();
                tmplabel = new JLabel("   Delta:");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar03.add(tmplabel);
                final JTextField JFselectdelta = new JTextField(30);
                JFselectdelta.setFont(new Font("Georgia", Font.BOLD, 15));
                JFselectdelta.setText("/home/");
                filed = JFselectdelta.getText();
                toolbar03.add(JFselectdelta);
                final JButton buttondelta = new JButton(folder);
                buttondelta.setAlignmentX(1f);
                buttondelta.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                JFileChooser fileopen = new JFileChooser();
                                FileFilter filtertxt = new FileNameExtensionFilter("*.dat", "dat");
                                FileFilter filterpar = new FileNameExtensionFilter("*.bin", "bin");
                                fileopen.addChoosableFileFilter(filtertxt);
                                fileopen.addChoosableFileFilter(filterpar);
                                int ret = fileopen.showDialog(panel, "Select");
                                if (ret == JFileChooser.APPROVE_OPTION) {
                                        File file = fileopen.getSelectedFile();
                                        JFselectdelta.setText(file.toString());
                                        filed = file.toString();
                                        System.out.println("delta file name : "+filed);
                                        }
                              }
                    });
                toolbar03.add(buttondelta);
                JFselectvelocity.setEnabled(false);
                buttonvelocity.setEnabled(false);
                JFselectepsilon.setEnabled(false);
                buttonepsilon.setEnabled(false);
                JFselectdelta.setEnabled(false);
                buttondelta.setEnabled(false);
                final JTextField JFvelocity = new JTextField(5);
                JFvelocity.setFont(new Font("Georgia", Font.BOLD, 15));
                JFvelocity.setText("2000");
                JFvelocity.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFvelocity.getText()))
                                        JOptionPane.showMessageDialog(JFvelocity, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                final JTextField JFepsilon = new JTextField(5);
                JFepsilon.setFont(new Font("Georgia", Font.BOLD, 15));
                JFepsilon.setText("40");
                JFepsilon.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFepsilon.getText()))
                                        JOptionPane.showMessageDialog(JFvelocity, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                final JTextField JFdelta = new JTextField(5);
                JFdelta.setFont(new Font("Georgia", Font.BOLD, 15));
                JFdelta.setText("20");
                JFdelta.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFdelta.getText()))
                                        JOptionPane.showMessageDialog(JFvelocity, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                checkboxISO.addActionListener(new ActionListener(){
                        public void actionPerformed(ActionEvent e) {
                                checkboxISO.setSelected(true);
                                checkboxVTI.setSelected(false);
                                flag_VTI = false;
                                 labelvtiiso.setText(">ISO   ");
                                JFselectepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                buttonepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                JFselectdelta.setEnabled(flag_SelectFile & flag_VTI);
                                buttondelta.setEnabled(flag_SelectFile & flag_VTI);
                                JFepsilon.setEnabled(!flag_SelectFile & flag_VTI);
                                JFdelta.setEnabled(!flag_SelectFile & flag_VTI);
                              }
                    });
                checkboxVTI.addActionListener(new ActionListener(){
                        public void actionPerformed(ActionEvent e) {
                                checkboxVTI.setSelected(true);
                                checkboxISO.setSelected(false);
                                flag_VTI = true;
                                labelvtiiso.setText(">VTI   ");
                                JFselectepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                buttonepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                JFselectdelta.setEnabled(flag_SelectFile & flag_VTI);
                                buttondelta.setEnabled(flag_SelectFile & flag_VTI);
                                JFepsilon.setEnabled(!flag_SelectFile & flag_VTI);
                                JFdelta.setEnabled(!flag_SelectFile & flag_VTI);
                              }
                    });
                checkboxSelectFile.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                flag_SelectFile = !flag_SelectFile;
                                if (flag_SelectFile) {
                                        selectYN.setText(">YES   ");
                                } else {
                                        selectYN.setText(">NO    ");
                                        }
                                JFselectvelocity.setEnabled(flag_SelectFile);
                                buttonvelocity.setEnabled(flag_SelectFile);
                                JFselectepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                buttonepsilon.setEnabled(flag_SelectFile & flag_VTI);
                                JFselectdelta.setEnabled(flag_SelectFile & flag_VTI);
                                buttondelta.setEnabled(flag_SelectFile & flag_VTI);
                                JFvelocity.setEnabled(!flag_SelectFile);
                                JFepsilon.setEnabled(!flag_SelectFile & flag_VTI);
                                JFdelta.setEnabled(!flag_SelectFile & flag_VTI);
                              }
                    });
                /*  Sp,Sq,SV,Snap  */
                final JToolBar toolbar04 = new JToolBar();
                final JCheckBox checkboxSp = new JCheckBox("Sp", true);
                checkboxSp.setFocusable(false);
                checkboxSp.setFont(new Font("Georgia", Font.BOLD, 15));
                checkboxSp.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                Sp = !Sp;
                              }
                    });
                toolbar04.add(checkboxSp);
                final JCheckBox checkboxSq = new JCheckBox("Sq", true);
                checkboxSq.setFocusable(false);
                checkboxSq.setFont(new Font("Georgia", Font.BOLD, 15));
                checkboxSq.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                Sq = !Sq;
                              }
                    });
                toolbar04.add(checkboxSq);
                final JCheckBox checkboxSV = new JCheckBox("muteSV", true);
                checkboxSV.setFocusable(false);
                checkboxSV.setFont(new Font("Georgia", Font.BOLD, 15));
                checkboxSV.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                SV = !SV;
                              }
                    });
                toolbar04.add(checkboxSV);
                /*Snap*/
                JLabel labelSnap = new JLabel(" Snap:");
                labelSnap.setFont(new Font("Georgia", Font.BOLD, 15));
                String[] snap = {"Horizontal stress            ",
                                 "Vertical stress              ",
                                 "Horizontal velocity component",
                                 "Vertical velocity component  ",
                                 "Horizontal stress x component",
                                 "Horizontal stress z component",
                                 "Vertical stress x component  ",
                                 "Vertical stress z component  "};
                JComboBox comboxSnap = new JComboBox<String>(snap);
                comboxSnap.setFont(new Font("Georgia", Font.BOLD, 15));
                comboxSnap.setSelectedIndex(0);
                Snap = 1;
                comboxSnap.addItemListener(new ItemListener(){
                        public void itemStateChanged(ItemEvent e) {
                                if (e.getStateChange() == ItemEvent.SELECTED) {
                                        JComboBox combo = (JComboBox) e.getSource();
                                        int index = combo.getSelectedIndex();
                                        Snap = index+1;
                                        }
                              }
                    });
                toolbar04.add(labelSnap);
                toolbar04.add(comboxSnap);
                /*nx*/
                JLabel labelnx = new JLabel(" nx:");
                labelnx.setFont(new Font("Georgia", Font.BOLD, 15));
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
                                else JFsx.setText( Integer.toString(
                                                   Integer.parseInt( JFnx.getText() )/2) );
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {  
                                JFsx.setText( Integer.toString(
                                              Integer.parseInt( JFnx.getText() )/2) );
                                }   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                /*nz*/
                JLabel labelnz = new JLabel(" nz:");
                labelnz.setFont(new Font("Georgia", Font.BOLD, 15));
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
                                else JFsz.setText( Integer.toString(
                                                   Integer.parseInt( JFnz.getText() )/2) );
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {  
                                JFsz.setText( Integer.toString(
                                              Integer.parseInt( JFnz.getText() )/2) );
                                }   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                /*dx*/
                JLabel labeldx = new JLabel(" dx[m]:");
                labeldx.setFont(new Font("Georgia", Font.BOLD, 15));
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
                /*dz*/
                JLabel labeldz = new JLabel(" dz[m]:");
                labeldz.setFont(new Font("Georgia", Font.BOLD, 15));
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
                /*sx*/
                JLabel labelsx = new JLabel(" sx:");
                labelsx.setFont(new Font("Georgia", Font.BOLD, 15));
                JFsx.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFsx.getText())){
                                        JOptionPane.showMessageDialog(JFsx, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                        return ;
                                        }
                                if(Integer.parseInt( JFnx.getText() )-4
                                  <Integer.parseInt( JFsx.getText() ))
                                        JOptionPane.showMessageDialog(JFsx, 
                                                   "You must set: sx <= nx-4",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                /*sz*/
                JLabel labelsz = new JLabel(" sz:");
                labelsz.setFont(new Font("Georgia", Font.BOLD, 15));
                JFsz.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(!isNumeric(JFsz.getText())){
                                        JOptionPane.showMessageDialog(JFsz, 
                                                   "Input is not digital(0-9)!",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                        return ;
                                        }
                                if(Integer.parseInt( JFnz.getText() )-4
                                  <Integer.parseInt( JFsz.getText() ))
                                        JOptionPane.showMessageDialog(JFsz, 
                                                   "You must set: sz <= nz-4",
                                                   "Error", JOptionPane.ERROR_MESSAGE);
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
                layout1.row().grid(labelnx).add(JFnx).grid(labelnz).add(JFnz);
                layout1.row().grid(labeldx).add(JFdx).grid(labeldz).add(JFdz);
                layout1.row().grid(labelsx).add(JFsx).grid(labelsz).add(JFsz);
                /*nt*/
                JLabel labelnt = new JLabel(" nt:");
                labelnt.setFont(new Font("Georgia", Font.BOLD, 15));
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
                /*dt*/
                JLabel labeldt = new JLabel(" dt(us):");
                labeldt.setFont(new Font("Georgia", Font.BOLD, 15));
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
                /*fm*/
                JLabel labelfm = new JLabel(" f(Hz):");
                labelfm.setFont(new Font("Georgia", Font.BOLD, 15));
                final JTextField JFfm = new JTextField(5);
                JFfm.setFont(new Font("Georgia", Font.BOLD, 15));
                JFfm.setText("40");
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
                /*npd*/
                JLabel labelnpd = new JLabel(" npd:");
                labelnpd.setFont(new Font("Georgia", Font.BOLD, 15));
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
                /*mm*/
                JLabel labelmm = new JLabel(" mm:");
                labelmm.setFont(new Font("Georgia", Font.BOLD, 15));
                String[] m = {" 2 "," 3 "," 4 "};
                JComboBox comboxM = new JComboBox<String>(m);
                comboxM.setFont(new Font("Georgia", Font.BOLD, 15));
                comboxM.setSelectedIndex(2);
                MM = 4;
                comboxM.addItemListener(new ItemListener(){
                        public void itemStateChanged(ItemEvent e) {
                                if (e.getStateChange() == ItemEvent.SELECTED) {
                                        JComboBox combo = (JComboBox) e.getSource();
                                        int index = combo.getSelectedIndex();
                                        MM = index+2;
                                        }
                              }
                    });
                layout2.row().grid(labelfm).add(JFfm).grid().add(new JLabel(""));
                layout2.row().grid(labelnt).add(JFnt).grid(labeldt).add(JFdt);
                layout2.row().grid(labelnpd).add(JFnpd).grid(labelmm).add(comboxM);
                /*velocity*/
                tmplabel = new JLabel("m/s");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar01.add(JFvelocity);
                toolbar01.add(tmplabel);
                /*epsilon*/
                tmplabel = new JLabel("E-2");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar02.add(JFepsilon);
                toolbar02.add(tmplabel);
                /*delta*/
                tmplabel = new JLabel("E-2");
                tmplabel.setFont(new Font("Georgia", Font.BOLD, 15));
                toolbar03.add(JFdelta);
                toolbar03.add(tmplabel);
                /*OK button*/
                ImageIcon iconOK = new ImageIcon("picture/ButtonImg/ok32.png");
                JButton buttonOK = new JButton(iconOK);
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
                        if(!flag_SelectFile){
                                velocity = (float)Integer.parseInt( JFvelocity.getText() );
                                epsilon = (float)(Integer.parseInt( JFepsilon.getText() )/100.0f);
                                delta = (float)(Integer.parseInt( JFdelta.getText() )/100.0f);
                        }else{
                                velocity = 0.0f;
                                epsilon = 0.0f;
                                delta = 0.0f;
                              }
                                System.out.println("nx = "+NX);
                                System.out.println("nz = "+NZ);
                                System.out.println("dx = "+DX);
                                System.out.println("dz = "+DZ);
                                System.out.println("sx = "+SX);
                                System.out.println("sz = "+SZ);
                                System.out.println("nt = "+NT);
                                System.out.println("dt = "+DT);
                                System.out.println("fm = "+FM);
                                System.out.println("npd = "+npd);
                                System.out.println("mm = "+MM);
                        if(!flag_SelectFile){
                                System.out.println("velocity = "+velocity);
                                System.out.println("epsilon = "+epsilon);
                                System.out.println("delta = "+delta);
                               }
                                new myXmovie1OrderPMLJFrame(velocity,epsilon,delta,
                                                SX,SZ,NX,NZ,DX,DZ,NT,DT,FM,MM,npd,
                                                filev,filee,filed,flag_SelectFile,flag_VTI,
                                                Sp, Sq, SV, Snap);
                              }
                    });
                toolbar0.add(new JLabel("   "));
                toolbar0.add(buttonOK);
                layout3.row().left().add(js1).fill().withOwnRowWidth();
                layout3.row().grid().add(panel1);
                layout3.row().left().add(js2).fill().withOwnRowWidth();
                layout3.row().grid().add(panel2);
                panel.add(toolbar0);
                panel.add(toolbar01);
                panel.add(toolbar02);
                panel.add(toolbar03);
                panel.add(toolbar04);
                //panel.add(panel3);
                add(panel, BorderLayout.NORTH);
                add(panel3, BorderLayout.CENTER);
                pack();
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
                
                new myXmovie1OrderPMLParasJFrame();
        }
}
//a#############################################################################################
//a##
//a##                   saveJPanel
//a##
//a############################################################################################# 
class saveJPanel{
        static void saveImage(JPanel panel, JMenu parent) {
        /*https://stackoverflow.com/questions/19621105/save-image-from-jpanel-after-draw */
                BufferedImage img = new BufferedImage(panel.getWidth(), 
                                                      panel.getHeight(), BufferedImage.TYPE_INT_RGB);
                panel.paint(img.getGraphics());
                JFileChooser jFile = new JFileChooser();
                jFile.showSaveDialog(parent);
                Path pth = jFile.getSelectedFile().toPath();
                JOptionPane.showMessageDialog(parent, pth.toString());
                try {
                        ImageIO.write(img, "png", new File(pth.toString()));
                        System.out.println("panel saved as image");
                } catch (Exception e) {
                        System.out.println("panel not saved" + e.getMessage());
                    }
          }
}
```
主界面如下图（右图为工具栏中的About窗口）：
![](https://img-blog.csdn.net/20180130124359881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
效果图如下（可以通过工具栏的save按钮保存为图片）：
![](https://img-blog.csdn.net/20180130124459891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
