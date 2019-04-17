# Ubuntu下mysql中文乱码的解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月01日 17:46:41[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5071








 1.以root登陆，在终端输入命令 sudo gedit /etc/mysql/my.cnf在打开的文件中

    找到［client]在下面加入

   default-character-set=utf8

   找到 [mysqld_safe]在下面加入

   default-character-set=utf8

   找到[mysqld]在下面加入

   default-character-set=utf8

   init_connect='SET NAMES utf8'

  找到［mysql］在下面加入

  default-character-set=utf8


2.重启mysql命令以便让你上面的修改生效

停用命令：sudo mysqladmin shutdown -u root -p

启用命令：sudo /etc/init.d/mysql restart

           或者：sudo mysqld_safe -user=mysql &


3.查看现在mysql的编码

进入mysql命令：sudo mysql －u root -p

检查mysql字符集命令：show variables like 'character%'; 

+--------------------------+----------------------------+

| Variable_name | Value |

+--------------------------+----------------------------+

| character_set_client | utf8 | 

| character_set_connection | utf8 | 

| character_set_database | utf8 | 

| character_set_filesystem | binary | 

| character_set_results | utf8 | 

| character_set_server | utf8 | 

| character_set_system | utf8 | 

| character_sets_dir | /usr/share/mysql/charsets/ | 

+--------------------------+----------------------------+


4.原来建的库和表似乎都无法生效，不过新建的则可以正常显示中文！



