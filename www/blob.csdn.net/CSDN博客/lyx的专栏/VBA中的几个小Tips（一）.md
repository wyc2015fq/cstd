# VBA中的几个小Tips（一） - lyx的专栏 - CSDN博客





2016年05月13日 16:13:07[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：371








1.如何获的表格的行数

当然这里的行数是指有效使用的行数。

i = Sheets(2).[A65535].End(xlUp).Row

这里i就是你能够获得的第二张sheet中的所有有效行的个数。Sheets（）括号里面的数字就是指第几个sheet。

2.在VBA里面使用Excel中的函数。

在函数前面加上Application.WorksheetFunction就可以了。比如使用vlookup函数，有


Cash = Application.WorksheetFunction.VLookup(Name, Worksheets("name").Range("B:F"), 2, 0)




