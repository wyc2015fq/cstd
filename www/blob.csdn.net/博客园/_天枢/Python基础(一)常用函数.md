# Python基础(一)常用函数 - _天枢 - 博客园
## [Python基础(一)常用函数](https://www.cnblogs.com/yhleng/p/7541781.html)
2017-09-18 10:17 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7541781)
**1、map() 此函数可以，将列表内每一个元素进行操作，并返回列表**
```
原型 map(function,[list])
```
```
def fc(x):
    return x * 2
print(map(fc,[1,2,3,4,5]))
```
**输出：**
```
[2,4,6,8,10]
```
**是不是很方便，当然有人说，可以写成for循环，是的这样也可以实现，但是我们有更简便的方法，有必要写这么一个for循环吗。**
```
list = []
for i in [1,2,3,4,5]:
    list.append(i*2)
print(list)
```
**输出：**
```
[2,4,6,8,10]
```
**当然map()函数还有更多，更复杂的用法**
```
print(map(str,[1,2,3,4,5]))
print(list(map(lambda x:x*2,[1,2,3,4,5]))) #每一个元素，乘2，返回列表
```
**结果：列表内元素都转换成了字符串**
```
['1', '2', '3', '4', '5']
```
**首字母大写；用到str.title属性**
```
print(map(str.title,['abc','def','ghj']))
```
**输出：**
```
['Abc', 'Def', 'Ghj']
```
**2、eval()此函数，将字符串，转换成其类型对象**
**为什么说eval()是转换成,其类型对象呢,这是因为,具体是什么类型的数据取决于,字符串内容**
**如:eval("1234")这个会转成int型**
**eval("{'user':'name'}") 这样会转成字典等**
**例子：将字符串，转换成字典**
```
strdic = '''{'username':'pyhleng','password':'q123456'}
'''
print eval(strdic)
print(eval(strdic)['password'])
```
**输出：**
```
{'username': 'pyhleng', 'password': 'q123456'}
q123456
```
** 有没有上边的代码是多此一举，直接定义一个字典对象不就行了，为什么要有三引号，引上。是啊为什么要这样做？？？？？**
**上边代码只是写一个例子，模拟将数据{'username':'pyhleng','password':'q123456'} 存储在数据表，然后在读出来的样子。**
**如果将{'username':'pyhleng','password':'q123456'}存在数据表中，在读出来的时候就是字符串。如果要当字典用那么**
**eval()函数是少不了的。**
**3、reduce()此函数，函数参数必须有两个，把结果与序列的下一个元素做累计**
```
def add(x,y):
    return x + y
print reduce(add,[1,2,3,4,5])
```
**输出：**
```
15
```
**add函数必须有两个参数，然后每次，结果和下一个做累计，稍加改动1*2*3*4*5**
```
def add(x,y):
    return x * y
print reduce(add,[1,2,3,4,5])
```
输出：
```
120
```
**4、filter()此函数，传入一个函数和序列做为参数**
**过滤，序列中的每一个元素，符合函数条件True的留下，False的去掉，最后返回一个过虑后的序列**
```
def fcq(x):
    return x==4
print filter(fcq,[1,3,4,5,4,6,4])
```
输出：
```
[4, 4, 4]
```
**5、lambda()匿名函数，有时不需要显示的定义函数，可以使用此函数**
**以map()为例：lambda y: y*2 其中y:是参数，y*2是函数体**
```
print(map(lambda y: y*2,[1,2,3,4,5]))
```
```
print map(lambda y: y*2,[1,2,3,4,5])
相当于：
def fc(y)
    return y * 2
print map(fc,[1,2,3,4,5])
```
**6、sorted()排序（可以对list，dict,字符串等排序）**
定义一个比较函数，定义x,y两个参数，进行比较，大于返回－1，小于返回1，等于返回0
然后使用sorted()进行排序。
倒序：
```
def cmp(x,y):
    if x>y:
        return -1
    if x<y:
        return 1
    return 0
print(sorted([1,2,3,4,5]),cmp)
```
输出：
```
[5, 4, 3, 2, 1]
```
正序：
```
print(sorted([1,2,3,4,5]))
```
输出：
```
[1, 2, 3, 4, 5]
```
**7、**args和**kwargs参数**
这两个是python的可变参数，其中*args是元组；**kwargs是字典类型参数
示例如下：*args和**kwargs可以同时使用。
```
def foo(*args,**kwargs):
    print(args)
    print(kwargs)
foo(1,2,3)#元组参数这样使用
foo(a=5,b=6,c=7) #字典传参写x =x这种格式
foo(a=1,b=2,c=3)
foo(4,5,6,e=5,f=6,g=7)
```
输出：因为函数定义两个类型参数，调用时有的只写了一个类型，所以会有()或{}输出。
```
(1, 2, 3)
{}
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
()
{'a': 5, 'c': 7, 'b': 6}
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
()
{'a': 1, 'c': 3, 'b': 2}
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
(4, 5, 6)
{'e': 5, 'g': 7, 'f': 6}
```
**8、日期时间函数 **
**获取当前日期时间**
```
import time
```
```
curTime = time.strftime('%Y.%m.%d %H:%M:%S',time.localtime()).decode('utf-8')
```
格式化字符串：
%Y 年
%m 月
%d  日
%H 时
%M 分
% S 秒
**9.list.reverse() 列表翻转**
```
list = ['a','b','c']
print list.reverse() #返回None，这个只是实现list的返向排列，返回None
print list #返回['c', 'b', 'a']
```
**10.MAX()**
```
print  max([1,2,3,4,5,6]) #返回列表中，最大的元素6
```
**11.ZIP()**
```
print zip('a','b','c','d') #返回元组[('a', 'b', 'c', 'd')]
```
**12.all()**
```
print all(['1','2','3','4']) #所有元素为True时返回True,all([])返回True
```
**13.any()**
```
print any(['1','2','3','4']) #所有元素为True时返回True,all([])返回False
```
**14.list.sort()**
```
list = ['c','b','a']
print list.sort() #返回None
print list #返回['a', 'b', 'c']
```
