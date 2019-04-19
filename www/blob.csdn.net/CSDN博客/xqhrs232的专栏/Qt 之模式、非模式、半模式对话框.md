# Qt 之模式、非模式、半模式对话框 - xqhrs232的专栏 - CSDN博客
2018年06月11日 17:30:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：156
原文地址::[https://blog.csdn.net/liang19890820/article/details/51067310](https://blog.csdn.net/liang19890820/article/details/51067310)
相关文章
1、QT模态对话框----[https://blog.csdn.net/imxiangzi/article/details/47982965](https://blog.csdn.net/imxiangzi/article/details/47982965)
# 简述
关于“模式”和“非模式”对话框，相信大家都比较熟悉，但其中有一个可能很多人都比较陌生，介于两者之间的状态，我们称之为“半模式“。
| 版权声明：一去、二三里，未经博主允许不得转载。
# 模式对话框
## 描述
阻塞同一应用程序中其它可视窗口输入的对话框。模式对话框有自己的事件循环，用户必须完成这个对话框中的交互操作，并且关闭了它之后才能访问应用程序中的其它任何窗口。模式对话框仅阻止访问与对话相关联的窗口，允许用户继续使用其它窗口中的应用程序。
显示模态对话框最常见的方法是调用其exec()函数，当用户关闭对话框，exec()将提供一个有用的返回值，并且这时流程控制继续从调用exec()的地方进行。通常情况下，要获得对话框关闭并返回相应的值，我们连接默认按钮，例如：”确定”按钮连接到accept()槽，”取消”按钮连接到reject()槽。另外我们也可以连接done()槽，传递给它Accepted或Rejected。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160405193105350)
## 源码
```
MainWindow *pMainWindow = new MainWindow();
pMainWindow->setWindowTitle(QStringLiteral("主界面"));
pMainWindow->show();
CustomWindow *pDialog = new CustomWindow(pMainWindow);
pDialog->setWindowTitle(QStringLiteral("模式对话框"));
// 关键代码
pDialog->exec();
// 关闭模态对话框以后才会执行下面的代码
pMainWindow->setWindowTitle(QStringLiteral("主界面-模式对话框"));
qDebug() << QStringLiteral("关闭模态对话框以后，可以继续向下执行");
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 主界面被阻塞，不能进行点击、拖动等任何操作。
- exec()之后的代码不会执行，直到关闭模态对话框。
# 非模式对话框
## 描述
和同一个程序中其它窗口操作无关的对话框。在文字处理中的查找和替换对话框通常是非模式的，允许用户同时与应用程序的主窗口和对话框进行交互。调用show()来显示非模式对话框，并立即将控制返回给调用者。
如果隐藏对话框后调用show()函数，对话框将显示在其原始位置，这是因为窗口管理器决定的窗户位置没有明确由程序员指定，为了保持被用户移动的对话框位置，在closeEvent()中进行处理，然后在显示之前，将对话框移动到该位置。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160405192916896)
## 源码
```
MainWindow *pMainWindow = new MainWindow();
pMainWindow->setWindowTitle(QStringLiteral("主界面"));
pMainWindow->show();
CustomWindow *pDialog = new CustomWindow(pMainWindow);
pDialog->setWindowTitle(QStringLiteral("非模式对话框"));
// 关键代码
pDialog->show();
// 下面的代码会立即运行
pMainWindow->setWindowTitle(QStringLiteral("主界面-非模式对话框"));
qDebug() << QStringLiteral("立即运行");
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 主界面不会被阻塞，可以进行点击、拖动等任何操作。
- show()之后的代码会立即执行。
# 半模式对话框
## 描述
调用setModal(true)或者setWindowModality()，然后show()。有别于exec()，show() 立即返回给控制调用者。
对于进度对话框来说，调用setModal(true)是非常有用的，用户必须拥有与其交互的能力，例如：取消长时间运行的操作。如果使用show()和setModal(true)共同执行一个长时间操作，则必须定期在执行过程中调用QApplication ::processEvents()，以使用户能够与对话框交互（可以参考QProgressDialog）。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160405192741036)
## 源码
```
MainWindow *pMainWindow = new MainWindow();
pMainWindow->setWindowTitle(QStringLiteral("主界面"));
pMainWindow->show();
CustomWindow *pDialog = new CustomWindow(pMainWindow);
pDialog->setWindowTitle(QStringLiteral("半模式对话框"));
// 关键代码
pDialog->setModal(true);
pDialog->show();
// 下面的代码会立即运行
pMainWindow->setWindowTitle(QStringLiteral("主界面-半模式对话框"));
qDebug() << QStringLiteral("立即运行");
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 主界面被阻塞，不能进行点击、拖动等任何操作。
- show()之后的代码会立即执行。
# 更多参考
- [QWidget、QDialog、QMainWindow的异同点](http://blog.csdn.net/liang19890820/article/details/50533262)
