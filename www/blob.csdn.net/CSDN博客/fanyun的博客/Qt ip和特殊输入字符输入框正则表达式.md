# Qt ip和特殊输入字符输入框正则表达式 - fanyun的博客 - CSDN博客
2017年05月31日 21:23:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：512
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
正则表达式来实现ip地址，网络端口，信号通道的输入功能的，不符合规范的数据将不能输入：
 QLineEdit的setValidator函数.
/* ip输入框正则表达式 */
// IP 前3段
```cpp
QRegExp regExp("[0-9][0-9.][0-9.][.]");
ui->lineEdit_1->setValidator(new QRegExpValidator(regExp, ui->lineEdit_1));
ui->lineEdit_2->setValidator(new QRegExpValidator(regExp, ui->lineEdit_2));
ui->lineEdit_3->setValidator(new QRegExpValidator(regExp, ui->lineEdit_3));
```
// 第4段
```cpp
regExp = QRegExp("[0-9][0-9][0-9]");
ui->lineEdit_4->setValidator(new QRegExpValidator(regExp, ui->lineEdit_4));
```
// 端口输入框
```cpp
regExp = QRegExp("[0-9][0-9][0-9][0-9][0-9]");
ui->lineEdit_6->setValidator(new QRegExpValidator(regExp, ui->lineEdit_6));
ui->lineEdit_7->setValidator(new QRegExpValidator(regExp, ui->lineEdit_7));
```
// 输入处理
```cpp
if (arg1.toInt() > 255) {
        QMessageBox::information(this, "information", "输入有误");
        ui->lineEdit_1->setText(tr(""));
}
// 如果输入.号
if ((arg1.indexOf(QString("."))) != -1) {
        ui->lineEdit_1->setText(arg1.section('.', 0, 0));
        focusNextPrevChild(true);
}
```
正则表达式网络配置通用代码：
```cpp
QRegExp regExpIP("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");  
QRegExp regExpNetPort("((6553[0-5])|[655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{3}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])");  
QRegExp regExpChannel("[0-5]");  
  
ui->lineEdit_locIP->setValidator(new QRegExpValidator(regExpIP,this));  
ui->lineEdit_subNetMask->setValidator(new QRegExpValidator(regExpIP,this));  
ui->lineEdit_GatewayIP->setValidator(new QRegExpValidator(regExpIP,this));  
ui->lineEdit_serIP->setValidator(new QRegExpValidator(regExpIP,this));  
ui->lineEdit_serPort->setValidator(new QRegExpValidator(regExpNetPort,this));  
ui->lineEdit_rfChannel->setValidator(new QRegExpValidator(regExpChannel,this)
```
