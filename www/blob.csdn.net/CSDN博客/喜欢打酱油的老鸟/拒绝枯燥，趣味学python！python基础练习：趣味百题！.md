
# 拒绝枯燥，趣味学python！python基础练习：趣味百题！ - 喜欢打酱油的老鸟 - CSDN博客


2019年01月16日 08:09:24[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：111


[https://www.toutiao.com/a6644376171280597517/](https://www.toutiao.com/a6644376171280597517/)

> 编程求10000以内的完全数。
啊完全数，我感觉我与欧拉，梅森等数学大师又近了一步。
解题思路：
思考五分钟后完全没有思路，怎么确定一个数的所有真因子呢？百度一下看看是不是有什么模块有类似的函数吧....
没找到模块，不过找到了三行代码
```python
shu=10000
result=[a for a in range(1,shu+1) if shu%a==0]
print(result)
```
（要不说Python牛逼，用c的话没有现成函数怎么也得几十行吧。）
好那现在可以尝试编写代码了
```python
list_num = []
for i in range(1,10000):
 if sum([ a for a in range(1,i) if i % a == 0]) == i:
 list_num.append(i)
print(list_num)
```
五行代码....py天下第一！！！py天下第一！！！py天下第一！！！
```python
#!/usr/bin/env python
def isPerfectNumber(n):
 a = 1
 b = n
 s = 0
 while a < b:
 if n % a == 0:
 s += a + b
 a += 1
 b = n / a
 if a == b and a * b == n:
 s += a
 return s - n == n
for k in range(2, 10000):
 if isPerfectNumber(k):
 print k
```
思路相似，但用列表推导更简单
# 第四题：相亲数
> 220的真因数之和为1+2+4+5+10+11+20+22+44+55+110=284

> 284的真因数之和为1+2+4+71+142=220

> 毕达哥拉斯把这样的数对A、B称为相亲数：A的真因数之和为B，而B的真因数之和为A。

> 求100000以内的相亲数。
解题思路：
构建列表，遍历列表，求x的真因数并求和记作a，然后对a求真因数之和b，判断x？=b。
尝试解题：
```python
list_num = []
for i in range(2,10000):
 x = sum([a for a in range(1,i) if i % a == 0])
 y = sum([b for b in range(1,x) if x % b == 0])
 if i == y:
 list_num.append((i,x))
print(list_num)
#运行结果：[(6, 6), (28, 28), (220, 284), (284, 220), (496, 496), (1184, 1210), (1210, 1184), (2620, 2924), (2924, 2620), (5020, 5564), (5564, 5020), (6232, 6368), (6368, 6232), (8128, 8128)]
```
可以看出有重复的，有完全数和本身的
可以考虑用集合，集合是无序的而且重复元素不出现
```python
list_num = []
for i in range(2,10000):
 x = sum([a for a in range(1,i) if i % a == 0])
 y = sum([b for b in range(1,x) if x % b == 0])
 if i == y and x != y and (i,x) :
 m = {i,x}
 list_num.append(m)
print(list_num)
#[{220, 284}, {220, 284}, {1184, 1210}, {1184, 1210}, {2924, 2620}, {2924, 2620}, {5564, 5020}, {5020, 5564}, {6232, 6368}, {6368, 6232}]
[Finished in 7.7s]
```
列表还是有重复，等回头再学习一下再修改
# 第五题：黑洞数
> 黑洞数又称陷阱数，是类具有奇特转换特性的整数。任何一个数字不全相同的整数，经限“重排求差”操作，总会得到某一个或一些数，这些数即为黑洞数。“重排求差”操作即把组成该数的数字重排后得到的最大数减去重排后得到的最小数。

> 举个例子，3位数的黑洞数为495.

> 简易推导过程：随便找个数，如297,3个位上的数从小到大和从大到小各排一次，为972和279，相减得693。按上面做法再做一次，得到594，再做一次，得到495，之后反复都得到495。

> 验证4位数的黑洞数为6174。
解题思路：
对所有四位数进行如下操作：将数字分解存入列表，列表排序，组合成数字，然后大小相减，对比6174相等则验证成功
列表排序相关函数：
```python
#将列表中元素反转排序
>>> x = [1,5,2,3,4]
>>> x.reverse()
>>> x
[4, 3, 2, 5, 1]
#此函数方法对列表内容进行正向排序，排序后的新列表会覆盖原列表（id不变），也就是sort排序方法是直接修改原列表list排序方法。
>>> a = [5,7,6,3,4,1,2]
>>> a.sort()
>>> a
[1, 2, 3, 4, 5, 6, 7]
#即可以保留原列表，又能得到已经排序好的列表sorted()操作方法如下：
>>> a = [5,7,6,3,4,1,2]
>>> b = sorted(a)
>>> a
[5, 7, 6, 3, 4, 1, 2]
>>> b
[1, 2, 3, 4, 5, 6, 7]
```
本来根据我的思路大致写了一下代码如下
```python
def number_x(num,other):
 list_num = []
 while num > 0:
 a = num % 10
 num = int( num / 10 )
 list_num.append(a) 
 if other == 'min':
 list_min = sorted(list_num)
 num_min = list_min[0] + list_min[1] * 10 + list_min[2] * 100 + list_min[3] * 1000
 return num_min
 elif other == 'max':
 list_min = sorted(list_num)
 list_max = sorted(list_min)
 num_max = list_max[0] + list_max[1] * 10 + list_max[2] * 100 + list_max[3] * 1000
 return num_max
for i in range(1000,10000):
 x = i
 a = 0
 while x != 6174:
 x_min = number_x(x,'min')
 x_max = number_x(x,'max')
 x = x_max - x_min
 a += 1
 if a == 1000:
 print('fail',i)
 break
 print('success')
```
运行出错先不说，发现自己思路很死，想到一个用while取各位的数字存到列表就用到死，重排列数字之后组合成一个数字的时候用的竟然只想到了最蠢的方法，简直愚蠢之极。
然后报错问题，报错是列表超出范围了，重新走一下流程之后发现第一个数1000重组之后的数字是1000和1，1只有1位数可不超范围了，再相减之后成了999，变成一个三位数了？？？成了求三位数的黑洞数了？而且999三个数字完全相同没有黑洞数。只能说明题意有问题。百度一下发现应该是相减之后不足四位前面应该添零，大致意思是这样
1000-0001 = 0999
9990-0999 = 8991
9981-1899 = 8082
8820-0288 = 8532
8532-2358 = 6174
问题变得复杂了好多，得考虑0的问题。作为练习的前几个题不应该这么麻烦才是，先看看官方给出的解题思路。
```python
def fun(n):
 a = [int(c) for c in str(n)]
 a.sort()
 s1 = reduce(lambda x, y: 10 * x + y, a[::-1])
 s2 = reduce(lambda x, y: 10 * x + y, a)
 return n if s1 - s2 == n else fun(s1 - s2)
res = fun(6294)
print 'res : ', res
#这是python2.x版本的代码
#查了一下3.x好像是reduce()从全局函数移除了，添加到了fucntools模块里引用要用
from functools import reduce
```
好吧，老师解题也不走心，完全没考虑我这么多，，，
题意是要验证四位数的黑洞数，这个只能验证一个数不说还没考虑0的情况。
不过老师用的方法就比我好多了，这就参考老师的代码修改一下我的程序吧
突然灵光一闪，将整数操作变为字符串操作，需要相减时再int，相减之后再与6174比较，如果不同再返回字符串，如果返回的字符串长度不到4在用字符串'0'+'返回的字符串'，这样循环操作不就比直接操作整数型数字简单了么。
```python
from functools import reduce
def fun(str_n):
 x = ['1111','2222','3333','4444','5555','6666','7777','8888','9999']
 if str_n in x:
 return str_n
 elif str_n == '6174':
 return 'succes'
 else:
 a = [int(c) for c in str_n]
 a.sort()
 s1 = reduce(lambda x, y: 10 * x + y, a[::-1])
 s2 = reduce(lambda x, y: 10 * x + y, a)
 m = s1 - s2 
 s = '0' * (4 - len(str(m))) + str(m)
 fun(s)
list_fail = []
for i in range(1000,10000):
 if fun(str(i)) == 'succes':
 pass
 else:
 list_fail.append(i)
print (list_fail)
```
![拒绝枯燥，趣味学python！python基础练习：趣味百题！](http://p1.pstatp.com/large/pgc-image/154701433847490cec9fdfb)

