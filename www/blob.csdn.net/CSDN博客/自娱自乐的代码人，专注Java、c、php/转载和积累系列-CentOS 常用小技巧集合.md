# 转载和积累系列 - CentOS 常用小技巧集合 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年08月29日 09:15:09[initphp](https://me.csdn.net/initphp)阅读数：726
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)









### 1. 设置用户可以获得root权限



```php
#/etc/sudoers 这个文件主要存放sudo权限的用户
vim /etc/sudoers
#添加一行：
admin   ALL=(ALL)     ALL
```



然后使用admin登录，使用 su 命令 并且输入admin的密码后，就能获得root的权限了

同时也可以使用 sudo 来访问root权限的文件





