# QT设计UI：QT模式对话框打开文件 - wishchinYang的专栏 - CSDN博客
2016年05月27日 10:53:10[wishchin](https://me.csdn.net/wishchin)阅读数：1817
使用QT模式对话框，并使显示框 为背景色；
       方法使用了QCheckBox*native;  
#include<QCheckBox>
初始化函数代码：
```cpp
//设置默认打开图像位置
    native = new QCheckBox(this);
    this->ui->lineEdit_ImageRoad->setText("./Image/Tulipa.jpg");//设置默认打开图像
    //设置 前景色 ：白色 ； 背景色：透明
   this->ui->lineEdit_ImageRoad->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(0,0,0,0)"));
```
使用模式框获取文件位置：
```cpp
QString CWishGUI::GetOpenFileName()
{
    QFileDialog::Options options;
    if (!native->isChecked())
        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("QFileDialog::getOpenFileName()"),
                                this->ui->lineEdit_ImageRoad->text(),
                                tr("All Files (*);;Text Files (*.txt)"),
                                &selectedFilter,
                                options);
    return fileName;
}
```
获取文件名：
std::stringSFilename=QFilename.toStdString();
