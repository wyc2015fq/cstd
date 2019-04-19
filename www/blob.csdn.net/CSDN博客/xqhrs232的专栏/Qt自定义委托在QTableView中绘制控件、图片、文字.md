# Qt自定义委托在QTableView中绘制控件、图片、文字 - xqhrs232的专栏 - CSDN博客
2018年01月06日 20:04:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1721
原文地址::[http://blog.csdn.net/zhi379/article/details/28412189](http://blog.csdn.net/zhi379/article/details/28412189)
相关文章
1、[在表格(QTableView)中插入图片](http://qimo601.iteye.com/blog/1538333)----[http://qimo601.iteye.com/blog/1538333](http://qimo601.iteye.com/blog/1538333)
2、[基于QItemDelegate的例子1 SpinBoxDelegate](http://qimo601.iteye.com/blog/1536453)----[http://qimo601.iteye.com/blog/1536453](http://qimo601.iteye.com/blog/1536453)
3、[基于QItemDelegate的例子2 trackeEditorDelegate](http://qimo601.iteye.com/blog/1536464)----[http://qimo601.iteye.com/blog/1536464](http://qimo601.iteye.com/blog/1536464)
4、[(六)Qt Delgate的使用 简单说明](http://qimo601.iteye.com/blog/1536444)----[http://qimo601.iteye.com/blog/1536444](http://qimo601.iteye.com/blog/1536444)
### [Qt自定义委托在QTableView中绘制控件、图片、文字](http://qimo601.iteye.com/blog/1539147)
原文链接：http://qimo601.iteye.com/blog/1539147
**博客分类：**[Qt](http://qimo601.iteye.com/category/189878)
[Qt](http://www.iteye.com/blogs/tag/Qt)[自定义委托](http://www.iteye.com/blogs/tag/%E8%87%AA%E5%AE%9A%E4%B9%89%E5%A7%94%E6%89%98)[QTableView](http://www.iteye.com/blogs/tag/QTableView)[QCheckBox](http://www.iteye.com/blogs/tag/QCheckBox)[进度条](http://www.iteye.com/blogs/tag/%E8%BF%9B%E5%BA%A6%E6%9D%A1)
自定义委托，继承于，QStyledItemDelegate类，重载Paint()函数，
1、实现在QTableView中绘制 格式字符串
2、实现在QTableView中绘制进度条
3、实现在QTableView中绘制QCheckBox
4、实现在QTableView中绘制星星
5、实现在QTableView中绘制Pixmap图片
![](http://dl.iteye.com/upload/picture/pic/112947/9aef6370-b7de-3a19-84b6-efacbf3e8322.jpg)
![](http://dl.iteye.com/upload/picture/pic/112949/0983b7fb-b49e-39ff-bcea-02add7260cf0.jpg)
**1、实现在QTableView中绘制 格式字符串**
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //重载绘制函数
- void DelReconQueue::paint(QPainter *painter, const QStyleOptionViewItem &option,  
- const QModelIndex &index) const
- {  
- //如果是第2列'病人Id'
- if (index.column() == 2)  
-     {  
- //获得当前项值
- int patientId = index.model()->data(index, Qt::DisplayRole).toInt();  
- //设置'病人Id'格式字符串: P:00000x;6位10进制数，不足补0;
-         QString text = QString("P:%1").arg(patientId, 6, 10, QChar('0'));  
- 
- 
- //获取项风格设置
-         QStyleOptionViewItem myOption = option;  
-         myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;  
- //绘制文本
-     QApplication::style()->drawItemText ( painter, myOption.rect , myOption.displayAlignment, QApplication::palette(), true,text );  
- 
- 
-     }  
- else
-     {  
- //否则调用默认委托
-         QStyledItemDelegate::paint(painter, option, index);  
-     }  
- 
- }  
**如果自定义委托继承于QItemDelegate类，绘制字符串，可以直接用QItemDelegate中的drawDisplay();**
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- void TrackDelegate::paint(QPainter *painter,    
- const QStyleOptionViewItem &option,    
- const QModelIndex &index) const
- {    
- //保存音轨的列  
- if (index.column() == durationColumn) {    
- //获得索引对应Model中的数据  
- int secs = index.model()->data(index, Qt::DisplayRole).toInt();    
- //设置时间格式字符串 分:秒  
-         QString text = QString("%1:%2")    
-                        .arg(secs / 60, 2, 10, QChar('0'))    
-                        .arg(secs % 60, 2, 10, QChar('0'));    
- //获取项风格设置  
-         QStyleOptionViewItem myOption = option;    
-         myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;    
- //绘制文本  
-         drawDisplay(painter, myOption, myOption.rect, text);    
- //如果当前有焦点，就绘制一个焦点矩形，否则什么都不做  
-         drawFocus(painter, myOption, myOption.rect);    
-     } else{    
- //否则默认  
-         QItemDelegate::paint(painter, option, index);    
-     }    
- }    
**2、实现在QTableView中绘制进度条**
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //重载绘制函数
- void DelReconQueue::paint(QPainter *painter, const QStyleOptionViewItem &option,  
- const QModelIndex &index) const
- {  
- //如果是'已经完成子任务数'
- if (index.column() == 9)  
-     {  
- const QAbstractItemModel *itemModel = index.model();  
- //获得索引对应Model中的数据
- int finishedSubTaskNum = itemModel->data(index, Qt::DisplayRole).toInt();  
- int subTaskNum = itemModel->data(itemModel->index(index.row(),8), Qt::DisplayRole).toInt();  
- 
- //进度条的风格选项
-         QStyleOptionProgressBarV2 *progressBarOption = new QStyleOptionProgressBarV2();  
-         progressBarOption->rect = option.rect;  
-         progressBarOption->minimum = 0;  
-         progressBarOption->maximum = subTaskNum;  
-         progressBarOption->progress = finishedSubTaskNum;  
- int t = finishedSubTaskNum/subTaskNum;  
-         progressBarOption->text = QString::number(t) + "%";  
-         progressBarOption->textVisible = true;  
- //绘制进度条
-         QApplication::style()->drawControl(QStyle::CE_ProgressBar, progressBarOption, painter);  
- 
-     }  
- else
-     {  
- //否则调用默认委托
-         QStyledItemDelegate::paint(painter, option, index);  
-     }  
- 
- }  
**3、实现在QTableView中绘制QCheckBox**
Java代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- #include <QtGui>  
- #include <QItemDelegate>  
- #include <QStyleOptionProgressBarV2>  
- #include "DelReconQueue.h"
- //重载绘制函数
- void DelReconQueue::paint(QPainter *painter, const QStyleOptionViewItem &option,  
- const QModelIndex &index) const
- {  
- if (index.column() == 11)  
-     {  
- //获取值
-         bool checked = index.model()->data(index, Qt::DisplayRole).toBool();  
- //按钮的风格选项
-         QStyleOptionButton *checkBoxOption = new QStyleOptionButton();  
-         checkBoxOption->state |= QStyle::State_Enabled;  
- //根据值判断是否选中
- if(checked)  
-         {  
-             checkBoxOption->state |= QStyle::State_On;  
-         }  
- else
-         {  
-             checkBoxOption->state |= QStyle::State_Off;  
-         }  
- //返回QCheckBox几何形状
-         checkBoxOption->rect = CheckBoxRect(option);  
- //绘制QCheckBox
-         QApplication::style()->drawControl(QStyle::CE_CheckBox,checkBoxOption,painter);  
- 
-     }  
- 
- else
-     {  
- //否则调用默认委托
-         QStyledItemDelegate::paint(painter, option, index);  
-     }  
- 
- }  
    //生成QCheckBox
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- QRect DgSystemLog::CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)const
- {  
- //绘制按钮所需要的参数
-     QStyleOptionButton checkBoxStyleOption;  
- //按照给定的风格参数 返回元素子区域
-     QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);  
- //返回QCheckBox坐标
-     QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,  
-                          viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);  
- //返回QCheckBox几何形状
- return QRect(checkBoxPoint, checkBoxRect.size());  
- }  
**4、实现在QTableView中绘制自定义类 星星**
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //重载绘制函数
- void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,  
- const QModelIndex &index) const
- {  
- //如果某项数据是星星类型
- if (qVariantCanConvert<StarRating>(index.data())) {  
- //获取该项数据，并转换成StarRating类型
-         StarRating starRating = qVariantValue<StarRating>(index.data());  
- //如果有控件被选中，我们就让选中的控件变亮
- if (option.state & QStyle::State_Selected)  
-             painter->fillRect(option.rect, option.palette.highlight());  
- 
-         starRating.paint(painter, option.rect, option.palette,  
-                          StarRating::ReadOnly);  
-     }  
- //如果没有控件选中，调用默认委托
- else {  
-         QStyledItemDelegate::paint(painter, option, index);  
-     }  
- }  
**星星自定义源文件**
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- #ifndef STARRATING_H
- #define STARRATING_H
- 
- #include <QMetaType>
- #include <QPointF>
- #include <QVector>
- 
- class StarRating  
- {  
- public:  
- enum EditMode { Editable, ReadOnly };  
- 
-     StarRating(int starCount = 1, int maxStarCount = 5);  
- 
- void paint(QPainter *painter, const QRect &rect,  
- const QPalette &palette, EditMode mode) const;  
-     QSize sizeHint() const;  
- int starCount() const { return myStarCount; }  
- int maxStarCount() const { return myMaxStarCount; }  
- void setStarCount(int starCount) { myStarCount = starCount; }  
- void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }  
- 
- private:  
-     QPolygonF starPolygon;  
-     QPolygonF diamondPolygon;  
- int myStarCount;  
- int myMaxStarCount;  
- };  
- 
- //让所有模板类型都知道该类，包括QVariant
- Q_DECLARE_METATYPE(StarRating)  
- 
- #endif
- 
- 
- 
- #include <QtGui>
- #include <math.h>
- 
- #include "starrating.h"
- 
- constint PaintingScaleFactor = 20;  
- 
- StarRating::StarRating(int starCount, int maxStarCount)  
- {  
-     myStarCount = starCount;  
-     myMaxStarCount = maxStarCount;  
- 
-     starPolygon << QPointF(1.0, 0.5);  
- for (int i = 1; i < 5; ++i)  
-         starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),  
-                                0.5 + 0.5 * sin(0.8 * i * 3.14));  
- 
-     diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)  
-                    << QPointF(0.6, 0.5) << QPointF(0.5, 0.6)  
-                    << QPointF(0.4, 0.5);  
- }  
- 
- QSize StarRating::sizeHint() const
- {  
- return PaintingScaleFactor * QSize(myMaxStarCount, 1);  
- }  
- 
- void StarRating::paint(QPainter *painter, const QRect &rect,  
- const QPalette &palette, EditMode mode) const
- {  
-     painter->save();  
- 
-     painter->setRenderHint(QPainter::Antialiasing, true);  
-     painter->setPen(Qt::NoPen);  
- 
- if (mode == Editable) {  
-         painter->setBrush(palette.highlight());  
-     } else {  
-         painter->setBrush(palette.foreground());  
-     }  
- 
- int yOffset = (rect.height() - PaintingScaleFactor) / 2;  
-     painter->translate(rect.x(), rect.y() + yOffset);  
- //画笔坐标
-     painter->scale(PaintingScaleFactor, PaintingScaleFactor);  
- 
- for (int i = 0; i < myMaxStarCount; ++i) {  
- if (i < myStarCount) {  
-             painter->drawPolygon(starPolygon, Qt::WindingFill);  
-         } elseif (mode == Editable) {  
-             painter->drawPolygon(diamondPolygon, Qt::WindingFill);  
-         }  
-         painter->translate(1.0, 0.0);  
-     }  
- 
-     painter->restore();  
- }  
** 5、实现在QTableView中绘制****Pixmap图片 ，**
**详细请看具体例子**[Qt-在表格(QTableView)中插入图片](http://qimo601.iteye.com/blog/1538333)
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- void MyItemDelegate::paint(QPainter * painter,    
- const QStyleOptionViewItem & option,    
- const QModelIndex & index) const
- {    
- if(index.column()!=0){    
-         QItemDelegate::paint(painter,option,index);    
- return;    
-     }    
- const QAbstractItemModel * model=index.model();    
-     QVariant var=model->data(index,Qt::CheckStateRole);    
- if(var.isNull()) var=false;    
- const QPixmap & star=var.toBool()?    
- favouritePixmap:notFavouritePixmap;    
- 
- int width=star.width();    
- int height=star.height();    
-     QRect rect=option.rect;    
- int x=rect.x()+rect.width()/2-width/2;    
- int y=rect.y()+rect.height()/2-height/2;    
- 
-     painter->drawPixmap(x,y,star);    
- }    

