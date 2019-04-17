# Qt技巧：QProcess与外部程序的调用 - DoubleLi - 博客园






项目做到一定阶段，常常须要在原来的project上调用外部程序。



Qt为此提供了QProcess类，QProcess可用于完毕启动外部程序，并与之交互通信。

一、启动外部程序的两种方式：
（1）一体式：void QProcess::start(const QString & program, const QStringList & arguments, OpenMode mode = ReadWrite)
        外部程序启动后。将随主程序的退出而退出。
（2）分离式：void QProcess::startDetached(const QString & program, const QStringList & arguments, const QString & workingDirectory = QString(), qint64 * pid = 0)
        外部程序启动后，当主程序退出时并不退出。而是继续执行。



    值得注意的是。当程序路径中含有空格的时候，仅设置程序路径program是无法被识别的。解决方法是再将程序路径传递给arguments。


比如：






- //1-不带空格。能够启动
- process->start("F:/AppInst/115/115com/115com.exe");
- 
- //2-带空格，无法启动
- process->start("C:/Program Files/HaoZip/HaoZip.exe");
- 
- //3-带空格，使用带參模式。能够启动
- process->start("C:/Program Files/HaoZip/HaoZip.exe", QStringList("C:/Program Files/HaoZip/HaoZip.exe"));




二、启动之前须要做的工作：
    To start a process, pass the name and command line arguments of the program you want to run as arguments to start(). Arguments are supplied as individual strings in a QStringList.
    启动一个外部程序，须要传递外部程序的路径和运行參数。參数用QStringList来带入。




（1）设置路径：          void QProcess::setProgram(const QString & program)
（2）设置參数[可选]： void QProcess:: setArguments(const QStringList & arguments)
（3）启动：                void QProcess::start(OpenMode mode = ReadWrite)

Qt官方自带的样例：

- QObject *parent;
- ...
- QString program = "./path/to/Qt/examples/widgets/analogclock";
- QStringList arguments;
- arguments << "-style" << "fusion";
- 
- QProcess *myProcess = new QProcess(parent);
- myProcess->start(program, arguments);



    事实上。start()或startDetached()已经整合了上面的三个步骤。假设须要分别设置，请採用以上的方式。

三、启动状态
    外部程序未启动时。其状态是NotRunning；
    当启动时，其状态转变为Starting，正在启动。但此时还未调用起来。
    启动之后，继续变为Running，同一时候发射出started()信号，此时，能够对QProcess进行读写操作了。
    当退出时，其状态改为NotRunning。并发射出finished()信号。finishe()信号会携带退出码和退出状态，能够分别通过exitCode()和exitStatus()来获得。
    当错误发生时，QProcess会发出一个error()信号。相同的。也能够通过error()来获得其错误类型，通过state()获得当前程序的状态。

四、QProcess 与 QIODevice
    QProcess继承于QIODevice，因此。我们能够把它当作是一个I/O设备进行读写操作。

五、交互
    QProcess有两种提前定义的输出通道：标准输出stdout与标准错误stderr。
    通过setReadChannel()能够选择当前读取输出的通道。
    当通道中的数据准备就绪时。QProcess会发出readyRead()信号。假设是标准输出。则发出readyReadStandardOutput()信号。假设是标准错误，则发出readyReadStandardError()信号。
    经常使用的读取方式有read(), readAll()或getChar()，也能够通过readAllStandardOutput()和readAllStandardError()读取标准输出和标准错误通道中的数据。





    某些程序须要环境设置才干进行特殊的操作。能够通过setEnvironment()来环境变量设置，通过setWorkingDirectory()来设置工作文件夹，默认的工作路径是当前调用程序的工作路径。

六、Synchronous Process API （同步进程API）
    QProcess提供了一系列的函数以提到事件循环来完毕同步操作：
（1）waitForStarted()          : 堵塞。直到外部程序启动
（2）waitForReadyRead()    : 堵塞，直到输出通道中的新数据可读
（3）waitForBytesWritten()  : 堵塞，直到输入通道中的数据被写入
（4）waitForFinished()        : 堵塞，直到外部程序结束
    假设在主线程（QApplication::exec()）中调用这些函数，可能会造成当前用户界面不响应。

七、主要API
    请參考Qt的官方说明：**[QProcess Class](http://qt-project.org/doc/qt-5/qprocess.html)**


PS：參考链接
[从QProcess说开来(一)](http://blog.csdn.net/dbzhang800/article/details/6876451)
[从QProcess说开来(二)](http://blog.csdn.net/dbzhang800/article/details/6877762)









