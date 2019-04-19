# 关于Qt5 QtSerialPort串口通信----带工程源码下载 - xqhrs232的专栏 - CSDN博客
2017年10月11日 13:28:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2574
原文地址::[http://blog.csdn.net/lovebird_27/article/details/49515881](http://blog.csdn.net/lovebird_27/article/details/49515881)
相关文章
1、[QT实现串口通信(附源码下载)](http://blog.csdn.net/qq_27312943/article/details/52900527)----[http://blog.csdn.net/qq_27312943/article/details/52900527](http://blog.csdn.net/qq_27312943/article/details/52900527)
2、QT5串口通信——基础篇----[http://blog.csdn.net/k331922164/article/details/46136819?ref=myread](http://blog.csdn.net/k331922164/article/details/46136819?ref=myread)
3、[Qt实现串口通信总结](http://www.cnblogs.com/CD-lulu/p/4090867.html)----[http://www.cnblogs.com/CD-lulu/p/4090867.html](http://www.cnblogs.com/CD-lulu/p/4090867.html)
4、用Qt编写一个串口通讯程序----[https://jingyan.baidu.com/article/72ee561a5a644ce16138df37.html](https://jingyan.baidu.com/article/72ee561a5a644ce16138df37.html)
5、Qt编写串口通信程序全程图文讲解(大图版)----[https://wenku.baidu.com/view/7dd7a283d0d233d4b14e694b.html](https://wenku.baidu.com/view/7dd7a283d0d233d4b14e694b.html)
最近用Qt5重写了以前用MFC做的串口通信程序，从中提取了基础的部分，
做了一个简单的程序完成了一些功能:
串口基本的收，发数据
串口是否可用判定，串口状态与按钮状态之间联系
串口发送:字符和16进制发送的选择，回车键也为发送
串口接收:16进制的接收，按1字节显示与按1个字显示选择
串口接收数据:txt的形式保存，清除接收数据
串口自动发送
我把这个简单的通信程序和一些用到的工具上传了，有需要的朋友可以去下载。
[http://download.csdn.net/detail/lovebird_27/9227893](http://download.csdn.net/detail/lovebird_27/9227893)
在这里，记录一些个人觉得重要的代码
新建一个Qt项目，基类选为QWidget，类名serial
在serial.h头文件中添加
#include<QtSerialPort/QSerialPort>#include<QtSerialPort/QSerialPortInfo>
private:Ui::serial*ui;QSerialPort*my_serialport;QByteArrayrequestData;在serial.cpp中
串口的接收
my_serialport=newQSerialPort();     //定义1个串口类型connect(my_serialport,SIGNAL(readyRead()),this,SLOT(updata()));//一旦串口接收到数据，便会去执行updata()函数voidserial::updata(){requestData=my_serialport->readAll();//读取数据  串口读取出来的数据类型 是QByteArray 不是QStringQStringbuf;··buf=ShowHex(requestData);//显示为1个字节16进制··}串口发送if(ui->checkBox->checkState())//判定是否选择16进制发送{my_serialport->write(QString2Hex(ui->sendlineEdit->text()));//将文本编辑框editEdit的内容转化为16进制发送//注意的是文本编辑框内容的类型是QString，串口的数据发送QByteArray}else{my_serialport->write(ui->sendlineEdit->text().toLatin1());//将文本编辑框editEdit的内容以字符的形式发送//QString向QByteArray转换，用.toLatin1()，有的版本是.toAsscii() }
我把数据的转换是用在了datadeal.cpp中向串口输入16进制，用下面两个函数就能实现//将1-9a-f字符转化为对应的整数charConvertHexChar(charch){if((ch>='0')&&(ch<='9'))returnch-0x30;elseif((ch>='A')&&(ch<='F'))returnch-'A'+10;elseif((ch>='a')&&(ch<='f'))returnch-'a'+10;elsereturn(-1);}//将字符型进制转化为16进制QByteArrayQString2Hex(QStringstr){QByteArraysenddata;inthexdata,lowhexdata;inthexdatalen=0;intlen=str.length();senddata.resize(len/2);charlstr,hstr;for(inti=0;i<len;){hstr=str[i].toLatin1();//字符型if(hstr==''){i++;continue;}i++;if(i>=len)break;lstr=str[i].toLatin1();hexdata=ConvertHexChar(hstr);lowhexdata=ConvertHexChar(lstr);if((hexdata==16)||(lowhexdata==16))break;elsehexdata=hexdata*16+lowhexdata;i++;senddata[hexdatalen]=(char)hexdata;hexdatalen++;}senddata.resize(hexdatalen);returnsenddata;}将串口接收的16进制数据显示出来//将接收的一串QByteArray类型的16进制,转化为对应的字符串16进制QStringShowHex(QByteArraystr){QDataStreamout(&str,QIODevice::ReadWrite);//将str的数据读到out里面去QStringbuf;while(!out.atEnd()){qint8outChar=0;out>>outChar;//每次一个字节的填充到outcharQStringstr=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper()+QString("");//2字符宽度buf+=str;}returnbuf;}将串口接收的16进制数据，每2个字节合并成1个字显示出来QStringConvert4Hex(QByteArraystr){QDataStreamout(&str,QIODevice::ReadWrite);//将str的数据读到out里面去QStringbuf;while(!out.atEnd()){qint16outChar=0;out>>outChar;//每次一个字的填充到outcharQStringstr=QString("%1").arg(outChar&0xFFFF,4,16,QLatin1Char('0')).toUpper()+QString("");//4字符宽度buf+=str;}returnbuf;}以上两种串口发送16进制，接收显示不同的处理，很类似。只是改变了QString.arg()的内容。QString.arg()函数用途很广，我前面的博文QString的使用中也提到了，希望大家做好对它的总结。以上是我在Qt5中编写简单串口通信的核心部分，希望对大家有所帮助，如果需要看源代码的，我已上传，连接在文章开头处。
