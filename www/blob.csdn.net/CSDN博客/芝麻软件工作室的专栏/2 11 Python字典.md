
# 2.11 Python字典 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 10:07:20[seven-soft](https://me.csdn.net/softn)阅读数：150个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



字典是Python 中的映射数据类型，工作原理类似Perl 中的关联数组或者哈希表，由键-值（key-value）对构成。几乎所有类型的Python 对象都可以用作键，不过一般还是以数字或者字符串最为常用。
值可以是任意类型的Python 对象，字典元素用大括号（{ }）包裹。
如果您熟悉JSON，那么字典和JSON是类似的。
>>> aDict = {'host': 'earth'}   \# create dict
>>> aDict['port'] = 80   \# add to dict
>>> aDict
{'host': 'earth', 'port': 80}
>>> aDict.keys()
['host', 'port']
>>> aDict['host']
'earth'
>>> for key in aDict:
... print key, aDict[key]
...
host earth
port 80
这里仅作简单介绍，后续章节将进行深入探讨。

