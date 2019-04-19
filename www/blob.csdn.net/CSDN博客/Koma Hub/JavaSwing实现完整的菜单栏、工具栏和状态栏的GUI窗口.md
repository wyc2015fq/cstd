# JavaSwing实现完整的菜单栏、工具栏和状态栏的GUI窗口 - Koma Hub - CSDN博客
2017年09月07日 20:02:20[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：7303
```java
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JPopupMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JToolBar;
import java.awt.Toolkit;
import java.awt.Dimension;
import javax.swing.JCheckBoxMenuItem;
import java.awt.BorderLayout;
import javax.swing.BorderFactory;
import javax.swing.border.EtchedBorder;
import java.awt.GridLayout;
class myJFrame extends JFrame{
        private Toolkit toolkit;
        myJFrame(String myJFrameName){
                setTitle(myJFrameName);
                setSize(600, 500);
                toolkit = getToolkit();
                Dimension size = toolkit.getScreenSize();
                setLocation((size.width/2 - getWidth())/2, (size.height -getHeight())/2);
                setDefaultCloseOperation(EXIT_ON_CLOSE);              
        }//myJFrame
}
class myJMenuBar extends JMenuBar{
        private JLabel statusbar;
        myJMenuBar(myJFrame frame){
       
                JMenu file = new JMenu("File");
                file.setMnemonic(KeyEvent.VK_F);
               
                ImageIcon New = new ImageIcon("pic/new.jpg");
                JMenuItem fileNew = new JMenuItem("New", New);
                fileNew.setMnemonic(KeyEvent.VK_N);
                fileNew.setToolTipText("New File");
               
                ImageIcon Open = new ImageIcon("pic/open.jpg");
                JMenuItem fileOpen = new JMenuItem("Open", Open);
                fileOpen.setMnemonic(KeyEvent.VK_O);
                fileOpen.setToolTipText("Open File");
               
                ImageIcon Close = new ImageIcon("pic/close.jpg");
                JMenuItem fileClose = new JMenuItem("Close", Close);
                fileClose.setMnemonic(KeyEvent.VK_C);
                fileClose.setToolTipText("Exit");
                fileClose.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.exit(0);
                              }
                    });
               
                JMenu imp = new JMenu("Import");
                imp.setMnemonic(KeyEvent.VK_M);
                JMenuItem newsf = new JMenuItem("Import newsfeed list...");
                JMenuItem bookm = new JMenuItem("Import bookmarks...");
                JMenuItem mail = new JMenuItem("Import mail...");
                imp.add(newsf);
                imp.add(bookm);
                imp.add(mail);
                file.add(fileNew);
                file.add(fileOpen);
                file.addSeparator();
                file.add(imp);
                file.addSeparator();
                file.add(fileClose);
                add(file);
       
       
                JMenu edit = new JMenu("Edit");
                edit.setMnemonic(KeyEvent.VK_E);
               
                ImageIcon Copy = new ImageIcon("pic/copy.jpg");
                JMenuItem editCopy = new JMenuItem("Copy", Copy);
                editCopy.setMnemonic(KeyEvent.VK_C);
                editCopy.setToolTipText("Copy");
               
                ImageIcon Paste = new ImageIcon("pic/paste.jpg");
                JMenuItem editPaste = new JMenuItem("Paste", Paste);
                editPaste.setMnemonic(KeyEvent.VK_V);
                editPaste.setToolTipText("Paste");
               
                ImageIcon Cut = new ImageIcon("pic/cut.jpg");
                JMenuItem editCut = new JMenuItem("Cut", Cut);
                editCut.setMnemonic(KeyEvent.VK_X);
                editCut.setToolTipText("Cut");
                edit.add(editCopy);
                edit.add(editPaste);
                edit.add(editCut);
                add(edit);
       
       
                JMenu view = new JMenu("View");
                view.setMnemonic(KeyEvent.VK_V);
                JCheckBoxMenuItem sbar = new JCheckBoxMenuItem("Show StatuBar");
                sbar.setState(true);
                sbar.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                if (statusbar.isVisible()) {
                                        statusbar.setVisible(false);
                                } else {
                                        statusbar.setVisible(true);
                                        }
                              }
                    });
                view.add(sbar);
                add(view);
       
                statusbar = new JLabel(" Status: Runing !");
                //statusbar.setText("nimabi");
                statusbar.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
                frame.add(statusbar, BorderLayout.SOUTH);
                frame.setJMenuBar(this);
        }//myJMenuBar
}
class myJPopupMenu extends JPopupMenu{
        myJPopupMenu(myJFrame frame){
                JMenuItem menuItemBeep = new JMenuItem("Beep");
                menuItemBeep.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                System.out.println("Beep");
                              }
                    });
                add(menuItemBeep);
                JMenuItem menuItemClose = new JMenuItem("Close");
                menuItemClose.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                System.exit(0);
                              }
                    });
                add(menuItemClose);
                frame.addMouseListener(new MouseAdapter() {
                        public void mouseReleased(MouseEvent e) {
                                if (e.getButton() == e.BUTTON3) {
                                        show(e.getComponent(), e.getX(), e.getY());
                                        }
                              }
                    });
        }//myJPopupMenu
}
class myJToolBars extends JPanel{
        myJToolBars(myJFrame frame){
                JToolBar toolbar1 = new JToolBar();
                JToolBar toolbar2 = new JToolBar();
                //setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
                setLayout(new GridLayout(1,2));
                ImageIcon newi = new ImageIcon( getClass().getResource("pic/new.jpg"));
                ImageIcon open = new ImageIcon( getClass().getResource("pic/open.jpg"));
                ImageIcon save = new ImageIcon( getClass().getResource("pic/save.jpg"));
                ImageIcon exit = new ImageIcon( getClass().getResource("pic/exit.jpg"));
                JButton newb = new JButton(newi);
                JButton openb = new JButton(open);
                JButton saveb = new JButton(save);
                toolbar1.add(newb);
                toolbar1.add(openb);
                toolbar1.add(saveb);
                toolbar1.setAlignmentX(0);
                JButton exitb = new JButton(exit);
                toolbar2.add(exitb);
                toolbar2.setAlignmentX(0);
                exitb.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent event) {
                                System.exit(0);
                              }
                    });
                add(toolbar1);
                add(toolbar2);
                frame.add(this, BorderLayout.NORTH);
        }//myJToolBar
}
class Demo{
        public static void main(String[]args){
                myJFrame frame = new myJFrame("frame");
                myJMenuBar menubar = new myJMenuBar(frame);
                myJPopupMenu popupmenu = new myJPopupMenu(frame);
                myJToolBars toolbars = new myJToolBars(frame);
                frame.setVisible(true); 
        }//main
}
```
![](https://img-blog.csdn.net/20170907200210526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
