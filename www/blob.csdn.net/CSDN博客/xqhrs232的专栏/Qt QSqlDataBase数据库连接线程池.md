# Qt QSqlDataBase数据库连接线程池 - xqhrs232的专栏 - CSDN博客
2018年05月28日 16:13:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：141
原文地址::[https://blog.csdn.net/lwwl12/article/details/76124210](https://blog.csdn.net/lwwl12/article/details/76124210)
相关文章
1、Qt远程连接SQlServer数据库----[https://blog.csdn.net/mingxia_sui/article/details/7759826](https://blog.csdn.net/mingxia_sui/article/details/7759826)
一般可用下面的函数创建和取得数据库连接：
```
void createConnectionByName(const QString &connectionName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName("127.0.0.1");
    db.setDatabaseName("qt"); // 如果是 SQLite 则为数据库文件名
    db.setUserName("root");   // 如果是 SQLite 不需要
    db.setPassword("root");   // 如果是 SQLite 不需要
    if (!db.open()) {
        qDebug() << "Connect to MySql error: " << db.lastError().text();
        return;
    }
}
QSqlDatabase getConnectionByName(const QString &connectionName) {
    return QSqlDatabase::database(connectionName);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
##### 虽然抽象出了连接的创建和获取，但是有几个弊端：
- 需要维护连接的名字
- 获取连接的时候需要传入连接的名字
- 获取连接的时候不知道连接是否已经被使用，使用多线程的时候，每个线程都必须使用不同的连接
- 控制连接的最大数量比较困难，因为不能在程序里无限制的创建连接
- 连接断了后不会自动重连
- 删除连接不方便
这一节我们将创建一个简易的数据库连接池，就是为了解决上面的几个问题。使用数据库连接池后，只需要关心下面 3 个函数，而且刚刚提到的那些弊端都通过连接池解决了，对调用者是透明的。
|功能|代码|
|----|----|
|获取连接|QSqlDatabase db = ConnectionPool::openConnection()|
|释放连接|ConnectionPool::closeConnection(db)|
|关闭连接池|ConnectionPool::release() // 一般在 main() 函数返回前调用|
数据库连接池的使用
在具体介绍数据库连接池的实现之前，先来看看怎么使用。
```cpp
```cpp
#include "ConnectionPool.h"
#include <QDebug>
void foo() {
    // 1. 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    // 2. 使用连接查询数据库
    QSqlQuery query(db);
    query.exec("SELECT * FROM user where id=1");
    while (query.next()) {
        qDebug() << query.value("username").toString();
    }
    // 3. 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}
int main(int argc, char *argv[]) {
    foo();
    ConnectionPool::release(); // 4. 释放数据库连接
    return 0;
}
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
## 数据库连接池的特点
- 获取连接时不需要了解连接的名字
- 支持多线程，保证获取到的连接一定是没有被其他线程正在使用
- 按需创建连接
- 可以创建多个连接
- 可以控制连接的数量
- 连接被复用，不是每次都重新创建一个新的连接
- 连接断开了后会自动重连
- 当无可用连接时，获取连接的线程会等待一定时间尝试继续获取，直到超时才会返回一个无效的连接
- 关闭连接很简单
## 数据库连接池的实现
数据库连接池的实现只需要 2 个文件：`ConnectionPool.h` 和 `ConnectionPool.cpp`。下面会列出文件的内容加以介绍。
`ConnectionPool.h`
```cpp
```cpp
#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
class ConnectionPool {
public:
    static void release(); // 关闭所有的数据库连接
    static QSqlDatabase openConnection();                 // 获取数据库连接
    static void closeConnection(QSqlDatabase connection); // 释放数据库连接回连接池
    ~ConnectionPool();
private:
    static ConnectionPool& getInstance();
    ConnectionPool();
    ConnectionPool(const ConnectionPool &other);
    ConnectionPool& operator=(const ConnectionPool &other);
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接
    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名
    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL
    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数
    static QMutex mutex;
    static QWaitCondition waitConnection;
    static ConnectionPool *instance;
};
#endif // CONNECTIONPOOL_H
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- `openConnection()` 用于从连接池里获取连接。
- `closeConnection(QSqlDatabase connection)` 并不会真正的关闭连接，而是把连接放回连接池复用。连接的底层是通过 Socket 来通讯的，建立 Socket 连接是非常耗时的，如果每个连接都在使用完后就给断开 Socket 连接，需要的时候再重新建立 Socket连接是非常浪费的，所以要尽量的复用以提高效率。
- `release()` 真正的关闭所有的连接，一般在程序结束的时候才调用，在 main() 函数的 return 语句前。
- `usedConnectionNames` 保存正在被使用的连接的名字，用于保证同一个连接不会同时被多个线程使用。
- `unusedConnectionNames` 保存没有被使用的连接的名字，它们对应的连接在调用 `openConnection()` 时返回。
- 如果 `testOnBorrow` 为 true，则连接断开后会自动重新连接（例如数据库程序崩溃了，网络的原因等导致连接断开了）。但是每次获取连接的时候都会先查询一下数据库，如果发现连接无效则重新建立连接。`testOnBorrow` 为 true 时，需要提供一条 SQL 语句用于测试查询，例如 MySQL 下可以用 `SELECT 1`。如果 `testOnBorrow` 为 false，则连接断开后不会自动重新连接。需要注意的是，Qt 里已经建立好的数据库连接当连接断开后调用 QSqlDatabase::isOpen() 返回的值仍然是 true，因为先前的时候已经建立好了连接，Qt 里没有提供判断底层连接断开的方法或者信号，所以 QSqlDatabase::isOpen() 返回的仍然是先前的状态 true。
- `testOnBorrowSql` 为测试访问数据库的 SQL，一般是一个非常轻量级的 SQL，如 `SELECT 1`。
- 获取连接的时候，如果没有可用连接，我们的策略并不是直接返回一个无效的连接，而是等待 `waitInterval` 毫秒，如果期间有连接被释放回连接池里就返回这个连接，没有就继续等待 `waitInterval` 毫秒，再看看有没有可用连接，直到等待 `maxWaitTime` 毫秒仍然没有可用连接才返回一个无效的连接。
- 因为我们不能在程序里无限制的创建连接，用 `maxConnectionCount` 来控制创建连接的最大数量。
`ConnectionPool.cpp`
```cpp
```cpp
#include "ConnectionPool.h"
#include <QDebug>
QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;
ConnectionPool::ConnectionPool() {
    // 创建数据库连接的这些信息在实际开发的时都需要通过读取配置文件得到，
    // 这里为了演示方便所以写死在了代码里。
    hostName     = "127.0.0.1";
    databaseName = "qt";
    username     = "root";
    password     = "root";
    databaseType = "QMYSQL";
    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";
    maxWaitTime  = 1000;
    waitInterval = 200;
    maxConnectionCount  = 5;
}
ConnectionPool::~ConnectionPool() {
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}
ConnectionPool& ConnectionPool::getInstance() {
    if (NULL == instance) {
        QMutexLocker locker(&mutex);
        if (NULL == instance) {
            instance = new ConnectionPool();
        }
    }
    return *instance;
}
void ConnectionPool::release() {
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}
QSqlDatabase ConnectionPool::openConnection() {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;
    QMutexLocker locker(&mutex);
    // 已创建连接数
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for (int i = 0;
         i < pool.maxWaitTime
         && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
         i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);
        // 重新计算已创建连接数
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }
    if (pool.unusedConnectionNames.size() > 0) {
        // 有已经回收的连接，复用它们
        connectionName = pool.unusedConnectionNames.dequeue();
    } else if (connectionCount < pool.maxConnectionCount) {
        // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        // 已经达到最大连接数
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }
    // 创建连接
    QSqlDatabase db = pool.createConnection(connectionName);
    // 有效的连接才放入 usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
    }
    return db;
}
void ConnectionPool::closeConnection(QSqlDatabase connection) {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();
    // 如果是我们创建的连接，从 used 里删除，放入 unused 里
    if (pool.usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}
QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);
        if (testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);
            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }
        return db1;
    }
    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);
    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
为了支持多线程，使用了 QMutex，QWaitCondition 和 QMutexLocker 来保护共享资源 usedConnectionNames 和 unusedConnectionNames 的读写。
在构造函数里初始化访问数据库的信息和连接池的配置，为了方便所以都硬编码写在了代码里，实际开发的时候这么做是不可取的，都应该从配置文件里读取，这样当它们变化后只需要修改配置文件就能生效，否则就需要修改代码，然后编译，重新发布等。虚构函数里真正的把所有连接和数据库断开。
`ConnectionPool` 使用了 Singleton 模式，保证在程序运行的时候只有一个对象被创建，`getInstance()` 用于取得这个唯一的对象。按理说使用 openConnection() 的方法在 Singleton 模式下的调用应该像这样 ~~ConnectionPool::getInstance().openConnection()~~，但是我们实现的却是 `ConnectionPool::openConnection()`，因为我们把 `openConnection()` 也定义成静态方法，在它里面调用 `getInstance()` 访问这个对象的数据，这样做的好处即使用了 Singleton 的优势，也简化了 `openConnection()` 的调用。
调用 `ConnectionPool::release()` 会删除 `ConnectionPool` 唯一的对象，在其虚构函数里删除所有的数据库连接。
##### `openConnection()` 函数相对比较复杂，也是 `ConnectionPool` 的核心
- 如果没有可复用连接 `pool.unusedConnectionNames.size() == 0` 且已经创建的连接数达到最大，则等待，等待期间有连接被释放回连接池就复用这个连接，如果超时都没有可用连接，则返回一个无效的连接 `QSqlDatabase()`。
- 如果没有可复用连接，但是已经创建的连接数没有达到最大，那么就创建一个新的连接，并把这个连接的名字添加到 `usedConnectionNames`。
- 如果有可复用的连接，则复用它，把它的名字从 `unusedConnectionNames` 里删除并且添加到 `usedConnectionNames`。
##### `createConnection()` 是真正创建连接的函数
- 如果连接已经被创建，不需要重新创建，而是复用它。`testOnBorrow` 为 true 的话，返回这个连接前会先用 SQL 语句 `testOnBorrowSql` 访问一下数据库，没问题就返回这个连接，如果出错则说明连接已经断开了，需要重新和数据库建立连接。
- 如果连接没有被创建过，才会真的建立一个新的连接。
##### `closeConnection()` 并不是真的断开连接
- 需要判断连接是否我们创建的，如果不是就不处理。
- 把连接的名字从 `usedConnectionNames` 里删除并放到 `unusedConnectionNames` 里，表示这个连接已经被回收，可以被复用了。
- 唤醒一个等待的线程，告诉它有一个连接可用了。
## 测试
`[测试](http://lib.csdn.net/base/softwaretest)用例`：连接池允许最多创建 5 个连接，我们启动 10 个线程用连接池里获取连接访问数据库。
`ConnectionTestThread.h`
```cpp
```cpp
#ifndef CONNECTIONTESTTHREAD_H
#define CONNECTIONTESTTHREAD_H
#include <QThread>
class ConnectionTestThread : public QThread {
protected:
    void run();
};
#endif // CONNECTIONTESTTHREAD_H
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
`ConnectionTestThread.cpp`
```cpp
```cpp
#include "ConnectionTestThread.h"
#include "ConnectionPool.h"
void ConnectionTestThread::run() {
    // 从数据库连接池里取得连接
    QSqlDatabase db = ConnectionPool::openConnection();
    qDebug() << "In thread run():" << db.connectionName();
    QSqlQuery query(db);
    query.exec("SELECT * FROM user where id=1");
    while (query.next()) {
        qDebug() << query.value("username").toString();
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
`main.cpp`
```cpp
```cpp
#include "ConnectionTestThread.h"
#include "ConnectionPool.h"
#include <QApplication>
#include <QPushButton>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton *button = new QPushButton("Access Database");
    button->show();
    QObject::connect(button, &QPushButton::clicked, []() {
        for (int i = 0; i < 10; ++i) {
            ConnectionTestThread *thread = new ConnectionTestThread();
            thread->start();
        }
    });
    int ret = a.exec();
    ConnectionPool::release(); // 程序结束时关闭连接，以免造成连接泄漏
    return ret;
}
```
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
执行程序，点击按钮 `Access Database`，输出如下：
> 
In thread run(): Connection-1
Alice
In thread run(): Connection-2
Alice
In thread run(): Connection-3
Alice
In thread run(): Connection-4
Alice
In thread run(): Connection-5
Test connection on borrow, execute: SELECT 1 , for Connection-1
Alice
In thread run(): Connection-1
Test connection on borrow, execute: SELECT 1 , for Connection-2
Alice
In thread run(): Connection-2
Test connection on borrow, execute: SELECT 1 , for Connection-3
Alice
In thread run(): Connection-3
Test connection on borrow, execute: SELECT 1 , for Connection-4
Alice
In thread run(): Connection-4
Test connection on borrow, execute: SELECT 1 , for Connection-5
Alice
In thread run(): Connection-5
Alice
可以看到，前 5 个连接是新创建的，后面 5 个连接复用了已经创建的连接。
可以再做一下几个测试，看看连接池是否都能正确的运行。
##### Case 1
- 点击按钮 `Access Database`，正常输出。
- 然后关闭数据库，点击按钮 `Access Database`，应该提示连不上数据库。
- 启动数据库，点击按钮 `Access Database`，正常输出。
##### Case 2
- 把线程数增加到 100 个，1000 个。
- 同时测试关闭和再次打开数据库。
##### Case 3
- 在线程的 run() 函数里随机等待一段时间，例如 0 到 100 毫秒。
数据库连接池基本已经完成，但是并不是很完善。考虑一下如果我们设置最大连接数为 100，高峰期访问比较多，创建满了 100 个连接，但是当闲置下来后可能只需要 2 个连接，其余 98 个连接都不长时间不用，但它们一直都和数据库保持着连接，这对资源（Socket 连接）是很大的浪费。需要有这样的机制，当发现连接一段时间没有被使用后就把其关闭，并从 `unusedConnectionNames` 里删除。还有例如连接被分配后没有释放回连接池，即一直在 `usedConnectionNames` 里面，即连接泄漏，超过一定时间后连接池应该主动把其回收。怎么实现这些的功能，这里就不在一一说明，大家独自思考一下应该怎么实现这些功能。
