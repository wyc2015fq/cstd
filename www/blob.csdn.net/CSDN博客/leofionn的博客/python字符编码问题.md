
# python字符编码问题 - leofionn的博客 - CSDN博客


2018年01月06日 13:01:37[leofionn](https://me.csdn.net/qq_36142114)阅读数：87个人分类：[python																](https://blog.csdn.net/qq_36142114/article/category/7385154)


抛错：编码问题：
SyntaxError: Non-UTF-8 code starting with '\xc1' in file C:\...\xxx.py on line 8, but no encoding declared; see http://python.org/dev/peps/pep-0263/ for details
在最上方添加：
\# coding=gbk
或者可以用以下方式：

```python
在最上方添加：# -*- coding: utf-8 -*-
```


