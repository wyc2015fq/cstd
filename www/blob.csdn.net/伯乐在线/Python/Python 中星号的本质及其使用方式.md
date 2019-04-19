# Python 中星号的本质及其使用方式 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Abandon_first](http://www.jobbole.com/members/wx1763043264) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Trey Hunner](https://treyhunner.com/2018/10/asterisks-in-python-what-they-are-and-how-to-use-them/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
在 Python 中有很多地方可以看到`*`和`**`。在某些情形下，无论是对于新手程序员，还是从其他很多没有完全相同操作符的编程语言迁移过来的人来说，这两个操作符都可能有点神秘。因此，我想讨论一下这些操作符的本质及其使用方式。
多年以来，`*`和`**`操作符的功能不断增强。在本文中，我将讨论目前这些操作符所有的使用方法，并指出哪些使用方法只能在目前的 Python 版本中应用。因此，如果你学习过 Python 2 中`*`和`**`的使用方法，那么我建议你至少浏览一下本文，因为 Python 3 中添加了许多`*`和`**`的新用途。
如果你是新接触 Python 不久，还不熟悉关键字参数(亦称为命名参数)，我建议你首先阅读我有关[Python中的关键字参数](https://treyhunner.com/2018/04/keyword-arguments-in-python/)的文章。
## 不属于我们讨论范围的内容
在本文中， 当我讨论`*`和`**`时，我指的是`*`和`**`*前缀* 操作符，而不是 *中缀* 操作符。
也就是说，我讲述的不是乘法和指数运算：

Python
```
>>> 2 * 5
10 
>>> 2 ** 5 
32
```
## 那么我们在讨论什么内容呢?
我们讨论的是`*`和`**`前缀运算符，即在变量前使用的`*`和`**`运算符。例如:

Python
```
>>> numbers = [2, 1, 3, 4, 7]
>>> more_numbers = [*numbers, 11, 18]
>>> print(*more_numbers, sep=', ')
2, 1, 3, 4, 7, 11, 18
```
上述代码中展示了`*`的两种用法，没有展示`**`的用法。
这其中包括：
- 使用`*`和`**`向函数传递参数
- 使用`*`和`**`捕获被传递到函数中的参数
- 使用`*`接受只包含关键字的参数
- 使用`*`在元组解包时捕获项
- 使用`*`将迭代项解压到列表/元组中
- 使用`**`将字典解压到其他字典中
即使你认为自己已经熟悉`*` 和 `**`的所有使用方法，我还是建议你查看下面的每个代码块，以确保都是你熟悉的内容。在过去的几年里，Python 核心开发人员不断地为这些操作符添加新的功能，对于使用者来说很容易忽略`*` 和 `**`‘的一些新用法。
## 星号用于将可迭代对象拆分并分别作为函数参数
当调用函数时，`*`运算符可用于将一个迭代项解压缩到函数调用中的参数中：

Python
```
>>> fruits = ['lemon', 'pear', 'watermelon', 'tomato']
>>> print(fruits[0], fruits[1], fruits[2], fruits[3])
lemon pear watermelon tomato 
>>> print(*fruits)
lemon pear watermelon tomato
```
`print(*fruits)`代码行将`fruits`列表中的所有项作为独立的参数传递给`print`函数调用，甚至不需要我们知道列表中有多少个参数。
`*`运算符在这里远不止是语法糖而已。要想用一个特定的迭代器将所有项作为独立的参数传输，若不使用`*`是不可能做到的，除非列表的长度是固定的。
下面是另一个例子：

Python
```
def transpose_list(list_of_lists):
    return [
        list(row)
        for row in zip(*list_of_lists)
    ]
```
这里我们接受一个二维列表并返回一个“转置”的二维列表。

Python
```
>>> transpose_list([[1, 4, 7], [2, 5, 8], [3, 6, 9]])
[[1, 2, 3], [4, 5, 6], [7, 8, 9]]
```
`**`操作符完成了类似的操作，只不过使用了关键字参数。`**`运算符允许我们获取键-值对字典，并在函数调用中将其解压为关键字参数。

Python
```
>>> date_info = {'year': "2020", 'month': "01", 'day': "01"} 
>>> filename = "{year}-{month}-{day}.txt".format(**date_info) 
>>> filename '2020-01-01.txt' `
```
根据我的经验，使用`**`将关键字参数解压缩到函数调用中并不常见。我最常看到它的地方是在实现继承时：对`uper()`的调用通常包括`*`和`**`。
如 Python 3.5 那样，在函数调用中，`*`和`**`都可以被多次使用。
有时，多次使用`*`会很方便：

Python
```
>>> fruits = ['lemon', 'pear', 'watermelon', 'tomato'] 
>>> numbers = [2, 1, 3, 4, 7] 
>>> print(*numbers, *fruits) 
2 1 3 4 7 lemon pear watermelon tomato `
```
多次使用`**`也可以达到相似的效果:

Python
```
>>> date_info = {'year': "2020", 'month': "01", 'day': "01"} 
>>> track_info = {'artist': "Beethoven", 'title': 'Symphony No 5'} 
>>> filename = "{year}-{month}-{day}-{artist}-{title}.txt".format( 
...     **date_info,
...     **track_info,
... ) 
>>> filename 
'2020-01-01-Beethoven-Symphony No 5.txt'
```
不过，在多次使用`**`时需要特别小心。Python 中的函数不能多次指定相同的关键字参数，因此在每个字典中与`**`一起使用的键必须能够相互区分，否则会引发异常。
## 星号用于压缩被传递到函数中的参数
在定义函数时，`*`运算符可用于捕获传递给函数的位置参数。位置参数的数量不受限制，捕获后被存储在一个元组中。

Python
```
from random import randint  
def roll(*dice):     
    return sum(randint(1, die) for die in dice)
```
这个函数接受的参数数量不受限制：

Python
```
>>> roll(20) 
18 
>>> roll(6, 6) 
9 
>>> roll(6, 6, 6) 
8
```
Python 的`print`和`zip`函数接受的位置参数数量不受限制。`*`的这种参数压缩用法，允许我们创建像`print`和`zip`一样的函数，接受任意数量的参数。
`**`运算符也有另外一个功能：我们在定义函数时，可以使用`**` 捕获传进函数的任何关键字参数到一个字典当中:

Python
```
def tag(tag_name, **attributes):
    attribute_list = [
        f'{name}="{value}"'
        for name, value in attributes.items()
    ]     
    return f"<{tag_name} {' '.join(attribute_list)}>"
```
`**` 将捕获我们传入这个函数中的任何关键字参数，并将其放入一个字典中，该字典将引用`attributes`参数。

Python
```
>>> tag('a', href="http://treyhunner.com")
'<a href="http://treyhunner.com">' 
>>> tag('img', height=20, width=40, src="face.jpg") 
'<img height="20" width="40" src="face.jpg">'
```
## 只有关键字参数的位置参数
在 Python 3 中，我们现在拥有了一种特殊的语法来接受只有关键字的函数参数。只有关键字的参数是*只能* 使用关键字语法来指定的函数参数，也就意味着不能按照位置来指定它们。
在定义函数时，为了接受只有关键字的参数，我们可以将命名参数放在`*`后：

Python
```
def get_multiple(*keys, dictionary, default=None):
    return [
        dictionary.get(key, default)
        for key in keys
    ]
```
上面的函数可以像这样使用：

Python
```
>>> fruits = {'lemon': 'yellow', 'orange': 'orange', 'tomato': 'red'} 
>>> get_multiple('lemon', 'tomato', 'squash', dictionary=fruits, default='unknown'）
['yellow', 'red', 'unknown']
```
参数`dictionary`和`default`在`*keys`后面，这意味着它们*只能* 被指定为[关键字参数](https://treyhunner.com/2018/04/keyword-arguments-inpython/)。如果我们试图按照位置来指定它们，我们会得到一个报错：

Python
```
>>> fruits = {'lemon': 'yellow', 'orange': 'orange', 'tomato': 'red'} 
>>> get_multiple('lemon', 'tomato', 'squash', fruits, 'unknown') 
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
TypeError: get_multiple() missing 1 required keyword-only argument: 'dictionary'
```
这种行为是通过 [PEP 3102](https://www.python.org/dev/peps/pep-3102/) 被引入到 Python 中的。
## 没有位置参数关键字的参数
只使用关键字参数的特性很酷，但是如果您希望只使用关键字参数而不捕获无限的位置参数呢?
Python 使用一种有点奇怪的 单独`*` 语法来实现：

Python
```
def with_previous(iterable, *, fillvalue=None):
    """Yield each iterable item along with the item before it."""     
    previous = fillvalue     
    for item in iterable:         
        yield previous, item         
        previous = item
```
这个函数接受一个`迭代器`参数，可以按照位置或名字来指定此参数（作为第一个参数），以及关键字参数`fillvalue`，这个填充值参数只使用关键字。这意味着我们可以像下面这样调用 with_previous：

Python
```
>>> list(with_previous([2, 1, 3], fillvalue=0)) 
[(0, 2), (2, 1), (1, 3)]
```
但像这样就不可以：

Python
```
>>> list(with_previous([2, 1, 3], 0))
Traceback (most recent call last):  
File "<stdin>", line 1, in <module> 
TypeError: with_previous() takes 1 positional argument but 2 were given `
```
这个函数接受两个参数，其中`fillvalue`参数*必须被指定为关键字参数*。
我通常在获取任意数量的位置参数时只使用关键字参数，但我有时使用这个`*`强制按照位置指定一个参数。
实际上，Python 的内置`sorted`函数使用了这种方法。如果你查看`sorted`的帮助信息，将看到以下信息：

Python
```
>>> help(sorted) 
Help on built-in function sorted in module builtins: 
sorted(iterable, /, *, key=None, reverse=False)     
    Return a new list containing all items from the iterable in ascending order.  
    A custom key function can be supplied to customize the sort order, and the     
    reverse flag can be set to request the result in descending order.
```
在`sorted`的官方说明中，有一个单独的`*`参数。
## 星号用于元组拆包
Python 3 还新添了一种 `*` 运算符的使用方式，它只与上面定义函数时和调用函数时`*`的使用方式相关。
现在，`*`操作符也可以用于元组拆包：

Python
```
>>> fruits = ['lemon', 'pear', 'watermelon', 'tomato'] 
>>> first, second, *remaining = fruits 
>>> remaining 
['watermelon', 'tomato'] 
>>> first, *remaining = fruits 
>>> remaining 
['pear', 'watermelon', 'tomato'] 
>>> first, *middle, last = fruits 
>>> middle 
['pear', 'watermelon']
```
如果你想知道什么情况下可以在你自己的代码中使用它，请查看我关于 [Python 中的 tuple 解包](https://treyhunner.com/2018/03/tuple-unpacking-improves-python-code-readability/) 文章中的示例。在那篇文章中，我将展示如何使用`*`操作符作为序列切片的替代方法。
通常当我教`*`的时候，我告诉大家只能在多重赋值语句中使用一个`*`表达式。实际来说这是不正确的，因为可以在嵌套解包中使用两个`*`（我在元组解包文章中讨论了嵌套解包）：

Python
```
>>> fruits = ['lemon', 'pear', 'watermelon', 'tomato'] 
>>> first, second, *remaining = fruits 
>>> remaining 
['watermelon', 'tomato'] 
>>> first, *remaining = fruits 
>>> remaining 
['pear', 'watermelon', 'tomato'] 
>>> first, *middle, last = fruits 
>>> middle 
['pear', 'watermelon']
```
但是，我从来没见过它有什么实际用处，即使你因为它看起来有点神秘而去寻找一个例子，我也并不推荐这种使用方式。
将此添加到 Python 3.0 中的 PEP 是 [PEP 3132](https://www.python.org/dev/peps/pep-3132/)，其篇幅不是很长。
## 列表文字中的星号
Python 3.5 通过 [PEP 448](https://www.python.org/dev/peps/pep-0448/) 引入了大量与`*`相关的新特性。其中最大的新特性之一是能够使用`*`将迭代器转储到新列表中。
假设你有一个函数，它以任一序列作为输入，返回一个列表，其中该序列和序列的倒序连接在了一起：

Python
```
def palindromify(sequence):   
    return list(sequence) + list(reversed(sequence))
```
此函数需要多次将序列转换为列表，以便连接列表并返回结果。在 Python 3.5 中，我们可以这样编写函数：

Python
```
def palindromify(sequence):  
    return [*sequence, *reversed(sequence)]
```
这段代码避免了一些不必要的列表调用，因此我们的代码更高效，可读性更好。
下面是另一个例子：

Python
```
def rotate_first_item(sequence):     
    return [*sequence[1:], sequence[0]]
```
该函数返回一个新列表，其中给定列表(或其他序列)中的第一项被移动到了新列表的末尾。
`*` 运算符的这种使用是将不同类型的迭代器连接在一起的好方法。`*` 运算符适用于连接任何种类的迭代器，然而 `+` 运算符只适用于类型都相同的特定序列。
除了创建列表存储迭代器以外，我们还可以将迭代器转储到新的元组或集合中：

Python
```
>>> fruits = ['lemon', 'pear', 'watermelon', 'tomato'] 
>>> (*fruits[1:], fruits[0]) 
('pear', 'watermelon', 'tomato', 'lemon') 
>>> uppercase_fruits = (f.upper() for f in fruits) 
>>> {*fruits, *uppercase_fruits}
{'lemon', 'watermelon', 'TOMATO', 'LEMON', 'PEAR', 'WATERMELON', 'tomato', 'pear'}
```
注意，上面的最后一行使用了一个列表和一个生成器，并将它们转储到一个新的集合中。在此之前，并没有一种简单的方法可以在一行代码中完成这项工作。曾经有一种方法可以做到这一点，可是并不容易被记住或发现：
## 两个星号用于字典文本
PEP 448 还通过允许将键/值对从一个字典转储到一个新字典扩展了`**`操作符的功能：

Python
```
>>> date_info = {'year': "2020", 'month': "01", 'day': "01"} 
>>> track_info = {'artist': "Beethoven", 'title': 'Symphony No 5'} 
>>> all_info = {**date_info, **track_info} 
>>> all_info
{'year': '2020', 'month': '01', 'day': '01', 'artist': 'Beethoven', 'title': 'Symphony No 5'}
```
我还写了另一篇文章：[在Python中合并字典的惯用方法](https://treyhunner.com/2016/02/howtomerdictionaries-python/)。
不过，`**`操作符不仅仅可以用于合并两个字典。
例如，我们可以在复制一个字典的同时添加一个新值：

Python
```
>>> date_info = {'year': '2020', 'month': '01', 'day': '7'} 
>>> event_info = {**date_info, 'group': "Python Meetup"} 
>>> event_info 
{'year': '2020', 'month': '01', 'day': '7', 'group': 'Python Meetup'}
```
或者在复制/合并字典的同时重写特定的值：

Python
```
>>> event_info = {'year': '2020', 'month': '01', 'day': '7', 'group': 'Python Meetup'} 
>>> new_info = {**event_info, 'day': "14"}
>>> new_info
{'year': '2020', 'month': '01', 'day': '14', 'group': 'Python Meetup'}
```
## Python 的星号非常强大
Python 的 `*` 和 `**` 运算符不仅仅是语法糖。 `*` 和 `**` 运算符允许的某些操作可以通过其他方式实现，但是往往更麻烦和更耗费资源。而且 `*` 和 `**` 运算符提供的某些特性没有替代方法实现：例如，函数在不使用 `*` 时就无法接受任意数量的位置参数。
在阅读了`*` 和 `**` 运算符的所有特性之后，您可能想知道这些奇怪操作符的名称。不幸的是，它们的名字并不简练。我听说过`*` 被称为“打包”和“拆包“运算符。我还听说过其被称为“splat”(来自 Ruby 世界)，也听说过被简单地称为“star”。
我倾向于称这些操作符为“星”和“双星”或“星星”。这种叫法并不能区分它们和它们的中缀关系(乘法和指数运算)，但是通常我们可以从上下文清楚地知道是在讨论前缀运算符还是中缀运算符。
请勿在不理解`*` 和 `**` 运算符的前提下记住它们的所有用法！这些操作符有很多用途，记住每种操作符的具体用法并不重要，重要的是了解你何时能够使用这些操作符。我建议使用这篇文章作为一个**备忘单**或者制作你自己的备忘单来帮助你在 Python 中使用解`*` 和 `**` 。
## 喜欢我的教学风格吗？
想了解更多关于 Python 的知识吗？我每周通过实时聊天分享我最喜欢的 Python 资源、回答 Python 问题。在下方注册，我将回答你提出的关于如何使 Python 代码更具有描述性、可读性和更 Python 化的**问题**。
