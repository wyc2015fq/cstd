
# Python 学习入门（34）—— PyDev 调试 - 阳光岛主 - CSDN博客

2013年12月22日 00:12:37[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：17159所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**运行程序**
运行 Python 源程序，有两种方法，以 example.py 为例：
example.py代码：

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-
# ithomer.net
a = 1
def display():
    print("hello ithomer")
    
    global a
    print("a = %d" % a)
    a = 2
    print("a = %d" % a)
if __name__ == '__main__':
    display()
```
运行结果：
hello ithomer
a = 1
a = 2
方法1：在 Pydev Package Explorer 中，双击 example.py，选择 菜单栏->Run -> Run As -> Python Run
方法2：在 Pydev Package Explorer 中，右键 example.py，在弹出的菜单栏中选择 Run As -> Python Run
以上两种方式是运行源程序的基本方法。

**Code Coverage**
Pydev 还提供一种特有的源程序运行功能 Run As Python Coverage，该功能不仅能显示出程序的运行结果，而且能将程序运行过程中代码的覆盖率显示出来。要查看代码的覆盖率，首先需要打开 Code Coverage，在 Pydev 透视图中，选择 Windows -> Show View -> Code Coverage
![](https://img-blog.csdn.net/20131221225841062)

在弹出视图的左栏中，可以看到三个按钮，”Choose folder to analyze”, “Clear” 和 ”Refresh”
用鼠标左键单击 ”Choose folder to analyze”，在弹出的 Folder Selection 窗口中选择需要运行的程序所在的包，单击 Ok。这样，这个包中所有的源程序便显示在左栏中。
接下来，仍以 example.py 为例，看看 Run As Python Coverage 功能展现出的结果。选择Run As -> Python Coverage，控制台 Console 中显示出了程序的运行结果。切换到刚才打开的 Code Coverage Results View 视图，单击左栏中的 example.py。
代码运行过程中的覆盖情况很清楚地显示在右栏中。
双击左栏中的 example.py，没有覆盖到的代码便在编辑器中以醒目的错误标志被标注出来。
如果关闭 Code Coverage Results View 视图，代码的覆盖信息并没有丢失，重新打开该视图同样可以显示出这些信息。只有通过单击左栏的 “Clear” 按钮，才可以清除程序运行后得到的这些覆盖信息。

**调试程序**
调试是程序开发过程中必不可少的，下面仍以 example.py 为例，介绍如何使用 Pydev 的调试功能。
调试需从添加断点开始，设置断点有三种方式：
1）双击编辑器中标尺栏左边灰白的空白栏，在某行添加断点
2）鼠标右键单击标尺栏，在弹出的菜单栏中选择 ”Add Breakpoint” 添加断点
3）将鼠标移至需要添加断点的代码行，使用快捷键 Ctrl+F10，在弹出的菜单栏中选择 ”Add Breakpoint” 添加断点。添加好断点后，选择 Debug As -> Python Run 启动调试器，弹出一个对话框，询问是否切换到调试器透视图，单击 Yes，即显示调试模式，如下图：
![](https://img-blog.csdn.net/20131221232134968)
程序调试过程中，常用的几个快捷键如下：
单步跳入 Step Into:F5
单步跳过 Step Over:F6
单步返回 Step Return:F7
重新开始 Resume:F8
在控制台 Console 中，显示出断点之前代码的执行结果。如果要查看某个变量的值，以变量 a 为例，可以手动在控制台中键入一行代码 ”a”，再连续按两次 Enter 键，即显示出变量的值（如上图）
在调试模式下，要查看表达式的值，选中后单击鼠标右键，选择 Watch。弹出 Expression面板，显示出了相应的变量或表达式的值。
如果想要在满足一定条件下已经添加的断点才有效，可以设置断点的属性。在编辑器的标尺栏中单击鼠标右键，弹出的菜单栏中选择 Breakpoint Properties。
在显示的窗口中，选中复选框 ”Enable”，输入需要满足的条件，单击 Ok。
![](https://img-blog.csdn.net/20131221233217031)
这样，当重新执行程序调试的时候，只有满足条件的情况下，该断点才有效，如下图，直接跳到满足条件 "a == 2"处
![](https://img-blog.csdn.net/20131221232936500)

**总结**
Pydev ecplise 插件实现了功能强大且易用的 Python IDE，Pydev for Eclipse 的出现为 Python 开发人员实现高效的项目开发提供了很好的条件，该项目也在不断的发展之中，其功能将会越来越强大。


**参考推荐：**
[PyDev for Eclipse 运行和调试](http://hi.baidu.com/cherry_ting/item/23f1f1554a88399e09be1791)
[PyDev - Debugger](http://pydev.org/manual_adv_debugger.html)



