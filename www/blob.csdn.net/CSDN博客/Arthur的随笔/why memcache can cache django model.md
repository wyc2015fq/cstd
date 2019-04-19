# why memcache can cache django model - Arthur的随笔 - CSDN博客
2011年12月08日 10:40:27[largetalk](https://me.csdn.net/largetalk)阅读数：1109标签：[django																[cache																[numbers																[memcached																[types																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=types&t=blog)](https://so.csdn.net/so/search/s.do?q=memcached&t=blog)](https://so.csdn.net/so/search/s.do?q=numbers&t=blog)](https://so.csdn.net/so/search/s.do?q=cache&t=blog)](https://so.csdn.net/so/search/s.do?q=django&t=blog)
个人分类：[django																[python](https://blog.csdn.net/largetalk/article/category/715661)](https://blog.csdn.net/largetalk/article/category/716448)
memcache 可以缓存一切可序列化和反序列化的对象
python-memcached使用pickle和cpickle来序列化和反序列化对象
而pickle可以序列化的类型有
The following types can be pickled:
- None, True,
 and False
- integers, long integers, floating point numbers, complex numbers
- normal and Unicode strings
- tuples, lists, sets, and dictionaries containing only picklable objects
- functions defined at the top level of a module
- built-in functions defined at the top level of a module
- classes that are defined at the top level of a module
- instances of such classes whose __dict__ or [__setstate__()](#object.__setstate__) is
 picklable (see section [*The pickle protocol*](#pickle-protocol) for details)
django model的__dict__除了各列内容就多一个'_state', 其只是用来保存数据库名的一个东西，so memcached can cache django model
