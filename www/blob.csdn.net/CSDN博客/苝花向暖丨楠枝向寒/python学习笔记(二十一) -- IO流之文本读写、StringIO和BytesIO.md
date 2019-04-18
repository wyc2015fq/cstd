# python学习笔记(二十一) -- IO流之文本读写、StringIO和BytesIO - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月28日 22:25:27[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：324
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**目录**

[文本读写](#main-toc)

[open函数](#open%E5%87%BD%E6%95%B0)

[read() 和 write()](#read()%20%E5%92%8C%20write())

[read(size)、readline()、readlines()](#read(size)%E3%80%81readline()%E3%80%81readlines())

[seek() 和 tell()](#seek()%20%E5%92%8C%20tell())

[二进制文件](#%E4%BA%8C%E8%BF%9B%E5%88%B6%E6%96%87%E4%BB%B6)

[字符编码](#%E5%AD%97%E7%AC%A6%E7%BC%96%E7%A0%81)

[with与open结合使用](#with%E4%B8%8Eopen%E7%BB%93%E5%90%88%E4%BD%BF%E7%94%A8)

[StringIO](#StringIO)

[API](#API)

[简单使用](#%E7%AE%80%E5%8D%95%E4%BD%BF%E7%94%A8)

[BytesIO](#BytesIO%C2%A0)

## 文本读写

## open函数

通过 open 函数我们可以对一个文件 进行读写操作，简单的使用open函数只需用到两个参数，第一个是文件路径，第二个是对文件执行的操作。

对于第二个参数的常用的具体作用如下 
|参数|作用|文件存在|文件不存在|
|----|----|----|----|
|r|只读|正常打开，光标在起始位置|报错|
|r+|读写|正常打开，光标在起始位置，此时通过read读取文件，读取过后光标在最后，此时通过write写入，就是追加的效果，如果没有读取，直接写入内容，比如我们写入了3个字符，那么就覆盖掉原有的前3个字符，后面的字符不会被覆盖掉|报错|
|w|只写|创建新文件并覆盖原文件，光标在起始位置|创建新文件|
|w+|读写|创建新文件并覆盖原文件，光标在起始位置，也就是说你如果想通过 w+ 来读取一个已经存在的文件是无法实现的，它会重新创建一个文件将之前的覆盖，也就是每次打开的文件都是空的。|创建新文件|
|a|只写|正常打开，光标在末尾位置|创建新文件|
|a+|读写|正常打开，光标在末尾位置|创建新文件|

具体的使用

```python
>>> fd = open('d.txt','w')  # 第一个参数为要写入信息的文件名，第二个参数就是我要进行写入操作
```

注意我第一个参数 只有文件名，并没有路径，那么他会在当前目录下创建，那么如何查看当前的目录呢？

```python
>>> import os
>>> print(os.getcwd())
D:\工作软件\python
```

如果我这样写，在文件名前面加一个 /  ，代表当前目录的根目录下打开 d.txt 文件，如下：

```python
fd = open('/dee.txt','w')
```

这样就会在 D盘下 如果存在dee.txt 就创建新的并覆盖原文件，然后打开。不存在就创建然后打开。

通过上面我们知道，当前的位置为D:\工作软件\python ，那么我们如果想在 C:\iotest 文件夹下 创建一个 a.txt 文件该如何操作？

```python
>>> fd = open(r'C:\iotest\a.txt','w')
Traceback (most recent call last):
  File "<pyshell#18>", line 1, in <module>
    fd = open(r'C:\iotest\a.txt','w')
FileNotFoundError: [Errno 2] No such file or directory: 'C:\\iotest\\a.txt'
```

首先上面报错的原因是 我并没有 iotest这个文件夹，w能够创建要被打开的文件，但不能创建文件夹。

其次， 第一个参数 单引号 前面的  r ，代表 取消 \ 的转义功能

下面两种写法也可以：

```python
fd = open('C:\\iotest\\a.txt','w')
```

```python
fd = open('C:/iotest/a.txt','w')
```

### read() 和 write()

下面我们来一个 完整的  创建文件  -- 写入字符串  -- 读取文件 -- 关闭 文件   的操作

```python
>>> fd = open('/d.txt','w')  # w 不支持读取
>>> fd.write('123')
3
>>> fd.read()
Traceback (most recent call last):
  File "<pyshell#25>", line 1, in <module>
    fd.read()
io.UnsupportedOperation: not readable
>>> fd.close()   # 关闭文件，以 r 的方式打开
>>> fd = open('/d.txt','r')
>>> fd.read()
'123'
```

上面这个很麻烦，写完了还得关闭文件，然后再重新打开读取。

下面优化版本

```python
>>> fd = open('/d.txt','w+')  # w+ 读写都可以
>>> fd.write('456')  # 写入 456
3 
>>> fd.read()  # 读取到空字符串，因为写入字符串456以后，光标在文件末尾处，所以此时读取，后面没有数据
''
>>> fd.seek(0) # 将光标移到 最前面
0
>>> fd.read() # 读取出 456，注意，我们之前这个文件中是有字符串 123的，但由于我用的是 w+ 打开的文件，它会重新创建一个文件并覆盖到原来的
'456'
```

我们再用r+ 来试试

```python
>>> fd = open('/d.txt','r+') # 这个文件之前的字符串为 456
>>> fd.write('789')  # 写入 789 ，此时光标在最后
3
>>> fd.seek(0)  # 将光标移到最前
0
>>> fd.read()  #读取出789，但是这里并不是 创建了一个新文件，将原文件覆盖了
'789'          #而是我们以 r+ 的方式打开文件，光标在最前面，此时输入789，这三个字符将 456 三个字符替换了，而不是整个文件被替换
>>> fd.close()
>>> fd = open('/d.txt','r+')  #我们重新打开该文件 ，此时文件内容为 789
>>> fd.read()  # 这次我们先读取数据，目的是将光标移动到最后面
'789'
>>> fd.write('123') #写入
3 
>>> fd.read()  # 读取到空字符，因为光标在最后
''
>>> fd.seek(0) # 将光标移动到最前面
0
>>> fd.read()  #读取 
'789123'
>>> fd.close()
```

通过上面的例子，可以看出如果我们想在一个已存在文件的最后添加字符串使用 r+ 太麻烦了，我们还要先读取再写入，那么有没有比较简单的方法呢？当然，我们可以使用 a+ 

```python
>>> fd = open('/d.txt','a+')
>>> fd.read()   # 读到空字符， 说明 此时光标已经在 最后
''
>>> fd.write('456')  # 写入 456
3
>>> fd.seek(0)  # 光标移到最前面
0
>>> fd.read()  # 读取数据
'789123456'
```

### read(size)、readline()、readlines()

```python
>>> fd = open('/d.txt','r')
>>> fd.read(3)  # 文本中是英文就读 3个 中文
'的脸孔'
>>> fd.read(6)  # 读 6个中文，最后一个 \n 是换行符，就是 我们在文本中输入了回车 换行
'次加息欧洲\n'
>>> fd.read(3)  # 如果是数字和英文就是读取 3个数字或3个英文
'789'
>>> fd.readline()  # 读取一行
'123456789123456789123\n'
>>> fd.readline()
'4567891234567891234567891234\n'
>>> fd.readlines() # 一行一行的读取，将文件读完，返回的是一个list
['5678912345678912345678912345678912\n', '34567891234567891234567891234567891234\n', '56789123456789123456789123456789123456789123456\n', '789123456789123456789123456789123456789123456789123456789\n', '123456789123456789123456789123456789123456789123456789123456789123\n', '456789123456789123456789123456789123456\n', '7891234567891234567891234567891234567\n', '3456789123456789123456789123456\n', '89123456789123456789123456789123456\n', '789123456789123456789123456789123456789\n', '89123456789123456789123456789123456789123456789123456789123456789123\n', '4567891234567891234567891234567891234567891234567891234567891234567\n', '8912345678912345678912345678912345678912345678912345678912345678912\n', '1234567891234567891234567891234\n', '56789123456789123456789123456']
```

### seek() 和 tell()

seek()函数 可以传两个参数

第一个参数 为 偏移量，就是将光标移动几个位置  

第二个参数 为 相对于谁偏移

一共有 3个值  ： 0代表从文件开头开始算起，1代表从当前位置开始算起，2代表从文件末尾算起。  默认值就是 0 

所以我们上面 直接 seek(0) 就代表我想为与 起始位置 偏移量为0， 所以就将光标移动到了起始位置。

tell() 函数 返回光标当前位置

代码演示如下：

```python
>>> fd = open('/d.txt','a+')
>>> fd.tell()
9
>>> fd.seek(-9,1)
Traceback (most recent call last):
  File "<pyshell#66>", line 1, in <module>
    fd.seek(-9,1)
io.UnsupportedOperation: can't do nonzero cur-relative seeks
>>> fd.seek(-9,2)
Traceback (most recent call last):
  File "<pyshell#67>", line 1, in <module>
    fd.seek(-9,2)
io.UnsupportedOperation: can't do nonzero end-relative seeks
>>> fd.seek(0)
0
```

我们注意到上面 两个都报错，是这样，如果seek的第二个参数 为 1或着 2， 那么我们打开文件的方式必须以二进制打开。

也就是 我们  需要用到   ab+  

如下：

```python
>>> fd = open('/d.txt','ab+')
>>> fd.tell()
9
>>> fd.seek(-9,2)
0
>>> fd.read()
b'789123456'
>>> fd.seek(-9,1)
0
```

### 二进制文件

上面用到了 ab+ ，其实只要是 包含了字母b ，就代表我们读取二进制文件，这个往往用于读取音频和视频。

如下我读取了一张图片，读取出来的内容以 16进制来表示

```python
>>> fd = open(r'C:\Users\Administrator\Desktop\img\1.jpg','rb')
>>> fd.read(5)
b'\xff\xd8\xff\xe1\x00'
```

### 字符编码

 open函数默认以操作系统的编码格式来打开一个文件，我的系统是windos10 默认为gbk，现在我新建一个txt文本，将其另存为utf8 的编码格式，这时候我读取文件会产生乱码，如下

```python
>>> fd = open(r'D:\test.txt','r')
>>> fd.read()
'锘挎祴璇曟枃浠讹紝缂栫爜鏍煎紡涓篻bk'
```

其实open函数还有一个参数，就是来指定编码格式的，用法如下：

```python
>>> fd = open(r'D:\test.txt','r',encoding='utf8')
>>> fd.read()
'\ufeff测试文件，编码格式为gbk'
```

 可以看到，中文正常打印出来了，但是 \ufeff 是什么？

\ufeff是用来标记该文本的编码格式为UTF-8、UTF-16或UTF-32编码的记号，那我们如何将这个记号去除呢？

如下：

```python
>>> fd = open(r'D:\test.txt','r',encoding='utf8')
>>> data = fd.read()
>>> new_data = data.encode('utf-8').decode('utf-8-sig') #编码格式转换
>>> new_data
'测试文件，编码格式为gbk'
```

### with与open结合使用

前一篇文章已经介绍了with的作用，不清楚的请看[这里](https://blog.csdn.net/weixin_40247263/article/details/82120213)。

我们为什么要 with 与 open来结合?

一: open 函数可能会抛出异常，在实际开发中为了不影响后面代码的运行，我们需要捕获异常，但是try ..catch .. finally 这样写很麻烦，with 也可以捕获异常，所以用其替代。

二: 打开一个文件以后，操作完毕应及时关闭，也就是调用clsoe()函数，这个有时候容易忘，open函数返回的对象的 __exit__函数中执行了close函数，也就是我们在 with 代码块中对文件操作过后，文件会自动关闭。

使用如下：

```python
with open('/path/to/file', 'r') as f:
    print(f.read())
```

## StringIO

StringIO 实际上是从内存当中读取字符串，然后放入缓冲区当中，缓冲区我们可以理解成一个类似于txt文件的一个容器，用来存放字符串，当字符串长度大的时候，使用SringIO能够提高效率，至于它为什么能提高效率我从网上看到了一段很形象的话来解释缓冲区：

        缓冲区， 说白了， 就是一个中介。 比如你要租一个房子， 刚好房子的主人也要出租房子， 你们可以直接联系， 但问题就在于： 房子的主人可能没有时间和你扯太多， 你也没有时间和房子的主人扯这扯那， 这个时候， 中介产生了。 中介有时间和你扯皮， 中介和房子的主人也提前沟通好。

        在上面的例子里， 你通过中介一次性把信息反馈给房子主人， 房子主人把自己的要求通过中介一次性给你。 本来，你和房子主人这两个大忙人的空余时间不一致， 不便于沟通， 但有了中介， 就好办事了。 提高了沟通的效率。

        缓冲区， 就是中间区， 主要方便两者沟通， 提高效率。 在编程中， 经常会听说这个概念。 在其他地方， 这个概念也经常用到![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)。

综上所述： 我个人理解，字符串的缓冲区应该是来协调 cpu 和 内存 的时间的。

### API

```
# StringIO 模块中的函数：
# s.read([n])
# 参数n限定读取长度，int类型；缺省状态为从当前读写位置读取对象s中存储的所有数据。读取结束后，读写位置被移动。
# 
# ----------------------
# s.readline([length])
# 参数length限定读取的结束位置，int类型，缺省状态为None：从当前读写位置读取至下一个以“\n”为结束符的当前行。读写位置被移动。
# 
# ----------------------
# 
# s.readlines([sizehint])
# 参数sizehint为int类型，缺省状态为读取所有行并作为列表返回，除此之外从当前读写位置读取至下一个以“\n”为结束符的当前行。读写位置被移动。
# 
# ----------------------
# s.write(s)
# 从读写位置将参数s写入给对象s。参数s为str或unicode类型。读写位置被移动。
# 
# ----------------------
# s.writelines(list)
# 从读写位置将list写入给对象s。参数list为一个列表，列表的成员为str或unicode类型。读写位置被移动。
# 
# ----------------------
# s.getvalue()
# 此函数没有参数，返回对象s中的所有数据。
# 
# ----------------------
# s.truncate([size])
# 从读写位置起切断数据，参数size限定裁剪长度，缺省值为None。
# 
# ----------------------
# s.tell()
# 返回当前读写位置。
# 
# ----------------------
# s.seek(pos[,mode])
# 移动当前读写位置至pos处，可选参数mode为0时将读写位置移动至pos处，为1时将读写位置从当前位置起向后移动pos个长度，
# 为2时将读写位置置于末尾处再向后移动pos个长度；默认为0。
# 
# ----------------------
# s.close()
# 释放缓冲区，执行此函数后，数据将被释放，也不可再进行操作。
# 
# ----------------------
# s.isatty()
# 此函数总是返回0。不论StringIO对象是否已被close()。
# 
# ----------------------
# s.flush()
# 刷新内部缓冲区
```

### 简单使用

```python
>>> from io import StringIO    # 导入io模块下的 StringIO类
>>> s = StringIO()   # 创建一个StringIO对象
>>> s.write('www.baidu.com\n')   # 向其中写入数据
14
>>> s.write('news.realsil.com.cn')  # 再写入一行数据
19
>>> print(s.getvalue())   #　通过getvalue()函数获得缓冲区中的值，注意这个函数不需要光标在最前面，无论光标在哪里它都可以获取到缓冲区中的元素　
www.baidu.com
news.realsil.com.cn
>>> s.seek(0)    # 将光标移动至最前面
0
>>> while 1:  # while = 1 比True 的效率要高
	strBuf = s.readline()  #  每次读取一行数据，read开头的方法则是从当前光标位置起读取之后的字符串
	if strBuf == "":   # 读到最后就跳出循环
		break
	print(strBuf.strip())  # 将读取到的字符串 去除前后空格后打印

	
www.baidu.com
news.realsil.com.cn
>>> s.close() # 释放缓冲区空间，注意这里和文件的close还不一样，由于文件是存在于硬盘当中的，所以close掉以后，我们还可以再次通过open函数打开，而StringIO使用close以后就真的不存在了
```

```python
>>> f = StringIO("hello!")   # 可以在创建StringIO对象的同时直接将字符串放入缓冲区
>>> f.getvalue()
'hello!'
>>> f.read()
'hello!'
```

## BytesIO 

```python
>>> from io import BytesIO
>>> f = BytesIO()
>>> f.write("中文".encode('utf-8'))  # 主要写入的内容不能是str，必须是字节
6
>>> print(f.getvalue())
b'\xe4\xb8\xad\xe6\x96\x87'

>>> f1 = BytesIO('中文'.encode('utf-8'))  # 同理也可以在创建对象时直接写明要存入的数据
>>> f1.read()
b'\xe4\xb8\xad\xe6\x96\x87'

>>> f2 = BytesIO(b'\xe4\xb8\xad\xe6\x96\x87')  # 也可以这样写
>>> f2.read()
b'\xe4\xb8\xad\xe6\x96\x87'
```

BytesIO和StringIO 一样，只不过StringIO存的是字符，而这个是字节


