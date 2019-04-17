# Python出现'ascii' codec can't encode characters...的解决方法 - Machine Learning with Peppa - CSDN博客





2018年04月12日 21:21:41[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：8851标签：[Python																[数据挖掘																[BUG																[编码](https://so.csdn.net/so/search/s.do?q=编码&t=blog)
个人分类：[编程之美：Python](https://blog.csdn.net/qq_39521554/article/category/7392111)





今天做新闻文本挖掘的时候导入某个文件，代码运行后提示

UnicodeEncodeError: 'ascii' codec can't encode characters in position 0-22: ordinal not in range(128)


以前没怎么见过这个错误，索性上网查了查，发现一个简单的方法，把这个好方法贴出来分享

第一步，别忘了给顶行加上：

```python
# -*- coding: utf-8 -*-
```

第二部，重新载入SYS模块并设置uft-8

```python
import sys
reload(sys) 
sys.setdefaultencoding('utf-8')
```](https://so.csdn.net/so/search/s.do?q=BUG&t=blog)](https://so.csdn.net/so/search/s.do?q=数据挖掘&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




