# Jupyter notebook使用记录 - 浅梦的博客 - CSDN博客





2017年01月08日 20:12:53[浅梦s](https://me.csdn.net/u012151283)阅读数：1278








# Jupyter notebook是什么？

notebook 是一种 Web 应用，能让用户将说明文本、数学方程、代码和可视化内容全部组合到一个易于共享的文档中。GitHub 上面也会自动提供 notebook。借助此出色的功能，你可以轻松共享工作。[http://nbviewer.jupyter.org/](http://nbviewer.jupyter.org/) 也会提供 GitHub 代码库中的 notebook 或存储在其他地方的 notebook。名称 Jupyter 由 Julia、Python 和 R 组合而成。如果有兴趣，不妨看看可用[内核](https://github.com/ipython/ipython/wiki/IPython-kernels-for-other-languages)的列表。

# 安装Jupyter notebook

到目前为止，安装 Jupyter 的最简单方法是使用 Anaconda。该发行版自动附带了 Jupyter notebook。你能够在默认环境下使用 notebook。 

要在 conda 环境中安装 Jupyter notebook，请使用 `conda install jupyter notebook`。 

也可以通过 pip 使用 `pip install jupyter notebook` 来获得 Jupyter notebook。
# 启动notebook服务器

在终端或控制台中输入`jupyter notebook`。服务器会在你运行此命令的目录中启动。 

运行此命令时，服务器主页会在浏览器中打开。默认情况下，notebook 服务器的运行地址是 `http://localhost:8888`。如果你不熟悉该地址，其含义是：`localhost` 表示你的计算机，而 `8888` 是服务器的通信端口。只要服务器仍在运行，你随时都能通过在浏览器中输入 `http://localhost:8888` 返回到服务器。 

如果启动其他服务器，新服务器会尝试使用端口 `8888`，但由于此端口已被占用，因此新服务器会在端口 `8889` 上运行。之后，可以通过 `http://localhost:8889` 连接到新服务器。每台额外的 notebook 服务器都会像这样增大端口号。 

通过在终端中按两次 Ctrl + C，可以关闭整个服务器。
# Markdown单元格

[markdown语法](https://daringfireball.net/projects/markdown/basics)

## 数学表达式

在 Markdown 单元格中，可以使用 [LaTeX](https://www.latex-project.org/) 符号创建数学表达式。notebook 使用 MathJax 将 LaTeX 符号呈现为数学符号。要启动数学模式，请在 LaTeX 符号两侧加上美元符号（例如 `$y = mx + b$`），以创建内联的数学表达式。对于数学符号块，请使用两个美元符号：

```
$$
y = \frac{a}{b+c}
$$
```

如果你没有用过 LaTeX，[请阅读这篇入门文档](http://data-blog.udacity.com/posts/2016/10/latex-primer/)，它介绍了如何使用 LaTeX 来创建数学表达式。 

在编写 Markdown 时，可以参考这个[速查指南](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)。我建议使用 Markdown 单元格，与使用一堆代码块相比，这使 notebook 变得更易于阅读。

# Magic 关键字

Magic 关键字是可以在单元格中运行的特殊命令，能让你控制 notebook 本身或执行系统调用（例如更改目录）。例如，可以使用 `%matplotlib` 将 matplotlib 设置为以交互方式在 notebook 中工作。 

Magic 命令的前面带有一个或两个百分号（`%` 或 `%%`），分别对应行 Magic 命令和单元格 Magic 命令。行 Magic 命令仅应用于编写 Magic 命令时所在的行，而单元格 Magic 命令应用于整个单元格。 
**注意**：这些 Magic 关键字是特定于普通 Python 内核的关键字。如果使用其他内核，这些关键字很有可能无效。
## 代码记时

可以使用 Magic 命令 `%timeit` 测算函数的运行时间 

如果要测算整个单元格的运行时间，请使用 `%%timeit`

## 在notebook中进行调试

对于 Python 内核，可以使用 Magic 命令 `%pdb` 开启交互式调试器。出错时，你能检查当前命名空间中的变量。 

要详细了解 `pdb`，请阅读[此文档](https://docs.python.org/3/library/pdb.html)。要退出调试器，在提示符中输入 `q` 即可。

## 其他内容

查看[此列表](http://ipython.readthedocs.io/en/stable/interactive/magics.html)，它列出了所有可用的Magic命令。

# 转换 notebook

Notebook 只是扩展名为 `.ipynb` 的大型 [JSON](http://www.json.org/) 文件。 

详细了解 nbconvert，请阅读相关详细了解 nbconvert，请阅读相关[文档](https://nbconvert.readthedocs.io/en/latest/usage.html)。






