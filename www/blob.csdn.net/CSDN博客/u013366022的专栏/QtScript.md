# QtScript - u013366022的专栏 - CSDN博客
2015年03月30日 14:34:01[slitaz](https://me.csdn.net/u013366022)阅读数：404
QtScript 模块起源于 QSA，在Qt4.6时，QtScript 进行过完全重写。原来的QtScript 以 Qt Script classic的名字放到了 Qt Solutions 中。
对 javascript 几乎一窍不通，主要学习一下 脚本代码 与 C++ 代码的整合。
## 练习一
- 为简单起见，假定所有的script文件都已被读入到一个字符串中
- 创建一个 QScriptEngine 的实例
- 执行 script脚本
- 脚本结果存放在 QScriptValue 中
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtScript>
const char script[]="1+2";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QScriptEngine engine;
    QScriptValue res = engine.evaluate(script);
    if (res.isError())
        qDebug()<<"Error";
    else
        qDebug()<<res.toNumber();
    return a.exec();
}
### 错误处理
当脚本执行出错时，返回一个Error对象，就像我们前面所做的，使用 isError 可判断。但若要获得更多的出错信息：
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtScript>
const char script[]="1+a";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QScriptEngine engine;
    QScriptValue res = engine.evaluate(script);
    if (engine.hasUncaughtException()){
        qDebug()<<engine.uncaughtException().toString();
        qDebug()<<engine.uncaughtExceptionBacktrace().join("/n");
    }else{
        qDebug()<<res.toNumber();
    }
    return a.exec();
}
可以使用 hasUncaughtException 来进行判断。程序结果如下：
"ReferenceError: Can't find variable: a"
"<anonymous>()@:1"
如果我们这儿的脚本是从hello.js文件的第3行读入的，我们可以传递额外的信息：
QScriptValue res = engine.evaluate(script, "hello.js", 3);
这样会得到更友好的信息：
"ReferenceError: Can't find variable: a"
"<anonymous>()@hello.js:3"
### globalobject()
这也是一个 QScriptValue，通过设置它的属性，可以在脚本中访问C++中的数据和对象
接前面的例子，在执行脚本前，添加一句：
engine.globalObject().setProperty("a", 797);
使得整数797在脚本中作为a被访问，这样错误即可消失。
## 练习二
- 在C++ 中通过信号槽使用 script 中的函数
- 效果：点击按钮，调用 script 中的函数。
#include <QtGui>
#include <QtScript>
const char script[]="(function() { print('hello script'); })";
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPushButton button("Click Me");
    QScriptEngine engine;
    QScriptValue func = engine.evaluate(script);
    qScriptConnect(&button, SIGNAL(clicked()), engine.globalObject(), func);
    button.show();
    return app.exec();
}- 信号槽的连接，使用 qScriptConnect 函数，大致和QObject::connect用法差不多。只不过后两个参数都是 QScriptValue 对象了。
- 
为了代码少点，此处有意省略了错误检查。在这是代码中是不可少的，不然可能都不知道自己怎么死的 :-)
注意：
- 我们的脚本被一对圆括号扩住了，而这在Qt4.5及以前是不需要的，因为Qt4.6采用 javascriptcore核心改写，故采用了和 javascript 的eval一致的用法。
- 或者，不使用匿名函数
const char script[]="f = function() { print('hello script'); }";
## 练习三
- 使我们的QObject子类的对象在脚本中能够被访问
#include <QtCore>
#include <QtGui>
#include <QtScript>
const char script[]="lineedit.setText('hello script')";
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLineEdit lineEdit;
    QScriptEngine engine;
    QScriptValue edit = engine.newQObject(&lineEdit);
    engine.globalObject().setProperty("lineedit", edit);
    engine.evaluate(script);
    lineEdit.show();
    return app.exec();
}- 通过 QScriptEngine 的 newQObject 为对象创建一个包装器(一个QScriptValue)
- 将该包装器设置为 globalObject 的属性，使得对象能以 lineedit 名字在脚本中被访问
## 练习四
- 在脚本中连接信号与槽
#include <QtCore>
#include <QtGui>
#include <QtScript>
const char script[]="var obj = {fun:function(){print('hello script');}};"
                    "btn.clicked.connect(obj, 'fun')";
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPushButton button("Click Me");
    QScriptEngine engine;
    QScriptValue btn = engine.newQObject(&button);
    engine.globalObject().setProperty("btn", btn);
    engine.evaluate(script);
    button.show();
    return app.exec();
}
上面的 btn.clicked.connect(obj, 'fun') 也可写为
btn.clicked.connect(obj.fun)
### 发射信号
脚本中发射信号，直接调用信号函数即可。
const char script[]="var obj = {fun:function(){print('hello script');}}/n"
                    "btn.clicked.connect(obj, 'fun')/n"
                    "btn.clicked()";
信号必须在C++中定义，script中不能定义信号
