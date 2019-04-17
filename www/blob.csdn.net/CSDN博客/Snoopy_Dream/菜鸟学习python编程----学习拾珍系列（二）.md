# 菜鸟学习python编程----学习拾珍系列（二） - Snoopy_Dream - CSDN博客





2018年08月13日 10:42:51[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：46








1.对于python这种语言，遇到问题的第一瞬间，应该是查找现有方法，而不是自己编写。

比如字符串的strip，replace方法等

2.pprint或者print(  ,sep='\t',)更好的输出

3.可选形参

    def a(b,c=200) c为可选参数，200是默认

4.寻找在d1中出现而d2没有出现的key，创建一个新的dict，然后 for 和 if 旧的两个的比较

```python
def subtract(d1,d2):
    res =dict()
    for key in d1:
        if key not in d2:
            res[key] = None
    return res
```

5.**os下常用函数总结**
- os.walk(top, topdown=True) topdown为True自顶向下遍历，默认为True，一般不管，迭代器。

for root,dirs,files in os.walk(path):

比如：  root: /home/ygx/Faster_RCNN_for_DOTA-master

              dirs: ['lib', 'faster_rcnn', 'experiments']

              files: ['init.bat', 'LICENSE', 'init.sh', '.gitignore', 'README.md', 'setup.py']

访问文件的时候，需要加入 os.path.join(root,files)
- os.**getcwd()**current working directory
- os.path.abspath('  ')   获取某文件的绝对路径
- os.**listdir() ** 返回目录中**文件的列表**。没有路径
- os.path.**exists(**`path`**) **  检查文件夹是否存在
- `os.**makedirs**(path) `
- os.path**.****isdir(**`path`** )**检查是否是路径
- `os.path.**splitext**('`/home/ygx/Faster_RCNN_for_DOTA-master/init.bat`')`

        >>> 返回尾缀名 /home/ygx/Faster_RCNN_for_DOTA-master/init  + .bat
- 
os.path.**basename**("/usr/local/python3/bin/python3/3.txt")

>>> '3.txt'

- 
 os.path.**dirname**("/usr/local/python3/bin/python3/3.txt")


        >>> '/usr/local/python3/bin/python3'

6.**异常处理**

   try except finally  raise 和 assert
- **  raise：**当程序出现错误，python会自动引发异常，也可以通过raise显式地引发异常。一旦执行了raise语句，raise后面的语句将不能执行
- **try  ...  except  ...   finally **

    一般用except Exception as e  其中，Exception表示常规错误的基类，类似下面的直接替代ZeroDivision是一样的。

```python
a=10

b=0

try:

    c=a/b

    print c

except ZeroDivision as e:

    print str(e)
else: #没有发生异常时输出
    print "no error"
finally: #无论程序是否发生异常，在结束前都要执行，没有except，else，也经常单独与try使用
    print "always excute"

print "done"
```
- ** assert**

    经常用于**逻辑判断**,等同于if 判断加一个 raise 提示，比如深度学习中，**短边必须小于长边**，某些参数必须是2的倍数，这种的，他可以让你更快地找到**最开始出错**的地方。如果不满足assert中条件直接打断在此处。

```python
import sys 
reload(sys) # Python2.5 初始化后删除了 sys.setdefaultencoding 方法，我们需要重新载入 
sys.setdefaultencoding('utf-8') 

short_size = 600
long_size =500
assert short_size<long_size,'短边不能大于长边'
```

7.self，other, 接受两个对象作为实参

```python
def is_after(self,other):
    return self.time_to_int() > other.time_to_int()
>>>end.is_after(start)
True
```

8.__int__ __str__
|**__init__(self,...)**|初始化对象，在创建新对象时调用|
|----|----|
|**__str__(self)**|在使用print语句时被调用|

```python
def __int__(self): #一般的做法是初始化对象所有属性 self.minute=minute

def __str__(self):

    return '%.2d:%.2d%.2d' %(self.hour，self.minute，self.second)
```

>>>time =Time(9,45)

>>>print time 09:45:00

9.操作符重载与多态

isinstance判断是不是这个对象的实例，然后做选择，更好的制作接口，满足更多的属性要求

```
def __add__(self,other):
    if isinstance(other，Time):
        return self.add_time(other)
    else:
        return self.increment(other)
def add_time(self,other):
    seconds = self.time_to_int()+other.time_to_int()
    return seconds
def increment(self,other):
    seconds+=self.time_to_int()
    return int_to_time(seconds)
```

多态：可处理多类型的函数 sum([t1,t2,t3])也是可以的，因为time类里有自定义的__add__方法。





