# 使用XlsxWriter创建Excel文件并编辑 - 小灰笔记 - CSDN博客





2017年05月21日 17:13:55[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3102








      之前操作Excel使用过其他的方式，针对Excel的写入来说，使用过xlwt模块，也直接使用过win32com接口。如果说哪个最能够发挥Excel的威力，那么肯定是win32com接口实现的方式，然而这种方式需要的是Office软件支持。至于xlwt，我觉得我们平时用到的功能基本上都有了，通常这个基本就能够满足我个人的要求。

      今天使用了一下XlsxWriter模块，暂时没有找到什么比较特殊的功能。当然，我自己也仅仅是照搬了一个demo程序而已。先展示一下程序代码：

#!/usr/bin/python

# -*- coding: cp936 -*-



import xlsxwriter



workbook = xlsxwriter.Workbook('demo.xlsx')

worksheet = workbook.add_worksheet('demo')



worksheet.set_column('A:A',20)

bold = workbook.add_format({'bold':True})



worksheet.write('A1','Hello')

worksheet.write('A2','WOrld',bold)

worksheet.write('B2',u'你好！',bold)



worksheet.write(2,0,32)

worksheet.write(3,0,35.5)

worksheet.write(4,0,'=SUM(A3:A4)')



workbook.close()

      程序运行后，生成了文件名为demo.xlsx的文件，打开后的内容如下：

![](https://img-blog.csdn.net/20170521171310494?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      通过编辑操作，创建了指定名称的Sheet，同时写入了相关的信息。还调用了一个求和的方法计算了两个单元格的数值之和。

      相比之前使用的xlwt模块，XlsxWriter模块要大得多。从庞大的体积大致能够猜到这个模块的功能应该是要更强大一些。从文件的格式支持上就可以看出一点端倪，这个模块支持xlwt并不支持的xlsx类型格式的文件。至于其它的功能，后续真可以了解下然后再做一次更为细致的学习。



