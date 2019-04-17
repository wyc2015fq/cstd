# Qt入门教程系列之三 第一个Qt项目：记事本 - DumpDoctorWang的博客 - CSDN博客





2018年08月13日 16:47:13[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：689








# 一、成果演示

这就是我们的记事本

![](https://img-blog.csdn.net/20180813163441601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看完这篇博客可能还有许多不懂，不过别急，我后面还会更新博客来讲Qt。你可以先百度来解决自己的问题。 

# 二、编写记事本

**所有新增的代码都应该自己敲一遍。**不懂的，可以先略过。**C++要是不懂，先补补C++**。
- 
## 创建项目Notpad


![](https://img-blog.csdn.net/20180813163844683?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

notepad.pro - 项目文件

main.cpp - 主文件

notepad.cpp - Notepad主界面的源码

notepad.h - Notepad主界面的头文件

notepad.ui - Notepad 的UI文件
- 
## main.cpp


```cpp
#include "notepad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /**
     * @brief a QApplication对象
     * 管理Qt应用的资源，每个使用了Widget的应用都需要QApplication的对象
     */
    QApplication a(argc, argv);
    /**
     * @brief w Notepad 的对象
     * Notepad也是一个Widget
     */
    Notepad w;
    // 显示Notepad的界面
    w.show();
    // 进入应用的事件循环，当程序在运行时，事件(键盘点击、鼠标点击等很多事件)会发送到
    // 本应用的所有Widget，比如在本应用中，事件就会发送给w。
    return a.exec();
}
```
- 
## notepad.ui


notepad.ui是定义用户界面的文件，实际上是一个XML文件。它包含了有什么控件、控件的位置、控件之间的所属关系等等。当构建（编译）项目时，编译程序会调用“Qt [User Interface Compiler (uic)](http://doc.qt.io/qt-5/uic.html)”来把notepad.ui编译为一个C++头文件ui_notepad.h；ui_notepad.h包含了有什么控件、控件的位置、控件之间的所属关系等等。

双击notepad.ui，会进入UI编辑器界面。截取出我们感兴趣的部分，如下，左右同种颜色的框一一对应

![](https://img-blog.csdn.net/20180812180332340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 Notepad类继承QMainWindow，一个QMainWindow包含菜单栏(menuBar)、工具栏(mainToolBar)、状态栏(statsuBar)和centralWidget(可用来放入其他控件，中文不知如何翻译)。

**添加Text Edit控件。**

![](https://img-blog.csdn.net/20180813164234995?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

控件的周围会出现8个深蓝色小方块，把鼠标移动到这些方块上，并拖动，就可以改变控件的大小。界面如下图所示

![](https://img-blog.csdn.net/20180812190310149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击运行，程序的界面如下

![](https://img-blog.csdn.net/20180812190649864?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

** 设计菜单项。**双击UI中的“在这里输入”，在输入框中输入“文件”，敲回车。然后同理，再双击后面的“在这里输入”，输入“编辑”，

![](https://img-blog.csdn.net/20180813164618601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

得到如下界面

![](https://img-blog.csdn.net/20180812191432892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 点击“文件”或者“编辑”，会出现一个下拉菜单，点击下拉菜单中中的“在这里输入”，就可以输入子菜单项。“文件”应该包含的子菜单项为“新建”、“打开”、“保存”、“另存为”；“编辑”应当包含的子菜单项为“粘贴”、“重做”、“撤销”。添加子菜单的时候，我发现不能输入中文，目前的解决方案是在别的地方输入中文，然后复制粘贴进去。添加完子菜单之后，界面如下

![](https://img-blog.csdn.net/20180812194411134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 红色框内就是控件之间的关系。Notepad包含centralWidget、menuBar、mainToolBar和statusBar；centralWidget包含一个textEdit；menuBar包含menu和menu_2；依次类推。**双击红色框内控件的名字，可以更改控件的名字。**控件名应该代表这个控件的作用，而不应该是action、action_2、action_3这样无意义的控件名。改完控件名之后，如下图所示

![](https://img-blog.csdn.net/20180812194819170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

红色框内是Action Editor(我翻译为动作编辑器) ，每一行为一个Action，也就是一个子菜单项。双击一行中的任意一个位置，会打开一个Action的属性窗口如下

![](https://img-blog.csdn.net/20180812195148108?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击一下红色框内的输入框，按下组合键，输入框里面就会出现你刚才按下的组合键。这个组合键就是这个Action的快捷键。修改所有Action的组合键，修改完成后如下图所示；**每完成一步都要按Ctrl+S保存！**

![](https://img-blog.csdn.net/20180812195630900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 此时在编译运行程序，菜单已经已经有了。但是，这时候点击菜单，是没有任何反应的。
- 
## notepad.h


下面就是notepad.h的定义，添加了详细的注释。**在Qt中，按住Ctrl，然后再点击宏或者类名，就会转到宏或者类的定义；点击变量名的时候，会转到变量的定义；点击函数名的时候，会在函数定义(函数声明)和函数实现之间来回跳转。**

```bash
#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

namespace Ui {
class Notepad;//这个类是ui_notepad.h中的UI界面类，和下面的Notepad不是同一个类
}

class Notepad : public QMainWindow
{
    /**
     * 每个UI相关的类(或者其他继承自QObject的类)都需要有下面这一行。
     * Q_OBJECT是一个宏，暂且不需要了解是什么。
     */
    Q_OBJECT

public:
    /**
     * @brief Notepad 构造函数。
     * 这个Notepad类是应用的功能实现，上面的Ui::Notepad是一个定义UI的类。
     * @param parent 父类指针
     */
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private:
    // 界面指针
    Ui::Notepad *ui;
};

#endif // NOTEPAD_H
```

**Qt的信号(signal)和槽(slot)机制简述 。**Qt中代替回调函数的机制叫做信号槽，继承自QObject类(Qt库中，很多类都直接或者间接继承自QObject类)，都可以有信号和槽。**信号和槽都是一个函数，信号只有函数名，没有实现；槽是一个完整的函数，可执行某种功能。信号和槽的参数需要相同；一个信号可以通过connect函数和多个槽相连**。当信号被触发时，槽函数会执行，所以，槽函数也可以理解为回调函数；有多个槽函数时，当信号被触发，会按照与信号相连的顺序顺序执行。

在Qt中的类中，定义信号和槽的关键字分别为signals和slots，具体示例如下

```cpp
#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

namespace Ui {
class Notepad;//这个类是ui_notepad.h中的UI界面类，和下面的Notepad不是同一个类
}

class Notepad : public QMainWindow
{
    /**
     * 每个UI相关的类(或者其他继承自QObject的类)都需要有下面这一行。
     * Q_OBJECT是一个宏，暂且不需要了解是什么。
     */
    Q_OBJECT

public:
    /**
     * @brief Notepad 构造函数。
     * 这个Notepad类是应用的功能实现，上面的Ui::Notepad是一个定义UI的类。
     * @param parent 父类指针
     */
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private:
    // 界面指针
    Ui::Notepad *ui;

signals: //信号
    void opened();

public slots: //公有槽定义区，可关联其他类中信号
    void onActionNewTriggered(bool checked); //新建
    void onActionOpenTriggered(bool checked);//打开
    void onActionSaveTriggered(bool checked);//保存
    void onActionSaveasTriggered(bool checked);//另存为
    void onActionPasteTriggered(bool checked);//粘贴
    void onActionUndoTriggered(bool checked);//撤销
    void onActionRedoTriggered(bool checked);//重做

private slots: //私有槽定义区，只能关联本类中的信号
    void onEdited();
};

#endif // NOTEPAD_H
```

**需要的注意的是，不管公由槽还是私有槽，都需要实现，否则编译通不过。**
- 
## notepad.cpp


按住Ctrl键，鼠标左键点击“explicit Notepad(QWidget *parent = 0)”中的“Notepad”，就会跳转到notepad.cpp中“explicit Notepad(QWidget *parent = 0)”的实现，如下

```cpp
#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad) //实例化ui
{
    ui->setupUi(this);//把ui和本类关联起来，ui中的事件什么的就可以发送到本类
}

Notepad::~Notepad()
{
    delete ui;
}
```

**把信号和槽相连。**信号和槽相连需要使用connect函数，connect有四个参数，第一个参数是sender的指针，第二个参数是sender的一个信号，第三个参数是receiver的指针，第四个参数是receiver的槽。

```cpp
#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad) //实例化ui
{
    ui->setupUi(this);//把ui和本类关联起来，ui中的事件什么的就可以发送到本类

    // 当actionNew的信号triggered被触发时，也就是被点击时，槽onActionNewTriggered会被调用
    this->connect(ui->actionNew, //sender
                  SIGNAL(triggered(bool)),//sender的信号
                  this,//receiver
                  SLOT(onActionNewTriggered(bool)));//receiver的槽
    // 连接剩下的信号与槽
    this->connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(onActionOpenTriggered(bool)));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(onActionSaveTriggered(bool)));
    connect(ui->actionSaveas,SIGNAL(triggered(bool)),this,SLOT(onActionSaveasTriggered(bool)));
    connect(ui->actionPaste,SIGNAL(triggered(bool)),this,SLOT(onActionPasteTriggered(bool)));
    connect(ui->actionRedo,SIGNAL(triggered(bool)),this,SLOT(onActionRedoTriggered(bool)));
    connect(ui->actionUndo,SIGNAL(triggered(bool)),this,SLOT(onActionUndoTriggered(bool)));
}

Notepad::~Notepad()
{
    delete ui;
}
```

完成上面的代码之后，菜单项发出的信号就能和Notepad类的槽关联起来了。当对应的菜单项被点击时，对应的槽就会被执行。但是槽函数并没有实现，所此时编译无法通过。

转到notepad.h中，左键单击一个槽函数的名字，比如“onActionNewTriggered”，然后Alt+Enter键，会弹出一个菜单，然后在菜单中选择"在notepad.cpp"中添加定义，此时，编辑器会跳转notepad.cpp中，并添加以下代码

```cpp
void Notepad::onActionNewTriggered(bool checked)
{
    
}
```

在上面的槽中添加一行输出Debug信息的代码

```cpp
void Notepad::onActionNewTriggered(bool checked)
{
    //输出函数名和checked的值，qDebug()需要包含头文件QDebug
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}
```

此时，按住Ctrl键，左键单击“onActionNewTriggered”，就又会跳转到notepad.h中“onActionNewTriggered”的定义处。重复上面的步骤，把槽都实现，整个notepad.cpp如下

```cpp
#include "notepad.h"
#include "ui_notepad.h"
#include <QDebug>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad) //实例化ui
{
    ui->setupUi(this);//把ui和本类关联起来，ui中的事件什么的就可以发送到本类

    // 当actionNew的信号triggered被触发时，也就是被点击时，槽onActionNewTriggered会被调用
    this->connect(ui->actionNew, //sender
                  SIGNAL(triggered(bool)),//sender的信号
                  this,//receiver
                  SLOT(onActionNewTriggered(bool)));//receiver的槽
    // 连接剩下的信号与槽
    this->connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(onActionOpenTriggered(bool)));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(onActionSaveTriggered(bool)));
    connect(ui->actionSaveas,SIGNAL(triggered(bool)),this,SLOT(onActionSaveasTriggered(bool)));
    connect(ui->actionPaste,SIGNAL(triggered(bool)),this,SLOT(onActionPasteTriggered(bool)));
    connect(ui->actionRedo,SIGNAL(triggered(bool)),this,SLOT(onActionRedoTriggered(bool)));
    connect(ui->actionUndo,SIGNAL(triggered(bool)),this,SLOT(onActionUndoTriggered(bool)));
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::onActionNewTriggered(bool checked)
{
    //输出函数名和checked的值
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionOpenTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionSaveTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionSaveasTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionPasteTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionUndoTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionRedoTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onEdited()
{
    qDebug()<<__FUNCTION__<<endl;
}
```

此时再运行项目，点击菜单项，可以看到“应用程序输出”下面的框中就有对应的输出

![](https://img-blog.csdn.net/20180813143730636?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**接下来是实现函数的功能。**

**实现onActionSaveTriggered()**。首先在Notepad类里定义一个QString类型的变量savePath，为保存路径

![](https://img-blog.csdn.net/20180813144839260?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后实现onActionSaveTriggered函数

```cpp
void Notepad::onActionSaveTriggered(bool checked)
{
    qDebug()<<"保存文件..."<<endl;
    if(savePath.isEmpty()){//如果保存路径为空，则询问保存路径
        QString file_path;
        while (file_path.isEmpty()) {
            //括号里的参数分别是：指定父控件、标题、默认打开后显示的目录、右下角的文件过滤器。
            file_path = QFileDialog::getSaveFileName(this,"选择路径",".","*.txt");
        }
        // 在下面的“应用程序输出中”输出程序路径
        qDebug()<<file_path<<endl;
        // 把保存路径存下来
        this->savePath = file_path;
    }
    // 定义QFile文件
    QFile txt(this->savePath);
    // 打开文件
    bool tag = txt.open(QIODevice::WriteOnly);
    if(tag){
        // 写入文件
        txt.write(ui->textEdit->toPlainText().toStdString().data());
    }else{
        qDebug()<<"打开文件失败"<<endl;
        // 提示打开文件失败
        QMessageBox::critical(NULL, "错误", "无法打开文件，请检查对文件目录是否具有读写权限",
                              QMessageBox::Yes, QMessageBox::Yes);
    }
    // 关闭文件
    txt.close();
    // 在头文件定义save，作为是否保存的标志
    this->saved = true;
}
```

**现在看上面的代码，你可能有很多不懂，不懂的部分，除了百度，还可以参考Qt的文档。**比如QFileDialog的用法，就可以百度“QFileDialog 使用”，会有很多博客教你使用；有的博客写的不好，就多看几篇，Qt的资源相对还是比较多。除了百度以外，把光标移动到类名，比如“QFileDialog”上，然后按F1，就可以打开QFileDialog的帮助文档；把光标移动到“getSaveFileName”，再按F1，就可以跳转到getSaveFileName的帮助文档。**上述函数每一行都搞清楚搞清楚之后再往下看。**

**接下来实现onActionNewTriggered()**。要新建文件，要检查是否已经保存，若保存了，则直接清空编辑框，若没有保存，提示用户是否保存。若输入框，也就是textEdit里面的文本被编辑，则可以认为文件被编辑过；当textEdit的里面的文本被编辑时，会触发信号"textChanged"，因此，可在Notepad中定义一个槽onEdited来接收文本变化的信息，并在Notepad的**构造函数**中关联

```cpp
this->saved = true;
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(onEdited()));
```

 实现onEdited

```cpp
void Notepad::onEdited()
{
    qDebug()<<"文本被修改..."<<endl;
    saved = false;
}
```

 实现onActionNewTriggered

```cpp
void Notepad::onActionNewTriggered(bool checked)
{
    qDebug()<<"新建文件..."<<endl;
    if(this->saved){//若已经保存
        //清空编辑框
        ui->textEdit->clear();
        this->saved = true;
    }else{//若没有保存
        // 询问是否保存
        int tag = QMessageBox::question(NULL, "注意", "是否保存文件？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(tag == QMessageBox::Yes){//点了“yes”
            this->onActionSaveTriggered(false);//保存
        }
        ui->textEdit->clear();
        this->saved = true;
    }
}
```

我没有实现onActionOpenTriggered，这个就留给读者去实现，作为练习。剩余的其他函数我都实现了。notepad.h如下

```cpp
#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class Notepad;//这个类是ui_notepad.h中的UI界面类，和下面的Notepad不是同一个类
}

class Notepad : public QMainWindow
{
    /**
     * 每个UI相关的类(或者其他继承自QObject的类)都需要有下面这一行。
     * Q_OBJECT是一个宏，暂且不需要了解是什么。
     */
    Q_OBJECT

public:
    /**
     * @brief Notepad 构造函数。
     * 这个Notepad类是应用的功能实现，上面的Ui::Notepad是一个定义UI的类。
     * @param parent 父类指针
     */
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private:
    // 界面指针
    Ui::Notepad *ui;
    // 定义保存路径
    QString savePath;
    // 是否保存了
    bool saved;

signals: //信号
    void opened();

public slots: //公有槽定义区，可关联其他类中信号
    void onActionNewTriggered(bool checked); //新建
    void onActionOpenTriggered(bool checked);//打开
    void onActionSaveTriggered(bool checked);//保存
    void onActionSaveasTriggered(bool checked);//另存为
    void onActionPasteTriggered(bool checked);//粘贴
    void onActionUndoTriggered(bool checked);//撤销
    void onActionRedoTriggered(bool checked);//重做

private slots: //私有槽定义区，只能关联本类中的信号
    void onEdited();
};

#endif // NOTEPAD_H
```

notepap.cpp如下

```cpp
#include "notepad.h"
#include "ui_notepad.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad) //实例化ui
{
    ui->setupUi(this);//把ui和本类关联起来，ui中的事件什么的就可以发送到本类

    // 当actionNew的信号triggered被触发时，也就是被点击时，槽onActionNewTriggered会被调用
    this->connect(ui->actionNew, //sender
                  SIGNAL(triggered(bool)),//sender的信号
                  this,//receiver
                  SLOT(onActionNewTriggered(bool)));//receiver的槽
    // 连接剩下的信号与槽
    this->connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(onActionOpenTriggered(bool)));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(onActionSaveTriggered(bool)));
    connect(ui->actionSaveas,SIGNAL(triggered(bool)),this,SLOT(onActionSaveasTriggered(bool)));
    connect(ui->actionPaste,SIGNAL(triggered(bool)),this,SLOT(onActionPasteTriggered(bool)));
    connect(ui->actionRedo,SIGNAL(triggered(bool)),this,SLOT(onActionRedoTriggered(bool)));
    connect(ui->actionUndo,SIGNAL(triggered(bool)),this,SLOT(onActionUndoTriggered(bool)));

    this->saved = true;
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(onEdited()));
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::onActionNewTriggered(bool checked)
{
    qDebug()<<"新建文件..."<<endl;
    if(this->saved){//若已经保存
        //清空编辑框
        ui->textEdit->clear();
        this->saved = true;
    }else{//若没有保存
        // 询问是否保存
        int tag = QMessageBox::question(NULL, "注意", "是否保存文件？",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(tag == QMessageBox::Yes){//点了“yes”
            this->onActionSaveTriggered(false);//保存
        }
        ui->textEdit->clear();
        this->saved = true;
    }
}

void Notepad::onActionOpenTriggered(bool checked)
{
    qDebug()<<__FUNCTION__<<":"<<checked<<endl;
}

void Notepad::onActionSaveTriggered(bool checked)
{
    qDebug()<<"保存文件..."<<endl;
    if(savePath.isEmpty()){//如果保存路径为空，则询问保存路径
        QString file_path;
        while (file_path.isEmpty()) {
            //括号里的参数分别是：指定父控件、标题、默认打开后显示的目录、右下角的文件过滤器。
            file_path = QFileDialog::getSaveFileName(this,"选择路径",".","*.txt");
        }
        // 在下面的“应用程序输出中”输出程序路径
        qDebug()<<file_path<<endl;
        // 把保存路径存下来
        this->savePath = file_path;
    }
    // 定义QFile文件
    QFile txt(this->savePath);
    // 打开文件
    bool tag = txt.open(QIODevice::WriteOnly);
    if(tag){
        // 写入文件
        txt.write(ui->textEdit->toPlainText().toStdString().data());
    }else{
        qDebug()<<"打开文件失败"<<endl;
        // 提示打开文件失败
        QMessageBox::critical(NULL, "错误", "无法打开文件，请检查对文件目录是否具有读写权限",
                              QMessageBox::Yes, QMessageBox::Yes);
    }
    // 关闭文件
    txt.close();
    // 在头文件定义save，作为是否保存的标志
    this->saved = true;
}

void Notepad::onActionSaveasTriggered(bool checked)
{
    qDebug()<<"另存为..."<<endl;
    this->savePath.clear();
    this->onActionSaveTriggered(checked);
}

void Notepad::onActionPasteTriggered(bool checked)
{
    qDebug()<<"粘贴..."<<endl;
    ui->textEdit->paste();
}

void Notepad::onActionUndoTriggered(bool checked)
{
    qDebug()<<"撤销..."<<endl;
    ui->textEdit->undo();
}

void Notepad::onActionRedoTriggered(bool checked)
{
    qDebug()<<"重做..."<<endl;
    ui->textEdit->redo();
}

void Notepad::onEdited()
{
    qDebug()<<"文本被修改..."<<endl;
    saved = false;
}
```





