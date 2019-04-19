# python with用法 - Arthur的随笔 - CSDN博客
2011年10月27日 13:28:04[largetalk](https://me.csdn.net/largetalk)阅读数：73391
python中with可以明显改进代码友好度，比如：
```python
with open('a.txt') as f:
    print f.readlines()
```
为了我们自己的类也可以使用with， 只要给这个类增加两个函数__enter__, __exit__即可：
```python
>>> class A:
	def __enter__(self):
		print 'in enter'
	def __exit__(self, e_t, e_v, t_b):
		print 'in exit'
>>> with A() as a:
	print 'in with'
in enter
in with
in exit
```
另外python库中还有一个模块contextlib，使你不用构造含有__enter__, __exit__的类就可以使用with：
```python
>>> from contextlib import contextmanager
>>> from __future__ import with_statement
>>> @contextmanager
... def context():
...     print 'entering the zone'
...     try:
...         yield
...     except Exception, e:
...         print 'with an error %s'%e
...         raise e
...     else:
...         print 'with no error'
...
>>> with context():
...     print '----in context call------'
...
entering the zone
----in context call------
with no error
```
使用的最多的就是这个contextmanager, 另外还有一个closing 用处不大
```python
from contextlib import closing
import urllib
with closing(urllib.urlopen('http://www.python.org')) as page:
    for line in page:
        print line
```
