# vs_code_002_VS Code中乱码问题 - 小灰笔记 - CSDN博客





2018年09月29日 22:23:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：179








       尝试使用一阵子VS Code，查看代码的时候发现现在的手头的一些代码出现乱码。后来，使用其他编辑器打开之后发现乱码都是中文。

       乱码的形式如下：

![](https://img-blog.csdn.net/20180929222310448?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



       其实，这是编码显示问题。编码显示方式可以在右下角看到：

![](https://img-blog.csdn.net/20180929222310431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



       针对我自己的代码，出现问题的原因是我原始的代码编码格式是GB2312。修改之后：

![](https://img-blog.csdn.net/20180929222310465?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180929222310438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





