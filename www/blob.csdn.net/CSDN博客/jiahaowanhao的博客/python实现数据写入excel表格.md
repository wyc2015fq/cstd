
# python实现数据写入excel表格 - jiahaowanhao的博客 - CSDN博客


2018年06月28日 21:24:22[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：665


[python实现数据写入excel表格](http://cda.pinggu.org/view/25945.html)
这篇文章主要为大家详细介绍了python实现数据写入excel表格，具有一定的参考价值，感兴趣的小伙伴们可以参考一下
本文实例为大家分享了python数据写入excel表格的具体代码，供大家参考，具体内容如下
安装： xlsxwriter第三方库
code：
\#!/usr/bin/env/python
\#_*_coding:utf-8_*_
\#Data:2017-08-13
\#Auther:苏莫
\#Link:http://blog.csdn.net/lingluofengzang
\#PythonVersion:python2.7
\#filename:xlsx.py
import sys
\# import os
import xlsxwriter
'''
pip install xlsxwriter
'''
reload(sys)
sys.setdefaultencoding("utf-8")
\# path = os.path.dirname(os.path.abspath(__file__))
\# 建立文件
workbook = xlsxwriter.Workbook("text.xlsx")
\# 可以制定表的名字
\# worksheet = workbook.add_worksheet('text')
worksheet = workbook.add_worksheet()
\# 设置列宽
\# worksheet.set_column('A:A',10)
\# 设置祖体
bold = workbook.add_format({'bold':True})
\# 定义数字格式
\# money = workbook.add_format({'num_format':'$\#,\#\#0'})
\# 写入带粗体的数据
worksheet.write('A1','data',bold)
worksheet.write('B1','work')
'''
worksheet.write(0, 0, 'Hello')     \# write_string()
worksheet.write(1, 0, 'World')     \# write_string()
worksheet.write(2, 0, 2)        \# write_number()
worksheet.write(3, 0, 3.00001)     \# write_number()
worksheet.write(4, 0, '=SIN(PI()/4)')  \# write_formula()
worksheet.write(5, 0, '')        \# write_blank()
worksheet.write(6, 0, None)       \# write_blank()
'''
worksheet.write('A3',15)
worksheet.write('B3',20)
worksheet.write('C3',44)
worksheet.write('D3',36)
\# xlsx计算数据
worksheet.write('E3','=SUM(A3:D3)')
'''
建立Chart对象： chart = workbook.add_chart({type, 'column'})
Chart: Area, Bar, Column, Doughnut, Line, Pie, Scatter, Stock, Radar
将图插入到sheet中： worksheet.insert_chart('A7', chart)
'''
\# 定义插入的图标样式
chart = workbook.add_chart({"type":'column'})
headings = ['a','b','c']
data = [
[1,2,3,4,5],
[2,4,6,8,10],
[3,6,9,12,15],
]
\# 按行插入数据
worksheet.write_row('A4',headings)
\# 按列插入数据
worksheet.write_column('A5',data[0])
worksheet.write_column('B5',data[1])
worksheet.write_column('C5',data[2])
\# 图行的数据区
\# name：代表图例名称；
\# categories：是x轴项，也就是类别；
\# values:是y轴项，也就是值；
chart.add_series({
'name':'=Sheet1!$B$4',
'categories':'=Sheet1!$A$5:$A$9',
'values':'=Sheet1!$B$5:$B$9',
})
chart.add_series({
'name':['Sheet1', 3, 2],
'categories':['Sheet1', 4, 0, 8, 0],
'values':['Sheet1', 4, 2, 8, 2],
})
\# 图形的标题
chart.set_title ({'name': 'Percent Stacked Chart'})
\# 图形X轴的说明
chart.set_x_axis({'name': 'Test number'})
\# 图形Y轴的说明
chart.set_y_axis({'name': 'Sample length (mm)'})
\# 设置图表风格
chart.set_style(11)
\# 插入图形,带偏移
worksheet.insert_chart('D12',chart,{'x_offset': 25, 'y_offset': 10})
workbook.close()
以上就是本文的全部内容，希望对大家的学习有所帮助

