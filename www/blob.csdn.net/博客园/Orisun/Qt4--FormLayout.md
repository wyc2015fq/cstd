# Qt4--FormLayout - Orisun - 博客园







# [Qt4--FormLayout](https://www.cnblogs.com/zhangchaoyang/articles/1987767.html)





FormLayout显然对于Form非常适合。

QFormLayout *formLay=new QFormLayout;    

QLineEdit *name=new QLineEdit;    

QLineEdit *email=new QLineEdit;    

QLineEdit *address=new QLineEdit;
formLay->addRow(tr("&Name:"),name);    

formLay->addRow(tr("&Email:"),email);    

formLay->addRow(tr("&Address:"),address);

setLayout(formLay);

仅需要这些代码就可以显示如下：

![](https://pic002.cnblogs.com/images/2011/103496/2011031810121054.png)

formLay->setLabelAlignment(Qt::AlignRight);

![](https://pic002.cnblogs.com/images/2011/103496/2011031810194367.png)

formLay->setRowWrapPolicy(QFormLayout::WrapAllRows);（默认的是QFormLayout::DontWrapAllRows）之后变为

![](https://pic002.cnblogs.com/images/2011/103496/2011031810140095.png)

另外注意到tr("&Name:")，加这个'&'是什么意思呢，实际上在为一个Label setBuddy时必须加&，在FormLayout中自动把LineEdit设为Label的Buddy了。如果使用其他Layout需要用下面代码显示指定Buddy:
QLineEdit *nameEd  = new QLineEdit(this);
 QLabel    *nameLb  = new QLabel("&Name:", this);
 nameLb->setBuddy(nameEd);
 QLineEdit *phoneEd = new QLineEdit(this);
 QLabel    *phoneLb = new QLabel("&Phone:", this);
 phoneLb->setBuddy(phoneEd);设置Buddy有神马用呢？当你按Alt+A时Address对应的LineEdit自动获得Focus。另外只有Label才可以有BuddyLayout之间可以相互嵌套，Vlayout->addLayout(hLayout);但是注意FormLayout没有addLayout，只有addChildLayout，并且addChildLayout的访问权限还是protected。











