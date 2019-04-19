# QThread 线程暂停 停止功能的实现 - xqhrs232的专栏 - CSDN博客
2018年05月28日 17:32:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1678
原文地址::[https://www.cnblogs.com/lvdongjie/p/3757434.html](https://www.cnblogs.com/lvdongjie/p/3757434.html)
为了实现Qt中线程的暂停运行，和停止运行的控制功能
需要在设置两个static型控制变量。
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //终止原始数据单元进队出队，并清空数据。
- staticbool stopSign;  
- //原始数据单元队列 允许进队标识符
- static QMutex enterQueueSign;  
- //原始数据单元队列 允许出队标识符
- static QMutex outQueueSign;  
一个是终止运行的变量 stopSign
一个是暂停进队线程的变量 enterQueueSign
另一个是暂停出队线程的变量outQueueSign
stopSign、enterQueueSign 用在我的入队线程：ImageProcessingThread，其中的Run函数如下：
如果有停止命令，我得15圈原始数据重建的过程被终止，跳出这个循环，进而结束这个线程。
如果有暂停命令，我得enterQueueSign锁，会暂时挂起在这里等待解锁。这个加锁和解锁的操作都在我得控制界面Main.cpp的函数里执行。
ImageProcessingThread的Run函数
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- void ImageProcessingThread::run()    
- {   
-     QTime t;  
- double  duration;  
-     t.start();  
-     qDebug() << "时间1:" << t.currentTime().toString("hh:mm:ss.zzz");  
- //影像数据对象
-     CtImageRecon *ctImageRecon = new CtImageRecon();  
- //图像重建进度
-     connect(ctImageRecon,SIGNAL(updateReconJobInfo(QList<QVariant>)),this,SLOT(updateReconJob(QList<QVariant>)));  
- //原始数据单元队列
-     RawUnitQueue rawUnitQueue;  
-     qDebug() << "==>>ImageProcessingThread<<==影像数据处理开始,线程Id:" << QThread::currentThreadId();  
- 
- for( int i = 0; i < 15;i++)  
-     {  
- //终止了影像处理
- if(RawUnitQueue::stopSign)  
- break;  
- //是否允许原始数据单元队列 出队
-         RawUnitQueue::outQueueSign.lock();  
- //是否存在已用空间
-         RawUnitQueue::rawTurnsUsedSpace.acquire();  
- //CT重建
-         ctImageRecon->recon(reconJob);  
-         sleep(3);  
-         rawUnitQueue.removeFirstRawTurn();  
-         RawUnitQueue::rawTurnsFreeSpace.release();  
-         qDebug() << "ImageProcessingThread:" << i;    
-         RawUnitQueue::outQueueSign.unlock();  
-     }  
- 
-     qDebug() << "==>>ImageProcessingThread<<==影像数据处理完毕,线程Id:" << QThread::currentThreadId();      
- 
-     qDebug() << "时间2:" << t.currentTime().toString("hh:mm:ss.zzz");  
-     duration = t.elapsed() ;  
-     qDebug() << "ImageProcessingThread消耗时间:" << duration << "ms";  
- 
- 
- }    
 Main.cpp
暂停影像处理进队线程函数
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- /**
-  *函数介绍:暂停Ct影像重建线程
-  *输入参数:无
-  *返回值:  无
-  */
- void MainWindow::suspendCtImageReconThread()  
- {  
- if(ctImageReconThread->isRunning())  
-     {  
- 
-         ui.startImageReconBtn->setEnabled(true);  
-         ui.startImageReconBtn->setText("继续重建");  
-         ui.suspendImageReconBtn->setEnabled(false);  
- //暂停原始数据单元队列 进队
-         RawUnitQueue::enterQueueSign.lock();  
-         qDebug() << "【命令】暂停原始数据单元队列 进队";  
- //暂停原始数据单元队列 出队
-         RawUnitQueue::outQueueSign.lock();  
-         qDebug() << "【命令】暂停原始数据单元队列 出队";  
- 
-     }  
- }  
停止影像处理进队线程函数
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- /**
-  *函数介绍:停止Ct影像重建线程
-  *输入参数:无
-  *返回值:  无
-  */
- void MainWindow::stopCtImageReconThread()  
- {  
- if(ctImageReconThread->isRunning())  
-     {  
- 
-         ui.startImageReconBtn->setEnabled(true);  
-         ui.startImageReconBtn->setText("开始重建");  
-         ui.suspendImageReconBtn->setEnabled(false);  
-         ui.stopImageReconBtn->setEnabled(false);  
- //终止了影像处理
-         RawUnitQueue::stopSign = true;  
-         ctImageReconThread->quit();  
-         qDebug() << "【命令】停止CT重建线程";  
- 
-     }  
- }  
