# mysql数据库delete数据时不支持表别名 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [mysql数据库delete数据时不支持表别名](https://www.cnblogs.com/xdp-gacl/p/4012853.html)



　　今天在帮同事查看一条删除的SQL语句执行出错的问题

　　SQL语句如下：

```
1 DELETE FROM LEAD_SYSTEM_MENU_ORG_REF as t WHERE t.resourceid='4028812348f28b890148f29253a80000'
```

这条SQL语句放到Oracle数据库中去执行是可以正常执行的，但是放到MySQL数据库中执行时就出现了如下的错误：

```
1 [Err] 1064 - You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'WHERE t.resourceid='4028812348f28b890148f29253a80000'
```

　　看到"SQL syntax"就知道SQL执行错误的原因是SQL语法不对，这就比较奇怪了，这条删除的SQL横看竖看都没有问题呀，Oracle能够正常执行，为什么MySQL却不行呢，上网查了一下原因，原来是MySQL数据库delete数据时，不支持表别名，这个问题以前也没有注意到，在此记录一下。









