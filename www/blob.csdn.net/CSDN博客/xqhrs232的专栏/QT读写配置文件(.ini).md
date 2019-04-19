# QT读写配置文件(.ini) - xqhrs232的专栏 - CSDN博客
2018年04月22日 22:19:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：231
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/anonymousrookie/article/details/47843057](https://blog.csdn.net/anonymousrookie/article/details/47843057)
相关文章
1、[Qt读写INI配置文件](http://www.cnblogs.com/chuncn/archive/2012/06/14/2549329.html)----[http://www.cnblogs.com/chuncn/archive/2012/06/14/2549329.html](http://www.cnblogs.com/chuncn/archive/2012/06/14/2549329.html)
2、Qt读写ini文件----[https://blog.csdn.net/qiurisuixiang/article/details/7760828](https://blog.csdn.net/qiurisuixiang/article/details/7760828)
INI
INI文件格式是某些平台或软件上的配置文件的非正式标准，以节(section)和键(key)构成，常用于微软Windows操作系统中。INI文件是Initialization File的缩写，即初始化文件。除了windows现在很多其他操作系统下面的应用软件也有.ini文件，用来配置应用软件以实现不同用户的要求。一般不用直接编辑这些.ini文件，应用程序的图形界面即可操作以实现相同的功能。INI文件被用来对操作系统或特定程序初始化或进行参数设置。
文件格式
节(section)
节用方括号括起来，单独占一行，例如：
[section]
键(key)
键(key)又名属性(property)，单独占一行用等号连接键名和键值，例如：
name=value
QT读写INI文件
Qt4.0一个非常有用的类QSettings，在windows平台，它提供了ini文件读写，注册表读写的功能。而且使用也非常简单。
QSettings::Format有两种:
QSettings::NativeFormat在windows平台可以读写windows注册表；
QSettings::IniFormat可以读写ini格式的配置文件。

[cpp][view
 plain](https://blog.csdn.net/anonymousrookie/article/details/47843057#)[copy](https://blog.csdn.net/anonymousrookie/article/details/47843057#)
- //读取配置文件
- void Dialog::on_pushButton_load_clicked()  
- {  
-     QString iniFilePath = "F:/QT_Project_201508/TestConfigFile/config_files/MyINI.ini";  
-     QSettings settings(iniFilePath,QSettings::IniFormat);  
- 
-     QString G1_path = settings.value("Group1/Path").toString();  
-     QString G1_fileName = settings.value("Group1/FileName").toString();  
- 
- int G2_size = settings.value("Group2/Count").toInt();  
- int G2_price = settings.value("Group2/Price").toInt();  
- 
-     ui->lineEdit_Path->setText(G1_path);  
-     ui->lineEdit_FileName->setText(G1_fileName);  
-     ui->lineEdit_Count->setText(QString::number(G2_size));  
-     ui->lineEdit_Price->setText(QString::number(G2_price));  
- }  
- //写入配置文件
- void Dialog::on_pushButton_update_clicked()  
- {  
-     QString iniFilePath = "F:/QT_Project_201508/TestConfigFile/config_files/MyINI.ini";  
-     QSettings settings(iniFilePath,QSettings::IniFormat);  
- 
-     settings.setValue("Group1/Path",ui->lineEdit_Path->text());  
-     settings.setValue("Group1/FileName",ui->lineEdit_FileName->text());  
-     settings.setValue("Group2/Count",ui->lineEdit_Count->text().toInt());  
-     settings.setValue("Group2/Price",ui->lineEdit_Price->text().toInt());  
- }  
配置文件：
![](https://img-blog.csdn.net/20150822083552844)
程序运行结果：
![](https://img-blog.csdn.net/20150822083941584)
