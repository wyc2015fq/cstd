# 使用Python通过win32 COM实现Word文档的写入与保存 - 小灰笔记 - CSDN博客





2017年05月10日 19:34:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：5600








      通过win32 COM接口实现软件的操作本质上来看跟直接操作软件一致，这跟我之前经常用的通过各种扩展的组件或者库实现各种文件的处理有较大的差异。如果有过Windows下使用Word的经历，那么使用win32 COM应该说是更为便捷的一种方式。

      先前通过拼凑网络上的代码实现过Word文档的处理，今天通过读文档从头开始做一次新的尝试。简单实现一个Word文件的创建、写入与存储。

      实现的代码如下：

#!/usr/bin/python



import os

from win32com.client import Dispatch



pwd = os.getcwd()



wordApp = Dispatch('word.Application')

wordApp.Visible = True

myDoc = wordApp.Documents.Add()

myRange = myDoc.Range(0,0)

myRange.InsertBefore('hello python word doc!')

myDoc.SaveAs(pwd + '\\python_word_demo.docx')

myDoc.Close()

wordApp.Quit()

      程序运行没有错误报出，运行的过程中会看到Word软件的打开以及操作最后关闭的过程。执行结束后，当前文件夹中多了一个名为python_word_demo.docx的文件。打开文件可以看到里面写入的内容如下：

![](https://img-blog.csdn.net/20170510193310010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      这样，先前想演示的功能便已经实现，实现的方法比我想象中的简单不少。如果有文件的批量处理需求，这无疑又是一个可以考虑的好方法。



