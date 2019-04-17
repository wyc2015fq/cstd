# python xlrd读取excel常用方法 - 别说话写代码的博客 - CSDN博客





2018年10月17日 13:42:04[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：98








转自：[https://www.cnblogs.com/feiyueNotes/p/7786579.html](https://www.cnblogs.com/feiyueNotes/p/7786579.html)

最近学习了python操作excel，记录下常用方法.

需要安装xlrd模块， 打开cmd，输入命令：pip install xlrd  进行安装，若已安装显示如下：

![](https://images2017.cnblogs.com/blog/1236609/201711/1236609-20171105095454076-1606477692.png)



 xlrd读取excel常用方法如下：

```python
'''一、打开文件'''
xl = xlrd.open_workbook(file)

'''二、获取sheet'''
print (xl.sheet_names())#获取sheet名称
print (xl.sheets())#获取sheet对象
print(xl.nsheets) #获取sheet总数
print(xl.sheet_by_name(u"目录"))
print (xl.sheet_by_index(1))


'''三、获取sheet内的汇总数据'''
table1 = xl.sheet_by_name(u"目录")
print(table1.name)
print (table1.ncols)
print(table1.nrows)

'''四、单元格批量读取'''
print(table1.row_values(0))#获取第n行的值 若是合并单元格 首行显示值 其它为空
print(table1.row(0))#获取值及类型
print (table1.row_types(0))
print(table1.col_values(0,1,4))#获取列，切片
print(table1.row_slice(1,0,2))

'''五、特定单元格读取'''
#取值
print(table1.cell(1,2).value)
print(table1.cell_value(1,2))
print(table1.row(1)[2]).value
print(table1.col(2)[1]).value
#取类型
print(table1.cell(1,2).ctype)
print(table1.cell_type(1,2))
print(table1.row(1)[2].ctype)

'''六、常用技巧（0,0）转换成A1'''
print(xlrd.cellname(0,0))
print(xlrd.cellnameabs(0,0))
print(xlrd.colname(0))
```





