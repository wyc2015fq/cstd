# 【Python3】数据挖掘 抓取小说 - YZXnuaa的博客 - CSDN博客
2018年02月22日 16:35:32[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：162

# !/usr/bin/python
# -*- coding: UTF-8 -*-
"""AI Demo: Look like the title
"""
__author__ = "Bruce Y"
__copyright__ = "property of mankind."
__license__ = "NUAA"
__version__ = "0.0.1"
__maintainer__ = "Gang B"
__email__ = "brucezixiang@163.com"
__status__ = "Development"
import numpy as np
import matplotlib.pyplot as plt
import os
import re, urllib
import urllib.request as urllib2
class BDTB:
    baseUrl = 'https://tieba.baidu.com/p/5556510729?see_lz=1'
#初始化
def __init__(self):
        self.user_agent = 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0'
self.html_data=[]
    #打开网页，获取源码
def getPage(self):
        try:
            url = self.baseUrl
            request = urllib2.Request(url)      # banbenxiuzheng
response = urllib2.urlopen(request)
            a = response.read()
            # print(a)
return a
        except Exception:
            print(Exception)
    def Title(self):
        html= self.getPage()
        reg= re.compile(r'content=".*?"')   #参数是正则表达式  编译，提高效率
html = html.decode('utf-8')  # python3
items= re.findall(reg, html)
        # print(items[0])   # 打印第一个
# 文件写入
for item in items:
            # print(item)
f=open('pachong1.txt','w') # 新建
f.write('标题'+'\t'+item)   #只会记录最后一个
f.close()
        return items
    def Text(self):
        html=self.getPage()
        reg=re.compile(r'class="d_post_content j_d_post_content ">            (.*?)</div><br>',re.S) # 匹配换行符
html = html.decode('utf-8')  # python3
req=re.findall(reg,html)
        # print(req)
for i in req:
            #剔除不需要的东西，用空格代替
q=re.compile('<a.*?>|</a>')
            t=re.compile('<img.*?>')          # 图片
y=re.compile('http.*?.html')      #网址
i = re.sub(q, "", i)
            i = re.sub(t, "", i)
            i = re.sub(y, "", i)
            i = i.replace('<br>','')
            print(i)
            f = open('pachong1.txt','a') # 追加模式
f.write('\n\n'+i)   #正文段落之间空两行
f.close()
bdtb = BDTB()  #实例化
bdtb.Title()
bdtb.Text()
