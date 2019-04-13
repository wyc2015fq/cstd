
# WordPress 建立数据库连接时出错解决方法 - 阳光岛主 - CSDN博客

2015年04月05日 08:29:56[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：14300



周末外出和朋友一起钓鱼去了，晚上回来准备在自己的[米扑博客](http://blog.mimvp.com/)（[http://blog.mimvp.com](http://blog.mimvp.com/)）写一篇钓鱼游记，打开电脑结果发现博客网站打不开了，提示”建立数据库连接时出错“
![wordpress_db_fix_05](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_05.png)
好吧，问题已经很明了了，白天我去池塘钓别人的鱼，别人却在网上掉我的鱼，把我的博客网站整塌了…
废话多说无益，开工吧
**问题分析**
**首先，备份数据库**
备份博客数据库时，提示错误“Table ‘./db_name/table_name’ is marked as crashed and last (automatic?) repair failed” when using LOCK TABLES
![wordpress_db_fix_03](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_031.png)
提示信息已经很明了了，是锁定数据库表后，自动修复数据库失败，导致无法从数据库查询出数据。
**接着，网页修复数据库**
WordPress 带有网页自动修复数据库的功能，在浏览器输入自己博客管理员域名，例如： 米扑博客
[http://blog.mimvp.com/wp-admin/](http://blog.mimvp.com/wp-admin/)
![wordpress_db_fix_01](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_011.png)
按照引导提示，点击“**修复数据库**”，继续
![wordpress_db_fix_02](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_021.png)
按照引导提示，添加如下一行宏定义，到自己博客根目录下的 wp-config.php 文件的最底部
**define(‘WP_ALLOW_REPAIR’, true);**
保存，刷新一下页面，继续出现下图
![wordpress_db_fix_04](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_041.png)
按照引导提示，点击“**修复并优化数据库**”，结果会提示首先备份数据库一样的错误，再次确认是数据库表被锁定导致的问题。
**问题解决**
**问题**
“Table ‘./db_name/table_name‘ is marked as crashed and last (automatic?) repair failed” when using LOCK TABLES
这个问题的原因，大多是myisam表数据太多，在某个时刻存放数据的这个MyISAM表数据急速长大，比如一些log表，当把硬盘写满了时还在继续写入，然后这个表就会lock掉；或者是mysiam的存储表的文件tbl_name.MYI 损坏了
**解决**
找到mysql的数据库存放的文件夹，一般默认在/var/lib/mysql/目录下
或者去mysql的配置文件 my.cnf (linux)  或 my.ini(windows) 里面找 datadir 路径
例如：**vim /etc/my.cnf**
![wordpress_db_fix_06](http://cdn-blog.mimvp.com/wp-content/uploads/2015/03/wordpress_db_fix_06.png)
找到对应的数据库文件夹进去后，在该数据库文件夹下执行命令：
myisamchk -r <table_name>
其中，<table_name> 是想要修复的表名，如 wordpress/wp_options
如果这样还不能解决，那么先停掉mysql，然后执行命令：
myisamchk -r -v -f <table_name>
**总结**
我的问题，通过进入MySQL数据库目录下后，执行myisamchk -r <table_name>就解决了
解决的米扑博客：[http://blog.mimvp.com](http://blog.mimvp.com/)


