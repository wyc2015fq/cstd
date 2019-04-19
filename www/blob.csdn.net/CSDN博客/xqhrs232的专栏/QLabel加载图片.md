# QLabel加载图片 - xqhrs232的专栏 - CSDN博客
2018年07月20日 13:33:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：447
原文地址::[https://blog.csdn.net/mangobar/article/details/52525555](https://blog.csdn.net/mangobar/article/details/52525555)
- 
QRect rect;
- 
MyLabel *pLabel = new MyLabel(ui->scrollAreaWidgetContents);
- 
rect.setX(x);
- 
rect.setY(y);
- 
rect.setWidth(320);
- 
rect.setHeight(320);
- 
pLabel->setGeometry(rect);
- 
- 
QString path = QString(":/new/prefix1/1234.jpg")
- 
- 
QPixmap px;
- 
px.load(path);
- 
pLabel->setPixmap(px);
- 
- 
pLabel->setScaledContents(true);
- 
pLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观
- 
pLabel->setScaledContents(true);
- 
pLabel->setVisible(true);
- 
- 
- 
- 
.
