# Aptana下Django1.6以后的项目模板结构改造 - =朝晖= - 博客园
# [Aptana下Django1.6以后的项目模板结构改造](https://www.cnblogs.com/dhcn/p/7106435.html)
      Django1.6以后的manage.py放在项目包目录的根目录下，这种情况下在create app的app也在这个目录下面，由此可能导致app的名称有可能会和广大的内建包或者第三方包发生命名冲突，解决问问题的方法很简单，把manage.py移动到项目名称的子目录下面，这样项目的app目录结构就和以前一样了，同时在.pydevproject文件中要修改一下DJANGO_MANAGE_LOCATION这个设置的Value值。

