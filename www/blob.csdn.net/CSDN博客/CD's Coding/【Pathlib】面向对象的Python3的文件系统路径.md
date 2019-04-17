# 【Pathlib】面向对象的Python3的文件系统路径 - CD's Coding - CSDN博客





2018年02月06日 15:44:58[糖果天王](https://me.csdn.net/okcd00)阅读数：502








# 【Pathlib】面向对象的Python3的文件系统路径

> 
**[Pathlib](https://docs.python.org/3/library/pathlib.html)：将文件系统路径作为对象使用**
[《pathlib —— Object-oriented filesystem paths》](https://docs.python.org/3/library/pathlib.html)


## 0x00 前言

> 
目前，Python 科学栈中的所有主要项目都同时支持 Python 3.x 和 Python 2.7，不过，这种情况很快即将结束。去年 11 月，Numpy 团队的一份声明引发了数据科学社区的关注：这一科学计算库即将放弃对于 Python 2.7 的支持，全面转向 Python 3。Numpy 并不是唯一宣称即将放弃 Python 旧版本支持的工具，pandas 与 Jupyter notebook 等很多产品也在即将放弃支持的名单之中。[【新闻链接】](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650736815&idx=1&sn=be5b6938ac346f2e43a803753002bc8f&chksm=871accd1b06d45c78b6fa0a48a169482db1f0113a5c9680f36fb6dd0bb53c01dabcf5377e431&scene=0#rd)


根据近期新闻，隐隐觉得必须尽早熟识掌握进阶到Python3的特性与区别，今天看了看文件路径方面，发现这个还挺好用的自带库，那么来看看它的特性吧。

## 0x01 获取对象（实例化）

面向对象编程的第一步，自然是需要获取pathlib的实例： 

不传参时默认使用当前位置`Path('.')`，也可传一个或多个字符串自动拼接

```python
from pathlib import Path

q = Path()  # WindowsPath('.')
q = Path('setup.py')  # WindowsPath('setup.py')
q = Path('test', 'setup.py')  # WindowsPath('test/setup.py')
```

这里需要知道的是，在不同的OS下，会自动适配不同的对象类型： 

Unix族的OS上是`PosixPath`，Windows上是`WindowsPath`

```python
Path('setup.py')      
# PosixPath('setup.py')  # on a Unix machine
Path('setup.py')
# WindowsPath('setup.py')  # on a Windows machine
```

特别的，如果传参包含绝对路径：

```python
# 都是绝对路径的话，会取最后一个
Path('c:/Windows', 'd:bar') 
# WindowsPath('d:bar')

Path('c:/Windows', '/Program Files')
WindowsPath('c:/Program Files')
```

## 0x02 除号重载，增强读写性

比较醒目而直观的一点，是可以简单的将路径字符串通过Pathlib实例化后，简单的使用除法操作符（`/`），就可以与字符串或是同为Pathlib实例的对象进行拼接操作。

```python
# pathlib_operator.py
usr = Path.PurePosixPath('/usr')
print(usr)  # /usr

usr_local = usr / 'local'
print(usr_local)  # /usr

usr_share = usr / Path.PurePosixPath('share')
print(usr_share)  # /usr/share

root = usr / '..'
print(root)  # /usr/..

etc = root / '/etc/'
print(etc)  # /etc
```

## 0x03 文件遍历与筛选

对于文件遍历，我们平时那些常用的组合拳，放在前面备查备忘； 

详细的成员函数和属性后面再列个清单好了。

### Resolve方法

`p.resolve()`可以理解为一个anchor或是一个cursor 

在文件系统里按照路径的顺序，获得这一路走下去走完之后的路径。

如下述示例代码中： 
`'/usr/local'`在返回上一级`'..'`后成为`'/usr'`
`'/usr'`在拼接`'share'`后成为`'usr/share'`
```python
usr_local = Path('/usr/local')
share = usr_local / '..' / 'share'
print(share.resolve())  # 'usr/share'
```

### 获取子目录列表

```python
p = Path('.')
[x for x in p.iterdir() if x.is_dir()]

# [ PosixPath('.hg'), 
# PosixPath('docs'), PosixPath('dist'),
# PosixPath('__pycache__'), PosixPath('build') ]
```

### 规则获取文件列表

```python
p = Path('.')
list(p.glob('**/*.py'))

# [ PosixPath('test_pathlib.py'), PosixPath('setup.py'),
# PosixPath('pathlib.py'), PosixPath('docs/conf.py'),
# PosixPath('build/lib/pathlib.py') ]
```

## 0x04 对象的使用

### 获取路径

获取当前路径`cwd()`与home路径`home()`

```python
Path.cwd()
# WindowsPath('C:/Users/okcd0')
Path.home()
# WindowsPath('C:/Users/okcd0')
```

### 获取信息

与先前的路径判断相似，可以调用对象的成员函数来获得这些信息：

```python
q.exists()  # 是否存在
q.is_dir()  # 是否文件夹
q.is_file()  # 是否文件
q.is_absolute()  # 是否绝对路径
q.match('*.py')  # 是否匹配该正则
```

获取当前Path对象所指目标的信息： 

如文件权限`chmod`、所属用户`owner`：

```python
p = Path('setup.py')
p.owner()  # okcd0
p.stat().st_size  # 956
p.stat().st_mtime  # 1327883547.852554
p.stat().st_mode  # 33277

p.chmod(0o444)
p.stat().st_mode  # 33060
```

### 切分路径

这个比起以往的要方便的多了，分析路径的时候不再需要字符串级别处理：

```python
q = Path('test', 'setup.py')  # WindowsPath('test/setup.py')
q.parts  # ('test', 'setup.py')
```

除了`parts`可以分割开以外，还有一些便捷的函数可以直接获得属性： 

如`path`，`name`，`suffix`，`stem`（路径、文件名、后缀、去掉后缀的文件名）

```python
p = pathlib.PurePosixPath('./source/pathlib/pathlib_name.py')
print('path  : {}'.format(p))
print('name  : {}'.format(p.name))
print('suffix: {}'.format(p.suffix))
print('stem  : {}'.format(p.stem))

# path  : source/pathlib/pathlib_name.py
# name  : pathlib_name.py
# suffix: .py
# stem  : pathlib_name
```

关于`suffix`还有一个`suffixes`的用法：

```python
Path('my/library.tar.gar').suffix
# '.gar'
Path('my/library.tar.gar').stem
# 'my/library.tar'

Path('my/library.tar.gar').suffixes
# ['.tar', '.gar']
Path('my/library.tar.gz').suffixes
# ['.tar', '.gz']
Path('my/library').suffixes
# []
```

再说在Windows里，还有一些与盘符相关的属性： 

如`drive`，`root`，`anchor`（盘符，根目录，锚点）

```python
Path('c:/Program Files/').drive  # 'c:'
Path('/Program Files/').drive  # ''
Path('/etc').drive  # ''
```

### 路径部分替换

当我们获得某个路径对象后，可以替换掉其中特定部分： 

如下述示例代码中： 

通过`with_name()`替换文件名部分（注意文件名是带后缀的） 

通过`with_suffix()`替换后缀部分
```python
ind = Path('source/pathlib/index.rst')
print(ind)  # source/pathlib/index.rst

py = ind.with_name('pathlib_from_existing.py')
print(py)  # source/pathlib/pathlib_from_existing.py

pyc = py.with_suffix('.pyc')
print(pyc)  # source/pathlib/pathlib_from_existing.pyc
```

### 上级路径

返回一个可迭代的`<WindowsPath.parents>`对象，可以迭代或下标获取：

```python
Path('test', 'setup.py').parents[0]  # 'test'
Path('test', 'setup.py').parents[1]  # '.'
```

### 文件读写

类似的使用实例化的文件路径对象来进行文件的读写，我们常用的方式如下：

```python
q = Path('example.txt')
with q.open() as f: 
    f.readline()

# '#!/bin/bash\n'
```

而Pathlib库提供了一个更加便捷的方法：

```python
f = Path('example.txt')
f.write_bytes('This is the content'.encode('utf-8'))
# f.write_text('This is the content')

with f.open('r', encoding='utf-8') as handle:
    print('read from open(): {!r}'.format(handle.read()))
# read from open(): 'This is the content'

print('read_text(): {!r}'.format(f.read_text('utf-8')))
# read_text(): 'This is the content'
```





