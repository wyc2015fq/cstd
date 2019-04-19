# Qt通过QNetworkAccessManager实现Http网络通信 - xqhrs232的专栏 - CSDN博客
2018年09月05日 17:41:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：245
原文地址::[https://blog.csdn.net/imxiangzi/article/details/52525049](https://blog.csdn.net/imxiangzi/article/details/52525049)
相关文章
1、QT HTTP 通讯 使用方法----[https://blog.csdn.net/mingzznet/article/details/9724479](https://blog.csdn.net/mingzznet/article/details/9724479)
2、Qt实现Http通信----[https://blog.csdn.net/wkx_boda/article/details/64442575](https://blog.csdn.net/wkx_boda/article/details/64442575)
3、[QT HTTP 通讯 使用方法](http://pswzyu.iteye.com/blog/1279318)----[http://pswzyu.iteye.com/blog/1279318](http://pswzyu.iteye.com/blog/1279318)
 最近在搞Qt的网络通信和音乐播放的应用，做完每一个部分都会上传上来。下面的这段代码中实现了访问百度主页，返回的信息可以再输出窗查看，代码中注释的部分是上传文件用的。QNetworkAccessManager是Qt4.4里才引入的类，qt4,.4之前一般都是使用QHttp实现。
类申明：
**[cpp]**[view plain](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)
- #ifndef DIALOG_H  
- #define DIALOG_H  
- #include <QtGui/QDialog>  
- #include <QNetworkAccessManager>  
- #include <QFile>  
- #include <QNetworkReply>  
- #include <QNetworkRequest>  
- #include <QPushButton>  
- #include <QHBoxLayout>  
- #include <QIODevice>  
- 
- /********************HTTP实现通信******************************/  
- /*************************************************************/  
- /*************************************************************/  
- /*************************************************************/  
- class Dialog : public QDialog  
- {  
-     Q_OBJECT  
- 
- public:  
-     Dialog(QWidget *parent = 0);  
-     ~Dialog();  
- protected slots:  
-     void httpDownload();  
-     void replyFinished(QNetworkReply*reply);  
-     void onUploadProgress(qint64 bytesSent,qint64 butesTotal);  
- 
- private:  
-     QPushButton *download_Btn;  
-     QFile *file;  
-     QNetworkAccessManager *accessManager;  
-     QNetworkRequest request;  
-     QNetworkReply *reply;  
-     QHBoxLayout *layout;  
- 
- 
- 
- };  
- 
- 
- 
- 
- #endif // DIALOG_H  
类的实现：
**[cpp]**[view plain](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)
- #include "dialog.h"  
- #include<QtDebug>  
- 
- Dialog::Dialog(QWidget *parent)  
-     : QDialog(parent)  
- {  
-     download_Btn=new QPushButton("download");  
-     layout=new QHBoxLayout();  
-     layout->addWidget(download_Btn);  
-     layout->sizeHint().height();  
-     setLayout(layout);  
- 
-     connect(download_Btn,SIGNAL(clicked()),this,SLOT(httpDownload()));  
- 
- }  
- 
- Dialog::~Dialog()  
- {  
- 
- }  
- void Dialog::httpDownload(){  
-     //QUrl url("http://localhost:8080/uploadtest/MyUpload");  
-     QUrl url("http://www.baidu.com");  
-     QByteArray post_data;//上传的数据可以是QByteArray或者file类型  
-     post_data.append("hello");  
-     file=new QFile("/home/vino/Desktop/a.txt");  
-     file->open(QIODevice::ReadOnly);//只读方式打开文件  
-     accessManager=new QNetworkAccessManager(this);  
- 
-     request.setUrl(url);  
-     /******************设置http的header***********************/  
-    // request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");  
-    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");//上传文件  
-    // request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");  
-     //request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());  
- 
- 
-     connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的  
-     reply=accessManager->post(request,post_data);//通过发送数据，返回值保存在reply指针里.  
- 
-     //connect(reply, SIGNAL(uploadProgress ( qint64 ,qint64 )), this, SLOT( onUploadProgress(qint64 ,qint64 ));//上传文件时进度  
**[cpp]**[view plain](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)
-     //void QNetworkReply::downloadProgress ( qint64 bytesReceived, qint64 bytesTotal )  qt也有提供下载文件的进度   
- 
- 
- 
- 
- }  
- void Dialog::replyFinished(QNetworkReply *reply){  
-     //获取响应的信息，状态码为200表示正常  
-     QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);  
- 
-     //无错误返回  
-     if(reply->error() == QNetworkReply::NoError)  
-     {  
-         download_Btn->setText("success");  
-         QByteArray bytes = reply->readAll();    
-         QString result(bytes);  //转化为字符串  
-         qDebug()<<result;  
-     }  
-     else  
-     {  
-         //处理错误  
-          download_Btn->setText("failed");  
-     }  
- 
-     reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;  
- 
- }  
- 
- void Dialog::onUploadProgress(qint64 bytesSent, qint64 butesTotal){  
**[cpp]**[view plain](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)
- //文件上传的进度  
主函数：由Qtcreator自动生成
**[cpp]**[view plain](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral#)
- #include <QtGui/QApplication>  
- #include "dialog.h"  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication a(argc, argv);  
-     Dialog w;  
-     w.show();  
-     return a.exec();  
- }  
FROM:  http://blog.csdn.net/helloxiaoyueyue/article/details/38417383?utm_source=tuicool&utm_medium=referral
