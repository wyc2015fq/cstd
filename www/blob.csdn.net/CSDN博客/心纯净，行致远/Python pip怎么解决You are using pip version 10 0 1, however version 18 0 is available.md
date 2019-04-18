# Python pip怎么解决You are using pip version 10.0.1, however version 18.0 is available. - 心纯净，行致远 - CSDN博客





2019年02月24日 17:01:56[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：48标签：[Python																[pip](https://so.csdn.net/so/search/s.do?q=pip&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[【Python】](https://blog.csdn.net/zhanshen112/article/category/7930576)








上述问题是pip下载器版本不对，解决需要分两步：

### 1 检查有无pip

在cmd环境下输入pip或者pip --version可以看到有无pip及其版本

### 2 升级

在cmd环境下输入python -m pip install --upgrade pip即可实现升级。注意最好在校园网环境下，因为感觉需要翻墙，否则会因为网速太慢终止。




