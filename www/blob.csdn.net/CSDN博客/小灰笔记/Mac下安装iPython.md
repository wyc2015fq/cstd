# Mac下安装iPython - 小灰笔记 - CSDN博客





2017年04月12日 23:31:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：10476








没有想象中那么顺利，尽管我已经搞定了pip，但是依然还是遇到了问题。安装的时候为了能够让安装顺利，需要使用sudo在管理员模式下安装。不过，为了能够让当前目录使用安装的东西，需要加上-H的参数。

安装操作如下：

GreydeMac-mini:~ greyzhang$ sudo -H pip install ipython

Password:

Collecting ipython

  Downloading ipython-5.3.0-py2-none-any.whl (750kB)

    100% |████████████████████████████████| 757kB 256kB/s 

Collecting prompt-toolkit<2.0.0,>=1.0.4 (from ipython)

  Downloading prompt_toolkit-1.0.14-py2-none-any.whl (248kB)

    100% |████████████████████████████████| 256kB 336kB/s 

Collecting decorator (from ipython)

  Downloading decorator-4.0.11-py2.py3-none-any.whl

Requirement already satisfied: setuptools>=18.5 in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from ipython)

Collecting pickleshare (from ipython)

  Downloading pickleshare-0.7.4-py2.py3-none-any.whl

Collecting pygments (from ipython)

  Downloading Pygments-2.2.0-py2.py3-none-any.whl (841kB)

    100% |████████████████████████████████| 849kB 466kB/s 

Collecting pexpect; sys_platform != "win32" (from ipython)

  Downloading pexpect-4.2.1-py2.py3-none-any.whl (55kB)

    100% |████████████████████████████████| 61kB 8.3MB/s 

Collecting pathlib2; python_version == "2.7" or python_version == "3.3" (from ipython)

  Downloading pathlib2-2.2.1-py2.py3-none-any.whl

Collecting backports.shutil-get-terminal-size; python_version == "2.7" (from ipython)

  Downloading backports.shutil_get_terminal_size-1.0.0-py2.py3-none-any.whl

Collecting simplegeneric>0.8 (from ipython)

  Downloading simplegeneric-0.8.1.zip

Collecting traitlets>=4.2 (from ipython)

  Downloading traitlets-4.3.2-py2.py3-none-any.whl (74kB)

    100% |████████████████████████████████| 81kB 1.6MB/s 

Collecting appnope; sys_platform == "darwin" (from ipython)

  Downloading appnope-0.1.0-py2.py3-none-any.whl

Collecting six>=1.9.0 (from prompt-toolkit<2.0.0,>=1.0.4->ipython)

  Downloading six-1.10.0-py2.py3-none-any.whl

Collecting wcwidth (from prompt-toolkit<2.0.0,>=1.0.4->ipython)

  Downloading wcwidth-0.1.7-py2.py3-none-any.whl

Collecting ptyprocess>=0.5 (from pexpect; sys_platform != "win32"->ipython)

  Downloading ptyprocess-0.5.1-py2.py3-none-any.whl

Collecting scandir; python_version < "3.5" (from pathlib2; python_version == "2.7" or python_version == "3.3"->ipython)

  Downloading scandir-1.5.tar.gz

Collecting enum34; python_version == "2.7" (from traitlets>=4.2->ipython)

  Downloading enum34-1.1.6-py2-none-any.whl

Collecting ipython-genutils (from traitlets>=4.2->ipython)

  Downloading ipython_genutils-0.2.0-py2.py3-none-any.whl

Building wheels for collected packages: simplegeneric, scandir

  Running setup.py bdist_wheel for simplegeneric ... done

  Stored in directory: /var/root/Library/Caches/pip/wheels/7b/31/08/c85e74c84188cbec6a6827beec4d640f2bd78ae003dc1ec09d

  Running setup.py bdist_wheel for scandir ... done

  Stored in directory: /var/root/Library/Caches/pip/wheels/47/af/a2/eb591a17e9709a17d8b53696f6ad89914a05eaf9c091f36e18

Successfully built simplegeneric scandir

Installing collected packages: six, wcwidth, prompt-toolkit, decorator, scandir, pathlib2, pickleshare, pygments, ptyprocess, pexpect, backports.shutil-get-terminal-size, simplegeneric, enum34, ipython-genutils, traitlets, appnope,
 ipython

  Found existing installation: six 1.4.1

    DEPRECATION: Uninstalling a distutils installed project (six) has been deprecated and will be removed in a future version. This is due to the fact that uninstalling a distutils project will only partially uninstall the project.

    Uninstalling six-1.4.1:

Exception:

Traceback (most recent call last):

  File "/Library/Python/2.7/site-packages/pip/basecommand.py", line 215, in main

    status = self.run(options, args)

  File "/Library/Python/2.7/site-packages/pip/commands/install.py", line 342, in run

    prefix=options.prefix_path,

  File "/Library/Python/2.7/site-packages/pip/req/req_set.py", line 778, in install

    requirement.uninstall(auto_confirm=True)

  File "/Library/Python/2.7/site-packages/pip/req/req_install.py", line 754, in uninstall

    paths_to_remove.remove(auto_confirm)

  File "/Library/Python/2.7/site-packages/pip/req/req_uninstall.py", line 115, in remove

    renames(path, new_path)

  File "/Library/Python/2.7/site-packages/pip/utils/__init__.py", line 267, in renames

    shutil.move(old, new)

  File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 302, in move

    copy2(src, real_dst)

  File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 131, in copy2

    copystat(src, dst)

  File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 103, in copystat

    os.chflags(dst, st.st_flags)

OSError: [Errno 1] Operation not permitted: ‘/tmp/pip-WgkEYU-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/six-1.4.1-py2.7.egg-info'




开始看着似乎蛮顺利，没想到到了后面出来一个崩溃性问题。通过提示得知应该是权限问题，但是切换到管理员下面去执行依然还是有这个问题。由此猜测应该是还有一层系统级的保护权限。借助于网络查了好一阵子，终于找到了解决方案，需要取消系统中的SIP机制。之后再次尝试安装，安装成功，具体的过程如下：

GreydeMac-mini:~ greyzhang$ sudo -H pip install ipython

Password:

Collecting ipython

  Using cached ipython-5.3.0-py2-none-any.whl

Collecting prompt-toolkit<2.0.0,>=1.0.4 (from ipython)

  Using cached prompt_toolkit-1.0.14-py2-none-any.whl

Collecting decorator (from ipython)

  Using cached decorator-4.0.11-py2.py3-none-any.whl

Requirement already satisfied: setuptools>=18.5 in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from ipython)

Collecting pickleshare (from ipython)

  Using cached pickleshare-0.7.4-py2.py3-none-any.whl

Collecting pygments (from ipython)

  Using cached Pygments-2.2.0-py2.py3-none-any.whl

Collecting pexpect; sys_platform != "win32" (from ipython)

  Using cached pexpect-4.2.1-py2.py3-none-any.whl

Collecting pathlib2; python_version == "2.7" or python_version == "3.3" (from ipython)

  Using cached pathlib2-2.2.1-py2.py3-none-any.whl

Collecting backports.shutil-get-terminal-size; python_version == "2.7" (from ipython)

  Using cached backports.shutil_get_terminal_size-1.0.0-py2.py3-none-any.whl

Collecting simplegeneric>0.8 (from ipython)

Collecting traitlets>=4.2 (from ipython)

  Using cached traitlets-4.3.2-py2.py3-none-any.whl

Collecting appnope; sys_platform == "darwin" (from ipython)

  Using cached appnope-0.1.0-py2.py3-none-any.whl

Collecting six>=1.9.0 (from prompt-toolkit<2.0.0,>=1.0.4->ipython)

  Using cached six-1.10.0-py2.py3-none-any.whl

Collecting wcwidth (from prompt-toolkit<2.0.0,>=1.0.4->ipython)

  Using cached wcwidth-0.1.7-py2.py3-none-any.whl

Collecting ptyprocess>=0.5 (from pexpect; sys_platform != "win32"->ipython)

  Using cached ptyprocess-0.5.1-py2.py3-none-any.whl

Collecting scandir; python_version < "3.5" (from pathlib2; python_version == "2.7" or python_version == "3.3"->ipython)

Collecting enum34; python_version == "2.7" (from traitlets>=4.2->ipython)

  Using cached enum34-1.1.6-py2-none-any.whl

Collecting ipython-genutils (from traitlets>=4.2->ipython)

  Using cached ipython_genutils-0.2.0-py2.py3-none-any.whl

Installing collected packages: six, wcwidth, prompt-toolkit, decorator, scandir, pathlib2, pickleshare, pygments, ptyprocess, pexpect, backports.shutil-get-terminal-size, simplegeneric, enum34, ipython-genutils, traitlets, appnope,
 ipython

  Found existing installation: six 1.4.1

    DEPRECATION: Uninstalling a distutils installed project (six) has been deprecated and will be removed in a future version. This is due to the fact that uninstalling a distutils project will only partially uninstall the project.

    Uninstalling six-1.4.1:

      Successfully uninstalled six-1.4.1

Successfully installed appnope-0.1.0 backports.shutil-get-terminal-size-1.0.0 decorator-4.0.11 enum34-1.1.6 ipython-5.3.0 ipython-genutils-0.2.0 pathlib2-2.2.1 pexpect-4.2.1 pickleshare-0.7.4 prompt-toolkit-1.0.14 ptyprocess-0.5.1
 pygments-2.2.0 scandir-1.5 simplegeneric-0.8.1 six-1.10.0 traitlets-4.3.2 wcwidth-0.1.7




尝试启动iPython，看到了熟悉的输入输出提示符交互界面。

GreydeMac-mini:~ greyzhang$ ipython

Python 2.7.10 (default, Jul 30 2016, 19:40:32) 

Type "copyright", "credits" or "license" for more information.




IPython 5.3.0 -- An enhanced Interactive Python.

?         -> Introduction and overview of IPython's features.

%quickref -> Quick reference.

help      -> Python's own help system.

object?   -> Details about 'object', use 'object??' for extra details.




In [**1**]: 



