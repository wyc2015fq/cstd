# python--爬取http://www.kuaidaili.com/并保存为xls - 默槑 - 博客园







# [python--爬取http://www.kuaidaili.com/并保存为xls](https://www.cnblogs.com/modaidai/p/7003407.html)





代码如下：

复制在python3上先试试吧^_^

```
# -*- coding: utf-8 -*-
"""
Created on Mon Jun 12 13:27:59 2017

@author: admin
"""

import urllib.request
import os
import re
from bs4 import BeautifulSoup
import xlwt

os.chdir(r'C:\Users\admin\Desktop') #把文件储存至桌面
url='http://www.kuaidaili.com/'     #网页地址
req=urllib.request.Request(url)     #打开
req.add_header('User-Agent','Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36')
#增加User-Agent，更改访问的信息，别让后台太关注
response=urllib.request.urlopen(req)    #继续打开
html=response.read().decode('utf-8')    #编码,变为str格式
soup=BeautifulSoup(html,'lxml')   #这个模块太好用了，这个模块也是这个脚本的核心


ww = soup.find_all('tbody')
ww=str(ww)                   
#rr = re.findall(r'<td (.*)</td>',ww)
#print (ww)      #这几行代码都没有用，但可以发现，ww是不能直接拿来
#print(rr)       #进行检索的，需要要str转一下格式
biaoti=re.findall(r'"(.*)"',ww)
biaoti = set(biaoti)            #把标题去冗余
biaoti=list(biaoti)
biaoti=list(reversed(biaoti))   #所有元素翻转，

item = biaoti.pop(0)   
biaoti.insert(2, item)  
 # 弹出第一个元素，并作为第三个元素插入，来进行位置的调换

list_name=[]
result=[]

for guanjianzi in biaoti:
#    if rr[i].find(guanjianzi) != -1:
    list_name=re.findall(r'"%s">(.*)</td>'%guanjianzi,ww)
    list_name.insert(0,guanjianzi)      #插入标题
    result.extend(list_name)
    
hh=[]

for i in range(0,len(result),11): #由一个列表变为
      hh.append(result[i:i+11])   #一个有很多个列表组成的嵌套列表
    
workbook=xlwt.Workbook()
worksheet=workbook.add_sheet('sheet1',cell_overwrite_ok = True)

for i in range(len(hh)):
    for e in range(len(hh[i])):
         worksheet.write(e,i,hh[i][e])
workbook.save('123.xls')
```














