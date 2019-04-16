# 修改mysql数据库字符集为UTF8的 - ggabcda的博客 - CSDN博客





2017年09月04日 14:49:48[ggabcda](https://me.csdn.net/ggabcda)阅读数：125








**第一种**

一、修改my.ini配置文件（MySQL配置文件）

character_set_server = utf8 #设置字符集





重启mysql数据库服务

查看当前数据库字符集

show VARIABLES like 'character%';



二、修改数据库字符集

alter database 数据库名 character set utf8;

ps:修改完数据库字符集，需要重启MySQL数据库。



三、修改表字符集

ALTER TABLE  表名 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci



四、修改列字符集

ALTER TABLE  表名 CHANGE  列名  列名  VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL;

第二种



一个是修改表的编码格式的





**[sql]** view plain copy






- ALTER TABLE `table` DEFAULT CHARACTER SET utf8;  


修改字段的编码格式 因为表的编码改了字段的没有所以还要修改字段 修改字段只能一条条该





**[sql]** view plain copy






- ALTER TABLE `tablename` CHANGE `字段名1` `字段名2` VARCHAR(36) CHARACTER SET utf8 NOT NULL;  


多条修改终极方式




**[sql]** view plain copy






- alter table `tablename` convert to character set utf8;  

第三种




1：查看当前的字符编码方法

mysql> show variables like'%character%';

+--------------------------+----------------------------+

|Variable_name           |Value                     |

+--------------------------+----------------------------+

|character_set_client    |latin1                    |

| character_set_connection |latin1                    |

| character_set_database   |latin1                    |

| character_set_filesystem |binary                    |

|character_set_results   |latin1                    |

|character_set_server    |latin1                    |

|character_set_system    |utf8                      |

|character_sets_dir      | /usr/share/mysql/charsets/ |

+--------------------------+----------------------------+

8 rows in set (0.00 sec)

 字符编码为latin1时，用户在给数据表插入一条字符类型（char）的记录，在显示此记录时可能会无法正常显示字符类型的数据，故此处我们将mysql字符编码修改为UTF8。操作如下：

步骤2：修改my.cnf配置文件，修改后如下（/etc/my.cnf）：

[client]

#password      = your_password

port           = 3306

socket         = /var/lib/mysql/mysql.sock

default-character-set=utf8     #设置默认编码为UTF8




