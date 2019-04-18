# 使用QT创建PythonGUI程序 - wishchinYang的专栏 - CSDN博客
2014年02月06日 18:36:17[wishchin](https://me.csdn.net/wishchin)阅读数：4197
**1. **挑选 GUI设计程序： wxPython Vs. pyQt4   
参考链接：[http://www.douban.com/group/topic/14590751/](http://www.douban.com/group/topic/14590751/)
[](http://www.douban.com/group/topic/14590751/)
（1）：wxWidgets wxPython ：开发商业软件完全没有问题，使用LGPL协议；前提是你不能对这个库的本身有什么改动，如果有改动的话，你要开放你改动的那部分代码。
如果你只是使用这些库的话，没什么问题，不要对库本身的代码进行改动。
         Q: 最近看一个wxpython的项目代码，想找个文档，难死了，基本没有，函数用法基本没有，类的用法说明没有； A: 文档应该写在源代码里了吧？就像tornado，或者像emacs的插件一样。
         抛开wx自身不谈，wxpython移植的非常烂，文档一坨屎，就是robin自己没事维护一下，搞不好哪天就玩完。PyQt还算个产品的样子。买了本wxPython in Action ，刚看完。现在可以写一些小工具了。
         A：Like Python and wxWidgets,  wxPython is Open Source which means that it is free for anyone to use and  the source code is available for anyone to look at and modify.              
（2）：pyQt4：pyQt4运行速度更快，写同样的程序代码量应该比wxpython少的多，pyQt现在也有LGPL版了，诺基亚的pyside，就是不知道稳定性怎么样，打包比wxpython大一点，一个helloworld小窗口，wxpython用py2exe打包压缩后2M多一点，pyQt打包压缩后8M左右。
           OxFAN 是可以链接，但只能动态链接，而且发行时不能绑定LGPL软件的二进制文件，比如安装包中不能含有QT的二进制文件（如果使用LGPL协议的话）；
           推荐使用NOKIA QT 的官方 LGPL 协议版本——PySide；
**2.** PyQt4：[Qt库的Python绑定PyQt](http://www.oschina.net/p/pyqt)
 Q : 为什么使用PyQt4： A:  权衡
PyQt向Python程序员提供了使用完整Qt应用程序接口的方法。您几乎可以用Python做任何C++能做的事。
既然是应用程序接口，用C++或是Python都是一样的。于是“为什么我得使用它”这个问题就可以归结为编程语言的选择上，而不一定要用PyQt。对我来 说作为一种编程语言，python相对于c++的优点是在编程效率上。您可以看到标准的Qt例子移植到PyQt后的代码。它们具有相同的功能，使用相同的 应用程序接口，但Python版的代码只有原来的50-60%，而且它们更容易阅读。
与编程效率相关的是Python更容易学习，而且它足以满足富有经验的程序员。奇趣公司已经发现PyQt可以使他们向应用高端技术的团体出售Qt，这些团体的成员是工业专家（化学家，航空工程师等）而不是精深的C++程序员。
当然PyQt既成熟又稳定，并且有庞大的用户群体。我从用户那儿得到的两个最普通的反馈是“它恰到好处"和“它很不错。”
**3.**使用QT设计UI：
用qt设计师设计ui文件：![](https://img-blog.csdn.net/20140206171915796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**4. **使用QT设计UI，编译生成Python工程：
参考链接：[http://www.cnblogs.com/fickleness/p/3145967.html](http://www.cnblogs.com/fickleness/p/3145967.html)
编译已经生成的用qt设计师保存的UI文件，使用命令pyuic4
```
```python
C:\Users\Administrator>cd C:\Python27\Lib\site-packages\PyQt4
C:\Python27\Lib\site-packages\PyQt4>pyuic4 C:\Python27\Lib\site-packages\PyQt4\d
emo.ui -o C:\Python27\Lib\site-packages\PyQt4\demo.py
```
```
 （在DOS环境下编译 ： ( 小插曲，进dos找不到  python命令，  想起来上次配置环境变量是   保存在administrater的变量中了，只能在本次生效，要想一直生效就得保存路径名
） 编译生成demo.ui---------------demo.py文件
在工程中import  filesystem，要不程序怎么知道要使用demo.py！![](https://img-blog.csdn.net/20140206172415640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
导入成功。
使用模块代码显示：
```python
import sys
from PyQt4 import QtGui,QtCore
from demo import *
if __name__ == "__main__":
    app =QtGui.QApplication(sys.argv)
    mywidget = Ui_MainWindow()    #you need use the class name in PyQtSampleinstead
    form = QtGui.QMainWindow()
    mywidget.setupUi(form)
    form.show()
app.exec_()
```
以测试方式运行：参考链接：[http://blog.csdn.net/lainegates/article/details/8774410](http://blog.csdn.net/lainegates/article/details/8774410)
```python
class UItest(QtGui.QMainWindow):  
        def __init__(self,parent=None):  
            QtGui.QWidget.__init__(self,parent)  
            self.loginGui()  
        def loginGui(self):  
            self.ui = Ui_Dialog()  
            self.ui.setupUi(self)  
            self.show()  
    app = QtGui.QApplication(sys.argv)  
    myqq = UItest()  
    sys.exit(app.exec_())
```
(注：pyuic参数说明：
```python
NAME  
           pyuic4 - compile Qt4 user interfaces to Python code  
    SYNOPSIS  
           pyuic4 [OPTION]... FILE  
    DESCRIPTION  
           pyuic4  takes  a Qt4 user interface description file and compiles it to  
           Python code. It can also show a preview of the user interface.  
    OPTIONS  
           -h, --help  
                  Show a summary of the options.  
           --version  
                  Display the version number of pyuic4 of the version of Qt  which  
                  PyQt4 was generated for.  
           -p, --preview  
                  Show a preview of the UI instead of generating Python code.  
           -o, --output=FILE  
                  Write the generated Python code to FILE instead of stdout.  
           -d, --debug  
                  Show  detailed  debugging  information  about  the UI generation  
                  process.  
           -x, --execute  
                  Generate extra code to test and display the class when  executed  
                  as a script.  
           -i, --indent=NUM  
                  Set the indentation width to NUM spaces. A TAB character will be  
                  used if NUM is 0 (default: 4).  
)
```
run 工程即可实现，不过逻辑设计还没有完成，期待原作者继续！
**4.  **使用py2exe生成win软件：
参考文章：[http://www.cnblogs.com/jans2002/archive/2006/09/30/519393.html](http://www.cnblogs.com/jans2002/archive/2006/09/30/519393.html)
py2exe是一种python发布工具，可以把python脚本转换成windows下的可执行程序，不需要安装python便可运行。
py2exe现在可以用来创建使用了wxPython, Tkinter, Pmw, PyGTK, pygame, win32com client and server 等模块的程序。
详细介绍可以看它的官方网站 http://starship.python.net/crew/theller/py2exe/
**一、简介**
py2exe是一个将python脚本转换成windows上的可独立执行的可执行程序(*.exe)的工具，这样，你就可以不用装python而在windows系统上运行这个可执行程序。
py2exe已经被用于创建wxPython,Tkinter,Pmw,PyGTK,pygame,win32com client和server,和其它的独立程序。py2exe是发布在开源许可证下的。
**二：我的过程：**
```python
set PATH=%PATH%;C:\Python27  //set path!
cd d/ D:\
cd D:\Develope\EclipseWorks\PythonGUI
```
在CMD栏里，运行命令 python  setup.py py2exe  
可在目录下生成 dist/xxx.exe 可以运行
注：对于使用pyQT4 产生：
```python
Traceback (most recent call last):
  File "_init_.py", line 2, in <module>
  File "PyQt4\QtGui.pyc", line 12, in <module>
  File "PyQt4\QtGui.pyc", line 10, in __load
ImportError: No module named sip
```
解决方案参考：http://www.py2exe.org/index.cgi/Py2exeAndPyQt
setup.py里面添加：
```python
windows = [{"script":"_init_.py"} ],
options={"py2exe":{"includes":["sip"]}} 即可
```
**三、py2exe的用法**
如果你有一个名为helloworld.py的python脚本，你想把它转换为运行在windows上的可执行程序，并运行在没有安装python的windows系统上，那么首先你应写一个用于发布程序的设置脚本例如mysetup.py，在其中的setup函数前插入语句import py2exe 。
mysetup.py示例如下:
```python
# mysetup.py
from distutils.core import setup
import py2exe
```
setup(console=["helloworld.py"])
然后按下面的方法运行mysetup.py:  python mysetup.py py2exe（运行时传入参数）
上面的命令执行后将产生一个名为dist的子目录，其中包含了helloworld.exe,python24.dll,library.zip这些文件。如果你的helloworld.py脚本中用了已编译的C扩展模块，那么这些模块也会被拷贝在个子目录中，同样，所有的dll文件在运行时都是需要的，除了系统的dll文件。
dist子目录中的文件包含了你的程序所必须的东西，你应将这个子目录中的所有内容一起发布。
默认情况下，py2exe在目录dist下创建以下这些必须的文件：
1、一个或多个exe文件。
2、python##.dll。 
3、几个.pyd文件，它们是已编译的扩展名，它们是exe文件所需要的；加上其它的.dll文件，这些.dll是.pyd所需要的。
4、一个library.zip文件，它包含了已编译的纯的python模块如.pyc或.pyo
上面的mysetup.py创建了一个控制台的helloword.exe程序，如果你要创建一个图形用户界的程序，那么你只需要将mysetup.py中的console=["helloworld.py"]替换为windows=["myscript.py"]既可。
py2exe一次能够创建多个exe文件，你需要将这些脚本文件的列表传递给console或windows的关键字参数。如果你有几个相关联的脚本，那么这是很有用的。
运行下面个命令，将显示py2exe命令的所有命令行标记。
python mysetup.py py2exe --help
**四、指定额外的文件**
一些应用程序在运行时需要额外的文件，诸如配置文件、字体、位图。
如果在安装脚本中用data_files可选项指定了那些额外的文件，那么py2exe能将这些文件拷贝到dist子目录中。data_files应包含一个元组(target-dir, files)列表，其中的files是这些额外的文件的列表。
示例如下：
```python
# mysetup.py
from distutils.core import setup
import glob
import py2exe
setup(console=["helloworld.py"],
      data_files=[("bitmaps",
                   ["bm/large.gif", "bm/small.gif"]),
                  ("fonts",
                   glob.glob("fonts\\*.fnt"))],
)
```
说明：data_files选项将创建一个子目录dist\bitmaps，其中包含两个.gif文件；一个子目录dist\fonts，其中包含了所有的.fnt文件。
**五、Windows NT services**
你可以通过传递一个service关键字参数给setup函数来建造Windows NT services
,这个service参数的值必须是一个Python模块名(包含一service类)的列表。
示例如下：
```python
# mysetup.py
from distutils.core import setup
import py2exe
setup(service=["MyService"])
```
所建造的可执行的service是可以通过在其后跟一定的命令行参数标记来自行安装和卸载的。你可以通过在这个可执行的service(exe)后跟一-help参数来得到更多的帮助。
**六、COM servers**
你可以通过传递一个com_server 关键字参数给setup函数来建造Windows NT services
,这个service参数的值必须是一个Python模块名(包含一个或多个COM server 类)的列表。
示例如下：
```python
# mysetup.py
from distutils.core import setup
import py2exe
```
