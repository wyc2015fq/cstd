
# python使用xlrd与xlwt对excel的读写和格式设定 - jiahaowanhao的博客 - CSDN博客


2018年06月12日 18:32:29[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：111


[python使用xlrd与xlwt对excel的读写和格式设定](http://cda.pinggu.org/view/25802.html)
python操作excel主要用到xlrd和xlwt这两个库，即xlrd是读excel，xlwt是写excel的库。本文主要介绍了python使用xlrd与xlwt对excel的读写和格式设定，下面话不多说，来看看详细的实现过程。
脚本里先注明\# -*- coding:utf-8 -*-
1.  确认源excel存在并用xlrd读取第一个表单中每行的第一列的数值。
import xlrd, xlwt
import os
assert os.path.isfile('source_excel.xls'),"There is no timesheet exist. Exit..."
book = xlrd.open_workbook('source_excel.xls')
sheet=book.sheet_by_index(0)
for rows in range(sheet.nrows):
value = sheet.cell(rows,0).value
2. 用xlwt准备将从源表中读出的数据写入新表，并设定行宽和表格的格式。合并单元格2行8列后写入标题，并设定格式为之前定义的tittle_style。
使用的是write_merge。
wbk = xlwt.Workbook(encoding='utf-8')
sheet_w = wbk.add_sheet('write_after', cell_overwrite_ok=True)
sheet_w.col(3).width = 5000
tittle_style = xlwt.easyxf('font: height 300, name SimSun, colour_index red, bold on; align: wrap on, vert centre, horiz center;')
sheet_w.write_merge(0,2,0,8,u'这是标题',tittle_style)
3. 当函数中要用到全局变量时，注意加global。否则会出现UnboundLocalError:local variable'xxx' referenced before assignment.
check_num = 0
def check_data(sheet):
global check_num
check_num=check_num+1
4. 写入日期和带格式的数值。原来从sheet中读取的日期格式为2014/4/10,处理后只保留日期并做成数组用逗号分隔后写入新的excel。
date_arr = []
date=sheet.cell(row,2).value.rsplit('/')[-1]
if date not in date_arr:
date_arr.append(date)
sheet_w.write_merge(row2,row2,6,6,date_num, normal_style)
sheet_w.write_merge(row2,row2,7,7,','.join(date_arr), normal_style)
5. 当从excel中读取的日期格式为xldate时，就需要使用xlrd的xldate_as_tuple来处理为date格式。先判断表格的ctype确实是xldate才能开始操作，否则会报错。之后date格式可以使用strftime来转化为string。如：date.strftime("%Y-%m-%d-%H")
from datetime import date,datetime
from xlrd import xldate_as_tuple
if (sheet.cell(rows,3).ctype == 3):
num=num+1
date_value = xldate_as_tuple(sheet.cell_value(rows,3),book.datemode)
date_tmp = date(*date_value[:3]).strftime("%d")
6. 最后保存新写的表
wbk.save('new_excel.xls')
总结
以上就是这篇文章的全部内容了，希望本文的内容对大家的学习或者工作能带来一定的帮助

