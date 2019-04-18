# 使用PyQT编写界面程序 - wishchinYang的专栏 - CSDN博客
2014年06月26日 10:26:37[wishchin](https://me.csdn.net/wishchin)阅读数：2267
                
使用PyQT比QT好在，可以随时监测函数正确性，省去编译时间 ! 这是个不小的节省.
**1. PyQt: 打开对话框**
```python
```
       msgbox = QtGui.QMessageBox(self)# 我的语句是 msgbox = QtGui.QMessageBox(self.Centerwidget)
       msgbox.setWindowTitle('warning')
       msgbox.setText("hello")
       msgbox.show()
**2.  详细教程，一定要参考网页**：[http://www.cppblog.com/mirguest/archive/2012/02/12/165386.html](http://www.cppblog.com/mirguest/archive/2012/02/12/165386.html)
比如：输入客户端显示：
QtGui.QInputDialog 提供了一个简单方便的对话框，用于获取用户输入的一个值。输入值可以是字符串，数字，或者一个列表中的一项。
```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
ZetCode PyQt4 tutorial
In this example, we receive data from
a QtGui.QInputDialog dialog.
author: Jan Bodnar
website: zetcode.com
last edited: October 2011
"""
import sys
from PyQt4 import QtGui
class Example(QtGui.QWidget):
    def __init__(self):
        super(Example, self).__init__()
        self.initUI()
    def initUI(self):
        self.btn = QtGui.QPushButton('Dialog', self)
        self.btn.move(20, 20)
        self.btn.clicked.connect(self.showDialog)
        self.le = QtGui.QLineEdit(self)
        self.le.move(130, 22)
        self.setGeometry(300, 300, 290, 150)
        self.setWindowTitle('Input dialog')
        self.show()
    def showDialog(self):
        text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog',
            'Enter your name:')
        if ok:
            self.le.setText(str(text))
def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
if __name__ == '__main__':
    main()
```
这个例子中用到了一个按钮和一个行编辑组件。按钮会显示一个输入对话框用于得到文本。而输入的文本将在行编辑组件中显示。
text,ok=QtGui.QInputDialog.getText(self,'Input Dialog','Enter your name:')
这一行显示了输入对话框。第一个字符串是对话框的标题，第二个字符串则是对话框中的消息。对话框将返回输入的文本和一个布尔值。如果点击了 ok 按钮，则布尔值为 true ，否则为 false 。
ifok:self.le.setText(str(text))
从对话框中接收到的文本就被设置到行编辑组件中。
3. 注意事项：由Pyuic转化的python代码和直接由pyqt写的代码有多不同，如1中所示
## 4.QtGui.QFileDialog
QtGui.QFileDialog 是允许用户选择文件或目录的对话框。文件可以用于打开或保存。
```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
ZetCode PyQt4 tutorial
In this example, we select a file with a
QtGui.QFileDialog and display its contents
in a QtGui.QTextEdit.
author: Jan Bodnar
website: zetcode.com
last edited: October 2011
"""
import sys
from PyQt4 import QtGui
class Example(QtGui.QMainWindow):
    def __init__(self):
        super(Example, self).__init__()
        self.initUI()
    def initUI(self):
        self.textEdit = QtGui.QTextEdit()
        self.setCentralWidget(self.textEdit)
        self.statusBar()
        openFile = QtGui.QAction(QtGui.QIcon('open.png'), 'Open', self)
        openFile.setShortcut('Ctrl+O')
        openFile.setStatusTip('Open new File')
        openFile.triggered.connect(self.showDialog)
        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(openFile)
        self.setGeometry(300, 300, 350, 300)
        self.setWindowTitle('File dialog')
        self.show()
    def showDialog(self):
        fname = QtGui.QFileDialog.getOpenFileName(self, 'Open file',
                '/home')
        f = open(fname, 'r')
        with f:
            data = f.read()
            self.textEdit.setText(data)
def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
if __name__ == '__main__':
    main()
```
这个例子中有菜单栏，文本编辑区以及状态栏。菜单中的选项显示 QtGui.QFileDialog 用于选择文件。而文件的内容则载入到文本编辑区。
classExample(QtGui.QMainWindow):def__init__(self):super(Example,self).__init__()
这个例子是基于 QtGui.QMainWindow 组件，因为我们要在中心设置文本编辑区。
fname=QtGui.QFileDialog.getOpenFileName(self,'Open file','/home')
我们弹出 QtGui.QFileDialog 。在getOpenFileName() 方法中第一个字符串是标题。第二个则是指定对话框的工作目录。默认情况下，文件过滤为所有文件（*
 ）。
```python
f = open(fname, 'r')
with f:
    data = f.read()
    self.textEdit.setText(data)
```
选择的文件将被读取，并且其文件内容设置到文本编辑区。
5.一个完整的文本编辑器
参考链接：[http://blog.csdn.net/kilvdn/article/details/4077183](http://blog.csdn.net/kilvdn/article/details/4077183)
