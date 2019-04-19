# Qt绘图——QPixmap的使用 - xqhrs232的专栏 - CSDN博客
2017年10月12日 10:12:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3048
原文地址::[http://blog.csdn.net/kongdefei5000/article/details/11694463](http://blog.csdn.net/kongdefei5000/article/details/11694463)
相关文章
1、QT中Qpixmap的清除和保存问题 ----[http://bbs.csdn.net/topics/391882069?page=1](http://bbs.csdn.net/topics/391882069?page=1)
QPixmap 的使用
     直接给个例子：
**[cpp]**[view
 plain](http://blog.csdn.net/kongdefei5000/article/details/11694463#)[copy](http://blog.csdn.net/kongdefei5000/article/details/11694463#)
[print](http://blog.csdn.net/kongdefei5000/article/details/11694463#)[?](http://blog.csdn.net/kongdefei5000/article/details/11694463#)
-        QPixmap pix(16,16); //作为绘图设备
- QPainter painter(&pix); //创建一直画笔
- painter.fillRect(0,0,16,16.Qt::black);//在pix上绘制正方形
     扩展：
    制作颜色选择框时可以利用上面绘制的正方形作为图标,效果如下图：
![](https://img-blog.csdn.net/20130915000823093)
其实想做出这个效果很简单,代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/kongdefei5000/article/details/11694463#)[copy](http://blog.csdn.net/kongdefei5000/article/details/11694463#)
[print](http://blog.csdn.net/kongdefei5000/article/details/11694463#)[?](http://blog.csdn.net/kongdefei5000/article/details/11694463#)
- 
-  QComboBox *comBox;  
-  QPixmap pix(16,16); //创建绘图设备   
-  QPainter painter(&pix);//创建一个画笔    
-  painter.fillRect(0,0,16,16,Qt::black);      
-  comBox->addItem(QIcon(pix),tr("黑色"),Qt::black);      
- //红色    
-  painter.fillRect(0,0,16,16,Qt::red);      
-  comBox->addItem(QIcon(pix),tr("红色"),Qt::red);      
- //green    
-  painter.fillRect(0,0,16,16,Qt::green);      
-  comBox->addItem(QIcon(pix),tr("绿色"),Qt::green);      
- //blue    
-  painter.fillRect(0,0,16,16,Qt::blue);      
-  comBox->addItem(QIcon(pix),tr("蓝色"),Qt::blue);      
- //yellow    
-  painter.fillRect(0,0,16,16,Qt::yellow);      
-  comBox->addItem(QIcon(pix),tr("黄色"),Qt::yellow);      
- //cyan    
-  painter.fillRect(0,0,16,16,Qt::cyan);     
-  comBox->addItem(QIcon(pix),tr("蓝绿色"),Qt::cyan);      
- //洋红    
-  painter.fillRect(0,0,16,16,Qt::magenta);      
-  comBox->addItem(QIcon(pix),tr("洋红"),Qt::magenta);  
- 
# QT中题
