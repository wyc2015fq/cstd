
# mysqldump: unknown option '--no-beep' 错误解决方法 - 追求卓越,做到专业 - CSDN博客


2015年12月10日 14:22:26[Waldenz](https://me.csdn.net/enter89)阅读数：10258


本文为大家讲解的是使用mysqldump导出数据时错误: unknown option '--no-beep' 的解决方法，感兴趣的同学参考下。
## 今天用 mysqldump 命令备份数据库时，出现了问题，截图如下：
![](http://img.phperz.com/data/img/20141222/1419235310_7030.png)
估计是版本的问题，我新装的 5.6 的，以前用的是 5.1 的，从来没出过问题。
网上找了一下，说查看**my.ini**发现[clien]下有**no-beep**参数，mysql客户端将会读取此参数(该参数作用暂时不知)。
## 解决办法

## 第一种方法：
## 删除my.ini [client]下的
## no-beep
## 参数;
## 第二种方法：在 mysqldump 后加--no-defaults参数,即:mysqldump --no-defaults -h主机IP -u用户名 -p密码 数据库 > xxx.sql 。
经测试，这两种方法有效。

