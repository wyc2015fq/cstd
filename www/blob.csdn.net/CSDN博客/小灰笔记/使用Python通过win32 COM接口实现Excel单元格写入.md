# 使用Python通过win32 COM接口实现Excel单元格写入 - 小灰笔记 - CSDN博客





2017年05月08日 21:31:33[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1747








      今天从电脑里找到一份不知道什么时候搜集的Python资料，主要介绍使用Win32 COM接口。简单看了几页发现这个还是很有用的，如果早点看了这个文档的话估计我的很多工作能够更早实现更高度的自动化。

    由浅入深做简单的学习性总结，先实现一个简单单元格内容写入。

      实现代码如下：

#!/usr/bin/python



from win32com.client import Dispatch



xlApp = Dispatch('Excel.Application')

xlApp.Visible = True

xlApp.Workbooks.Add()

xlSheet = xlApp.Sheets(1)

xlSheet.Cells(1,1).Value = 'title'

xlSheet.Cells(2,1).Value = 123

      程序执行后，Excel软件被打开同时对两个单元格进行了编辑，具体的编辑状态如下：

![](https://img-blog.csdn.net/20170508213036134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      从上面的结果可以看出，编辑单元格实现内容写入成功。程序中需要的技术方式就是使用了win32 COM，通过COM实现了对Excel程序的访问和操作。关于COM的知识，做了一点小结如下：

      1，可以使不同的语言对象之间进行通信；

      2，可以使不同的进程对象之间进行通信；

      3，可以使不同的机器对象之间进行通信；

      4，隐藏来自于程序员的细节信息；

      5，与DLLs相比没有性能上的损失。



