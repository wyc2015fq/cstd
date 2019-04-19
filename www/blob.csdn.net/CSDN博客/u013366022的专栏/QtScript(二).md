# QtScript(二) - u013366022的专栏 - CSDN博客
2015年03月30日 14:35:23[slitaz](https://me.csdn.net/u013366022)阅读数：625
   Qt 包含完全集成的 ECMA 标准脚本引擎。Qt Script 集成了 QObject，为脚本提供了 Qt 的信号与槽 (Signals & Slots) 机制，可在 C++ 和脚本之间进行集成。 脚本支持可使开发事半功倍，QtScript提供脚本错误报告，用户可用自己的脚本跟踪程序缺陷.
   下面是一个简单的hello程序。
**[cpp]**[view
 plain](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)[copy](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)
- //main.cpp
- #include <QtGui/QApplication>
- #include <QMessageBox>
- #include <QtScript/QtScript>
- #include <QPushButton>
- #include "mainwindow.h"
- 
- int main(int argc, char *argv[])  
- {  
-     Q_INIT_RESOURCE(helloscript);  
-     QApplication app(argc, argv);  
-     QScriptEngine engine;  
- 
- //国际化
-     QTranslator translator;  
-     translator.load("helloscript_la");  
-     app.installTranslator(&translator);  
-     engine.installTranslatorFunctions();  
- 
-     QPushButton button;  
-     QScriptValue scriptButton = engine.newQObject(&button);//生成C++对象的代理脚本对象
-     engine.globalObject().setProperty("button", scriptButton);//将对象注册到javascript
- 
- //读取js文件
-     QString fileName(":/helloscript.js");  
-     QFile scriptFile(fileName);  
-     scriptFile.open(QIODevice::ReadOnly);  
-     QTextStream stream(&scriptFile);  
-     QString contents = stream.readAll();  
-     scriptFile.close();  
- 
-     QScriptValue result = engine.evaluate(contents, fileName);//执行脚本
- if(result.isError()) {//解析js文件是否有错误
-         QMessageBox::critical(0, "Hello Script",  
-                               QString::fromLatin1("%0:%1: %2")  
-                               .arg(fileName)//文件名
-                               .arg(result.property("lineNumber").toInt32())//错误行号
-                               .arg(result.toString()));//错误信息
- return -1;  
-     }  
- return app.exec();  
- }  

**[javascript]**[view
 plain](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)[copy](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)
- //helloscript.js
- button.text = qsTr('Hello World');//note that the qTr() function is used to allow for translation
- button.styleSheet = 'font-style: italic; color: #12FF12';  
- button.show();//调用show()槽函数
- button.clicked.connect(  
- function(){  
-        print('Hello QtScript');  
-     }  
- );  
记住还需要在.pro文件中加入：QT       += script
    1、QScriptEngine类为程序提供一个嵌入式脚本环境。一个应用程序中可以添加多个脚本引擎；每一个引擎都是一个轻量级自包含的虚拟机。通过调用脚本引擎的evaluate()函数可以执行脚本.
**[c-sharp]**[view
 plain](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)[copy](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)
- QScriptEngine engine;  
- QScriptValue result = engine.evaluate("(3*2)-1");  
- qDebug() << "Result: " << result.toInt32();  

**[cpp]**[view
 plain](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)[copy](http://blog.csdn.net/liuhongwei123888/article/details/6162159#)
- QScriptEngine engine;  
- QScriptValue func = engine.evaluate("(function(a, b) { return a+b;})");  
- QScriptValueList args;  
- args << 1 << 2;  
- QScriptValue result = func.call(QScriptValue(), args);  
- qDebug() << "Result: " << result.toInt32();  
    2、QScriptValue是一个Qt Script数据类型的容器,支持ECMA-262定义的类型，如：
     原始数据类型：Undefined,Null,Boolean,Number,String,对象类型。
     另外Qt Script还内建支持QVariant,QObject和QMetaObject.
      >>对于基对象(object-based)类型（包括Date和RegExp)，使用QScriptEngine对象的newT()函数（如QScriptEngine::newObject())创建一个你期望的QScriptValue类型。
      >>对于原始数据类型,直接使用QScriptValue构造函数加载。
      isT()方法能够被用来测试一个值是否是你确定的数据类型。toT()方法用来将QScriptValue转换成其他类型。当然你也可以用qscriptvalue_cast()方法.
