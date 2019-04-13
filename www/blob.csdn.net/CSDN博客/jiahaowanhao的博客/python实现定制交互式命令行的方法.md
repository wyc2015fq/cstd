
# python实现定制交互式命令行的方法 - jiahaowanhao的博客 - CSDN博客


2018年03月08日 20:56:02[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：471


[python实现定制交互式命令行的方法](http://cda.pinggu.org/view/24934.html)
Python的交互式命令行可通过启动文件来配置。
当Python启动时，会查找环境变量PYTHONSTARTUP,并且执行该变量中所指定文件里的程序代码。该指定文件名称以及地址可以是随意的。按Tab键时会自动补全内容和命令历史。这对命令行的有效增强，而这些工具则是基于readline模块实现的（这需要readline程序库辅助实现）。
此处为大家举一个简单的启动脚本文件例子，它为python命令行添加了按键自动补全内容和历史命令功能。
[python@python ~]$ cat .pythonstartup
import readline
import rlcompleter
import atexit
import os
\#tab completion
readline.parse_and_bind('tab: complete')
\#history file
histfile = os.path.join(os.environ['HOME'], '.pythonhistory')
try:
readline.read_history_file(histfile)
except IOError:
pass
atexit.register(readline.write_history_file,histfile)
del os,histfile,readline,rlcompleter
设置环境变量
[python@python ~]$ cat .bash_profile|grep PYTHON
export PYTHONSTARTUP=/home/python/.pythonstartup
验证Tab键补全和历史命令查看。
[python@python ~]$ python
Python 2.7.5 (default, Oct 6 2013, 10:45:13)
[GCC 4.1.2 20080704 (Red Hat 4.1.2-44)] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import md5
>>> md5.
md5.__class__(     md5.__getattribute__( md5.__reduce__(    md5.__subclasshook__(
md5.__delattr__(    md5.__hash__(     md5.__reduce_ex__(   md5.blocksize
md5.__dict__      md5.__init__(     md5.__repr__(     md5.digest_size
md5.__doc__      md5.__name__      md5.__setattr__(    md5.md5(
md5.__file__      md5.__new__(      md5.__sizeof__(    md5.new(
md5.__format__(    md5.__package__    md5.__str__(      md5.warnings
>>> import os
>>> import md5
注意：如果在make的时候出现：
Python build finished, but the necessary bits to build these modules were not found:
_tkinter            gdbm      readline      sunaudiodev
如果对此忽略了的话，import readline会报错。表示没有指定模块！
这里是缺少指定包：
redhat:   readline-devel.xxx.rpm
安装上重新编译执行，问题即可得到解决。

