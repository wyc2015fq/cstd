# python学习笔记(二十二)  -- 操作文件和目录、序列化 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月31日 09:02:12[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：46
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[前言](#%E5%89%8D%E8%A8%80)

[创建、删除目录](#%E5%88%9B%E5%BB%BA%E3%80%81%E5%88%A0%E9%99%A4%E7%9B%AE%E5%BD%95)

[创建、删除一层目录](#%E5%88%9B%E5%BB%BA%E3%80%81%E5%88%A0%E9%99%A4%E4%B8%80%E5%B1%82%E7%9B%AE%E5%BD%95)

[获取当前目录](#%E8%8E%B7%E5%8F%96%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95)

[创建、删除多层目录](#%E5%88%9B%E5%BB%BA%E3%80%81%E5%88%A0%E9%99%A4%E5%A4%9A%E5%B1%82%E7%9B%AE%E5%BD%95)

[重命名文件、文件夹](#%E9%87%8D%E5%91%BD%E5%90%8D%E6%96%87%E4%BB%B6%E3%80%81%E6%96%87%E4%BB%B6%E5%A4%B9)

[os.path 模块](#os.path%20%E6%A8%A1%E5%9D%97)

[序列化](#%E5%BA%8F%E5%88%97%E5%8C%96)

[序列化为bytes](#%E5%BA%8F%E5%88%97%E5%8C%96%E4%B8%BAbytes)

[序列化为json](#%E5%BA%8F%E5%88%97%E5%8C%96%E4%B8%BAjson)

## 前言

 操作文件和目录的函数基本上都在 os 和 os.path 这两个模块中

os 模块中包含文件夹的创建、删除，对文件的删除（文件的创建通过之前说过的open函数），还有文件的重命名等。

os.path 涉及获取路径、对路径的拼接以及拆分，还可以获取文件后缀名等。

## 创建、删除目录

### 创建、删除一层目录

我们可以通过 os 模块中的函数来进行目录的创建

```python
>>> os.mkdir('test/testdir')  # 最后一个 / 后面跟的是文件名
```

删除一个目录

```python
>>> os.rmdir('test/testdir')　　# 如果我参数是这样的 /test/testdir 代表删除当前目录所在磁盘的根目录下的test文件中的testdir文件夹，删除D:\test 下的testdir
```

上面这种写法指的是在当前目录下的test文件夹下，创建一个名为testdir的文件夹。

### 获取当前目录

当前目录是什么？ 可以通过 如下两种方式查看

```python
>>> os.getcwd()
'D:\\python'
```

```python
>>> os.path.abspath('.')    # abspath这个函数必须有参数，参数为 . 即返回当前目录
'D:\\python'
>>> os.path.abspath('test')  # 除了 .　以外，输入的参数为什么就返回　当前目录的下级目录，目录名为参数名
'D:\\python\\test'
```

### 创建、删除多层目录

```python
>>> os.makedirs(r'test\testdir')  # 在当前目录下创建 test文件夹，并在test文件夹下创建testdir文件夹

>>> os.removedirs('test/testdir') # 删除test文件夹和testdir文件夹
```

### 重命名文件、文件夹

rename 和 renames 都可以更改文件名或文件夹名。

并且可以同时修改文件以及文件所在的位置。

第一个参数为 当前文件或文件夹的路径，第二个参数为重命名后的 路径，注意这里的重命名不单单是操作系统下的重命名，这里的重命名还涉及路径的更改

```python
>>> os.rename('test/a.txt','test/a.py') # 将test文件夹下的a.txt改名为a.py
```

上面要注意第二个参数必须也写清楚路径，如果第二个参数为 'a.py' ，那么python会将 test中的 a.txt 删除，并在文件夹test所在的目录下创建文件 a.py 。

**rename 和 renames 的 区别**就在在于第二个 参数，第二个参数中路径包含的文件夹如果不存在，rename会报错，而renames则会创建该目录。

```python
>>> os.rename('/安装文件/e.py','/test/a.py')
	       
Traceback (most recent call last):
  File "<pyshell#23>", line 1, in <module>
    os.rename('/安装文件/e.py','/test/a.py')
FileNotFoundError: [WinError 3] 系统找不到指定的路径。: '/安装文件/e.py' -> '/test/a.py'
>>> os.renames('/安装文件/e.py','/test/a.py')
```

上我我将  D 盘下的 安装文件 文件夹下的 e.py 文件， 移动到 D盘下的 test文件夹下，并改名为 a.py，第一行用的是rename函数，但是由于 D盘中并不存在 test文件夹，所以报错。下面我们改用renames函数就成功了。

### os.path 模块

```python
>>> os.path.join('/a/b/c','test/subtest')  # 拼接路径
'/a/b/c\\test/subtest'
>>> os.path.split('a/b/c/file.txt')  # 取文件名
('a/b/c', 'file.txt')
>>> os.path.splitext('a/b/c/file.txt')  # 取后缀名
('a/b/c/file', '.txt')

# split 和 splitext 返回的都是tuple ，直接通过索引位就可以获取到
```

## 序列化

当我们想把内存中的对象进行网络中的传输或者想将内存中的对象存入硬盘中时，我们则需要将对象序列化，因为对象实际上是内存中的一块空间，空间里面又存放很多信息例如变量名、变量值之类的，你直接去保存一个对象它不知道如何去保存。但是如果有过java学习经验的人会有疑问，为什么将一个pojo保存到数据库，但是该pojo并没有实现序列化接口，也能保存呢，实际上数据库保存的并不是对象，而是对象中的变量的值。

序列化分为两种，一种是序列化为bytes，一种是序列化为json。

一般存入硬盘中我们会将对象序列化为bytes，因为硬盘中保存的文件都是以二进制保存的，传输也是。

如果用于不同语言之间传输我们序列过为json，因为json实际上是javascipt语言的对象，并且它是所有语言都支持的一种数据格式，例如对应java中就是map，对应python中就是dict。

### 序列化为bytes

方式一：

```python
>>> import pickle   # 导入模块
>>> class Student(object):  # 定义Studnet类
	def __init__(self,name,age,score):
		self.name = name
		self.age = age
		self.score = score

		
>>> s = Student('Bob',20,88)  # 创建对象
>>> f = open('D:/a.txt','wb')  # 创建文件，注意要包含参数b 
		      
>>> f.write(pickle.dumps(s))  # 将对象序列化为bytes 同时保存到文件
		    
80
```

方式二：

```python
>>> f = open('D:/a.txt','wb')
>>> pickle.dump(s,f)  # 直接通过 dump函数，对象序列化的同时存入文件
```

反序列化：从文件中再读取出对象。

```python
>>> f = open('D:/a.txt','rb')  # 以二进制方式读取文件
		    
>>> pickle.load(f)  # 从文件中 加载出对象
		    
<__main__.Student object at 0x00000297E1EBDBE0>
>>> f.seek(0)  # 记住 load和 read 方法类似，加载完以后光标在最后，如果想重新读取，必须将光标移至最前面
		    
0
>>> s = pickle.load(f) # 再次加载
		    
>>> s.name    # 查看变量值
		    
'Bob'
>>> s.age
		    
20
>>> s.score
		    
88
```

### 序列化为json

如果我们想把一个对象序列化为json，我们必须告诉python序列化后数据的样子，要不然它不知道一个对象如何序列化，在java我们使用的一些jar，如fastjson，它是通过调用pojo的get方法最终拼接成的json。但python中并不是这样。

使用方法如下：

```python
>>> import json # 导模块
>>> class Student(object):  
	def __init__(self,name,age,score):
		self.name = name
		self.age = age
		self.score = score

		
>>> s = Student('Bob',20,88)
# json.dumps函数中，第一个参数为被序列化的对象，第二个参数为对象内部的函数，该函数的返回值应是一个dict。
# 首先一个简单的函数可以通过lambda表达式来代替，其次每一个对象都有一个__dict__，用来保存对象的变量名和值。
# 当然也有特殊的保存在了__solt__中。
# dumps内部会自动调用 default指向的函数，并将参数一的对象传入进去
>>> print(json.dumps(s, default=lambda obj: obj.__dict__)) 		    
{"name": "Bob", "age": 20, "score": 88}
```

反序列化

反序列化实际上就是 将 json 转换为 自定义类的对象，也就是我们需要根据json 给实体类的属性赋值，所以需要定义一个函数，它会返回一个自定义类对象，如下

```python
>>> def dict2student(d):  # 定义一个函数，用来接收dict，返回Studnet
		return Student(d['name'], d['age'], d['score'])
```

```python
#  json.loads 接收的第一个参数是一个json，然后会调用 object_hook 指向的函数，并将json传进去
>>> print(json.loads(json_str, object_hook=dict2student)) 
		    
<__main__.Student object at 0x00000297E1FC6668>
```

