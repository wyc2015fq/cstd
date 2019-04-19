# java中button背景图片大小设置，JFrame.getContentPane().add(pane)与JFrame.add(pane)区别 - BlueSky - CSDN博客
2015年03月08日 11:24:15[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1795
给按钮添加背景图片，如何调整背景图片的大小，纠结了很久，终于在网上找到了一个解决方法，拿出来和大家分享一下：
- publicvoid setIcon(String file, JButton iconButton) {  
-         ImageIcon icon = new ImageIcon(file);  
-         Image temp = icon.getImage().getScaledInstance(iconButton.getWidth(),  
-                 iconButton.getHeight(), icon.getImage().SCALE_DEFAULT);  
-         icon = new ImageIcon(temp);  
-         iconButton.setIcon(icon);  
-     }  
其中，file是图片的路径，iconButton是按钮的变量名。封装成一个函数就是这样简单。
或者
ImageIcon image = new ImageIcon("C:\\1.jpg");
 image.setImage(image.getImage().getScaledInstance(400,200,Image.SCALE_DEFAULT));
**JFrame.getContentPane().add(pane)与JFrame.add(pane)区别**
当你创建一个JFrame的时候JFrame jf = new JFrame();
在构造方法JFrame()内部会给jf默认添加一个rootPane
所以执行完JFrame jf = new JFrame();这句话之后jf上面已经添加了一个默认的rootpanel了
然后你再调用jf.add(panel) 这个时候,panel和rootPane是平级的
理由:1,你可以读源代码 ,查看构造方法怎么写的
2,或者你可以测试一下,分别执行
jf.setBackground(Color.blue);
jf.getContentPane().setBackground(Color.black);
这两句代码,看看效果(实际上上面一句并不能改变界面的背景色,下面一句才可以,因为rootPane把jf给挡住了,上面一句是改变了jf的背景色,但是你眼睛看到的并不是jf,其实是rootPane.)
另外
jf.getContentPane()==jf.getRootPane().getContentPane()
上面的比较返回的true
所以你调用jf.getContentPane().add(panel) 其实是把panel添加到rootPane上面了 这个时候panel和rootPane就不是平级的了
