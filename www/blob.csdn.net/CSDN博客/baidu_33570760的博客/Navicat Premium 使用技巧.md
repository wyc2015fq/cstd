# Navicat Premium 使用技巧 - baidu_33570760的博客 - CSDN博客
2017年11月12日 21:58:08[carman_风](https://me.csdn.net/baidu_33570760)阅读数：5525
原文转载自：http://blog.csdn.net/lixingyu10/article/details/53389878
Navicat Premium 常用功能讲解
1.快捷键
1.1. F8 快速回到当前对象列表 
1.2. Ctrl + q 打开查询界面 
1.3. Ctrl + d 快速修改当前的表结构 
1.4. Ctrl + r 运行当前查询界面里面的 sql 语句 
1.5. Ctrl + shift + r 运行当前选中的 sql 语句
2.快速查看表结构
2.1.右键表信息->选择对象信息
![查看mysql对象信息-常规信息](http://www.linuxidc.com/upload/2016_04/160413083673383.png)
2.2.右键表信息->选择对象信息->选择 DDL信息
![这里写图片描述](http://www.linuxidc.com/upload/2016_04/160413083673384.png)
3.工具
3.1.数据传输：是把一个数据库的里面的数据复制到另一个数据库里面去 
3.2.数据同步：顾名思义是让两个数据库的数据保持同步的 
3.3.结构同步：故名事宜是保持两个数据库的数据结构一致的
![数据库工具](http://www.linuxidc.com/upload/2016_04/160413083673385.png)
3.4.复制当前的记录为 insert 语句
![复制当前记录为 insert 语句](http://www.linuxidc.com/upload/2016_04/160413083673389.png)
4.监控功能
4.1.打开监控
![服务器监控位置](http://www.linuxidc.com/upload/2016_04/160413083673381.png)
4.2.勾选自己需要监控的[mysql](http://lib.csdn.net/base/14)数据库 
4.3.监控当前服务器的状态，找出当前耗时较长的 sql 语句
![这里写图片描述](http://www.linuxidc.com/upload/2016_04/160413083673382.png)
5.sql 语句性能分析
5.1.看 sql 语句执行时间，如果时间太长肯定是有问题的，通常情况下，一条 sql 语句执行时间应该控制在 0.01 秒内，如果大于这个时间，那么说明此 sql 语句，或者此表的表结构需要优化，如下图所示
![sql 语句的执行时间](http://www.linuxidc.com/upload/2016_04/160413083673386.png)
5.2.看 sql 的执行计划，就是此 sql 在执行过程中，有没有用到索引 等等
![看 slq 的执行计划](http://www.linuxidc.com/upload/2016_04/160413083673387.png)
5.3.看 sql 的概况，看此sql 执行过程中，各个过程的耗时比例
![查看 sql 的执行中各个环节耗时时间](http://www.linuxidc.com/upload/2016_04/160413083673388.png)
