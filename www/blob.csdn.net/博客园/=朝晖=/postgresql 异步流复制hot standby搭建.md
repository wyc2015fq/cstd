# postgresql 异步流复制hot standby搭建 - =朝晖= - 博客园
# [postgresql 异步流复制hot standby搭建](https://www.cnblogs.com/dhcn/p/7124625.html)
先说说环境，主从库版本都是9.5，主库在CentOS物理机上，从库在Ubuntu虚拟机上
**一、主库上的操作：**
1、添加同步访问规则：
- host   replication     dbuser          192.168.1.26/16         md5  
说明一下，上面的replication这个字符串不能改
2、postgresql.conf修改：
- listen_addresses = '*'  
- max_wal_senders = 5  
- wal_level = hotstandby  
- wal_keep_segments = 64     
简单说明一下，一般主库配置要求打开archive_mod，某贴说那个比较耗资源，就暂时用wal_keep_segments配置起类似作用
**二、从库操作**
1、生成基础备份
- pg_basebackup -D ./hotstandbydata -W -R -F p -P --xlog-method=stream --dbname="host=192.168.1.27 user=dbuser"  
2、修改从库配置，在刚才的hotstandbydata目录里面找到postgresql.conf启动其从库模式：
- hot_standby = on  
3、启动从库
- pg_ctl -D ./hotstandbydata -l hotstandby.log start  
4、从库关闭：
- pg_ctl -D ./hotstandbydata -l hotstandby.log stop  
采用3中命令行启动的数据库服务，必须采用4中的命令关闭，用/etc/init.d中的命令关闭，会导致下次采用3中命令启动时报错：pid文件被占。
说几个问题：1、是大多数的操作最好在postgres用户下执行，2、最后的pg_ctl命令找不到，不用去装postgres-xc那个东西，postgresql本身就有，只是整到系统路径下，做个软链接就可用了。
参考资料：https://book.douban.com/subject/26389922/
https://wiki.postgresql.org/wiki/Hot_Standby
http://www.postgresql.org/docs/9.5/static/app-pgbasebackup.html
http://francs3.blog.163.com/blog/static/40576727201305159244/
http://francs3.blog.163.com/blog/static/40576727201108864230/

