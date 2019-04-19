# Java判断字符串是否为纯数字，如果不是则报错的GUI - Koma Hub - CSDN博客
2018年01月25日 19:26:59[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：186
个人分类：[Java																[Java AWT/Swing																[GUI](https://blog.csdn.net/Rong_Toa/article/category/7517387)](https://blog.csdn.net/Rong_Toa/article/category/7157478)](https://blog.csdn.net/Rong_Toa/article/category/7156200)
```java
import java.awt.BorderLayout;   
import javax.swing.InputVerifier;   
import javax.swing.JComponent;   
import javax.swing.JFrame;   
import javax.swing.JTextField;  
import javax.swing.JOptionPane; 
import javax.swing.event.DocumentListener;
import javax.swing.event.DocumentEvent;
class Demo {   
        public static void main(String args[]) {   
                JFrame frame = new JFrame("Verifier Sample");   
                frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);   
                final JTextField textField1 = new JTextField();   
                textField1.getDocument().addDocumentListener (new DocumentListener() {  
                        @Override  
                        public void insertUpdate(DocumentEvent e) {  
                                if(isNumeric(textField1.getText())){
                                        System.out.println("num");
                                }else{
                                         System.out.println("str");
                                        JOptionPane.showMessageDialog(textField1, "Please Input Title.",
                                                   "Warning", JOptionPane.WARNING_MESSAGE);
                                         }
                                }  
                        @Override  
                        public void removeUpdate(DocumentEvent e) {}   
                        @Override  
                        public void changedUpdate(DocumentEvent e) {}        
                    });
  
                frame.add(textField1, BorderLayout.NORTH);   
 
                frame.setSize(300, 100);   
                frame.setVisible(true);   
          }  
        public static boolean isNumeric(String str){
                for (int i = 0; i < str.length(); i++){
                        System.out.println(str.charAt(i));
                        if (!Character.isDigit(str.charAt(i))){
                                return false;
                              }
                      }
                return true;
          }
} 
/*
                                JOptionPane.showMessageDialog(textField1, "Please Input Title.",
                                                   "Warning", JOptionPane.WARNING_MESSAGE);
textField_1.getDocument().addDocumentListener (new DocumentListener() {  
    @Override  
    public void insertUpdate(DocumentEvent e) {  
                              
    }  
    @Override  
    public void removeUpdate(DocumentEvent e) {}  
    @Override  
    public void changedUpdate(DocumentEvent e) {  
                              
    }        
});   
*/
```
![](https://img-blog.csdn.net/20180125192614427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20180125192618575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20180125192624560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
