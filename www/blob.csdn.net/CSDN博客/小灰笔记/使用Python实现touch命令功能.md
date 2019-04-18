# 使用Python实现touch命令功能 - 小灰笔记 - CSDN博客





2017年03月16日 23:45:22[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3887
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









       公司指定的办公平台是Windows，而且给了诸多不自由的限制。如果对shell有一点点依赖，那么会是一个很麻烦的事情，毕竟对Windows的批处理命令熟悉度不够。由于touch命令使用的频繁，我觉得可以拿Python去模拟一个简单的伪touch。我所用到的touch最多的功能其实就只有一个——创建文件！这还是得说到公司办公的电脑环境上，鼠标的右键点一次反应有时候会达到一分钟，的确是很诡异。

       在尝试了申请安装Cygwin失败之后，我觉得自己还是得写点什么才好了。

       今晚状态不佳，困顿异常，但是却也丝毫没有一下子能够睡着的感觉。尝试用Python实现一下我自己需要的小功能吧！

       代码如下：

#!/usr/bin/python



import os



defTouch(file_name):

       if file_name in os.listdir('.'):

              print("file exist!")

       else:

              print("creating %s" %file_name)

              fid = open(file_name,'w')

              fid.close()



# test interface



for eacn_file in['a.txt','b.txt','c.txt']:

       Touch(eacn_file)

       测试记录如下：

E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03 的目录



2017/03/16  23:40   <DIR>          .

2017/03/16  23:40   <DIR>          ..

2017/03/16  23:38              294 exp_03.py

               1 个文件            294 字节

               2 个目录79,102,218,240 可用字节



E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03>pythonexp_03.py

creating a.txt

creating b.txt

creating c.txt



E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03>dir

 驱动器 E 中的卷没有标签。

 卷的序列号是 2013-5089



 E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03 的目录



2017/03/16  23:40   <DIR>          .

2017/03/16  23:40   <DIR>          ..

2017/03/16  23:40                 0 a.txt

2017/03/16  23:40                 0 b.txt

2017/03/16  23:40                 0 c.txt

2017/03/16  23:38               294 exp_03.py

               4 个文件            294 字节

               2 个目录79,102,218,240 可用字节



E:\WorkSpace\02_技术实践\01_编程语言\04_Python\exp_03>pythonexp_03.py

file exist!

file exist!

file exist!

       这样，文件创建的功能至少是有了。如果文件已经存在也不会出现覆盖的情况。如果想要时间戳功能，再增加额外的功能就好了。代码中加入了几个print用来显示执行情况，实际使用的代码这部分当然是去掉更舒服一些。速度快，而且少了很多不必要的信息，沉默是金嘛！



