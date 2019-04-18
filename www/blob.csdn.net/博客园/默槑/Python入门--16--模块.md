# Python入门--16--模块 - 默槑 - 博客园







# [Python入门--16--模块](https://www.cnblogs.com/modaidai/p/7290383.html)





模块的定义：

模块是一个包含所有你定义的函数和变量的文件，其后缀是.py。模块可以被别的程序引入，以使用该模块中的函数等功能

比如

import random

secret=random.randint(1,10)   #调用random模块中的randint函数，随机生成一个十以内的一个随机整数

import os   #OS：Operation System   插一句：ios就是 Iphone Operation System  苹果操作系统的意思

　　　　　#因为不同的操作系统的文件管理方式是不同的，所python的作者就搞出了这么一个跨平台的文件操作模块

　　　　　#到什么平台，它就变为什么样的文件操作模式，你妈再也不用担心你的文件操作了



**os模块中关于文件/目录常用的函数使用方法**


|**函数名**|**使用方法**|
|----|----|
|getcwd()|返回当前工作目录|
|chdir(path)|改变工作目录|
|listdir(path='.')|列举指定目录中的文件名（'.'表示当前目录，'..'表示上一级目录）|
|mkdir(path)|创建单层目录，如该目录已存在抛出异常|
|makedirs(path)|递归创建多层目录，如该目录已存在抛出异常，注意：'E:\\a\\b'和'E:\\a\\c'并不会冲突|
|remove(path)|删除文件|
|rmdir(path)|删除单层目录，如该目录非空则抛出异常|
|removedirs(path)|递归删除目录，从子目录到父目录逐层尝试删除，遇到目录非空则抛出异常|
|rename(old, new)|将文件old重命名为new|
|system(command)|运行系统的shell命令|
|walk(top)|遍历top路径以下所有的子目录，返回一个三元组：(路径, [包含目录], [包含文件])【具体实现方案请看：第30讲课后作业^_^】|
|*以下是支持路径操作中常用到的一些定义，支持所有平台*| |
|os.curdir|指代当前目录（'.'）|
|os.pardir|指代上一级目录（'..'）|
|os.sep|输出操作系统特定的路径分隔符（Win下为'\\'，Linux下为'/'）|
|os.linesep|当前平台使用的行终止符（Win下为'\r\n'，Linux下为'\n'）|
|os.name|指代当前使用的操作系统（包括：'posix',  'nt', 'mac', 'os2', 'ce', 'java'）|


########################################os.path不属于os模块，需要单独import

**os.path模块中关于路径常用的函数使用方法**
|**函数名**|**使用方法**|
|----|----|
|basename(path)|去掉目录路径，单独返回文件名|
|dirname(path)|去掉文件名，单独返回目录路径|
|join(path1[, path2[, ...]])|将path1, path2各部分组合成一个路径名|
|split(path)|分割文件名与路径，返回(f_path, f_name)元组。如果完全使用目录，它也会将最后一个目录作为文件名分离，且不会判断文件或者目录是否存在|
|splitext(path)|分离文件名与扩展名，返回(f_name, f_extension)元组|
|getsize(file)|返回指定文件的尺寸，单位是字节|
|getatime(file)|返回指定文件最近的访问时间（浮点型秒数，可用time模块的gmtime()或localtime()函数换算）|
|getctime(file)|返回指定文件的创建时间（浮点型秒数，可用time模块的gmtime()或localtime()函数换算）|
|getmtime(file)|返回指定文件最新的修改时间（浮点型秒数，可用time模块的gmtime()或localtime()函数换算）|
|*以下为函数返回 True 或 False*| |
|exists(path)|判断指定路径（目录或文件）是否存在|
|isabs(path)|判断指定路径是否为绝对路径|
|isdir(path)|判断指定路径是否存在且是一个目录|
|isfile(path)|判断指定路径是否存在且是一个文件|
|islink(path)|判断指定路径是否存在且是一个符号链接|
|ismount(path)|判断指定路径是否存在且是一个挂载点|
|samefile(path1, paht2)|判断path1和path2两个路径是否指向同一个文件|





print(os.name)      #window会输出nt  ios会输出mac

插一句：

getatime(file)显示出来的是一大串浮点型数字，所以需要转化为时间来查看。

import time 

time.localtime(os.path.getatime('C:\\log.txt'))      #这样输出的是你能看懂的时间



os实例：

import os

path=r'D:'
os.chdir(path)
print(os.getcwd())                                             #输出当前所在目录

os.makedirs(os.getcwd()+'\\a\\b')　　　　　　#创建一个递归目录，当前目录+\a\b
os.rmdir(os.getcwd()+'\\a\\b')　　　　　　　   #删除b
os.rmdir(os.getcwd()+'\\a')　　　　　　　　　#删除a
os.system('cmd')　　　　　　　　　　　　　 #调用系统命令，这是调用cmd
os.system('calc')　　　　　　　　　　　　　  #调用计算器
print(os.name)　　　　　　　　　　　　　　  #输出操作系统名称　












