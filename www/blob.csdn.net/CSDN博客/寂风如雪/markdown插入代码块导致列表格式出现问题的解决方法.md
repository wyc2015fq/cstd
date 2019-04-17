# markdown插入代码块导致列表格式出现问题的解决方法 - 寂风如雪 - CSDN博客





2018年07月27日 10:31:50[寂风如雪](https://me.csdn.net/qq_41679006)阅读数：1763








在列表中插入代码块，代码块总是顶格的。并且导致代码块之后的文本也是顶格的。（也就是代码块的插入导致了列表的结束。） 

譬如这样写：

![有问题](https://img-blog.csdn.net/20180727102542314?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

效果是这样的：
- 试试 

这里没问题

```python
print(a)
```

这就有问题

应对方法是在代码块开头的三个反引号前加两个空格：

![解决方法](https://img-blog.csdn.net/20180727102954707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjc5MDA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

效果如下：
- 
试试 

这没问题。

```python
print (a)
```

这也没问题。




