# [MySQL 6.0 体验] 集群(cluster)+复制(replicate) - weixin_33985507的博客 - CSDN博客
2008年02月22日 20:26:43[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
作/译者：叶金荣（Email: ![email.gif](http://www.imysql.cn/files/pictures/email.gif)），来源：[http://imysql.cn](http://imysql.cn/)，转载请注明作/译者和出处，并且不能用于商业用途，违者必究。
本文主要是尝试MySQL 6.0下的集群+复制，没有做深入的测试，仅可作安装配置参考。
参考资料：[MySQL 6.0手册](http://dev.mysql.com/doc/refman/6.0/en/index.html)，本文中的图片引用自该手册。
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 简介、准备工作](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_intro_prepare)
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 配置节点](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_configure_nodes)
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 启动节点](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_start_nodes)
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 简单管理](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_manage_nodes)
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 备份、恢复](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_backup_and_restore)
- [[MySQL 6.0 体验] 集群(cluster)+复制(replicate) -- 集群下的复制](http://www.imysql.cn/2008_02_22_mysql6_cluster_and_replicate_rep_under_cluster)
- 本文出自 “MySQL中文网”博客 [http://www.imysql.cn/](http://www.imysql.cn/)
