# GoogleAppEngineLauncher不能启动的解决方法 - weixin_33985507的博客 - CSDN博客
2009年11月12日 16:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
运行GoogleAppEngineLauncher的时候，提示出错，按照提示查看出错日志具体出错日志如下：
Traceback (most recent call last):
  File "GoogleAppEngineLauncher.py", line 42, in <module>
  File "wx\_core.pyc", line 7913, in __init__
  File "wx\_core.pyc", line 7487, in _BootstrapApp
  File "launcher\app.pyc", line 53, in OnInit
  File "launcher\app.pyc", line 97, in _CreateModels
  File "launcher\maintable.pyc", line 35, in __init__
  File "launcher\maintable.pyc", line 86, in _LoadProjects
  File "launcher\project.pyc", line 63, in ProjectWithConfigParser
  File "launcher\project.pyc", line 250, in _LoadFromConfigParser
  File "ConfigParser.pyc", line 520, in get
ConfigParser.NoOptionError: No option 'name' in section: '1' 
在网上搜了一圈，没有发现解决办法。不过从日志看是在_LoadProjects的时候出错的，所以很可能是项目配置文件有问题。但是在GAE的安装目录看了一圈，不知道项目信息放置到哪里的。在把GAE的安装目录删了重装还是不能解决的时候，想得项目的信息应该不是保存到安装目录下，那很可能是放到当前的用户目录下，果然在C:\Users\QLeelulu\Google(win7系统)目录下找到了，懒得看，直接把目录下的文件删了，重新启动GoogleAppEngineLauncher，哦了~~~！！
