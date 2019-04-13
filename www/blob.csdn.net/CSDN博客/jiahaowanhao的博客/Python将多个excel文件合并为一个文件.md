
# Python将多个excel文件合并为一个文件 - jiahaowanhao的博客 - CSDN博客


2018年06月02日 14:18:32[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1127


[Python将多个excel文件合并为一个文件](http://cda.pinggu.org/view/25711.html)
这篇文章主要为大家详细介绍了Python将多个excel文件合并为一个文件，具有一定的参考价值，感兴趣的小伙伴们可以参考一下
思路
利用python xlrd包读取excle文件，然后将文件内容存入一个列表中，再利用xlsxwriter将内容写入到一个新的excel文件中。
完整代码
\# -*- coding: utf-8 -*-
\#将多个Excel文件合并成一个
import xlrd
import xlsxwriter
\#打开一个excel文件
def open_xls(file):
fh=xlrd.open_workbook(file)
return fh
\#获取excel中所有的sheet表
def getsheet(fh):
return fh.sheets()
\#获取sheet表的行数
def getnrows(fh,sheet):
table=fh.sheets()[sheet]
return table.nrows
\#读取文件内容并返回行内容
def getFilect(file,shnum):
fh=open_xls(file)
table=fh.sheets()[shnum]
num=table.nrows
for row in range(num):
rdata=table.row_values(row)
datavalue.append(rdata)
return datavalue
\#获取sheet表的个数
def getshnum(fh):
x=0
sh=getsheet(fh)
for sheet in sh:
x+=1
return x

if __name__=='__main__':
\#定义要合并的excel文件列表
allxls=['F:/test/excel1.xlsx','F:/test/excel2.xlsx']
\#存储所有读取的结果
datavalue=[]
for fl in allxls:
fh=open_xls(fl)
x=getshnum(fh)
for shnum in range(x):
print("正在读取文件："+str(fl)+"的第"+str(shnum)+"个sheet表的内容...")
rvalue=getFilect(fl,shnum)
\#定义最终合并后生成的新文件
endfile='F:/test/excel3.xlsx'
wb1=xlsxwriter.Workbook(endfile)
\#创建一个sheet工作对象
ws=wb1.add_worksheet()
for a in range(len(rvalue)):
for b in range(len(rvalue[a])):
c=rvalue[a][b]
ws.write(a,b,c)
wb1.close()
print("文件合并完成")
源文件excel1:
![](http://cda.pinggu.org/uploadfile/image/20180602/20180602064347_42217.png)
源文件excel2:
![](http://cda.pinggu.org/uploadfile/image/20180602/20180602064354_47113.png)

运行结果：
![](http://cda.pinggu.org/uploadfile/image/20180602/20180602064402_33267.png)
合并后的excel3

![](http://cda.pinggu.org/uploadfile/image/20180602/20180602064412_73396.png)


