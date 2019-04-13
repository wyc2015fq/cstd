
# Qt信号和槽 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:29:51[seven-soft](https://me.csdn.net/softn)阅读数：260个人分类：[Qt																](https://blog.csdn.net/softn/article/category/6274094)



GUI 程序除了要绘制控件，还要响应系统和用户事件，例如重绘、绘制完成、点击鼠标、敲击键盘等。当事件发生时，UI 会产生相应的变化，让用户直观地看到。
大部分编程（例如Win SDK、Web前端）中使用回调函数来响应事件，而 Qt 却独创了信号和槽机制。所谓回调函数，就是程序员提前定义一个函数，当事件发生时就调用该函数。
信号和槽是 Qt 的核心，它让两个互不相干的对象连接起来，当一个对象的状态改变时，可以通知另一个对象。
我们先通过例子来演示一下信号和槽：\#include<QApplication>
\#include<QMainWindow>
\#include<QLabel>
\#include<QPushButton>
\#include<QLineEdit>
intmain(intargc,char*argv[])
{
QApplicationapp(argc,argv);
QMainWindoww;
w.setWindowTitle("Widget工程");
w.resize(325,120);
QLineEditlineEdit(&w);
lineEdit.setGeometry(30,20,180,36);
lineEdit.setPlaceholderText("请输入文本");
QPushButtonbtn("取消",&w);
btn.setGeometry(220,20,70,36);
QLabellabel(&w);
label.setGeometry(30,70,250,30);
//连接clicke()信号和quit()槽
QObject::connect(&btn,SIGNAL(clicked()),&app,SLOT(quit()));
//连接textChanged()信号和setText()槽
QObject::connect(&lineEdit,SIGNAL(textChanged(QString)),&label,SLOT(setText(QString)));
w.show();
returnapp.exec();
}

该代码中创建了三个控件，一个文本输入框(LineEdit)、一个按钮(PushButton)和一个Label，他们都是主窗口 w 的子对象。运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/151218/1-15121PTS3146.png)
点击“取消”按钮，程序就关闭了，这是第25行代码的作用；在文本输入框中输入一段文本，下面的 Label 会随时显示出来，这是第27行代码的作用。
先关注第25行代码。clicked() 是按钮 btn 的信号，quit() 是 app 的槽，connect() 函数将它们连接起来，就像“楔子”插入“凹槽”中。用户点击按钮时会发出 clicked() 信号，然后由
 quit() 槽接收并关闭程序。
信号是只有函数声明、没有函数体的成员函数。槽是拥有完整函数体的普通成员函数，你可以在槽函数中实现各种功能，与普通函数相比并没有区别，例如 quit() 的作用就是退出程序。
connect() 是 QObject 类的静态成员函数；QObject 是 Qt 中所有类的基类，它就像“树根”，从这里派生出了所有其他“树枝”。

需要注意的是，信号不是事件。当用户点击“取消”按钮时，Qt 会捕获该点击事件，进行预处理，然后发射 clicked() 信号； clicked() 和 quit() 关联起来了，接下来就会调用 quit() 函数。
信号和槽机制归根结底也是回调函数，只不过绕了个圈子。在这种机制下，程序员有两次处理事件的机会，一是在捕获事件后发射信号前进行预处理（事件不符合预期可以不发射信号），二是在槽函数中进行主要处理。
再来看第27行。textChange() 信号会在文本改变时发出，setText() 槽用来设置 Label 的文本，QString 是要传递的数据的类型。当用户输入文本时，lineEdit 会发出 textChange()
 信号，该信号将携带数据，数据类型为 QString，数据内容为输入的文本；setText() 槽接收到信号后先解析信号携带的数据，获取用户输入的文本，然后填充到 Label 中。
## 关于 connect()
connect() 是 QObject 类的静态成员函数，它有多个原型：connect(QObject*sender,char*signal,
QObject*receiver,char*method);
connect(QObject*sender,PointerToMemberFunctionsignal,
QObject*receiver,PointerToMemberFunctionmethod);
connect(QObject*sender,PointerToMemberFunctionsignal,
QObject*context,Functorfunctor);
connect(QObject*sender,QMetaMethod&signal,
QObject*receiver,QMetaMethod&method);
connect(QObject*sender,PointerToMemberFunctionsignal,Functorfunctor);

简单起见，上面省略了 connect() 的返回值和最后一个参数，以及某些参数前面的 const 修饰符，读者可以在 Qt 帮助手册中查看完整的原型。
> connect() 函数返回值类型为
> QMetaObject::Connection
> ，表示当前连接句柄。最后一个参数为
> Qt::ConnectionType
 type = Qt::AutoConnection
> ，表示连接类型，一般默认即可。
观察上面的原型，除了最后一个有3个参数，其他都有4个参数，其中：
1) sender 为信号发送者，receiver 为信号接收者，它们都是对象指针。
2) 第1个原型中，signal 为信号，method 为槽函数，它们都是字符串，必须借助 SIGNAL() 和 SLOT() 将函数形式转换为字符串形式。SIGNAL() 和 SLOT() 是宏，而非函数。上面的示例中就使用了该原型，它是常用的原型，初学者必须要掌握。
3) 第2个原型中，PointerToMemberFunction 为指向成员函数的指针。你可以将示例中的代码做如下更改：QObject::connect(&btn, &QPushButton::clicked, &app, &QApplication::quit);
QObject::connect(&lineEdit, &QLineEdit::textChanged, &label, &QLabel::setText);这是 Qt 5 新增的原型，可以在编译期间进行检查，如果信号和槽不存在或者不匹配，则会报错。而第1种原型是从 Qt 诞生以来一直支持的，不能在编译期进行检测，如果信号和槽有误，只会在程序运行期间给出警告并返回 false，不容易发现问题，这是它的一个缺陷。所以在 Qt 5 中我们鼓励使用第2种原型。

