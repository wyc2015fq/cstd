# deploy django to pypy - Arthur的随笔 - CSDN博客
2013年01月09日 15:55:09[largetalk](https://me.csdn.net/largetalk)阅读数：3242
# tips: 本文档原用sphinx书写，是直接copy到博客来的。
# 4. PyPy部署
## 4.1 PyPy安装[](#id2)
[PyPy](http://pypy.org/) 是一个python语言实现的带jit的python解释器，现支持X86, X86_64, ARM等平台，最新版本是2.0beta1,
 但在服务器上安装64位版本时都存在安装问题，所以最后选择了1.9。
从官网 [下载PyPy](http://pypy.org/download.html) 相应版本，解压后 cd pypy/bin; pypy 就能进入pypy命令解释器。
sample:
> 
wget [https://bitbucket.org/pypy/pypy/downloads/pypy-1.9-linux64.tar.bz2](https://bitbucket.org/pypy/pypy/downloads/pypy-1.9-linux64.tar.bz2)
tar xvf pypy-1.9-linux64.tar.bz2 -C /opt
cd /usr/local/bin
ln -s /opt/pypy-1.9/bin/pypy pypy
curl -O [http://python-distribute.org/distribute_setup.py](http://python-distribute.org/distribute_setup.py)
curl -O [https://raw.github.com/pypa/pip/master/contrib/get-pip.py](https://raw.github.com/pypa/pip/master/contrib/get-pip.py)
pypy distribute_setup.py
pypy get-pip.py
/opt/pypy-1.9/bin/pip install pygments # for example
不过更好的做法是用virtualenv 来构建PyPy运行环境，从而不干扰其他python环境, 而且pip也自动安装好了
> 
下载并解压PyPy后，
virtualenv -p /path/to/bin/pypy my-pypy-env
source my-pypy-env/bin/active
pip install django #for example
## 4.2 Package安装[](#package)
目前 [PyPy兼容性](http://pypy.org/compat.html) 还行， 对于 [PyPy性能](http://pypy.org/performance.html) 也有一些说明,
 对于 [第三方库兼容](https://bitbucket.org/pypy/compatibility/wiki/Home) 有一个wiki页面。
目前可以直接安装（不一定兼容）的库有：
> 
pip install django flask tornado gunicorn sqlalchemy
pip pillow #PIL
pip BeautifulSoup south pytidylib simplejson
[pillow](http://pypi.python.org/pypi/Pillow) :PIL fork
MySQL-Python([MySQLdb](http://sourceforge.net/projects/mysql-python/)) 需要打一个 [小patch](https://bitbucket.org/pypy/compatibility/wiki/mysql-python) ,
 他的patch是基于mysqldb 1.2.3c1 修改的，不过应用在1.2.4b4 上也可以。
> 
按照patch修改MySQLdb代码
python setup.py build
python setup.py install
现在python manage.py runserver应该就可以运行起来了
## 4.3 PyPy Bugs(持续更新)[](#pypy-bugs)
[Issue1116](https://bugs.pypy.org/issue1116) : AttributeError: ‘GzipFile’ object has no
 attribute ‘fileobj’ with Django
fix 方法(未确定): add the following line at the end of manage.py the error goes away:
> 
import gc; gc.collect(); gc.collect()
后记：费了九牛二虎之力把我原来程序deploy到pypy上了，可是pypy现在还不成熟阿，结果有些令人失望， 原来只需要60ms的一个页面现在却需要300ms了。其实我是做过基准测试的，pypy完胜阿，但**Benchmarks are just lies damn lies**
用profile看了下，发现两个很影响性能的地方： deepcopy和pickle
```python
import cProfile as profile
from pstats import Stats
from timeit import timeit
from copy import copy, deepcopy
import cPickle as pickle
lst = [[str(x)] for x in  range(100)]
def f():
    def inner():
        x = copy(lst)
        y = deepcopy(lst)
    timeit(inner, number=100)
def p():
    def inner():
        s = pickle.dumps(lst)
        pickle.loads(s)
    timeit(inner, number=100)
st = Stats(profile.Profile().runctx('f()', globals(), locals()))
st.sort_stats('time').print_stats()
st = Stats(profile.Profile().runctx('p()', globals(), locals()))
st.sort_stats('time').print_stats()
```
CPython2.7 运行结果:
```
$ python tcopy.py 
         191318 function calls (161318 primitive calls) in 0.559 seconds
   Ordered by: internal time
   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
20100/100    0.191    0.000    0.556    0.006 /usr/lib/python2.7/copy.py:145(deepcopy)
10100/100    0.095    0.000    0.554    0.006 /usr/lib/python2.7/copy.py:226(_deepcopy_list)
    20100    0.088    0.000    0.140    0.000 /usr/lib/python2.7/copy.py:267(_keep_alive)
    50400    0.066    0.000    0.066    0.000 {id}
    40300    0.052    0.000    0.052    0.000 {method 'get' of 'dict' objects}
    40000    0.052    0.000    0.052    0.000 {method 'append' of 'list' objects}
    10000    0.012    0.000    0.012    0.000 /usr/lib/python2.7/copy.py:198(_deepcopy_atomic)
      100    0.001    0.000    0.558    0.006 tcopy.py:19(inner)
        1    0.001    0.001    0.559    0.559 /usr/lib/python2.7/timeit.py:96(inner)
      100    0.000    0.000    0.001    0.000 /usr/lib/python2.7/copy.py:66(copy)
      100    0.000    0.000    0.000    0.000 /usr/lib/python2.7/copy.py:113(_copy_with_constructor)
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:143(setup)
        1    0.000    0.000    0.559    0.559 /usr/lib/python2.7/timeit.py:178(timeit)
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:121(__init__)
        1    0.000    0.000    0.559    0.559 /usr/lib/python2.7/timeit.py:227(timeit)
        2    0.000    0.000    0.000    0.000 {time.time}
        2    0.000    0.000    0.000    0.000 {isinstance}
        1    0.000    0.000    0.559    0.559 tcopy.py:18(f)
        1    0.000    0.000    0.000    0.000 {hasattr}
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:94(_template_func)
        1    0.000    0.000    0.000    0.000 {gc.disable}
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
        1    0.000    0.000    0.000    0.000 <string>:1(<module>)
        1    0.000    0.000    0.000    0.000 {gc.enable}
        1    0.000    0.000    0.000    0.000 {gc.isenabled}
        1    0.000    0.000    0.000    0.000 {globals}
         318 function calls in 0.031 seconds
   Ordered by: internal time
   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
      100    0.019    0.000    0.019    0.000 {cPickle.dumps}
      100    0.011    0.000    0.011    0.000 {cPickle.loads}
      100    0.001    0.000    0.031    0.000 tcopy.py:25(inner)
        1    0.000    0.000    0.031    0.031 /usr/lib/python2.7/timeit.py:96(inner)
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:143(setup)
        1    0.000    0.000    0.031    0.031 /usr/lib/python2.7/timeit.py:178(timeit)
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:121(__init__)
        1    0.000    0.000    0.031    0.031 /usr/lib/python2.7/timeit.py:227(timeit)
        2    0.000    0.000    0.000    0.000 {time.time}
        1    0.000    0.000    0.031    0.031 tcopy.py:24(p)
        2    0.000    0.000    0.000    0.000 {isinstance}
        1    0.000    0.000    0.000    0.000 {globals}
        1    0.000    0.000    0.000    0.000 {hasattr}
        1    0.000    0.000    0.000    0.000 /usr/lib/python2.7/timeit.py:94(_template_func)
        1    0.000    0.000    0.000    0.000 <string>:1(<module>)
        1    0.000    0.000    0.000    0.000 {gc.isenabled}
        1    0.000    0.000    0.000    0.000 {gc.disable}
        1    0.000    0.000    0.000    0.000 {gc.enable}
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
```
PyPy 2.0.0-beta1 运行结果：
```
$ python tcopy.py 
         191318 function calls (161318 primitive calls) in 1.015 seconds
   Ordered by: internal time
   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
20100/100    0.457    0.000    1.013    0.010 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:145(deepcopy)
10100/100    0.211    0.000    1.010    0.010 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:226(_deepcopy_list)
    20100    0.173    0.000    0.224    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:267(_keep_alive)
    40300    0.083    0.000    0.083    0.000 {method 'get' of 'dict' objects}
    40000    0.067    0.000    0.067    0.000 {method 'append' of 'list' objects}
    50400    0.015    0.000    0.015    0.000 {id}
    10000    0.007    0.000    0.007    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:198(_deepcopy_atomic)
      100    0.001    0.000    0.001    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:66(copy)
        1    0.000    0.000    1.015    1.015 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:96(inner)
      100    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/copy.py:113(_copy_with_constructor)
      100    0.000    0.000    1.014    0.010 tcopy.py:19(inner)
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:143(setup)
        1    0.000    0.000    1.015    1.015 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:178(timeit)
        1    0.000    0.000    0.000    0.000 {gc.disable}
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:121(__init__)
        1    0.000    0.000    1.015    1.015 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:227(timeit)
        2    0.000    0.000    0.000    0.000 {time.time}
        1    0.000    0.000    1.015    1.015 tcopy.py:18(f)
        1    0.000    0.000    0.000    0.000 {gc.isenabled}
        2    0.000    0.000    0.000    0.000 {isinstance}
        1    0.000    0.000    0.000    0.000 {gc.enable}
        1    0.000    0.000    0.000    0.000 {hasattr}
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:94(_template_func)
        1    0.000    0.000    0.000    0.000 {globals}
        1    0.000    0.000    0.000    0.000 <string>:1(<module>)
         714740 function calls (674740 primitive calls) in 4.780 seconds
   Ordered by: internal time
   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
      100    1.295    0.013    2.787    0.028 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:152(load)
    20100    0.826    0.000    0.919    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:227(memoize)
20100/100    0.451    0.000    1.982    0.020 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:269(save)
    20100    0.388    0.000    0.434    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:466(load_put)
    20000    0.361    0.000    0.449    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:480(load_append)
    10000    0.208    0.000    0.291    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:264(load_string)
10100/100    0.133    0.000    1.974    0.020 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:608(_batch_appends)
    10100    0.132    0.000    0.143    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:550(load_mark)
    70400    0.121    0.000    0.121    0.000 {method 'read' of 'cStringIO.StringI' objects}
10100/100    0.115    0.000    1.979    0.020 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:591(save_list)
    10000    0.113    0.000    0.848    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:480(save_string)
    20100    0.094    0.000    1.018    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:110(memoize)
    60300    0.085    0.000    0.085    0.000 {method 'write' of 'cStringIO.StringO' objects}
    40203    0.082    0.000    0.082    0.000 {method 'get' of 'dict' objects}
    30100    0.064    0.000    0.064    0.000 {method 'readline' of 'cStringIO.StringI' objects}
    40100    0.062    0.000    0.062    0.000 {method 'append' of 'list' objects}
    10000    0.050    0.000    0.050    0.000 {method 'decode' of 'str' objects}
    20100    0.047    0.000    0.047    0.000 {method 'pop' of 'list' objects}
    70400    0.043    0.000    0.043    0.000 {ord}
    80400    0.034    0.000    0.034    0.000 {id}
    20100    0.022    0.000    0.029    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:250(put)
    30100    0.016    0.000    0.016    0.000 {repr}
    40300    0.012    0.000    0.012    0.000 {len}
    10100    0.006    0.000    0.006    0.000 {iter}
    10100    0.005    0.000    0.007    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:179(marker)
    10100    0.005    0.000    0.012    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:325(load_list)
      100    0.002    0.000    1.989    0.020 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:121(dumps)
    20100    0.002    0.000    0.002    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:333(persistent_id)
      100    0.001    0.000    0.003    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:102(__init__)
      100    0.001    0.000    2.789    0.028 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:594(loads)
      100    0.001    0.000    0.001    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:173(__init__)
      100    0.001    0.000    4.779    0.048 tcopy.py:25(inner)
      100    0.001    0.000    0.001    0.000 /opt/pypy-2.0-beta1/lib_pypy/cPickle.py:136(__init__)
      100    0.001    0.000    1.983    0.020 /opt/pypy-2.0-beta1/lib-python/2.7/pickle.py:220(dump)
      100    0.001    0.000    0.001    0.000 {method 'getvalue' of 'cStringIO.StringO' objects}
        1    0.001    0.001    4.780    4.780 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:96(inner)
      200    0.000    0.000    0.000    0.000 {cStringIO.StringIO}
        1    0.000    0.000    0.000    0.000 {__import__}
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:2(<module>)
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/__init__.py:71(search_function)
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:143(setup)
        1    0.000    0.000    4.780    4.780 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:178(timeit)
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:121(__init__)
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/__init__.py:49(normalize_encoding)
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/codecs.py:77(__new__)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
        1    0.000    0.000    4.780    4.780 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:227(timeit)
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:29(getregentry)
        4    0.000    0.000    0.000    0.000 {isinstance}
        2    0.000    0.000    0.000    0.000 {time.time}
        1    0.000    0.000    4.780    4.780 tcopy.py:24(p)
        1    0.000    0.000    0.000    0.000 {method 'replace' of 'str' objects}
        2    0.000    0.000    0.000    0.000 {hasattr}
        1    0.000    0.000    0.000    0.000 {method 'translate' of 'str' objects}
        1    0.000    0.000    0.000    0.000 {method 'split' of 'str' objects}
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:10(Codec)
        1    0.000    0.000    0.000    0.000 {method 'join' of 'str' objects}
        1    0.000    0.000    0.000    0.000 {method '__new__' of 'type' objects}
        1    0.000    0.000    0.000    0.000 {gc.enable}
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:15(IncrementalEncoder)
        1    0.000    0.000    0.000    0.000 /opt/pypy-2.0-beta1/lib-python/2.7/timeit.py:94(_template_func)
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:19(IncrementalDecoder)
        1    0.000    0.000    0.000    0.000 {gc.isenabled}
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:23(StreamWriter)
        1    0.000    0.000    0.000    0.000 {gc.disable}
        1    0.000    0.000    0.000    0.000 /home/largetalk/venv/pypy-env/lib-python/2.7/encodings/string_escape.py:26(StreamReader)
        1    0.000    0.000    0.000    0.000 <string>:1(<module>)
        1    0.000    0.000    0.000    0.000 {globals}
```
deepcopy性能差距在一倍左右， pickle差距就不知到有多少倍了
