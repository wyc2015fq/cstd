# 细数10个隐藏在Python中的彩蛋 - 知乎
# 



1Python中的“八荣八耻”

import this 中隐藏了一首《Python之禅》的诗，它是Python中的『八荣八耻』，作者是 Tim Peters ，每个有追求的Python程序员都应该谨记于心。


参考翻译：


```
>>> import this
The Zen of Python, by Tim Peters

Beautiful is better than ugly.
优美胜于丑陋。

Explicit is better than implicit.
显式胜于隐式。

Simple is better than complex.
简单胜于复杂。

Complex is better than complicated.
复杂胜于难懂。

Flat is better than nested.
扁平胜于嵌套。

Sparse is better than dense.
分散胜于密集。

Readability counts.
可读性应当被重视。

Special cases aren’t special enough to break the rules. Although practicality beats purity.
尽管实用性会打败纯粹性，特例也不能凌驾于规则之上。

Errors should never pass silently. Unless explicitly silenced.
除非明确地使其沉默，错误永远不应该默默地溜走。

In the face of ambiguity, refuse the temptation to guess.
面对不明确的定义，拒绝猜测的诱惑。

There should be one– and preferably only one –obvious way to do it.
用一种方法，最好只有一种方法来做一件事。

Although that way way not be obvious at first unless you’re Dutch.
虽然一开始这种方法并不是显而易见的，但谁叫你不是Python之父呢。

Now is better than never. Although never is often better than right now.
做比不做好，但立马去做有时还不如不做。

If the implementation is hard to explain, it’s a bad idea.
如果实现很难说明，那它是个坏想法。

If the implementation is easy to explain, it may be a good idea.
如果实现容易解释，那它有可能是个好想法。

Namespaces are one honking great idea – let’s do more of those!
命名空间是个绝妙的想法，让我们多多地使用它们吧！
```





2enumerate函数用于遍历列表中的元素以及它们的下标
![](https://pic3.zhimg.com/v2-9cbbe962c33dac6c7ecf029a99d24f0e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='196'></svg>)
3对默认实参要多加小心
![](https://pic3.zhimg.com/v2-bd51bc82553ad18e3529cd1c269fae42_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='337' height='219'></svg>)



相反，你应该使用一个标记值表示“无定义”，来替换“[]”。
![](https://pic2.zhimg.com/v2-325364143142d2d5d55faf5bafb31199_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='322' height='203'></svg>)
4对于C系的那些更喜欢括号而不是缩进的开发者：

C++、Java编程中使用花括号表示代码块，而 Python 社区却给 C++ 一类程序员开了一个玩笑，braces 翻译过来是「花括号」的意思，导入 braces 就可以使用花括号，然而，Python 社区对此的真实态度是：没门儿！


```
>>> from __future__ import braces
SyntaxError: not a chance
```


5切片操作中的tricks


```
a = [1,2,3,4,5] >>> a[::2] [1,3,5]
```


特殊的例子是x[::-1]，它可以将列表反转


```
>>> a[::-1] [5,4,3,2,1]
```


6装饰器

装饰器实现了在一个函数中调用其它函数或方法来增加功能性，从而修改参数或结果等，在函数定义前加上装饰器，只需一个“@”符号。


以下示例显示了一个print_args装饰器的用法：
![](https://pic3.zhimg.com/v2-7b26de1030c1f15c7c105d610d2d3406_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='432' height='275'></svg>)
7取参的trick




你可以用*或者**来取出列表或字典作为函数参数
![](https://pic3.zhimg.com/v2-c74d1aeef938e4882eb4e5cf446ddaf2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='327' height='190'></svg>)
8Exception else语句
![](https://pic1.zhimg.com/v2-5aec8de8fd7f5fd39813948a2d34206c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='415' height='178'></svg>)
使用“else”比在“try”语句中添加多余的代码更好，因为它避免了意外获取不被try语句保护的异常…除了声明之外。

9嵌套列表推导式和生成器表达式
![](https://pic2.zhimg.com/v2-bc4238fd594e0ae72cfb70c172ed7e41_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='88'></svg>)
这些语句可以取代大量的嵌套循环代码块




10使用re.DEBUG查看正则表达式的匹配过程

正则表达式是Python的一大特色，但是调试起来会很痛苦，很容易得出一个bug。幸运的是，Python可以打印出正则表达式的解析树，通过re.debug来显示re.compile的完整过程。
![](https://pic1.zhimg.com/v2-a412998b13bd9c9460e3d6d53e1fb45c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='629' height='577'></svg>)
一旦你理解了语法，你就可以发现你的错误。在这里我们可以看到[/font]忘了去除[]


