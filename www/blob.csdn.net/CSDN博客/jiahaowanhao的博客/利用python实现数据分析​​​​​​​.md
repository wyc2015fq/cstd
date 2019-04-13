
# 利用python实现数据分析​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年07月20日 21:46:56[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：52标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[数据分析																](https://so.csdn.net/so/search/s.do?q=数据分析&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=python&t=blog)


**[利用python实现数据分析](http://cda.pinggu.org/view/26146.html)**
为什么要利用python进行数据分析？python拥有一个巨大的活跃的科学计算社区，拥有不断改良的库,能够轻松的集成C,C++,Fortran代码（Cython项目），可以同时用于研究和原型的构建以及生产系统的构建。
1:文件内容格式为json的数据如何解析
import json,os,sys
current_dir=os.path.abspath(".")
filename=[file for file in os.listdir(current_dir) if ".txt" in file]\#得到当前目录中，后缀为.txt的数据文件
fn=filename[0] if len(filename)==1 else "" \#从list中取出第一个文件名
if fn: \# means we got a valid filename
fd=open(fn)
content=[json.loads(line) for line in fd]
else:
print("no txt file in current directory")
sys.exit(1)
for linedict in content:
for key,value in linedict.items():
print(key,value)
print("\n")
2：出现频率统计
import random
from collections import Counter
fruits=[random.choice(["apple","cherry","orange","pear","watermelon","banana"]) for i in range(20)]
print(fruits) \#查看所有水果出现的次数
cover_fruits=Counter(fruits)
for fruit,times in cover_fruits.most_common(3):
print(fruit,times)
\#\#\#\#\#\#\#\#运行结果如下：apple在fruits里出了5次
apple 5
banana 4
pear 4
3:重新加载module的方法py3
import importlib
import.reload(modulename)
4:pylab中包含了哪些module
from pylab import *
等效于下面的导入语句：
from pylab import *
from numpy import *
from scipy import *
import matplotlib

