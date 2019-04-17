# wxPython：一套Python语言的GUI工具箱 - zhusongziye的博客 - CSDN博客





2018年10月20日 15:46:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：94








![](https://img-blog.csdn.net/20181020154248182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**简单介绍**

**wxPython**是Python语言中一套优秀的GUI图形库，可以很方便地创建完整、功能健全的GUI图形用户界面。

如Wxpython官网说的那样，它产出的应用程序可以在windows、mac os以及linux系统上不做或做很少修改的情况下完美运行。

当然，wxpython是对像我这样的小白非常友好的，它具有相对完备的文档支持，当遇到什么棘手的问题时，翻阅文档也不失为一种很好的解决办法。

**"Hello world！"**

![](https://img-blog.csdn.net/20181020154318601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



要创建你的第一个wxpython应用程序，其实很简单。

首先，导入wxpython图形库import wx。

然后，创建一个类class App(wx.App)，它是该应用程序的“领袖”，负责统筹各种组件、框架等等。该类中负责初始化的方法是OnInit，在该方法下，创建窗体Frame（基于wx.Frame，最简单的只需输入它的上级None以及窗体标题title即可），然后将它展示出来frame.Show()。

最后，创建该类的实例app = App()，然后，开始它的主循环app.MainLoop()。

```
import wx

class App(wx.App):
    def OnInit(self):
        frame = wx.Frame(None, title="Hello wxpython's world!")  # Create a frame.
        frame.Show()  # Show it.
        return True


if __name__ == '__main__':
    app = App()  # create an application object.
    app.MainLoop()  # Then start the event loop.
```



**基本组件**

![](https://img-blog.csdn.net/201810201543441?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



面板(Panel)

所有组件都要放在面板上。(每个控件都有一个参数parent，就是给它留的。当然它自己的parent是wx.Frame实例)

```
wx.Panel(parent, id=ID_ANY, pos=DefaultPosition, 
    size=DefaultSize, style=TAB_TRAVERSAL, name=PanelNameStr)
```





静态文本(StaticText)

```
wx.StaticText(parent, id=ID_ANY,label="", pos=DefaultPosition, 
    size=DefaultSize, style=0, name=StaticTextNameStr)
```



文本输入框(TextCtrl)

```
wx.TextCtrl(parent, id=ID_ANY, value="", pos=DefaultPosition,size=DefaultSize,
    style=0, validator=DefaultValidator, name=TextCtrlNameStr)
```





按钮(Button)

基础按钮控件。

```
wx.Button(parent, id=ID_ANY, label="", pos=DefaultPosition, size=DefaultSize,
    style=0, validator=DefaultValidator, name=ButtonNameStr)
```





列表框(ListCtrl)

展示完整的表格文本。

```
wx.ListCtrl(parent, id=ID_ANY, pos=DefaultPosition, size=DefaultSize, style=LC_ICON,
    validator=DefaultValidator, name=ListCtrlNameStr)
```





下拉选择框(Choice)

创建选项，用于选择其中之一。

```
wx.Choice(parent, id=ID_ANY, pos=DefaultPosition, size=DefaultSize, choices=[ ], 
    style=0, validator=DefaultValidator, name=ChoiceNameStr)
```







字体控制(Font)

创建字体，为其它控件的文字服务。

```
wx.Font(pointSize, family, style, weight, underline=False , 
    faceName="", encoding=FONTENCODING_DEFAULT)
```





容器(Boxsizer)

一个为其他组件合理分配空间以使界面看起来更整洁的容器组件。
`wx.BoxSizer(orient=HORIZONTAL)`
常用方法：boxSizer.Add()、

                   panel.SetSizerAndFit(boxSizer)





图标(Icon)

给应用程序设定一个图标Logo。
`wx.Icon(name, type=BITMAP_TYPE_ANY, desiredWidth=-1, desiredHeight=-1)`




上述所述组件的简单代码实现



```
import wx

class Frame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, title='wxpython基本组件介绍')
        self.InitUI()

    def InitUI(self):
        #面板
        panel = wx.Panel(self)

        #文本
        staticText = wx.StaticText(panel, label='静态文本控件')
        dynamicText = wx.TextCtrl(panel, value='动态文本输入框')
        #按钮
        button = wx.Button(panel, label='单击按钮')
        #列表
        listText = wx.ListCtrl(panel); listText.InsertItem(0, '列表控件')
        #下拉列表选择
        choiceList = wx.Choice(panel, choices=['下拉列表选择控件', '选项2', '选项3']); choiceList.SetSelection(0)
        #图标
        icon = wx.Icon('icon_demo.ico'); self.SetIcon(icon)
        #字体改变
        font = wx.Font(20, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False, 'SimHei')
        demo = wx.StaticText(panel, label='字体变为黑体'); demo.SetFont(font)
        #容器
        boxsizer = wx.BoxSizer(wx.VERTICAL)
        boxsizer.Add(staticText, wx.ALL); boxsizer.Add(dynamicText, wx.ALL); boxsizer.Add(button, wx.ALL); 
        boxsizer.Add(listText, wx.ALL); boxsizer.Add(choiceList, wx.ALL); boxsizer.Add(demo, wx.ALL)
        panel.SetSizerAndFit(boxsizer)


class App(wx.App):
    def OnInit(self):
        frame = Frame()
        frame.Show()
        return True


if __name__ == '__main__':
    app = App()
    app.MainLoop()
```





**一个实用的窗体跳转模式**

![](https://img-blog.csdn.net/2018102015443133?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这种模式会经常用到，譬如确认窗口、警告、温馨提示等等，用它就对了。

它包含简单的三个文件：Main.py、GUImanager.py、Frame.py。

Main.py负责重要的App类的创建以及应用程序的主循环。

```
import wx
from guimanager import Manager

class MainApp(wx.App):

    def OnInit(self):
        self.guimanager = Manager(self.UpdateUI)
        self.frame = self.guimanager.GetFrame(number=1)
        self.frame.Show()
        return True

    def UpdateUI(self,number):
        self.frame = self.guimanager.GetFrame(number)
        self.frame.Show(True)

def main():
    app = MainApp()
    app.MainLoop()

if __name__ == '__main__':
    main()
```



GUImanager.py负责两个（多者不限）窗体之间切换。

```
import wx

import frame

class Manager():

    def __init__(self,UpdateUI):
        self.UpdateUI = UpdateUI
        self.frameDict = {} # 用来装载已经创建的Frame对象

    def GetFrame(self,number):
        frame = self.frameDict.get(number)

        if frame:
            pass
        else:
            frame = self.CreateFrame(number)
            self.frameDict[number] = frame

        return frame

    def CreateFrame(self,number):
        if number == 1:
            return frame.My_First_Frame(id=number, UpdateUI=self.UpdateUI)
        elif number == 2:
            return frame.My_Second_Frame(id=number, UpdateUI=self.UpdateUI)
```



![](https://img-blog.csdn.net/2018102015445517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Frame.py 顾名思义，就是窗体框架在这个文件中。

```
import wx

class My_First_Frame(wx.Frame):

    def __init__(
            self, parent=None, id=-1, title='窗体一', pos=wx.DefaultPosition,size=wx.DefaultSize, 
            style=wx.DEFAULT_FRAME_STYLE^(wx.MAXIMIZE_BOX|wx.MINIMIZE_BOX|wx.RESIZE_BORDER), UpdateUI=None
    ):
        wx.Frame.__init__(self, parent, id,title=title, size=size, pos=pos, style=style)
        self.UpdateUI = UpdateUI
        self.InitUI() # 绘制UI界面

    def InitUI(self):
        panel = wx.Panel(self) 
        button = wx.Button(panel, -1, '打开到窗体二')
        self.Bind(wx.EVT_BUTTON, self.OnClick, button)

    def OnClick(self, event):
        self.UpdateUI(2)


class My_Second_Frame(wx.Frame):

    def __init__(
        self, parent=None, id=0, title='窗体二', pos=wx.DefaultPosition,size=wx.DefaultSize, 
        style=wx.DEFAULT_FRAME_STYLE^(wx.MAXIMIZE_BOX|wx.MINIMIZE_BOX|wx.RESIZE_BORDER), UpdateUI=None
    ):
        wx.Frame.__init__(self, parent, id,title=title, size=size, pos=pos, style=style)
        self.UpdateUI = UpdateUI
        self.InitUI() # 绘制UI界面

    def InitUI(self):
        panel = wx.Panel(self)
        button = wx.Button(panel, 0, '关闭')
        self.Bind(wx.EVT_BUTTON, self.OnClick, button)

    def OnClick(self,event):
        self.Close()
```





**小结**

![](https://img-blog.csdn.net/20181020154505367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最基本的wxPython程序开发其实包含五个步骤：

1.导入必须的wxPython包

2.子类化wxPython应用程序类

3. 定义一个应用程序的初始化方法（包含一个最基本的Frame）

4. 创建一个应用程序类的实例

5. 进入这个应用程序的主事件循环



wxPython对小白真的很友好、很友好（ps：我不是它的脑残粉），它的文档足以满足所有人对wxPython的需求，不妨试试看?

![](https://img-blog.csdn.net/2018102015455337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





