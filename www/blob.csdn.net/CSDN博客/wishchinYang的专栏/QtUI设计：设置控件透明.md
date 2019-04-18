# QtUI设计：设置控件透明 - wishchinYang的专栏 - CSDN博客
2016年05月27日 11:07:39[wishchin](https://me.csdn.net/wishchin)阅读数：3311
QT设置按钮控件透明：
        代码：
```cpp
//设置按钮 背景 前景
    this->ui->ShowCvRGB->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(100,100,100,100)"));
    // 第一个 为前景色；第二个 为北京色； 透明度 为a
    this->ui->ShowPcdFile->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(100,100,100,100)"));
    this->ui->QwtPlotDraw->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(0,0,0,0)"));
    this->ui->Train->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(100,100,100,100)"));
    this->ui->Classify->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(100,100,100,100)"));
    this->ui->Abnormal->setStyleSheet(QString("color:rgba(255,255,255,255);background-color:rgba(100,100,100,100)"));
```
