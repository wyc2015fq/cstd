# 理解（os等常用模块，列表解析，字典解析和集合解析） - wydbyxr的博客 - CSDN博客
2017年02月25日 16:13:21[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：262
# 前言
Python 3 带有一个模块叫做 os，代表 “操作系统(operating system)。”Python 尽最大的努力在所有支持的操作系统上提供一个统一的API， 这样你就可以在保证程序能够在任何的计算机上运行的同时尽量少的包含平台特定的代码。 
- 将examples目录加入到导入搜索路径中
- 将当前工作目录切换到examples目录
## OS模块
### os.getcwd() 和os.chdir()
os 模块提供了两个函数处理当前工作目录，如下：
```python
>>> import os ①
>>> print(os.getcwd()) ②
C:\Python31
>>> os.chdir('/Users/pilgrim/diveintopython3/examples') ③
>>> print(os.getcwd()) ④
```
- 
使用os.getcwd() 函数获得当前工作目录。当你运行一个图形化的Python Shell 时，当前工作目录默认将是Python Shell的可执行文件所在的目录。
- 
使用os.chdir()函数改变当前工作目录，即使在Windows上，我也总是使用Linux风格的路径(正斜杠，没有盘符)。这就是Python 尝试隐藏操作系统差异的一个地方。
- 
win下 / ，Linux下 \ 
路径名添加一个额外的斜杠。由于我在Windows 上写这个例子， 这个斜杠是一个反斜杠而不是正斜杠。如果你在Linux 或者Mac OS X上重现这个例子， 你将会看见正斜杠.
### os.path.join()和os.path.expanduser()
os.path.join() 函数从一个或多个路径片段中构造一个路径名。 
os.path.expanduser() 用来将包含~符号（表示当前用户Home目录）的路径扩展为完整的路径。
```
>>> import os
>>> print(os.path.join('/Users/pilgrim/diveintopython3/examples/', 'humansize.py')) ①
/Users/pilgrim/diveintopython3/examples/humansize.py
>>> print(os.path.join('/Users/pilgrim/diveintopython3/examples', 'humansize.py')) ②
/Users/pilgrim/diveintopython3/examples\humansize.py
>>> print(os.path.expanduser('~')) ③
c:\Users\pilgrim
>>> print(os.path.join(os.path.expanduser('~'), 'diveintopython3', 'examples', 'humansize.py')) ④
c:\Users\pilgrim\diveintopython3\examples\humansize.py
```
### split
- split 函数分割一个完整路径并返回目录和文件名。
- os.path.split() 函数返回元组中的第一个元素，文件所在的目录； 
元组中的第二个元素，文件名。 
- os.path 也包含os.path.splitext() 函数，它分割一个文件名并返回短文件名和扩展名。可以使用同样的技术将它们的值赋值给不同的变量。
```python
>>> pathname = '/Users/pilgrim/diveintopython3/examples/humansize.py'
>>> os.path.split(pathname) ①
('/Users/pilgrim/diveintopython3/examples', 'humansize.py')
>>> (dirname, filename) = os.path.split(pathname) ②
>>> dirname ③
'/Users/pilgrim/diveintopython3/examples'
>>> filename ④
'humansize.py'
>>> (shortname, extension) = os.path.splitext(filename) ⑤
>>> shortname
'humansize'
>>> extension
'.py'
```
## glob 模块
它是Python标准库中的另一个工具，它可以通过编程的方法获得一个目录的内容，并且它使用熟悉的命令行下的通配符。 
### .glob
- glob 模块接受一个通配符并返回所有匹配的文件和目录的路径。在这个例子中，通配符是一个目录名加上 “*.xml”， 它匹配examples子目录下的所有.xml 文件。
- 现在我们将当前工作目录切换到examples 目录。 os.chdir() 可以接受相对路径.
- 在glob模式中你可以使用多个通配符。这个例子在当前工作目录中找出所有扩展名为.py并且在文件名中包含单词test 的文件。 
```
>>> os.chdir('/Users/pilgrim/diveintopython3/')
>>> import glob
>>> glob.glob('examples/*.xml') ①
['examples\\feed-broken.xml',
'examples\\feed-ns0.xml',
'examples\\feed.xml']
>>> os.chdir('examples/') ②
>>> glob.glob('*test*.py') ③
['alphameticstest.py',
'pluraltest1.py',
'pluraltest2.py',
'pluraltest3.py',
'pluraltest4.py',
'pluraltest5.py',
'pluraltest6.py',
'romantest1.py',
'romantest10.py',
'romantest2.py',
'romantest3.py',
'romantest4.py',
'romantest5.py',
'romantest6.py',
'romantest7.py',
'romantest8.py',
'romantest9.py']
```
### time 模块
```python
>>> import os
>>> print(os.getcwd()) ①
c:\Users\pilgrim\diveintopython3\examples
>>> metadata = os.stat('feed.xml') ②
>>> metadata.st_mtime ③
1247520344.9537716
>>> import time ④
>>> time.localtime(metadata.st_mtime) ⑤
time.struct_time(tm_year=2009, tm_mon=7, tm_mday=13, tm_hour=17,
tm_min=25, tm_sec=44, tm_wday=0, tm_yday=194, tm_isdst=1)
```
### os.stat()
每一个现代文件系统都对文件存储了元信息: 创建时间，最后修改时间，文件大小等等。Python 单独提供了一个的API 用于访问这些元信息。 你不需要打开文件。知道文件名就足够了。  
调用os.stat() 函数返回一个包含多种文件元信息的对象。
```python
>>> import os
>>> print(os.getcwd()) ①
c:\Users\pilgrim\diveintopython3\examples
>>> metadata = os.stat('feed.xml') ②
>>> metadata.st_mtime ③
1247520344.9537716
>>> import time ④
>>> time.localtime(metadata.st_mtime) ⑤
time.struct_time(tm_year=2009, tm_mon=7, tm_mday=13, tm_hour=17,
tm_min=25, tm_sec=44, tm_wday=0, tm_yday=194, tm_isdst=1)
```
### os.path.realpath()构造绝对路径
当你希望构造一个从根目录开始或者是包含盘符的绝对路径时，你就需要用到os.path.realpath()函数了。
```
>>> import os
>>> print(os.getcwd())
c:\Users\pilgrim\diveintopython3\examples
>>> print(os.path.realpath('feed.xml'))
c:\Users\pilgrim\diveintopython3\examples\feed.xml
```
# 列表解析
列表解析提供了一种紧凑的方式，实现了通过对列表中每一个元素应用一个函数的方法来将一个列表映射到另一个列表. 
- 你可以在列表解析中使用任何的Python表达式， 包括os 模块中用于操作文件和目录的函数。列表解析并不限制表达式的复杂程度。
```
>>> import os, glob
>>> glob.glob('*.xml') ①
['feed-broken.xml', 'feed-ns0.xml', 'feed.xml']
>>> [os.path.realpath(f) for f in glob.glob('*.xml')] ②
['c:\\Users\\pilgrim\\diveintopython3\\examples\\feed-broken.xml',
'c:\\Users\\pilgrim\\diveintopython3\\examples\\feed-ns0.xml',
'c:\\Users\\pilgrim\\diveintopython3\\examples\\feed.xml']
```
这个列表解析在当前目录查找所有.py 文件，而 if 表达式通过测试文件大小是否大于6000字节对列表进行过滤。有6个符合条件的文件，所以这个列表解析返回包含六个文件名的列表。 
```python
>>> import os, glob
>>> [f for f in glob.glob('*.py') if os.stat(f).st_size > 6000] ①
['pluraltest6.py',
'romantest10.py',
'romantest6.py',
'romantest7.py',
'romantest8.py',
'romantest9.py']
```
更复杂的：
```python
>>> import os, glob
>>> [(os.stat(f).st_size, os.path.realpath(f)) for f in glob.glob('*.xml')] ①
[(3074, 'c:\\Users\\pilgrim\\diveintopython3\\examples\\feed-broken.xml'),
(3386, 'c:\\Users\\pilgrim\\diveintopython3\\examples\\feed-ns0.xml'),
(3070, 'c:\\Users\\pilgrim\\diveintopython3\\examples\\feed.xml')]
>>> import humansize
>>> [(humansize.approximate_size(os.stat(f).st_size), f) for f in glob.glob('*.xml')] ②
[('3.0 KiB', 'feed-broken.xml'),
('3.3 KiB', 'feed-ns0.xml'),
('3.0 KiB', 'feed.xml')]
```
# 字典解析
字典解析和列表解析类似，只不过它生成字典而不是列表。
```
>>> import os, glob
>>> metadata = [(f, os.stat(f)) for f in glob.glob('*test*.py')] ①
>>> metadata[0] ②
('alphameticstest.py', nt.stat_result(st_mode=33206, st_ino=0, st_dev=0,
st_nlink=0, st_uid=0, st_gid=0, st_size=2509, st_atime=1247520344,
st_mtime=1247520344, st_ctime=1247520344))
>>> metadata_dict = {f:os.stat(f) for f in glob.glob('*test*.py')} ③
>>> type(metadata_dict) ④
<class 'dict'>
>>> list(metadata_dict.keys()) ⑤
['romantest8.py', 'pluraltest1.py', 'pluraltest2.py', 'pluraltest5.py',
'pluraltest6.py', 'romantest7.py', 'romantest10.py', 'romantest4.py',
'romantest9.py', 'pluraltest3.py', 'romantest1.py', 'romantest2.py',
'romantest3.py', 'romantest5.py', 'romantest6.py', 'alphameticstest.py',
'pluraltest4.py']
>>> metadata_dict['alphameticstest.py'].st_size ⑥
2509
```
- 这不是字典解析; 
而是列表解析。它找到所有名称中包含test的.py文件，然后构造包含文件名和文件元信息(通过调用os.stat()函数得到)的元组。
- 结果列表的每一个元素是元组。
- 这是一个字典解析。 除了两点以外，它的语法同列表解析很类似。 
首先，**它被花括号而不是方括号包围**;  
第二，**对于每一个元素它包含由 **冒号** 分隔的两个表达式，而不是列表解析的一个**。 
冒号前的表达式(在这个例子中是f)是字典的键;冒号后面的表达式(在这个例子中是os.stat(f))是值。 
> 
换行:  前一行\    后一行…
```
>> humansize_dict = {os.path.splitext(f)[0]:humansize.approximate_size(meta.st_size) \ 
...  for f, meta in metadata_dict.items() if meta.st_size > 6000}
```
小技巧: 交换字典的键和值。 
```
>>> a_dict = {'a': 1, 'b': 2, 'c': 3}
>>> {value:key for key, value in a_dict.items()}
{1: 'a', 2: 'b', 3: 'c'}
```
## 集合解析
它和字典解析的非常相似，唯一的不同是集合只有值而没有键:值对。 
```bash
>>> a_set = set(range(10))
>>> a_set
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
>>> {x ** 2 for x in a_set} ①
{0, 1, 4, 81, 64, 9, 16, 49, 25, 36}
>>> {x for x in a_set if x % 2 == 0} ②
{0, 8, 2, 4, 6}
>>> {2**x for x in range(10)} ③
{32, 1, 2, 4, 8, 64, 128, 256, 16, 512}
```
