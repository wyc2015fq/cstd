# QT学习笔记之QTableView设置属性的方法 - xqhrs232的专栏 - CSDN博客
2018年05月17日 14:26:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：227
原文地址::[https://blog.csdn.net/pkgk2013/article/details/69218292](https://blog.csdn.net/pkgk2013/article/details/69218292)
相关文章
1、
## QT QTableView用法小结
 ----[http://blog.sina.com.cn/s/blog_4ba5b45e0102e976.html](http://blog.sina.com.cn/s/blog_4ba5b45e0102e976.html)
2、Qt之QTableView的使用----[https://blog.csdn.net/zhuyunfei/article/details/51111558](https://blog.csdn.net/zhuyunfei/article/details/51111558)
**3、QTableView根据内容自动调整大小（resizeColumnToContents解决不了的）----[https://blog.csdn.net/Heaven_Evil/article/details/78617461?locationNum=7&fps=1](https://blog.csdn.net/Heaven_Evil/article/details/78617461?locationNum=7&fps=1)**
## 写在前面
这是在网上找到的，因为抄袭的人太多，我也不知道原创是谁了，不好意思贴出原博客的地址了，博主请原谅我！！
## 方法
```
一 添加表头
     //准备数据模型
    QStandardItemModel *student_model = new QStandardItemModel();
    student_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Name")));
    student_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("NO.")));
    student_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Sex")));
    student_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("Age")));
    student_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("College")));
    //利用setModel()方法将数据模型与QTableView绑定
    ui->student_tableview->setModel(student_model);
二 设置表格属性
    //设置列宽不可变动，即不能通过鼠标拖动增加列宽        
    ui->student_tableview->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);   
    ui->student_tableview->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);   
    ui->student_tableview->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);   
    ui->student_tableview->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);   
    ui->student_tableview->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);   
    //设置表格的各列的宽度值        
    ui->student_tableview->setColumnWidth(0,100);    
    ui->student_tableview->setColumnWidth(1,100);    
    ui->student_tableview->setColumnWidth(2,100);    
    ui->student_tableview->setColumnWidth(3,100);    
    ui->student_tableview->setColumnWidth(4,100);        
    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
    ui->student_tableview->verticalHeader()->hide();       
    //设置选中时为整行选中        
    ui->student_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);         
    //设置表格的单元为只读属性，即不能编辑        
    ui->student_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);          
    //如果你用在QTableView中使用右键菜单，需启用该属性        
    ui->tstudent_tableview->setContextMenuPolicy(Qt::CustomContextMenu);
三 动态添加行 
在表格中添加行时，我们只需要在model中插入数据即可，一旦model中的数据发生变化，QTabelView显示就会做相应的变动
//在第一行添加学生张三的个人信息(setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据)
    student_model->setItem(0, 0, new QStandardItem(“张三"));
    student_model->setItem(0, 1, new QStandardItem("20120202"));
    student_model->setItem(0, 2, new QStandardItem("男"));
    student_model->setItem(0, 3, new QStandardItem("18"));
    student_model->setItem(0, 4, new QStandardItem("土木学院"));
四 设置数据显示的样式
 //设置单元格文本居中，张三的数据设置为居中显示
    student_model->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    student_model->item(0, 1)->setTextAlignment(Qt::AlignCenter);
    student_model->item(0, 2)->setTextAlignment(Qt::AlignCenter);
    student_model->item(0, 3)->setTextAlignment(Qt::AlignCenter);
    student_model->item(0, 4)->setTextAlignment(Qt::AlignCenter);
    //设置单元格文本颜色，张三的数据设置为红色
    student_model->item(0, 0)->setForeground(QBrush(QColor(255, 0, 0))); 
    student_model->item(0, 1)->setForeground(QBrush(QColor(255, 0, 0))); 
    student_model->item(0, 2)->setForeground(QBrush(QColor(255, 0, 0))); 
    student_model->item(0, 3)->setForeground(QBrush(QColor(255, 0, 0))); 
    student_model->item(0, 4)->setForeground(QBrush(QColor(255, 0, 0))); 
    //将字体加粗
    student_model->item(0, 0)->setFont( QFont( "Times", 10, QFont::Black ) );
    student_model->item(0, 1)->setFont( QFont( "Times", 10, QFont::Black ) );
    student_model->item(0, 2)->setFont( QFont( "Times", 10, QFont::Black ) );
    student_model->item(0, 3)->setFont( QFont( "Times", 10, QFont::Black ) );
    student_model->item(0, 4)->setFont( QFont( "Times", 10, QFont::Black ) );
    //设置排序方式，按年龄降序显示
    student_model->sort(3, Qt::DescendingOrder);
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
## 自己的试验品
自己根据需求以及上述的帮助写了个功能块，可以借大家参考下 
这是截图： 
![这里写图片描述](https://img-blog.csdn.net/20170404225625520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGtnazIwMTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以下是这部分的代码：
```
//这里根据房间状态的不同分别显示不同的颜色
//设置单元格的颜色
void setRowColor(int index, int status, QStandardItemModel* itemModel)
{
    if (status == -1)
    {
        itemModel->item(index, 0)->setBackground(QBrush(QColor(200,200,169)));
        itemModel->item(index, 0)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 1)->setBackground(QBrush(QColor(200, 200, 169)));
        itemModel->item(index, 1)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 2)->setBackground(QBrush(QColor(200, 200, 169)));
        itemModel->item(index, 2)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 3)->setBackground(QBrush(QColor(200, 200, 169)));
        itemModel->item(index, 3)->setForeground(QBrush(Qt::white));
    }
    if (status == 0)
    {
        itemModel->item(index, 0)->setBackground(QBrush(QColor(131, 175, 155)));
        itemModel->item(index, 0)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 1)->setBackground(QBrush(QColor(131, 175, 155)));
        itemModel->item(index, 1)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 2)->setBackground(QBrush(QColor(131, 175, 155)));
        itemModel->item(index, 2)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 3)->setBackground(QBrush(QColor(131, 175, 155)));
        itemModel->item(index, 3)->setForeground(QBrush(Qt::white));
    }
    if (status == 1)
    {
        itemModel->item(index, 0)->setBackground(QBrush(QColor(254, 67, 101)));
        itemModel->item(index, 0)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 1)->setBackground(QBrush(QColor(254, 67, 101)));
        itemModel->item(index, 1)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 2)->setBackground(QBrush(QColor(254, 67, 101)));
        itemModel->item(index, 2)->setForeground(QBrush(Qt::white));
        itemModel->item(index, 3)->setBackground(QBrush(QColor(254, 67, 101)));
        itemModel->item(index, 3)->setForeground(QBrush(Qt::white));
    }
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
另外，附上QTableView获取某个单元格的数据的操作：
```
int curRow=ui->tableView->currentIndex().row(); //选中行
QAbstractItemModel *modessl = ui->tableView->model();
QModelIndex indextemp = modessl->index(curRow,i);//遍历第一行的所有列
i 是你要取值的列的下标
//这个是一个单元格的值。tostring()----ok
QVariant datatemp = modessl->data(indextemp);
```
- 1
- 2
- 3
- 4
- 5
- 6
再次声明，这不是我的原创，因为不知道原作者所以没有贴出原地址，希望大家原谅我没有贴出原博客的地址！
