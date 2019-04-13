
# python的命令解析getopt.getopt（）函数分析 - 高科的专栏 - CSDN博客

2011年08月12日 14:13:04[高科](https://me.csdn.net/pbymw8iwm)阅读数：3599


【转自[http://hi.baidu.com/javalang/blog/category/Python](http://hi.baidu.com/javalang/blog/category/Python)】
可以参考http://docs.python.org/lib/module-getopt.html
\# -*- coding: cp936 -*-
import getopt
import sys
def usage():
print '''Help Information:
-h: Show help information
-xValue:
...'''
if __name__=='__main__':
\#set default values
x=1
y='y'
try:
print sys.argv[1:]
opts,args=getopt.getopt(sys.argv[1:],'hx:y:d')
\#opts 是带-选项的参数
\#args 是没有选项的参数
print opts
print args
\#h表示使用-h,h选项没有对应的值
\#x:表示你要使用-xValue,x选项必须有对应的值.
except getopt.GetoptError:
\#打印帮助信息并退出
usage()
sys.exit(2)
\#处理命令行参数
for o,a in opts:
if o=='-h':
usage()
sys.exit()
if o=='-x':
try:
x=x+int(a) \#注意默认a为字符串
except ValueError:
print 'Invalid Value'
print x
if o=='-d':
print 'use -d'
if o=='-y':
y=y+a
运行结果：
getopt_example.py -x12 -y ss -d sdf s123
['-x12', '-y', 'ss', '-d', 'sdf', 's123']
[('-x', '12'), ('-y', 'ss'), ('-d', '')]
['sdf', 's123']
13
use -d

