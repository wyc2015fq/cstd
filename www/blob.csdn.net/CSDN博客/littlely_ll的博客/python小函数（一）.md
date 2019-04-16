# python小函数（一） - littlely_ll的博客 - CSDN博客





2016年12月28日 15:14:31[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：715








## 1. shape函数

numpy模块 

shape(a) 

 - a: 数组
```
np.shape(np.eye(3)) 
## 返回结果 (3L, 3L)

np.shape([[1, 2]]) 
## 返回结果(1L, 2L)

np.shape([0]) ##返回元组
## (1L,)

shape(0) ##返回空元组
## ()

a = array([[3,4,5],[6,3,6]])
a.shape[0] ##返回行数
## 2L

a.shape[1] ## 返回列数
## 3L
```

## 2. tile函数

numpy模块 

tile(A, reps) 

 - A: 数组 

 - reps: 重复次数
```
a = array([0,1,2])
tile(a,2) ##重复a 2次
## array([0, 1, 2, 0, 1, 2])

tile(a,(2,2)) ##重复a 2行，每行重复2次
## array([[0, 1, 2, 0, 1, 2],
       [0, 1, 2, 0, 1, 2]])

tile(a,(2,3,2)) 重复a 2层，每层3行，每行重复2次
## array([[[0, 1, 2, 0, 1, 2],
        [0, 1, 2, 0, 1, 2],
        [0, 1, 2, 0, 1, 2]],

       [[0, 1, 2, 0, 1, 2],
        [0, 1, 2, 0, 1, 2],
        [0, 1, 2, 0, 1, 2]]])

b = array([[1,3,2],[3,4,1]])
tile(b,2)
##array([[1, 3, 2, 1, 3, 2],
       [3, 4, 1, 3, 4, 1]])

tile(b,(2,3)) 
##array([[1, 3, 2, 1, 3, 2, 1, 3, 2],
       [3, 4, 1, 3, 4, 1, 3, 4, 1],
       [1, 3, 2, 1, 3, 2, 1, 3, 2],
       [3, 4, 1, 3, 4, 1, 3, 4, 1]])
```

## 3. argsort函数

numpy模块 

argsort(a, axis=-1, kind=’quicksort’, order=None) 

 - a: 数组 

 - axis: 按行或列排序 

 - kind: 排序算法
```
x = array([3,1,2])
argsort(x)
##返回array([1, 2, 0], dtype=int64)

 x = array([[0, 3], [2, 2]])
 argsort(x)
 ##返回array([[0, 1],
       [0, 1]], dtype=int64)

argsort(x,axis=1) ##默认按行排序
##返回array([[0, 1],
       [0, 1]], dtype=int64)

argsort(x,axis=0) ##按列排序
##返回array([[0, 1],
       [1, 0]], dtype=int64)
```

## 4. sum函数

numpy模块 

sum(a, axis=None, dtype=None, out=None, keepdims=class numpy._globals._NoValue) 

 - a: 数组 

 - axis: 按行或列求和
```
sum([[0, 1], [0, 5]]) ##全部求和
## 6

sum([[0, 1], [0, 5]], axis=0) ##按列求和
## array([0, 6])

sum([[0, 1], [0, 5]], axis=1) ##按行求和
## array([1, 5])
```

## 5. strip函数

numpy模块 

strip() 
**s为字符串，rm为要删除的字符序列**

s.strip(rm)      $\qquad$ 删除s字符串中开头、结尾处，位于 rm删除序列的字符 

s.lstrip(rm)    $\qquad$ 删除s字符串中开头处，位于 rm删除序列的字符 

s.rstrip(rm)      $\qquad$删除s字符串中结尾处，位于 rm删除序列的字符 

 当rm为空时，默认删除空白符（包括’\n’, ‘\r’,  ‘\t’,  ’ ‘)
## 6. split函数

numpy模块 

split(str=”“, num=string.count(str)) 

 - str – 分隔符，默认为空格 

 - num – 分割次数。
```
str = ('www.google.com')
str.split() #1
str.split(".") #2
str.split(".",1) #3
```

结果： 

1 [‘www.google.com’] 

2 [‘www’, ‘google’, ‘com’] 

3 [‘www’, ‘google.com’]
## 7. append和extend函数

```
a=[1,2,3]
b=[4,5,6]

a.append(b)
a
## 结果[1, 2, 3, [4, 5, 6]]

a=[1,2,3]
a.extend(b)
a
## 结果[1, 2, 3, 4, 5, 6]
```

## 8. set函数

set为内建函数，创建一个无序不重复的集合。可以做交、差、并、补集。

```
a = set("sadfssaef")
 a ##无序，不重复，集合
 ## set(['a', 's', 'e', 'd', 'f'])

x = set('spam')
y = set(['h','a','m','b'])
x, y ##返回元组
##(set(['a', 'p', 's', 'm']), set(['a', 'h', 'b', 'm']))

 x & y ##返回交集
 ##set(['a', 'm'])

x | y ##返回并集
##set(['a', 'b', 'h', 'm', 'p', 's'])

x - y ##返回差集
##set(['p', 's'])
y-x
##set(['h', 'b'])
```

## 9. nonzero函数

numpy模块 

输入为矩阵，输出对应的非零行列坐标

```
a = np.array([[1,2,3],[4,5,6],[7,8,9]])
a
```

array([[1, 2, 3], 

       [4, 5, 6], 

       [7, 8, 9]])`nonzero(a)`
(array([0, 0, 0, 1, 1, 1, 2, 2, 2], dtype=int64), array([0, 1, 2, 0, 1, 2, 0, 1, 2], dtype=int64)) 

对应的第一个矩阵为行号，即第0行，1行，2行；第二个矩阵对应的为列号，即第0，1，2列等。总起来说，第0，0位置非零，总共有9个。











