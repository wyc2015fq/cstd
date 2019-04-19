# signal-slot 信号/槽(也译信号/插槽)机制 - 三少GG - CSDN博客
2011年10月14日 11:03:51[三少GG](https://me.csdn.net/scut1135)阅读数：2284标签：[signal																[qt																[makefile																[object																[编译器																[compiler](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
# [原创翻译：信号&槽(QT 基础) crystalblue，sand.fj.wen翻译 cxt_programmer校正](http://www.devdiv.com/article-1162-1.html)
2010-10-22 14:55| 发布者: [Vincent](http://www.devdiv.com/home.php?mod=space&uid=1)| 查看: 1396| 评论: [1](http://www.devdiv.com/portal.php?mod=comment&id=1162&idtype=aid)
# 前言
##### Qt是Nokia今后最重要的开发平台，Symbian、Maemo、MeeGo都将使用Qt。
###### 鉴于以下原因：
n  市场上面几乎不存在相关的中文资料
n  许多开发者对E文很是感冒
n  电子版的文档方便技术传播和交流
###### [DevDiv.com移动开发论坛](http://www.devdiv.com/)的翻译组重操宝刀，利用业余时间和相关方面的经验，把一些好的Qt英文资料翻译成中文，为广大嵌入式移动开发者尽一点绵薄之力。
## 关于DevDiv翻译组
请参考[DevDiv详细内容](http://www.devdiv.com/thread-36364-1-1.html)。
## 技术支持
首先DevDiv翻译组对您能够阅读本系列文档以及关注DevDiv表示由衷的感谢。Qt for Symbian开发是一个比较新的技术，在您学习和开发过程中，一定会遇到一些问题。DevDiv论坛集结了国内一流嵌入式移动开发专家，我们很乐意与您一起探讨Qt及相关问题。如果您有什么问题和技术需要支持的话，请登陆网站[www.devdiv.com](http://www.devdiv.com/)或者发送邮件到webmaster@DevDiv.com，我们将尽快回复您。
## 关于本文的翻译
感谢crystalblu，sand.fj.wen对本文的翻译，同时非常感谢Cxt_programmer在百忙中抽出时间对翻译初稿的认真校验。才使本文与读者尽快见面。由于书稿内容多，我们的知识有限，尽管我们进行了细心的检查，但是还是会存在错误，这里恳请广大读者批评指正，并发送邮件至BeyondVincent@devdiv.com,在此我们表示衷心的感谢。
注：本文原文地址：[信号&槽](http://doc.qt.nokia.com/4.7/signalsandslots.html)。
               第一章      信号&槽（Signals and Slots)
信号和槽通常用于对象间的通信。信号和槽机制是 Qt 的主要特性并且也很有可能是它与其他框架特性区别最大的部分。
第二章      简介
在GUI编程中，当我们想要改变某个widget时，通常想要其它widget获悉这一改变通知。更常用的情景是，我们想要各种类型的对象间能够互相通信。比如，当用户点击了“关闭”按钮，我们可能希望它调用“关闭”功能把窗口关掉。
Older toolkits使用回调**（callbacks）来达到这样的目的。回调是一个指向函数的指针，所以如果你希望一个处理函数通知你某些事件发生了，你可以传递一个指向其他函数的指针（回调）给它。这个处理函数将在适当的时候调用回调函数。回调函数有两个明显的缺点，第一，它们并不是类型安全，我们永远都不能确定调用者是否将通过正确的参数来调用“回调函数”；第二，回调函数与处理函数是紧耦合（strongly coupled）的，因为调用者必须知道应该在什么时候调用哪个回调函数。**
                                          第三章      信号&槽
Qt使用了信号和槽来代替回调函数。当一个特定的事件发生时，信号会被发送出去。Qt的窗体部件（widget）拥有众多预先定义好的信号，当然，我们也可以创建窗体部件（widget）的子类来为它们添加我们需要的自定义信号。槽，则是对一个特定的信号进行的反馈。Qt的窗体部件（widget）同样拥有众多预先定义好的槽，但是通常的做法是，创建窗体部件（widget）的子类并添加自定义槽，以便对感兴趣的信号进行处理。
![](http://www.devdiv.com/attachments/2010/10/1_2010102215034114fKj.jpg)
信号和槽机制是类型安全的（type-safe）：一个信号的参数必须和接收槽的参数匹配。（槽的参数可以比它接收的信号的参数短，第十章的范例很好的解释了这一点。）由于这种参数匹配机制，编译器以帮助我们检查类型不匹配的签名。信号与槽是松耦合（loosely coupled）的：一个发出信号的类既不知道也不关心哪一个槽接收到这个信号。Qt的信号和槽机制保证了如果你将一个信号连接到一个槽上，槽会在正确的时间以号的参数被调用。信号与槽可以携带任意个、任意类型的参数。他们是完全的类型安全。
所有从QObject或者它的一个子类（比如：QWidget）继承的类都可以使用号与槽。对象中以这种方式通信：一个对象的状态发生了改变并发送信号，关心这个改变的另一对像接收到这个信号。发送信号的对象并不知道也不感兴趣什么对象接收它所发出的信号，这是真正的信息封装，保证了对象能被当作软件组件来使用。
槽能被用来接收信号，除此之外它们也是普通的成员函数。槽不知道是否有信号与它连接起来，正如对象不知道它发出信号是否会被接收一样。这样的机制确保了可以使用Qt创建一个个完全独立的组件。
你可以把你感兴趣的多个信号与一个个槽进行连接，也可以把一个信号与多个槽进行连接。甚至可以直接把一个信号连接到另一个信号（当第一个信号发送出去的时候，第二个信号紧接着被发送）。
就这样，信号与插槽建立了强大的组件编程机制。
                                      第四章      一个小例子
一个最小的C++类声明可能表示为：
class Counter
 {
 public:
     Counter() { m_value = 0; }
     int value() const { return m_value; }
     void setValue(int value);
 private:
     int m_value;
 };
一个小的QObject-based类可能表示为：
#include <[QObject](http://doc.qt.nokia.com/4.7/qobject.html)>
 class Counter : public [QObject](http://doc.qt.nokia.com/4.7/qobject.html)
 {
     Q_OBJECT
 public:
     Counter() { m_value = 0; }
     int value() const { return m_value; }
 public slots:
     void setValue(int value);
 signals:
     void valueChanged(int newValue);
 private:
     int m_value;
 };
QObject-based版本与C++版本一样，有相同的内部状态，并能提供公共方法来访问这个状态，除此之外，QObject-based版本还可以利用信号和槽来支持组件编程。这个类通过发送一个valueChanged()信号类可以告知外界它的状态已经改变，同时它还有一个槽可以用来接收其他对象发来的信号。
所有包含信号或槽的类在他们声明的顶端都必须写上Q_OBJECT，同样也必须直接或间接继承自Qobject。
应用程序员需要实现槽。下面是Counter::setValue()槽的一种可能实现：
void Counter::setValue(int value)
 {
     if (value != m_value) {
         m_value = value;
*emit* valueChanged(value);
     }
 }
*emit*行从对象中以新的value值作为参数，发送信号valueChanged()。
在下面的代码段中，我们创建两个Counter对象，然后利用QObject::connect():将第一个对象的valueChanged()信号与第二个对象的setValue()槽连接起来。
Counter a, b;
QObject::connect(&a, SIGNAL(valueChanged(int)),
&b, SLOT(setValue(int)));
a.setValue(12);     //  a.value() == 12, b.value() == 12
b.setValue(48);        //   a.value() == 12, b.value() == 48
调用 a.setValue(12) 使a发送一个valueChanged(12) 信号，b将利用setValue() 槽来接收这个信号。比如，b.setValue(12)被调用，然后b发送相同的valueChanged()信号，但是因为没有槽与b的 valueChanged() 信号连接，因此这个信号被忽略。
注意，只有当value != m_value时，setValue()函数才可以设定值并且发送信号。这样就避免了循环连接时出现死循环（例如，如果b.valueChanged()与a.setValue()连接）。
一般情况下，发送一个信号的连接和发送两个信号的重复连接，都可以通过调用QObject::connect连接信号和槽时使用的参数。如果你在连接信号和槽时传递了Qt::UniqueConnection类型，只有它不是一个重复连接，连接才会成功。如果之前已经有了一个链接（相同的信号连接到同一对象的同一个槽上），那么连接将会失败并将返回false。
这个例子说明对象之间可以在不需要了解彼此信息的情况下相互协作。要实现这样的功能，对象之间仅需要互相连接就可以了，这种连接可以通过简单的调用QObject::connect() 函数或利用uic（User Interface Compiler，UI Designer工具之一，从.ui文件（XML样式）读取描述，然后生成C++代码。）的自动连接特性来完成。
                                        第五章      示例编译
Qt工程编译时，C++预处理器会删改 signals, slots与emit关键字,以便让标准的C++编译器能够识别。因为这些关键字是Qt内部自定义的，而不是C++标准的关键字。
通过对包含信号与槽的类定义运行 moc 程序,会产生一个相应的C++源文件,这个源文件将会被编译,并参与应用程序的目标文件(Object file)链接过程。如果你直接使用 qmake 命令, 它会把自动调用 moc 程序的makefile规则添加到工程的makefile文件中。
                                 第六章      信号(Signals)
信号在对象的内部状态改变的时候以某种方式发送出来,以通知该对象感兴趣的客户端或拥有者。只有在类中定义了信号才能在该类及其子类中发送信号。
当信号被发送时，一般情况下，与之相连的槽会立即被调用，这个过程与一般的函数调用是一样的。这一切的发生，信号与槽机制完全与任何的GUI事件循环无关。emit语句后面代码在所有的槽代码执行完毕之后将会继续执行。这个情况在使用“队列连接”([Queued Connections](http://doc.qt.nokia.com/4.7/qt.html#ConnectionType-enum))时，会有些不同，这种情况下，emit关键字后面的代码会立即执行，而槽会在之后被执行。
如果一个信号同时与好几个槽连接，那么在信号产生时，这些槽将按与信号连接的先后顺序，逐个调用。
信号由moc程序自动产生，并且不能在“.cpp”文件中实现。信号不能有返回值（只能使用void）。
关于信号函数的注释：经验显示，如果信号与槽的参数不使用一些特殊的类型，那么它们的重用性将更好。如果[QScrollBar::valueChanged](http://doc.qt.nokia.com/4.7/qabstractslider.html#valueChanged)()信号使用了一个特殊的参数，如：QScrollBar::Range，那么它仅可能被连接到为QScrollBar设计的特殊槽上。这是软件复用思想中最深恶痛绝的。这么做的话，该信号就不能与其他不同的窗口输入部件（Input
 Widget）中的槽连接了。
                                       第七章      槽(Slots)
      槽与信号连接之后,当信号发送时就会被调用。槽其实就是普通的C++函数,并且可以像普通函数一样调用.它们唯一的特点就是可以与信号相连。
      因为槽是普通的成员函数，所以当它们被直接调用时，它们也遵循C++函数调用规则。然而，它们也可以通过信号-槽之间的连接，由其它组件调用而不管槽的访问级别(如：private级别的槽)。换句话说，从任意的类实例中发送出来的信号都能调用一个不相关类中的private级别的槽。
      槽也可以用virutal定义，实际上，后面我们会发现这样子做很有用。
      与回调函数(callback)相比，信号与槽会稍微慢一些（译者注：大概慢一个数量级左右，但以现在的硬件来说，我们根本感觉不出来。），这是因为信号-槽机制更灵活。当然，实际应用程序上，它们的不同点是忽略不计的。一般来讲，引发一个连接了多个槽的信号，会比直接调用信号的接收者，慢10倍左右（这里不与虚函数调用比较）。这是因为，信号-槽机制需要定位连接对象，以确保安全地遍历所有的连接（例如，检查在信号发送过程中，所有要接收信号的对象未被销毁），以及参数的正反序列化，这些过程都必须占用花费。举个例子说明下，同时调用十个非虚函数听起来好像很多，但它的花费将比任意的new或delete操作少得多。试想下，你执行创建或销毁字符串，向量或列表的操作，它们将引发new或delete操作，与这些操作相比，信号-槽所需要的花费仅仅占整个函数调用花费的一小部分。
      在槽中进行系统调用的花费与上面讲述的类似。在一台i586-500机器上，你每秒可以进行2,000,000次的1对1信号-槽调用，或者1,200,000次的1对2信号-槽调用。信号-槽机制的简单性与灵活性，与它们所占花费比起来，那真是物超所值。当然客户在使用你的程序时，完全不会察觉到这种效率上的微小变化。
      注意，有些第三方库可能定义了一些变量，像signals或slots，这些库在Qt程序中使用时，这可能会引发编译器的警告或错误。要解决这个问题，使用#undef预处理器关闭这些预处理符号。
                     第八章      元对象(Meta-Object)信息
元对象编译器（meta-object compiler, moc）会解析C++文件中的类声明，并生成相应的C++代码以初始化元对象。元对象包含所有信号，槽成员的名称，以及这些函数的指针。
元对象也包含了一些额外的信息，如：对象的类名。这样子你就可以检查一个对象是否派生自一个特殊的类，如下代码所示：
        if (widget->[inherits](http://doc.qt.nokia.com/4.7/qobject.html#inherits)("QAbstractButton")) {
[QAbstractButton](http://doc.qt.nokia.com/4.7/qabstractbutton.html) *button = static_cast<QAbstractButton *>(widget);
         button->[toggle](http://doc.qt.nokia.com/4.7/qabstractbutton.html#toggle)();
  }
元对象信息也被qobject_cast<T>()操作符所使用，它与[QObject::inherits](http://doc.qt.nokia.com/4.7/qobject.html#inherits)()相似,但它更少出错
        if (QAbstractButton *button = qobject_cast<QAbstractButton *>(widget))
               button->[toggle](http://doc.qt.nokia.com/4.7/qabstractbutton.html#toggle)();
更多元对象系统信息参见: [元对象系统](http://doc.qt.nokia.com/4.7/metaobjects.html#meta-object-system)。
                                        第九章      范例分析
现在我们看一个简单的窗口部件示例代码，摘自LcdNumber类，代码有些与信号-槽不相关的成员函数已被省略。
        #ifndef LCDNUMBER_H
         #define LCDNUMBER_H
          #include <[QFrame](http://doc.qt.nokia.com/4.7/qframe.html)>
         class LcdNumber : public [QFrame](http://doc.qt.nokia.com/4.7/qframe.html)
         {
                  Q_OBJECT
      QFrame继承自QWidget，QWidget继承自QObject，从而使LcdNumber具有我们前面所讲的信号-槽特性。
      Q_OBJECT宏由预处理器展开，该宏声明了几个由moc实现的成员函数。如果编译时，你发现“undefined reference to vtable for LcdNumber”错误消息，那么你就有可能是忘了运行moc程序，或者是在链接程序中，没有包含moc的输出文件。
        public:
               LcdNumber(QWidget *parent = 0);
      LcdNumber的构造函数与moc的联系看上去没那么明显，但如果你从QWidget继承，那你可能想在你的构造函数中传递parent参数，并传递给基类的的构造函数。
      一些构造函数在此被省略了。moc程序会忽略成员函数。
        signals:
               void overflow();
      LcdNumber要显示的值超过它的值范围时，会发送一个信号。
      如果你不担心值溢出，或者你知道值溢出是不可能发生的，那么可以忽略overflow()信号，例如，不要让它与任何槽连接。
另一方面，如果你想在值溢出时，调用两个不同的错误处理函数，那么简单地将该信号与两个不同的槽连接即可。Qt会调用这两个槽（按它们的连接顺序）。
        public slots:
               void display(int num);
               void display(double num);
               void display(const QString &str);
               void setHexMode();
               void setDecMode();
               void setOctMode();
               void setBinMode();
               void setSmallDecimalPoint(bool point);
        };
        #endif
      上面的槽，用于接收其它窗口部件发生状态改变的信息。正如代码所示，LcdNumber用这些槽来显示数字。因为display()是LcdNumber类的接口，所以该槽的访问级别是public的。
      Qt说明文档中有好几个示例程序将QScrollBar的valueChanged()信号连接到了display()槽，这样子LcdNumber就能在滚动条滚动时，不断的更新显示的值了。。
      注意display槽的重载。当你将信号与槽连接时，Qt会选择一个最接近的版本与信号相连。如果使用回调函数，你就得寻找五个不同的名称，并且你要确保函数签名是正确的。
                             第十章      信号-槽的默认参数
信号与槽的签名包含参数，且参数可能含有不同的默认值。让我们看下[QObject::destroyed](http://doc.qt.nokia.com/4.7/qobject.html#destroyed)():
void destroyed(QObject* = 0);
当QObject被销毁时，它会发送QObject::destroyed()信号。当我们想对被删除的QObject对象进行一些处理时，例如，做些清理工作，就可以捕捉该信号。一种接收该信号的槽的函数签名可能像下面这样：
        void objectDestroyed(QObject* obj = 0);
为了将信号与-槽相连接，我们会用到QObject::connect()，SIGNAL()和SLOT（）宏。在SIGNAL()与SLOT（）宏中是否要包含参数，有以下规则：如果信号的参数有默认值，那么传递给SIGNAL()宏的参数个数不能少于SLOT()宏的参数个数。
下面这些代码都会正常工作：
        connect(sender, SIGNAL(destroyed(QObject*)), this,
               SLOT(objectDestroyed(Qbject*)));
        connect(sender, SIGNAL(destroyed(QObject*)), this,
               SLOT(objectDestroyed()));
  connect(sender, SIGNAL(destroyed()), this, SLOT(objectDestroyed()));
但下面的代码就不能正常工作：
connect(sender, SIGNAL(destroyed()), this,
SLOT(objectDestroyed(QObject*)));
由于槽需要得到一个QObject作为参数，而信号却不带这个参数,会导致这个信号-槽连接报一个运行时错误，这种错误往往很难找出来。
                           第十一章   信号-槽的高级用法
有时候,你可能想要获取信号发送者的信息,这样子我们就可以判断是谁发出的信号。Qt提供了[QObject::sender](http://doc.qt.nokia.com/4.7/qobject.html#sender)()函数，它返回一个指向信号发送者的指针。
有时候，会出现一个槽连接多个信号，而这个槽可能需要对不同的信号执行不同的处理方式，[QSignalMapper](http://doc.qt.nokia.com/4.7/qsignalmapper.html)类就是为这种情况设计的。
假设你有三个不同的按钮，它们分别用于打开不同的文件，如：传真文件，账号文件与报表文件。
为了打开正确的文件，你可以使用[QSignalMapper::setMapping](http://doc.qt.nokia.com/4.7/qsignalmapper.html#setMapping)()将所有按钮的clicked()信号映射到[QSignalMapper](http://doc.qt.nokia.com/4.7/qsignalmapper.html)对象，然后你就可以将[QPushButton::clicked](http://doc.qt.nokia.com/4.7/qabstractbutton.html#clicked)()信号与[QSignalMapper::map](http://doc.qt.nokia.com/4.7/qsignalmapper.html#map)()槽相连接，如下代码所示：
        signalMapper = new QSignalMapper(this);
        signalMapper->setMapping(taxFileButton, [QString](http://doc.qt.nokia.com/4.7/qstring.html)("taxfile.txt"));
        signalMapper->setMapping(accountFileButton,
[QString](http://doc.qt.nokia.com/4.7/qstring.html)("accountsfile.txt"));
        signalMapper->setMapping(reportFileButton, [QString](http://doc.qt.nokia.com/4.7/qstring.html)("reportfile.txt"));
        connect(taxFileButton, SIGNAL(clicked()),
         signalMapper, SLOT (map()));
        connect(accountFileButton, SIGNAL(clicked()),
         signalMapper, SLOT (map()));
        connect(reportFileButton, SIGNAL(clicked()),
         signalMapper, SLOT (map()));
然后将[mapped()](http://doc.qt.nokia.com/4.7/qsignalmapper.html#mapped)信号与readFile()槽相连，这个槽依据不同的按钮点击事件，打开不同的文件。
        connect(signalMapper, SIGNAL(mapped(QString)),
               this, SLOT(readFile(QString)));
注：下面的代码也可以编译运行，但由于签名规范化（signature normalization，查了GOOGLE，似乎找不到这词的中文翻译，个人认为应该是函数签名的转换，译者注），这些代码的运行效率会低些
        //由于函数签名运行时的规范化，这样子会慢些
        connect(signalMapper, SIGNAL(mapped(const QString &)),
         this, SLOT(readFile(const QString &)));
            第十二章   在Qt中使用第三方库的信号与槽
也可以在Qt中使用第三方库的信号-槽机制。甚至可以在同一个项目中同时使用这两种机制。你只需向项目的qmake规则中添加如下代码即可：
CONFIG += no_keywords
这行代码告诉Qt不要定义moc关键字signals，slots与emit，这些关键字由第三方库使用，例如：Boost库。为了在no_keywords标志下继续使用Qt信号和槽，只要将源代码中原来使用到signals,slots,emit等关键字的地方，换成[Q_SIGNALS](http://doc.qt.nokia.com/4.7/qobject.html#Q_SIGNALS) (或[Q_SIGNAL](http://doc.qt.nokia.com/4.7/qobject.html#Q_SIGNAL)), [Q_SLOTS](http://doc.qt.nokia.com/4.7/qobject.html#Q_SLOTS) (或[Q_SLOT](http://doc.qt.nokia.com/4.7/qobject.html#Q_SLOT))和[Q_EMIT](http://doc.qt.nokia.com/4.7/qobject.html#Q_EMIT)这些Qt宏即可。
参见：[元对象系统](http://doc.qt.nokia.com/4.7/metaobjects.html#meta-object-system) 与 Qt'属性系统。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**[C++信号&插槽库: C++ Signal/Slot Library](http://www.cppblog.com/xinus/archive/2010/10/27/signal_slot_libraries.html)**
在此记录几个signal & slot 的实现库，如下：
1. QT
[http://qt.nokia.com/](http://qt.nokia.com/)
Qt是本人喜欢并熟悉的一个cross-platform库，用来创建跨平台的应用程序非常方便。不过这不是推荐的重点，因为没办法把它用到我们自己的非QT程序中。不过QT借助自己实现moc预处理器，将signal和slot的声明，完美的融合到了C++类声明中是非常不错的考虑。
2. boost::signal & boost::signal2
[http://www.boost.org/](http://www.boost.org/)
大名鼎鼎的boost库中的子库，拥有boost其它模版库的完美支持。boost库值得去研究一下，不过只打算在自己的程序中使用signal &slot机制，还是不推荐该库。
3. libsigc++
[http://libsigc.sourceforge.net/](http://libsigc.sourceforge.net/)
这个库已经用在了gtkmm中，与boost::signal类似。
4. sigslot  **(Attention: WebRTC采用)**
[http://sigslot.sourceforge.net/](http://sigslot.sourceforge.net/)
一个非常精简的signal & slot 实现库，整个库只有一个<< sigslot.h >>文件。
Signal & slot 比较好的实现了对象间的解耦，在GUI框架中应用广泛。C++中signal & slot的实现，几乎都用到了Template, Delegation,  Functor等。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
![](http://hi.csdn.net/attachment/201110/14/0_1318561711PxyO.gif)
![](http://hi.csdn.net/attachment/201110/14/0_13185617360wRo.gif)
+++++++++++++++
**[QT学习笔记-4.信号与插槽](http://www.cppblog.com/gaimor/archive/2011/03/15/141857.html)**
**本文主要是对C++ GUI Programming with Qt4一书 Signals and Slots in Depth 部分的翻译。**
**信号与插槽机制是Qt编程的基础.它可以绑定对象而不需要对象之间彼此了解。槽类似于c++中的成员函数他可以是虚拟的，可重载的，私有的，公开的，受保护的。**
不同点式槽可以链接到信号。通过这种方式可以在每次信号发射的的时候做到调用槽函数
connect()语句是这样的
connect(sender, SIGNAL(signal), receiver, SLOT(slot));
在这里sender和receiver是指向信号对象和槽对象的指针。宏SIGNAL()和SLOTS()负责转换他们的参数到字符串。
当然一个信号可以连接到多个槽(似乎都是这样的)
connect(slider, SIGNAL(valueChanged(int)),
        spinBox, SLOT(setValue(int)));
connect(slider, SIGNAL(valueChanged(int)),
this, SLOT(updateStatusBarIndicator(int)));
同样多个信号可以连接到单个槽
例如:
connect(lcd, SIGNAL(overflow()),
this, SLOT(handleMathError()));
connect(calculator, SIGNAL(divisionByZero()),
this, SLOT(handleMathError()));
除此之外信号可以连接到其他信号(见过的其他插槽系统似乎不大可以?)
connect(lineEdit, SIGNAL(textChanged(const QString &)),
this, SIGNAL(updateRecord(const QString &)));
需要指出的是信号信号链接和信号插槽连接时不同的
既然信号和插槽可以连接那么他们应该可以断开，如下：
disconnect(lcd, SIGNAL(overflow()),
this, SLOT(handleMathError()));
一个简单的例子:
class Employee : public QObject
{
    Q_OBJECT
public:
    Employee() { mySalary =0; }
int salary() const { return mySalary; }
public slots:
void setSalary(int newSalary);
signals:
void salaryChanged(int newSalary);
private:
int mySalary;
};
void Employee::setSalary(int newSalary)
{
if (newSalary != mySalary) {
        mySalary = newSalary;
        emit salaryChanged(mySalary);
    }
}
说明
关键字 public slots:和signals 他们用于修饰插槽函数和信号函数;
至于信号的发射通过关键字 emit来实现
通过本文基本掌握了QT的信号插槽机制

