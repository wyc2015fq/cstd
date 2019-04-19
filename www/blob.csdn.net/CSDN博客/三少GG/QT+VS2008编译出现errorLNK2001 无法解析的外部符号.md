# QT+VS2008编译出现errorLNK2001:无法解析的外部符号 - 三少GG - CSDN博客
2013年06月15日 01:11:25[三少GG](https://me.csdn.net/scut1135)阅读数：2578

若出现如下错误：
错误 1 error LNK2001: 无法解析的外部符号 "public: virtual struct Q[Meta](http://www.haogongju.net/tag/meta)Object const * __thiscall Widget::metaObject(void)const
 " (?metaObject@Widget@@UBEPBUQMetaObject@@XZ) 
错误 2 error LNK2001: 无法解析的外部符号 "public: virtual void * __thiscall Widget::qt_metacast(char const *)" (?qt_metacast@Widget@@UAEPA[XP](http://www.haogongju.net/tag/XP)BD@Z) 
错误 3 error LNK2001: 无法解析的外部符号 "public: virtual int __thiscall Widget::qt_metacall(enum QMetaObject::Call,int,void * *)" (?qt_metacall@Widget@@UAEHW4Call@QMetaObject@@HPAPAX@Z)
错误 4 fatal error LNK1120: 3 个无法解析的外部命令
查找原因：
1. 是否加上了Q_OBJECT宏？
2.  widget里用到signal slot了吧？（也许是后续加的）
3. 类定义在cpp里？ 没有相应的.h文件？
4. 如果以前没信号槽新增加的， 那么makefile需要重新生成。
总结的原因如下：
vs在编译时会产生一系列的命令以控制该.h、.cpp文件的编译过程，并且，以后不会自动修改这些命令。
利用Qt[编程](http://www.haogongju.net/tag/%E7%BC%96%E7%A8%8B)时，写好.h、.cpp文件并第一次编译之后，若.h文件中定义类时没有添加Q_OBJECT宏以及信号函数、槽函数，VS编译命令中就没有moc命令，就不会调用Qt的moc工具以生成moc_*.cpp文件；并且即使添加了Q_OBJECT宏之后，**VS也不会自动添加moc命令，因而不会生成moc文件。**VS在LINK时，**找不到moc文件，就出现了error LNK2001的错误**。
解决方法：假设代码文件分别为hello.h、hello.cpp、ui_hello.h。需要编译生成并添加上moc_hello.cpp文件。
右击hello.h，选择“自定义生成步骤”，“常规”
　　命令行：moc.exe hello.h -o moc_hello.cpp
　　输出：moc_hello.cpp
　　附加依赖项：moc.exe hello.h
　　确定，然后，右击hello.h，选择 “编译”，则在文件夹中生成moc_hello.cpp，**再将其添加到vs的源文件列表中**。
