# Jupyter Notebook的三大短板，都被它补齐了 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI

在机器学习和数据科学领域，Jupyter已经家喻户晓。它把笔记、代码、图表、注释融合在一个交互式的笔记本里，还能添加各种扩展功能。可谓机器学习入门进阶研究之神器。

可是，**神器也有短板**：
- 文件是固定的JSON格式，体积还非常大；
- 要在浏览器里编辑文档好烦，好怀念自己喜欢的编辑器；
- 版本控制好艰难，想用标准的合并工具来协作，可是人家只支持文本！

最近推出的一个工具，帮Jupyter Notebook把这些短板补齐了。

这个工具叫**Jupytext**，顾名思义，能够把Jupyter Notebook的内容转化成纯文本，用你最顺手的IDE打开。
![](https://pic1.zhimg.com/v2-7997e1a659246283521bfc7441317558_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='527'></svg>)
于是，编辑协作版本控制等一系列问题，迎刃而解。

Jupytext现在支持的语言还不能覆盖Jupyter Notebook的全部，但搞定了最流行的几种。它支持Jupyter Notebook和下面这些格式之间的相互转换：

Julia脚本（.jl）、Python脚本（.py）、R脚本（.R）、Markdown文件（.md）、R Markdown文件（.Rmd）。

Jupytext作者Marc Wouts在一篇博客里展示了怎样借助这个工具在IDE或者编辑器里来编辑Jupyter Notebook、进行版本控制。
![](https://pic4.zhimg.com/v2-137acb17c32019aa7401677a5624aae3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='293'></svg>)
上面就是Jupytext使用过程的一个例子：把一个Jupyter Notebook存储为.ipynb和.py两种文件格式、用PyCharm打开.py脚本来修改代码、保存之后刷新浏览器，就能在Jupyter Notebook里看到新结果了。

想要在一大堆代码和笔记里查找内容、编辑、执行cell、debug……用IDE比用Jupyter Notebook要方便得多。
![](https://pic3.zhimg.com/v2-a3ad7451ec8fa0f3de21cdca7424d51a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='415'></svg>)
上图则是对文本执行版本检查，然后将有意义的差异合并的过程。

看起来不错，怎么安装呢？

这个工具支持pypi.安装，代码是这样的：


```
upytext from pip
pip install jupytext --upgrade
# Append this to .jupyter/jupyter_notebook_config.py c.NotebookApp.contents_manager_class="jupytext.TextFileContentsManager"
# And restart your notebook server
jupyter notebook
```


在Jupyter Notebook的元数据里输入**jupytext_formats**，就能激活笔记本和文本文件的配对，实现在IDE里编辑脚本，Notebook里也跟着更新的效果。

如果想在编辑文本文件的时候还开着Jupyter，可以在一个单元格里运行**%autosave 0**，关掉Jupyter的自动保存功能。
![](https://pic3.zhimg.com/v2-d74fdaef385f95e41e3640728893083e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='583'></svg>)



最后，附上Jupytext的传送门：

[https://github.com/mwouts/jupytext](https://link.zhihu.com/?target=https%3A//github.com/mwouts/jupytext)

— **完** —


