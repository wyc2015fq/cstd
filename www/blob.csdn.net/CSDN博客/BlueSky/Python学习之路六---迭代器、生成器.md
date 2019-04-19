# Python学习之路六---迭代器、生成器 - BlueSky - CSDN博客
2015年11月09日 15:23:15[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：414
## 迭代器
在Python中，for循环可以用于Python中的任何类型，包括列表、元祖等等，实际上，for循环可用于任何“可迭代对象”，这其实就是迭代器
迭代器是一个实现了迭代器协议的对象，Python中的迭代器协议就是有next方法的对象会前进到下一结果，而在一系列结果的末尾是，则会引发StopIteration。任何这类的对象在Python中都可以用for循环或其他遍历工具迭代，迭代工具内部会在每次迭代时调用next方法，并且捕捉StopIteration异常来确定何时离开。
使用迭代器一个显而易见的好处就是：每次只从对象中读取一条数据，不会造成内存的过大开销。
比如要逐行读取一个文件的内容，利用readlines()方法，我们可以这么写：
```
for line in open("a.txt").readlines():
    print line
```
上面的读取文件是把文件一次性加载到内存中，然后打印。当文件很大时这个方法的内存开销就很大了。 
利用file的迭代器则可以：
```
for line in open("a.txt")
    print line
```
这种实现不仅运行速度快，而且没有显示的读取文件，知识利用且带起每次读取下一行。
迭代器不要求你事先准备好整个迭代过程中所有的元素。迭代器仅仅在迭代至某个元素时才计算该元素，而在这之前或之后，元素可以不存在或者被销毁。这个特点使得它特别适合用于遍历一些巨大的或是无限的集合，比如几个G的文件，或是斐波那契数列等等。这个特点被称为延迟计算或惰性求值(Lazy evaluation)。
**迭代器更大的功劳是提供了一个统一的访问集合的接口。只要是实现了*_iter_*()方法的对象，就可以使用迭代器进行访问。**
**迭代器的两个基本优点**： 
next方法：返回迭代器的下一个元素 
**iter**方法：返回迭代器对象本身 
生成菲波那切数列示例：
**代码1**
```python
def fab(max): 
 n, a, b = 0, 0, 1
 while n < max: 
   print b 
   a, b = b, a + b 
   n = n + 1
```
**代码2**
```python
class Fab(object): 
 def __init__(self, max): 
   self.max = max
   self.n, self.a, self.b = 0, 0, 1
 def __iter__(self): 
   return self
 def next(self): 
   if self.n < self.max: 
     r = self.b 
     self.a, self.b = self.b, self.a + self.b 
     self.n = self.n + 1
     return r 
   raise StopIteration()
```
## 使用迭代器
可以通过使用内建的工厂函数iter()来获取迭代器对象： 
eg：
```
lst = range(2)
it = iter(lst)
it.next()
```
## 生成器
生成器也是属于迭代器，只不过是生成器的函数中一般包含yield语句。当函数调用时，返回一个生成器的对象，这个对象支持迭代器接口。yield就相当于普通函数中的return语句，当做隐形返回对待就可以了。 
**生成器是逐渐产生结果的复杂递归算法的理想实现工具。**
## 八皇后问题
对于八皇后问题，是一个经典的回溯问题，则利用生成器则可以方便的解决。如下： 
conflict(state，nextx)函数，用来判断函数的在某一位置是否为可用位置。 
其中：state代表已排好位置的皇后的元组，则state[i]代表第i行即第i个皇后位于第几列（即位置）（i，state(i)）–>>第i行的第state(i)列，nextx代表这个待排皇后的水平位置。
```python
def conflict(state,nextx):
    #检查是否冲突
    nexty = len(state)
    for i in range(nexty):
        if abs(state[i]-nextx) in (0,nexty-i): 
        #检查是否位置与以前的皇后位于同一列或者对角线
            return True
    return False
def queens(num,state=()):
    for pos in range(num):
        if not conflict(state,pos):
            if pos == num -1:
            #如果是最后一个皇后则返回方法元组
                yield (pos,)
            else:
            #如果不是最后一个则将这个位置加入结果元组中
                for result in queens(num,state+(pos,))
                    yield result+(pos,)
```
