# django里面一些小细节 - Arthur的随笔 - CSDN博客
2012年05月26日 23:28:55[largetalk](https://me.csdn.net/largetalk)阅读数：3754
django源代码非常多，框架比较大，以我水平要分析一下还是比较吃力得，能看懂就不错了，要讲出来并且讲得比较清楚就更难了。所以先从小处入手，一点点看django得细节。
**1. 信用卡验证算法-luhn算法**。
in django/utils/checksums.py
代码比较简单
```python
LUHN_ODD_LOOKUP = (0, 2, 4, 6, 8, 1, 3, 5, 7, 9) # sum_of_digits(index * 2)
def luhn(candidate):
    """
    Checks a candidate number for validity according to the Luhn
    algorithm (used in validation of, for example, credit cards).
    Both numeric and string candidates are accepted.
    """
    if not isinstance(candidate, basestring):
        candidate = str(candidate)
    try:
        evens = sum([int(c) for c in candidate[-1::-2]])
        odds = sum([LUHN_ODD_LOOKUP[int(c)] for c in candidate[-2::-2]])
        return ((evens + odds) % 10 == 0)
    except ValueError:  # Raised if an int conversion fails
        return False
```
原理是：
1、从卡号最后一位数字开始，逆向将奇数位(1、3、5等等)相加。
2、将偶数位数字相加，但是这里有个麻烦。必须先将数字乘以2，如果结果是两位数，将两个位上数字相加。然后将这些结果加入总和中。
3、将奇数位总和加上偶数位总和，如果信用卡号码是合法的，结果应该可以被10整除。
**2. smart_unicode, smart_str**
基本上就是unicode和str，不过更安全一点，对于非字符串对象更好支持, 对于字符串使用decode和encode，支持更多字符集
smart_unicode(s, encoding='utf-8', strings_only=False, errors='strict')
smart_str(s, encoding='utf-8', strings_only=False, errors='strict')
**3. memoize**
in django/utils/functional.py
```python
def memoize(func, cache, num_args):
    """
    Wrap a function so that results for any argument tuple are stored in
    'cache'. Note that the args to the function must be usable as dictionary
    keys.
    Only the first num_args are considered when creating the key.
    """
    @wraps(func)
    def wrapper(*args):
        mem_args = args[:num_args]
        if mem_args in cache:
            return cache[mem_args]
        result = func(*args)
        cache[mem_args] = result
        return result
    return wrapper
```
看懂这个函数之前要先了解functools.wraps 和functools.update_wrap
其实update_wrap主要是将wrapped对象拷贝一些属性到wrapper对象中去， wraps则是update_wrap的partial
[http://autumn-sea.appspot.com/page/agphdXR1bW4tc2VhcgwLEgRCbG9nGNGifww](http://autumn-sea.appspot.com/page/agphdXR1bW4tc2VhcgwLEgRCbG9nGNGifww) 中解释的比较清楚了
明白了wraps，则上面mimoize也就没什么难的了
**4. curry**
in django/utils/functional.py
功能和functools.partial一样，只不过parital返回partial对象，而curry返回函数
**5.lazy**
in django/utils/functional.py
先看在其他地方看到的一个lazy init class的例子：
```python
class LazyProxy(object):
    def __init__(self, cls, *params, **kwargs):
        self.__dict__["_cls"]=cls
        self.__dict__["_params"]=params
        self.__dict__["_kwargs"]=kwargs
        self.__dict__["_obj"]=None
    def __getattr__(self, name):
        if self.__dict__["_obj"] is None:
            self.__init_obj()
        return getattr(self.__dict__["_obj"], name)
    def __setattr__(self, name, value):
        if self.__dict__["_obj"] is None:
            self.__init_obj()
        setattr(self.__dict__["_obj"], name, value)
    def __init_obj(self):
        self.__dict__["_obj"]=object.__new__(self.__dict__["_cls"], *self.__dict__["_params"], **self.__dict__["_kwargs"])
        self.__dict__["_obj"].__init__(*self.__dict__["_params"], **self.__dict__["_kwargs"])
class LazyInit(object):
    def __new__(cls, *params, **kwargs):
        return LazyProxy(cls, *params, **kwargs)
class A(LazyInit): # classes meant to be lazy loaded are derived from LazyInit
    def __init__(self, x):
        print("Init A")
        self.x=14+x
a=A(1)
print("Go")
print("15=", a.x)
```
通过阻断某些操作（new）来达到lazy的效果，django中的lazy实质也是一样， lazy对原来函数包装一下，包装后的函数执行时如果不是需要显示，保存等时，不会执行被包装的函数。
```python
def lazy(func, *resultclasses):
    class __proxy__(Promise):
        __dispatch = None
        def __init__(self, args, kw):
            self.__args = args
            self.__kw = kw
            if self.__dispatch is None:
                self.__prepare_class__()
        def __prepare_class__(cls):
            cls.__dispatch = {}
            for resultclass in resultclasses:
                cls.__dispatch[resultclass] = {}
                for type_ in reversed(resultclass.mro()):
                    for (k, v) in type_.__dict__.items():
                        # All __promise__ return the same wrapper method, but they
                        # also do setup, inserting the method into the dispatch
                        # dict.
                        meth = cls.__promise__(resultclass, k, v)
                        if hasattr(cls, k):
                            continue
                        setattr(cls, k, meth)
            cls._delegate_str = str in resultclasses
            cls._delegate_unicode = unicode in resultclasses
            assert not (cls._delegate_str and cls._delegate_unicode), "Cannot call lazy() with both str and unicode return types."
            if cls._delegate_unicode:
                cls.__unicode__ = cls.__unicode_cast
            elif cls._delegate_str:
                cls.__str__ = cls.__str_cast
        __prepare_class__ = classmethod(__prepare_class__)
        def __promise__(cls, klass, funcname, method):
            # Builds a wrapper around some magic method and registers that magic
            # method for the given type and method name.
            def __wrapper__(self, *args, **kw):
                # Automatically triggers the evaluation of a lazy value and
                # applies the given magic method of the result type.
                res = func(*self.__args, **self.__kw)
                for t in type(res).mro():
                    if t in self.__dispatch:
                        return self.__dispatch[t][funcname](res, *args, **kw)
                raise TypeError("Lazy object returned unexpected type.")
            if klass not in cls.__dispatch:
                cls.__dispatch[klass] = {}
            cls.__dispatch[klass][funcname] = method
            return __wrapper__
        __promise__ = classmethod(__promise__)
     
    @wraps(func)
    def __wrapper__(*args, **kw):
        # Creates the proxy object, instead of the actual value.
        return __proxy__(args, kw)
    return __wrapper__
```
上面代码看起来有点复杂，其实本质是用到了__setattr__, __getattr__, __str__, __unicode__做的一些奇怪事情，下面的代码也实现了lazy，比较简单易懂，跟踪一下就知道怎么回事了， 来自：[http://blitiri.com.ar/p/python/](http://blitiri.com.ar/p/python/)
```python
"""
lazy - Decorators and utilities for lazy evaluation in Python
Alberto Bertogli (albertito@blitiri.com.ar)
"""
class _LazyWrapper:
	"""Lazy wrapper class for the decorator defined below.
	It's closely related so don't use it.
	We don't use a new-style class, otherwise we would have to implement
	stub methods for __getattribute__, __hash__ and lots of others that
	are inherited from object by default. This works too and is simple.
	I'll deal with them when they become mandatory.
	"""
	def __init__(self, f, args, kwargs):
		self._override = True
		self._isset = False
		self._value = None
		self._func = f
		self._args = args
		self._kwargs = kwargs
		self._override = False
	def _checkset(self):
		print '111111111111', self._isset, self._value
		if not self._isset:
			self._override = True
			self._value = self._func(*self._args, **self._kwargs)
			self._isset = True
			self._checkset = lambda: True
			self._override = False
	def __getattr__(self, name):
		print '----------getattr----',  name
		if self.__dict__['_override']:
			return self.__dict__[name]
		self._checkset()
		print '@@@@@@@@@', self._value, type(self._value), name, self._value.__getattribute__(name)
		return self._value.__getattribute__(name)
	def __setattr__(self, name, val):
		print '----------setattr----',  name, val
		if name == '_override' or self._override:
			self.__dict__[name] = val
			return
		self._checkset()
		print '222222222222222'
		setattr(self._value, name, val)
		return
def lazy(f):
	"Lazy evaluation decorator"
	def newf(*args, **kwargs):
		return _LazyWrapper(f, args, kwargs)
	return newf
@lazy
def quick_exe():
	print '---------quick exe-----------'
	return 'quickquick'
import pdb
#pdb.set_trace()
quick_exe()
print '#####################'
print quick_exe()
```

