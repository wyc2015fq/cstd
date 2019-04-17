# 一句python，一句R︱列表、元组、字典、数据类型、自定义模块导入（格式、去重） - 素质云笔记/Recorder... - CSDN博客





2016年08月23日 11:46:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4606








先学了R，最近刚刚上手python,所以想着将python和R结合起来互相对比来更好理解python。最好就是一句python，对应写一句R。

pandas中有类似R中的read.table的功能，而且很像。




————————————————————————————————————————————————————


# 一、数据类型




（来源：[Python 变量类型](http://www.runoob.com/python/python-variable-types.html)）





Python有五个标准的数据类型：
- Numbers（数字）
- String（字符串）
- List（列表）              使用：[]      list()
- Tuple（元组）          使用：（）  tuple()
- Dictionary（字典）   使用：{ }    dict()
- 
其中pandas和numpy中的数组格式 以及Series DataFrame都是基于此之上而得到的。其中比R要多：Tuple、Dictionary两种类型。



## 1、数字格式 int()  float() long()  complex()






Python支持四种不同的数字类型：
- int（有符号整型）
- long（长整型[也可以代表八进制和十六进制]）
- float（浮点型）
- complex（复数）




一些数值类型的实例：
|int|long|float|complex|
|----|----|----|----|
|10|51924361L|0.0|3.14j|
|100|-0x19323L|15.20|45.j|
|-786|0122L|-21.9|9.322e-36j|
|080|0xDEFABCECBDAECBFBAEl|32.3+e18|.876j|
|-0490|535633629843L|-90.|-.6545+0J|
|-0x260|-052318172735L|-32.54e100|3e+26J|
|0x69|-4721885298529L|70.2-E12|4.53e-7j|
- 长整型也可以使用小写"L"，但是还是建议您使用大写"L"，避免与数字"1"混淆。Python使用"L"来显示长整型。
- Python还支持复数，复数由实数部分和虚数部分构成，可以用a + bj,或者complex(a,b)表示， 复数的实部a和虚部b都是浮点型




格式转换






格式判断：



```python
import types
if type(1) is types.Integer:
    print('1是int类型')
else:
    print('1不是int类型')
```
还可使用：

```python
isinstance(2, float)
```








以下几个内置的函数可以执行数据类型之间的转换。这些函数返回一个新的对象，表示转换的值。
|函数|描述|
|----|----|
|int(x [,base])|将x转换为一个整数|
|long(x [,base] )|将x转换为一个长整数|
|float(x)|将x转换到一个浮点数|
|complex(real [,imag])|创建一个复数|
|str(x)|将对象 x 转换为字符串|
|repr(x)|将对象 x 转换为表达式字符串|
|eval(str)|用来计算在字符串中的有效Python表达式,并返回一个对象|
|tuple(s)|将序列 s 转换为一个元组|
|list(s)|将序列 s 转换为一个列表|
|set(s)|转换为可变集合|
|dict(d)|创建一个字典。d 必须是一个序列 (key,value)元组。|
|frozenset(s)|转换为不可变集合|
|chr(x)|将一个整数转换为一个字符|
|unichr(x)|将一个整数转换为Unicode字符|
|ord(x)|将一个字符转换为它的整数值|
|hex(x)|将一个整数转换为一个十六进制字符串|
|oct(x)|将一个整数转换为一个八进制字符串|





## 2、字符串 str（）




字符串或串(String)是由数字、字母、下划线组成的一串字符。


可参考：

# [一句python，一句R︱python中的字符串操作、中文乱码](http://blog.csdn.net/sinat_26917383/article/details/52300938)



其中：

字符串中的反引号为：



```python
>>> print b + repr(a)   #repr(a)与上面的类似
free1989
```




这里 repr()是一个函数，其实就是反引号的替代品，它能够把结果字符串转化为合法的 python 表达式。

可能看官看到这个，就要问它们三者之间的区别了。首先明确，repr()和 `\` 是一致的，就不用区别了。接下来需要区别的就是 repr()和 str，一个最简单的区别，repr 是函数，str 是跟 int 一样，一种对象类型。




————————————————————————————————————————————————————





## 二、列表型/list/ []  =R=c()向量






**速查手册：**



```
L.append(var)   #追加元素  
L.insert(index,var)  
L.pop(var)      #返回最后一个元素，并从list中删除之  
L.remove(var)   #删除第一次出现的该元素  
L.count(var)    #该元素在列表中出现的个数  
L.index(var)    #该元素的位置,无则抛异常   
L.extend(list)  #追加list，即合并list到L上  
L.sort()        #排序  
L.reverse()     #倒序  
list 操作符:,+,*，关键字del  
a[1:]       #片段操作符，用于子list的提取  
[1,2]+[3,4] #为[1,2,3,4]。同extend()  
[2]*4       #为[2,2,2,2]  
del L[1]    #删除指定下标的元素  
del L[1:3]  #删除指定下标范围的元素  
list的复制  
L1 = L      #L1为L的别名，用C来说就是指针地址相同，对L1操作即对L操作。函数参数就是这样传递的  
L1 = L[:]   #L1为L的克隆，即另一个拷贝。
```






List（列表） 是 Python 中使用最频繁的数据类型。



列表可以完成大多数集合类的数据结构实现。它支持字符，数字，字符串甚至可以包含列表（所谓嵌套）。

```
list = [ 'abcd', 786 , 2.23, 'john', 70.2 ]
tinylist = [123, 'john']
```


list中的元素追加，那可以直接：

list = []

list = list + list

或者list.append

**append是添加单个元素，如果要追加同样元组，可以用list.extend**




**[]或者() 追加用 加号 + 或者 list.append**




两个列表同时迭代：




```
nfc = ["Packers", "49ers"]
afc = ["Ravens", "Patriots"]
for teama, teamb in zip(nfc, afc):
     print teama + " vs. " + teamb
>>> Packers vs. Ravens
>>> 49ers vs. Patriots
```


**格式转化：**



（1）列表转为字符串

''.join(a)




（2）列表转换为元组

l = ['a','b','c']

tuple(l)




（3）列表转换为字典

list1 = dic.items()

dict(list1)




（4）把List压平

sum(list,[])



```python
[[1,2,3], [5, 2, 8], [7,8,9]]
[1,2,3,5,2,8,7,8,9]
```
其余方式可参考：https://www.zhihu.com/question/27010691





————————————————————————————————————————————————————





# 三、元组——（）/ tuple（）  =R= 固定的c（）






元组是另一个数据类型，类似于List（列表）。

元组用"()"标识。内部元素用逗号隔开。但是元组不能二次赋值，相当于只读列表。不能用append来新赋值




```
以下是元组无效的，因为元组是不允许更新的。而列表是允许更新的：
#!/usr/bin/python
# -*- coding: UTF-8 -*-

tuple = ( 'abcd', 786 , 2.23, 'john', 70.2 )
list = [ 'abcd', 786 , 2.23, 'john', 70.2 ]
tuple[2] = 1000 # 元组中是非法应用
list[2] = 1000 # 列表中是合法应用
```

相当于固定的c()



元组中元素的追加，就可以直接用:

用 '+' 号

a+a




**元组不可以用append添加元素**




**格式转化：**



元组转换为字符串

''.join(t)

元组转换为列表

t = ('a','b','c')

list(t)

['a','b','c']




————————————————————————————————————————————————————




# 四、Python元字典 { }  =R= list()




字典(dictionary)是除列表以外python之中最灵活的内置数据结构类型。列表是有序的对象结合，字典是无序的对象集合。




两者之间的区别在于：字典当中的元素是通过键来存取的，而不是通过偏移存取。

字典用"{ }"标识。字典由索引(key)和它对应的值value组成。




**速查手册：**



```
dictionary的方法  
D.get(key, 0)       #同dict[key]，多了个没有则返回缺省值，0。[]没有则抛异常  
D.has_key(key)      #有该键返回TRUE，否则FALSE  
D.keys()            #返回字典键的列表  
D.values()          #以列表的形式返回字典中的值，返回值的列表中可包含重复元素  
D.items()           #将所有的字典项以列表方式返回，这些列表中的每一项都来自于(键,值),但是项在返回时并没有特殊的顺序           
  
D.update(dict2)     #增加合并字典  
D.popitem()         #得到一个pair，并从字典中删除它。已空则抛异常  
D.clear()           #清空字典，同del dict  
D.copy()            #拷贝字典  
D.cmp(dict1,dict2)  #比较字典，(优先级为元素个数、键大小、键值大小)  
                    #第一个大返回1，小返回-1，一样返回0  
              
dictionary的复制  
dict1 = dict        #别名  
dict2=dict.copy()   #克隆，即另一个拷贝。
```








**生成方式一：用在函数、for循环中**

dict = {}
dict['one'] = "This is one"
dict[2] = "This is two"

但是dict有一个好处，就是不仅可以list[1] 还可以list[strings]，其中可以装下字符。




**生成方式二：{}**

tinydict = {'name': 'john','code':6734, 'dept': 'sales'}




输出方式：

print dict[2] # 输出键为 2 的值
print tinydict # 输出完整的字典
print tinydict.keys() # 输出所有键
print tinydict.values() # 输出所有值






延伸

一种特殊的，字典的生成方式：


`dict(dim=[1, 3, 227, 227])`





格式转化，由list->数组：



```
np.array('d',[1,2,3])

转回来的话调用tolist函数

_.tolist()
```
还有一种方式是：.toarray()变为array






延伸二：dict格式转化



```
字典转换为列表
dic={'a':1,'b':2}
dic.items()
[('a',1),('b',2)]
```
或者：

```
D.get(key, 0)       #同dict[key]，多了个没有则返回缺省值，0。[]没有则抛异常  
D.has_key(key)      #有该键返回TRUE，否则FALSE  
D.keys()            #返回字典键的列表  
D.values()          #以列表的形式返回字典中的值，返回值的列表中可包含重复元素  
D.items()           #将所有的字典项以列表方式返回，这些列表中的每一项都来自于(键,值),但是项在返回时并没有特殊的顺序
```

其中的.values（）就可以实现dict转化为list




字符串转化为字典：
`eval(user)`
字典转dataframe:



```python
def dict2dataframe(content_dict):
    return pd.DataFrame(list(content_dict.values()), index = list(content_dict.keys()))
```










延伸三：去掉List中的空格



```python
filter(None,[None,1,2,3,None])
```

即可




延伸四：两个dict合并



```python
dict(dict1, **dict2)
```









————————————————————————————————————————————————————





五、模组、模块导入与复查、自定义模块




1、一般传统模块

下载模块是一个麻烦的事情，一般用pip来执行，但是貌似每次下载都是一堆麻烦提示，于是转而用pycharm，很方面，傻瓜版.

一般模块就像R中的函数包，需要先调用


`library(packages)=import pandas as pd`
查看模块是否载入，一般import pandas，如果该包下载就不会用任何提示，如果没有加载成功，就会报错：



`ImportError: No module named da`
查看已有的加载包


`help("modules")    #查看安装包`



按照R语言中曾经存在的问题：

1、如何取消模块的加载?

2、模块的位置是在哪?

3、模块的信息如何调用出来？就像R中的介绍一样，有没有比较详细的说明？




## 2、自定义模块导入



上网查了下资料和自己实验了下，有几个方法：

1.如果导入的模块和主程序在同个目录下，直接import就行了

2.如果导入的模块是在主程序所在目录的子目录下，可以在子目录中增加一个空白的__init__.py文件，该文件使得python解释器将子目录整个也当成一个模块，然后直接通过“import 子目录.模块”导入即可。

3.如果导入的模块是在主程序所在目录的父目录下，则要通过修改path来解决，有两种方法：

(1)通过”import sys，sys.path.append('父目录的路径')“来改变，这种方法属于一次性的，只对当前的python解释器进程有效，关掉python重启后就失效了。

(2)直接修改环境变量：在windows中是 “ set 变量=‘路径’  ” 例如：set PYTHONPATH=‘C:\test\...’ 查看是否设置成功用echo %PYTHONPATH%,而且进到python解释器中查看sys.path,会发现已经有了新增加的路径了。这　种方式是永久的，一次设置以后一直都有效。在linux中是 "export 变量=‘路径’ “，查看是" echo $变量 "

通过修改path是通用的方法，因为python解释器就是通过sys.path去一个地方一个地方的寻找模块的。




**笔者实践一般用第二种办法，__init__.py文件，同时譬如现在有这样的目录结构：**

C:\\Users\\filename\\function_file\\file.function.py


file.function.py里面装着function1函数。




```python
import sys
ImportPath = 'C:\\Users\\filename'
sys.path.append(ImportPath)
from function_file.function import function1
```


如果报错：



```python
python ImportError: cannot import name
```

一般是.pyc文件的问题，找到对应的pyc删除掉


参考博客：[原来可以 RUN 的 突然出现此提示 ImportError: cannot import name webdriver](https://testerhome.com/topics/2741)



# [解决ImportError: cannot import name webdriver](http://www.cnblogs.com/bluescorpio/archive/2013/03/20/2971435.html)



————————————————————————————————————————





# 六、数据读入、写出




## 1、python的read_csv






```
#数据导入
df = pd.read_csv('./cpu.csv',header=0)
#中文encoding = 'gbk'
```



约等于R中的read.csv('./cpu.csv',header=T,encoding= UTF-8)


`pd.read_csv("C:\\Users\\long\\Desktop\\ex2.csv",header=None,names=["a","b","c","e","message"],index_col=["message","a"])`其中：header=None，就代表没有行名称，names代表另外命名行名称，index_col代表列。



其中读入数据的时候，不要出现中文，不然读不进去。

会出现以下的错误：



```
IOError: File C:\Users\long\Desktop\ch06\ex2.csv does not exist
```


如果出现中文，中文导入、导出都需要加上：



```
df = pd.read_csv("001.csv",encoding="gbk")
dataname.to_csv("001.csv",encoding="gbk")
```





## 2、python的to_csv

to_csv=write.csv



```
#数据导出
df.to_csv('uk_rain.csv')  #write.csv(df,"uk_rain.csv")
```


约等于R中的write.csv(df,"uk_rain.csv"),其中df是数据集的名称，跟前面的read_csv不太一样。



更一般的表现形式：


`pd.read_table("./marks.csv", sep=",")`

![](https://img-blog.csdn.net/20161022125713623)


## 3、txt文件导入——np.loadtxt

用numpy中的一个函数可以实现txt文件的导入。


`np.loadtxt("/caffe/examples/lmdb_test/train/synset.txt", str, delimiter='\t')`






4、读取word文档——docx包

参考：[Python读取word文档（python-docx包）](http://www.cnblogs.com/TensorSense/p/7413297.html)



```python
pip install  python-docx
```
安装与下载。记住不是直接pip docx




```python
import docx
```

数据读入




```python
# 读取word内容
#　这里是以段落为单位的，下面用一个for 遍历所有段落
doc = docx.Document("D:\\test2.docx")
parag_num = 0
for para in doc.paragraphs :
    print(para.text)
    parag += 1  
print ('This document has ', parag, ' paragraphs')
```
其中doc.paragraphs代表文档的段落，如何输出每个段落的内容是用：para.text。




数据保存：



```python
# 使用参数16表示将doc转换成docx，保存成docx后才能 读文件

doc.SaveAs(r"D:\\test2.docx",16)
doc.Close()
word.Quit()
```


5 其他一些格式导入

```python
f = open('file.txt','r+',encoding='utf-8')#encoding参数可以指定文件的编码
            f.readline()#读一行
            f.readable()#判断文件是否可读
            fr.writable()#判断文件是否可写
            fr.encoding#打印文件的编码
            f.read()#读取所有内容，大文件时不要用，因为会把文件内容都读到内存中，内存不够的话，会把内存撑爆
            f.readlines()#读取所有文件内容，返回一个list，元素是每行的数据，大文件时不要用，因为会把文件内容都读到内存中，内存不够的话，会把内存撑爆
            f.tell()#获取当前文件的指针指向
            f.seek(0)#把当前文件指针指向哪
            f.write('爱情证书')#写入内容
            f.fulsh()#写入文件后，立即从内存中把数据写到磁盘中
            f.truncate()#清空文件内容
            f.writelines(['爱情证书','孙燕姿'])#将一个列表写入文件中
            f.close()关闭文件
```
参考来自：[python学习笔记(三):文件操作和集合](http://www.nnzhp.cn/archives/160)






————————————————————————————————————————————————————




# 七、数据查看——行列名、查看




R中常有的两种方式——$  []：

data$colnames

data["colnames",]

函数使用办法都是：sum(data)

python中通过 . 传导式的：

data.sum




## 1、数据查看




查看数据的前5个，后5个。

data.head(5)

data.tail(5)

在R中为head(data)/tail(data)




## 2、数据类型




type(data)




## 3、列数量、行数量 len（R中的length）




len(data)      #行数

len(data.T)  #列数

其中data.T是数据转置，就可以知道数据的行数、列数。




————————————————————————————————————————




# 延伸一：遍历文件方法




   笔者作为小白在遍历文件的时候，看到几种办法挺好的：os.listdir  和  os.walk



```
os.listdir返回的是该文件夹下的所有文件名称；
os.walk可以返回父文件夹路径+文件夹下路径，貌似比较给力。
```





   网上有帮他们打包成函数的博客：[Python遍历目录的4种方法实例介绍](http://www.jb51.net/article/63965.htm)






```
#!/usr/bin/python
import os
from glob import glob

def printSeparator(func):
    def deco(path):
        print("call method %s, result is:" % func.__name__)
        print("-" * 40)
        func(path)
        print("=" * 40)
    return deco
@printSeparator
def traverseDirByShell(path):
    for f in os.popen('ls ' + path):
        print f.strip()
@printSeparator
def traverseDirByGlob(path):
    path = os.path.expanduser(path)
    for f in glob(path + '/*'):
        print f.strip()
@printSeparator
def traverseDirByListdir(path):
    path = os.path.expanduser(path)
    for f in os.listdir(path):
        print f.strip()
@printSeparator
def traverseDirByOSWalk(path):
    path = os.path.expanduser(path)
    for (dirname, subdir, subfile) in os.walk(path):
        #print('dirname is %s, subdir is %s, subfile is %s' % (dirname, subdir, subfile))
        print('[' + dirname + ']')
        for f in subfile:
            print(os.path.join(dirname, f))
if __name__ == '__main__':
    path = r'~/src/py'
    traverseDirByGlob(path)
    traverseDirByGlob(path)
    traverseDirByListdir(path)
    traverseDirByOSWalk(path)
```





    1、traverseDirByGlob、traverseDirByOSWalk两种函数可以拿到带全部路径的文件，类似：






`/data/trainlmdb/val/test_female/image_00009.jpg`
    2、traverseDirByListdir(path)可以拿到里面的文件名：






`image_00009.jpg`




   当然这个函数里面是print出来的。基于笔者的小白级写函数方法，笔者改进：






```
def traverseDirByGlob(path):
    path = os.path.expanduser(path)
    list={}
    i=0
    for f in glob(path + '/*'):
        list[i]=f.strip()
        i=i+1
    return list
```


   就可以跟其他的def函数一样return出来。




————————————————————————————————————————





# 延伸二：pickle模块的基本使用：pkl文件




　python的pickle模块实现了基本的数据序列和反序列化。通过pickle模块的序列化操作我们能够将程序中运行的对象信息保存到文件中去，永久存储；通过pickle模块的反序列化操作，我们能够从文件中创建上一次程序保存的对象


保存：



```
#使用pickle模块将数据对象保存到文件

import pickle

data1 = {'a': [1, 2.0, 3, 4+6j],
         'b': ('string', u'Unicode string'),
         'c': None}

selfref_list = [1, 2, 3]
selfref_list.append(selfref_list)

output = open('data.pkl', 'wb')

# Pickle dictionary using protocol 0.
pickle.dump(data1, output)

# Pickle the list using the highest protocol available.
pickle.dump(selfref_list, output, -1)

output.close()
```
读取：




```
with open(path, 'rb') as f:
    labels = pickle.load(f)
```
path是pkl的路径名




————————————————————————————————————————


# 延伸三：报错TypeError: 'str' object is not callable


因为一些函数名字，被用来命名临时变量了。比如：



```
len=1
len(data)
TypeError: 'str' object is not callable
```

len这个函数被之前命名了。




————————————————————————————————————————


# 延伸四：在元组和list中添加元素

一般添加元素的办法有用：

用加号 或者 append

两者的使用效果不同。

append是：list+list = 两个list，list + 元素 = 一个list

+号是： 元组 + 元组 = 一个元组

list可以使用append，而元组不可以用append添加元素

————————————————————————————————————————





# 延伸五：去重

1、for循环，同时保持原来的位置






```
ids = [1,2,3,3,4,2,3,4,5,6,1]
news_ids = []
for id in ids:
    if id not in news_ids:
        news_ids.append(id)
print news_ids
```
保持之前的排列顺序。




2、set 可以去重+排序



```
ids = [1,4,3,3,4,2,3,4,5,6,1]
ids = set(ids)
```


3、count函数迭代去重




```python
from collections import Counter\

ImagePath = traverseDirByGlob(root_dir)

Path = []
[Path.append(v)  for _,v in  ImagePath.items()]
Duplicate = [ (str(k),str(v)) for k,v in dict(Counter(tuple(Path))).items() if v>1]
```

.

## 延伸七：在Python命令行中使用pip 安装


```python
get_ipython().system(u'pip2 install -U google-cloud-vision')
```













