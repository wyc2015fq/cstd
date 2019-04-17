# python︱函数、for、if、_name_、迭代器、防范报错、类定义、装饰器、argparse模块 - 素质云笔记/Recorder... - CSDN博客





2017年01月09日 18:59:35[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1184








新手入门python,开始写一些简单函数，慢慢来，加油~




### 文章目录
- [一、函数 +三个内建函数filter，map和reduce +  if](#_filtermapreduce___if_10)
- [1、def/lambda](#1deflambda_13)
- [2、if](#2if_36)
- [如果if + for列表的方式同时使用：](#if__for_50)
- [3、filter](#3filter_67)
- [4、map](#4map_89)
- [5、reduce](#5reduce_117)
- [二、for、while](#forwhile_143)
- [三、迭代器](#_268)
- [四、**__name__**](#__name___328)
- [**延伸：if __name__ == '__main__':**](#if___name______main___354)
- [五、防范报错](#_366)
- [1、assert 断言](#1assert__369)
- [2、try...except...](#2tryexcept_387)
- [报错并提示异常信息：](#_430)
- [3、报错日志记录：Logger.exception](#3Loggerexception_476)
- [3.1 logger - basicConfig 常规存储](#31_logger__basicConfig__504)
- [3.2 常规：logging + handlers + TimedRotatingFileHandler](#32_logging__handlers__TimedRotatingFileHandler_528)
- [延伸错误信息一起载入log：](#log_618)
- [3.3 用.yaml文件配置Log](#33_yamlLog_645)
- [4、with...as...](#4withas_703)
- [5 主动发出报错raise](#5_raise_735)
- [六、类定义](#_745)
- [1、类里面一直存着参数](#1_758)
- [.
2、类里面函数如何调用](#
2_782)
- [.
3、类内函数套函数](#
3_814)
- [6.1 argparse模块](#61_argparse_839)
- [延伸一：关于如何从调用另一个.py文件中的类](#py_889)
- [七、装饰器](#_924)
- [1、闭包](#1_930)
- [2、初级装饰器](#2_948)
- [.
3、带装饰符@-装饰器](#
3_988)
- [.
4、带参数+装饰符的装饰器](#
4_1015)
- [.
5、传参数+装饰符@+多选项装饰器](#
5_1080)
- [.
6、传参数+装饰符@+类 装饰器](#
6__1112)




# 一、函数 +三个内建函数filter，map和reduce +  if

## 1、def/lambda

```
def myadd(a=1,b=100):
    result = 0
    i = a
    while i <= b:    # 默认值为1+2+3+……+100
        result += i    
        i += 1
    return result
```

跟R一样都可以用return返回

**lambda函数**

lambda表达式有什么好处？匿名函数，一般用来给filter，map这样的函数式编程服务，map(lambda x: x*2,[1,2,3,4,5])

或者就是一次性产品。类似于这种即用即扔的产品：(lambda x: x*2)(3)

或者作为回调函数，传递给某些应用，比如消息处理。

## 2、if

[python 中的三元表达式（三目运算符）](http://www.cnblogs.com/djangochina/archive/2013/06/12/3132438.html)

这个输出1，但没有什么意义，仅仅是一个例子。

而在python中的格式为
```
为真时的结果 if 判定条件 else 为假时的结果
```

还是上面的例子

```
1 if 5>3 else 0
```

### 如果if + for列表的方式同时使用：

```
conf = (11,2,3)
[i for i in conf if i > 2]
>>> [11, 3]
```

输出的结果是conf中大于2的元素，这里非常有意思的是，将if 和for 循环放在一个[]中就做掉了。

上面是返回conf中大于2的元素，下面是conf中大于2的序号。

```
conf = (11,2,3)
[i for i, c in enumerate(conf) if c > 2]
>>> [0, 2]
```

conf中大于2的序号。

## 3、filter

来源：[Python内建函数之——filter,map,reduce](http://blog.csdn.net/prince2270/article/details/4681299)

filter(bool_func,seq)：此函数的功能相当于过滤器。调用一个布尔函数bool_func来迭代遍历每个seq中的元素；返回一个使bool_seq返回值为true的元素的序列。

例如：
```
>>> filter(lambda x : x%2 == 0,[1,2,3,4,5])  
[2, 4]
```

filter内建函数的python实现：

```
def filter(bool_func,seq):  
    filtered_seq = []  
    for eachItem in seq:  
        if bool_func(eachItem):  
            filtered_seq.append(eachItem)  
    return filtered_seq
```

## 4、map

来源：[Python内建函数之——filter,map,reduce](http://blog.csdn.net/prince2270/article/details/4681299)

map(func,seq1[,seq2…])：将函数func作用于给定序列的每个元素，并用一个列表来提供返回值；如果func为None，func表现为身份函数，返回一个含有每个序列中元素集合的n个元组的列表。

例如：
```
>>> map(lambda x : None,[1,2,3,4])  
[None, None, None, None]  
>>> map(lambda x : x * 2,[1,2,3,4])  
[2, 4, 6, 8]  
>>> map(lambda x : x * 2,[1,2,3,4,[5,6,7]])  
[2, 4, 6, 8, [5, 6, 7, 5, 6, 7]]  
>>> map(lambda x : None,[1,2,3,4])  
[None, None, None, None]
```

map内建函数的python实现：

```
>>> def map(func,seq):  
    mapped_seq = []  
    for eachItem in seq:  
        mapped_seq.append(func(eachItem))  
    return mapped_seq
```

## 5、reduce

来源：[Python内建函数之——filter,map,reduce](http://blog.csdn.net/prince2270/article/details/4681299)

reduce(func,seq[,init])：func为二元函数，将func作用于seq序列的元素，每次携带一对（先前的结果以及下一个序列的元素），连续的将现有的结果和下一个值作用在获得的随后的结果上，最后减少我们的序列为一个单一的返回值：如果初始值init给定，第一个比较会是init和第一个序列元素而不是序列的头两个元素。

例如：
```
>>> reduce(lambda x,y : x + y,[1,2,3,4])  
10  
>>> reduce(lambda x,y : x + y,[1,2,3,4],10)  
20
```

reduce的python实现：

```
def reduce(bin_func,seq,initial=None):  
    lseq = list(seq)  
    if initial is None:  
        res = lseq.pop(0)  
    else:  
        res = initial  
    for eachItem in lseq:  
        res = bin_func(res,eachItem)  
    return res
```

# 二、for、while

博客：[Python for 循环语句](http://www.runoob.com/python/python-for-loop.html)

python中的for比R中的要广泛很多，R中的for (i in 1:3)循环的较多的是数值，python包括数值+文本列表。

1、字母当做循环

```
for letter in 'Python':     # 第一个实例
   print '当前字母 :', letter

当前字母 : P
当前字母 : y
当前字母 : t
当前字母 : h
当前字母 : o
当前字母 : n
```

2、列表当做循环

```
fruits = ['banana', 'apple',  'mango']
for fruit in fruits:        # 第二个实例
   print '当前字母 :', fruit
```

3、数字当做循环

```
for num in range(10,20):  # 迭代 10 到 20 之间的数字
   for i in range(2,num): # 根据因子迭代
      if num%i == 0:      # 确定第一个因子
         j=num/i          # 计算第二个因子
         print '%d 等于 %d * %d' % (num,i,j)
         break            # 跳出当前循环
   else:                  # 循环的 else 部分
      print num, '是一个质数'
```

4、for 中的 _ 下划线的应用

for 中出现了 _ 那么作用跟R里面的repeat一致，有复制的功能。

```
import numpy as np
l=[("f1",'g1'),("f2","g2")]
h=['go1','go2']

In [17]:{iter:np.zeros(2) for iter,_ in l }
Out[17]: {'f1': array([ 0.,  0.]), 'f2': array([ 0.,  0.])}

In [18]:[{iter:np.zeros(2) for iter,_ in l } for _ in h]
Out[18]: 
[{'f1': array([ 0.,  0.]), 'f2': array([ 0.,  0.])},
 {'f1': array([ 0.,  0.]), 'f2': array([ 0.,  0.])}]
```

比较小白，对python里面的生成器一开始看不懂，不过后来觉得跟R里面的lappy功能很像。

for iter,_ in l 提取了l=[(“f1”,‘g1’),(“f2”,“g2”)]两个的第一个数字，屏蔽了第二个g1/g2

然后f1 f2赋值给了前面的函数，函数内容是iter:np.zeros(2)，那么就是出现out[17]的结果。

for _ in h，完全就是repeat两次。
5、while - 重复执行

无限循环的时候，可以用ctrl + C结束。

while 语句用于循环执行程序，即在某条件下，循环执行某段程序，以处理需要重复处理的相同任务。其基本形式为：

（1）常规用法：重复执行
```
#!/usr/bin/python
count = 0
while (count < 9):   
   print 'The count is:', count    count = count + 1
print "Good bye!"

The count is: 0
The count is: 1
The count is: 2
The count is: 3
The count is: 4
The count is: 5
The count is: 6
The count is: 7
The count is: 8
Good bye!
```

for里面的重复执行，可以用 for _ in np.arrange(10):

（2）continue，break + while

while 语句时还有另外两个重要的命令 continue，break 来跳过循环，continue 用于跳过该次循环，break 则是用于退出循环，此外”判断条件”还可以是个常值，表示循环必定成立，具体用法如下：

```
# continue 和 break 用法i = 1
while i < 10:      i += 1    if i%2 > 0:     
# 非双数时跳过输出        
continue    print i         
# 输出双数2、4、6、8、10i = 1
while 1:            
# 循环条件为1必定成立   
 print i        
i += 1    
if i > 10:    
 # 当i大于10时跳出循环  
       break
```

（3）循环使用 else 语句

```
#!/usr/bin/python
count = 0
while count < 5:   
  print count, " is  less than 5"   count = count + 1
else:   
  print count, " is not less than 5"

>>>
0 is less than 5
1 is less than 5
2 is less than 5
3 is less than 5
4 is less than 5
5 is not less than 5
```

# 三、迭代器

两个列表同时迭代

```
nfc = ["Packers", "49ers"]
afc = ["Ravens", "Patriots"]
for teama, teamb in zip(nfc, afc):
     print teama + " vs. " + teamb
>>> Packers vs. Ravens
>>> 49ers vs. Patriots
```

带索引的列表迭代

```
teams = ["Packers", "49ers", "Ravens", "Patriots"]
for index, team in enumerate(teams):
    print index, team
>>> 0 Packers
>>> 1 49ers
>>> 2 Ravens
>>> 3 Patriots
```

已知一个列表，刷选出偶数列表方法：

```
numbers = [1,2,3,4,5,6]
even = []
for number in numbers:
    if number%2 == 0:
        even.append(number)
```

字典推导

```
teams = ["Packers", "49ers", "Ravens", "Patriots"]
print {key: value for value, key in enumerate(teams)}
>>> {'49ers': 1, 'Ravens': 2, 'Patriots': 3, 'Packers': 0}
```

**和collections库一样，还有一个库叫itertools**

```
from itertools import combinations
teams = ["Packers", "49ers", "Ravens", "Patriots"]
for game in combinations(teams, 2):
    print game
>>> ('Packers', '49ers')
>>> ('Packers', 'Ravens')
>>> ('Packers', 'Patriots')
>>> ('49ers', 'Ravens')
>>> ('49ers', 'Patriots')
>>> ('Ravens', 'Patriots')
```

# 四、****name****

参考来自：[百度知道](https://zhidao.baidu.com/question/189875539.html)

> 
****name****在当前文件中：****name****=****main****
****name****在别的文件中：****name****=def的名字


****name****是指示当前py文件调用方式的方法。如果它等于"****main****"就表示是直接执行，如果不是，则用来被别的文件调用，这个时候if就为False，那么它就不会执行最外层的代码了。

比如你有个Python文件里面

```
def XXXX():
#body
print "asdf"
```

这样的话，就算是别的地方导入这个文件，要调用这个XXXX函数，也会执行print “asdf”，因为他是最外层代码，或者叫做全局代码。但是往往我们希望只有我在执行这个文件的时候才运行一些代码，不是的话（也就是被调用的话）那就不执行这些代码，所以一般改为

```
def XXXX():
#body
if __name__="__main__":
print "asdf"
```

## **延伸：if **name** == ‘**main**’:**

本python文档，if **name** == ‘**main**’:以下的内容可以执行，

别的python文档， 调用的话，if **name** == ‘**main**’:以下内容不管了。

同时main()的意义是：python 如何判断应该哪个是主执行文件呢？应用场景是在互相调用python内函数的时候。

来源：[Python 初探](http://liuzxc.github.io/blog/python-beginner/)

.

.
# 五、防范报错

## 1、assert 断言

Python的assert是用来检查一个条件，如果它为真，就不做任何事。如果它为假，则会抛出AssertError并且包含错误信息。例如：

```
py> x = 23
py> assert x > 0, "x is not zero or negative"
py> assert x%2 == 0, "x is not an even number"
Traceback (most recent call last):
File "", line 1, in 
AssertionError: x is not an even number
```

类似R中的stopifnot

参考博客：[Python 中何时使用断言？](http://blog.jobbole.com/76285/)

.

## 2、try…except…

```
try:
    f = open('xxx')
except:
    print 'fail to open'
    exit(-1)
```

如果try中open不出来，那么就except返回相应的内容“fail to open”

```
try:
<语句>        #运行别的代码
except <名字>：
<语句>        #如果在try部份引发了'name'异常
except <名字>，<数据>:
<语句>        #如果引发了'name'异常，获得附加的数据
else:
<语句>        #如果没有异常发生
```

[参考](http://www.runoob.com/python/python-exceptions.html)

看一个[案例](https://eastlakeside.gitbooks.io/interpy-zh/content/exception/try_else_clause.html)：

```
try:
    print('I am sure no exception is going to occur!')
except Exception:
    print('exception')
else:
    # 这里的代码只会在try语句里没有触发异常时运行,
    # 但是这里的异常将 *不会* 被捕获
    print('This would only run if no exception occurs. And an error here '
          'would NOT be caught.')
finally:
    print('This would be printed in every case.')

# Output: I am sure no exception is going to occur!
# This would only run if no exception occurs.
# This would be printed in every case.
```

### 报错并提示异常信息：

来源：[Python中获取异常（Exception）信息](http://www.cnblogs.com/klchang/p/4635040.html)

1、str(e)

返回字符串类型，只给出异常信息，不包括异常信息的类型，如1/0的异常信息

‘integer division or modulo by zero’

2、repr(e)

给出较全的异常信息，包括异常信息的类型，如1/0的异常信息

“ZeroDivisionError(‘integer division or modulo by zero’,)”

3、e.message

获得的信息同str(e)

4、采用traceback模块

需要导入traceback模块，此时获取的信息最全，与python命令行运行程序出现错误信息一致。使用traceback.print_exc()打印异常信息到标准错误，就像没有获取一样，或者使用traceback.format_exc()将同样的输出获取为字符串。你可以向这些函数传递各种各样的参数来限制输出，或者重新打印到像文件类型的对象。

```
try:
    1/0
except Exception, e:
    print 'str(Exception):\t', str(Exception)
    print 'str(e):\t\t', str(e)
    print 'repr(e):\t', repr(e)
    print 'e.message:\t', e.message
    print 'traceback.print_exc():'; traceback.print_exc()
    print 'traceback.format_exc():\n%s' % traceback.format_exc()
```

或者：

```
def this_fails():
    x = 1/0

try:
    this_fails()
except :
    print('Handling run-time error:')
    raise
print(1)
```

# 3、报错日志记录：Logger.exception

以ERROR级别记录日志消息，异常跟踪信息将被自动添加到日志消息里。Logger.exception通过用在异常处理块中，如：

来源：[Python模块学习：logging 日志记录](http://python.jobbole.com/81521/)

```
import logging
logging.basicConfig(filename = os.path.join(os.getcwd(), 'log.txt'), level = logging.DEBUG)
log = logging.getLogger('root')
try:
    a===b
except:
    log.exception('exception') #异常信息被自动添加到日志消息中
```

就会在指定目录里面，载入到Log.txt之中

来看看在Log中显示如下：

```
ERROR:root:exception
Traceback (most recent call last):
  File "test.py", line 12, in <module>
    a==b
NameError: name 'a' is not defined
```

所以，在 log.exception()可以简单定义一些关键词来帮助定位问题所在。

## 3.1 logger - basicConfig 常规存储

来源：[python logging模块打印log到指定文件](https://blog.csdn.net/niedongri/article/details/79304770)

```
import logging
import unittest
class lgtest(unittest.TestCase):
    logging.basicConfig(filename='../LOG/'+__name__+'.log',format='[%(asctime)s-%(filename)s-%(levelname)s:%(message)s]', level = logging.DEBUG,filemode='a',datefmt='%Y-%m-%d%I:%M:%S %p')

    def test(self):

        logging.error("这是一条error信息的打印")
        logging.info("这是一条info信息的打印")
        logging.warning("这是一条warn信息的打印")
        logging.debug("这是一条debug信息的打印")
if __name__=='__main__':
    unittest.main()
```
- Filename：指定路径的文件。这里使用了+—name—+是将log命名为当前py的文件名
- Format：设置log的显示格式（即在文档中看到的格式）。分别是时间+当前文件名+log输出级别+输出的信息
- Level：输出的log级别，优先级比设置的级别低的将不会被输出保存到log文档中
- Filemode： log打开模式
- a：代表每次运行程序都继续写log。即不覆盖之前保存的log信息。
- w：代表每次运行程序都重新写log。即覆盖之前保存的log信息


## 3.2 常规：logging + handlers + TimedRotatingFileHandler

参考：[Python + logging 输出到屏幕，将log日志写入文件](https://www.cnblogs.com/nancyzhu/p/8551506.html)

参考：[python中logging模块下篇](https://zhuanlan.zhihu.com/p/38782314)

```
# 信息写入log
import logging
from logging import handlers
class Logger(object):
    level_relations = {
        'debug':logging.DEBUG,
        'info':logging.INFO,
        'warning':logging.WARNING,
        'error':logging.ERROR,
        'crit':logging.CRITICAL
    }#日志级别关系映射

    def __init__(self,filename,level='info',when='D',backCount=15,\
                 fmt='%(asctime)s - %(levelname)s: %(message)s'):
        self.logger = logging.getLogger(filename)   # 创建log文件
        format_str = logging.Formatter(fmt)#设置日志格式
        self.logger.setLevel(self.level_relations.get(level))#设置日志级别
        if not self.logger.handlers:
            sh = logging.StreamHandler()  # 初始化 1 ，往屏幕上输出
            sh.setFormatter(format_str)   #  设置屏幕上显示的格式
            th = handlers.TimedRotatingFileHandler(filename=filename,when=when,\
                                                   backupCount=backCount,encoding='utf-8')
            
            th.setFormatter(format_str)#设置文件里写入的格式
            self.logger.addHandler(sh) #把对象加到logger里
            self.logger.addHandler(th)
if __name__ == '__main__':
    log = Logger('all.log',level='debug')
    log.logger.debug('debug')
    log.logger.info('info')
    log.logger.warning('警告')
    log.logger.error('报错'', exc_info=True)
    log.logger.critical('严重')
    Logger('error.log', level='error').logger.error('error')
```

屏幕上的结果如下：

```
2018-03-13 21:06:46,092 - DEBUG: debug
2018-03-13 21:06:46,092 - INFO: info
2018-03-13 21:06:46,092- WARNING: 警告
2018-03-13 21:06:46,099 - ERROR: 报错
2018-03-13 21:06:46,099- CRITICAL: 严重
2018-03-13 21:06:46,100 - ERROR: error
```

由于when=D，新生成的文件名上会带上时间，如下所示。
![在这里插入图片描述](https://images2018.cnblogs.com/blog/412654/201803/412654-20180313210806712-650581417.png)
其中：
- 
level_relations ，代表日志级别，默认等级是WARNING，这意味着仅仅这个等级及以上的才会反馈信息，除非logging模块被用来做其它事情。
- 日志级别： debug < info < warning < error < critical

- 
TimedRotatingFileHandler - 按照时间自动分割日志文件

- 
interval是时间间隔

- 
when是间隔的时间单位，单位有以下几种：
- S 秒
- M 分
- H 小时、
- D 天、
- W 每星期（interval==0时代表星期一）
- midnight 每天凌晨

- 
backupCount 是保留日志个数。默认的0是不会自动删除掉日志。若设10，则在文件的创建过程中库会判断是否有超过这个10，若超过，则会从最先创建的开始删除。

- 
exc_info，True，代表把错误信息也保存下来，默认是False，不保存


同时，

你会发现如果不设置`if not self.logger.handlers:`，那么会出现，重复写日志问题。问题出在，他会不断往`log.logger.handlers`添加handlers，上限是三个，就会出现：

第一条记录写一次，第二条记录写两次，第三条记录写三次。

(解决方案来自：[python logging 重复写日志问题](https://blog.csdn.net/huilan_same/article/details/51858817))

原理在于，如果`self.logger.handlers`已经有类了，那么就不要额外添加了。
```
# 正常的self.logger.handlers长什么样子：
[<logging.StreamHandler at 0x12eba1e3128>,
 <logging.handlers.TimedRotatingFileHandler at 0x12eba1e3160>]

# 不正常的self.logger.handlers长什么样子：
[<logging.StreamHandler at 0x12eba1e3128>,
 <logging.handlers.TimedRotatingFileHandler at 0x12eba1e3160>,
 [<logging.StreamHandler at 0x12eba1e3128>,
 <logging.handlers.TimedRotatingFileHandler at 0x12eba1e3160>,]
```

#### 延伸错误信息一起载入log：

```
import logging
logging.basicConfig(level=logging.INFO,
                   format='%(asctime)s  %(message)s',
                   datefmt='%a, %d %b %Y %H:%M:%S +0000',
                   filename='my.log')

logging.info('this is a info')
try:
    do
except Exception:
    logging.error('There are something wrong', exc_info=True)

logging.info('continue')


# 报错信息暴露
Sun, 01 Jul 2018 21:10:53 +0000  There are something wrong
Traceback (most recent call last):
  File "learn.py", line 9, in <module>
    do
NameError: name 'do' is not defined
Sun, 01 Jul 2018 21:10:53 +0000  continue
```

## 3.3 用.yaml文件配置Log

[python中logging模块下篇](https://zhuanlan.zhihu.com/p/38782314)

我们不仅可以通过python代码进行logging配置，而且可以通过写一个yaml文件进行配置，每次需要用logging时只要调用这个文件就配置完成。

config.yaml文件内容如下

```
version: 1
formatters:
  simple:
    format: "%(message)s"
  more:
    format: "%(asctime)s - %(levelname)s - %(message)s"
handlers:
  console:
    class : logging.StreamHandler
    formatter: simple
    level: INFO
    stream: ext://sys.stdout
  file:
    class: logging.FileHandler
    formatter: more
    level: DEBUG
    filename: debug.log
loggers:
  mainlogger:
    level: DEBUG
    handlers: [console, file]
  root:
    level: DEBUG
    handlers: [console]
```

main.py文件中编写代码如下

```
import logging
import logging.config
import yaml
import a

with open('config.yaml', 'r', encoding='utf-8') as f:
    config = yaml.load(f)
    logging.config.dictConfig(config)

logging.info('main file log')
a.run()
```

## 4、with…as…

那么with和as也是一种防止报错的防范方式，

当python执行这一句时，会调用__enter__函数，然后把该函数return的值传给as后指定的变量。

之后，python会执行下面do something的语句块。最后不论在该语句块出现了什么异常，都会在离开时执行__exit__。
```
ith open("x.txt") as f:
    data = f.read()

with open("x.txt") as f1, open('xxx.txt') as f2:
    do something with f1,f2
```

那么try和with也可以合起：

```
try:
    with open( "a.txt" ) as f :
        do something
except xxxError:
    do something about exception
```

延伸：报错提示如何写ValueError（）

```
if pretrained_model not in self._models:
    raise ValueError(
        'The n_class needs to be supplied as an argument.')
```

## 5 主动发出报错raise

参考url：[https://blog.csdn.net/skullfang/article/details/78820541](https://blog.csdn.net/skullfang/article/details/78820541)

```
raise RuntimeError('testError')
```

.

.

# 六、类定义

参考于:[Python的hasattr() getattr() setattr() 函数使用方法详解](http://www.cnblogs.com/cenyu/p/5713686.html)

```
class <类名>:
    <语句>
```

类需注意：
- 类适合存函数，不用非要return
- 在类中定义的函数只有一点不同，就是第一个参数永远是实例变量self，并且，调用时，不用传递该参数。

## 1、类里面一直存着参数

__init__存入

```
class save_def(object):

    def __init__(self, name, score):
        self.name = name
        self.score = score
```

这样save_def里面就会一直存着name、score函数。

```
initialization = save_def（name,score）
```

初始化的方式跟函数一样，直接传入。

同时，[initialization.name](http://initialization.name)，initialization.score 也相当于initialization的属性。

不过，如果你觉得还需要额外加入一些临时属性的话，可以直接赋值：
```
initialization.other = "other"
```

## .2、类里面函数如何调用

类内的参数前面加self.para

```
class save_def(object):
    def __init__(self, name, score):
        self.name = name
        self.score = score
        
	def function1(self,dataframe):
	temp_data = type(dataframe[self.name])
	return temp_data
```

如果类里面包着函数，而且函数里面包着属性，那么可以直接用：

```
initialization = save_def（name,score）
 initialization.function1(dataframe)
```

如果是函数的话，则为：

```
>>>def function1(name,dataframe):
	temp_data = type(dataframe[name])
	return temp_data 

>>>function1(name,dataframe)
```

不同的地方在于类里面，可以直接用类内存着的函数。

## .3、类内函数套函数

类内函数调用类内的函数，前面需要加self.

```
class save_def(object):
    def __init__(self, name, score):
        self.name = name
        self.score = score
        
	def function1(self,dataframe):
	temp_data = type(dataframe[self.name])
	return temp_data 

	def function2(self):
		temp_data2 = self.function1(dataframe.ix[self.score>10,:])
```

self.function1就是一种类内调用函数的方式。从这一案例可以看出：

类内调用函数+参数，都需要用self.

.
# 6.1 argparse模块

argparse模块是把python代码变成可以命令行模式的一种方案，然后就可以写进.bat里面一键运行都没有问题。

```
import argparse
import ast
parser = argparse.ArgumentParser()
parser.add_argument("-t", "--t2s",type=ast.literal_eval,default = False, help=" True False")
parser.add_argument("-f", "--filename",default = '7w.csv', help="data file path and the format must utf-8 or gbk")
args = parser.parse_args()

if args.t2s:
    print('yes')
```

其中`parser.add_argument`是定义的核心环节，其中`-t`代表命令行时候需要键入的内容；`--t2s`代表在python中调用的内容（以`args.t2s`），`type`代表格式，`default`代表默认值，`help`是一些备注。

此时只要键入：`python xx.py -t True -f 7w.csv`,即可开始运行。

其中`7w.csv`是字符型，不能写成`'7w.csv'`

**注意：**

如果键入true/false，判断类的，需要用ast这类，不然的话，怎么输入都会变为True（虽然我们指定了其类型为bool，但无论我们在命令行中给这个参数传入什么值（如True，False等），my_bool的值总是为True。）。

解决方案参考：[使用Python中的argparse从命令行接收boolean类型的参数](https://blog.csdn.net/Yaokai_AssultMaster/article/details/77928629)
**触发器：**

其中，关于True/False还是一种方式：

```
parser.add_argument(
        "--show-mask-heatmaps",
        dest="show_mask_heatmaps",
        help="Show a heatmap probability for the top masks-per-dim masks",
        action="store_true",
    )
```

store_true 是指带触发action时为真，不触发则为假，2L说的代码去掉default初始化，其功能也不会变化

```
parser.add_argument('-c', action='store_true')
#python test.py -c         => c是true（触发）
#python test.py             => c是false（无触发）
```

.

## 延伸一：关于如何从调用另一个.py文件中的类

因为，Python里面全局变量所谓的全局是针对在一个文件里面的函数而言的，跨文件就等于定义在不同的包里面，也就相当于是使用了不同的命名空间。（来源：[PYTHON全局变量在模块中的无法引用？](http://forum.ubuntu.org.cn/viewtopic.php?t=336662)）

笔者在调用另一个.py文件中的类的时候，发生大面积的报错：

```
globel name 'xxx' is not defined
```

无奈只能写在一个文档中。

一般来说，调用的方式有两种：import 和 from…import…：

```
import datetime
print(datetime.datetime.now())

from datetime import datetime
print(datetime.now())
```

通俗的说：

```
from import : 从车里把矿泉水拿出来，给我
import : 把车给我
```

.

# 七、装饰器

参考于：[深入浅出Python装饰器](http://ajucs.com/2016/01/14/dive-into-python-decorator.html)、[Python装饰器学习（九步入门）](http://www.cnblogs.com/rhcad/archive/2011/12/21/2295507.html)、[Python 的闭包和装饰器](https://segmentfault.com/a/1190000004461404)

> 
其中笔者认为，类class适合打包函数，装饰器@适合包装函数，并不是包装参数。 同时，装饰器适合，函数之间有一定递进、级联关系，比较适合。


## 1、闭包

装饰器相当于升级版的闭包，一个闭包就是你调用了一个函数A，这个函数A返回了一个函数B给你。这个返回的函数B就叫做闭包。你在调用函数A的时候传递的参数就是自由变量。

```
def func(name):
    def inner_func(age):
        print 'name:', name, 'age:', age
    return inner_func

bb = func('the5fire')
bb(26)  # >>> name: the5fire age: 26
```

这里面调用func的时候就产生了一个闭包——inner_func,并且该闭包持有自由变量——name，因此这也意味着，当函数func的生命周期结束之后，name这个变量依然存在，因为它被闭包引用了，所以不会被回收。

.

## 2、初级装饰器

函数传入、函数传出,装饰器的本质就是函数闭包，或者说利用了函数闭包的特性。

```
def outer(function):
    def inner():
        print("执行function之前可以进行额外操作")
        result = function()
        print("执行function之后还可以进行额外操作")
        result *= 2        # 对function的返回值本身进行额外操作
        return result      # 返回‘加强’后的结果
    return inner

def wait_for_deco():
    return 1024

decorated = outer(wait_for_deco)
print decorated()
# 输出
2048
```

上例就是纯手工实现的一个最简单的装饰器。装饰器函数outer并没有修改被装饰函数wait_for_deco，但我们调用被装饰后的decorated函数闭包却能够得到原函数的加强版结果，还能进行额外的操作。

**超级简单的案例一则：**

```
def function1(function):
	return function() + 1
# function作为参数传入，且这个参数名称是任意的，跟普通参数一致，需要对齐，函数也要写function

def function2():
	return 2
#这样的装饰器，一般不传入参数

# 使用
>>> function1(function2)
>>> 3
# 其中，不能写成function1(function2())
```

## .3、带装饰符@-装饰器

实际上装饰符@就仅仅是帮我们自动地把返回的闭包函数名字替换为原函数的名字。

```
def outer(function):
    def inner():
        print("执行function之前可以进行额外操作")
        result = function()
        print("执行function之后还可以进行额外操作")
        result *= 2        # 对function的返回值本身进行额外操作
        return result      # 返回‘加强’后的结果
    return inner

@outer
def wait_for_deco():
    return 1024

print wait_for_deco()
# 输出
2048
```

## .4、带参数+装饰符的装饰器

```
# 例如为下面的函数写一个装饰器，应该在内部的wapper中按原样传递参数
def decorator(func):
    def wrapper(x, y)
      ret = func(x, y)    # 原函数的返回值
      return ret*2        # 原函数的结果“加强”后再返回
    return wrapper

@decorator
def wait_for_deco(x, y):
  return x + y

print(wait_for_deco(1, 2))

# 输出
6
```

可调节的多参数

按照上面这种写法虽然可以传参了但有个缺陷，参数个数不确定的函数就没法使用这个装饰器了。

```
def decorator(func):
    def wrapper(*args, **kwargs)
      ret = func(*args, **kwargs)
      return ret*2
    return wrapper

@decorator
def wait_for_deco_a(x, y):
  return x + y

@decorator
def wait_for_deco_b(x, y, z):
  return x + y + z

print(wait_for_deco_a(1, 2))
6
print(wait_for_deco_b(1, 2, 3))
12
```

**超级简单案例一则：**

```
def function2(function):
	# 因为要传入参数，所以下面的def 就相当于在写了一次_function2，但也不是简单的加个def，其中_代表，专属于函数内
    def _function2(b):
	    # 并不能写出：function2(function)
        a = function(b)
        return a + 1
    return _function2
# 返回的是函数本身

@function2
def function1(b):
    return b + 1

function2(function1)
```

## .5、传参数+装饰符@+多选项装饰器

来源文章：[Python装饰器02](http://mp.weixin.qq.com/s?__biz=MzI0MDcyMzg5NQ==&mid=2247483726&idx=1&sn=aa4638d82f3635eb096df7406d16795d&chksm=e9173396de60ba80e6a3608966e3674ac6af40e37d8d0ee8d499e8c47b2a2dcc12aad9cade62&mpshare=1&scene=1&srcid=0428MlOKVY5gYu4vXxKaw9kU#rd)

```
def mydec(is_print):
    def mydec_f(func):
        def mydec_a(*args, **kwargs):
            r = func(*args, **kwargs)
            if is_print:
                print("结果是:{}".format(r))
            else:
                print("不打印结果")
        return mydec_a
    return mydec_f
 
@mydec(True)
def mysum(a, b): 
    c = a + b 
    return c
@mydec(False)
def mysum2(a, b, c): 
    d = a + b + c 
    return d
 
mysum(1, 2)
mysum2(1, 2, 3)


>>>结果是:3
```

## .6、传参数+装饰符@+类 装饰器

来源文章：[Python装饰器02](http://mp.weixin.qq.com/s?__biz=MzI0MDcyMzg5NQ==&mid=2247483726&idx=1&sn=aa4638d82f3635eb096df7406d16795d&chksm=e9173396de60ba80e6a3608966e3674ac6af40e37d8d0ee8d499e8c47b2a2dcc12aad9cade62&mpshare=1&scene=1&srcid=0428MlOKVY5gYu4vXxKaw9kU#rd)

```
class mycls():
    def __init__(self):
        print("类初始化了")
              
    @staticmethod
    def test1():
        print("我是mycls.test1")
                     
    @staticmethod
    def test2():
        print("我是mycls.test2")
 
  
# 装饰器
def mydec(cls):
    def mydec_f(func):
        def mydec_a(*args, **kwargs):
            cls.test1()
            r = func(*args, **kwargs)
            print("结果是:{}".format(r))
            cls.test2()
        return mydec_a
    return mydec_f
 
@mydec(mycls)
def mysum(a, b): 
    c = a + b 
    return c
@mydec(mycls)
def mysum2(a, b, c): 
    d = a + b + c 
    return d
 
mysum(1, 2)
mysum2(1, 2, 3)
```

结果：

```
我是mycls.test1
结果是:3
我是mycls.test2
我是mycls.test1
结果是:6
我是mycls.test2
```
















