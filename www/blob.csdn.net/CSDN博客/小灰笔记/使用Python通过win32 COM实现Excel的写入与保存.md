# 使用Python通过win32 COM实现Excel的写入与保存 - 小灰笔记 - CSDN博客





2017年05月08日 22:03:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：5343








      很久之前通过东拼西凑实现过使用Python通过win32 COM实现word文档的批量处理，后来那段程序功能在我的工作以及生活中被我使用了好多次。在批量处理的时候，不过是一个读写的过程，而读写过后还需保存。类比到Excel的操作中，自然也需要一个保存退出的动作。

    我搜索了自己手头获得的只有几页的文档，没有相应的功能介绍，我也找不到程序的操作方式。只好借着自己的兴趣猜测尝试一下，仿照的自然是word文档的处理。还真让我猜出来了，具体的示例代码如下：

#!/usr/bin/python



from win32com.client import Dispatch

import os



pwd = os.getcwd()



xlApp = Dispatch('Excel.Application')

xlApp.Visible = True

xlBook = xlApp.Workbooks.Add()

xlApp.Worksheets.Add().Name = 'test'

xlSheet = xlApp.Worksheets('test')

xlSheet.Cells(1,1).Value = 'title'

xlSheet.Cells(2,1).Value = 123

xlBook.SaveAs(pwd + '\\demo.xlsx')

xlApp.Quit() # exit app

      运行程序，Excel的界面会一闪而过。之后在当前文件夹中会出现一个名为demo.xlsx的文件。文件打开后的内容如下：

![](https://img-blog.csdn.net/20170508220234179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








