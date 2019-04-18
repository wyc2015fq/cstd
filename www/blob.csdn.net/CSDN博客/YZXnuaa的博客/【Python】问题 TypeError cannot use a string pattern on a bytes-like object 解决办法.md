# 【Python】问题 TypeError: cannot use a string pattern on a bytes-like object  解决办法 - YZXnuaa的博客 - CSDN博客
2018年02月22日 14:24:25[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：577
代码
import numpy as np
import matplotlib.pyplot as plt
import os
import re, urllib
import urllib.request as urllib2
class BDTB:
    baseUrl = 'https://tieba.baidu.com/p/5556510729?see_lz=1'
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
        print(items)
bdtb = BDTB()  #实例化
bdtb.getPage()
bdtb.Title()
