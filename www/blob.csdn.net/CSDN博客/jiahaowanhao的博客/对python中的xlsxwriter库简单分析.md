
# 对python中的xlsxwriter库简单分析 - jiahaowanhao的博客 - CSDN博客


2018年06月29日 21:25:43[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：179


[对python中的xlsxwriter库简单分析](http://cda.pinggu.org/view/25958.html)
今天小编就为大家分享一篇对python中的xlsxwriter库简单分析，具有很好的参考价值，希望对大家有所帮助。一起跟随小编过来看看吧
一、xlsxwriter 基本用法，创建 xlsx 文件并添加数据
官方文档：http://xlsxwriter.readthedocs.org/
xlsxwriter 可以操作 xls 格式文件
注意：xlsxwriter 只能创建新文件，不可以修改原有文件。如果创建新文件时与原有文件同名，则会覆盖原有文件
Linux 下安装： sudo pip install XlsxWriter
Windows 下安装： pip install XlsxWriter
\# coding=utf-8
from __future__ import division
import sys
import xlsxwriter
import xlrd
import datetime
output_file = u"D:\\python和shell脚本\\10969的代码\\test.xlsx"
wb = xlsxwriter.Workbook(output_file)
ws = wb.add_worksheet(u"活动标签")
ws.set_column('A:A', 20)
ws.set_column('B:B', 20)
ws.set_column('C:C', 20)
ws.set_column('D:D', 20)
ws.write(2, 0, "123")
wb.close()
二、xlsxwriter 格式处理，将待添加数据转换成相应的格式，添加到 xlsx 文件中
先设置格式，使用方法：workbook.add_format
再指定格式写入，使用方法：worksheet.write_string
\#!/usr/bin/python
\#coding: utf-8
from datetime import datetime
import xlsxwriter
workbook = xlsxwriter.Workbook('Expenses02.xlsx')
worksheet = workbook.add_worksheet()
\#设定格式，等号左边格式名称自定义，字典中格式为指定选项
\#bold：加粗，num_format:数字格式
bold_format = workbook.add_format({'bold':True})
money_format = workbook.add_format({'num_format':'$\#,\#\#0'})
date_format =workbook.add_format({'num_format':'mmmm d yyyy'})
\#将二行二列设置宽度为15(从0开始)
worksheet.set_column(1, 1, 15)
\#用符号标记位置，例如：A列1行
worksheet.write('A1', 'Item', bold_format)
worksheet.write('B1', 'Cost', bold_format)
worksheet.write('C1', 'Cost', bold_format)
expenses = (
['Rent', '2016-03-11', 1000],
['Gad', '2016-03-12', 100],
['Food', '2016-03-13', 400],
['Gym', '2016-03-14', 50],
)
row = 1
col = 0
for item, date_str, cost in (expenses):
\#将数据格式转化为Python datetime.datetime 格式
\#之后用write_datetime方法录入日期格式
date = datetime.strptime(date_str, "%Y-%m-%d")
\#使用write_string方法，指定数据格式写入数据
worksheet.write_string(row, col,  item)
worksheet.write_datetime(row, col + 1, date, date_format)
worksheet.write_number(row, col + 2, cost, money_format)
row += 1
worksheet.write(row, 0, 'Total',  bold_format)
worksheet.write(row, 1, '=SUM(B2:B5)', money_format)
workbook.close()
三、xlsxwriter 添加表格，在 xlsx 文件中添加表格
\#!/usr/bin/python
\#coding: utf-8
import xlsxwriter
workbook = xlsxwriter.Workbook('chart.xlsx')
worksheet = workbook.add_worksheet()
\#新建图标对象
chart = workbook.add_chart({'type': 'column'})
\#向 excel 中写入数据，建立图标时要用到
data = [
[1, 2, 3, 4, 5],
[2, 4, 6, 8, 10],
[3, 6, 9, 12, 15],
]
worksheet.write_column('A1', data[0])
worksheet.write_column('B1', data[1])
worksheet.write_column('C1', data[2])
\#向图表中添加数据，例如第一行为：将A1~A5的数据转化为图表
chart.add_series({'values': '=Sheet1!$A$1:$A$5'})
chart.add_series({'values': '=Sheet1!$B$1:$B$5'})
chart.add_series({'values': '=Sheet1!$C$1:$C$5'})
\#将图标插入表单中
worksheet.insert_chart('A7', chart)
workbook.close()
四、更多可用的单元格式对象（Format Class）
ecxel 中每一个单元，都有如下属性：字体（fonts）、颜色（colors）、模式（patterns）、边界（borders）、alignment、number formatting
设置属性：
format = workbook.add_format()
\#用对象接口设置格式属性
format.set_bold()
format.set_font_color('red')
\#用字典设置格式属性
property = {
'bold': True,
'font_color': 'red'
}
format = workbook.add_format(property)
以上这篇对python中的xlsxwriter库简单分析就是小编分享给大家的全部内容了，希望能给大家一个参考

