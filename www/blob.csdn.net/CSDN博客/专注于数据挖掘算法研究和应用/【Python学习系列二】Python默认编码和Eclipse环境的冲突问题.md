# 【Python学习系列二】Python默认编码和Eclipse环境的冲突问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月06日 17:26:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2029
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、问题：在Eclipse+PyDev开发环境中写个



```python
print 'Hello,python!';
```
执行却提示：


`SyntaxError: Non-ASCII character '\xc4'`

2、原因：python的默认编码文件是用的ASCII码，eclipse将文件存成了UTF-8，所以遇到中文输出或注释时运行脚本时会提示这个错误，会提示编码错误。




3、解决：

在文件开头加一行：#coding:utf-8

代码如下：



```python
#coding:utf-8

'''
Created on 2017年6月6日

@author: Administrator
'''

print 'Hello,python!';
```





