# 菜鸟学习python编程----学习拾珍系列（一） - Snoopy_Dream - CSDN博客





2018年08月07日 13:46:10[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：108








以下内容是前12章，讲完了元组这些

--------------------------------------------------------------------------------------------------

0.for...if...break

   break停止掉最内部的for循环，且注意break的位置

```python
if len(xuexiao)>2:
                for i in range(len(xuexiao)-2):
                    if xuexiao[i+1]-xuexiao[i]<33:
                        for j in range(xuexiao[i+1]+find_start,xuexiao[i+1]+step):
                            if 0<y[j]<36 and 0.5<(x.index(j,xuexiao[i+1]+find_start,xuexiao[i+1]+step)-xuexiao[i])/36<1.3:
                                xuexiao[i+1] = j
                                break
```



1.polypon(bob,n=7,length=70) #增加可读性

2.写函数接口的时候，为了保持整洁（clean）。接口的参数，尽可能只和功能有关系，内部的尽可能想办法写在里面

  e.g.用turtle用多边形画圆，涉及到n，可以用圆的周长来 n = int(2*pi*r/3)+1

    当你发现一个接口很难解释的时候，很有可能有改进的空间

3.重构

    重新组织函数，改善接口，提高复用率。

    情景：当你发现有类似代码出现的时候

4.键盘输入

```python
python2 raw_input
python3 input
>>>text = input('what ... is your name?\n')
what ... is your name?[输出提示]
yan guoxing [键入名称给text]
```

5.元组比较大小，只比较第一个。

6.遇到return的话，直接不执行剩余的代码（dead code）

    尽量保证每个可能执行的路径上都遇到return，调试的时候方便。

7.增量开发 （只增加测试一小部分代码）  保证可以正常返回,return 0.0，中间添加print

```python
def distance(x1,y1,x2,y2):

     dx , dy = x2-x1 , y2-y1

     print "dx is",dx

     return 0.0
```



9.while True 然后if break,这种的更用

```python
while Ture:
    line = input_raw('> ')
    if line == 'done':
        break
    print line

print 'done'
```

11.如果你的import了自己的模块，然后之后做了修改，需要重启，或者reload。最好是重启，否则可能会有问题。

12.针对可能无限循环（ maximu recursion depth exceeded）的地方，循环前后各自加print语句.

13.无限递归的情况，要确保自己有基准情形，然后在函数的开头加print语句，观察参数有没有向基准情形靠近。

14.对于菜鸟，可以在自己每个函数的开头加入print "进入**函数"的提示，就可以很显眼的知道执行的流程

15.对于字符串的操作，总是a = b.strip().split(' '),a = ' '.join(b),但是对于列表一般都是无返回值的，如a = a.sort(),必须是 a.sort()，a.sort()的返回值为None

16.对于一个东西，你既想对他进行改变，还想用它原来的值，用[:]。注：对于列表，如果用a = b 则 a is b。b成为是a的引用，其实在一个函数中，形参就相当于是引用（一个重命名），所以函数内部列表发生改变，外部也发生了改变。

  还有import copy

  output = copy.deepcopy(img_resize)  ·「创建一个和原来一样的变量」

```python
t=[1,2,3]
t2=t[:]
t2.sort() 
>>>t
[1,2,3]
>>>t2
[3,2,1]
```

17.列表删除pop del  remove

```python
t.pop(1) 指定位置，默认删除尾部

del t[1:6] 指定位置【为了坚持一个风格，我自己常用pop】

t.remove('b') 指定内容
```

18. in 也经常用于dict，判断key，也可以把值取到list中，再用in判断

    a = dict.values() a返回了一个列表，a['key']是用的[ ]取value。

    知识点：in在list中随着长度边长，搜索时间边长，但是在dict中算法用的散列表算法，长短时间都一样。

19.**for key in sorted(dict)按照顺序遍历所有键**

20.列表可以作为字典的value

21.返回一个元组的函数 

```python
def min_max(x):
    return min(x),max(x)
```

22. 可变长参数元组

    *开头的参数名，会将所有参数放到一个元组上，接受不定的参数。比如内置函数max

```python
def printall(*args):
    print args
```

23.**zip**，接受字符串与元组与列表，变成元组列表，常与for、**dict**连用

    访问**元组列表**

```python
s = 'abc'
t = 1,2,3【元组】或者 t = [1,2,3]
print zip(s,t)
>>>[('a',1),('b',2),('c',3)]获得一个元组列表
print zip(s,t)[1]
>>>('b',2)

for s,t in zip(s,t)访问元组列表
***
end

print dict(zip(s,t)) 快捷创造字典
>>>{'a': 1, 'c': 3, 'b': 2}
```

24.dict & items 字典与元组列表

```python
temp_x2 = list(count_dict.keys())
    temp_y2 = list(count_dict.values())
```

    字典用items获得元组列表，元组列表用dict获得字典。

```python
d = {'a': 1, 'c': 3, 'b': 2}
t = d.items()
print t   >>>[('a',1),('b',2),('c',3)]
```

25.字典的键必须是元组或者字符串，不可变的这种，不能是列表。

    在有些环境中，如返回语句中，一般是选择**创建元组**

    当你向一个函数传序列的参数的时候，选择元组可以减少潜在的由于建明导致的不可预知的行为。









