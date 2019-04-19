# Qt实现背景图片旋转缩放 - fanyun的博客 - CSDN博客
2019年02月28日 23:01:06[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：39
1.给QWidget 增加背景图片，图片可随窗口放大缩小
```cpp
this->Mwind->setStyleSheet("border-image: url(:/icon/loj.png);");//注意一定要使用border-image,使用background-image就会出现窗口放大，背景图片过小重叠的现象
```
2. 给QPushButton 增加背景图片：背景图片可根据Button大小自由缩放
```cpp
void setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)  
{  
     
    QPixmap pixmap(image);  
    QPixmap fitpixmap=pixmap.scaled(55,55).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  
    button->setIcon(QIcon(fitpixmap));  
    button->setIconSize(QSize(sizeW,sizeH));  
    button->setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用  
    button->setStyleSheet("border: 0px");//消除边框，取消点击效果  
}
```
3. 给QWidget 增加背景图片，图片可自由缩放
```cpp
this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。  
QPixmap pixmap(":/images/bg_news.png");  
QPixmap fitpixmap=pixmap.scaled(1200, 1200).scaled(config->mainWindowW,config->mainWindowH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  
QPalette palette;  
palette.setBrush(QPalette::Background, QBrush(fitpixmap));  
this->setPalette(palette);
```
4. 给QLabel 增加背景图片：图片可自由缩放
```cpp
QPixmap pixmap(normalIcon);  
QPixmap fitpixmap=pixmap.scaled(labelIcon->width(), labelIcon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  
labelIcon->setPixmap(fitpixmap);
```
5. 采用QSS样式，增加背景图片，图片显示原始比例
```cpp
lastBtn->setStyleSheet("background-image: url(:/images/btn_previous_normal.png);border: 0px");
```
6.QPixmap旋转图片显示
```cpp
QMatrix leftmatrix;  
leftmatrix.rotate(270);  
ui->label->setPixmap(pixmap.transformed(leftmatrix,Qt::SmoothTransformation));
```

