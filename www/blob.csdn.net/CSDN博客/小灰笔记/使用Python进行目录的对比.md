# 使用Python进行目录的对比 - 小灰笔记 - CSDN博客





2017年04月18日 07:52:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1318








如果进行单个文件的比较，可以使用difflib模块。虽然filecmp模块也能够进行单个文件的对比，但是前者能够提供观感更好的报告。如果我们只是想看一下两个目录中的某个文件是否一致而不关系其中的内容，那么后者或许是更改选的选择。

如果进行目录的对比，那么可以使用filecmp模块了。在IPython中进行简单的命令交互演示功能如下：

In [**1**]:
**import****filecmp**




In [**2**]:
ls

Chapter_01/
Chapter_02/




In [**3**]:
dirobj = filecmp.dircmp('Chapter_01','Chapter_02')




输出两个目录差异报告

In [**4**]:
dirobj.report()

diff Chapter_01 Chapter_02

Only in Chapter_01 : ['ip.py', 'os_mem.py', 'pid.py']

Only in Chapter_02 : ['d1.py', 'd2.py', 'diff.py', 'diff.zip', 'dns_parser.py', 'join.py', 'pydiff.py', 'report.html']

Identical files : ['c01.py']




输出部分报告（最外面的目录曾经，这里与第一个无差异是因为没有子目录）

In [**5**]:
dirobj.report_partial_closure()

diff Chapter_01 Chapter_02

Only in Chapter_01 : ['ip.py', 'os_mem.py', 'pid.py']

Only in Chapter_02 : ['d1.py', 'd2.py', 'diff.py', 'diff.zip', 'dns_parser.py', 'join.py', 'pydiff.py', 'report.html']

Identical files : ['c01.py']




输出全部报告

In [**6**]:
dirobj.report_full_closure()

diff Chapter_01 Chapter_02

Only in Chapter_01 : ['ip.py', 'os_mem.py', 'pid.py']

Only in Chapter_02 : ['d1.py', 'd2.py', 'diff.py', 'diff.zip', 'dns_parser.py', 'join.py', 'pydiff.py', 'report.html']

Identical files : ['c01.py']




查看一下输出结果对象格式，这个格式略显奇怪了一些

In [**7**]: type(dirobj.report_full_closure())

diff Chapter_01 Chapter_02

Only in Chapter_01 : ['ip.py', 'os_mem.py', 'pid.py']

Only in Chapter_02 : ['d1.py', 'd2.py', 'diff.py', 'diff.zip', 'dns_parser.py', 'join.py', 'pydiff.py', 'report.html']

Identical files : ['c01.py']

Out[**7**]:
NoneType




尝试把结果对象转换为字符串

In [**8**]: str(dirobj.report_full_closure())

diff Chapter_01 Chapter_02

Only in Chapter_01 : ['ip.py', 'os_mem.py', 'pid.py']

Only in Chapter_02 : ['d1.py', 'd2.py', 'diff.py', 'diff.zip', 'dns_parser.py', 'join.py', 'pydiff.py', 'report.html']

Identical files : ['c01.py']

Out[**8**]:
'None'



输出左边目录的清单

In [**9**]:
dirobj.left_list

Out[**9**]:
['c01.py', 'ip.py', 'os_mem.py', 'pid.py']




右边目录清单

In [**10**]:
dirobj.right_list

Out[**10**]: 

['c01.py',

 'd1.py',

 'd2.py',

 'diff.py',

 'diff.zip',

 'dns_parser.py',

 'join.py',

 'pydiff.py',

 'report.html']




仅在右边目录中存在的文件清单

In [**11**]:
dirobj.right_only

Out[**11**]: 

['d1.py',

 'd2.py',

 'diff.py',

 'diff.zip',

 'dns_parser.py',

 'join.py',

 'pydiff.py',

 'report.html']




共有的子目录

In [**12**]:
dirobj.common_dirs

Out[**12**]:
[]




共有的文件

In [**13**]:
dirobj.common_files

Out[**13**]:
['c01.py']




无法比较的目录

In [**14**]:
dirobj.common_funny

Out[**14**]:
[]




相同的文件

In [**15**]:
dirobj.same_files

Out[**15**]:
['c01.py']




无法比较的文件

In [**16**]:
dirobj.funny_files

Out[**16**]:
[]




在MATLAB中有一个我经常使用的命令——visdiff，可以用于文件或者目录的对比。如此比较下来，Python基本上也提供了MATLAB相应命令所提供的功能。不过，MATLAB的使用更为简单，似乎也更为细致一些。不过，考虑使用Python还是有必要的。首先是Python免费，其次是启动速度大概比MATLAB快不少吧！除此之外，现在的Python凑代码也是蛮简单的，在自己需要的时候几行代码拼凑一般就能够实现自己要的功能了。






