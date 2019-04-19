# AttributeError: module 'enum' has no attribute 'IntFlag' - =朝晖= - 博客园
# [AttributeError: module 'enum' has no attribute 'IntFlag'](https://www.cnblogs.com/dhcn/p/7444242.html)
      Mac PyCharm新建以Python3.6.1为解释器的Django项目的时候出现以下错误提示：
```
AttributeError: module 'enum' has no attribute 'IntFlag'
$ /Library/Frameworks/Python.framework/Versions/3.6/bin/python3  
Failed to import the site module  
Traceback (most recent call last):  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site.py", line 544, in <module>  
    main()  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site.py", line 530, in main  
    known_paths = addusersitepackages(known_paths)  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site.py", line 282, in addusersitepackages  
    user_site = getusersitepackages()  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site.py", line 258, in getusersitepackages  
    user_base = getuserbase() # this will also set USER_BASE  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site.py", line 248, in getuserbase  
    USER_BASE = get_config_var('userbase')  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/sysconfig.py", line 601, in get_config_var  
    return get_config_vars().get(name)  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/sysconfig.py", line 580, in get_config_vars  
    import _osx_support  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/_osx_support.py", line 4, in <module>  
    import re  
  File "/usr/local/Cellar/python3/3.6.1/Frameworks/Python.framework/Versions/3.6/lib/python3.6/re.py", line 142, in <module>  
    class RegexFlag(enum.IntFlag):  
AttributeError: module 'enum' has no attribute 'IntFlag'
```
        解决方法，.bash_profile中设置PYTHONPATH的代码全部注解，然后source .bash_profile，然后再新建项目，便再没出现错误。命令如下：
```
source .bash_profile
unset PYTHONPATH
```
        错误分析，PYTHONPATH设置是为了Python2.7设置的，其中安装的包覆盖了Python3正确的包，于是出现以上错误。
参考：https://stackoverflow.com/questions/43124775/why-python-3-6-1-throws-attributeerror-module-enum-has-no-attribute-intflag

