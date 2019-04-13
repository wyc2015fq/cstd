
# Python 学习入门（31）—— 序列化 - 阳光岛主 - CSDN博客

2013年12月21日 22:02:40[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3527个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



Python内置marshal, cPickle等序列化库，但cPickle效率不太理想，marshal文档也说不保证版本兼容性。今天在列表中看到几个第三方库，故自己测试下：
安装第三方库 tnetstring：sudo pip install tnetstring\# 然后添加到PyDev应用库（Console 直接 import tnetstring）
测试脚本：
```python
#!/usr/bin/env python
import sys, os, time
import cPickle
import marshal
import shelve
import tnetstring
import msgpack
def get_dict():
    d = {}
    for i in xrange(500000):
        d[i] = "abcd" * 10
    return d
import time, functools
def timeit(func):
    @functools.wraps(func)
    def __do__(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        print '%s used time %ss' % (func.__name__, time.time() - start)
        return result
    return __do__
@timeit
def marshal_dump(d):
    return marshal.dumps(d)
@timeit
def marshal_load(s):
    return marshal.loads(s)
def marshal_test(d):
    s = marshal_dump(d)
    marshal_load(s)
@timeit
def cPickle_dump(d):
    return cPickle.dumps(d, cPickle.HIGHEST_PROTOCOL)
@timeit
def cPickle_load(s):
    return cPickle.loads(s)
def cPickle_test(d):
    s = cPickle_dump(d)
    cPickle_load(s)
@timeit
def tnetstring_dump(d):
    return tnetstring.dumps(d)
@timeit
def tnetstring_load(s):
    return tnetstring.loads(s)
def tnetstring_test(d):
    s = tnetstring_dump(d)
    tnetstring_load(s)
@timeit
def msgpack_dump(d):
    return msgpack.packb(d)
@timeit
def msgpack_load(s):
    return msgpack.unpackb(s)
def msgpack_test(d):
    s = msgpack_dump(d)
    msgpack_load(s)
def main():
    d = get_dict()
    marshal_test(d)
    cPickle_test(d)
    tnetstring_test(d)
    msgpack_test(d)
if __name__ == "__main__":
    main()
```
运行结果：
marshal_dump used time 0.0640780925751s
marshal_load used time 0.0637950897217s
cPickle_dump used time 0.0497329235077s
cPickle_load used time 0.0702350139618s
tnetstring_dump used time 0.0855898857117s
tnetstring_load used time 0.0689609050751s
msgpack_dump used time 0.0564160346985s
msgpack_load used time 0.0650169849396s

**结论：**
cPickle序列化采用了marshal，反序列化采用了pickle，所以速度优化了很多（订正）
marshal虽然有不兼容隐患，但如果能保证相同的Python版本，还是可以一用
tnetstring无论序列化或反序列化速度都比较中庸
msgpack反序列化最快，如果读多写少的应用场景是最佳选择。而且msgpack支持多语言。

**参考推荐：**
[pickle, cPickle 对象序列化/反序列化](http://blog.csdn.net/jgood/article/details/4564014)



