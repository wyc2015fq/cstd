# Qt 设置某项Windows服务的启动方式 - DumpDoctorWang的博客 - CSDN博客





2017年07月24日 17:27:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1350








原理实际就是调用Windows系统管理服务的程序sc.exe， 然后给出参数。

头文件

```
#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>

#define mDebug(x)     (qDebug()    << "Debug   " + QString(__DATE__).replace(" ",".") + " " + QString(__TIME__) + " " + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x)

class ServiceController : public QObject
{
    Q_OBJECT

public:
    ServiceController(QString serviceName);
    typedef enum{Auto,Manual,Disabled,Delayed_Auto} StartType;//自动、手动、禁用、自动启动(延迟) 
#ifdef Q_OS_WIN
    void start();
    void stop();
    void setStartType(StartType type);
#endif

private:
    QString serviceName;
    QProcess *process;

private slots:
    void readReult();//读取运行结果

};

#endif // SERVICECONTROLLER_H
```

源文件

```
#include "servicecontroller.h"

ServiceController::ServiceController(QString serviceName):
    serviceName(serviceName)
{
    process = new QProcess();
    connect(process,SIGNAL(readyRead()),this,SLOT(readReult()));
}

#ifdef Q_OS_WIN
/**
 * @brief ServiceController::setStartType 设置服务的启动类型（需要管理员权限）
 * @param type 服务启动类型
 */
void ServiceController::setStartType(StartType type){
    QStringList args;
    args<<"config"<<serviceName<<"start=";
    switch (type) {
    case Auto:
        process->start("sc",args<<"auto");
        break;
    case Manual:
        process->start("sc",args<<"demand");
        break;
    case Disabled:
        process->start("sc",args<<"disabled");
        break;
    case Delayed_Auto:
        process->start("sc",args<<"delayed-auto");
        break;
    default:
        break;
    }
    process->waitForFinished();
}

void ServiceController::start(){
    process->start("sc",QStringList()<<"start"<<serviceName);
    process->waitForFinished();
}

void ServiceController::stop(){
    process->start("sc",QStringList()<<"stop"<<serviceName);
    process->waitForFinished();
}
#endif

void ServiceController::readReult(){
    QByteArray bytes = process->readAll();
    QTextCodec *gbk = QTextCodec::codecForName("gb2312");
    QString data = gbk->toUnicode(bytes);
    if(!data.isEmpty()){
        mDebug(data);
    }
}
```

调用方法

```
//首先包含头文件
        #include "servicecontroller.h"
        //调用
        ServiceController service("Apache2.4");
        service.setStartType(ServiceController::Manual) //手动启动
```



