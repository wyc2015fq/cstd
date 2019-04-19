# QT：如何获取鼠标坐标点和键盘响应事件 - xqhrs232的专栏 - CSDN博客
2017年10月11日 11:40:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5633
原文地址::[http://blog.sina.com.cn/s/blog_9c4b67b00102vdcd.html](http://blog.sina.com.cn/s/blog_9c4b67b00102vdcd.html)
相关文章
1、QT中鼠标位置获取问题之event->globalPos()与pos()函数----[http://blog.csdn.net/xuejie_n/article/details/42364997](http://blog.csdn.net/xuejie_n/article/details/42364997)
voidMainWindow::mouseMoveEvent(QMouseEvent*m){QCursormy(QPixmap("D:/123.png"));QApplication::setOverrideCursor(my);intx=m->pos().x();inty=m->pos().y();ui->pushButton->setText(tr("mousepos(%1,%2),yesit'sright").arg(x).arg(y));ui->pushButton->move(m->pos());}voidMainWindow::keyPressEvent(QKeyEvent*k){if(k->key()==Qt::Key_A){ui->label->setPixmap(QPixmap("D:/321.png"));ui->label->resize(100,100);}}
