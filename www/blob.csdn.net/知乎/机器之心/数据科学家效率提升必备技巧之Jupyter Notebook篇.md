# 数据科学家效率提升必备技巧之Jupyter Notebook篇 - 知乎
# 



选自codeburst

**作者：Nok**

**机器之心编译**

**参与：黄小天、刘晓坤**

> 本文作者参加过 fast.ai 的深度学习课程，了解到很多适用于一般软件工程的技巧，所以写作本文梳理所学，并共享给大家。




**1. Jupyter Notebook 扩展**

标准的 Jupyter Notebook 很不错，但还有更多的扩展，其中集成了大量的函数。

**安装 Jupyter 扩展包**


```python
# Install Jupyterextension package
pip install jupyter_contrib_nbextensions
jupyter contrib nbextension install—user
# Install configurator
pip install jupyter_nbextensions_configurator
# Install theme
pip install jupyterthemes
## Change theme (This is my default)
''' Note that you need to use 1 command to configure the setting, if you do 2 jt command, the second one will replace the first one.
'''
jt -t grade3 -T
```


你可以在这里找到更多的 Jupyter theme：[https://github.com/dunovank/jupyter-themes](https://link.zhihu.com/?target=https%3A//github.com/dunovank/jupyter-themes)

在安装了 Configurator 之后，你可以看到一个新的「Nbextensions」标签。将这几项打钩。

1. Autopep8

2. Collapsible Headings

3. Gist-it
![](https://pic2.zhimg.com/v2-80b8ccef6d39a5c7a1accfa1590aea79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1026' height='547'></svg>)



**A. Collapsible Headings**

现在，你可以将 notebook 里的长代码折叠，而不用再辛苦地滚动浏览。根据我的经验，当进行探索性数据分析和画图表的时候，我需要写很长的代码，经常需要滚动查询很久才能找到我想查看的代码。现在你可以自由选择将代码折叠或展开。并且我认为你甚至可以做一个目录（我尚未尝试这种扩展）。
![](https://pic2.zhimg.com/v2-33f4f483f3aeb3b5fa4f1b804faedde9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='991' height='199'></svg>)
**B. Gist-it**
![](https://pic2.zhimg.com/v2-cb2462b4bd9a26414fdb56ecd38440f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='872' height='89'></svg>)
你可以看到上图位置中小小的 GitHub 图标，只需要点击它，就能发表你的 Gist。

Gist 是一个可分享 notebook 的地方，你可以在里面分享你遇到的 bug 和其它技术难题。

其默认发表的是匿名 Gist，如果你想要用你的 GitHub 账号发表，需要生成身份验证的标记。两者的主要区别在于，当你用自己的账号发表时，可以对你的 Gist 进行编辑。

这是我为这篇文章写的 notebook：[https://gist.github.com/noklam/a0d020c17ce1715bf1d031b1cb8a9fa4](https://link.zhihu.com/?target=https%3A//gist.github.com/noklam/a0d020c17ce1715bf1d031b1cb8a9fa4)




**C. Autopep8**
![](https://pic2.zhimg.com/v2-9817e1dcfb7990eac6df851e794a5721_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='872' height='85'></svg>)
你可以用这个按钮或快捷键，up to you！使用这个按钮可以帮你写入所有的空格。PEP 8 是 Python 代码的风格设计指南。

PEP 8：[https://www.python.org/dev/peps/pep-0008/](https://link.zhihu.com/?target=https%3A//www.python.org/dev/peps/pep-0008/)




**2. 为你的任务计时**

我通常会在执行循环命令之前声明开始时间，然后用结束时间减去它以得到运行时间。这没问题，但其实可以更简单。使用内建的魔术命令（magic command）。它们可能看起来很不自然，但很好用（魔术命令以% 起始）。

以一个简单的函数为例，它计算的是小于 n 的最后一个斐波那契数。
![](https://pic2.zhimg.com/v2-3b36118c843461d9222c4307719638f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='987' height='361'></svg>)
你可以使用%time 为单次运行计时，或用%timeit 进行多次计时，然后得到平均值和标准差。因此这对于简单的函数很有用。那对于调用其它函数的函数，情况如何呢？

你可以使用%prun，我创建了一个哑函数（dummy function），可以多次调用 fib1()。你可以看到该循环过程大多数时间消耗在 fib1() 上。
![](https://pic1.zhimg.com/v2-835bcaef3941500e431bc1077438bef4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='993' height='445'></svg>)



**3. Cython**

Cython 是一个工具包，可以使你在 Python 中编译 C 语言，这就是为什么 numpy 和 pandas 很快的原因。确保你已安装 Cython：

pip install cython

你可以不改变任何代码而获得双倍的性能。这很棒，但一点也不惊奇。
![](https://pic1.zhimg.com/v2-e3c158ce0aaf837601905cbaabcc4c9c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='993' height='460'></svg>)
如果你稍微改变脚本，看看你可以获得什么。如果你有 C 语言编程经验，你很可能知道当我们声明一个变量时，我们需要定义一个数据类型。脚本确实改变了一些，因为像这样的操作对 Python 来说是唯一的，C 语言并不具备这样的功能。因为我们需要分配一个临时变量以存储这个值。

a,b = b,a
![](https://pic2.zhimg.com/v2-844d4aa549c7bbe504a7011e2877171d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='999' height='386'></svg>)
*（感谢 James Martini 之前指出了 fib3 中的一些错误）*

从 582 ns 到 48 ns，快了 10 倍，实际上你并不需要改变太多脚本。我感到很兴奋，因为大多数时间慢代码对你来说是 okay 的。你真正关心的是一次又一次被调用的代码。通过%prun 和一些 Cython 代码，你可以获得 C 语言的运行速度而无需编译任何文件。

除了魔术命令，我发现 Jupyter 之中的 shell 命令也很有帮助。（魔术命令以% 开始，shell 命令以! 开始）
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)![](https://pic3.zhimg.com/v2-e80497146f664bcd74f1dabeb215ac06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1025' height='142'></svg>)
*原文链接：[https://codeburst.io/jupyter-notebook-tricks-for-data-science-that-enhance-your-efficiency-95f98d3adee4](https://link.zhihu.com/?target=https%3A//codeburst.io/jupyter-notebook-tricks-for-data-science-that-enhance-your-efficiency-95f98d3adee4)*




**本文为机器之心编译，转载请联系本公众号获得授权。**


