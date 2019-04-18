# PyQｔ小程序 - YZXnuaa的博客 - CSDN博客
2018年04月26日 19:15:44[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：162
```
通过下面的一个PyQt5简单例子，来简单了解一下关于如何创建PyQt5的。具体代码如下：
```
```python
#-*- coding:utf-8 -*-
'''
Frist PyQt5 program
'''
__author__ = 'Tony Zhu'
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QHBoxLayout, QPushButton, QLineEdit, QVBoxLayout,QMessageBox
import sys
class ShowWindow(QWidget):
    def __init__(self):
        super(ShowWindow,self).__init__()      
        self.initUI()
    def initUI(self):
        self.inputLabel = QLabel("Input your text")
        self.editLine = QLineEdit()
        self.printButton = QPushButton("Print")
        self.clearButton = QPushButton("Clear")
        self.printButton.clicked.connect(self.printText)
        self.clearButton.clicked.connect(self.clearText)
        inputLayout = QHBoxLayout()
        inputLayout.addWidget(self.inputLabel)
        inputLayout.addWidget(self.editLine)
        buttonLayout = QHBoxLayout()
        buttonLayout.addWidget(self.printButton)
        buttonLayout.addWidget(self.clearButton)
        mainLayout = QVBoxLayout()
        mainLayout.addLayout(inputLayout)
        mainLayout.addLayout(buttonLayout)
        self.setLayout(mainLayout)
        self.setWindowTitle('FristWindow') 
        self.show()
    def printText(self):
        text = self.editLine.text()
        if text == '':
            QMessageBox.information(self, "Empty Text",
                                    "Please enter the letter.")
        else :
            QMessageBox.information(self, "Print Success",
                                    "Text: %s" % text)
    def clearText(self):
        text = self.editLine.text()
        if text == '':
            return
        else :
            self.editLine.clear()
if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = ShowWindow()
    sys.exit(app.exec_())
```
***运行的结果：***
![程序运行后的结果](https://img-blog.csdn.net/20160831174243380)
***结合代码和运行的结果，分析代码：***
Line7~8：
```python
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QHBoxLayout, QPushButton, QLineEdit, QVBoxLayout,QMessageBox
import sys
```
导入了程序运行所必须的模块
Line10：
`class ShowWindow(QWidget):`
创建一个ShowWindow类继承QWidget，其中PyQt中非常重要的通用窗口类，是所有用户界面对象的基类，所有和用户界面相关的控件类都是继承自QWidger类。
Line12~14：
```python
def __init__(self):
        super(ShowWindow,self).__init__()      
        self.initUI()
```
定义了ShowWindow类的构造函数 **init**，由于ShowWindow继承自QWidgert类，因此在构造函数中调用父类QWidget的构造函数super.**init**()。 
同时在构造函数中调用自定义的初始化函数initUI()，在该函数中初始化GUI中所需各个控件。
Line17~20：
```
self.inputLabel = QLabel("Input your text")
self.editLine = QLineEdit()
self.printButton = QPushButton("Print")
self.clearButton = QPushButton("Clear")
```
创建成员：一个标签（inputLabel ），输入框（editLine ），两个按钮（printButton ，clearButton ）
Line22~23：
`self.printButton.clicked.connect(self.printText)        self.clearButton.clicked.connect(self.clearText)`
printButton和clearButton点击事件处理的逻辑：点击printButton之后会调用自定义函数printText()中的处理逻辑；点击clearButton之后调用自定义函数clearText()中的处理逻辑。通过connect()方法将点击事件和处理逻辑关联起来 
    这个涉及到PyQt信号与槽的概念，信号和槽用于对象之间的通信。当指定事件发生，一个事件信号会被发射。槽可以被任何Python脚本调用。当和槽连接的信号被发射时，槽会被调用。有关信号与槽的概念，在后续的文章中会专门讲解。
Line25~26:
```
inputLayout = QHBoxLayout()
inputLayout.addWidget(self.inputLabel)
inputLayout.addWidget(self.editLine)
```
创建一个inputLayout的水平布局(QHBoxLayout)，在inputLayout中添加已定义的控件inputLabel和editLine。QHBoxLayout让添加的控件水平排布
Line29~31:
```
buttonLayout = QHBoxLayout()
buttonLayout.addWidget(self.printButton)
buttonLayout.addWidget(self.clearButton)
```
同上创建一个buttonLayout 的水平布局(QHBoxLayout)，在buttonLayout 中添加printButton和clearButton
Line33~35：
```
mainLayout = QVBoxLayout()
mainLayout.addLayout(inputLayout)
mainLayout.addLayout(buttonLayout)
```
创建一个mainLayout 的垂直布局(QVBoxLayout)，在mainLayout 中嵌套子布局inputLayout和buttonLayout。
通过如下的布局图，进一步了解一下该程序中layout是如何布局的。 
![layout](https://img-blog.csdn.net/20160831185816991)
有layout的概念，在后续的文章中会专门讲解。
Line37：
`self.setLayout(mainLayout)`
通过将setLayout()方法，将mainLayout设置为窗口layout。
Line38：
`self.setWindowTitle('FristWindow') `
通过setWindowTitle()方法设置窗口的标题
Line39：
`self.show()`
通过show()方法将窗口显示到屏幕上
Line40~48：
```python
def printText(self):
        text = self.editLine.text()
        if text == '':
            QMessageBox.information(self, "Empty Text",
                                    "Please enter the letter.")
        else :
            QMessageBox.information(self, "Print Success",
                                    "Text: %s" % text)
```
定义了处理printButton点击信号的槽函数，当editLine输入框中的文本为空的时候弹出消息框(QMessageBox)，提示“Please enter the letter.”；当editLine输入框中的文本不为空时候弹出消息框，显示editLine中的文本。
触发槽函数printText()之后，当editLine输入框内容为空时界面显示如下： 
![MessageBox](https://img-blog.csdn.net/20160831191126380)
Line49~55:
```python
def clearText(self):
        text = self.editLine.text()
        if text == '':
            return
        else :
            self.editLine.clear()
```
定义了处理clearButton点击信号的槽函数,清空editLine输入框中的文本信息。
Line 57~60：
```
if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = ShowWindow()
    sys.exit(app.exec_())
```
程序运行的入口函数类似C语言中的main()方法。 
app = QApplication(sys.argv)，每一个pyqt程序必须创建一个application对象，sys.argv是命令行参数，可以通过命令启动的时候传递参数。 
ex = ShowWindow()，创建ShowWindow对象。 
sys.exit(app.exec_())，app.exec_() 事件处理开始，进入程序主循环。主循环等待时间，并把事件发送给指定处理的任务中。当调用app.exit()或者程序因为各种原因被破坏后，使用sys.exit()关闭程序，并释放内存资源。
到此为止，第一个PyQt程序分析结束，可能会存在输入框，布局，等各类控件如何使用，如何确定对应的控件在那个模块中？在后续的文章中会详细进行介绍。
