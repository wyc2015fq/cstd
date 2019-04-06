#  			[Python进阶内容（三）--- reduce](https://www.cnblogs.com/echo1937/p/7611621.html) 		



### 描述

**functools.reduce()** 函数会对参数序列中元素进行累积。函数将一个数据集合（列表，元组等）中的所有数据进行下列操作：用传给reduce中的函数  function（有两个参数）先对集合中的第 1、2 个元素进行操作，得到的结果再与第3个数据用 function  函数运算，最后返回一个结果。

### 语法

functools.reduce() 函数语法：

```
functools.reduce(function, iterable[, initializer])
```

假设function为bin_func，iterable为seq，则functools.reduce( bin_func, seq ) 的执行流程为：

![img](https://images2017.cnblogs.com/blog/922424/201709/922424-20170929160003325-1542243387.png)

### 参数

- function -- 函数，有两个参数
- iterable -- 可迭代对象
- initializer -- 可选，初始参数

### 返回值

返回函数计算结果。

### 示例

以下示例展示了 reduce() 的使用方法：

```
def add(x, y):
    return x + y

functools.reduce(add, [1, 2, 3, 4, 5])  # 计算列表和：(((1+2)+3)+4)+5
functools.reduce(lambda x, y: x + y, [1, 2, 3, 4, 5])  # 使用 lambda 匿名函数，等价于add(x,y)
```

### 实现

functools.reduce函数的实现大致如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
def reduce(function, iterable, initializer=None):
    it = iter(iterable)
    if initializer is None:
        value = next(it)
    else:
        value = initializer
    for element in it:
        value = function(value, element)
    return value
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

可选参数initializer的默认值为None，functools.reduce函数可简化为：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
def reduce(function, iterable):
    it = iter(iterable)                     # 返回一个iterator迭代器
    value = next(it)                        # 取得迭代器it的第1个值，赋值给value

    for element in it:                      # 遍历迭代器it，取第2，3，4，...，n个元素
        value = function(value, element)    # 将value（第1个元素）和element（第2个元素）给function，并将结果赋值给value
    return value
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在掌握了reduce函数的实现原理后，initializer不为None的情况也就很好理解了 -- value的初始值为initializer，element则从迭代器的第1个值开始遍历

### 作用

filter()、map()、reduce()是Python的函数式编程的内容，最基本的用法是替代循环，也可以用来实现很多复杂功能：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
import functools


def even_filter(nums):
    return filter(lambda x: x % 2 == 0, nums)         # 选择偶数


def multiply_by_three(nums):
    return map(lambda x: x * 3, nums)                 # 乘以3


def convert_to_string(nums):
    return map(lambda x: 'The Number: %s' % x, nums)  # 打印输出


def pipeline_func(data, fns):
    return functools.reduce(lambda a, x: x(a), fns, data)


if __name__ == "__main__":

    nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    # 巧妙地利用reduce函数,实现了以下语句的功能
    # pipeline = convert_to_string(multiply_by_three(even_filter(nums)))
    pipeline = pipeline_func(nums, [even_filter, multiply_by_three, convert_to_string])

    for element in pipeline:
        print(element)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 输出结果

The Number: 6
The Number: 12
The Number: 18
The Number: 24
The Number: 30
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 拓展阅读

- [Python函数式编程lambda,filter,map,reduce的使用](http://blog.tdoly.com/coding/python-lambda-filter-map-reduce/)

 