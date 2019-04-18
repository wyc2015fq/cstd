# ［Ｐｙｔｈｏｎ］安装pyqt - YZXnuaa的博客 - CSDN博客
2018年04月26日 19:32:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：152
众所周知anaconda安装任何python插件非常轻而易举的事情。今天我们安装Python的图形界面插件PyQt，这个插件支持py2.x，py3.x。可以快速的开发图形界面。在anaconda里面py3.x的安装都是pyqt5. 
1、首先得有anaconda
2、点击打开进入anaconda navigator，在root环境下或者自己新创建一个环境。 
输入搜索字符pyqt结果如下，我这里是已经安装了 
![这里写图片描述](https://img-blog.csdn.net/20170708151202286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVG91Y2hfRHJlYW0=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、下面我们通过在这样环境下安装的Spyder来测试。
```python
# -*- coding: utf-8 -*-
"""
Spyder Editor
This is a temporary script file.
"""
import sys
from PyQt5 import QtWidgets, QtCore
app = QtWidgets.QApplication(sys.argv)#定义一个窗口程序
widget = QtWidgets.QWidget()#定义一个窗口对象，用户可以通过接口来自定义
widget.resize(360, 360)
widget.setWindowTitle("Hello, PyQt5!")
widget.show()
sys.exit(app.exec_())
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
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
结果显示一个窗口： 
![这里写图片描述](https://img-blog.csdn.net/20170708151427371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVG91Y2hfRHJlYW0=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
至此就可以好好愉快的玩耍PyQt图形界面
