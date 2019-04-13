
# QT常用问题 - 嵌入式Linux - CSDN博客

2016年12月07日 20:53:55[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：460


Int 装QString
QString::number(changInt,10)
QString 转 QByteArray
QByteArraymacValue=ui->lineEdit->text().mid(4,8).toLatin1();**把****QByteArray****转成****QString**

QStrings;s.prepend(postData);ui->textBrowser->append(s);

**获取系统时间**
QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
label->setText(str);//在标签上显示时间


**获取打开文件的路径名称**
QStringListfiles=QFileDialog::getOpenFileNames(this,"选择需要更新的数据库文件","/home","*.txt");MyThreadthread;thread.macFileDir=(QString)files.at(0);qDebug()<<files;if(thread.macFileDir.null){ui->textBrowser->append("\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#选择文件不正确\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#");return;}**把QString转成QByteArray**
QByteArraymactext=ui->plainTextEdit->toPlainText().toLatin1();
**把Int转成对应的十六进制数字符串QString**

intintQrand=qrand()%65535;QStringstringQrand=QString::number(intQrand,16).toUpper();**把显示界面窗口最大化**
QWidget::setWindowState(Qt::WindowMaximized);**用随机数之前要设置一下输入参数**
+   QTime t;
+   t= QTime::currentTime();
+   qsrand(t.msec()+t.second()*1000);


/*生产随机数*/intintQrand=qrand()%65535;QStringstringQrand=QString::number(intQrand,16).toUpper();QStringstringQrandS;switch(stringQrand.length()){case0:stringQrandS="0000";break;case1:stringQrandS="000"+stringQrand;break;case2:stringQrandS="00"+stringQrand;break;case3:stringQrandS="0"+stringQrand;break;case4:stringQrandS=stringQrand;break;default:stringQrandS=stringQrand;qDebug()<<"over65535";break;}qDebug()<<stringQrandS;returnstringQrandS;**用当前时间转换成UTC时间**
qint64time=QDateTime::currentDateTimeUtc().toTime_t();**字符串去掉空格，这个不能用库函数**
QStringMainWindow::deBlank(QStringstrs){intlen=strs.length();QStringlsv_Strs=NULL;for(inti=0;i<len;i++){if(strs.at(i).isSpace()){//strs+="";}else{lsv_Strs+=strs.at(i);}}returnlsv_Strs;}
## 输入框并获取输入框的数据
\#include <QInputDialog>boolisOK;QStringtext=QInputDialog::getText(NULL,"InputDialog","Pleaseinputyourcomment",QLineEdit::Normal,"yourcomment",&isOK);
把exe生成在当前这个目录
DESTDIR+=$$PWD



