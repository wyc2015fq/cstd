# python学习小结 - xmdxcsj的专栏 - CSDN博客





2014年01月19日 18:47:25[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1553








Python学习了半个月了，现在将其知识点罗列一下：

一对python脚本语言的认识

直观上面来讲，在语法和对象的概念上面python跟C++很像，作为脚本语言，它又具备简单便捷的特点，尤其体现在对文本资料的处理上面，简单的几行代码，就能完成C语言几十行代码的功能。

有了其他语言的基础，再来学习python就容易很多，推荐的做法是拿来别人的python脚本直接学习，不懂就百度，很快就能熟悉python的语法和常用函数。

二  python安装和调试

Windows下面的python集成开发环境推荐使用Pythonwin，相比于Python Shell，它可以像C++一样对整个.py脚本文件进行调试（包括断电和单步运行），比较容易上手。

Linux下一般自带有python，或者sudoapt-get install python。在命令行下输入python，出现python解释器即说明安装成功。Linux下面的python脚本调试：

首先你选择运行的 py

python -m pdb myscript.py

(Pdb) 会自动停在第一行，等待调试,这时你可以看看帮助

(Pdb) h 

说明下这几个关键命令



>断点设置

(Pdb)b  10 #断点设置在本py的第10行

或(Pdb)b  ots.py:20 #断点设置到 ots.py第20行

删除断点（Pdb）b #查看断点编号
           (Pdb)cl 2 #删除第2个断点



>运行

(Pdb)n #单步运行

(Pdb)s #细点运行也就是会下到，方法

(Pdb)c #跳到下个断点



>查看

(Pdb)p param #查看当前变量值

(Pdb)l #查看运行到某处代码

(Pdb)a #查看全部栈内变量

三基本的知识

1.   特殊的运算符：

a**b：幂运算，相当于pow(a,b)

//：整除符号

真：True

假：False

2.   模块

Math：

floor()：向下取整

ceil()：向上取整

   cmath：复数的运算

四python的基本数据结构

1.      元组

元组不能修改，以（）表示，用到的比较少。使用元组作为字典的键的时候，元组不能被列表代替。

2.      列表

a=[1,2,3,4,5,6]

2.1   索引

a[-1]表示最后一个元素，a[0]表示第一个元素

2.2   分片

a[1,3]表示：2，3

a[-3:-1]表示：5，6

a[-3:]表示：5，6

a[:3]表示：1，2，3

a[:]表示：1，2，3，4，5，6

a[0:5:2]表示（2表示步长）：1，3，5

a[5:0:-1]表示：6，5，4，3，2

2.3   增加

a=[1,5]

a[1:1]=[2,3,4]

a=[1,2,3,4,5]

2.4   删除

a=[1,2,3,4,5]

a[1:4]=[]

a=[1,5]

2.5   函数

.append() 列表尾部添加

.count() 某个元素出现的次数

.extend() 列表后面添加列表

.index() 第一个匹配项的索引位置

.insert(x,y) 在x索引插入元素y

.pop() 列表移除最后一个元素，并且返回该元素的值

.remove(x) 移除列表中的第一个x元素

.reverse() 列表中的元素反向存储

.sort() 改变原始列表；key cmpreverse参数可以实现高级排序

.sorted() 获取已排序的列表副本，不改变原始列表

2.6   其他运算

相加：[1，2]+[3，4] [1，2，3，4]

相乘：[5]*2  [5，5]

Len，max，min

List：字符串转化为列表

“”.join()：列表转化为字符串

复制列表：

Y=x[:] 复制

Y=x x和y同时指向一个列表，不是复制

3.      字典

3.1   创建

直接创建

phonebook = {‘a’:’1’, ‘b’:’2’}

使用dict函数

item = [(‘a’,’b’),(‘1’,’2’)]

phonebook = dict(item)

3.2   基本操作

len(d) 字典的条目数

d[k] 键值k对应的值

del d[k] 删除键值k的项

3.3   函数

clear 清楚字典的项

.copy() 浅拷贝

deepcopy() 深拷贝

.fromkeys() 对给定的键值建立字典

.get() 获取字典中的项

.has_key() 检查字典是否包含给出的键值

… …

4.      字符串

4.1   格式化

price = 40

print “The price is %d”%price

4.2   常用函数

b.find(a,i,j) 返回b中字串a的最左边索引，i和j限定b的范围

a.join(b) 列表b的元素之间用a来连接

a.replace() 替换

strip() 取出两侧空格

.translate() 可以一次完成多个替换，结合maketrans(“cd”,”ab”)，将c替换为a，将d替换为b

五语句结构

1.      判断

if  :

elif :

else:

2.      循环

while :

    break;/continue;

for :

六函数和模块

1.      函数

def square(x):

‘Calculatesthe square of the number x‘

2.      模块

from module import methord

methord()



七文件的读写

1.      文件名作为参数传入

import sys

sys.argv[1]#表示文件a

sys.argv[2]#表示文件b



python test.py a b

2.      打开文件

f = open(“file”, ‘r/w/a/b/+’)

3.      读取所有内容

while True:

line = f.readline()

if not line:

    break

…

f.close()



for line inf.readlines():

f.close()



