# qt之网络ftp(下) - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:51:39[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1219
## 转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/40.html
## 导语
前面讲述了一个最简单的FTP客户端程序的编写，这一节我们将这个程序进行扩展，使其可以浏览并能下载服务器上的所有文件。
环境：Windows Xp + Qt 4.8.5+QtCreator 2.8.0
## 目录
- 一、修改界面
- 二、功能实现
## 正文
一、修改界面
我们删除了`TextBrowser` ，加入了几个`Label` ，`Line Edit` ，`Push Button`部件，一个`Tree Widge`t及一个`Progress Bar`部件。然后我们对其中几个部件做如下更改。
（1）将“FTP服务器”标签后的`Line Edit`的`objectName`属性改为`ftpServerLineEdit`，其`text` 属性改为`ftp.qt-project.org`。
（2）将“用户名”标签后的`Line Edit`的`objectName`属性改为`userNameLineEdit`，其`text`属性改为`anonymous`，将其`toolTip`属性改为“默认用户名请使用：anonymous ，此时密码任意。”
（3）将“密码”标签后的`Line Edit`的`objectName`属性改为`passWordLineEdit`，其`text`属性改为`123456`，将其`echoMode`属性改为`Password`。
（4）将“连接”按钮的`objectName`属性改为`connectButton`。
（5）将“返回上一级目录”按钮的`objectName`属性改为`cdToParentButton`。
（6）将“下载”按钮`的objectName`属性改为`downloadButton`。
（7）将`Tree Widget`的`objectName`属性改为`fileList`，然后在`Tree Widget`部件上单击鼠标右键，选择`Edit Items`菜单，添加列属性如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/34-1.jpg)
最终界面如图所示：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/34-2.jpg)
下面我们的程序中，就是实现在用户填写完相关信息后，按下“连接”按钮，就可以连接到FTP服务器，并在`TreeWidget`中显示服务器上的所有文件，我们可以按下“下载”按钮来下载选中的文件，并使用进度条显示下载进度。
二、功能实现
1．更改`widget.h`文件。
（1）添加头文件`#include <QtGui>`
（2）在`private`中添加变量：
```cpp
QHash<QString, bool> isDirectory; //用来存储一个路径是否为目录的信息
QString currentPath; //用来存储现在的路径
QFile *file;
```
（3）添加槽：
```cpp
private slots:
void on_downloadButton_clicked();
void on_cdToParentButton_clicked();
void on_connectButton_clicked();
void ftpCommandFinished(int,bool);
void ftpCommandStarted(int);
void updateDataTransferProgress(qint64,qint64 );//更新进度条
//将服务器上的文件添加到Tree Widget中
void addToList(const QUrlInfo &urlInfo);
void processItem(QTreeWidgetItem*,int);//双击一个目录时显示其内容
```
2．更改`widget.cpp`的内容。
（1）实现“连接”按钮的单击事件槽。
```cpp
void Widget::on_connectButton_clicked()   //连接按钮
{
    ui->fileList->clear();
    currentPath.clear();
    isDirectory.clear();
    ftp = new QFtp(this);
    connect(ftp,SIGNAL(commandStarted(int)),
this,SLOT(ftpCommandStarted(int)));
    connect(ftp,SIGNAL(commandFinished(int,bool)),
           this,SLOT(ftpCommandFinished(int,bool)));
    connect(ftp,SIGNAL(listInfo(QUrlInfo)),
this,SLOT(addToList(QUrlInfo)));
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),
           this,SLOT(updateDataTransferProgress(qint64,qint64)));
    QString ftpServer = ui->ftpServerLineEdit->text();
    QString userName = ui->userNameLineEdit->text();
    QString passWord = ui->passWordLineEdit->text();
    ftp->connectToHost(ftpServer,21); //连接到服务器,默认端口号是21
    ftp->login(userName,passWord);   //登录
}
```
我们在“连接”按钮的单击事件槽函数中新建了`ftp`对象，然后关联了相关的信号和槽。这里的`listInfo()`信号由`ftp->list()`函数发射，它将在登录命令完成时调用，下面我们提到。而`dataTransferProgress()`信号在数据传输时自动发射。最后我们从界面上获得服务器地址，用户名和密码等信息，并以它们为参数执行连接和登录命令。
（2）更改`ftpCommandFinished()`函数。
我们在相应位置做更改。
首先，在登录命令完成时，我们调用list()函数：
```cpp
ui->label->setText(tr("登录成功"));
ftp->list();   //发射listInfo()信号，显示文件列表   
然后，在下载命令完成时，我们使下载按钮可用，并关闭打开的文件。
ui->label->setText(tr("已经完成下载"));
ui->downloadButton->setEnabled(true);
file->close();     
delete file;
```
最后再添加一个`if`语句，处理`list`命令完成时的情况：
```cpp
if (ftp->currentCommand() == QFtp::List){
       if (isDirectory.isEmpty())
       { //如果目录为空,显示“empty”
           ui->fileList->addTopLevelItem(
                  new QTreeWidgetItem(QStringList()<< tr("<empty>")));
           ui->fileList->setEnabled(false);
           ui->label->setText(tr("该目录为空"));
       }
}
```
我们在`list`命令完成时，判断文件列表是否为空，如果为空，就让`Tree Widget`不可用，并显示“empty”条目。
（3）添加文件列表函数的内容如下。
```cpp
void Widget::addToList(const QUrlInfo &urlInfo)  //添加文件列表
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, urlInfo.name());
    item->setText(1, QString::number(urlInfo.size()));
    item->setText(2, urlInfo.owner());
    item->setText(3, urlInfo.group());
    item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));
    QPixmap pixmap(urlInfo.isDir() ? "../myFtp2/dir.png" : "../myFtp2/file.png");
    item->setIcon(0, pixmap);
    isDirectory[urlInfo.name()] = urlInfo.isDir();
    //存储该路径是否为目录的信息
    ui->fileList->addTopLevelItem(item);
    if (!ui->fileList->currentItem()) {
       ui->fileList->setCurrentItem(ui->fileList->topLevelItem(0));
       ui->fileList->setEnabled(true);
    }
}
```
当`ftp->list()`函数执行时会发射`listInfo()`信号，此时就会执行`addToList()`函数，在这里我们将文件信息显示在`Tree Widget`上，并在`isDirectory`中存储该文件的路径及其是否为目录的信息。为了使文件与目录进行区分，我们使用了不同的图标`file.png`和`dir.png`来表示它们，这两个图标放在了工程文件夹中。
（4）将构造函数的内容更改如下。
```cpp
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    //鼠标双击列表中的目录时，我们进入该目录
    connect(ui->fileList,SIGNAL(itemActivated(QTreeWidgetItem*,int)),
           this,SLOT(processItem(QTreeWidgetItem*,int)));
}
```
这里我们只是让进度条的值为0，然后关联了`Tree Widget`的一个信号`itemActivated()`。当鼠标双击一个条目时，发射该信号，我们在槽函数中判断该条目是否为目录，如果是则进入该目录。
（5）`processItem()`函数的实现如下。
```cpp
void Widget::processItem(QTreeWidgetItem* item,int)  //打开一个目录
{
    QString name = item->text(0);
    if (isDirectory.value(name)) {  //如果这个文件是个目录，则打开
       ui->fileList->clear();
       isDirectory.clear();
       currentPath += '/';
       currentPath += name;
       ftp->cd(name);
       ftp->list();
       ui->cdToParentButton->setEnabled(true);
    }
}
```
（6）“返回上一级目录”按钮的单击事件槽函数如下。
```
void Widget::on_cdToParentButton_clicked()  //返回上级目录按钮
{
    ui->fileList->clear();
    isDirectory.clear();
    currentPath = currentPath.left(currentPath.lastIndexOf('/'));
    if (currentPath.isEmpty()) {
       ui->cdToParentButton->setEnabled(false);
       ftp->cd("/");
    } else {
       ftp->cd(currentPath);
    }
    ftp->list();
}
```
在返回上一级目录时，我们取当前路径的最后一个`/`之前的部分，如果此时路径为空了，我们就让“返回上一级目录”按钮不可用。
（7）“下载”按钮单击事件槽函数如下。
```cpp
void Widget::on_downloadButton_clicked()  //下载按钮
{
    QString fileName = ui->fileList->currentItem()->text(0);
    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly))
    {
       delete file;
       return;
}
//下载按钮不可用，等下载完成后才可用
    ui->downloadButton->setEnabled(false);    ftp->get(ui->fileList->currentItem()->text(0), file);
}
```
在这里我们获取了当前项目的文件名，然后新建文件，使用`get()`命令下载服务器上的文件到我们新建的文件中。
（8）更新进度条函数内容如下。
```cpp
void Widget::updateDataTransferProgress( //进度条
                  qint64 readBytes,qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(readBytes);
}
```
3．流程说明。
整个程序的流程就和我们实现函数的顺序一样。用户在界面上输入服务器的相关信息，然后我们利用这些信息进行连接并登录服务器，等登录服务器成功时，我们列出服务器上所有的文件。对于一个目录，我们可以进入其中，并返回上一级目录，我们可以下载文件，并显示下载的进度。
对于`ftp`的操作，全部由那些命令和信号来完成，我们只需要调用相应的命令，并在其发出信号时，进行对应的处理就可以了。而对于文件的显示，则是视图部分的知识了。
4．运行程序，效果如下图所示。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/34-3.jpg)
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/34-4.jpg)
## 结语
最后需要说明的是，因为为了更好的讲解知识，使得程序简单化，所以我们省去了很多细节上的处理，如果需要，你可以自己添加。比如断开连接和取消下载，你都可以使用
`ftp->abort()`函数。你也可以参考Qt自带的Ftp Example例子。对于其他操作，比如上传等，你可以根据需要添加。
FTP的相关编程就讲到到这里。
            
