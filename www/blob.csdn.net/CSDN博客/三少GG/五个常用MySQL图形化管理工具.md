# 五个常用MySQL图形化管理工具 - 三少GG - CSDN博客
2013年08月02日 10:35:13[三少GG](https://me.csdn.net/scut1135)阅读数：8063

# 五个常用MySQL图形化管理工具
核心提示：MySQL的管理维护工具非常多，除了系统自带的命令行管理工具之外，还有许多其他的图形化管理工具，这里我介绍五个我经常使用的MySQL图形化管理工具，供大家参考。
MySQL是一个非常流行的小型关系型数据库管理系统，2008年1月16号被Sun公司收购。目前MySQL被广泛地应用在Internet上的中小型网站中。由于其体积小、速度快、总体拥有成本低，尤其是开放源码这一特点，许多中小型网站为了降低网站总体拥有成本而选择了MySQL作为网站数据库。
**1、[web]phpMyAdmin**（http://www.phpmyadmin.net/）
![phpMyAdmin](http://upload.chinaz.com/upimg/allimg/090306/0948540.jpg)
phpMyAdmin是最常用的MySQL维护工具，是一个用PHP开发的基于Web方式架构在网站主机上的MySQL管理工具，支持中文，管理数据库非常方便。不足之处在于对大数据库的备份和恢复不方便。
**2、[web]MySQLDumper**（http://www.mysqldumper.de/en/）
![MySQLDumper](http://upload.chinaz.com/upimg/allimg/090306/0948541.jpg)
MySQLDumper使用PHP开发的MySQL数据库备份恢复程序，解决了使用PHP进行大数据库备份和恢复的问题，数百兆的数据库都可以方便的备份恢复，不用担心网速太慢导致中间中断的问题，非常方便易用。这个软件是德国人开发的，还没有中文语言包。
**3、[win]Navicat（**http://www.navicat.com/）
![Navicat](http://upload.chinaz.com/upimg/allimg/090306/0948542.jpg)
Navicat是一个桌面版MySQL数据库管理和开发工具。和微软SQLServer的管理器很像，易学易用。Navicat使用图形化的用户界面，可以让用户使用和管理更为轻松。支持中文，有免费版本提供。
**4、[win]MySQL GUI Tools**（http://dev.mysql.com/downloads/gui-tools/）
![MySQL GUI Tools](http://upload.chinaz.com/upimg/allimg/090306/0948543.jpg)
MySQL GUI Tools是MySQL官方提供的图形化管理工具，功能很强大，值得推荐，可惜的是没有中文界面。
**5、[win]MySQL ODBC Connector**（http://dev.mysql.com/downloads/connector/odbc/）
![MySQL ODBC Connector](http://upload.chinaz.com/upimg/allimg/090306/0948544.jpg)
MySQL官方提供的ODBC接口程序，系统安装了这个程序之后，就可以通过ODBC来访问MySQL，这样就可以实现SQLServer、Access和MySQL之间的数据转换，还可以支持ASP访问MySQL数据库。
