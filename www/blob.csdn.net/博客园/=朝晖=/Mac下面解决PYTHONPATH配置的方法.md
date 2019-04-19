# Mac下面解决PYTHONPATH配置的方法 - =朝晖= - 博客园
# [Mac下面解决PYTHONPATH配置的方法](https://www.cnblogs.com/dhcn/p/7120920.html)
      问题起因：MacPort安装的[Python](http://lib.csdn.net/base/python)包在/opt目录里面,和系统安装的[python](http://lib.csdn.net/base/python)包不在一起，由于Mac下面编译安装无止境的找不到问题,虽然MacPort解决了这个问题，但是它安装的东西，得自己到它的目录下面去找，用MacPort安装了psycopg2驱动，却无法导入，看了一些资料就是修改系统bash配置文件，但是没有找到PythonPath自己的配置位置，最后想了一招，用sys.path.append把MacPort安装的site-packages目录给添加上，就算找到了psycopg2。
    最后找到另外解决办法，因为我用的是PyCharm。在pycharm的interpreter设置里面可以给interpreter添加root路径，进去以后吧opt里面的site-packages添加上即可。而且好像必须在界面里面添加，在配置文件里面手动添加好像无法生效。

