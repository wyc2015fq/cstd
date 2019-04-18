# python学习笔记(三十一) -- virtualenv、图形界面 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月10日 10:58:33[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：34
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**目录**

[virtualenv](#virtualenv)

[图形界面](#%E5%9B%BE%E5%BD%A2%E7%95%8C%E9%9D%A2)

# virtualenv

virtualenv与Anacodna 类似，用来管理环境

关于virtualenv的安装以及使用看下面这两个文章足以，分别有linux和windows的安装步骤

[https://www.jianshu.com/p/ab93973286cc](https://www.jianshu.com/p/ab93973286cc)

[http://www.tendcode.com/article/virtualenv-for-python/](http://www.tendcode.com/article/virtualenv-for-python/)

# 图形界面

如果想开发一个图形界面那必然涉及调用操作系统提供的图形界面接口。

Python支持多种图形界面的第三方库，包括：
- 
Tk

- 
wxWidgets

- 
Qt

- 
GTK

等等。

但是Python自带的库是支持Tk的Tkinter，使用Tkinter，无需安装任何包，就可以直接使用。本章简单介绍如何使用Tkinter进行GUI编程

**例一**：

```python
# encoding=utf-8
from tkinter import *

class Application(Frame):
    def __init__(self, master=None):
        # 创建一个窗口对象
        Frame.__init__(self, master)    # master代表主窗口标题
        self.pack()   # 调用pack函数，窗口才会显示
        self.createWidgets()

    def createWidgets(self):
        self.helloLabel = Label(self, text='Hello, world!')   # 在窗口中生成一行文字，内容为'Hello,world!'
        self.helloLabel.pack()   # 调用pack函数，将文字放入窗口中
        self.quitButton = Button(self, text='Quit', command=self.quit)   # 生成一个按钮，点击按钮触发quit函数，按钮显示的文本为Quit
        self.quitButton.pack()   # 调用pack函数，将按钮放入窗口中

# 创建窗口对象
app = Application()
# 设置窗口标题:
app.master.title('Hello World')
# 使窗口一直存在不会消失:
app.mainloop()
```

效果 

![](https://img-blog.csdn.net/2018091010554054?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

** 例二：**

```python
# encoding=utf-8
from tkinter import *
import tkinter.messagebox as messagebox

class Application(Frame):

    def __init__(self, master=None):
        # 创建一个窗口对象
        Frame.__init__(self, master)  # master代表主窗口标题
        self.pack()  # 调用pack函数，窗口才会显示
        self.createWidgets()

    # 生成窗口内部的内容
    def createWidgets(self):
        self.nameInput = Entry(self)   # 生成输入框
        self.nameInput.pack()   # 把输入框放入窗口中
        self.alertButton = Button(self, text='Hello', command=self.hello)   # 生成一个按钮，触发hello函数
        self.alertButton.pack()  # 把按钮放入窗口中

    def hello(self):
        name = self.nameInput.get() or 'world'   # 从输入框中获取文本内容，如果内容为None则 使用 world
        messagebox.showinfo('Message', 'Hello, %s' % name)    # 弹出对话框，显示拼接的字符串

app = Application()
# 设置窗口标题:
app.master.title('Hello World')
# 主消息循环:
app.mainloop()
```

效果

![](https://img-blog.csdn.net/2018091010571586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

