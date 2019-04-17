# QT 学习笔记 - Keith - CSDN博客





2014年02月21日 23:04:32[ke1th](https://me.csdn.net/u012436149)阅读数：643








# 一、QT零散知识

### （一）Qt模块的添加和移除

Qt中所提供的类都是按模块进行组织的，反映到文件就是不同的文件夹。使用qtcreator新建工程时，可以看到默认情况下已经包含了qtcore和qtgui模块，需要增加或移除某个模块的引用时只需要将相应的复选框勾上或取消即可。如果工程已经创建后需要增加或移除，可以打开pro文件编辑。比如：

如果不想使用qtgui模块的话，可以在工程文件*.Pro中使用 

QT-=gui

同样的需要增加其它模块时也可以使用QT+=来增加某个模块的支持，比如

QT+=sql。

### （二）Qt头文件引用知识

在QtCreator中包含头文件时，可以有好几种写法，常常让人感到迷惑，其实这个应该是QtCreator为了方便我们的使用进行了优化。因为Qt毕竟是一个C++库，它也是遵循标准C++的标准的。在Qt中，类名和头文件名相同，而且它是按照模块进行组织的，一个模块反映到文件上就是一个同名的文件夹。同一模块下所有类的头文件都在这个和模块名一样的目录下。同时为了方便使用，在每个模块下另有一个名称为【模块名.H】的头文件，打开看看就知道其实这个头文件的内容就是挨个将该目录下另外的头文件include了一遍。所以，在QtCreator中，可以有以下2种写法：

**#include <目录名/类名.h> **也就是**<模块名/类名.h>**

比如 #include <QtGui/QLabel>，意思是指定引用Qlabel类的头文件

**#include <目录名/模块名.h> **也就是**<模块名/模块名.h>**

比如 #include <QtGui/QtGui.h>，意思是指定引用QtGui模块中所有类的头文件

由于在QtCreator中，已经定义了include命令的search目录，所以可以把前面的目录去掉，因此又增加了2种简化写法，即【**#include <类名.h>**】和【**#include <模块名.h>**】。

同样的，由于C++支持可以把后缀 .H 省略，因此又可以演化出4种不带后缀的写法。

虽然这几种方式都可以正常工作，但是还是强烈建议按照【模块名/类名】的格式书写，比如：#include<QtGui/QLabel>。这样做有一个好处就是潜移默化中让我们记住Qt中类的组织形式。

有时候为了简便，想要一下子包含该模块下所有类的头文件时也可以写成这样子：

#include <QtGui/QtGui>

这就表示包含QtGui模块中所有类的头文件，其中第一个QtGui是常规的模块名字，也就是文件夹的名称。第二个QtGui是该模块（文件夹）下的一个已经存在的预定义头文件QtGui.h。按照c++语法，我们直接写成#include <QtGui>效果也是一样的，不过此时的QtGui表示的是一个名为QtGui.h的实体头文件。这样子，我们又回到了简单的写法上来了，比如#include <QLabel>与#include <QtGui/QLabel>。其中的QLabel均表示一个名为QLabel.H的头文件，只不过qt把类的头文件名称写的与类名称一样而已。

### （三）QtCreator的代码自动完成和信息提示

QtCreator的代码完成默认是区分大小写的，也就是说你输入qlabel 时是得不到任何提示的，你输入的字符必须正确的匹配才可以，比如 QLabel。但这个是可以设置的，只需要在tools-options下的Text Editor下的Completion 将 Case-sensitive completion 的复选框取消选择即可。

另外就是自动完成的快捷键默认是 Ctrl + Space ，这个和Windows系统的输入法切换相冲突。由于输入法用的是系统级快捷键，会导致Qtcreator的快捷键不起作用。解决办法就是重新设置一下咯，同样在tools-options下的Environment下的Keyboard中设置 TextEditor-CompleteThis 即可，先删除默认的，再按键盘指定一个即可，推荐 Shift + z 。

### （四）QComboBox组件显示文本的对齐方式

有时需要指定QComboBox显示文本时的对齐方式，可是查看其类函数并没有类似的setalignment，后来再看看文档原来可以来个曲线救国，那就是给其指定一个QLineEdit用来显示文本，比如：

    QLineEdit *p=new QLineEdit();

    p->setAlignment(Qt::AlignHCenter);

ui->comboBox->setLineEdit(p);

不过这种情况下，这个LineEdit是可以编辑的，但是你可以像操作普通QLineEidt一样进一步进行定制后再设置其为comboBox的LineEdit。

### （五）Qt从源代码编译容易忽略的Perl问题

网络上有各种各样的帖子来说明如何从源代码编译Qt，但是都大多忽略了一个问题，那就是编译前先安装Perl，因为里面有些脚本是用Perl写的，如果不安装的话，在configure的时候会导致这些脚本不能执行，因此会少复制很多文件到指定的文件夹，那么编译的时候就会出现“找不到文件”之类的错误。这个是必须要安装的，win下可以直接安装activeperl即可。

转自：http://rcyboom.blog.163.com/blog/static/12764851120111013103445732/









