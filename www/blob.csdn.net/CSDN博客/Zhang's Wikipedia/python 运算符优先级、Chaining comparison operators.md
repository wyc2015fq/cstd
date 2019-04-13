
# python 运算符优先级、Chaining comparison operators - Zhang's Wikipedia - CSDN博客


2019年02月19日 21:56:53[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：137



## 1. **
`>> 2**2**3
256
>> (2**2)**3
64
>> 2**(2**3)
256`相同优先级的运算符会优先进行左结合，也即第一个操作数和第二个操作数结合，结果再与第三个操作数结合。对于 ** 是个例外。
## 2. 比较运算符与 Chaining comparison operators
`>> 1<2==2>1
True
>> 1<2==2
True
>> 2>1==2>1
False`无论是左结合还是右结合，均不适用于比较运算符（==, >、< 等）。比较运算符尤其在进行 chaining comparison operators 链式比较时，需将其拆分成独立的逻辑运算。也即，`x [cmp] y [cmp] z`等价于`(x [cmp] y) and (y [cmp] z)`。
`>> 1<2 and 2==2 and 2>1
True
>> 1<2 and 2==2
True
>> 2>1 and 1==2 and 2>1
False`
## references
[https://stackoverflow.com/questions/3328355/python-operator-precedence](https://stackoverflow.com/questions/3328355/python-operator-precedence)
[https://www.geeksforgeeks.org/chaining-comparison-operators-python/](https://www.geeksforgeeks.org/chaining-comparison-operators-python/)
[http://docs.python.org/reference/expressions.html\#comparisons](http://docs.python.org/reference/expressions.html#comparisons)

