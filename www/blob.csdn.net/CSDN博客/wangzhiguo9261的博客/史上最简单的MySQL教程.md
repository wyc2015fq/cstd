
# 史上最简单的MySQL教程 - wangzhiguo9261的博客 - CSDN博客


2018年02月11日 09:38:47[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：86个人分类：[MySQL](https://blog.csdn.net/wangzhiguo9261/article/category/7454395)


版权声明：Follow your heart and intuition.[http://blog.csdn.net/qq_35246620/article/details/70823903](http://blog.csdn.net/qq_35246620/article/details/70823903)
目录[(?)](http://blog.csdn.net/qq_35246620/article/details/70823903#)[[+]](http://blog.csdn.net/qq_35246620/article/details/70823903#)

> 温馨提示：
> 本系列博文已经同步到 GitHub，如有需要的话，欢迎大家到「
> [mysql-tutorial](https://github.com/guobinhit/mysql-tutorial)
> 」进行
> Star
> 和
> Fork
> 操作！

## 1 前言
数据库（Database）是按照数据结构来组织、存储和管理数据的仓库，它产生于距今六十多年前，随着信息技术和市场的发展，特别是二十世纪九十年代以后，数据管理不再仅仅是存储和管理数据，而转变成用户所需要的各种数据管理的方式。数据库有很多种类型，从最简单的存储有各种数据的表格到能够进行海量数据存储的大型数据库系统都在各个方面得到了广泛的应用。
## 2 目录
在本篇博客中，博主将列出所有关于 MySQL 数据库的一系列博文，以方便大家阅读，当然，也需要在此声明：**如果博文中出现了错误，请大家帮忙斧正，深感荣幸。**
第 1 篇：[史上最简单的 MySQL 教程（一）「数据库」](http://blog.csdn.net/qq_35246620/article/details/70832166)
第 2 篇：[史上最简单的 MySQL 教程（二）「关系型数据库」](http://blog.csdn.net/qq_35246620/article/details/70832791)
第 3 篇：[史上最简单的 MySQL 教程（三）「 MySQL 数据库」](http://blog.csdn.net/qq_35246620/article/details/70889287)
第 4 篇：[史上最简单的 MySQL 教程（四）「SQL 基本操作 之 库操作」](http://blog.csdn.net/qq_35246620/article/details/70941728)
第 5 篇：[史上最简单的 MySQL 教程（五）「SQL 基本操作 之 表操作」](http://blog.csdn.net/qq_35246620/article/details/70945890)
第 6 篇：[史上最简单的 MySQL 教程（六）「SQL 基本操作 之 数据操作」](http://blog.csdn.net/qq_35246620/article/details/71154252)
第 7 篇：[史上最简单的 MySQL 教程（七）「中文数据问题」](http://blog.csdn.net/qq_35246620/article/details/71195565)
第 8 篇：[史上最简单的 MySQL 教程（八）「校对集问题」](http://blog.csdn.net/qq_35246620/article/details/71211524)
第 9 篇：[史上最简单的 MySQL 教程（九）「列类型 之 数值型」](http://blog.csdn.net/qq_35246620/article/details/71241842)
第 10 篇：[史上最简单的 MySQL 教程（十）「列类型 之 日期时间型」](http://blog.csdn.net/qq_35246620/article/details/71250634)
第 11 篇：[史上最简单的 MySQL 教程（十一）「列类型 之 字符串型」](http://blog.csdn.net/qq_35246620/article/details/71274162)
第 12 篇：[史上最简单的 MySQL 教程（十二）「记录长度」](http://blog.csdn.net/qq_35246620/article/details/72597909)
第 13 篇：[史上最简单的 MySQL 教程（十三）「列属性 之 空属性、列描述和默认值」](http://blog.csdn.net/qq_35246620/article/details/72629220)
第 14 篇：[史上最简单的 MySQL 教程（十四）「列属性 之 主键」](http://blog.csdn.net/qq_35246620/article/details/72667229)
第 15 篇：[史上最简单的 MySQL 教程（十五）「列属性 之 自动增长」](http://blog.csdn.net/qq_35246620/article/details/72669193)
第 16 篇：[史上最简单的 MySQL 教程（十六）「列属性 之 唯一键」](http://blog.csdn.net/qq_35246620/article/details/72673014)
第 17 篇：[史上最简单的 MySQL 教程（十七）「索引」](http://blog.csdn.net/qq_35246620/article/details/72673473)
第 18 篇：[史上最简单的 MySQL 教程（十八）「关系」](http://blog.csdn.net/qq_35246620/article/details/72675304)
第 19 篇：[史上最简单的 MySQL 教程（十九）「范式」](http://blog.csdn.net/qq_35246620/article/details/72677626)
第 20 篇：[史上最简单的 MySQL 教程（二十）「数据的高级操作 之 主键冲突」](http://blog.csdn.net/qq_35246620/article/details/72854271)
第 21 篇：[史上最简单的 MySQL 教程（二十一）「数据的高级操作 之 蠕虫复制」](http://blog.csdn.net/qq_35246620/article/details/72854523)
第 22 篇：[史上最简单的 MySQL 教程（二十二）「数据的高级操作 之 更新 & 删除」](http://blog.csdn.net/qq_35246620/article/details/73722704)
第 23 篇：[史上最简单的 MySQL 教程（二十三）「数据的高级操作 之 查询（上）」](http://blog.csdn.net/qq_35246620/article/details/73725106)
第 24 篇：[史上最简单的 MySQL 教程（二十四）「数据的高级操作 之 查询（中）」](http://blog.csdn.net/qq_35246620/article/details/73822869)
第 25 篇：[史上最简单的 MySQL 教程（二十五）「数据的高级操作 之 查询（下）」](http://blog.csdn.net/qq_35246620/article/details/74938609)
第 26 篇：[史上最简单的 MySQL 教程（二十六）「连接查询（上）」](http://blog.csdn.net/qq_35246620/article/details/74999220)
第 27 篇：[史上最简单的 MySQL 教程（二十七）「连接查询（下）」](http://blog.csdn.net/qq_35246620/article/details/75138921)
第 28 篇：[史上最简单的 MySQL 教程（二十八）「外键（上）」](http://blog.csdn.net/qq_35246620/article/details/77418027)
第 29 篇：[史上最简单的 MySQL 教程（二十九）「外键（下）」](http://blog.csdn.net/qq_35246620/article/details/77426996)
第 30 篇：[史上最简单的 MySQL 教程（三十）「联合查询」](http://blog.csdn.net/qq_35246620/article/details/77606809)
第 31 篇：[史上最简单的 MySQL 教程（三十一）「子查询（上）」](http://blog.csdn.net/qq_35246620/article/details/77608761)
第 32 篇：[史上最简单的 MySQL 教程（三十二）「子查询（下）」](http://blog.csdn.net/qq_35246620/article/details/77619106)
第 33 篇：[史上最简单的 MySQL 教程（三十三）「视图（上）」](http://blog.csdn.net/qq_35246620/article/details/77823968)
第 34 篇：[史上最简单的 MySQL 教程（三十四）「视图（下）」](http://blog.csdn.net/qq_35246620/article/details/77914472)
第 35 篇：[史上最简单的 MySQL 教程（三十五）「数据备份与还原（上）」](http://blog.csdn.net/qq_35246620/article/details/78146831)
第 36 篇：[史上最简单的 MySQL 教程（三十六）「数据备份与还原（中）」](http://blog.csdn.net/qq_35246620/article/details/78147839)
第 37 篇：[史上最简单的 MySQL 教程（三十七）「数据备份与还原（下）」](http://blog.csdn.net/qq_35246620/article/details/78150410)
第 38 篇：[史上最简单的 MySQL 教程（三十八）「事务（上）」](http://blog.csdn.net/qq_35246620/article/details/78305872)
第 39 篇：[史上最简单的 MySQL 教程（三十九）「事务（下）」](http://blog.csdn.net/qq_35246620/article/details/78566894)
第 40 篇：[史上最简单的 MySQL 教程（四十）「数据库变量」](http://blog.csdn.net/qq_35246620/article/details/78703404)
第 41 篇：[史上最简单的 MySQL 教程（四十一）「触发器」](http://blog.csdn.net/qq_35246620/article/details/78946070)
第 42 篇：[史上最简单的 MySQL 教程（四十二）「代码执行结构」](http://blog.csdn.net/qq_35246620/article/details/78966087)

## 3 教程相关
第 1 篇：[详述 MySQL 数据库的安装及配置](http://blog.csdn.net/qq_35246620/article/details/71105110)
第 2 篇：[详述 MySQL 数据库输入密码后闪退的问题及解决方案](http://blog.csdn.net/qq_35246620/article/details/72636887)
第 3 篇：[详述查看 MySQL 数据文件存储位置的方法](http://blog.csdn.net/qq_35246620/article/details/78147260)
第 4 篇：[详述 MySQL 导出数据遇到 secure-file-priv 的问题](http://blog.csdn.net/qq_35246620/article/details/78148505)

---
此外，附上一句格言，望共勉：**好学若饥，谦卑若愚。**


