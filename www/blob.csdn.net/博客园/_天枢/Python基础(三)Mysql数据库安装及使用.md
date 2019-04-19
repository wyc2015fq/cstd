# Python基础(三)Mysql数据库安装及使用 - _天枢 - 博客园
## [Python基础(三)Mysql数据库安装及使用](https://www.cnblogs.com/yhleng/p/7650725.html)
2017-10-11 15:16 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7650725)
**在python下使用mysql需要：**
1、安装mysql
2、安装python  pymysql包(pymysql包支持py3 跟mysqldb用法差不多)
**一、安装mysql**
mysql下载地址:[https://dev.mysql.com/downloads/windows/](https://dev.mysql.com/downloads/windows/)
如果下载的不是setup.exe安装的话，需要在cmd命令行安装
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171011171753840-1160233592.png)
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171011171817059-905254125.png)
命令行安装：mysqld.exe --install
mysql-->bin-->mysqld.exe
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171011171848355-1398041802.png)
上图提示安装成功！
**需要注意的是，命令行安装，有时会报Install/Remove of the Service Denied!这样的错误，**
**这个错误是由于，你打开cmd命令行不是以管理员身份运行的cmd.exe**
