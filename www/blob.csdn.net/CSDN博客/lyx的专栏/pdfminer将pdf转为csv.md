# pdfminer将pdf转为csv - lyx的专栏 - CSDN博客





2017年06月02日 17:08:39[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1775








之前随便做了一下中金所杯的金融知识大赛的试题，低分飘过。看到复试名单，突然有一个想法，这个是pdf，万一有人想分析一下每个区域的人的分布，那怎么办。

pdf文件大概是这样的。

![](https://img-blog.csdn.net/20170602170252241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


用的python库是pdfminer，这个库说实话还是有点复杂的，具体使用的时候，还是慢慢调试，print看看能够出来些什么，明白了规律之后再处理。本文作为一个记录。



```python
#!/usr/bin/python
#-*- coding: utf-8 -*-

from pdfminer.converter import PDFPageAggregator
from pdfminer.pdfparser import PDFParser
from pdfminer.pdfdocument import PDFDocument
from pdfminer.pdfpage import PDFPage
from pdfminer.pdfpage import PDFTextExtractionNotAllowed
from pdfminer.pdfinterp import PDFResourceManager
from pdfminer.pdfinterp import PDFPageInterpreter
from pdfminer.layout import *
import pandas as pd
import re
import csv


csvfile = file('csv_test.csv', 'wb')
writer = csv.writer(csvfile)
writer.writerow(['location', 'school', 'name', 'sex', 'stu_number', 'major'])
# df = pd.DataFrame(columns=[u'区域信息', u'学校名称', u'考生姓名', u'性别', u'学号', u'专业名称'])
# 打开一个pdf文件
fp = open("./list.pdf", 'rb')
#   创建一个PDF文档解析器对象
parser = PDFParser(fp)
# 创建一个PDF文档对象存储文档结构
# 提供密码初始化，没有就不用传该参数
# document = PDFDocument(parser, password)
document = PDFDocument(parser)
# 检查文件是否允许文本提取
if not document.is_extractable:
    raise PDFTextExtractionNotAllowed
# 创建一个PDF资源管理器对象来存储共享资源
# caching = False不缓存
rsrcmgr = PDFResourceManager(caching=False)
# 创建一个PDF设备对象
laparams = LAParams()
# 创建一个PDF页面聚合对象
device = PDFPageAggregator(rsrcmgr, laparams=laparams)
# 创建一个PDF解析器对象
interpreter = PDFPageInterpreter(rsrcmgr, device)
# 处理文档当中的每个页面
# doc.get_pages() 获取page列表
#for i, page in enumerate(document.get_pages()):
#PDFPage.create_pages(document) 获取page列表的另一种方式
replace = re.compile(r'\s+')
page_cnt = 0
# 循环遍历列表，每次处理一个page的内容
for page in PDFPage.create_pages(document):
    page_cnt += 1
    interpreter.process_page(page)
    # 接受该页面的LTPage对象
    layout = device.get_result()
    # 这里layout是一个LTPage对象 里面存放着 这个page解析出的各种对象
    # 一般包括LTTextBox, LTFigure, LTImage, LTTextBoxHorizontal 等等
    # if page_cnt == 1:
    #     row_num = 34
    # else:
    #     row_num = 36
    page_list = list()
    for x in layout:
        # 如果x是水平文本对象的话
        if isinstance(x, LTTextBoxHorizontal):
            text = x.get_text()
            page_list.append(text.encode('utf-8'))
            # text=re.sub(replace,'',x.get_text())
            # if len(text) != 0:
            #     print text
    page_list = page_list[7:-1]
    cut_point = len(page_list) / 6
    data = [ele for ele in zip(page_list[0:cut_point], page_list[cut_point:cut_point*2], page_list[cut_point*2:cut_point*3],\
                               page_list[cut_point*3:cut_point*4], page_list[cut_point*4:cut_point*5], page_list[cut_point*5:cut_point*6])]
    writer.writerows(data)

csvfile.close()
```
大概就是这样。




