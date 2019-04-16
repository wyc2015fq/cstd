# python 笔记（刷题常用函数） - 技术成长笔记 - CSDN博客





2017年07月05日 14:38:47[zhixuhao](https://me.csdn.net/u012931582)阅读数：2979








抱着C++，Python两手都要抓的原则，又把python复习了一下，并且刷了一些题，发现有的笔试题用python刷起来真是爽，尤其是涉及字符串的题目。

所以这里总结一下这两天常用的函数。

对循环输入的字符串做处理：

```
while True:
    try:
        x = raw_input()

        if(not x):
            break           
    except:
        break
```

str()：数字转字符串

int()：浮点数转int或整数字符串转int，int(‘1’) = int(1.2) = 1 

同样可以做进制转换，int(‘0xA1’,16) = 161

isalpha()：判断字符串是否全是字母

isdigit()：判断字符串是否全是数字

is_integer()：判断浮点数是否为整数

join()：”分隔符”.join(字符串序列)

str.count(substr,start,end)：计数substr在范围内出现的次数

str.upper()：转大写

str.lower()：转小写

列表反转可以用reverse()，改变原有列表顺序。

也可以用切片进行反转，l[::-1],对于字符串，只能用切片返回一个反转后的字符串。

列表删除元素：list.remove(值)，删除匹配到的第一个元素或者list.pop(索引)

字典：和c++中的map类似，key-value方式。dict.has_key(key)判断有无元素,dict[key] = value插入或赋值。字典

排序：列表有自带的sort函数，直接对原列表进行更改。 

对于字典，字符串等数据结构，必须用sorted函数。 

sorted(iterable[, cmp[, key[, reverse]]])
cmp和key都是函数，key的输出是cmp的输入

比如dic = {‘a’:2,’b’:1}

sorted(dic.items(),key=lambda d:d[0])是按照key排序， 

返回[(‘a’,2),(‘b’,1)] 

把d[0]换成d[1]是按照value排序，cmp函数默认是升序
sorted(“123321”)返回[‘1’, ‘1’, ‘2’, ‘2’, ‘3’, ‘3’]





