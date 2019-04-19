# Qt--Http请求封装(Get Post) - xqhrs232的专栏 - CSDN博客
2018年09月06日 15:36:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：249
原文地址::[https://blog.csdn.net/wangshubo1989/article/details/52588938](https://blog.csdn.net/wangshubo1989/article/details/52588938)
相关文章
1、Qt使用http简单实现请求----[https://blog.csdn.net/One_L_Star/article/details/80639941](https://blog.csdn.net/One_L_Star/article/details/80639941)
之前使用c++开发的程序都是使用curl库进行各种http操作。
qt为我们继承了一些网络操作的类，因此不需要第三方库的支持了。今天就跟大家分享一下qt中对http请求的封装。
其中用到了： 
**QNetworkRequest**
The QNetworkRequest class holds a request to be sent with QNetworkAccessManager. 
[http://doc.qt.io/qt-4.8/qnetworkrequest.html#details](http://doc.qt.io/qt-4.8/qnetworkrequest.html#details)
**QNetworkAccessManager**
The QNetworkAccessManager class allows the application to send network requests and receive replies. 
[http://doc.qt.io/qt-5/qnetworkaccessmanager.html#details](http://doc.qt.io/qt-5/qnetworkaccessmanager.html#details)
**QNetworkReply**
The QNetworkReply class contains the data and headers for a request sent with QNetworkAccessManager. 
[http://doc.qt.io/qt-5/qnetworkreply.html#details](http://doc.qt.io/qt-5/qnetworkreply.html#details)
这里不再过多的介绍上面三个类，因为qt的文档描述的非常非常的清楚。
开始写我们自己的http api：
写一个基类，命名为**BaseAPI**: 
baseapi.h
```
#ifndef BASEAPI_H
#define BASEAPI_H
#include <QSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "base/constants.h"
class BaseAPI : public QObject
{
    Q_OBJECT
public:
    BaseAPI();
    ~BaseAPI();
    void get(const QString url);
    void post(const QString url, const QByteArray &data);
protected:
    virtual void requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode) = 0;
public slots:
    void serviceRequestFinished(QNetworkReply *reply);
private:
    QNetworkRequest httpRequest;
    QNetworkAccessManager networkAccessManager;
    QSettings *settings;
};
#endif // BASEAPI_H
```
baseapi.cc
```
#include "baseapi.h"
BaseAPI::BaseAPI()
{
    httpRequest.setRawHeader("Accept", API_ACCEPT);
    httpRequest.setRawHeader("User-Agent", API_USER_AGENT);
    httpRequest.setRawHeader("X-XXX-API-Key", API_KEY);
    httpRequest.setRawHeader("X-XXX-API-Secret", API_SECRET);
    httpRequest.setRawHeader("Accept-Encoding", "gzip, deflate");
    httpRequest.setRawHeader("Content-Type", "application/json");
    settings = new QSettings("XXX");
    QString id = settings->value(SETTING_ACCOUNT_ID, "").toString();
    QString token = settings->value(SETTING_ACCOUNT_TOKEN, "").toString();
    if(!id.isEmpty()) {
        httpRequest.setRawHeader("X-XXX-User-ID", id.toStdString().c_str());
    }
    if (!token.isEmpty()) {
        httpRequest.setRawHeader("X-XXX-User-Token", token.toStdString().c_str());
    }
    qDebug() << "BaseAPI...id:" << id << " token:" + token;
    QObject::connect(&networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
}
BaseAPI::~BaseAPI()
{
    networkAccessManager.disconnect();
    if (settings){
        delete settings;
        settings = nullptr;
    }
}
void BaseAPI::get(const QString url)
{
    httpRequest.setUrl(QUrl(url));
    networkAccessManager.get(httpRequest);
}
void BaseAPI::post(const QString url, const QByteArray &data)
{
    httpRequest.setUrl(QUrl(url));
    networkAccessManager.post(httpRequest, data);
}
void BaseAPI::serviceRequestFinished(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "BaseAPI...serviceRequestFinished...statusCode:" << statusCode;
    if(reply->error() == QNetworkReply::NoError) {
        requestFinished(reply, reply->readAll(), statusCode);
    } else {
        requestFinished(reply, "", statusCode);
    }
    // At the end of that slot, we won't need it anymore
    reply->deleteLater();
}
```
解释： 
1 使用了qt中的QSettings，就是一个存储功能 
The QSettings class provides persistent platform-independent application settings 
[http://doc.qt.io/qt-4.8/qsettings.html](http://doc.qt.io/qt-4.8/qsettings.html)
2 声明了一个纯虚函数，使得子类根据自己的情况进行重写
基类就这样写完了，下面写一个应用，比如从服务器请求游戏列表： 
写一个子类**GameAPI**继承自BaseAPI： 
gameapi.h
```
#ifndef GAMEAPI
#define GAMEAPI
#include <functional>
#include <QList>
#include <QPair>
#include "baseapi.h"
class GameAPI: public BaseAPI
{
public:
    GameAPI();
    ~GameAPI();
    void getGameList(std::function<void(bool, QList<QPair<QString,QString>>)> callback);
protected:
    void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);
private:
    std::function<void(bool, QList<QPair<QString,QString>>)> checkCallback;
};
#endif // GAMEAPI
```
gameapi.cc
```
#include "gameapi.h"
#include "qdebug.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
const QString GET_GAME = QString("https://www.xxx.com").append("/apps");
GameAPI::GameAPI()
{
}
GameAPI::~GameAPI()
{
}
void GameAPI::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
{
    if (statusCode == 200) {
        QJsonParseError jsonError;
        QJsonArray json_array = QJsonDocument::fromJson(data, &jsonError).array();
        if(jsonError.error == QJsonParseError::NoError) {
            QList<QPair<QString,QString>> games;
            for(int i = 0; i < json_array.size(); ++i) {
                   QJsonObject json = json_array.at(i).toObject();
                   games.append(qMakePair(json.value("key1").toString(), json.value("key2").toString()));
            }
            this->checkCallback(true, games);
            return;
        }
    }
    QList<QPair<QString,QString>> games;
    this->checkCallback(false, games);
}
void GameAPI::getGameList(std::function<void(bool, QList<QPair<QString,QString>>)> callback)
{
    this->checkCallback = callback;
    get(GET_GAME);
}
```
使用接口：
```
GameAPI game_api = new GameAPI();
    game_api->getGameList([&](bool success, QList<QPair<QString,QString>> games){
        if (success)
        {
           //do something
        }
    });
```
