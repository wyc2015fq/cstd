# list tuple dict set的对比（挖坑） - 博客堂 - CSDN博客





2017年03月08日 22:24:11[最小森林](https://me.csdn.net/u012052268)阅读数：481









- [基本用法](#基本用法)- [List 列表](#list-列表)
- [Tuple 元组](#tuple-元组)
- [Dict字典](#dict字典)
- [set 集合](#set-集合)

- [考虑性能](#考虑性能)



# 基本用法

定义: 

list:链表,有序的项目, 通过索引进行查找,使用方括号”[]”; 

tuple:元组,元组将多样的对象集合到一起,不能修改,通过索引进行查找, 使用括号”()”; 

dict:字典,字典是一组键(key)和值(value)的组合,通过键(key)进行查找,没有顺序, 使用大括号”{}”; 

set:集合,无序,元素只出现一次, 自动去重,使用”set([])” 

应用场景: 

list, 简单的数据集合,可以使用索引; 

tuple, 把一些数据当做一个整体去使用,不能修改; 

dict,使用键值和值进行关联的数据; 

set,数据只出现一次,只关心数据是否出现, 不关心其位置;
## List 列表

创建List：L = [‘Adam’, ‘Lisa’, ‘Bart’, ‘Gechong’, ‘Kongming’]

显示List：L[0]

遍历List：print (L)和for循环

更新List：append()和insert()和直接赋值

删除List：pop()和pop(n)

**List特点**：

可以添加删除

```
Li = ['Adam', 'Lisa', 'Bart', 'Gechong', 'Kongming']

print (Li)

for name in Li:
    print(name)
```

## Tuple 元组

创建Tuple：Tu = (‘Adam’, ‘Lisa’, ‘Bart’, ‘Gechong’, ‘Kongming’)

显示Tuple：Tu[0]

遍历Tuple：print (Tu)和for循环

更新Tuple：不可以更新

删除Tuple：不可以删除

Tuple特点：不可以改变，如果创建之后一个元素的Tuple时，要加”,”

```
Tu = ('Adam', 'Lisa', 'Bart', 'Gechong', 'Kongming')

print (Tu)

for name in Tu:
    print(name)
```

## Dict字典

创建Dict：Di = {‘Adam’:100, ‘Lisa’:90, ‘Bart’:93, ‘Gechong’:100, ‘Kongming’:100}

显示Dict：Di[‘Adam’]和Di.get(‘Adam’)

遍历Dict：print(Di)和for循环

```
Di = {'Adam':100, 'Lisa':90, 'Bart':93, 'Gechong':100, 'Kongming':100}

for key in Di:
    print (key)
```

```
Di = {'Adam':100, 'Lisa':90, 'Bart':93, 'Gechong':100, 'Kongming':100}

for key in Di:
    print (Di[key])
```

更新Dict：对应位置赋值即可

删除Dict：

**Dict特点**：

1：查找速度快

2：浪费空间

3：key不可以重复，且不可变

4：数据无序排放

## set 集合

创建set：s = set([‘Adam’, ‘Lisa’, ‘Bart’, ‘Gechong’, ‘Kongming’])

显示set：

```
s = set(['Adam', 'Lisa', 'Bart', 'Gechong', 'Kongming'])

if 'Test' in s:
    print ('Test')
else:
    print ('No')
```

遍历set：print (s)

```bash
s = set(['Adam', 'Lisa', 'Bart', 'Gechong', 'Kongming'])

for name in s:
    print (name)
```

更新set：s.add()

删除set：s.remove()

# 考虑性能

Python中Set和List的性能差距能有数百倍  

如果有需要求（集合，列表等）的并集和交集的时候，最好使用Set。

set和lsit可以自由转换，在删除list中多个/海量重复元素时，可以先转换成set，然后再转回list并排序(set没有排序)。此种方法不仅方便且效率较高。




