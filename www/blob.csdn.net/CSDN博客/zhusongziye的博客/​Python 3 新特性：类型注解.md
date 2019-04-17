# ​Python 3 新特性：类型注解 - zhusongziye的博客 - CSDN博客





2018年05月17日 21:18:21[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：715








前几天有同学问到，这个写法是什么意思：


```
def add(x:int, y:int) -> int:
    return x + y
```

我们知道 Python 是一种动态语言，变量以及函数的参数是不区分类型。因此我们定义函数只需要这样写就可以了：

```
def add(x, y):
    return x + y
```

这样的好处是有极大的灵活性，但坏处就是对于别人代码，无法一眼判断出参数的类型，IDE 也无法给出正确的提示。

于是 Python 3 提供了一个新的特性：
函数注解

也就是文章开头的这个例子：

```
def add(x:int, y:int) -> int:
    return x + y
```

用`: 类型`的形式指定函数的参数类型，用`-> 类型`的形式指定函数的返回值类型。

然后特别要强调的是，Python 解释器并不会因为这些注解而提供额外的校验，没有任何的类型检查工作。也就是说，这些类型注解加不加，对你的代码来说没有任何影响：

![](https://img-blog.csdn.net/20180517211648962?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输出：

![](https://img-blog.csdn.net/20180517211700915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但这么做的好处是：
- 
让别的程序员看得更明白

- 
让 IDE 了解类型，从而提供更准确的代码提示、补全和语法检查（包括类型检查，可以看到 str 和 float 类型的参数被高亮提示）


![](https://img-blog.csdn.net/2018051721171893?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在函数的`__annotations__`属性中会有你设定的注解：

![](https://img-blog.csdn.net/2018051721173143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输出：

![](https://img-blog.csdn.net/20180517211748728?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在 Python 3.6 中，又引入了对变量类型进行注解的方法：


```
a: int = 123
b: str = 'hello'
```

更进一步，如果你需要指明一个全部由整数组成的列表：

```
from typing import List
l: List[int] = [1, 2, 3]
```

但同样，这些仅仅是“注解”，不会对代码产生任何影响。

不过，你可以通过mypy库来检验最终代码是否符合注解。

安装 mypy：
`pip install mypy`
执行代码：
`mypy test.py`
如果类型都符合，则不会有任何输出，否则就会给出类似输出：

![](https://img-blog.csdn.net/20180517211800135?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这些新特性也许你并不会在代码中使用，不过当你在别人的代码中看到时，请按照对方的约定进行赋值或调用。

当然，也不排除 Python 以后的版本把类型检查做到解释器里，谁知道呢。






