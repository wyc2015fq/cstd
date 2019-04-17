# Python单元测试(doctest和unittest) - 浅梦的博客 - CSDN博客





2017年08月23日 21:30:38[浅梦s](https://me.csdn.net/u012151283)阅读数：2543标签：[Python																[单元测试](https://so.csdn.net/so/search/s.do?q=单元测试&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python																[单元测试](https://blog.csdn.net/u012151283/article/category/7123381)](https://blog.csdn.net/u012151283/article/category/6669030)







# docstring

```python
def collect_vowels(s):
    """ (str) -> str

    Return the vowels (a, e, i, o, and u) from s.

    >>> collect_vowels('Happy Anniversary!')
    'aAiea'
    >>> collect_vowels('xyz')
    ''
    """

    vowels = ''
    for char in s:
        if char in 'aeiouAEIOU':
            vowels = vowels + char
    return vowels
```

通常我们可以在Python函数的docstring总编写一些测试用例其格式如下:

```bash
>>>func(param1)
return_value1
>>>func(param2)
return_value2
```

# doctest

使用Python的doctest库，可以一次性运行函数中所有的测试用例。 

直接在交互式会话中

```python
>>> import doctest
>>> doctest.testmod()
TestResults(failed=0, attempted=2)
```

或在模块末尾中加入:

```java
if __name__=='__main__':
    import doctest
    doctest.testmod()
```

这样，每次在调用模块时都会先进行单元测试。

# unittest

Python的unittest模块提供了类似`doctest`的测试框架。但是和`doctest`不同，`unittest`可以和待测试的函数分开编写。 

如下的`doctest`可以被翻译成对应`unittest`

```python
def get_divisors(num, possible_divisors):
    ''' (int, list of int) -> list of int

    Return a list of the values from possible_divisors
    that are divisors of num.

    >>> get_divisors(8, [1, 2, 3])
    [1, 2]
    >>> get_divisors(4, [-2, 0, 2])
    [-2, 2]
    '''

    divisors = []
    for item in possible_divisors:
        if item != 0 and num % item == 0:
            if num % item == 0:
                divisors.append(item)
    return divisors
```

对应的`unittest`如下

```python
import unittest
import divisors

class TestDivisors(unittest.TestCase):
    """Example unittest test methods for get_divisors."""

    def test_divisors_example_1(self):
        """Test get_divisors with 8 and [1, 2, 3]."""

        actual = divisors.get_divisors(8, [1, 2, 3])
        expected = [1, 2]
        self.assertEqual(expected, actual)  

    def test_divisors_example_2(self):  
        """Test get_divisors with 4 and [-2, 0, 2]."""

        actual = divisors.get_divisors(4, [-2, 0, 2])
        expected = [-2, 2]
        self.assertEqual(expected, actual)
```

调用`unittest.main()`，会检查当前模块内所有`TestCase`的子类，然后带用以”test”开头的方法，报告不符合预期的结果。 

当在IDLE中调用`unittest`时，形参`exit`应当被赋值为`False`:`unittest.main(exit=False)`

一般地，每一个待测试的函数有一个`TestCase`子类，每一次函数调用对应一个test方法。
# 选择测试用例

一般在选择测试用例时，考虑一下因素：
- 大小 

对于集合(string,list,tuple,dict) 
- 空集合
- 单元素集
- 最少数量用例
- 较多用例
- 对分 
- 奇数/偶数
- 正/负
- 空/满

- 边界 

函数在接近阈值的时候处理不同，测试阈值。
- 顺序 

函数对不同顺序的输入值，测试不同的顺序。 

通常不同的种类会有重叠，所以一个测试用例可能归属于不止一个类别。







