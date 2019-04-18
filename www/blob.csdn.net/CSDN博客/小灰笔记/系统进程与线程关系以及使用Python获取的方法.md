# 系统进程与线程关系以及使用Python获取的方法 - 小灰笔记 - CSDN博客





2017年04月13日 08:21:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：940








以前一直把两个概念混为一谈，其实，两者之间还是有很大的差异。进程确切来说是一个程序，一个程序需要有自己独立的内存等信息，而线程则是在这些框定下能够独立执行的程序单元。这样，一个进程的话至少会有一个线程。

修改一份之前写过的代码如下：

  1 #!/usr/bin/python

  2 

  3 import psutil

  4 

  5 pids = psutil.pids()

  6 for pid in pids:

  7     p = psutil.Process(pid)

  8 print("pid-%d,pname-%s.Thread number:%d" %(pid,p.name(),p.num_threads()))

  9 print("number of PIDs: %d" %len(pids))




程序执行结果：

GreydeMac-mini:01_系统基础信息模块详解 greyzhang$ sudo python pid.py 

Password:

pid-600,pname-Python.Thread number:1

pid-598,pname-sudo.Thread number:4

pid-597,pname-AddressBookSourceSync.Thread number:5

pid-584,pname-mdworker.Thread number:5

pid-570,pname-CVMCompiler.Thread number:2

pid-562,pname-bash.Thread number:1

pid-563,pname-com.apple.audio.SandboxHelper.Thread number:2




省略部分信息




pid-47,pname-configd.Thread number:8

pid-46,pname-appleeventsd.Thread number:2

pid-43,pname-mediaremoted.Thread number:2

pid-41,pname-fseventsd.Thread number:8

pid-40,pname-kextd.Thread number:2

pid-39,pname-uninstalld.Thread number:3

pid-37,pname-syslogd.Thread number:5

pid-36,pname-UserEventAgent.Thread number:2

pid-1,pname-launchd.Thread number:5

number of PIDs: 253

在Mac中，有些进程的线程获取还需要管理员的权限。如果没有管理员权限可能会遇到运行异常，比如下面的异常信息：

GreydeMac-mini:01_系统基础信息模块详解 greyzhang$ python pid.py 

pid-592,pname-Python.Thread number:1

pid-587,pname-AddressBookSourceSync.Thread number:5

pid-584,pname-mdworker.Thread number:5

Traceback (most recent call last):

  File "pid.py", line 8, in <module>

    print("pid-%d,pname-%s.Thread number:%d" %(pid,p.name(),p.num_threads()))

  File "/Library/Python/2.7/site-packages/psutil/__init__.py", line 890, in num_threads

    return self._proc.num_threads()

  File "/Library/Python/2.7/site-packages/psutil/_psosx.py", line 306, in wrapper

    raise AccessDenied(self.pid, self._name)

psutil.AccessDenied: psutil.AccessDenied (pid=570, name=‘CVMCompiler')






