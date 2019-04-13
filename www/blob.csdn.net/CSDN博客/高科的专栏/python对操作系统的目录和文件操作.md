
# python对操作系统的目录和文件操作 - 高科的专栏 - CSDN博客

2011年08月31日 10:03:49[高科](https://me.csdn.net/pbymw8iwm)阅读数：846标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[脚本																](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)[import																](https://so.csdn.net/so/search/s.do?q=import&t=blog)[filenames																](https://so.csdn.net/so/search/s.do?q=filenames&t=blog)[path																](https://so.csdn.net/so/search/s.do?q=path&t=blog)[os																](https://so.csdn.net/so/search/s.do?q=os&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=path&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=filenames&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=filenames&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=import&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=import&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=python&t=blog)
一、获取当前目录下的特定文件列表
>>>import glob,os
>>>curdir = os.getcwd()   \#获取当前目录
>>>os.chdir(workdir)        \#设置当前目录
>>>dir = glob.glob('*.dat')  \#获取当前目录的dat文件列表
>>>os.chdir(curdir)           \#还原当前工作目录
>>>print dir
['mudlog.dat','ddd.dat']

二、os模块的文件和目录操作函数
1．获得当前路径
os.getcwd()
该函数不需要传递参数，它返回当前的目录。
>>> import os
>>> print 'current directory is ',os.getcwd()
current directory is  D:/Python25/Lib/site-packages/pythonwin
\#这里是PythonWin的安装目录
2．获得目录中的内容
os.listdir(path)     path：要获得内容目录的路径。
>>> import os
>>> os.listdir(os.getcwd())    \# 获得当前目录中的内容
['dde.pyd', 'license.txt', 'Pythonwin.exe', 'scintilla.dll', 'win32ui.pyd',
'win32uiole.pyd', 'pywin']
3．创建目录
os.mkdir(path)    path：要创建目录的路径。
>>> import os
>>> os.mkdir('E://book//temp')   \# 使用os.mkdir创建目录
4．删除目录
os.rmdir(path)   path：要删除的目录的路径。
>>> import os
>>> os.rmdir('E://book//temp')   \# 删除目录
需要说明的是，使用os.rmdir删除的目录必须为空目录，否则函数出错。
若想删除非空目录，先删除目录下的文件，然后再删除目录，递归过程。
5．判断是否是目录
os.path.isdir(path)    path：要进行判断的路径。
>>> import os
>>> os.path.isdir('E://book//temp')  \# 判断E:/book/temp是否为目录
True           \# 表E:/book/temp是目录
6．判断是否为文件
os.path.isfile(path)   path：要进行判断的路径。
>>> import os
>>> os.path.isfile('E://book//temp')  \# 判断是否为文件
False           \# 表示E:/book/temp不是文件

三、批量重命名
在日常工作中经常会遇到这样的情况，需要将某个文件夹下的文件按照一定的规律重新命名。如果手工完成的话，需要耗费大量的时间，而且
容易出错。在学习Python以后，完全可以写一个简单的脚本完成这样的工作。
import os
perfix = 'Python'      \# perfix 为重命名后的文件起始字符
length = 2         \# length 为除去perfix后，文件名要达到的长度
base = 1         \# 文件名的起始数
format = 'mdb'       \# 文件的后缀名
\# 函数PadLeft将文件名补全到指定长度
\# str 为要补全的字符
\# num 为要达到的长度
\# padstr 为达到长度所添加的字符
def PadLeft(str , num , padstr):
stringlength = len (str)
n = num - stringlength
if n >=0 :
str=padstr * n + str
return str
\# 为了避免误操作，这里先提示用户
print 'the files in %s will be renamed' % os.getcwd()
input = raw_input('press y to continue/n') \# 获取用户输入
if input != 'y':       \# 判断用户输入，以决定是否执行重命名操作
exit()
filenames = os.listdir(os.curdir)   \# 获得当前目录中的内容
\# 从基数减1，为了使下边 i = i + 1在第一次执行时等于基数
i = base - 1
for filename in filenames:    \# 遍历目录中内容，进行重命名操作
i = i+1
\# 判断当前路径是否为文件，并且不是“rename.py”
if filename != "rename.py" and os.path.isfile(filename):
name = str(i)      \# 将i转换成字符
name = PadLeft(name,length,'0') \# 将name补全到指定长度
t = filename.split('.')   \# 分割文件名，以检查其是否是所要修改的类型
m = len(t)
if format == '':     \# 如果未指定文件类型，则更改当前目录中所有文件
os.rename(filename,perfix+name+'.'+t[m-1])
else:        \# 否则只修改指定类型
if t[m-1] == format:
os.rename(filename,perfix+name+'.'+t[m-1])
else:
i = i – 1     \# 保证i连续
else:
i = i – 1       \# 保证i连续

四、代码框架生成器
编写代码要养成良好的习惯，为了使脚本更具可读性，往往需要添加注释，而且还应该在脚本头添加基本的说明，如作者、文件名、日期、用途、版权说明，以及所需要使用的模块等信息。这样，不仅便于保存脚本，而且也便于交流。但是，如果每次编写一个脚本就依次添加这样的信息，不免有些麻烦，以下代码实现了一个简单的代码框架生成器。
\# -*- coding:utf-8 -*-
\# file: MakeCode.py
\#
import os
import sys
import string
import datetime
\# python脚本模板
py = '''\#-----------------------------------------------------
\# TO:
\#-----------------------------------------------------
\# BY:
\#-----------------------------------------------------
'''
\# c模板
c = ''' *-----------------------------------------------------
* TO:
*-----------------------------------------------------
* BY:
*-----------------------------------------------------
'''
if os.path.isfile(sys.argv[1]):   \# 判断要创建的文件是否存在，如果存在则退出脚本
print '%s already exist!' % sys.argv[1]
sys.exit()
file = open(sys.argv[1], 'w')    \# 创建文件
today = datetime.date.today()    \# 获得当前日期，并格式化为xxxx-xx-xx的形式
date = today.strftime('%Y')+'-'+today.strftime('%m')+'-'+today.strftime('%d')
filetypes = string.split(sys.argv[1],'.') \# 判断将创建的文件是什么类型以便对其分别处理
length = len(filetypes)
filetype = filetypes[length - 1]
if filetype == 'py':
print 'use python mode'
file.writelines('\# -*- coding:utf-8 -*-')
file.write('/n')
file.writelines('\# File: ' + sys.argv[1])
file.write('/n')
file.write(py)
file.write('\# Date: ' + date)
file.write('/n')
file.write('\#-----------------------------------------------------')
elif filetype == 'c' or filetype == 'cpp':
print 'use c mode'
file.writelines(' /n')
else:
print 'just create %s' % sys.argv[1]
file.close()        \# 关闭文件
写好的脚本可以放到Windows的系统目录中，这样就可以随时运行，方便地在目录中产生Python脚本或者C/C++文件。

五、运行其他程序
在Python中可以方便地使用os模块运行其他的脚本或者程序，这样就可以在脚本中直接使用其他脚本，或者程序提供的功能，而不必再次编写实现该功能的代码。
使用os.system函数运行其他程序
os模块中的system()函数可以方便地运行其他程序或者脚本。其函数原型如下所示。
os.system(command)      command ：要执行的命令，相当于在Windows的cmd窗口中输入的命令。如果要向程序或者脚本传递参数，可以使用空格分隔程序及多个参数。
以下实例实现通过os.system()函数打开系统的记事本程序。
>>> import os
\# 使用os.system()函数打开记事本程序
>>> os.system('notepad')
0 \# 关闭记事本后的返回值
\# 向记事本传递参数，打开python.txt文件
>>> os.system('notepad python.txt')
From：http://book.51cto.com/art/200710/58166.htm

