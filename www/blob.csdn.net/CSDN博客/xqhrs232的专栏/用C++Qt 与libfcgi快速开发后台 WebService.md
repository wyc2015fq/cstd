# 用C++Qt 与libfcgi快速开发后台 WebService - xqhrs232的专栏 - CSDN博客
2018年09月07日 11:05:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：173
原文地址::[https://blog.csdn.net/goldenhawking/article/details/69672429](https://blog.csdn.net/goldenhawking/article/details/69672429)
相关文章
1、Qt：终于来了！基于QML和WebGL开发网页端----[https://blog.csdn.net/wsj18808050/article/details/78791001/](https://blog.csdn.net/wsj18808050/article/details/78791001/)
2、如何在自己的计算机上做一个web服务器----[https://blog.csdn.net/zzwu/article/details/44116919](https://blog.csdn.net/zzwu/article/details/44116919)
3、[Qt 之 WebSockets]()----[https://blog.csdn.net/liang19890820/article/details/52573443](https://blog.csdn.net/liang19890820/article/details/52573443)
4、QT Websocket实现服务器客户端通信（客户端部分）----[https://blog.csdn.net/qq_39573345/article/details/80682232](https://blog.csdn.net/qq_39573345/article/details/80682232)
在与APP接口的后台WebService开发方面，估计很少有人直接使用C接口的libfcgi-dev进行开发的了。但是，这不代表此方法是不可行的。在强大的Qt库的支持下，原来使用C++开发webService也是非常方便的。这里我们以获取OpenStreetMap数据库中的地理信息为例子，看看现代C++的威力。 
项目地址： 
[https://code.csdn.net/goldenhawking/query_osm/tree/master](https://code.csdn.net/goldenhawking/query_osm/tree/master)
# 1 需求
我们有一个OpenStreetMap瓦片服务器数据库，现在希望在提供瓦片服务的基础上，提供根据地理位置获取附近物体、根据物体名称查询位置、根据地理位置获取高程海拔等功能，输出采用JSON格式。 
数据库是这样的： 
1、瓦片服务器位于postgresql 数据库gis里，包括四个表，planet_osm_line, planet_osm_point, planet_osm_polygon与planet_osm_roads; 
2、高程数据位于postgresql数据库contour里，包括陆地海拔等高线 planet_osm_line 表、海洋深度等值线 contour表。 
希望提供简单的URL接口（？&传值），输出JSON格式的数据。我们直接在OpenStreetMap宿主服务器上开发，操作系统为ArchLinux，工具链为 apache2 + libfcgi + Qt5
# 2 FCGI框架搭建
我们希望在几个独立的线程中响应用户的请求。因此，采用异步FCGI模式，设计几个QThread派生类的对象负责具体的事物处理。
# 2.1 Qt-Pro文件
Qt的工程文件如下，为控制台程序。
```
QT += core sql
QT -= gui
CONFIG += c++11
TARGET = query_osm.fcgi
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    listenthread.cpp
LIBS += -lfcgi
HEADERS += \
    listenthread.h
```
工程总共就3个问价，一个主函数入口main.cpp，外加事物线程类listenthread的声明与实现。
# 2.2 主函数
主函数负责启动事物线程，并等待程序结束：
```
#include <QCoreApplication>
#include <QList>
#include <fcgi_stdio.h>
#include "listenthread.h"
using namespace std;
const int thread_count = 4; //根据电脑性能自行调整
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //初始化CGI库
    FCGX_Init();
    //初始化事物线程
    QList<listenThread *> threadpool;
    for (int i=0;i<thread_count;++i)
        threadpool.push_back(new listenThread(&a));
    //开始处理事物
    foreach (listenThread * t, threadpool)
        t->start();
    //循环、等待结束。
    int alives = 0;
    do
    {
        alives = 0;//统计目前仍然活跃的事物线程
        foreach (listenThread * t, threadpool)
        {
            if (t->isRunning())
            {
                ++alives;
                t->wait(200);
            }
            else
                QThread::msleep(200);
            a.processEvents();//维护主线程消息循环
        }
    }while (alives>0);
    a.quit();
    return 0;
}
```
# 2.3 事物线程基本结构
事物线程为一个QThread派生类，声明listenthread.h如下：
```
#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H
#include <QHash>
#include <QThread>
#include <QJsonObject>
#include <functional>
#include <fcgi_stdio.h>
class listenThread : public QThread
{
    Q_OBJECT
public:
    explicit listenThread(QObject *parent = 0);
private:
    QHash <QString, std::function< void (const QHash < QString, QString> query_paras, QJsonObject & jsonObj) > >
    m_functions;
    QString m_threadDBName;
protected:
    void run();
    void deal_client(FCGX_Request * request);
    //各个功能处理函数
    void func_help(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );
};
#endif // LISTENTHREAD_H
```
说明： 
1. 我们的一个fcgi入口可以提供很多种功能，这个框架仅包含一个“帮助”功能。 
2. 每增加一个功能，只要增加一个功能处理函数即可。 
3. 功能处理函数的接口有两个。一个是输入的变量query_paras，代表了用户URL里包含的内容。另一个是输出变量 jsonObj ，用于存储输出的内容。 
4. 成员变量 m_threadDBName 用来存储和线程对应的数据库连接名称。Qt中，每个线程必须使用自己的数据库连接。
该类的实现如下：
```
#include "listenthread.h"
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QUrl>
#include <QVariant>
listenThread::listenThread(QObject *parent)
    : QThread(parent)
    , m_threadDBName(QString("TDB%1").arg((quint64(this))))
{
    //注册方法，这里是“帮助”方法
    m_functions["help"] = std::bind(&listenThread::func_help,this,std::placeholders::_1,std::placeholders::_2);
}
//重载的QThread::run()，用于事务处理的总接口 
void listenThread::run()
{
    //1.连接数据库，这里是一个，可以多个。
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",m_threadDBName+"_gis");
        if (db.isValid()==false)
            return;
        db.setHostName("127.0.0.1");
        db.setPort(5432);
        db.setUserName("XXX");
        db.setPassword("XXX");
        db.setDatabaseName("gis");
        if (db.open()==false)
            return;
    }
    //2.开始不断接受请求
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    int rc = FCGX_Accept_r(&request);
    while (rc >=0)
    {
        //2.1调用处理客户端的方法listenThread::deal_client
        deal_client(&request);
        FCGX_Finish_r(&request);
        rc = FCGX_Accept_r(&request);
    }
    //3.退出
    QSqlDatabase::removeDatabase(m_threadDBName+"_gis");
    QSqlDatabase::removeDatabase(m_threadDBName+"_contours");
    quit();
}
//具体处理客户端的逻辑, 这个函数无需改动
void listenThread::deal_client(FCGX_Request * request)
{
    QHash < QString, QString> query_paras;
    //1. 获得输入变量,存储在 FCGX_Request 里
    const char * const query_string=FCGX_GetParam("QUERY_STRING",request->envp);
    QString str = QString::fromUtf8(query_string) ;
    QStringList lst = str.split("&",QString::SkipEmptyParts);
    //2. 生成输入变量字典
    foreach (QString pai, lst)
    {
        int pd = pai.indexOf("=");
        if (pd>0 && pd < pai.length())
        {
            QString key = pai.left(pd);
            QString v = pai.mid(pd+1);
            query_paras[key.trimmed()]  = v;
        }
    }
    //3. 获得公共变量
    //3.1. indented参数控制结果显示时，是否缩进Json
    const bool bJsonIndented = query_paras["indented"].toInt()?true:false;
    //3.2. function参数指定具体功能
    const QString functionStr = query_paras["function"];
    //4. 生成结果对象
    QJsonObject root;
    //4.1 根据功能继续操作，查找给定的具体功能有没有对应的接口，有的话就调用
    if (m_functions.contains(functionStr))
        m_functions[functionStr](query_paras,root);
    //4.2 没有的话显示帮助
    else
        func_help(query_paras,root);
    //5. 输出到客户端
    QJsonDocument doc(root);
    QByteArray arrJson = doc.toJson(bJsonIndented?QJsonDocument::Indented:QJsonDocument::Compact);
    //Output
    FCGX_PutS("Content-type: text/plain; charset=UTF-8\n\n",request->out);
    FCGX_PutStr(arrJson.constData(),arrJson.length(),request->out);
}
//框架提供的帮助接口
void listenThread::func_help(const QHash < QString, QString> query_paras, QJsonObject & jsonObj )
{
    foreach (QString s, query_paras.keys())
        jsonObj[s] = query_paras[s];
    jsonObj["usage"] = "Please put your help message here.";
}
```
说明： 
1. 核心思想是使用了std::functional 的函数绑定，使得接口可以存储在字典中，便于扩展。 
2. 该框架理论上可以扩展任意功能。增加新的功能只需要三步： 
(1) 在头文件里添加一个接口处理入口
`void func_foo(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );`
(2) 在CPP里注册接口
`m_functions["foo"] = std::bind(&listenThread::func_foo,this,std::placeholders::_1,std::placeholders::_2);`
(3) 实现具体功能
```
void listenThread::func_foo(const QHash < QString, QString> query_paras, QJsonObject & jsonObj )
{
}
```
# 2.4 测试调用效果
把fcgi拷贝到apache文件夹下，
```
$sudo systemctl  restart httpd
$sudo cp ./query_osm.fcgi /var/www/html/cgi-bin
```
而后访问
`http://192.168.1.10:8088/cgi-bin/query_osm.fcgi?function=help&indented=1`
返回：
```
{
    "function": "help",
    "indented": "1",
    "usage": "Please put your help message here."
}
```
# 3 具体实现功能
有了框架，我们来具体实现三个功能。
# 3.1 增加接口声明
我们向listenthread.h增加三个接口，分别为altitude、object_by_pos, object_by_name
```
//各个功能函数
    void func_help(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );
    //新增的
    void func_altitude(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );
    void func_object_by_pos(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );
    void func_object_by_name(const QHash < QString, QString> query_paras, QJsonObject & jsonObj );
```
# 3.2 注册接口
我们在listenthread.cpp中注册接口：
```
//注册方法
    m_functions["help"] = std::bind(&listenThread::func_help,this,std::placeholders::_1,std::placeholders::_2);
    //新增加的
    m_functions["altitude"] = std::bind(&listenThread::func_altitude,this,std::placeholders::_1,std::placeholders::_2);
    m_functions["object_by_pos"] = std::bind(&listenThread::func_object_by_pos,this,std::placeholders::_1,std::placeholders::_2);
    m_functions["object_by_name"] = std::bind(&listenThread::func_object_by_name,this,std::placeholders::_1,std::placeholders::_2);
```
# 3.3 实现接口
以func_object_by_name为例：
```
void listenThread::func_object_by_name(const QHash < QString, QString> query_paras, QJsonObject & jsonObj )
{
    //1. 首先产生运行结果字段
    jsonObj["result"] = "error";
    //2. 把输入参数原本不懂地作为输出
    foreach (QString s, query_paras.keys())
        jsonObj[s] = query_paras[s];
    //3. 检查是否给定了待查名称字段"name"
    if (query_paras.contains("name")==false)
    {
        jsonObj["reason"] = "need name element.";
        return;
    }
    //4. 获得待查字段，如果有中文，会是封装格式（%Hex），直接调用QUrl解码
    QString rawnamestr = jsonObj["name"].toString();
    jsonObj["raw_name"] = rawnamestr;
    QUrl url(rawnamestr);
    QString namestring = url.toDisplayString();
    //5. 清除非法字符，防止注入
    namestring.remove(QRegExp("[\\pP‘’“”,\\+\\-()[\\]\\^%~`\\!]"));
    namestring = namestring.trimmed();
    jsonObj["name"] = namestring;
    //6. 长度限制
    if (namestring.length()<2)
    {
        jsonObj["reason"] = "name must contain more than 1 characters.";
        return;
    }
    //7.开始查询数据库
    QSqlDatabase db = QSqlDatabase::database(m_threadDBName+"_gis");
    if (db.isOpen()==false)
    {
        jsonObj["reason"] = "Database connection is not ok.";
        jsonObj["error_msg"] = db.lastError().text();
        return;
    }
    //7.1 生成Sql
    QSqlQuery query(db);
    QString str = QString("select * from ... where name like '%1%%';")
                        .arg(namestring);
    //7.2执行
    if (query.exec(str)==false)
    {
        jsonObj["reason"] = "database query error.";
        jsonObj["error_msg"] = query.lastError().text();
        return;
    }
    //7.3 返回结果，直接利用数据库字段名作为json键
    int nItems = 0;
    while (query.next())
    {
        QJsonObject objitem;
        int cols = query.record().count();
        for (int i=0;i<cols;++i)
            objitem[query.record().fieldName(i)] = query.value(i).toString();
        jsonObj[QString("result%1").arg(nItems)] = objitem;
        ++nItems;
    }
    //8.返回总结果数。
    jsonObj["items"] = nItems;
    jsonObj["result"] = "succeeded";
}
```
# 3.4 测试接口
输入
`http://192.168.1.10:8088/cgi-bin/query_osm.fcgi?function=object_by_name&name=%E4%B8%AD%E5%9B%BD%E5%9C%B0%E8%B4%A8%E5%A4%A7%E5%AD%A6&indented=1`- 1
输出
```
{
    "function": "object_by_name",
    "indented": "1",
    "items": 17,
    "name": "中国地质大学",
    "raw_name": "%E4%B8%AD%E5%9B%BD%E5%9C%B0%E8%B4%A8%E5%A4%A7%E5%AD%A6",
    "result": "succeeded",
    "result0": {
        "center_pos": "POINT(113.940106140169 22.5320149618608)",
        "geotype": "ST_Polygon",
        "name": "中国地质大学产学研基地",
        "osm_id": "220880942",
        "trans_name_chs": ""
    },
    "result1": {
        "center_pos": "POINT(116.33961555325 39.9909730291633)",
        "geotype": "ST_Polygon",
        "name": "中国地质大学校医院",
        "osm_id": "436059504",
        "trans_name_chs": ""
    },
    ...
    "result17": {
        "center_pos": "POINT(114.251718450378 30.5881765656673)",
        "geotype": "ST_Polygon",
        "name": "中国地质大学(汉口校区)",
        "osm_id": "132730572",
        "trans_name_chs": ""
    }
}
```
# 4 体会
其实，webService 可以理解为基于字符串的输出输出处理。理论上，只要一种语言的字符串处理能力很强，就适合做WebService。 
以前，C++/FCGI比较麻烦，是因为C++本身的字符串处理实在有点那啥，而C++的JSON类也良莠不齐。 
不过，有了Qt后，C++对字符串、JSON、XML可就不瘸腿啦！主要有： 
1. 正则表达式与QString的原生契合； 
2. QUrl以及內建的QLocale对字符编码的转换； 
3. QByteArray及Base-64编码解码； 
4. QJson基于类似map的键值操作、无限嵌套； 
5. QJson\QVariant的“伪”动态语言特性，使得对类型转换有了保证； 
6. Qt库的强大能力，包括对硬件、媒体的控制，使得Webservice可以完成几乎所有的事情！ 
有了这些，再加上现代C++的functional/bind特性，使得可以一劳永逸的制作WebService接口框架了。
