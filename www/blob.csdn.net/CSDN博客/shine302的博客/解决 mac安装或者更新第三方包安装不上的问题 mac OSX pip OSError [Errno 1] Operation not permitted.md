# 解决 mac安装或者更新第三方包安装不上的问题 mac OSX pip OSError: [Errno 1] Operation not permitted - shine302的博客 - CSDN博客
2018年08月21日 15:18:25[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：139
**(1) 写在前面**
在安装机器学习python包时，提示Macbook里面的Python库有点旧，就用pip更新一下。
$ sudo pip install pandas
$ pip install --upgrade numpy
在安装过程中出现了如下错误：
OSError: [Errno 1] Operation not permitted: '/var/folders/bm/dccwv2v97y75hdshqnh1bbpr0000gn/T/pip-_dyADZ-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy-1.8.0rc1-py2.7.egg-info'
**(2) 具体错误报告如下：**
- 
`localhost:Applications a6$ sudo pip install pandas`
- 
`The directory '/Users/a6/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`The directory '/Users/a6/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`Collecting pandas`
- 
`Downloading pandas-0.21.0-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (16.7MB)`
- 
`100% |████████████████████████████████| 16.7MB 48kB/s`
- 
`Requirement already satisfied: pytz>=2011k in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from pandas)`
- 
`Requirement already satisfied: python-dateutil in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from pandas)`
- 
`Collecting numpy>=1.9.0 (from pandas)`
- 
`Downloading numpy-1.13.3-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (4.6MB)`
- 
`100% |████████████████████████████████| 4.6MB 6.4kB/s`
- 
`Installing collected packages: numpy, pandas`
- 
`Found existing installation: numpy 1.8.0rc1`
- 
`DEPRECATION: Uninstalling a distutils installed project (numpy) has been deprecated and will be removed in a future version. This is due to the fact that uninstalling a distutils project will only partially uninstall the project.`
- 
`Uninstalling numpy-1.8.0rc1:`
- 
`Exception:`
- 
`Traceback (most recent call last):`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/basecommand.py", line 215, in main`
- 
`status = self.run(options, args)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/commands/install.py", line 342, in run`
- 
`prefix=options.prefix_path,`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_set.py", line 778, in install`
- 
`requirement.uninstall(auto_confirm=True)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_install.py", line 754, in uninstall`
- 
`paths_to_remove.remove(auto_confirm)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_uninstall.py", line 115, in remove`
- 
`renames(path, new_path)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/utils/__init__.py", line 267, in renames`
- 
`shutil.move(old, new)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 302, in move`
- 
`copy2(src, real_dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 131, in copy2`
- 
`copystat(src, dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 103, in copystat`
- 
`os.chflags(dst, st.st_flags)`
- 
`OSError: [Errno 1] Operation not permitted: '/tmp/pip-jPsGEm-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy-1.8.0rc1-py2.7.egg-info'`
- 
- 
`localhost:Applications a6$ pip install -U numpy`
- 
`Collecting numpy`
- 
`Using cached numpy-1.13.3-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl`
- 
`Installing collected packages: numpy`
- 
`Found existing installation: numpy 1.8.0rc1`
- 
`DEPRECATION: Uninstalling a distutils installed project (numpy) has been deprecated and will be removed in a future version. This is due to the fact that uninstalling a distutils project will only partially uninstall the project.`
- 
`Uninstalling numpy-1.8.0rc1:`
- 
`Exception:`
- 
`Traceback (most recent call last):`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/basecommand.py", line 215, in main`
- 
`status = self.run(options, args)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/commands/install.py", line 342, in run`
- 
`prefix=options.prefix_path,`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_set.py", line 778, in install`
- 
`requirement.uninstall(auto_confirm=True)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_install.py", line 754, in uninstall`
- 
`paths_to_remove.remove(auto_confirm)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_uninstall.py", line 115, in remove`
- 
`renames(path, new_path)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/utils/__init__.py", line 267, in renames`
- 
`shutil.move(old, new)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 302, in move`
- 
`copy2(src, real_dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 131, in copy2`
- 
`copystat(src, dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 103, in copystat`
- 
`os.chflags(dst, st.st_flags)`
- 
`OSError: [Errno 1] Operation not permitted: '/var/folders/bm/dccwv2v97y75hdshqnh1bbpr0000gn/T/pip-_dyADZ-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy-1.8.0rc1-py2.7.egg-info'`
- 
- 
`localhost:Applications a6$ sudo pip install -U numpy`
- 
`The directory '/Users/a6/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`The directory '/Users/a6/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`Collecting numpy`
- 
`Downloading numpy-1.13.3-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (4.6MB)`
- 
`100% |████████████████████████████████| 4.6MB 198kB/s`
- 
`Installing collected packages: numpy`
- 
`Found existing installation: numpy 1.8.0rc1`
- 
`DEPRECATION: Uninstalling a distutils installed project (numpy) has been deprecated and will be removed in a future version. This is due to the fact that uninstalling a distutils project will only partially uninstall the project.`
- 
`Uninstalling numpy-1.8.0rc1:`
- 
`Exception:`
- 
`Traceback (most recent call last):`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/basecommand.py", line 215, in main`
- 
`status = self.run(options, args)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/commands/install.py", line 342, in run`
- 
`prefix=options.prefix_path,`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_set.py", line 778, in install`
- 
`requirement.uninstall(auto_confirm=True)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_install.py", line 754, in uninstall`
- 
`paths_to_remove.remove(auto_confirm)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/req/req_uninstall.py", line 115, in remove`
- 
`renames(path, new_path)`
- 
`File "/Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg/pip/utils/__init__.py", line 267, in renames`
- 
`shutil.move(old, new)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 302, in move`
- 
`copy2(src, real_dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 131, in copy2`
- 
`copystat(src, dst)`
- 
`File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 103, in copystat`
- 
`os.chflags(dst, st.st_flags)`
- 
`OSError: [Errno 1] Operation not permitted: '/tmp/pip-QissHG-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy-1.8.0rc1-py2.7.egg-info'`
**(3) 解决办法**
**1) 一番百度，Google后找到这个解决方案:**
**$ pip install --upgrade pip$ sudo pip install numpy --upgrade --ignore-installed$ sudo pip install scipy --upgrade --ignore-installed$ sudo pip install scikit-learn --upgrade --ignore-installed$ sudo pip install scipy --upgrade --ignore-installed**
**2) 具体代码如下:**
- 
`localhost:Applications a6$ sudo pip install scipy --upgrade --ignore-installed`
- 
`The directory '/Users/a6/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`The directory '/Users/a6/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`Collecting scipy`
- 
`Downloading scipy-1.0.0-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (16.8MB)`
- 
`100% |████████████████████████████████| 16.8MB 41kB/s`
- 
`Collecting numpy>=1.8.2 (from scipy)`
- 
`Downloading numpy-1.13.3-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (4.6MB)`
- 
`100% |████████████████████████████████| 4.6MB 89kB/s`
- 
`Installing collected packages: numpy, scipy`
- 
`Successfully installed numpy-1.13.3 scipy-1.0.0`
- 
`localhost:Applications a6$ pip install --upgrade pip`
- 
`Requirement already up-to-date: pip in /Library/Python/2.7/site-packages/pip-9.0.1-py2.7.egg`
- 
`localhost:Applications a6$ sudo pip install -U numpy`
- 
`The directory '/Users/a6/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`The directory '/Users/a6/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`Requirement already up-to-date: numpy in /Library/Python/2.7/site-packages`
- 
- 
`localhost:Applications a6$ sudo pip install pandas`
- 
`The directory '/Users/a6/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`The directory '/Users/a6/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.`
- 
`Collecting pandas`
- 
`Downloading pandas-0.21.0-cp27-cp27m-macosx_10_6_intel.macosx_10_9_intel.macosx_10_9_x86_64.macosx_10_10_intel.macosx_10_10_x86_64.whl (16.7MB)`
- 
`100% |████████████████████████████████| 16.7MB 54kB/s`
- 
`Requirement already satisfied: pytz>=2011k in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from pandas)`
- 
`Requirement already satisfied: python-dateutil in /System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python (from pandas)`
- 
`Requirement already satisfied: numpy>=1.9.0 in /Library/Python/2.7/site-packages (from pandas)`
- 
`Installing collected packages: pandas`
- 
`Successfully installed pandas-0.21.0`
- 
`localhost:Applications a6$`
