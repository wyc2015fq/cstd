# datrie中文支持不够？ - Arthur的随笔 - CSDN博客
2013年07月01日 23:51:24[largetalk](https://me.csdn.net/largetalk)阅读数：5279
要找一个trie的python包，希望性能不错，支持中文。pytrie是python实现，而且是基于hash数据结构的，先pass，找了下，发现[datrie](https://pypi.python.org/pypi/datrie/0.1.1) , 看起来是不错， C实现的 double array trie数据结构，应该可以的，用上才发现有点问题，我也不清楚是用的不对，还是其他原因。上点测试代码看看，有用过的请指点一下。
先看看英文的：
```python
#coding:utf-8
import datrie
import string
t1 = datrie.Trie(string.ascii_lowercase)
t1[u'abc'] = 1
t1[u'bcd'] = 2
t1[u'b'] = 3
assert not u'cd' in t1
assert not u'a' in t1
assert u'abc' in t1
assert not u'abcd' in t1
print t1.items()
```
输出很正常：
```
[(u'abc', 1), (u'b', 3), (u'bcd', 2)]
```
可以看出， bcd 和 b 在trie树里，cd 不会被判断在树里，符合逻辑
中文 的测试代码：
```python
t2 = datrie.Trie([u'\u4e00', u'\u9fff']) #chinese unicode range
w1 = '地位'.decode('utf-8')
w2 = '的地位'.decode('utf-8')
w3 = '极高的地位'.decode('utf-8')
w4 = '极高'.decode('utf-8')
w5 = '的'.decode('utf-8')
t2[w2] = 1
t2[w4] = 2
t2[w5] = 3
print 'w1 %s in t2 is '%w1, w1 in t2
print 'w3 %s in t2 is '%w3, w3 in t2
assert not w1 in t2
assert w2 in t2
assert not w3 in t2
print t2.items()
```
输出：
```
w1 地位 in t2 is  True
w3 极高的地位 in t2 is  False
Traceback (most recent call last):
  File "test_datrie.py", line 30, in <module>
    assert not w1 in t2
AssertionError
```
这个就有问题了，“ 的” 和 “的地位” 在trie树中， “地位” 也被判断为在trie中，这就不对了
so。 这就是我目前碰到的问题，datrie对中文支持如何？
另，如果datrie不可用，还有其他可以用的python trie数据结构吗
