# Python-字符串及列表操作-Day2 - lincappu - 博客园







# [Python-字符串及列表操作-Day2](https://www.cnblogs.com/lincappu/p/8157504.html)





## 1.数据类型

### 1.1 变量引出数据类型

变量：用来记录状态
变量值的变化就是状态的变化，程序运行的本质就是来处理一系列的变化

### 1.2 五大基本数据类型：
- 数字
- 字符串
- 列表
- 元组
- 字典

#### 1.2.1 数字

```cpp
二进制：0101010     #对应的调用bin()
    八进制：1-7         #对应的调用oct()   
    十六进制：0-9abcdef #对应的调用hex()
```
- 整型
- 长整型
- 浮点
- 布尔
- 复数

##### 1.2.2 整型

> 
> 
内置函数是：int()
int(a,base=b)
a是变量或者是"a(未定义变量)"
base=b b告诉计算机以什么进制进行运算
一切皆对象
age = 10 ---> int(10)--->*init* --->调用
以上说明所有的变量最终都是通过对象来生成创建的
python3中int()已经没有区分整型与长整型



#### 1.2.3 布尔

```python
True 和 False
1和0
```

#### 1.2.4 浮点float

Python的浮点数就是数学中的小数

#### 1.2.5 复数

复数由实数部分和虚数部分组成，一般形式为x＋yj，其中的x是复数的实数部分，y是复数的虚数部分，这里的x和y都是实数。
注意，虚数部分的字母j大小写都可以，

> 
> 
> 
1.3 + 2.5j == 1.3 + 2.5J
True




### 1.3 字符串

字符串的定义：
msg="hello world"

#### 1.3.1 字符串模块方法1：

```
#首字母大写:
print(msg.capitalize())

#按照字符串是20个 居中显示
print(msg.center(20,'*'))

#字符串中包含所选字符的计数 模式=（'字符',start=1,end=2） 
print(msg.count('l',1,3))
其中包含所选的字符是通过start和end字符位置确定的

#按照字符提取，[]内数字代表字符串的字符下标
print(msg[4])

#判断一个字符串是否是以'd'结尾的
print(msg.endswith('d'))

#指定空格的个数 expandtabs只是针对tab键或者空格键
msg1='a\tb'#\t
print(msg1)
print(msg1.expandtabs(10))

#找到对应字符的索引
print(msg.find('d'))

#没有找到就直接输出-1
print(msg.find('z'))
print(msg.find('d',1,11))

print('{0},{1}'.format('name','age'))
print('{name}'.format(name = "stone"))
```

#### 1.3.2 字符串模块方法2：

```
index
索引：查看指定字符的索引

isalnum：  
判断字符都是数字

msg3='123a'   
isalpha：  
字符都是字母

isdecimal:  
判断是否是十进制数字

## isdigit(): ## 
判断是否是整型数字

isnumeric：  
判断是不是数字  

isidentifier:  
判断字符串内是否包含关键字;

islower():  
判断字符串是否是小写 

isspace():  
判断字符串是否是空格

istitle():   
是否是标题 单词首字母必须是大写的 其他的字符不能是大写的，

isupper():  
是否都是大写

lhust():   
左对齐  
print(msg.ljust(10,'*'))

rjust():  
右对齐

lower():  
将所有的大写转换成小写

upper():  
将所有的小写转换成大写

lstrip():  
左边去除空格

rstrip():  
去除右边的空格

maketrans():  
将字符串进行替换 替换的字符串需要跟等长

yfill():  
从右侧填充
```

#### 1.3.3 字符串常用的操作
- 移除空白 strip()：去除空白
- 分割 split()
- 长度 len()
- 索引 msg[3]:字符索引
- 切片 msg[0:3]:字符串切片 msg[2:7:2]:按步长取

## 2. 运算符

### 2.1 算数运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011085945461-141241132.png)

### 2.2 比较运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090004352-1793064865.png)

### 2.3 赋值运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090015164-795424787.png)

### 2.4 位运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090217446-1562320241.png)
注： ~ 举例： ~5 = -6 解释： 将二进制数+1之后乘以-1，即~x = -(x+1)，-(101 + 1) = -110

按位反转仅能用在数字前面。所以写成 3+~5 可以得到结果-3，写成3~5就出错了

### 2.5 逻辑运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090025071-1236899190.png)

#### and注解：

```python
在Python 中，and 和 or 执行布尔逻辑演算，如你所期待的一样，
但是它们并不返回布尔值；而是，返回它们实际进行比较的值之一。  
在布尔上下文中从左到右演算表达式的值，如果布尔上下文中的所有
值都为真，那么 and 返回最后一个值。  
如果布尔上下文中的某个值为假，则 and 返回第一个假值
```

#### or注解：

```python
使用 or 时，在布尔上下文中从左到右演算值，就像 and 一样。
如果有一个值为真，or 立刻返回该值
如果所有的值都为假，or 返回最后一个假值
注意 or 在布尔上下文中会一直进行表达式演算直到找到第一个
真值，然后就会忽略剩余的比较值
```

#### and-or结合使用：

```python
结合了前面的两种语法，推理即可。
为加强程序可读性，最好与括号连用，例如：

（1 and 'x') or 'y'
```

### 2.6 成员运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090033196-468321512.png)

### 2.7 身份运算

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011090232368-83323653.png)

## 3. for循环

#### 写程序最忌讳的就是重复代码！！！

循环10次：

```
for i in range(10):
    print(i)
run执行输出：0 1 2 3 4 5 6 7 8 9 0
```

如果不想从0开始执行：

```
for i in range(1,10):
    print(i)   
run 执行结果：1 2 3 4 5 .. 9
range（1,10）：表示从1开始一直到9；
range（2，7）：表示从2开始一直到6；
```

### 3.1 猜年龄程序：

```python
#!/usr/bin/env python
right_age = 60
for i in  range(3):
    age = int(input("print you guess age:"))
    if age == right_age:
        print("guess right!")
        break
    elif age > right_age:
        print("guess smaller!")
    else:
        print("guess bigger!")
else: #如果for循环正常结束，就执行下面的语句
    print("too many times!!!") # == exit("too many attempts")
```

### 3.2 for循环else语法测试：

```
for i  in range(10):
    print(i)
    if i == 5:
        print("5退出！")
        break
else:
    print("循环正常退出！")

执行结果
```

### 3.3 for循环-嵌套循环：

```python
for i in range(10):
    print(i)
    for j in range(10):
        if j < 6:
            #break # 跳出当前层循环
            continue #跳出当次循环，继续下一次循环
        print(i,j)
```

### 3.4 while 循环

死循环
判断年龄，使用while+条件执行循环。

```python
#!/usr/bin/env python
age=56
count = 0      # 计数
while count<3: # 判断执行三次跳出loop
    guess = input("guess age:")
    if guess.isdigit():
        guess = int(guess)
    else:
        continue
    if guess == 56:
        print("OK")
        break
    elif guess > age:
        print("bigger")
    else:
        print("smaller")
    count += 1
```

## 4. 列表

### 4.1 列表功能

###### 定义一个列表：names = []

###### 或者name = list(1,2,4)

###### names = []最终调用的就是list这个类

#### 4.1.1 增 append

##### names.append()

```
例子：names.append("zheng")  
print(names)  
names.insert(2,"app")在指定的index之前插入
```

#### 4.1.2 删 del pop() remove()

###### 删除的三种方式：

###### names.remove()

###### names.pop()

###### del names[index]

```
names.remove("app"):删除找到的第一个key  
del names[3]:直接指定索引删除  
names.pop(3):指定索引删除，不指定索引直接删除最后一个。
```

#### 4.1.3 改

```
names[1] = "wang" 直接指定索引替换成想要的  
print(names)
```

#### 4.1.4 查

```
print(name[-2])        #直接指定索引取出  
print(name[0::2])       #按照步长取值  
print(name[-3:])        #取最后三个元素  
print(name[:3])         #取前三个元素  
print(name.index("wang"))#取元素的下标
```

#### 4.1.5 计数 count()
`print("count",names.count("wang"))# 获取  `
#### 4.1.6 清空 clear()

```cpp
name.clear()            #清空列表
```

#### 4.1.7 扩展合并 extend()

```
name.extend("liang")    #作用是两个列表的合并  
name1=["hello","nihao"]  
name2=["ca","haha"]  
name1.extend(name2)     #列表扩展  
name1  
['hello', 'nihao', 'ca', 'haha']
```

#### 4.1.8 排序 sort()

###### name.sort() #排序

#### 4.1.9 反转 reverse()

```
name.reverse()          #反转  
python3中列表中必须统一 元素中字符串与数字不能混用
```

#### 4.1.10 copy()

###### copy语法重新制定内存与原内容内存id存储位置不同

#### 4.1.11 列表的长度len()

```
name = [11,22,33]
print(len(name))
3
```

#### 4.3 循环一个列表

```
for i in names:
    print(i)
enunerate(name):
```

#### 4.4 列表的判断

```
len(a)
# 判断一个列表的长度
val >=0 and val <len(a)
```

#### 4.5 数据类型转换内置函数汇总

![](https://images2015.cnblogs.com/blog/1036857/201610/1036857-20161011101326368-808569007.png)

## 5 Python整理-小记

### 5.1 执行脚本传入参数

Python有大量的模块，从而使得开发Python程序非常简洁。类库有包括三中：

```
Python内部提供的模块
业内开源的模块
程序员自己开发的模块
```

Python内部提供一个 sys 的模块，其中的 sys.argv 用来捕获执行执行python脚本时传入的参数

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import sys
 
print sys.argv
```



### 5.2 sys os 模块小记

Python的强大之处在于他有非常丰富和强大的标准库和第三方库，几乎你想实现的任何功能都有相应的Python库支持，以后的课程中会深入讲解常用到的各种库，现在，我们先来象征性的学2个简单的。

#### 5.2.1 sys 模块

```python
sys 
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
print(sys.argv)

#输出
$ python test.py helo world
['test.py', 'helo', 'world']  #把执行脚本时传递的参数获取到了
```

#### 5.2.2 os模块

```python
os
#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import os
 
os.system("df -h") #调用系统命令
```

#### 5.2.3 完全结合一下　　

```
import os,sys
 
os.system(''.join(sys.argv[1:])) #把用户的输入的参数当作一条命令交给os.system来执行
```












