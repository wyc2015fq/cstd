# Qt5 和 Qt4 的一些改动和不同 - Grandyang - 博客园







# [Qt5 和 Qt4 的一些改动和不同](https://www.cnblogs.com/grandyang/p/4296393.html)







Qt5从Qt4升级后，很多原来Qt4中定义的函数或者变量由于种种原因在Qt5中被遗弃，所以用Qt5来编译Qt4的一些工程项目时，难以避免的会需要修改一些地方，下面就罗列一些我遇到的一些需要修改的地方：



- Qt5中UnicodeUTF8不再是QCoreApplication的一个成员，所以下面的代码需要修改：

例如：

```
YourClass->setWindowTitle(QApplication::translate(YourClass, “List”, 0, QApplication::UnicodeUTF8));
```

需要修改为：

```
YourClass->setWindowTitle(QApplication::translate(YourClass, “List”, 0));
```



- Qt5中的QString不再定义toAscii()函数，所以所有出现该函数的地方需要用toLatin1()来替换：

例如：

```
this->_cvCap = cvCaptureFromAVI(path.toAscii().constData());
```

需要修改为：

```
this->_cvCap = cvCaptureFromAVI(path.toLatin1().constData());
```














