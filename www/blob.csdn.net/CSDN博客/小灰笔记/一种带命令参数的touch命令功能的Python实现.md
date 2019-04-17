# 一种带命令参数的touch命令功能的Python实现 - 小灰笔记 - CSDN博客





2017年04月12日 22:16:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1237








       这个还是很实用的，尤其是在封闭的系统环境中，你自己无法安装甚至拷贝程序的时候。恰好你的计算机中有Python，那么一切都水到渠成了。

       主要用的模块是Python的sys模块，用它是为了支持参数的传递。

       简单的代码实现如下：

#!/usr/binpython



import sys



defTouch(file_name):

       fid = open(file_name,'w')

       fid.close()



creat_flg = False

for each_file insys.argv:

       if creat_flg:

              Touch(each_file)

       else:

              creat_flg = True



       代码的运行结果如下：

In [6]: %runtouch.py one.c two.pl three.m four.py



In [7]: ls

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\01_编程语言\03_Python\python_exp05的目录



2017/04/12  22:14   <DIR>          .

2017/04/12  22:14   <DIR>          ..

2017/04/12  22:14                 0 four.py

2017/04/12  22:14                 0 one.c

2017/04/12  22:14                 0 three.m

2017/04/12  22:14               216 touch.py

2017/04/12  22:14                 0 two.pl

               5 个文件            216 字节

               2 个目录76,869,181,440 可用字节

     顺利实现了多个文件的创建，这个比我之前写的一个傻瓜版本又有提升咯。



