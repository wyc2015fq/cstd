
# 2.3 Python字符串 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 10:01:57[seven-soft](https://me.csdn.net/softn)阅读数：171


字符串是字符的一个序列，字符串通常只是一串单词。
在你写的每一个Python程序中都要使用字符串，因此要注意以下部分。
## 单引号
你可以使用单引号例如'Quote me on this'指定字符串。所有的空白，例如空格和制表符都按原样保留。
## 双引号
在双引号中的字符串和在单引号中的字符串工作起来完全一样。例如"What's your name?"
## 三重引号
您可以使用三重引号-("""或''')指定多行字符串。在三重引号中您可以自由使用单引号和双引号。例如：'''This is a multi-line string. This is the first line.
This is the second line.
"What's your name?," I asked.
He said "Bond, James Bond."
'''

## 字符串是不可改变的
这意味着，一旦您已经创建了一个字符串，你就不能改变它。虽然这看起来似乎是一件坏事，但它真不是(坏事)。在我们后面看到的各种程序中，将会明白这不是一个限制。
C/C++程序员要注意，在Python中没有单独的“char”(字符型)数据。这里没有真正的需要它，我相信你不会错过它。
Perl/PHP程序员要注意，单引号字符串和双引号字符串是相同的——他们不以任何方式不同。
## 格式方法
有时，我们可能想要从其它信息构建字符串。这就是“format()”方法有用之处。
保存下面几行到文件"str_format.py"中：age=20
name='Swaroop'
print('{0} was {1} years old when he wrote this book'.format(name,age))
print('Why is {0} playing with that python?'.format(name))

输出：
$ python3 str_format.py
Swaroop was 20 years old when he wrote this book
Why is Swaroop playing with that python?
它是如何工作的？
一个字符串可以使用特定的格式，随后调用format方法，用format方法替代那些使用适当参数的格式。
观察使用第一处，我们使用“{0}”对应于变量‘name’,这是format(格式)方法的第一个参数。类似的,第二个格式是“{1}”对应的“age”,这是格式方法的第二个参数。注意，Python从0开始计数，这意味着第一位置的索引是0，第二个位置的索引是1，等等。
注意，我们可以使用字符串的连接，name+'is'+str(age)+'years old' 实现同样的目的，但这非常讨厌、容易出错。第二，在这种情况下，通过format方法自动转换为字符串，而不是显式地转换为需要的字符串。第三，当使用的format 方法，我们可以改变消息，而无需处理使用的变量，反之亦然。
还要注意，这些数字(索引)都是可选的，所以你也可以写成：age=20
name='Swaroop'
print('{} was {} years old when he wrote this book'.format(name,age))
print('Why is {} playing with that python?'.format(name))

这将给与前面的程序相同的输出。
Python在format方法中做的是，用每个参数值替代规格的地方。这里有更详细的规格，如：[复制](http://c.biancheng.net/cpp/html/1809.html#)[纯文本](http://c.biancheng.net/cpp/html/1809.html#)[新窗口](http://c.biancheng.net/cpp/html/1809.html#)
decimal(.)precision of3forfloat'0.333'
>>>'{0:.3}'.format(1/3)
fill withunderscores(_)with the text centered
(^)to11width'___hello___'
>>>'{0:_^11}'.format('hello')
keyword-based'Swaroop wrote A Byte of Python'
>>>'{name} wrote {book}'.format(name='Swaroop',book='A Byte of Python')


