# Cython入门到放弃（二） - lyx的专栏 - CSDN博客





2018年12月11日 20:25:12[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：604








上一篇文章讲了Cython的使用流程，没有具体展开讲别的，例子也很简单。今天首先使用一个官方文档上使用Cython的例子，然后抛出几个连续的小demo，看看Cython是如何一步一步加速的。

首先我们新建一个文件，primes.pyx，然后写一个cython文件：

```python
def primes(int nb_primes):
    cdef int n, i, len_p
    cdef int p[1000]
    if nb_primes > 1000:
        nb_primes = 1000

    len_p = 0  # The current number of elements in p.
    n = 2
    while len_p < nb_primes:
        # Is n prime?
        for i in p[:len_p]:
            if n % i == 0:
                break

        # If no break occurred in the loop, we have a prime.
        else:
            p[len_p] = n
            len_p += 1
        n += 1

    # Let's return the result in a python list:
    result_as_list  = [prime for prime in p[:len_p]]
    return result_as_list
```

大家可以注意到这不再是单纯的python的语法了，而是结合了c语言的特色。首先，我们知道c语言在定义函数的时候，参数是要设定类型的，上面的代码就是这样。其次，c语言是一个静态语言，所以很显然，在变量使用前我们要先定义变量的类型。我们不用去纠结代码本身，我们可以直观的看一下，这一段代码在cython中是怎么运行的。

同样的，我们写一个setup文件：

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("prime.pyx", annotate=True)
)
```

和上一次的setup文件相比，这次多了一个参数，也就是

```python
annotate=True
```

这是让cython在编译的时候把一些信息输入出来。编译完成后，我们发现除了.c文件和.so或.pyd文件之外，还有一个html文件，用浏览其打开就是：

![](https://img-blog.csdn.net/20180611212512291)



打开之后就是上面这张图这样，黄色的部分是与python交互的部分，颜色越深的地方，与python的交互越密切。显然，我们希望在整个程序的开始和结尾才交互一下，内部全部是cython编译之后的c在执行，这样理论上就会快很多。





