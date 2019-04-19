# QSetting读取.ini配置文件----按组读取 - xqhrs232的专栏 - CSDN博客
2018年05月29日 14:46:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：655
原文地址::[https://blog.csdn.net/san_junipero/article/details/79172787](https://blog.csdn.net/san_junipero/article/details/79172787)
# QSetting
在qt上读取.ini配置文件，qt内部已经设计了相关的库，就是QSetting。
# 使用方法
配置文件包括读和写两部分，大同小异
### 读配置文件
```
#include<qsettings.h>
int main()
{
    //设置setting
    QSetting setting(QString::fromStdString(path),QSetting::IniFormat);
    //从配置文件中读全部的Group
    QStringList all = setting.childGroups();
    //此处只拿用一组举例，有多个可以自行加设循环
    setting.beginGroup(all[0]);
    QString p1 = setting.value("para_1").toString();
    string p2 = setting.value("para_2").toString().toStdString();
    setting.endGroup(); 
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
### 写配置文件
```
#include<qsettings.h>
int main()
{
    //设置setting
    QSetting setting(QString::fromStdString(path),QSetting::IniFormat);
    //从配置文件中读全部的Group
    QStringList all = setting.childGroups();
    //此处只拿用一组举例，有多个可以自行加设循环
    setting.beginGroup(all[0]);
    //存数据前要保存成QString形式的
    setting.setValue("para_1",QString::number(number1))
    setting.endGroup(); 
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
### 配置文件
```
[General] ##GroupName
para_1 = 1 #ParameterName
para_2 = 2
[Use] ##Group_2
para_3 = 3
para_4 = 4
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 注意
有时候section读不出来，根据实验发现和section名字有关，section一般取名为：
