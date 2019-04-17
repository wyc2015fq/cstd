# Qt应用程序单实例化 - DoubleLi - 博客园






在实际应用中，我们经常需要让应用程序只有一个实例，再打开新的文档或者页面时，只是替换现在的窗口或者新打开一个标签，而不是重新启动一次应用程序。Qt中是否可以做到这样呢，答案是肯定的，因为Qt本身可以直接调用系统API，肯定可以做到，但是我们希望找到一个[跨平台](https://www.baidu.com/s?wd=%E8%B7%A8%E5%B9%B3%E5%8F%B0&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的通用的解决方案。

      这就要用到Qt的QLocalSocket，QLocalServer类了，这两个类从接口上看和网络通信socket没有区别，但是它并不是真正的网络API，只是模仿了而已。这两个类在Unix/Linux系统上采用Unix域socket实现，而在Windows上则采用有名管道（named pipe）来实现。

      既然是网络API，那么思路就很简单了，应用程序启动时首先会去连一个[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)（这里通过应用程序的名字来标识，就像网络端口一样），如果连接失败，那么则自己是第一个实例，就创建这么一个[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，否则将启动参数发送到[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，然后自动退出，而服务器会在收到通知以后进行处理。

      这些动作我想最好是放在创建Application实例后，因为Qt本身有很多操作没有Application实例是无法进行操作的（至少事件循环是在创立Application以后才能启动吧），因此最好的位置就是通过继承QApplicaiton或者QCoreApplication自定义一个YourOwnApplication，然后在构造函数中进行，下面是一个示意。

首先是YourOwnApplication构造函数：




QString serverName = QCoreApplication::applicationName();

    QLocalSocket socket;

    socket.connectToServer(serverName);

    if (socket.waitForConnected(500)) { //如果能够连接得上的话，将参数发送到服务器，然后退出

        QTextStream stream(&socket);

        QStringList args = QCoreApplication::arguments();

        if (args.count() > 1)

            stream << args.last();

        else

            stream << QString();

        stream.flush();

        socket.waitForBytesWritten();

qApp->quit();

        return;

}

//运行到这里，说明没有实例在运行，那么创建服务器。

    m_localServer = new QLocalServer(this);

    connect(m_localServer, SIGNAL(newConnection()),

            this, SLOT(newLocalSocketConnection())); //监听新到来的连接

    if (!m_localServer->listen(serverName)) {

        if (m_localServer->serverError() == QAbstractSocket::AddressInUseError

            && QFile::exists(m_localServer->serverName())) { //确保能够监听成功

            QFile::remove(m_localServer->serverName());

            m_localServer->listen(serverName);

        }

    }




    这样就保证了新启动的程序在检测到有其他实例在运行时就会自动退出，但是它发出的请求还没有被处理，下面看一下处理函数，也就是前段代码中的newLocalSocketConnection()。


    QLocalSocket *socket = m_localServer->nextPendingConnection();

    if (!socket)

        return;

    socket->waitForReadyRead(1000);

    QTextStream stream(socket);

   … … //其他处理

    delete socket;

    mainWindow()->raise();

    mainWindow()->activateWindow(); //记得激活窗口哦










