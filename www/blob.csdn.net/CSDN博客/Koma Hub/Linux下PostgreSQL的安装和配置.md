# Linux下PostgreSQL的安装和配置 - Koma Hub - CSDN博客
2019年03月16日 13:40:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：29
**Table of Contents**
[一、在CentOS上安装PostgreSQL数据库](#%E4%B8%80%E3%80%81%E5%9C%A8CentOS%E4%B8%8A%E5%AE%89%E8%A3%85PostgreSQL%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[1.使用yum安装postgresql](#1.%E4%BD%BF%E7%94%A8yum%E5%AE%89%E8%A3%85postgresql%C2%A0)
[2.初始化postgresql数据库](#2.%E5%88%9D%E5%A7%8B%E5%8C%96postgresql%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[2.启动postgresql服务](#2.%E5%90%AF%E5%8A%A8postgresql%E6%9C%8D%E5%8A%A1%C2%A0)
[3.查看postgresql的服务状态](#3.%E6%9F%A5%E7%9C%8Bpostgresql%E7%9A%84%E6%9C%8D%E5%8A%A1%E7%8A%B6%E6%80%81%C2%A0)
[二、连接PostgreSQL数据库](#%E4%BA%8C%E3%80%81%E8%BF%9E%E6%8E%A5PostgreSQL%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[1.切换的postgres用户，并连接数据库](#1.%E5%88%87%E6%8D%A2%E7%9A%84postgres%E7%94%A8%E6%88%B7%EF%BC%8C%E5%B9%B6%E8%BF%9E%E6%8E%A5%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[2.列出所有的数据库](#2.%E5%88%97%E5%87%BA%E6%89%80%E6%9C%89%E7%9A%84%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[3.退出数据库](#3.%E9%80%80%E5%87%BA%E6%95%B0%E6%8D%AE%E5%BA%93%C2%A0)
[三、PostgreSQL数据库目录](#%E4%B8%89%E3%80%81PostgreSQL%E6%95%B0%E6%8D%AE%E5%BA%93%E7%9B%AE%E5%BD%95%C2%A0)
[四、PostgrepSQL的简单配置](#%E5%9B%9B%E3%80%81PostgrepSQL%E7%9A%84%E7%AE%80%E5%8D%95%E9%85%8D%E7%BD%AE%C2%A0)
[1.修改监听的ip和端口](#1.%E4%BF%AE%E6%94%B9%E7%9B%91%E5%90%AC%E7%9A%84ip%E5%92%8C%E7%AB%AF%E5%8F%A3%C2%A0)
[2.修改数据库log相关的参数](#2.%E4%BF%AE%E6%94%B9%E6%95%B0%E6%8D%AE%E5%BA%93log%E7%9B%B8%E5%85%B3%E7%9A%84%E5%8F%82%E6%95%B0%C2%A0)
[3.内存参数的配置](#3.%E5%86%85%E5%AD%98%E5%8F%82%E6%95%B0%E7%9A%84%E9%85%8D%E7%BD%AE%C2%A0)
# 一、在CentOS上安装PostgreSQL数据库 
下面我们就以CentOS系统为例，给大家讲解一下PostgreSQL的安装过程。 
## 1.使用yum安装postgresql 
`sudo yum install postgresql-server.x86_64 `
## 2.初始化postgresql数据库 
```
$ sudo service postgresql initdb 
Initializing database: [  OK  ]
```
## 2.启动postgresql服务 
```
$ sudo service postgresql start 
Starting postgresql service: [  OK  ]
```
## 3.查看postgresql的服务状态 
```
$ sudo service postgresql status 
postmaster (pid  3496) is running...
```
# 二、连接PostgreSQL数据库 
如果想连接到数据库，需要切换到postgres用户下，然后使用psql连接到数据库中。在该用户下连接数据库，是不需要密码的。 
## 1.切换的postgres用户，并连接数据库 
```
$ sudo su - postgres 
-bash-4.1$ psql 
psql (8.4.20) 
Type "help" for help. 
postgres=#
```
## 2.列出所有的数据库 
```
postgres=# \l 
                                  List of databases 
   Name    |  Owner   | Encoding |  Collation  |    Ctype    |   Access privileges    
-----------+----------+----------+-------------+-------------+----------------------- 
 postgres  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |  
 template0 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres 
                                                             : postgres=CTc/postgres 
 template1 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres 
                                                             : postgres=CTc/postgres 
(3 rows)
```
## 3.退出数据库 
```
postgres=# \q 
-bash-4.1$
```
# 三、PostgreSQL数据库目录 
默认安装上，PostgreSQL的数据库目录在/var/lib/pgsql/data目录。 
```
-bash-4.1$ ls -l /var/lib/pgsql/data/ 
total 80 
drwx------. 5 postgres postgres  4096 Nov 16 23:55 base 
drwx------. 2 postgres postgres  4096 Nov 16 23:55 global
```
# 四、PostgrepSQL的简单配置 
PostgreSQL数据库的配置主要是通过修改数据目录下的postgresql.conf文件来实现的。 
## 1.修改监听的ip和端口 
使用postgresql用户连接数据库后，进入到/var/lib/pgsql/data目录下，编辑postgresql.conf文件： 
```
# - Connection Settings - 
#listen_addresses = '*'                 # what IP address(es) to listen on; 
                                        # comma-separated list of addresses; 
                                        # defaults to 'localhost', '*' = all 
                                        # (change requires restart) 
#port = 5432                            # (change requires restart)
```
修改这两个参数之后，需要重启之后才能生效 
```
[pengchengxiang@localhost ~]$ sudo service postgresql restart 
Stopping postgresql service: [  OK  ] 
Starting postgresql service: [  OK  ]
```
## 2.修改数据库log相关的参数 
日志收集，一般是打开的
```
# This is used when logging to stderr: 
logging_collector = on                  # Enable capturing of stderr and csvlog 
                                        # into log files. Required to be on for 
                                        # csvlogs. 
                                        # (change requires restart)
```
日志目录，一般使用默认值 
```
# These are only used if logging_collector is on: 
log_directory = 'pg_log'                # directory where log files are written, 
                                        # can be absolute or relative to PGDATA
```
只保留一天的日志，进行循环覆盖 
```
log_filename = 'postgresql-%a.log'      # log file name pattern, 
                                        # can include strftime() escapes 
log_truncate_on_rotation = on           # If on, an existing log file of the 
                                        # same name as the new log file will be 
                                        # truncated rather than appended to. 
                                        # But such truncation only occurs on 
                                        # time-driven rotation, not on restarts 
                                        # or size-driven rotation.  Default is 
                                        # off, meaning append to existing files 
                                        # in all cases. 
log_rotation_age = 1d                   # Automatic rotation of logfiles will 
                                        # happen after that time.  0 disables. 
log_rotation_size = 0                   # Automatic rotation of logfiles will
```
## 3.内存参数的配置 
共享内存的大小，用于共享数据块。如果你的机器上有足够的内存，可以把这个参数改的大一些，这样数据库就可以缓存更多的数据块，当读取数据时，就可以从共享内存中读，而不需要再从文件上去读取。 
```
# - Memory - 
shared_buffers = 32MB                   # min 128kB 
                                        # (change requires restart)
```
单个SQL执行时，排序、hash json所用的内存，SQL运行完后，内存就释放了。 
```
# actively intend to use prepared transactions. 
#work_mem = 1MB                         # min 64kB
```
> 
转自：[1024工场](https://blog.csdn.net/p106786860)
原文：[PostgreSQL：PostgreSQL的安装和配置](https://blog.csdn.net/p106786860/article/details/49877483)
