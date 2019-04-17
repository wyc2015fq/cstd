# Python核心编程的四大神兽：迭代器、生成器 、闭包以及装饰器 - zhusongziye的博客 - CSDN博客





2018年11月01日 20:56:25[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：47标签：[Python核心编程的四大神兽：迭代器、生成器 、闭包以及装](https://so.csdn.net/so/search/s.do?q=Python核心编程的四大神兽：迭代器、生成器 、闭包以及装&t=blog)
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)









**生成器**

生成器是生成一个值的特殊函数，它具有这样的特点：第一次执行该函数时，先从头按顺序执行，在碰到yield关键字时该函数会暂停执行该函数后续的代码，并且返回一个值；在下一次调用该函数执行时，程序将从上一次暂停的位置继续往下执行。

通过一个例子来理解生成器的执行过程。求1-10的所有整数的立方并将结果打印输出，正常使用列表的实现如下：

```
def lifang_ls():
 """求1-10所用整数的立方数-列表方式实现"""
 ls = []
 for i in range(1,11):
   result = i ** 3
   ls.append(result)
 print(ls)

if __name__ == '__main__':
 lifang_ls()
```

输出结果如下：

![](https://img-blog.csdnimg.cn/20181101205422760.png)

当数据量很少时，可以很快得到结果。但是如果范围扩大到10000甚至是100000000，就会发现程序执行时间会变长，变卡，甚至有可能会因超出内存空间而出现程序崩溃的现象。这是因为当数据量变得非常大的时候，内存需要开辟很大的空间去存储这些数据，内存都被吃了，自然会变慢变卡。使用生成器就能解决这个问题。

对于上述同一个问题用生成器实现如下，将范围扩大到1-10000000：

```
def lifang_generate():
 """求1-10000000所用整数的立方数-生成器方式实现"""
 for i in range(1,10000001):
   result = i ** 3
   yield result

if __name__ == '__main__':
 G = lifang_generate()
```

执行效果如下：

![](https://img-blog.csdnimg.cn/20181101205433651.png)

可以看到没有任何的结果输出，这说明程序已经可以顺利执行。对于迭代器来讲需要用next()方法来获取值，修改主函数为以下情况可以打印输出前4个整数的立方数：

```
if __name__ == '__main__':
 G = lifang_generate()
 print(next(G))
 print(next(G))
 print(next(G))
 print(next(G))
```

输出结果如下：

![](https://img-blog.csdnimg.cn/20181101205443860.png)

到此可以看到，生成器生成的值需要使用next()方法一个一个的取，它不会一次性生成所有的计算结果，只有在取值时才调用，这时程序会返回计算的一个值且程序暂停；下一次取值时从上一次中断了的地方继续往下执行。

以取出前3个值为例，下图为生成器代码解析图：

![](https://img-blog.csdnimg.cn/20181101205455274.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

图解：Python解释器从上往下解释代码，首先是函数定义，这时在计算机内存开辟了一片空间来存储这个函数，函数没有被执行，继续往下解释；到了主函数部分，首先执行蓝色箭头1，接着往下执行到蓝色箭头2第一次调用生成器取值，此时生成器函数lifang_generate()开始执行，执行到生成器函数lifang_generate()的蓝色箭头2碰到yield关键字，这时候生成器函数暂停往下执行并且将result的结果返回，由于是第一次执行，因此result存储着1的立方的值，此时将1返回，第54行代码print(first)将结果打印输出。

主函数中程序接着往下执行到蓝色箭头3，生成器函数lifang_generate()第二次被调用，与第一次不同，第二次从上一次(也就是第一次)暂停的位置继续往下执行，上一次停在了yield处，因此蓝色箭头3所作的事情就是执行yield后面的语句，也就是第48行print('end')，执行完成之后因for循环条件满足，程序像第一次执行那样，执行到yield处暂停并返回一个值，此时返回的是2的立方数，在第57行打印输出8。

第三次调用(蓝色箭头4)与第二次类似，在理清了执行过程之后，程序执行结果如下：

![](https://img-blog.csdnimg.cn/20181101205504901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**迭代器**

这里先抛出两个概念：可迭代对象、迭代器。

凡是可以通过for循环遍历其中的元素的对象，都是可迭代对象；之前学习得组合数据类型list(列表)、tuple(元组)、dict(字典)、集合(set)等，上一小节介绍得生成器也可以使用for循环来遍历，因此，生成器也是迭代器，但迭代器不一定就是生成器，例如组合数据类型。

凡是可以通过next访问取值得对象均为迭代器，生成器就是一种迭代器。可以看到，生成器不仅可以用for循环来获取值，还可以通过next()来获取。

Python中有一个库collections，通过该库的Iterable方法来判断一个对象是否是可迭代对象；如果返回值为True则说明该对象为可迭代的，返回值为False则说明该对象为不可迭代。用Iterator方法来判断一个对象是否是迭代器，根据返回值来判断是否为迭代器。

使用Iterable分别判断列表，字典，字符串以及一个整数类型是否是可迭代对象的代码如下：

```
from collections import Iterable

def isiterable():
 """分别判断列表，字典，字符串100，整形100是不是可迭代对象"""

 ls = isinstance([],Iterable)
 dic = isinstance({},Iterable)
 strs = isinstance('100',Iterable)
 ints = isinstance(100,Iterable)
 print('输出True表示可迭代，False表示不可迭代\n\
 ls为{}，dic为{}，strs为{}，ints为{}'.format(ls,dic,strs,ints))

def main():
 isiterable()

if __name__ == '__main__':
 main()
```

执行的输出结果如下：

![](https://img-blog.csdnimg.cn/20181101205516664.png)



使用Iterator判断一个对象是否是迭代器的代码如下，与判断是否为可迭代对象类似：

```
from collections import Iterable,Iterator


def print_num():
 """定义一个产生斐波那契数列的生成器"""
 a,b = 0,1
 for i in range(1,10):
   yield b
   a,b = b,a + b

def isiterator():
 """分别判断列表，字典、生成器是否为迭代器"""

 ls_ret = isinstance([],Iterator)
 dict_ret = isinstance({},Iterator)
 genarate_ret = isinstance((x * 2 for i in range(10)),Iterator)
 print_num_ret = isinstance(print_num(),Iterator)
 print('输出True表示该对象为迭代器，False表示该对象不是迭代器\n\
   ls输出为{}，dict输出为{}，genarate输出为{}，print_num输出为{}'.format(ls_ret,dict_ret,genarate_ret,print_num_ret))

def main():
 isiterator()

if __name__ == '__main__':
 main()
```

输出的结果如下：

![](https://img-blog.csdnimg.cn/20181101205527167.png)

 组合数据类型不是迭代器，但是属于可迭代对象，可以通过iter()函数将其转换位迭代器，这样就可以使用next方法来获取对象各个元素的值，代码如下：

```
from collections import Iterable,Iterator

def trans_to_iterator():
 """使用iter()将可迭代类型-列表转换为迭代器"""

 ls = [2,4,6,8,10]
 ls_ierator = iter(ls)
 ls_ierator_is = isinstance(ls_ierator,Iterator)
 print('转换后的返回值为{}，使用next取出的第一个元素的值为{}'.format(ls_ierator_is,next(ls_ierator)))
def main():
 trans_to_iterator()

if __name__ == '__main__':
 main()
```

输出结果为：

![](https://img-blog.csdnimg.cn/20181101205540282.png)

**闭包**

内部函数对外部函数变量的引用，则将该函数与用到的变量称为闭包。以下为闭包的例子：

```
def func(num):

print('start')
def func_in():
"""闭包内容"""
  new_num = num ** 3
  print(new_num)
return func_in

if __name__ == '__main__':
ret = func(10)
ret()
```

理解闭包是理解装饰器的前提，同样通过一张图来理解闭包的执行过程：

![](https://img-blog.csdnimg.cn/20181101205551705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

图解:Python解释器从上往下解释代码，首先定义一个函数，func指向了该函数(红箭头所示)；接着到主函数执行第14行代码 ret = func(10)，此时先执行赋值号“=”右边的内容，这里调用了函数func()并传入10这个实参，函数func()代码开始执行，先是打印输出“start”，接着定义了一个函数func_in()，func_in指向了该函数，函数没有被调用，程序接着往下执行，return func_in 将函数的引用返回，第14行代码用ret接收了这个返回值，到此ret就指向了func_in所指向的函数体(绿箭头所示)。最后执行ret所指的函数。这就是闭包的整个过程，func_in()函数以及该函数内用到的变量num就称为闭包。

**装饰器**

代码的编写需要遵循封闭开放原则，封闭是指对于已有的功能代码实现不允许随意进行修改，开放是指能够对已有的功能进行扩展。例如一款手游，现在已经能够实现现有的游戏模式，但随着外部环境的变化发展(市场竞争，用户体验等)，现有的游戏模式已经不能满足用户的需求了。为了留住用户，需要加入更多的玩法来保持用户对该款游戏的新鲜感，于是开发方在原来游戏的基础上又开发了好几种游戏模式。像这样，新的游戏版本既增加了先的游戏模式，又保留了原有的游戏模式，体现了封闭开放的原则。 装饰器的作用就是在不改变原来代码的基础上，在原来的功能上进行拓展，保证开发的效率以及代码的稳定性。 打印输出九九乘法表可以通过以下代码实现：

```
def func_1():
"""打印输出九九乘法表"""
for i in range(1,10):
  for j in range(1,i + 1):
    result = i * j
    print('{}*{}={}'.format(i,j,result),end=' ')
  print('')
if __name__ == '__main__':
func_1()
```



输出结果如下： 假如现在需要实现一个功能，在不修改func_1函数代码的前提下，在九九乘法表前增加一个表头说明，在乘法表最后也增加一个说明。下面的代码实现了装饰器的功能：

```
def shuoming(func):
def shuoming_in():
  print('以下为九九乘法表:')
  func()
  print('以上为九九乘法表')
return shuoming_in

def func_1():
"""打印输出九九乘法表"""
for i in range(1,10):
  for j in range(1,i + 1):
    result = i * j
    print('{}*{}={}'.format(i,j,result),end=' ')
  print('')

if __name__ == '__main__':
func_1 = shuoming(func_1)
func_1()
```



输出结果如下： 可以看到func_1函数的代码没有任何修改，还实现了问题提出的要求，这其中的核心就在于最后两行代码。通过下图来理解装饰器执行的过程：

![](https://img-blog.csdnimg.cn/20181101205614994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



图解：跟之前一样，Python解释器自上往下解释代码，遇到定义函数的代码不用管，因为没有调用函数是不会执行的；这样直接就来到了第22行代码中，程序先执行赋值号“=”右边的代码，shuoming(func_1)调用了之前定义的函数，并传入了func_1实参，程序转到shuoming(func)执行，形参func接收实参func_1，此时func也指向了func_1所指向的函数(如图中分界线上方白色方框内的蓝箭头所示)；在shuoming()函数中代码继续往下走，在shuoming()函数内容又定义了一个shuoming_in()函数(如图中分界线上方白色方框内的蓝色方框所示)，接着往下，将shuoming_in()函数的引用返回，至此shuoming()函数执行完毕，程序回到第22行代码执行，shuoming()函数的返回值被func_1接收，此时，func_1不在指向原来的函数，转成指向shuoming_in所指向的函数(如图中分界线下方白色方框内的黄色箭头)。最后调用func_1所指向的函数，也就是shuoming_in()函数，shuoming_in()函数内的func指向了原来func_1()所指的函数(也就是生成九九乘法表的函数)，因此程序最终的结果就在九九乘法表前后各加了一个说明性字符串。

以上为装饰器的执行过程，但是以上装饰写法不够简洁，大多数情况下采取以下写法，输出结果是一样的:

```
def shuoming(func):
 def shuoming_in():
   print('以下为九九乘法表:')
   func()
   print('以上为九九乘法表')
 return shuoming_in

"""@shuoming相当于func_1 = shuoming(fucn_1)"""
@shuoming
def func_1():
 """打印输出九九乘法表"""
 for i in range(1,10):
   for j in range(1,i + 1):
     result = i * j
     print('{}*{}={}'.format(i,j,result),end=' ')
   print('')
if __name__ == '__main__':
 """直接调用func_1即可完成装饰"""
 func_1()
```

有时候有些被装饰的函数可能有以下几种情况：存在或不存在参数，有返回值或没有返回值，参数可能定长或不定长等等，为了通用性，与爬虫的请求代码一样，装饰器有着通用的写法：

```
def tongyong(func):
def tongyong_in(*args,**kwargs):
  ret = func(*args,**kwargs)
  return ret
return tongyong_in
```



使用这个装饰器装饰九九乘法表一样可以正常输出，如果需要特定的装饰效果，修改这个通用代码即可。 

**结束**

以上为生成器、迭代器、闭包以及装饰器的所有内容，其中装饰器属于难点。理解装饰器的执行过程能够更好的帮助我们进阶学习Python。

```
'''
作者：Lucker_SSY
源自：https://www.cnblogs.com/ssy3340/p/9747722.html
'''
```



