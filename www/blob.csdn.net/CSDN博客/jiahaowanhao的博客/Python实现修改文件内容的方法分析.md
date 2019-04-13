
# Python实现修改文件内容的方法分析 - jiahaowanhao的博客 - CSDN博客


2018年06月06日 19:00:09[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：104


[Python实现修改文件内容的方法分析](http://cda.pinggu.org/view/25751.html)
本文实例讲述了Python实现修改文件内容的方法。分享给大家供大家参考，具体如下：
1 替换文件中的一行
1.1 修改原文件
① 要把文件中的一行Server=192.168.22.22中的IP地址替换掉，因此把整行替换。
data = ''
with open('zhai.conf', 'r+') as f:
for line in f.readlines():
if(line.find('Server') == 0):
line = 'Server=%s' % ('192.168.1.1',) + '\n'
data += line
with open('zhai.conf', 'r+') as f:
f.writelines(data)
② 把原文件的hello替换成world。
\#!/usr/local/bin/python
\#coding:gbk
import re
old_file='/tmp/test'
fopen=open(old_file,'r')
w_str=""
for line in fopen:
if re.search('hello',line):
line=re.sub('hello','world',line)
w_str+=line
else:
w_str+=line
print w_str
wopen=open(old_file,'w')
wopen.write(w_str)
fopen.close()
wopen.close()
1.2 临时文件来存储数据
实现如下功能：将文件中的指定子串 修改为 另外的子串
python 字符串替换可以用2种方法实现:
①是用字符串本身的方法。str.replace方法。
②用正则来替换字符串: re
方法1：
\#!/usr/bin/env python
\#_*_ coding:utf-8 _*_
import sys,os
if len(sys.argv)<4 or len(sys.argv)>5:
sys.exit('There needs four or five parameters')
elif len(sys.argv)==4:
print 'usage:./file_replace.py old_text new_text filename'
else:
print 'usage:./file_replace.py old_text new_text filename --bak'
old_text,new_text=sys.argv[1],sys.argv[2]
file_name=sys.argv[3]
f=file(file_name,'rb')
new_file=file('.%s.bak' % file_name,'wb')\#文件名以.开头的文件是隐藏文件
for line in f.xreadlines():\#f.xreadlines()返回一个文件迭代器，每次只从文件（硬盘）中读一行
new_file.write(line.replace(old_text,new_text))
f.close()
new_file.close()
if '--bak' in sys.argv: \#'--bak'表示要求对原文件备份
os.rename(file_name,'%s.bak' % file_name)  \#unchanged
os.rename('.%s.bak' % file_name,file_name)  \#changed
else:
os.rename(file_name,'wahaha.txt')\#此处也可以将原文件删除，以便下一语句能够正常执行
os.rename('.%s.bak' % file_name,file_name)
方法2：
open('file2', 'w').write(re.sub(r'world', 'python', open('file1').read()))
2 使用sed
2.1 sed命令：
sed -i "/^Server/ c\Server=192.168.0.1" zhai.conf  \#-i表示在原文修改
sed -ibak "/^Server/c\Server=192.168.0.1" zhai.conf  \#会生成备份文件zhai.confbak
2.2 python调用shell的方法
① os.system(command)
在一个子shell中运行command命令，并返回command命令执行完毕后的退出状态。这实际上是使用C标准库函数system()实现的。这个函数在执行command命令时需要重新打开一个终端，并且无法保存command命令的执行结果。
② os.popen(command,mode)
打开一个与command进程之间的管道。这个函数的返回值是一个文件对象，可以读或者写(由mode决定，mode默认是'r')。如果mode为'r'，可以使用此函数的返回值调用read()来获取command命令的执行结果。
③ commands.getstatusoutput(command)
使用os. getstatusoutput ()函数执行command命令并返回一个元组(status,output)，分别表示command命令执行的返回状态和执行结果。对command的执行实际上是按照{command;} 2>&1的方式，所以output中包含控制台输出信息或者错误信息。output中不包含尾部的换行符。
④ subprocess.call(["some_command","some_argument","another_argument_or_path"])
subprocess.call(command,shell=True)**
⑤ subprocess.Popen(command, shell=True)
如果command不是一个可执行文件，shell=True不可省。
使用subprocess模块可以创建新的进程，可以与新建进程的输入/输出/错误管道连通，并可以获得新建进程执行的返回状态。使用subprocess模块的目的是替代os.system()、os.popen*()、commands.*等旧的函数或模块。
最简单的方法是使用class subprocess.Popen(command,shell=True)。Popen类有Popen.stdin，Popen.stdout，Popen.stderr三个有用的属性，可以实现与子进程的通信。
将调用shell的结果赋值给python变量
代码如下:
handle = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
print handle.communicate()[0]

