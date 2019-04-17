# 使用python处理中文csv文件，并让excel正确显示中文（避免乱码） - 别说话写代码的博客 - CSDN博客





2018年02月03日 17:33:16[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：1881标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)









使用python2处理中文时，经常会出现乱码问题，之前使用python处理中文csv文件时，处理过程程序输出出现了乱码，最后在程序中能够正常显示出中文，但是导出文件时，使用excel打开出现了中文乱码问题，但是使用ultraedit打开时，能正常显示。现在找到了问题的所在，总结如下：




1）程序输出中出现乱码的原因是因为python2中中文编码的问题，需要注意的是要将处理的中文文件的编码和python源文件的编码保持一致，这样不会出现中文乱码。可以参考这两篇文章[关于Python脚本开头两行的：#!/usr/bin/python和#
 -*- coding: utf-8 -*-的作用 – 指定](http://blog.csdn.net/xw_classmate/article/details/51933904)和[Python中用encoding声明的文件编码和文件的实际编码之间的关系](http://blog.csdn.net/xw_classmate/article/details/51933851)

2）在程序中能够正常输出中文，但是导出到文件后使用excel打开是出现中文乱码是因为excel能够正确识别用gb2312、**gbk**、gb18030或**utf_8 with BOM** 编码的中文，如果是**utf_8 no BOM**编码的中文文件，excel打开会乱码。




根据以上两点，结合自己的情况：我使用的是jupyter的ide，创建的python源文件为utf8 no BOM格式的编码，而要处理的csv文件是用gbk编码的，所以我需要将使用gbk编码的csv文件转为utf_8编码。








**[python]**[view
 plain](http://blog.csdn.net/xw_classmate/article/details/51940430#)[copy](http://blog.csdn.net/xw_classmate/article/details/51940430#)



- import codecs  
- 
- def ReadFile(filePath,encoding):  
-     with codecs.open(filePath,"r",encoding) as f:  
- return f.read()  
- def WriteFile(filePath,u,encoding):  
-     with codecs.open(filePath,"w",encoding) as f:  
-         f.write(u)  
- '''''
- 定义GBK_2_UTF8方法，用于转换文件存储编码
- '''
- def GBK_2_UTF8(src,dst):  
-     content = ReadFile(src,encoding='gbk')  
-     WriteFile(dst,content,encoding='utf_8')  
- 
- 
- '''''
- qyx.csv文件使用GBK编码存储，现在将其转为UTF_8存储
- '''
- src = 'qyx.csv'
- dst = 'qyx_utf8.csv'
- GBK_2_UTF8(src,dst)  






使用上面的代码能够将使用GBK编码的文件转为UTF8编码的文件。接下来就是真正处理csv文件了。





**[python]**[view
 plain](http://blog.csdn.net/xw_classmate/article/details/51940430#)[copy](http://blog.csdn.net/xw_classmate/article/details/51940430#)



- import pandas as pd  
- from urllib import unquote  
- 
- '''''
- 读取转换存储格式后的文件
- '''
- path = 'qyx_utf8.csv'
- data = pd.read_csv(path,)  
- data.head()  







![](https://img-blog.csdn.net/20160718114029688)


处理csv文件：





**[python]**[view
 plain](http://blog.csdn.net/xw_classmate/article/details/51940430#)[copy](http://blog.csdn.net/xw_classmate/article/details/51940430#)



- '''''
- 使用unquote方法对第三列的url进行decode
- '''
- #data['newcol'] = data[u'关键词代码'].map(unquote) #这样会导致key识别错误，正确做法应该是去掉u
- data['newcol'] = data['关键词代码'].map(unquote)#也可以使用ix来取得第三列的数据
- #data['newcol'] = data.ix[:,3].map(unquote)
- 
- data.head()  






![](https://img-blog.csdn.net/20160718114131032)


将data导出到文件中：





**[python]**[view
 plain](http://blog.csdn.net/xw_classmate/article/details/51940430#)[copy](http://blog.csdn.net/xw_classmate/article/details/51940430#)



- '''''
- 将结果导出到result.csv中,以UTF_8 with BOM编码（微软产品能正确识别UTF_8 with BOM存储的中文文件）存储
- '''
- #data.to_csv('result_utf8_no_bom.csv',encoding='utf_8')#导出的结果不能别excel正确识别
- data.to_csv('result_utf8_with_bom.csv',encoding='utf_8_sig')  



使用excel打开result_utf8_no_bom.csv和result_utf8_with_bom.csv，发现能够正确识别result_utf8_with_bom.csv文件



