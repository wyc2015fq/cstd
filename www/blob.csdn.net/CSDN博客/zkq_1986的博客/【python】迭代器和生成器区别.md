# 【python】迭代器和生成器区别 - zkq_1986的博客 - CSDN博客





2017年12月13日 15:47:02[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：224








# 迭代器和生成器区别

1 什么是迭代器

任何实现了`__iter__`和`__next__()`方法的对象都是迭代器，`__iter__`返回迭代器自身，`__next__`返回容器中的下一个值，如果迭代器中没有更多元素了，则抛出StopIteration异常。


例如：


```python
class Fib(object):
    def __init__(self, max):
        super(Fib, self).__init__()
        self.max = max

    def __iter__(self):
        self.a = 0
        self.b = 1
        return self

    def __next__(self):
        fib = self.a
        if fib > self.max:
            raise StopIteration
        self.a, self.b = self.b, self.a + self.b
        return fib

# 定义一个main函数，循环遍历每一个菲波那切数
def main():
    # 20以内的数
    fib = Fib(20)
    for i in fib:
        print(i)

# 测试
if __name__ == '__main__':
    main()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28


2 什么是可迭代对象

能通过iter(.)内置方法转为迭代器的对象，都称为可迭代对象。包括字符串、列表、元组、字典以及迭代器、生成器。




3
 什么是生成器

生成器包括生成器函数和生成器表达式。生成器是一种特殊的迭代器，之所以特殊是因为它是单迭代器。所谓单迭代器就是当多个变量指向同一个生成器，这些变量共享生成器的指针。也就是说当其中一个变量通过next(*)方法遍历完所有元素后，其他变量就不能再遍历了，遍历的话，就抛出stopiteration异常。

例如：

>>> G=(c for c in 'SPAM')

        >>> I1=iter(G)

        >>> next(I1)


        'S' 


        >>> next(I1)


        'P'


        >>> I2 = iter(G)


        >>> next(I2)


        'A'              #输出的是A不是S





（1）生成器函数


       通过yield获得。

      例如：

def fun(n):
    for i in range(n):
        print("a")
        yield i**2


a=fun(4)
print(a)
print(next(a))
print(next(a))





输出：

<generator object fun at 0x0000000002D87D00>
a

0
a

1





（2）生成器表达式

       用括号表达：

     >>> （x**2 for x in range(10)）                #这就是一个生成器表达式



