# 【codecs.BOM】使用Python程序输出csv格式数据 - CD's Coding - CSDN博客





2016年03月17日 18:04:30[糖果天王](https://me.csdn.net/okcd00)阅读数：4081








这些天做一个数据处理的任务：

要求是输出一个csv格式的文件。

每次用NotePad++打开都正常，一用excel打开就一万个乱码，

好不容易乱码没了，用作分隔符的逗号也进单元格去了（变成单列了）




辛苦的查原因调试代码测试输出过程略去（以后要是有心情再来写好了）……

重点是！~

把能用的模板记录一下，以后说不定还要用呢QwQ




[Convert2Csv.py]



```python
# -*- coding: utf-8 -*-

import codecs  
from multiprocessing import Pool

import os
import re
import sys
import csv
import urllib2
import commands
import ConfigParser

if __name__ == '__main__' :
    infile = "./outSort"
    with open('Features.csv', 'wb+') as csvfile:
        csvfile.write(codecs.BOM_UTF8) 
        spamwriter = csv.writer(csvfile,dialect='excel')
        spamwriter.writerow(['Sign', 'Count', 'Feature', 'Info'])
        for each in [line for line in file(infile)] :
            list = ['0']
            sublist1 = ['','','']
            sublist2 = ['','','']
            segs = each.split('\t')
            if not segs :
                break
            list.append(segs[0])
            list.append(segs[1])
            list.append(segs[2])
            if segs[3]:
                sublist1.append(segs[3])
            if segs[4]:
                sublist2.append(segs[4])
            spamwriter.writerow(list)
            spamwriter.writerow(sublist1)
            spamwriter.writerow(sublist2)
```


输入：UTF-8编码文件



输出：Scv格式数据，使用Excel打开或NotePad打开中文均不乱码




要点：



```python
with open('Features.csv', 'wb+') as csvfile:
        csvfile.write(codecs.BOM_UTF8) 
        spamwriter = csv.writer(csvfile,dialect='excel')
```






CSV写入函数解释：



```python
spamwriter.writerow(list) 
# 传入一个list即可，写一行，每个元素为当前行的一列，若需要空行，请务必增加一个空字符串元素
```





【Updated 2016/03/18】

现在难度大了点，要合并单元格，于是就学着用了用xlwt和xlrd：



```python
# -*- coding: utf-8 -*-

import codecs  
from multiprocessing import Pool

import os
import re
import sys
import csv
import xlrd
import xlwt
import urllib2
import commands
import ConfigParser

def MakeCsv():
    infile = "./outSort"
    with open('Features.csv', 'wb+') as csvfile:
        csvfile.write(codecs.BOM_UTF8) 
        spamwriter = csv.writer(csvfile,dialect='excel')
        spamwriter.writerow(['Sign', 'Count', 'Feature', 'Info'])
        for each in [line for line in file(infile)] :
            list = ['0']
            sublist1 = ['','','']
            sublist2 = ['','','']
            segs = each.split('\t')
            if not segs :
                break
            list.append(segs[0])
            list.append(segs[1])
            list.append(segs[2])
            if segs[3]:
                sublist1.append(segs[3])
            if segs[4]:
                sublist2.append(segs[4])
            spamwriter.writerow(list)
            spamwriter.writerow(sublist1)
            spamwriter.writerow(sublist2)

def MakeInput() :
    (status, output) = commands.getstatusoutput('sort -k1 -n -r outData > outSort')
            
def Combine() :
    # Create a Workbook
    f = xlwt.Workbook()
    sheet = f.add_sheet(u'Feature',cell_overwrite_ok=True)
    # sheet.write(codecs.BOM_UTF8)
    
    # Generate 0-th line
    row0 = ['Sign', 'Count', 'Feature', 'Info']
    for i in range(0,len(row0)):
        sheet.write(0,i,row0[i])
        
    # Generate Others
    cur_row = 1
    infile = "./outSort"
    for each in [line.decode('utf-8') for line in file(infile)] :   
        segs = each.split('\t')
        if not segs :
            break
        sheet.write_merge(cur_row,cur_row+2,0,0,'0')
        sheet.write_merge(cur_row,cur_row+2,1,1,segs[0])
        sheet.write_merge(cur_row,cur_row+2,2,2,segs[1])
        sheet.write(cur_row,3,segs[2])
        if segs[3]:
            sheet.write(cur_row+1,3,segs[3])
        if segs[4]:
            sheet.write(cur_row+2,3,segs[4])
        cur_row += 3

    # print 'OVER'
    f.save('./Demo.xls')
    
if __name__ == '__main__' :
    # MakeCsv()
    MakeInput()
    Combine()
```







