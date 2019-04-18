# 使用Python实现一个文本对比报告生成工具 - 小灰笔记 - CSDN博客





2017年04月17日 08:27:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3305








借助于difflib的功能，可以针对我们的使用情况进一步进行功能的聚合。我想要的功能是输入两个文件名以及一个输出报告的文件名之后，运行直接给出最终的报告。

代码实现如下：

  1 import sys

  2 import difflib

  3 

  4 try:

  5     file1 = sys.argv[1]

  6     file2 = sys.argv[2]

  7     report = sys.argv[3]

  8 exceptException,e:

  9 print('Error:' +
str(e))

 10     sys.exit()

 11 

 12 defGetLines(file_name):

 13 returnopen(file_name).readlines()

 14 

 15 txt_line1 = GetLines(file1)

 16 txt_line2 = GetLines(file2)

 17 

 18 d = difflib.HtmlDiff()

 19 fid = open(report,'w')

 20 fid.write(d.make_file(txt_line1,txt_line2))

 21 fid.close()




测试如下：

GreydeMac-mini:Chapter_02 greyzhang$ ls

c01.pyd2.pydns_parser.pypydiff.py

d1.pydiff.pyjoin.py

GreydeMac-mini:Chapter_02 greyzhang$ python diff.py d1.py d2.py report.html

GreydeMac-mini:Chapter_02 greyzhang$ ls

c01.pyd2.pydns_parser.pypydiff.py

d1.pydiff.pyjoin.pyreport.html




使用浏览器打开报告如下：

![](https://img-blog.csdn.net/20170417082633509?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




报告生成成功，而整个操作也简化了不少。




