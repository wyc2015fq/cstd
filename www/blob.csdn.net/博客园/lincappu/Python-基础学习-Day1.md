# Python-基础学习-Day1 - lincappu - 博客园







# [Python-基础学习-Day1](https://www.cnblogs.com/lincappu/p/8157502.html)






# 1 Python介绍

## 1.1 Python 是一门什么样的语言？

> 
python是一门动态解释性的强类型定义语言。


> 
编译型的特点：可一致性差，运行速度快。


> 
解释型的特点：边执行边解释,速度慢


## 1.2 Python 优缺点

### 优点

> - Python的定位是“优雅”、“明确”、“简单”，所以Python程序看上去总是简单易懂，初学者学Python，不但入门容易，而且将来深入下去，可以编写那些非常非常复杂的程序。
- 开发效率非常高，Python有非常强大的第三方库，基本上你想通过计算机实现任何功能，Python官方库里都有相应的模块进行支持，直接下载调用后，在基础库的基础上再进行开发，大大降低开发周期，避免重复造轮子。
- 高级语言————当你用Python语言编写程序的时候，你无需考虑诸如如何管理你的程序使用的内存一类的底层细节
- 可移植性————由于它的开源本质，Python已经被移植在许多平台上（经过改动使它能够工 作在不同平台上）。如果你小心地避免使用依赖于系统的特性，那么你的所有Python程序无需修改就几乎可以在市场上所有的系统平台上运行
- 可扩展性————如果你需要你的一段关键代码运行得更快或者希望某些算法不公开，你可以把你的部分程序用C或C++编写，然后在你的Python程序中使用它们。
- 可嵌入性————你可以把Python嵌入你的C/C++程序，从而向你的程序用户提供脚本功能。


### 缺点

> - 速度慢


> - 不能加密


> - 线程不能利用多CPU问题


# 2 Python的安装

## 2.1 windows

> 
1、下载安装包
[https://www.python.org/downloads/](https://www.python.org/downloads/)

2、安装
默认安装路径：C:\python27

3、配置环境变量
【右键计算机】--》【属性】--》【高级系统设置】--》【高级】--》【环境变量】--》【在第二个内容框中找到 变量名为Path 的一行，双击】 --> 【Python安装目录追加到变值值中，用 ； 分割】
`如：原来的值;C:\python27，切记前面有分号`

### 注意：windows同时安装python3.5和python2.7，配置环境变量后输入python2提示：不是内部或外部命令，也不是可运行的程序

> - 已经配置环境变量


> - 根据python2.7的安装路径修改python.exe的名称为python2.exe


> - cmd命令直接输入python2正常


## 2.2 linux安装python2.7

下载源码，并重新编译安装：

> 
```
1. cd /usr/local/src/
2. wget http://python.org/ftp/python/2.7.8/Python-2.7.8.tgz
3. tar xf Python-2.7.8.tgz
4. cd Python-2.7.8
5. ./configure --prefix=/usr/local/python27
6. make && make install
7.  mv /usr/bin/python /usr/bin/python26 #将系统安装的重命名转移
8.  ln -s /usr/local/python27/bin/python /usr/bin/ #创建软连接直接调用2.7版本python
9.  vim /usr/bin/yum ##!/usr/bin/python2.6 修改原系统默认的python版本 以保证yum命令应用
```


## 2.3 linux安装python3.5

下载源码，并重新编译安装
下载Python3.5的源码包并编译

```
wget https://www.python.org/ftp/python/3.5.0/Python-3.5.0.tgz
tar xf Python-3.5.0.tgz
cd Python-3.5.0
./configure --prefix=/usr/local --enable-shared
make
make install
温馨提醒：如果编译遇到如下问题：
Ignoring ensurepip failure: pip 7.1.2 requires SSL/TLS
解决方法：
yum install -y openssl openssl-devel
继续：
ln -s /usr/local/bin/python3 /usr/bin/python3

在运行Python之前需要配置库：

echo /usr/local/lib >> /etc/ld.so.conf.d/local.conf
ldconfig
可以设置别名：alias py=python3方便使用
```

# 3 Hello world等程序代码编写

程序执行信息：

在windows上执行python后进入python交互器：

```python
>>> print ("Hello world!")
Hello world!
```

注意：python2的raw_input（） == python3的input（） 但是在python3中执行不带括号的会报错：

```python
>>> print "Hello world!"
File "<stdin>", line 1
print "Hello world!"
                   ^
SyntaxError: Missing parentheses in call to 'print'
```

## 3.1 小知识点

> - 退出Python交互器：exit() windows下ctrl+z 回车 linux下Ctrl+z退出


> - = 左边永远是变量名 =右边字符串的话需要添加"string" 数字可不加，运算：+-*/


> - print (“字符串”,变量) #数字不需要加引号 引用的变量不加引号


> - 在 python2 里输出中文 需要声明字符编码： #-*- coding:utf-8 -*-，python3 默认就是UTF-8字符编码


> - 一个二进制数字=1bite bit = 计算机的最小存储单位。8比特 = 1bytes 字节 1024bytes = 1kbytes KB


> - 双引号同单引号一样 ：单双引号


> - 单行和多行注释：ctrl + /


> - 多行注释：’’’ ’’’ 通三个双引号 同时表示一个段落的字符串。


> - ctrl +d 复制当前行


> - id() 可查看变量内存地址


## 3.2 变量命名规则：

### 字符编码及变量声明

> 
python2中一定要声明字符编码 #_*_coding:utf-8_*_


> 
python3中默认是utf-8


> 
name = "Stone"


> 
上述代码声明了一个变量，变量名为： name，变量name的值为："Stone"　


### 变量定义的规则：

> - 变量名只能是 字母、数字或下划线的任意组合


> - 变量名的第一个字符不能是数字


> - 以下关键字不能声明为变量名['and', 'as', 'assert', 'break', 'class', 'continue', 'def', 'del', 'elif', 'else', 'except', 'exec', 'finally', 'for', 'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'not', 'or', 'pass', 'print', 'raise', 'return', 'try', 'while', 'with', 'yield']


### 变量程序实例：

```python
name = input("name:")   定义变量
age = input("age:")
job = input("job:")
hobby = input("hobby:")

info = '''              定义输出变量
-----info of %s ----- 
Name : %s
Age  : %s
Job  : %s
Hobby: %s
----- end -----         
''' %(name,name,age,job,hobby)
print(info)             输出
```

## 3.3 用户交互

```
>>> name = input("pls input your name:")
pls input your name:stone
>>> name
'stone'
```

将input的值赋予给name变量，输入的是什么name就是什么

```
#!/usr/bin/env python
#_*_coding:utf-8_*_
#name = raw_input("What is your name?") #only on python 2.x
name = input("What is your name?")
print("Hello " + name )
```

输入密码时，如果想要不可见，需要利用getpass 模块中的 getpass方法，即：

```bash
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import getpass

# 将用户输入的内容赋值给 name 变量
pwd = getpass.getpass("请输入密码：")

# 打印输入的内容
print(pwd)
```

## 3.4 条件判断与首行缩进

### 猜年龄程序

```python
#!/usr/bin/env python       #指定解释器为python
right_age = 23              #首先定义正确的年龄
guess_age = int(input("please input your guess num:"))#将input的值定义一个变量名
if right_age == guess_age:  #进行条件判断 正确（注意判断后加":"）
    print("right")          #注意缩进统一级别缩进要相同
elif guess_age > right_age: #猜的年龄大于真实年龄
    print("please guess smaller!")
else:
    print("please guess bigger!") #猜的年龄小于真实年龄
```

### 猜年龄-循环次数限定

```python
#!/usr/bin/env python
right_age = 23      #定义正确年龄
for n in range(3):  #执行3次，没有猜对退出
    guess_age = int(input("please input your guess num:"))
    if right_age == guess_age:
        print("right")
        break       # break 猜对后退出当层循环
    elif guess_age > right_age:
        print("please guess smaller!")
    else:
        print("please guess bigger!")
else:               #猜测次数过多，退出
    print("too many times!!!")
```

### 小知识点：

> 
出现缩进错误提示：注意同一级别的代码缩进必须保持一致。

选择多行+tab 进行多行缩进

type + 变量 输出变量类型





















# [Python-基础学习-Day1](https://www.cnblogs.com/lincappu/p/8157502.html)






# 1 Python介绍

## 1.1 Python 是一门什么样的语言？

> 
python是一门动态解释性的强类型定义语言。


> 
编译型的特点：可一致性差，运行速度快。


> 
解释型的特点：边执行边解释,速度慢


## 1.2 Python 优缺点

### 优点

> - Python的定位是“优雅”、“明确”、“简单”，所以Python程序看上去总是简单易懂，初学者学Python，不但入门容易，而且将来深入下去，可以编写那些非常非常复杂的程序。
- 开发效率非常高，Python有非常强大的第三方库，基本上你想通过计算机实现任何功能，Python官方库里都有相应的模块进行支持，直接下载调用后，在基础库的基础上再进行开发，大大降低开发周期，避免重复造轮子。
- 高级语言————当你用Python语言编写程序的时候，你无需考虑诸如如何管理你的程序使用的内存一类的底层细节
- 可移植性————由于它的开源本质，Python已经被移植在许多平台上（经过改动使它能够工 作在不同平台上）。如果你小心地避免使用依赖于系统的特性，那么你的所有Python程序无需修改就几乎可以在市场上所有的系统平台上运行
- 可扩展性————如果你需要你的一段关键代码运行得更快或者希望某些算法不公开，你可以把你的部分程序用C或C++编写，然后在你的Python程序中使用它们。
- 可嵌入性————你可以把Python嵌入你的C/C++程序，从而向你的程序用户提供脚本功能。


### 缺点

> - 速度慢


> - 不能加密


> - 线程不能利用多CPU问题


# 2 Python的安装

## 2.1 windows

> 
1、下载安装包
[https://www.python.org/downloads/](https://www.python.org/downloads/)

2、安装
默认安装路径：C:\python27

3、配置环境变量
【右键计算机】--》【属性】--》【高级系统设置】--》【高级】--》【环境变量】--》【在第二个内容框中找到 变量名为Path 的一行，双击】 --> 【Python安装目录追加到变值值中，用 ； 分割】
`如：原来的值;C:\python27，切记前面有分号`

### 注意：windows同时安装python3.5和python2.7，配置环境变量后输入python2提示：不是内部或外部命令，也不是可运行的程序

> - 已经配置环境变量


> - 根据python2.7的安装路径修改python.exe的名称为python2.exe


> - cmd命令直接输入python2正常


## 2.2 linux安装python2.7

下载源码，并重新编译安装：

> 
```
1. cd /usr/local/src/
2. wget http://python.org/ftp/python/2.7.8/Python-2.7.8.tgz
3. tar xf Python-2.7.8.tgz
4. cd Python-2.7.8
5. ./configure --prefix=/usr/local/python27
6. make && make install
7.  mv /usr/bin/python /usr/bin/python26 #将系统安装的重命名转移
8.  ln -s /usr/local/python27/bin/python /usr/bin/ #创建软连接直接调用2.7版本python
9.  vim /usr/bin/yum ##!/usr/bin/python2.6 修改原系统默认的python版本 以保证yum命令应用
```


## 2.3 linux安装python3.5

下载源码，并重新编译安装
下载Python3.5的源码包并编译

```
wget https://www.python.org/ftp/python/3.5.0/Python-3.5.0.tgz
tar xf Python-3.5.0.tgz
cd Python-3.5.0
./configure --prefix=/usr/local --enable-shared
make
make install
温馨提醒：如果编译遇到如下问题：
Ignoring ensurepip failure: pip 7.1.2 requires SSL/TLS
解决方法：
yum install -y openssl openssl-devel
继续：
ln -s /usr/local/bin/python3 /usr/bin/python3

在运行Python之前需要配置库：

echo /usr/local/lib >> /etc/ld.so.conf.d/local.conf
ldconfig
可以设置别名：alias py=python3方便使用
```

# 3 Hello world等程序代码编写

程序执行信息：

在windows上执行python后进入python交互器：

```python
>>> print ("Hello world!")
Hello world!
```

注意：python2的raw_input（） == python3的input（） 但是在python3中执行不带括号的会报错：

```python
>>> print "Hello world!"
File "<stdin>", line 1
print "Hello world!"
                   ^
SyntaxError: Missing parentheses in call to 'print'
```

## 3.1 小知识点

> - 退出Python交互器：exit() windows下ctrl+z 回车 linux下Ctrl+z退出


> - = 左边永远是变量名 =右边字符串的话需要添加"string" 数字可不加，运算：+-*/


> - print (“字符串”,变量) #数字不需要加引号 引用的变量不加引号


> - 在 python2 里输出中文 需要声明字符编码： #-*- coding:utf-8 -*-，python3 默认就是UTF-8字符编码


> - 一个二进制数字=1bite bit = 计算机的最小存储单位。8比特 = 1bytes 字节 1024bytes = 1kbytes KB


> - 双引号同单引号一样 ：单双引号


> - 单行和多行注释：ctrl + /


> - 多行注释：’’’ ’’’ 通三个双引号 同时表示一个段落的字符串。


> - ctrl +d 复制当前行


> - id() 可查看变量内存地址


## 3.2 变量命名规则：

### 字符编码及变量声明

> 
python2中一定要声明字符编码 #_*_coding:utf-8_*_


> 
python3中默认是utf-8


> 
name = "Stone"


> 
上述代码声明了一个变量，变量名为： name，变量name的值为："Stone"　


### 变量定义的规则：

> - 变量名只能是 字母、数字或下划线的任意组合


> - 变量名的第一个字符不能是数字


> - 以下关键字不能声明为变量名['and', 'as', 'assert', 'break', 'class', 'continue', 'def', 'del', 'elif', 'else', 'except', 'exec', 'finally', 'for', 'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'not', 'or', 'pass', 'print', 'raise', 'return', 'try', 'while', 'with', 'yield']


### 变量程序实例：

```python
name = input("name:")   定义变量
age = input("age:")
job = input("job:")
hobby = input("hobby:")

info = '''              定义输出变量
-----info of %s ----- 
Name : %s
Age  : %s
Job  : %s
Hobby: %s
----- end -----         
''' %(name,name,age,job,hobby)
print(info)             输出
```

## 3.3 用户交互

```
>>> name = input("pls input your name:")
pls input your name:stone
>>> name
'stone'
```

将input的值赋予给name变量，输入的是什么name就是什么

```
#!/usr/bin/env python
#_*_coding:utf-8_*_
#name = raw_input("What is your name?") #only on python 2.x
name = input("What is your name?")
print("Hello " + name )
```

输入密码时，如果想要不可见，需要利用getpass 模块中的 getpass方法，即：

```bash
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import getpass

# 将用户输入的内容赋值给 name 变量
pwd = getpass.getpass("请输入密码：")

# 打印输入的内容
print(pwd)
```

## 3.4 条件判断与首行缩进

### 猜年龄程序

```python
#!/usr/bin/env python       #指定解释器为python
right_age = 23              #首先定义正确的年龄
guess_age = int(input("please input your guess num:"))#将input的值定义一个变量名
if right_age == guess_age:  #进行条件判断 正确（注意判断后加":"）
    print("right")          #注意缩进统一级别缩进要相同
elif guess_age > right_age: #猜的年龄大于真实年龄
    print("please guess smaller!")
else:
    print("please guess bigger!") #猜的年龄小于真实年龄
```

### 猜年龄-循环次数限定

```python
#!/usr/bin/env python
right_age = 23      #定义正确年龄
for n in range(3):  #执行3次，没有猜对退出
    guess_age = int(input("please input your guess num:"))
    if right_age == guess_age:
        print("right")
        break       # break 猜对后退出当层循环
    elif guess_age > right_age:
        print("please guess smaller!")
    else:
        print("please guess bigger!")
else:               #猜测次数过多，退出
    print("too many times!!!")
```

### 小知识点：

> 
出现缩进错误提示：注意同一级别的代码缩进必须保持一致。

选择多行+tab 进行多行缩进

type + 变量 输出变量类型




















