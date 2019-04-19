# Qt的QStringList - LC900730的博客 - CSDN博客
2017年09月22日 14:33:46[lc900730](https://me.csdn.net/LC900730)阅读数：374
是最简单的模型类，具备向视图提供字符串数据的能力。 
QStringList是一种常用的数据类型，实际上是一个字符串列表(也就是QList`<QString>`)
QStringListModel很多时候会作为QListView或者QComboBox这种只有一列的视图组件的数据模型。
```
//myistview.h
class myListView：public QWidget{
    Q_OBJECT
public:
    myListView(QWidget *parent);
    void insertData();
    void DeleteData();
    void showData();
    QStringListModel *model;
    QListView *listView;
}
```
```
//
myListView::myListView(QWidget *parent):QWidget(parent)
{
    QStringList data;
    data<<"LetterA"<<"LetterB"<<"LetterC";
    model=new QStringListModel(this);
    model->setStringList(data);
    listView= new QListView(this);
    listView->setModel(model);
    QHBoxLayout *btnLayout=new QHBoxLayout;
    QPushButton *insertBtn=new QPushButton(tr("insert"),this);
    connect(insertBtn,&QPushButton::clicked,this,&myListView::insertData);
    QPushButton *delBtn=new QPushButton(tr("Delete"),this);
    connect(delBtn,&QPushButton::clicked,this,&myListView::deleteData);
    QPushButton *showBtn=new QPushButton(tr("Show"),this);
    connect(showBtn,&QPushButton::clicked,this,&myListView::showData);
    btnLayout->addWidget(insertBtn);
    btnLayout->addWidget(delBtn);
    btnLayout->addWidget(showBtn);
    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(listView);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}
void myListView::deleteData(){
    if(model->rowCount()>1){
        model->removeRows(listView->currentIndex().row(),1);
    }
}
void myListView::insertData(){
    bool isOk;
    QString text=QInputDialog::getText(this,"Insert ","Please input new data:",QLineEdit::Normal,"You are inset  new data ",&isOk);
    if(isOk){
        //插入一行数据
        QModelIndex currIndex=listView->currentIndex();
        model->insertRows(currIndex.row(),1);
        model->setData(currIndex,text);
        listView->edit(currIndex);
    }
}
void myListView::showData(){
    QStringList data=model->stringList();
    QString str;
    foreach (QString s, data) {
        str+=s+"\n";
    }
    QMessageBox::information(this,"Data",str);
}
```
```
//main.cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myListView *myview=new myListView();
    myview->show();
    return a.exec();
}
```
QInputDialog
