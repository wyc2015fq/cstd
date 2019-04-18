# mysql 5.7.17 设置字符集为UTF-8 - z69183787的专栏 - CSDN博客
2018年04月24日 16:08:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：532
今天使用mysql进行中文插入数据的时候，系统报错，确认是编码问题后开始修改
`ERROR 1366 (HY000): Incorrect string value:XXX`
目前网上的通用版本是分别修改`/etc/mysql/my.cnf`文件中的`[client]`、`[musqld]`、`[mysql]`三个部分，添加utf8设置
`[client]``port = 3306``socket = /var/lib/mysql/mysql.sock``default-character-set=utf8``[mysqld]`
```
port
 = 3306
```
`socket = /var/lib/mysql/mysql.sock``character-set-server=utf8``[mysql]``no-auto-rehash``default-character-set=utf8`
然后重启服务
`sudo /etc/init.d/mysql restart`
但自己尝试的时候，发现在`/etc/mysql/my.cnf`文件跟示例不一样，根本没有`[client]`、`[musqld]`、`[mysql]`三个部分，而是如下：
`#`` 2 # The MySQL database server configuration file.`` 3 #`
```
4 # You can copy this to one
 of:
```
` 5 # - "/etc/mysql/my.cnf" to set global options,`` 6 # - "~/.my.cnf" to set user-specific options.`
```
7 #
```
`8 # One can use all long options that the program supports.`` 9 # Run program with --help to get a list of available options and with`
```
10 # --print-defaults to see which it would actually understand and use.
```
` 11 #`` 12 # For explanations see`
```
13 #
[http://dev.mysql.com/doc/mysql/en/server-system-variables.html](http://dev.mysql.com/doc/mysql/en/server-system-variables.html)
```
```
14
```
`15 # This will be passed to all mysql clients`` 16 # It has been reported that passwords should be enclosed with ticks/quotes`
```
17 # escpecially if they contain "#" chars...
```
` 18 # Remember to edit /etc/mysql/debian.cnf when changing the socket location.`` 19 !includedir /etc/mysql/conf.d/`
```
~
```
文件结构也有所不同
`.``├── conf.d``│ ├── msql.cnf│``└── mysqldump.cnf├──`
```
debian.cnf├──
```
`debian-start├──``my.cnf -> /etc/alternatives/my.cnf├──``my.cnf.fallback├──`
```
mysql.cnf└──
```
`mysql.conf.d `
```
├── mysqld.cnf
```
`└── mysqld_safe_syslog.cnf```
查了半天才发现，原来`mysqld`直接放在了`mysql.conf.d`文件夹下的`mysqld.cnf`里面，打开文件备注里面写到
`# You can copy this to one of:`` # - "/etc/mysql/my.cnf" to set global options,`` # - "~/.my.cnf" to set user-specific options.`
于是返回上一层目录，将`my.cnf`重新配置，添加了utf8设置
`...`` 18 # Remember to edit /etc/mysql/debian.cnf when changing the socket location.`
```
19
```
`20 [client]`` 21 ##port = 3306`` 22 ##socket = /var/lib/mysql/mysql.sock`` 23 default-character-set=utf8`
```
24
```
`25 [mysqld]`` 26 character-set-server=utf8`` 27 collation-server=utf8_general_ci`
```
28
```
`29 [mysql]`` 30 no-auto-rehash`
```
31 default-character-set=utf8
```
`...`
重启mysql服务之后，发现终于修改完成了。这么一个简单的事情，花费了近三个小时，惭愧~~
`mysql> show variables like '%character%';``+--------------------------+----------------------------+``| Variable_name | Value |``+--------------------------+----------------------------+`
```
|
 character_set_client | utf8 |
```
`| character_set_connection | utf8 |``| character_set_database | utf8 |`
```
| character_set_filesystem
 | binary |
```
`| character_set_results | utf8 |``| character_set_server | utf8 |``| character_set_system | utf8 |`
```
|
 character_sets_dir | /usr/share/mysql/charsets/ |
```
`+--------------------------+----------------------------+``8 rows in set (0.01 sec)`
> 
ps. 在设置utf8之前，如果已经创建了数据库，那之前的数据库编码依旧保持之前的状态，直接插入中文仍旧会报错。最好重新创建，并在数据库创建的时候就指明编码状态。
`CREATE DATABASE mydb`` DEFAULT CHARACTER SET utf8`` DEFAULT COLLATE utf8_general_ci;`
