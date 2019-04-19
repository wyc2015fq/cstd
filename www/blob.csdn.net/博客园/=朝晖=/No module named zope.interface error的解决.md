# No module named zope.interface error的解决 - =朝晖= - 博客园
# [No module named zope.interface error的解决](https://www.cnblogs.com/dhcn/p/7125172.html)
    明明安装了 zope.interface，还是出现标题错误，错误语句是
- from zope.interface import ooxx  
    根据 http://stackoverflow.com/questions/11010412/importerror-no-module-named-zope-interface 帖子的提示，原因是zope目录下缺一个__init__文件，这个文件干什么的，具体看Python模块资料吧，解决办法就是在zope目录下面新建一个__init__空文件即可解决问题。

