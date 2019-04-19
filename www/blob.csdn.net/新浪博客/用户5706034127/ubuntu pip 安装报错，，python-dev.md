# ubuntu pip 安装报错，，python-dev_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
			fatal error: Python.h: No such file or directory
还有很多其他错误。在ubuntu下pip安装包的时候报的错误。
这些错误往往是安装第三方非正规的包的时候包的错误。因为他们需要python-dev
而你新装的系统里面没有python-dev。
只需要执行apt-get install Python-dev即可
linux发行版通常会把类库的头文件和相关的pkg-config分拆成一个单独的xxx-dev(el)包.
以python为例, 以下情况你是需要python-dev的
你需要自己安装一个源外的python类库, 而这个类库内含需要编译的调用python api的c/c++文件
你自己写的一个程序编译需要链接libpythonXX.(a|so)
(注:以上不含使用ctypes/ffi或者裸dlsym方式直接调用libpython.so)
其他正常使用python或者通过安装源内的python类库的不需要python-dev.
python-dev负责外部第三方python相关包的编译之类的需要的文件。							
		
