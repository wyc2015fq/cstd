# Qt 使用UDPSocket遇到的丢包问题 - xqhrs232的专栏 - CSDN博客
2018年08月15日 15:43:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：361
原文地址::[https://blog.csdn.net/bsbsxll/article/details/40824755](https://blog.csdn.net/bsbsxll/article/details/40824755)
相关文章
1、QT中UDPSocket丢包问题----[https://blog.csdn.net/rabbitjerry/article/details/72674458](https://blog.csdn.net/rabbitjerry/article/details/72674458)
2、QT中UDPSocket丢包问题(续)----[https://blog.csdn.net/rabbitjerry/article/details/72830004](https://blog.csdn.net/rabbitjerry/article/details/72830004)
3、[UDP主要丢包原因及具体问题分析]()----[https://blog.csdn.net/kobejayandy/article/details/35797609](https://blog.csdn.net/kobejayandy/article/details/35797609)
4、解决UDP丢包问题的经验----[https://blog.csdn.net/kristpan/article/details/50553592](https://blog.csdn.net/kristpan/article/details/50553592)
5、Linux网络编程-UDP接收数据丢包解决方案----[https://blog.csdn.net/baidu_35692628/article/details/76165337](https://blog.csdn.net/baidu_35692628/article/details/76165337)
6、Linux UDP严重丢包问题的解决----[https://blog.csdn.net/u013267687/article/details/52594126](https://blog.csdn.net/u013267687/article/details/52594126)
最近烦人的事情很多，所以博客一直被落下了。这样不好，希望可以敦促自己不要懒惰。
前些日子接下了一个撂摊子的项目，这个项目中大量的使用udp socket进行多软件多硬件的来回通讯过程，但说实话通信量不是特别大。但是经常遇到各种各样奇怪的丢包现象。在解决这些问题过程中，也算加强了一些基础知识的学习，在此也顺便记录下解决步骤，以便下次项目中使用。
该项目中软件部分有A、B两个软件。其中A和B都有各自的发送和接受UDP功能。
代码实现很简单：
发送端：
- 
`void UdpServer::send(const QString &message)`
- 
`{`
- 
`qDebug() << "message not null" << message;`
- 
`QByteArray byte;`
- 
`byte.append(message.toAscii());`
- 
`_socket->writeDatagram(byte,mcast_addr,tempport);`
- 
`}`
- 
- 
`UdpServer::UdpServer(QObject *parent):QObject(parent)`
- 
`{`
- 
`_socket = new QUdpSocket();`
- 
`}`
接收端：
- 
`void UdpClient::recv()`
- 
`{`
- 
`while(_socket->hasPendingDatagrams())`
- 
`{`
- 
`QByteArray data;`
- 
`data.resize(_socket->pendingDatagramSize());`
- 
`_socket->readDatagram(data.data(),(qint64)data.size());`
- 
`// QString byteStr = QString(data.toHex());`
- 
`// qDebug() << "此次接受的报文内容是：" << byteStr;`
- 
`if(byteStr.left(4)=="5555"){`
- 
`.....`
- 
`}else if(byteStr.left(4)=="6666"){`
- 
`.....`
- 
`}`
- 
`....`
- 
`}`
- 
`}`
以上是原程序。
刚开始测试的时候，另一程序以每500ms速度发送长度为512长度的报文，发现丢包现象比较严重。开始怀疑是否是因为收包程序while中有大量的逻辑判断逻辑而造成报文并没有及时的接受到。于是新建一个数据缓存池CDataPool类，并继承于Qthread类。接受到报文后，不进行任何处理，直接放至CDataPool类的队列中，然后另起线程进行报文解析操作，代码如下：
CDataPool.h
- 
`/***********************************************************************`
- 
`* Module: CDataPool.h`
- 
`* Author: 振星`
- 
`* Modified: 2014年11月4日星期二 09:57:45`
- 
`* Purpose: Declaration of the class CDataPool`
- 
`* Comment: 数据管理类`
- 
`***********************************************************************/`
- 
- 
`#ifndef CDATAPOOL_H`
- 
`#define CDATAPOOL_H`
- 
`#include <QtCore/qglobal.h>`
- 
`#include <QMutex>`
- 
`#include <QString>`
- 
`#include <QWaitCondition>`
- 
`#include <QThread>`
- 
`#include <QQueue>`
- 
`#include <QSharedPointer>`
- 
- 
`#define CONTAINER_MAX_NUM 10000 // 缓存队列中最大数目`
- 
`typedef struct msghead`
- 
`{`
- 
`unsigned short headFst;`
- 
`unsigned short headSnd;`
- 
`}_MSGHEAD;`
- 
- 
`#if defined(DATAPOOL_LIBRARY)`
- 
`# define DATAPOOLSHARED_EXPORT Q_DECL_EXPORT`
- 
`#else`
- 
`# define DATAPOOLSHARED_EXPORT Q_DECL_IMPORT`
- 
`#endif`
- 
- 
`class DATAPOOLSHARED_EXPORT CDataPool : public QThread {`
- 
`Q_OBJECT`
- 
`public:`
- 
`static CDataPool * instance();`
- 
`~CDataPool();`
- 
`//void insert(char*);`
- 
`bool recv(const QByteArray); // 接收数据并缓存`
- 
`/* 线程停止 */`
- 
`void stop();`
- 
`void init();`
- 
`// 设置报文回发页面`
- 
`void setMark(int);`
- 
`protected:`
- 
`void run(); // 线程`
- 
- 
`signals: `
- 
- 
`private:`
- 
`explicit CDataPool();`
- 
`static CDataPool * m_instance; // 唯一实例`
- 
`bool handleData(); // 处理队列中数据数据`
- 
`// 线程停止`
- 
`volatile bool stopped;`
- 
`QMutex mutex;`
- 
`QWaitCondition queueWait;`
- 
`// 队列数组`
- 
`QQueue<QByteArray> qqByteArr;`
- 
`};`
- 
- 
`#endif // CDATAPOOL_H`
CDataPool.cpp
- 
`CDataPool.cpp`
- 
`/***********************************************************************`
- 
`* Module: CDataPool.cpp`
- 
`* Author: 振星`
- 
`* Modified: 2014年11月4日星期二 09:57:45`
- 
`* Purpose: Declaration of the class CDataPool`
- 
`* Comment: 数据管理类`
- 
`***********************************************************************/`
- 
`#include "cdatapool.h"`
- 
`#include "monitorDefine.h"`
- 
`#include "picDefine.h"`
- 
`#include "QDebug"`
- 
- 
- 
`CDataPool * CDataPool::m_instance = NULL;`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::instance()`
- 
`// Purpose: Implementation of CDataPool::instance()`
- 
`// Comment: 应用实例`
- 
`// Return: static CDataPool *`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`CDataPool * CDataPool::instance()`
- 
`{`
- 
`if ( NULL == m_instance ){`
- 
`m_instance = new CDataPool;`
- 
`}`
- 
`return m_instance;`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::CDataPool()`
- 
`// Purpose: Implementation of CDataPool::CDataPool()`
- 
`// Return:`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`CDataPool::CDataPool()`
- 
`{`
- 
`stopped = false;`
- 
`qqByteArr.clear();`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::handleData`
- 
`// Purpose: 处理数据池中的数据`
- 
`// Comment: 应用实例`
- 
`// Return: -1:fail; >=0:success`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`bool CDataPool::handleData()`
- 
`{`
- 
`if(qqByteArr.length() < 1)`
- 
`return -1;`
- 
`// QMutexLocker locker(&mutex);`
- 
`QByteArray byteArrFirst = qqByteArr.takeFirst();`
- 
`QString byteStr = QString(byteArrFirst.toHex());`
- 
`qDebug() << "即将处理的报文是：" << byteStr;`
- 
- 
`// 为什么这么做？我也不知道，这个是之前的代码，看不懂，很麻烦的判断`
- 
`if( 0x3333 == mh->headFst || 0x4444 == mh->headFst){// `
- 
`....`
- 
`}else if(0x5555 == mh->headFst && 0xAAAA ==mh->headSnd){// `
- 
`switch (mark) {`
- 
`case MARK_1: `
- 
`{`
- 
`.....`
- 
`}`
- 
`break;`
- 
`case MARK_2:`
- 
`{`
- 
`.....`
- 
`}`
- 
`break;`
- 
`case MARK_3:`
- 
`{`
- 
`.....`
- 
`}`
- 
`break;`
- 
`case MARK_4: `
- 
`{`
- 
`.....`
- 
`}`
- 
`break;`
- 
`default:`
- 
`break;`
- 
`}`
- 
- 
`}else{ // 其他处理逻辑`
- 
`if(mark == MARK_1)`
- 
`{`
- 
`.....`
- 
`}`
- 
`else if(mark == MARK_3)`
- 
`{`
- 
`.....`
- 
`}`
- 
`else if(mark == MARK_4)`
- 
`{`
- 
`.....`
- 
`}`
- 
`return 1;`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::~CDataPool()`
- 
`// Purpose: Implementation of CDataPool::~CDataPool()`
- 
`// Return:`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`CDataPool::~CDataPool()`
- 
`{`
- 
`//delete m_instance;`
- 
`QMutexLocker locker(&mutex);`
- 
`SFDelete(mh);`
- 
`qqByteArr.clear();`
- 
`quit();`
- 
`wait();`
- 
`stopped = true;`
- 
`mark = 0;`
- 
`// TODO : implement`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::recv`
- 
`// Purpose: 接收数据并缓存`
- 
`// Parameters: 1. buf:数据首址,2.len ：长度`
- 
`// Comment: 应用实例`
- 
`// Return: -1:fail; >=0:success`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`bool CDataPool::recv(const QByteArray byte)`
- 
`{`
- 
`QMutexLocker locker(&mutex); // 锁定队列对象，禁止同时操作`
- 
`if(qqByteArr.size() >CONTAINER_MAX_NUM){ // 当队列大于限定值时，`
- 
`qqByteArr.removeFirst();`
- 
`}`
- 
- 
`// QString byteStr = QString(byte.toHex());`
- 
`// qDebug() << "放到队列里面的报文是：" << byteStr;`
- 
- 
`qqByteArr.enqueue(byte); // 插入指针到队列尾部`
- 
`queueWait.wakeOne(); // 唤醒睡着中的线程`
- 
`return 1;`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::run()`
- 
`// Purpose: Implementation of CDataPool::run()`
- 
`// Comment: 线程 用于推送解析信号至显示模块`
- 
`// Return: void`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`void CDataPool::run()`
- 
`{`
- 
`forever{`
- 
`QMutexLocker locker(&mutex);`
- 
`if(qqByteArr.isEmpty()){`
- 
`queueWait.wait(&mutex);`
- 
`}else{`
- 
`handleData();`
- 
`}`
- 
`}`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::init()`
- 
`// Purpose: Implementation of CDataPool::init()`
- 
`// Comment: 启动线程`
- 
`// Return: void`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`void CDataPool::init()`
- 
`{`
- 
`if(!stopped){`
- 
`start();`
- 
`}`
- 
`}`
- 
- 
`////////////////////////////////////////////////////////////////////////`
- 
`// Name: CDataPool::setMark(int m)`
- 
`// Purpose: Implementation of CDataPool::setMark(int m)`
- 
`// Comment: 启动线程`
- 
`// Return: void`
- 
`////////////////////////////////////////////////////////////////////////`
- 
`void CDataPool::setMark(int m)`
- 
`{`
- 
`mark = m;`
- 
`}`
CDataPool 是一个单例，同时使用线程在不停的执行报文解析，当有新的报文进来后，唤醒线程，否则线程休眠。
然后修改原来的接受部分程序：
- 
`void UdpClient::recv()`
- 
`{`
- 
`while(_socket->hasPendingDatagrams())`
- 
`{`
- 
`QByteArray data;`
- 
`data.resize(_socket->pendingDatagramSize());`
- 
`_socket->readDatagram(data.data(),(qint64)data.size());`
- 
`// QString byteStr = QString(data.toHex());`
- 
`// qDebug() << "此次接受的报文内容是：" << byteStr;`
- 
`cDataPool->recv(data);`
- 
`}`
- 
`}`
改完以上的程序后，我满心以为，应该不会丢包了吧？使用100ms的发送次数，依然丢包，只是稍微好了一点点。但至少有些进步。大概1000包数据中至少会丢400包。伤心。
通过上述的实践说明，在报文接受过程中，报文解析逻辑比较复杂时，如果在同一线程处理，会对造成一定程度的丢包现象。但绝不是主要原因。
客户几个电话打过来催促解决问题的时间点。无奈继续没有注释阅读代码，突然发现了一个比较有趣的逻辑，很值得分享下。大家都知道到udp报文大于一定值时，将会被截包或者丢包。之前写这个项目的人写了以下的代码：
- 
`foreach (PICINFO info, selectedMoubleInfoList) {`
- 
`QString tempStr = info.module_userId + BACKSLASH`
- 
`+ info.module_FactoryId + BACKSLASH`
- 
`+ mnMap.value(info.module_userId);`
- 
`if(updUseData.size() + tempStr.size() < maxUpdlength){`
- 
`updUseData += tempStr + COLON;`
- 
`}else{`
- 
`updUseDataList.append(updUseData);`
- 
`updUseData = STR_EAMPTY;`
- 
`updUseData += tempStr + COLON;`
- 
`}`
- 
`selectedMoubleList.append(info.module_FactoryId);`
- 
`} `
- 
- 
`if(updUseData.size() > 0){`
- 
`updUseDataList.append(updUseData);`
- 
`}`
- 
- 
`if(updUseDataList.size() > 0){`
- 
`ui->progressBar->setRange(0,selectedMoubleInfoList.size());`
- 
- 
`//字符>1000的场合，分段截取`
- 
`foreach (QString str, updUseDataList) {`
- 
- 
`int index = str.lastIndexOf(COLON);`
- 
- 
`QString sendStr = useCode + DOLLAR + QString::number(selectedMoubleInfoList.size())+ COMMA`
- 
`+ doConnParamStr(paramMap)`
- 
`+ str.left(index);`
- 
- 
`udpServer->send(sendStr);`
- 
- 
`}`
上面的代码主要是将要发送的报文拼接成字符串时，按照1000的长度进行分割，分割完毕后在发送到socket中。想法是好的，但是问题就来了，他在一个for循环中发送报文，这个速度可不是100ms或者其他值，可能都是微秒级别的。
我很想将上面的代码改动下，但是发现几乎所有的类全部都关联这个里面，改动实在太大，所以我使用了一个临时解决方法在foreach循环中加上Sleep(100).
这个操作完成后，惊喜很大，在发送频率为150ms的时候，就从没有出现丢包现象，但是如果设置成100ms，基本还是会丢10包左右。
于是继续寻找可能出现的问题，基本上所有接受和发送的地方我都检查过，依然不知道为什么会丢包。后来打电话问了某研究所的博士，他对我说了一个可能的原因，就是如果两个软件部署在同一台机器上自发自收，可能会因为socket缓存池的满的问题造成上述丢包现象。于是将两个软件分开部署后，即使调整至50ms的发送频率也不会出现丢包现象。不过调整socket的缓存大小目前我还没有去实现，毕竟已经可以和客户说原因了。
这篇文章应该没有什么技术含量，但是却记录着一些解决问题的方法和思路，希望可以互相共享并且请大牛们指点不足。谢谢
