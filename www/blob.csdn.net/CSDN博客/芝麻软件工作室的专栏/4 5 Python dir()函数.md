
# 4.5 Python dir()函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:47:12[seven-soft](https://me.csdn.net/softn)阅读数：190个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



您可以使用内置的dir()函数列出一个定义对象的标识符。例如，对于一个模块，包括在模块中定义的函数，类和变量。
当你给dir()提供一个模块名字时，它返回在那个模块中定义的名字的列表。当没有为其提供参数时, 它返回当前模块中定义的名字的列表。
dir() 函数举例：
>>> import sys  \# 获得属性列表，在这里是sys模块的属性列表
>>> dir(sys)
['__displayhook__', '__doc__', '__excepthook__', '__name__', '__package__', '__stderr__', '__stdin__', '__stdout__', '_clear_type_cache', '_compact_freelists','_current_frames',
 '_getframe', 'api_version', 'argv', 'builtin_module_names', 'byteorder', 'call_tracing', 'callstats', 'copyright', 'displayhook', 'dllhandle', 'dont_write_bytecode', 'exc_info', 'excepthook', 'exec_prefix', 'executable','exit', 'flags', 'float_info', 'getcheckinterval',
 'getdefaultencoding', 'getfilesystemencoding', 'getprofile', 'getrecursionlimit', 'getrefcount', 'getsizeof','gettrace', 'getwindowsversion', 'hexversion', 'intern', 'maxsize', 'maxunicode', 'meta_path', 'modules', 'path', 'path_hooks', 'path_importer_cache',
 'platform', 'prefix', 'ps1', 'ps2', 'setcheckinterval', 'setprofile', 'setrecursionlimit', 'settrace', 'stderr', 'stdin', 'stdout', 'subversion', 'version', 'version_info', 'warnoptions', 'winver']
>>> dir()  \# 获得当前模块的属性列表
['__builtins__', '__doc__', '__name__', '__package__', 'sys']
>>> a = 5  \# 创建了一个新变量 'a'
>>> dir()
['__builtins__', '__doc__', '__name__', '__package__', 'a', 'sys']
>>> del a  \# 删除/移除一个名字
>>> dir()
['__builtins__', '__doc__', '__name__', '__package__', 'sys']
>>>
它是如何工作的：
首先，我们看到在导入sys模块上使用使用dir。我们能看到模块包含的巨大的属性列表。
然后，我们使用没有传递参数的dir函数。默认情况下，它返回模块的属性的列表。注意，导入模块的列表仍然是这个列表的一部分。
为了看到 dir在起作用，我们定义了一个新的变量，并为其赋值，然后检查dir，我们发现列表中添加了一个同名变量。我们使用del语句移除当前模块的变量或属性，在 del函数的输出中变化再次得到体现。
关于del的一点注意事项--这个语句用于删除一个变量/属性，语句运行后，这里是del a，你不能再访问变量a--就像它从来根本没有存在过。
注意，dir()函数对任何对象都起作用。例如，运行dir('print')来学习print函数的属性的更多知识，或运行dir(str)学习str类的属性的更多知识。

