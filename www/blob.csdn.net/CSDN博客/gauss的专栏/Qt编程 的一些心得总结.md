# Qt编程 的一些心得总结 - gauss的专栏 - CSDN博客
2013年01月03日 18:08:25[gauss](https://me.csdn.net/mathlmx)阅读数：250
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
                
 一. 背景刷成黑色,前景色设为白色。 方法一、paltette方式，经测试，该方法不会影响到其他控件,推荐使用 QPalette bgpal =
- palette();
- 
- bgpal.setColor (QPalette::Background, QColor (0, 0 , 0, 255));
- 
- //bgpal.setColor (QPalette::Background, Qt::transparent);
- 
- bgpal.setColor (QPalette::Foreground, QColor (255,255,255,255)); setPalette (bgpal);
*复制代码*
方法二、stylesheet方式 影响子控件的方法是：
setStyleSheet ("background-color: rgb(0,0,0);color: rgb(255,255,255);");
不影响子控件的方法是：
setStyleSheet ("venus--TitleBar {background-color: rgb(0,0,0);color: rgb(255,255,255);}");
二. 圆角控件 用stylesheet方式
setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
三. 圆角窗口
- RoundRectWin::RoundRectWin() { QPalette p = palette(); QPixmap img("roundrect.png");
- 
-     QBitmap mask("roundrect_mask.png");
- 
-     p.setBrush(QPalette::Window, QBrush(img));
- 
-     setPalette(p);
- 
-     setMask(mask);
- 
-     resize(img.size());
- 
-     //setWindowFlags(Qt::FramelessWindowHint);//这句会去掉标题栏 } 
*复制代码*
注意:mask的图多余部分设为白色
四. 半透明窗口
1.窗口整体透明，但是窗体上的控件不透明。 通过设置窗体的背景色来实现，将背景色设置为全透。 QPalette pal = palette(); 
pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00)); setPalette(pal); 试验效果：
  窗体标题栏不透明；
  窗体客户区上的控件不透明，QLabel控件只是字显示，控件背景色透明； 窗体客户区完全透明。
  另外从网上看到的方法： setAttribute(Qt::WA_TranslucentBackground, true)； 试验的结果是类似于上面的方法，但有时候窗体会被一些杂色斑点填充，未找到原因。
2.窗口及其上面的控件都半透明：
setWindowOpacity(0.7)
试验效果：窗口及控件都半透明。注意不能够setWindowFlags(Qt::FramelessWindowHint);要不就不起作用
3.窗口整体不透明，局部透明：
在Paint事件中使用Clear模式绘图。
- void TestWindow::paintEvent( QPaintEvent* ) 
- 
- { QPainter p(this); 
- 
-                p.setCompositionMode( QPainter::CompositionMode_Clear ); p.fillRect( 10, 10, 300, 300, Qt::SolidPattern ); }
*复制代码*
试验效果：绘制区域全透明。如果绘制区域有控件不会影响控件。
以上实验均是基于Directfb的Qte测试。在Linux下及Windows下效果会略有不同。 比如在Windows下第3个实验，绘制区域就是一个黑窟窿，如果Window本身半透，则是Window下面的内容加上半透效果，不是全透明效果。
五. 控制QPixmap的alpha
- QPixmap temp(pixmapTop.size()); temp.fill(Qt::transparent);
- 
-     QPainter p(&temp);
- 
-     p.setCompositionMode(QPainter::CompositionMode_Source);
- 
-     p.drawPixmap(0, 0, pixmapTop);
- 
-     p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
- 
-     p.fillRect(temp.rect(), QColor(0, 0, 0, alpha)); //--lable显示前景图片 ui->label->setScaledContents(true);
- 
-     ui->label->setPixmap(temp);
*复制代码*
六. layout 的边界 layout->setMargin (0);
            
