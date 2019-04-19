# 关于Qt中常用的延时方法--自我总结 - xqhrs232的专栏 - CSDN博客
2018年01月12日 14:15:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：189
原文地址::[http://blog.csdn.net/yanche521/article/details/50977301](http://blog.csdn.net/yanche521/article/details/50977301)
相关文章
1、[Qt阻塞延时与非阻塞延时(四种方法之个人笔记)](http://blog.csdn.net/lusirking/article/details/51728320)----[http://blog.csdn.net/lusirking/article/details/51728320](http://blog.csdn.net/lusirking/article/details/51728320)
## 一般情况下，延时大概分为两类，一个是非阻塞延时，一个是阻塞延时，但从名称上应该都可以看出来具体的含义，下面针对这两类延时方法，做一个具体的说明和代码实现：
#### 一、关于Qt实现非阻塞延时的方法：
void QSleepTimeSet::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
其中的msec为需要延时的毫秒数，例如，你要实现5秒延时，则msec的值就是 ：5000
####  二、关于Qt中的阻塞延时实现方法：
void QSleepTimeSet::Delay_MSec_Suspend(unsigned int msec)
{
    QTime _Timer = QTime::currentTime();
    QTime _NowTimer;
    do{
              _NowTimer=QTime::currentTime();
    }while (_Timer.msecsTo(_NowTimer)<=msec);
}
其中的msec为需要延时的毫秒数，例如，你要实现5秒延时，则msec的值就是 ：5000
大家在具体的编程实践中，可以根据实际需要来决定使用哪一种程序延时方法。
以上方法也是自己的一个总结，给大家分享出来，希望对大家有所帮助。

