# 使用Python进行文本信息的比较并生成HTML报告 - 小灰笔记 - CSDN博客





2017年04月17日 07:47:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2476








使用Python可以实现类似Linux中diff的命令，还能够输出HTML文档，这是我一直期待的。如果办公环境十分封闭，使用diff命令都难的时候，这无疑是很好的补充。

Python中的标准库有一个difflib，借助于这个库就能够实现所需的比较。

两个文件分别如下：

d1.py

  1 from IPyimport IP

  2 

  3 ip1 = IP('192.168.1.2')

  4 ip2 = IP('11.12.13.14')

  5 

  6 print("ip1 type: %s" % ip1.iptype())

  7 print("ip2 type: %s" % ip2.iptype())

  8 

  9 print("ip2 int value: %d" % ip2.int())

 10 print("ip2 hex value: %s" % ip2.strHex())

 11 print("ip2 bin value: %s" % ip2.strBin())

 12 

 13 print("IP for 0x1234567: %s" % IP(0x1234567))

 14 




d2.py

  1 from IPyimport IP

  2 

  3 ip1 = IP('192.168.1.2')

  4 ip2 = IP('11.33.13.14')

  5 

  6 print("ip1 type: %s" % ip1.iptype())

  7 print("ip2 type: %s" % ip2.iptype())

  8 

  9 print("ip2 int value: %d" % ip1.int())

 10 print("ip2 hex value: %s" % ip1.strHex())

 11 print("ip2 bin value: %s" % ip1.strBin())

 12 

 13 print("IP for 0x1234567: %s" % IP(0x1234567))

 14 




比较结果如下：

GreydeMac-mini:Chapter_02 greyzhang$ python pydiff.py 

  from IPy import IP









  ip1 = IP('192.168.1.2')




- ip2 = IP('11.12.13.14')




?              ^^




+ ip2 = IP('11.33.13.14')




?              ^^









  print("ip1 type: %s" % ip1.iptype())




  print("ip2 type: %s" % ip2.iptype())









- print("ip2 int value: %d" % ip2.int())




?                               ^




+ print("ip2 int value: %d" % ip1.int())




?                               ^




- print("ip2 hex value: %s" % ip2.strHex())




?                               ^




+ print("ip2 hex value: %s" % ip1.strHex())




?                               ^




- print("ip2 bin value: %s" % ip2.strBin())




?                               ^




+ print("ip2 bin value: %s" % ip1.strBin())




?                               ^









  print("IP for 0x1234567: %s" % IP(0x1234567))




差异被顺利比较出来，最终展示的格式也全都跟Linux的diff命令类似。

借用这个模块库还能够生成HTML文档，把前面的代码做一下修改如下：

  1 import difflib

  2 

  3 text1 = open('d1.py').readlines()

  4 text2 = open('d2.py').readlines()

  5 

  6 d = difflib.HtmlDiff()

  7 fid =open('report.html','w')

  8 fid.write(d.make_file(text1,text2))

  9 fid.close()




程序运行后生成了一个html文件，运行的结果以及信息如下：

GreydeMac-mini:Chapter_02 greyzhang$ python pydiff.py 

GreydeMac-mini:Chapter_02 greyzhang$ ls

c01.pyd2.pypydiff.py

d1.pydns_parser.pyreport.html




使用浏览器打开生成的报告文件如下：

![](https://img-blog.csdn.net/20170417074706032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这报告的观感确实是非常好，看多了命令行再看这个感觉简直是精美绝伦！




