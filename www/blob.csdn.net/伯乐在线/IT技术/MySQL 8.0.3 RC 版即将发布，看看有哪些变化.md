# MySQL 8.0.3 RC 版即将发布，看看有哪些变化 - 文章 - 伯乐在线
原文出处： [MySQL](https://dev.mysql.com/doc/relnotes/mysql/8.0/en/)   译文出处：[老叶茶馆 微信公众号](https://www.oschina.net/news/88499/mysql-8-0-3-rc-coming-soon)
MySQL 8.0.3 即将发布，一起看看有什么新变化吧
MySQL 8.0.3 即将发布 RC 版本，这预示着 8.0 版本离 GA 越来越近了。
下面分享一下 8.0.3 版本的一些重要新特性、新变化：
- 直接**废除 query cache**（应该只是关闭入口，自行编译源码的话，还能再次启用）
- 查询优化器支持直接在 SQL 中利用 **SET_VAR** 这个 HINT 语法修改某些会话级的选项，比如：- SELECT /*+ SET_VAR(sort_buffer_size = 16M) */ name FROM people ORDER BY name;- INSERT /*+ SET_VAR(foreign_key_checks=OFF) */ INTO t2 VALUES(2);
（这个功能超级实用啊，直接在 SQL 内可以修改了，都不用再写一个 SQL，而且还担心会影响别人）
- 查询优化器现在支持将各个列统计结果直方图存储在 **column_statistics** 数据字典中，以便构造执行计划。有了这个直方图，就可用于该列和常量的比较判断
- 查询优化器中新增标记位 **use_invisible_indexes**，用于控制在构造执行计划时是否考虑不可见索引（invisible indexes）因素
- InnoDB 新增**备份专用锁**，以前 InnoDB 表在线热备时若同时进行 DML 可能导致 InnoDB 文件的快照不一致，有了这个备份锁就不用担心了。备份锁用法是 **LOCK INSTANCE FOR BACKUP、UNLOCK INSTANCE**，需要有 **BACKUP_ADMIN** 权限
- InnoDB 现在支持**表 DDL 的原子性**，也就是 InnoDB 表上的 DDL 也可以实现事务完整性了，要么失败回滚，要么成功提交，不至于出现 DDL 时部分成功的问题，此外还**支持 crash-safe 特性**（这个功能大赞啊）
- 若是延迟初始化组复制（GR，Group Replication），也就是在 single-primary 模式下，辅助节点上可以通过异步复制通道写入数据，这在正常初始化组复制插件时是不允许的（Bug #26314756，这是个内部 BUG ID，我们看不到的，别费劲了。不过还有万能的 github 啊，也能看到相关描述，参见：[https://github.com/mysql/mysql-server/commit/acbf1d13ecd1d89557f8ca62116f06e8633dc90d](https://github.com/mysql/mysql-server/commit/acbf1d13ecd1d89557f8ca62116f06e8633dc90d)）
- 系统库 INFORMATION_SCHEMA 中的 **FILES、PARTITIONS、REFERENTIAL_CONSTRAINTS **等几个视图被重新实现了一番（这个没说太具体，可能和所有的 DDL 都放入 InnoDB 有关系，不再使用 FRM 文件存储 DDL 信息）
- 由于外键约束锁功能改造，涉及到外键约束的父表上的列改名功能将暂时被禁用一段时间（估计过几个小版本就能正常用了）
- InnoDB 通用表空间新增改名语法：**ALTER TABLESPACE … RENAME TO**（作为 MySQL DBA，貌似没什么人玩通用表空间，可能从 Oracle DBA 转过来更习惯使用吧）
- MySQL 复制的 slave 节点上，**log_slave_updates **选项默认值改为 ON（这个也是比较实用，方便将该 slave 直接作为中继节点使用）
- 选项 **sql_log_bin **的全局作用域被改成回话级作用域，也就是修改本选项只影响本会话，不再影响全局（这个选项一般也是在会话级使用）
- 选项 **max_allowed_packet **的默认值从 4M 提高到 64M（这个貌似比较实在）
- 选项 **event_scheduler **的默认值由 OFF 改成 ON，使得默认启用 event scheduler 功能（我觉得 event 功能挺好的呀，不过貌似没几个人使用 event）
- 选项 **max_error_count **的默认值从 64 提高到 1024（没感觉的说）
- 最后一个，utf8mb4 字符集增加俄语的校验集（新增 utf8mb4_ru_0900_ai_ci、utf8mb4_ru_0900_as_cs 两种 collations set）
一起期待 8.0.3 的发布吧！
