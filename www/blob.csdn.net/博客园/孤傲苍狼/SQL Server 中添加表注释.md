# SQL Server 中添加表注释 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [SQL Server 中添加表注释](https://www.cnblogs.com/xdp-gacl/p/3506099.html)



今天在创建完表之后，发现没有办法给表添加注释说明，字段的注释可以在建表的时候就添加，上网查了一下使用SQL给表添加注释的方法，方法如下：

```
1 -- 表加注释
2 EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'注释内容' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'表名'
3 --例如：
4 EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'系统设置表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'CM01_SYSTEM'
```

顺便也记录一下使用SQL给表的字段添加注释的方法，方法如下：

```
1 -- 字段加注释
2 EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'注释内容' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'表名', @level2type=N'COLUMN',@level2name=N'字段名'
```











