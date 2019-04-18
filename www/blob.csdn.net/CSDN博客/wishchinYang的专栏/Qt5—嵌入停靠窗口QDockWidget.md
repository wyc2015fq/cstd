# Qt5—嵌入停靠窗口QDockWidget - wishchinYang的专栏 - CSDN博客
2015年01月07日 17:09:34[wishchin](https://me.csdn.net/wishchin)阅读数：2386
参考链接：http://blog.csdn.net/summer_xiyer/article/details/12875899
新建一个GUI工程：
QDockWidget是QWidget的子类，也等于是说他可以像其他的Widgets一样互相嵌套，搭配布局。
1.一个简单的例子，选择QMainWindow作为基类
填写头文件：
```cpp
public slots:
    void on_PleaseGo_clicked();
public:
    void pleaseGo();
private:
    Ui::wishchin *ui;
    QDockWidget* dockInsertFRight;
    QDockWidget* dockInsertFLeft;
    QDockWidget* dockInsertButton;
```
填写CPP中的构造函数：
```cpp
ui->setupUi(this);
    //捕获主界面按钮窗口
    this->dockInsertButton = new QDockWidget(tr("Capture the button!"), this);
    //QDockWidget* dockInsertFRight = new QDockWidget(tr("First Dock Window"), this);
    //QTextEdit *dockText = new QTextEdit();
    //dockText->setText("Capture the button!");
    dockInsertButton->setWidget(this->ui->PleaseGo);
    this->addDockWidget(Qt::RightDockWidgetArea, dockInsertButton);
    //捕获主界面显示窗口
    QDockWidget *dockInsert = new QDockWidget(tr("Insert Dock Window"), this);
    //QTextEdit *dockText3 = new QTextEdit();
    //dockText3->setText("Third Window,Try to move The title.");
    dockInsert->setWidget(this->ui->label_Show);
    addDockWidget(Qt::RightDockWidgetArea,dockInsert);
    //配置第一个停靠窗口
    this->dockInsertFRight = new QDockWidget(tr("First Dock Window"), this);
    //QDockWidget* dockInsertFRight = new QDockWidget(tr("First Dock Window"), this);
    QTextEdit *dockText = new QTextEdit();
    dockText->setText("First Window,Try to move The title.");
    dockInsertFRight->setWidget(dockText);
    this->addDockWidget(Qt::RightDockWidgetArea, dockInsertFRight);
    //配置第二个停靠窗口
    this->dockInsertFLeft = new QDockWidget(tr("Second Dock Window"), this);
    //QTextEdit *dockText = new QTextEdit();
    //dockText->setText("First Window,Try to move The title.");
    QLabel* dockLabelShow =new QLabel();
    dockLabelShow->setMaximumHeight(320);
    dockLabelShow->setMaximumHeight(240);
    dockInsertFLeft->setWidget(dockLabelShow);
    this->addDockWidget(Qt::LeftDockWidgetArea, dockInsertFLeft);
    //配置第三个停靠窗口
    QDockWidget *dockOverlay = new QDockWidget(tr("Third Dock Window"), this);
    QTextEdit *dockText3 = new QTextEdit();
    dockText3->setText("Third Window,Try to move The title.");
    dockOverlay->setWidget(dockText3);
    addDockWidget(Qt::RightDockWidgetArea, dockOverlay);
```
运行显示结果：
![](https://img-blog.csdn.net/20150107171456890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样就实现了悬浮窗和嵌入窗，并能捕获主界面的显示Dock和按钮widget！
