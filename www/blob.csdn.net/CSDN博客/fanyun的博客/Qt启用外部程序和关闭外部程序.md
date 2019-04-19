# Qt启用外部程序和关闭外部程序 - fanyun的博客 - CSDN博客
2018年02月25日 22:11:39[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2771
    在我们的多方合做的项目中，经常会遇到需要调用第三方外部程序。在Qt中提供了QProcess类，可以用来启动和关闭外部程序，并与之通讯。QProcess继承于QIODevice，因此，我们可以把它当作是一个I/O设备进行读写操作。
    具体用法：
1.一体式启动：外部程序启动后，将随着主程序的退出而退出
    启动方法：
   void QProcess::start(const QString & program, const QStringList & arguments, OpenMode mode = ReadWrite)
2.分离式启动：外部程序先启动，启动后再启动外部程序，主程序退出时，该程序继续运行。
     启动方法：
  void QProcess::startDetached(const QString & program, const QStringList & arguments, const QString & workingDirectory = QString(), qint64 * pid = 0)
3.实际调用：
      process->start("D:/mypro/data/Rose.exe");
      QProcess::start()和QProcess::execute()都能完成启动外部程序的任务，但是区别很大，start()是非阻塞的，而execute()是阻塞的。execute()=start()+waitforFinished()。
