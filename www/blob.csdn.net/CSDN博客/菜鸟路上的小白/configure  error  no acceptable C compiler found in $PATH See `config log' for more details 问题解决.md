# configure: error: no acceptable C compiler found in $PATH See `config.log' for more details 问题解决 - 菜鸟路上的小白 - CSDN博客





2016年09月05日 17:16:51[somnus_小凯](https://me.csdn.net/u012486840)阅读数：3311








**安装**

1、下载postgresql最新版：[http://www.postgresql.org/ftp/source/](http://www.postgresql.org/ftp/source/)

2、解压文件：

　　tar zxvf postgresql-8.3.7.tar.gz

　　cd postgresql-8.3.7

3、配置：

　　./configure --prefix=/usr/local/pgsql

4、编译：

　　make

5、安装：

　　make install

6、创建用户组和用户：

　　groupadd postgres

　　useradd -g postgres postgres

7、创建数据库库文件存储目录、给postgres赋予权限：

　　mkdir /usr/local/pgsql/data

　　cd /usr/local/pgsql

　　chown postgres.postgres data

8、初始化数据库目录：

切换用户

　　su - postgresql

初始化数据

　　/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data

启动数据库

　　/usr/local/pgsql/bin/postmaster -D /usr/local/pgsql/data

9、配置监听地址和端口：

vi /usr/local/pgsql/data/postgresql.conf

修改

　　listen_addresses = '*'

　　port = 5432

10、允许远程连接：

vi /usr/local/pgsql/data/pg_hba.conf

修改

　　host all all 0.0.0.0/0 trust

配置iptables让远程主机能访问：

　　vi /etc/sysconfig

　　添加

　　　　-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 5432 -j ACCEPT

　　service iptables restart

11、让postgresql数据库随系统启动而启动：

将启动脚本拷贝到/etc/init.d/目录下，具体执行如下命令：

cd /etc/rc.d/init.d

cp (第一步解压的安装文件目录)/postgresql-8.3.7/contrib/start-scripts/linux postgresql

chmod +x postgresql

vi postgresql

　　prefix=/usr/local/pgsql

　　PGDATA="/usr/local/pgsql/data"

　　PGUSER=postgres

　　PGLOG="/var/log/pgsql.log"

　　chkconfig --add postgresql

启动数据库：

service postgresql start



**手动启动关闭：**

su - postgres

pg_ctl start -D /usr/local/pgsql/data -o -i 

pg_ctl stop -D /usr/local/pgsql/data -o -i 

pg_ctl restart -D /usr/local/pgsql/data -o -i 



**psql 使用**

[test@test bin]$ ./psql -n dbname -U usrname

stay81=# select count(*) from table1;

 count

-------

  5986

(1 row)

// 退出，使用结束后退出，不然连接一直被挂起

stay81=# /q



**数据备份还原**

// postgres 数据导出

[postgres@test bin]$ ./pg_dump -U postgres -t testTable -d testDB > testTable.dump

// postgres 数据导入

[postgres@test bin]$ ./psql -U postgres -d testDB < testTable.dump

// postgres 数据导出

[postgres@test bin]$ ./pg_dump -U postgres -d stay81 -t item_search -F c -v > item_search.dump

// postgres 数据导入

[postgres@test bin]$ ./pg_restore -U postgres -d testDB < testTable.dump

// 一台主机上将数据库目录转储到另一台主机上

pg_dump -h host1 -p post1 dbname | psql -h host2 -p post1 dbname

./pg_dump -U postgres -d testDB1 -t testTable | ./psql -U postgres -d testDB2



