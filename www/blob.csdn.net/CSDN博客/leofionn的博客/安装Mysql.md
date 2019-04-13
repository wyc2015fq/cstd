
# 安装Mysql - leofionn的博客 - CSDN博客


2018年01月05日 13:06:34[leofionn](https://me.csdn.net/qq_36142114)阅读数：93


##### tar xzvf mysql-5.6.23-linux-glibc2.5-x86_64.tar.gz
##### mv mysql-5.6.23-linux-glibc2.5-x86_64 my

##### 创建用户和组
##### groupadd -g 101 dba
##### useradd -u 514 -g dba -G root -d /usr/local/mysql mysqladmin
##### id mysqladmin
##### passwd mysqladmin

##### copy 环境变量配置文件至mysqladmin用户的home目录中,为了以下步骤配置个人环境变量
##### cp /etc/skel/.* /usr/local/mysql

##### vi /etc/my.cnf
##### [client]
##### port            = 3306
##### socket          = /usr/local/mysql/data/mysql.sock

##### [mysqld]
##### user              =mysqladmin
##### port            = 3306
##### socket          = /usr/local/mysql/data/mysql.sock

##### skip-external-locking
##### key_buffer_size = 256M
##### sort_buffer_size = 2M
##### read_buffer_size = 2M
##### read_rnd_buffer_size = 4M
##### query_cache_size= 32M
##### max_allowed_packet = 16M
##### myisam_sort_buffer_size=128M
##### tmp_table_size=32M

##### table_open_cache = 512
##### thread_cache_size = 8
##### wait_timeout = 86400
##### interactive_timeout = 86400
##### max_connections = 600

##### \# Try number of CPU's*2 for thread_concurrency
##### thread_concurrency = 32

##### \#isolation level and default engine
##### default-storage-engine = INNODB
##### transaction-isolation = READ-COMMITTED

##### server-id  = 1
##### basedir     = /usr/local/mysql
##### datadir     = /usr/local/mysql/data
##### pid-file     = /usr/local/mysql/data/hostname.pid

##### \#open performance schema
##### log-warnings
##### sysdate-is-now

##### binlog_format = MIXED
##### log_bin_trust_function_creators=1
##### log-error  = /usr/local/mysql/data/hostname.err
##### log-bin=/usr/local/mysql/arch/mysql-bin
##### \#other logs
##### \#general_log =1
##### \#general_log_file  = /usr/local/mysql/data/general_log.err
##### \#slow_query_log=1
##### \#slow_query_log_file=/usr/local/mysql/data/slow_log.err

##### \#for replication slave
##### \#log-slave-updates
##### \#sync_binlog = 1

##### \#for innodb options
##### innodb_data_home_dir = /usr/local/mysql/data/
##### innodb_data_file_path = ibdata1:500M:autoextend
##### innodb_log_group_home_dir = /usr/local/mysql/arch
##### innodb_log_files_in_group = 2
##### innodb_log_file_size = 200M

##### innodb_buffer_pool_size =
##### 2048M
##### innodb_additional_mem_pool_size = 50M
##### innodb_log_buffer_size = 16M

##### innodb_lock_wait_timeout = 100
##### \#innodb_thread_concurrency = 0
##### innodb_flush_log_at_trx_commit = 1
##### innodb_locks_unsafe_for_binlog=1

##### \#innodb io features: add for mysql5.5.8
##### performance_schema
##### innodb_read_io_threads=4
##### innodb-write-io-threads=4
##### innodb-io-capacity=200
##### \#purge threads change default(0) to 1 for purge
##### innodb_purge_threads=1
##### innodb_use_native_aio=on

##### \#case-sensitive file names and separate tablespace
##### innodb_file_per_table = 1
##### lower_case_table_names=1

##### [mysqldump]
##### quick
##### max_allowed_packet = 16M

##### [mysql]
##### no-auto-rehash

##### [mysqlhotcopy]
##### interactive-timeout

##### [myisamchk]
##### key_buffer_size = 256M
##### sort_buffer_size = 256M
##### read_buffer = 2M
##### write_buffer = 2M

##### 给予权限
##### chown  mysqladmin:dba /etc/my.cnf
##### chmod  640 /etc/my.cnf
##### 创建文件夹
##### su - mysqladmin
##### mkdir arch
##### 第一次安装
##### scripts/mysql_install_db  --user=mysqladmin --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data




来自@若泽大数据


