# selenium+python自动化-python3.5.4上SendKeys报错用PyUserInput取代 - zhusongziye的博客 - CSDN博客





2018年03月16日 21:26:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1115









# 前言

# 首先安装pywin32，小编安装过了，就不安装了，注意安装版本。

1.pywin32下载地址：[https://www.lfd.uci.edu/~gohlke/pythonlibs/#lxml](https://www.lfd.uci.edu/~gohlke/pythonlibs/#lxml)

![](https://img-blog.csdn.net/20180316212144884?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


python2上安装SendKeys库，对于不好定位的元素，用快捷键操作是极好的，那么在3.5.4上安装时，会报错

# python3.5.4安装SendKeys报错

1.python3.5.4安装SendKeys是无法安装的，会报错

> 
pip install SendKeys


![](https://img-blog.csdn.net/20180316211015286?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


# 32位的python3.5.4安装PyUserInput

1.python3.5.4分2个版本，1个是32位的python3.5.4;另外一个是64位的python3.5.4
- 
如果你的电脑是32位的，装python3.5.4 32位版本,直接用pip安装是没问题的

- 
如果你的电脑是64位的，装python3.5.4 32位版本，也是没问题的


> 
pip install PyUserInput


![](https://images2017.cnblogs.com/blog/1070438/201712/1070438-20171215163817949-68969520.png)

# 64位的python3.5.4安装PyUserInput

1.如果你的电脑是64位的，装python3.5.4 64位版本，那么问题就来了

![](https://img-blog.csdn.net/20180316211201393?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


2.报错：No matching distribution found for pyHook (from PyUserInput)

# pyHook安装

1.pyHook下载地址：[https://www.lfd.uci.edu/~gohlke/pythonlibs/#lxml](https://www.lfd.uci.edu/~gohlke/pythonlibs/#lxml)

![](https://images2017.cnblogs.com/blog/1070438/201712/1070438-20171215165129793-570135803.png)

2.下载到本地后，用pip安装

> 
pip install pyHook的本地路径


![](https://img-blog.csdn.net/20180316211242458?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


3.安装完pyHook后，继续安装PyUserInput，即可

![](https://img-blog.csdn.net/20180316211328986?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


# 导入PyMouse

1.环境安装完了，测试下导入成功。

![](https://img-blog.csdn.net/20180316211839601?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








标签:[selenium](http://www.cnblogs.com/yoyoketang/tag/selenium/)




