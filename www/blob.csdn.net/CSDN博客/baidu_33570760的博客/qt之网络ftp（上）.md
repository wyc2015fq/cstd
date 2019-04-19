# qt之网络ftp（上） - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:50:10[carman_风](https://me.csdn.net/baidu_33570760)阅读数：509
## 转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/39.html
## 导语
上一节我们讲述了HTTP的编程，这一节讲述与其及其相似的FTP的编程。FTP即FileTransfer Protocol，也就是文件传输协议。FTP的主要作用，就是让用户连接上一个远程计算机，查看远程计算机有哪些文件，然后把文件从远程计算机上拷贝到本地计算机，或者把本地计算机的文件送到远程计算机上。
环境：Windows Xp + Qt 4.8.5+QtCreator 2.8.0
## 目录
- 一、简介
- 二、实现简单的文件下载
## 正文
一、简介
在Qt中，我们可以使用上一节讲述的`QNetworkAccessManager`和`QNetworkReply`类来进行FTP 程序的编写，因为它们用起来很简单。但是，对于较复杂的FTP操作，Qt还提供了`QFtp`类，利用这个类，我们很容易写出一个FTP客户端程序。下面我们先在帮助中查看这个类。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/33-1.jpg)
在QFtp中，所有的操作都对应一个特定的函数，我们可以称它们为命令。如`connectToHost()`连接到服务器命令，`login()`登录命令，`get()`下载命令，`mkdir()`新建目录命令等。因为`QFtp`类以异步方式工作，所以所有的这些函数都不是阻塞函数。也就是说，如果一个操作不能立即执行，那么这个函数就会直接返回，直到程序控制权返回Qt事件循环后才真正执行，它们不会影响界面的显示。
所有的命令都返回一个`int` 型的编号，使用这个编号让我们可以跟踪这个命令，查看其执行状态。当每条命令开始执行时，都会发出`commandStarted()`信号，当该命令执行结束时，会发出`commandFinished()`信号。我们可以利用这两个信号和命令的编号来获取命令的执行状态。当然，如果不想执行每条命令都要记下它的编号，也可以使用`currentCommand()`来获取现在执行的命令，其返回值与命令的对应关系如下图。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/33-2.jpg)
二、实现简单的文件下载
下面我们先看一个简单的FTP客户端的例子，然后对它进行扩展。在这个例子中我们从FTP服务器上下载一个文件并显示出来。
1．我们新建Qt Gui应用。项目名次为`myFtp`，基类选择`QWidget`，类名保持`Widget`即可。完成后打开`muFtp.pro`文件，在上面添加一行：`QT += network`，然后保存该文件。
2．修改`widget.ui`文件。在其中添加一个`TextBrowser` 和一个
`Label`，效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/33-3.jpg)
3．在`main.cpp`中进行修改。
为了在程序中可以使用中文，我们在`main.cpp`中添加头文件`#include <QTextCodec>`
并在`main()`函数中添加代码：
```cpp
QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
```
4．在`widget.h`中进行修改。
先添加头文件：`#include <QFtp>`
再在`private`中定义对象：`QFtp *ftp;`
添加私有槽函数：
```cpp
private slots:
    void ftpCommandStarted(int);
void ftpCommandFinished(int,bool);
```
5．在`widget.cpp`中进行更改。
（1）在构造函数中添加代码：
```cpp
ftp = new QFtp(this);
ftp->connectToHost("ftp.qt-project.org"); //连接到服务器
ftp->login();   //登录
ftp->cd("qt/source");   //跳转到“qt”目录下的source目录中
ftp->get("INSTALL");  //下载“INSTALL”文件
ftp->close();   //关闭连接
// 当每条命令开始执行时发出相应的信号
connect(ftp,SIGNAL(commandStarted(int)),
        this,SLOT(ftpCommandStarted(int)));
// 当每条命令执行结束时发出相应的信号
connect(ftp,SIGNAL(commandFinished(int,bool)),
        this,SLOT(ftpCommandFinished(int,bool)));
```
我们在构造函数里执行了几个FTP的操作，登录站点，并下载了一个文件。然后又关联了两个信号和槽，用来跟踪命令的执行情况。
（2）实现槽函数：
```cpp
void Widget::ftpCommandStarted(int)
{
    if(ftp->currentCommand() == QFtp::ConnectToHost){
       ui->label->setText(tr("正在连接到服务器..."));
    }
    if (ftp->currentCommand() == QFtp::Login){
       ui->label->setText(tr("正在登录..."));
    }
    if (ftp->currentCommand() == QFtp::Get){
       ui->label->setText(tr("正在下载..."));
    }
    else if (ftp->currentCommand() == QFtp::Close){
       ui->label->setText(tr("正在关闭连接..."));
    }
}
```
每当命令执行时，都会执行`ftpCommandStarted()`函数，它有一个参数`int id`，这个`id`就是调用命令时返回的`id`，如`int loginID= ftp->login();`这时，我们就可以用`if(id == loginID)`来判断执行的是否是`login()`函数。但是，我们不想为每个命令都设置一个变量来存储其返回值，所以，我们这里使用了`ftp->currentCommand()`，它也能获取当前执行的命令的类型。在这个函数里我们让开始不同的命令时显示不同的状态信息。
```cpp
void Widget::ftpCommandFinished(int,bool error)
{
    if(ftp->currentCommand() == QFtp::ConnectToHost){
       if(error)
           ui->label->setText(tr("连接服务器出现错误：%1")
                               .arg(ftp->errorString()));
       else ui->label->setText(tr("连接到服务器成功"));
    }
    if (ftp->currentCommand() == QFtp::Login){
       if(error)
           ui->label->setText(tr("登录出现错误：%1")
                               .arg(ftp->errorString()));
       else ui->label->setText(tr("登录成功"));
    }
    if (ftp->currentCommand() == QFtp::Get){
       if(error)
           ui->label->setText(tr("下载出现错误：%1")
                               .arg(ftp->errorString()));
       else {
           ui->label->setText(tr("已经完成下载"));
           ui->textBrowser->setText(ftp->readAll());
       }
    }
    else if (ftp->currentCommand() == QFtp::Close){
       ui->label->setText(tr("已经关闭连接"));
    }
}
```
这个函数与`ftpCommandStarted()`函数相似，但是，它是在一个命令执行结束时执行的。它有两个参数，第一个`intid`，就是调用命令时返回的编号，我们在上面已经讲过了。第二个是`bool error`，它标志现在执行的命令是否出现了错误。如果出现了错误，那么`error` 为`true` ，否则为`false`。我们可以利用它来输出错误信息。在这个函数中，我们在完成一条命令时显示不同的状态信息，并显示可能的出错信息。在
```
if
 (ftp->currentCommand() == QFtp::Get)
```
中，也就是已经完成下载时，我们让`textBrowser`显示下载的信息。
6．运行程序，效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/33-4.jpg)
7．出错演示。
下面我们演示一下出错时的情况。
将构造函数中的代码`ftp->login();`改为`ftp->login("tom","123456");`
这时我们再运行程序：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/33-5.jpg)
可以看到，它输出了错误信息，指明了错误的指令和出错的内容。其实我们设置的这个错误，也是想告诉大家，在FTP中如果没有设置用户名和密码，那么默认的用户名应该是`anonymous`，这时密码可以任意填写，而使用其他用户名是会出错的。
## 结语
在下一节中，我们将会对这个程序进行扩展，让它可以浏览服务器上的所有文件，并进行下载。
            
