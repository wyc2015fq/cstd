# Python 深度学习模型训练数据预处理 批文件重命名 os.listdir方法  os.rename方法 file.spilt方法 - YZXnuaa的博客 - CSDN博客
2018年01月15日 19:52:59[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：234
批文件重命名
# # !/usr/bin/env python
# # 2.py
# # use UTF-8
# # Python 3.6.0
import os
path = '/home/ubuntu/图片/data_anti/4'
count = 1
for file in os.listdir(path):
    os.rename(os.path.join(path,file),os.path.join( path, "mouse." + str(count)))
    count+=1![](https://img-blog.csdn.net/20180115195755043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- os.rename(old_file_path, new_file_path), 只能对相应的文件进行重命名, 不能重命名文件的上级目录名.
- os.renames(old_file_path, new_file_path), 是os.rename的升级版, 既可以重命名文件, 也可以重命名文件的上级目录名.
二、os.path.join方法
在拼接路径的时候用的。举个例子，
os.path.join(“home”, "me", "mywork")
在[Linux系统](https://www.baidu.com/s?wd=Linux%E7%B3%BB%E7%BB%9F&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbnjbsuhRdnhnsnjRknWfv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHn3n1TkrHfdPWc1nWDdnHRYn0)上会返回
“home/me/mywork"
在[Windows系统](https://www.baidu.com/s?wd=Windows%E7%B3%BB%E7%BB%9F&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dbnjbsuhRdnhnsnjRknWfv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHn3n1TkrHfdPWc1nWDdnHRYn0)上会返回
"home\me\mywork"
好处是可以根据系统自动选择正确的路径分隔符"/"或"\"
三、spilt方法
## 描述
Python split()通过指定分隔符对字符串进行切片，如果参数num 有指定值，则仅分隔 num 个子字符串
## 语法
split()方法语法：
str.split(str="", num=string.count(str)).
## 参数
- str -- 分隔符，默认为所有的空字符，包括空格、换行(\n)、制表符(\t)等。
- num -- 分割次数。
## 返回值
返回分割后的字符串列表。
## 实例
以下实例展示了split()函数的使用方法：
#!/usr/bin/python
str ="Line1-abcdef \nLine2-abc \nLine4-abcd";print str.split();print str.split(' ',1);
以上实例输出结果如下：
['Line1-abcdef','Line2-abc','Line4-abcd']['Line1-abcdef','\nLine2-abc \nLine4-abcd']
re模块可以使用多种分隔方式
import re
a='Beautiful, is; better*than\nugly'
# 四个分隔符为：,  ;  *  \n
x= re.split(',|; |\*|\n',a)
print(x)

