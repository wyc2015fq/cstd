# qt creator单步调试 - xqhrs232的专栏 - CSDN博客
2018年05月14日 14:07:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：983
原文地址::[https://blog.csdn.net/dreamtdp/article/details/8765165](https://blog.csdn.net/dreamtdp/article/details/8765165)
相关文章
1、Qt断点调试怎么看一个变量的值啊，添加到监视窗口也没找到监视窗口在哪里啊。----[https://bbs.csdn.net/topics/390364772/](https://bbs.csdn.net/topics/390364772/)
PS：有多个重载构造 函数时，按键盘中的上，下，左，右键选择。
调试器是MinGW
Qt Creator 运行时是先构造
Notepad  w 对象，运行到Notepad构造函数那里，看变量值右击添加表达式求值，就可以看了。    QApplication a(argc, argv);    Notepad w;    char s[20]="123";    strcpy(s,"456456");    w.show();    return a.exec();
调试信号和槽的话，要在槽的实现函数设置断点，再发送信号，如pushbotton,就会跳到槽的实现函数：voidNotepad::on_actionOpen_triggered(){QStringfileName=QFileDialog::getOpenFileName(this,tr("Open File"),QString(),tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));if(!fileName.isEmpty()){QFilefile(fileName);if(!file.open(QIODevice::ReadOnly)){QMessageBox::critical(this,tr("Error"),tr("Could not open file"));return;}QTextStreamin(&file);ui->textEdit->setText(in.readAll());file.close();}}
Qt Creator的常用快捷键给列一下。
F1:  选中某一类或函数，按下F1，出现帮助文档；
F2： 选中某一类或函数，按下F2，迅速定位到给类或函数申明的地方或被调用的地方；
F3:  查找時按F3查找下一個你選中的內容，Shift +　F３為查找上一個；
F4： 在*.cpp  和*.h之間切換；
调试 F5(Debug)
停止调试 Shift+F5
单步跳过 F10
单步进入 F11
单步跳出 Shift + F11
重启调试 Ctrl + Shift + F5
设置/切换 断点 F9
激活欢迎模式 Ctrl + 1
激活编辑模式 Ctrl + 2
激活调试模式 Ctrl + 3
激活项目模式 Ctrl + 4
激活帮助模式 Ctrl + 5
激活输出模式 Ctrl + 6
查找当前文档 Ctrl + F
查找下一个 F3
查找上一个 Shift + F3
弹出查找对话框 Ctrl + Shift + F
返回到代码编辑状态 Esc
页面之间切换 Alt +左，按Alt +右
自动完成 Ctrl + Space
声明和定义之间切换 F2
头文件和源文件之间切换 F4
隐藏或显示边栏 Alt + 0
切换至生成窗口 Alt + 1
切换至搜索结果窗口 Alt + 2
切换至应用输出窗口 Alt + 3
切换至编译输出窗口 Alt + 4
下一个书签 Ctrl + .
上一个书签 Ctrl + ,
书签之间切换 Ctrl + M
