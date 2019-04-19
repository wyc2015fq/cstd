# 利用系统缓存提高PostgreSQL操作效率 - 文章 - 伯乐在线
原文出处： [PGSmith](https://my.oschina.net/Suregogo/blog/546516)
环境介绍：
OS：Centos 6.4 64bit
Database：PostgreSQL9.4
Memory：2G
CPU：1核
下载安装：
在[pgfoundry](http://pgfoundry.org/frs/?group_id=1000444&release_id=1915#pgfincore-_1.1.1-title-content)下载[pgfincore-v1.1.1.tar.gz](http://pgfoundry.org/frs/download.php/3186/pgfincore-v1.1.1.tar.gz)，，将源码解压到数据库源码下的contrib下。不要在其github上下载，目前应该有一些bug，最新版本为1.1.1，1.1.2在我试用的时候发现大部分函数无法使用。
其次在我编译时一直出现如下错误：

Shell
```
[postgres@localhost pgfincore-1.1.1]$ make
/bin/sh: pg_config: command not found
make: pg_config: Command not found
cp pgfincore.sql pgfincore--1.1.1.sql
cp: cannot create regular file `pgfincore--1.1.1.sql': Permission denied
make: *** [pgfincore--1.1.1.sql] Erro<code>r 1
```
所以在此我修改了一下Makefile文件，内容如下：

Shell
```
# contrib/pgfincore/Makefile
MODULE_big = pgfincore
OBJS = pgfincore.o
EXTENSION = pgfincore
DATA = pgfincore--1.1.1.sql pgfincore--unpackaged--1.1.1.sql
ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pgfincore
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
```
现在进行make，即可：

Shell
```
[postgres@localhost pgfincore-1.1.1]$ make
gcc -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -Wmissing-format-attribute -Wformat-security -fno-strict-aliasing -fwrapv -g -O2 -fpic -I. -I. -I../../src/include -D_GNU_SOURCE   -c -o pgfincore.o pgfincore.c
pgfincore.c: In function ‘pgsysconf’:
pgfincore.c:227: warning: implicit declaration of function ‘heap_form_tuple’
pgfincore.c:227: warning: assignment makes pointer from integer without a cast
pgfincore.c: In function ‘pgfadvise’:
pgfincore.c:477: warning: assignment makes pointer from integer without a cast
pgfincore.c: In function ‘pgfadvise_loader’:
pgfincore.c:710: warning: assignment makes pointer from integer without a cast
pgfincore.c: In function ‘pgfincore’:
pgfincore.c:1016: warning: assignment makes pointer from integer without a cast
gcc -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -Wmissing-format-attribute -Wformat-security -fno-strict-aliasing -fwrapv -g -O2 -fpic -shared -o pgfincore.so pgfincore.o -L../../src/port -L../../src/common -Wl,--as-needed -Wl,-rpath,'/opt/hg3.0/lib',--enable-new-dtags  
[postgres@localhost pgfincore-1.1.1]$ make install
/bin/mkdir -p '/opt/hg3.0/lib/postgresql'
/bin/mkdir -p '/opt/hg3.0/share/postgresql/extension'
/bin/mkdir -p '/opt/hg3.0/share/postgresql/extension'
/usr/bin/install -c -m 755  pgfincore.so '/opt/hg3.0/lib/postgresql/pgfincore.so'
/usr/bin/install -c -m 644 pgfincore.control '/opt/hg3.0/share/postgresql/extension/'
/usr/bin/install -c -m 644 pgfincore--1.1.1.sql pgfincore--unpackaged--1.1.1.sql '/opt/hg3.0/share/postgresql/extension/'
[postgres@localhost pgfincore-1.1.1]$
```
实验记录：
1、安装：

Shell
```
[postgres@localhost bin]$ ./psql 
psql (9.4.5)
Type "help" for help.
postgres=# create extension pgfincore ;
CREATE EXTENSION
postgres=# select * from pg_extension ;
  extname  | extowner | extnamespace | extrelocatable | extversion | extconfig |
 extcondition 
-----------+----------+--------------+----------------+------------+-----------+
--------------
 plpgsql   |       10 |           11 | f              | 1.0        |           |
 
 pgfincore |       10 |         2200 | t              | 1.1.1      |           |
 
(2 rows)
postgres=#
```
2、准备工作：记录比较使用pgfincore前后的性能差，使用插件pg_stat_statments记录每条sql的使用时间。
其次将shared_buffer改为16mb，减少其对pgfincore的影响。

Shell
```
postgres=# create extension pg_stat_statements ;
CREATE EXTENSION
postgres=# select * from pg_extension ;
      extname       | extowner | extnamespace | extrelocatable | extversion | ex
tconfig | extcondition 
--------------------+----------+--------------+----------------+------------+---
--------+--------------
 plpgsql            |       10 |           11 | f              | 1.0        |   
        | 
 pgfincore          |       10 |         2200 | t              | 1.1.1      |   
        | 
 pg_stat_statements |       10 |         2200 | t              | 1.2        |   
        | 
(3 rows)
```
3、建立表testmem，分别在使用和不适用pgfincore情况下进行select, update, delete的操作，并对比执行时间，在这里为了更好的去作比较，建立三张表testmem1，testmem2，testmem3，分别进行三种操作进行对比。并插入相同的数据。

Shell
```
[postgres@localhost bin]$ ./psql 
psql (9.4.5)
Type "help" for help.
postgres=# d
No relations found.
postgres=# create table testmem1(t1 int, t2 text, t3 text, t4 bigint, t5 text, t6 text, t7 varchar, t8 char(22), t9 numeric);CREATE TABLE
postgres=# create table testmem2(t1 int, t2 text, t3 text, t4 bigint, t5 text, t6 text, t7 varchar, t8 char(22), t9 numeric);
CREATE TABLE
postgres=# create table testmem3(t1 int, t2 text, t3 text, t4 bigint, t5 text, t6 text, t7 varchar, t8 char(22), t9 numeric);
CREATE TABLE
postgres=# insert into testmem1 select generate_series(1,999999), 'wangshuo'||generate_series(1,999999)::text||random()::text, generate_series(1,999999)::text||'sure',generate_series(1,999999), random()::text,random()::text,random()::text,'liuyuanyuan', generate_series(1,999999);
INSERT 0 999999
postgres=# select pg_relation_size('testmem1')/1024/1024||'MB';
 ?column? 
----------
 166MB
(1 row)
postgres=# insert into testmem2 select generate_series(1,999999), 'wangshuo'||generate_series(1,999999)::text||random()::text, generate_series(1,999999)::text||'sure',generate_series(1,999999), random()::text,random()::text,random()::text,'liuyuanyuan', generate_series(1,999999);
INSERT 0 999999
postgres=# insert into testmem3 select generate_series(1,999999), 'wangshuo'||generate_series(1,999999)::text||random()::text, generate_series(1,999999)::text||'sure',generate_series(1,999999), random()::text,random()::text,random()::text,'liuyuanyuan', generate_series(1,999999);
INSERT 0 999999
postgres=# select pg_relation_size('testmem2')/1024/1024||'MB'; ?column? 
----------
 166MB
(1 row)
postgres=# select pg_relation_size('testmem3')/1024/1024||'MB';
 ?column? 
----------
 166MB
(1 row)
postgres=#
```
三种操作性能对比：

Shell
```
postgres=# select * from testmem1 where t1  1208;
   t1   |                 t2                 |     t3     |   t4   |          t5
          |          t6          |          t7          |           t8          
 |   t9   
--------+------------------------------------+------------+--------+------------
----------+----------------------+----------------------+-----------------------
-+--------
   1209 | wangshuo12090.964191045146435      | 1209sure   |   1209 | 0.075409890
152514    | 0.468206159770489    | 0.692714618053287    | liuyuanyuan           
 |   1209
   1210 | wangshuo12100.652063825167716      | 1210sure   |   1210 | 0.026932121
720165    | 0.802233531605452    | 0.706556385848671    | liuyuanyuan           
 |   1210
   1211 | wangshuo12110.724962076637894      | 1211sure   |   1211 | 0.732285636
477172    | 0.816857317462564    | 0.868489125277847    | liuyuanyuan           
 |   1211
   1212 | wangshuo12120.0478062951005995     | 1212sure   |   1212 | 0.190716865
006834    | 0.898683389648795    | 0.537546805106103    | liuyuanyuan           
 |   1212
   1213 | wangshuo12130.582098880317062      | 1213sure   |   1213 | 0.161297460
086644    | 0.395338968373835    | 0.684920639265329    | liuyuanyuan           
 |   1213
   1214 | wangshuo12140.53120110463351       | 1214sure   |   1214 | 0.253457680
810243    | 0.428548218682408    | 0.671272001229227    | liuyuanyuan           
postgres=# update testmem2 set t7 = random()::text where t1  1208;
UPDATE 788223
postgres=# delete from testmem3 where t1  1208;
DELETE 788223
```
使用pgfincore前：通过查询表pg_stat_statments查看三种操作时间：

Shell
```
postgres=# SELECT query, calls, total_time, rows, 100.0 * shared_blks_hit /                
               nullif(shared_blks_hit + shared_blks_read, 0) AS hit_percent
          FROM pg_stat_statements ORDER BY total_time DESC;
-[ RECORD 1 ]------------------------------------------------------
query       | update testmem2 set t7 = random()::text where t1  ?;
calls       | 1
total_time  | 14303.53
rows        | 788223
hit_percent | 97.7162513440240383
-[ RECORD 2 ]------------------------------------------------------
query       | delete from testmem3 where t1  ?;
calls       | 1
total_time  | 5256.305
rows        | 788223
hit_percent | 97.4004941321803582
-[ RECORD 6 ]------------------------------------------------------
query       | select * from testmem1 where t1  ?;
calls       | 1
total_time  | 2397.866
rows        | 788223
hit_percent | 0.15039714245429336843
```
使用pgfincore后：通过查询表pg_stat_statments查看三种操作时间（重启机器后，重新建库，重新建表、插数据）：

Shell
```
SELECT query, calls, total_time, rows, 100.0 * shared_blks_hit /
               nullif(shared_blks_hit + shared_blks_read, 0) AS hit_percent
          FROM pg_stat_statements ORDER BY total_time DESC;
-[ RECORD 1 ]------------------------------------------------------
query       | update testmem2 set t7 = random()::text where t1  ?;
calls       | 1
total_time  | 6800.237
rows        | 788223
hit_percent | 97.6908892215405358
-[ RECORD 2 ]------------------------------------------------------
query       | delete from testmem3 where t1  ?;
calls       | 1
total_time  | 3295.312
rows        | 788223
hit_percent | 97.3715873996294009
-[ RECORD 3 ]------------------------------------------------------
query       | select * from testmem1 where t1  ?;
calls       | 1
total_time  | 891.002
rows        | 788223
hit_percent | 0.000000000000000000000000
```
明显看出当执行select时速度提升明显，其他的时间提升并不明显，这是因为数据还需要进行IO操作，所以在这里速度提升应该仅仅是读入的时间节省掉了。

Shell
```
postgres=# select pgfadvise_willneed('testmem1');
-[ RECORD 1 ]------+-------------------------------------
pgfadvise_willneed | (base/13003/16384,4096,42554,279311)
postgres=# select * from pgfincore('testmem1');
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 42554
group_mem     | 1
os_pages_free | 279317
databit       | 
postgres=# select count(*) from testmem1 where t1  12;
-[ RECORD 1 ]
count | 99986
postgres=# insert into testmem1 select generate_series(1,9999), 'wangshuo'||generate_series(1,9999)::text||random()::text, generate_series(1,9999)::text||'sure',generate_series(1,9999), random()::text,random()::text,random()::text,'liuyuanyuan', generate_series(1,9999);
INSERT 0 9999
postgres=# select * from pgfincore('testmem1');-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42980
pages_mem     | 42980
group_mem     | 1
os_pages_free | 277433
databit       | 
postgres=# select pg_relation_size('testmem1');
-[ RECORD 1 ]----+----------
pg_relation_size | 176046080
postgres=# select 42980*4096;
-[ RECORD 1 ]-------
?column? | 176046080
postgres=# insert into testmem1 select generate_series(1,999999), 'wangshuo'||generate_series(1,999999)::text||random()::text, generate_series(1,999999)::text||'sure',generate_series(1,999999), random()::text,random()::text,random()::text,'liuyuanyuan', generate_series(1,999999);
INSERT 0 999999
postgres=# select * from pgfincore('testmem1');
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 85532
pages_mem     | 6188
group_mem     | 22
os_pages_free | 403050
databit       | 
postgres=#
```
上面这个实验验证了willneed会将小部分新加入的数据加载到cache中，能够快速的定位查找，但是当插入数据量较大时，就不会及时的写入到cache中了。
总结：pgfincore目前适用于数据量非常大、更新不频繁、更新量较小的表去加载到缓存中，有助于提高应用效率。
插件函数介绍：
这里会对函数进行介绍，并对所有函数进行操作实验。
1、pgsysconf：
这个函数输出OS block的大小（os_page_size），OS中剩余的page数(os_pages_free)和OS拥有的page总数(os_total_pages)。

Shell
```
postgres=# select * from pgsysconf()；
 os_page_size | os_pages_free | os_total_pages 
--------------+---------------+----------------
         4096 |        167445 |         476688
(1 row)
```
2、pgsysconf_pretty：
这个函数和上面的一样，仅仅是输出更易懂些。

Shell
```
postgres=# select * from pgsysconf_pretty();
 os_page_size | os_pages_free | os_total_pages 
--------------+---------------+----------------
 4096 bytes   | 654 MB        | 1862 MB
(1 row)
```
3、pgfadvise_willneed：
这个函数是
输出文件名（relpath），OS block大小（os_page_size），对象占用系统page数（rel_os_pages）,OS剩余的page数（os_pages_free）。

Shell
```
postgres=# select * from pgfadvise_willneed('testmem1');
     relpath      | os_page_size | rel_os_pages | os_pages_free 
------------------+--------------+--------------+---------------
 base/13003/16384 |         4096 |        42554 |        167294
(1 row)
```
4、pgfadvise_dontneed：
这个函数对当前对象设置dontneed标记。dontneed标记的意思就是当操作系统需要释放内存时优先释放标记为dontneed的pages。输出的意义和上面一致，就不多做介绍了。

Shell
```
postgres=# select * from pgfadvise_dontneed('testmem1');
     relpath      | os_page_size | rel_os_pages | os_pages_free 
------------------+--------------+--------------+---------------
 base/13003/16384 |         4096 |        42554 |        332798
(1 row)
```
5、pgfadvise_NORMAL、pgfadvise_SEQUENTIAL、pgfadvise_RANDOM：
和pgfadvise_dontneed大同小异，分别将对象标记为normal、sequential、random，意思按照字面意思理解就可以了。
其实pgfadvise是一个单独的函数，参数是relname,fork,action,含义分别为对象名、文件分支名以及行为id，上面的3、4、5函数都是输入不同参数而设置的函数，其中willneed、dontneed、normal、sequential、random的值分别是10、20、30、40、50，且默认fork为main，即表文件。
这里的pgfadvise主要调用了Linux下的函数posix_fadvise，标记值也是posix_fadvise所需要的。
6、pgfincore：
这个函数是是提供对象在操作系统缓存中的信息的。
它分为三个函数，参数分别为（relname, fork, getdatabit），（relname, getdatabit），（relname），三个参数意思为对象名，进程名（这个地方默认是main），是否要显示databit（很长，注意显示），第一个函数需要全部输入，第二个函数默认fork为main，第三个函数默认fork为main，getdatabit为false。
它输出的是文件位置及名称（relpath），文件顺序（segment），OS page或block大小（os_page_size），对象占用系统缓存需要的页面个数（rel_os_pages），对象已经占用缓存页面个数（pages_mem），在缓存中连续的页面组的个数（group_mem），OS剩余的page数（os_pages_free），加载信息的位图（databit）。

Shell
```
postgres=# select * from pgfincore('testmem1', 'main', false);
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 2
group_mem     | 1
os_pages_free | 325475
databit       | 
postgres=# select * from pgfincore('testmem1', false);
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 2
group_mem     | 1
os_pages_free | 325475
databit       | 
postgres=# select * from pgfincore('testmem1');
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 2
group_mem     | 1
os_pages_free | 325475
databit       |
```
7、pgfadvise_loader：
这个函数可以对页面缓存直接进行两方面的作用。因此，它能通过页面的位图在缓存中来对页面进行加载或卸载。
它分为两个函数和上边的类似，就是设置缺省值，的输入参数是（relname, fork, segment, load, unload, databit）和（relname, segment, load, unload, databit），分别是对象名，文件分支名，文件序号，是否加载，是否卸载，位图信息。第二个函数默认fork为main。
它输出的是物理文件名及path（relpath），OS page或block大小（os_page_size）, OS中剩余的page数(os_pages_free)，加载的page数（pages_load），卸载的page数（pages_unload）。

Shell
```
postgres=# select * from pgfincore('testmem1');
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 0
group_mem     | 0
os_pages_free | 331290
databit       | 
postgres=# select * from pgfadvise_loader('testmem1', 0, true, true, B'1100');
-[ RECORD 1 ]--+-----------------
relpath        | base/13003/16384
os_page_size   | 4096
os_pages_free  | 331294
pages_loaded   | 2
pages_unloaded | 2
postgres=# select * from pgfincore('testmem1');
-[ RECORD 1 ]-+-----------------
relpath       | base/13003/16384
segment       | 0
os_page_size  | 4096
rel_os_pages  | 42554
pages_mem     | 2
group_mem     | 1
os_pages_free | 331290
databit       |
```
