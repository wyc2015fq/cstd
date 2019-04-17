# 使用Python向Excel中插入图片 - 小灰笔记 - CSDN博客





2017年05月21日 17:30:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：10686








      使用Python向Excel文件中插入图片，这个功能之前学习xlwt的时候通过xlwt模块实现过。那时候是在公司做的尝试，印象中插入的图片最终缩小为了一个单元格，同时由于公司的加密系统如此实现了图片插入的Excel文件不能够再次打开。

      这次试用XlsxWriter这个模块，推测这个模块应该是具有比xlwt强大的多的功能。代码实现如下：

#!/usr/bin/python

# -*- codding: cp936 -*-



import xlsxwriter



book = xlsxwriter.Workbook('pict.xlsx')

sheet = book.add_worksheet('demo')

sheet.insert_image('D4','VCFY6863.jpg')

book.close()

      程序执行结束后，生成了名称为pict.xlsx的Excel文件。打开后可以看到图片插入成功，插入点是指定的单元格，而整个图片的左上角落于此指定单元格。大致的信息如下图：



![](https://img-blog.csdn.net/20170521173139164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      插入功能实现，同时也是一种比较理想的插入效果。



