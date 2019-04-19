# Java 好看的label-textfield布局 - Koma Hub - CSDN博客
2018年01月19日 15:55:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：373
```java
class myBorderJPanel extends JPanel{
        private TitledBorder titledborder;
        private JPanel panel = new JPanel();
        myBorderJPanel(String title, GridLayout gridlayout){
                titledborder = BorderFactory.createTitledBorder(
                                BorderFactory.createEtchedBorder(
                                  EtchedBorder.LOWERED), title);
                titledborder.setTitleFont(new Font("Georgia",Font.BOLD,20));
                titledborder.setTitleJustification(TitledBorder.LEFT);
                titledborder.setTitlePosition(TitledBorder.DEFAULT_POSITION);
                setBorder(BorderFactory.createEmptyBorder(0,10,10,10));
                panel.setLayout(gridlayout);
                panel.setBorder(titledborder);
                add(panel);
        }
        void addC(Component c){
                c.setFont(new Font("Georgia",Font.BOLD,20));
                panel.add(c);
        }
}
```
使用：
```java
myBorderJPanel panel1 = new myBorderJPanel("rong",new GridLayout(3,2));
panel1.addC(new JLabel("nx ="));
panel1.addC(new JTextField(5));
panel1.addC(new JLabel("nz ="));
panel1.addC(new JTextField(5));
panel1.addC(new JLabel("nz ="));
panel1.addC(new JTextField(5));
```
截图：
![](https://img-blog.csdn.net/20180119155459070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
整体代码展示：
```java
import java.awt.*;
import java.awt.event.*;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.*;
class Demo{
        public static void main(String[]args){
                JFrame frame = new JFrame("BorderDemo");
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                myBorderJPanel panel1 = new myBorderJPanel("File");
                panel1.addC(new JTextField(15),0,0,5,1);
                panel1.addC(new JButton(new ImageIcon("picture/ButtonImg/folder16.png")),5,0,1,1);
                myBorderJPanel panel2 = new myBorderJPanel("");
                panel2.addC(new JLabel("nx"),0,0,1,1);
                panel2.addC(new JTextField(5),1,0,1,1);
                panel2.addC(new JLabel("nz"),0,1,1,1);
                panel2.addC(new JTextField(5),1,1,1,1);
                myBorderJPanel panel3 = new myBorderJPanel("");
                panel3.addC(new JLabel("dx"),0,0,1,2);
                panel3.addC(new JTextField(5),1,0,3,2);
                panel3.addC(new JLabel("dz"),0,2,1,2);
                panel3.addC(new JTextField(5),1,2,3,2);
                myBorderJPanel panel4 = new myBorderJPanel("++++");
                panel4.addC(panel2,0,0,1,1);
                panel4.addC(panel3,1,0,1,1);
                myBorderJPanel panel = new myBorderJPanel("");
                panel1.setAlignmentX(1f);
                panel.addC(panel1,0,0,1,1);
                panel.addC(panel4,0,1,1,1);
                frame.add(panel);
                frame.pack();
                frame.setVisible(true);
        }
}
class myBorderJPanel extends JPanel{
        private TitledBorder titledborder;
        private JPanel panel = new JPanel();
        private GridBagConstraints gc = new GridBagConstraints();
        private GridBagLayout gridbag = new GridBagLayout();
        myBorderJPanel(String title){
                titledborder = BorderFactory.createTitledBorder(
                                BorderFactory.createEtchedBorder(
                                  EtchedBorder.LOWERED), title);
                titledborder.setTitleFont(new Font("Georgia",Font.BOLD,20));
                titledborder.setTitleJustification(TitledBorder.LEFT);
                titledborder.setTitlePosition(TitledBorder.DEFAULT_POSITION);
                setBorder(BorderFactory.createEmptyBorder(6,6,6,6));
                panel.setLayout(gridbag);
                panel.setBorder(titledborder);
                JPanel tmp = new JPanel();
                tmp.add(panel);
                add(tmp);
        }
        void addC(JComponent c, int gridx, int gridy,int gridwidth,int gridheight){
                gc.gridx = gridx; //x grid position
                gc.gridy = gridy; //y grid position
                gc.gridwidth = gridwidth;
                gc.gridheight = gridheight;
                gridbag.setConstraints(c, gc);
                c.setFont(new Font("Georgia",Font.BOLD,25));
                panel.add(c);
        }
}
```
