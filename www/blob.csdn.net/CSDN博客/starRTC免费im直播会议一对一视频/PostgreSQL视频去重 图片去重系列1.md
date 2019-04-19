# PostgreSQL视频去重 图片去重系列1 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月13日 17:52:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：458
PostgreSQL 在视频、图片去重，图像搜索业务中的应用
图片搜索
PostgreSQL的图像搜索插件使用了非常主流的Haar wavelet技术对图像进行变换后存储
gist 索引方法(支持pattern和signature类型), 以及KNN操作符，可以用于搜索相似度
1,建立图片表
create table image (id serial, data bytea);
2,导入图片到数据库
insert into image(data) select pg_read_binary_file('文件全路径');
错误：
ERROR:  absolute path not allowed
PGDATA 为/var/lib/pgsql/9.6/data/，在vi /usr/lib/systemd/system/postgresql-9.5.service里面有
拷贝文件到该目录后
insert into image(data) select pg_read_binary_file('./2/1K7A9699.JPG');

3,将图片转换成 patten 和 signature,很耗时
CREATE TABLE pat AS (SELECT id,	shuffle_pattern(pattern) AS pattern, pattern2signature(pattern) AS signature FROM (	SELECT 	id, jpeg2pattern(data) AS pattern FROM image) x );
4,创建索引
ALTER TABLE pat ADD PRIMARY KEY (id);CREATE INDEX pat_signature_idx ON pat USING gist (signature);
5,相似度排行
SELECT id, smlr FROM (	SELECT id, pattern <-> (SELECT pattern FROM pat WHERE id = 74) AS smlr	    FROM   pat WHERE id <> 74 ORDER BY signature <-> (SELECT signature FROM pat WHERE id = 74) LIMIT 100) x ORDER BY x.smlr ASC LIMIT 10;
**视频去重**
抽取视频中的关键帧
1,建立图片表
2,导入图片到数据库
3,将图片转换成 patten 和 signature
CREATE TABLE pat AS (    SELECT        id, movie_id,        shuffle_pattern(pattern)   AS pattern,         pattern2signature(pattern) AS signature     FROM (        SELECT             id, movie_id,            jpeg2pattern(data) AS pattern         FROM             image    ) x );
create table targer_table as select * from source_table是会复制表结构+表数据，
[https://stackoverflow.com/questions/4629979/nested-select-statement-in-sql-server](https://stackoverflow.com/questions/4629979/nested-select-statement-in-sql-server)
SELECT name FROM (SELECT name FROM agentinformation) 这种是不对的
You need to alias the subquery.
SELECT name FROM (SELECT name FROM agentinformation) a  
or to be more explicit
SELECT a.name FROM (SELECT name FROM agentinformation) a  
即
select x.id, x.movie_id...
4.
计算不同视频的相似度
select t1.movie_id, t1.id, t1.signature<->t2.signature from 
pat t1 join pat t2 on (t1.movie_id<>t2.movie_id) 
order by t1.signature<->t2.signature desc 
or 
select t1.movie_id, t1.id, t1.signature<->t2.signature from 
pat t1 join pat t2 on (t1.movie_id<>t2.movie_id) 
where t1.signature<->t2.signature > 0.9  
order by t1.signature<->t2.signature desc 
多上面红色那一行，其它一样
报错：
ERROR:  function jpeg2pattern(bytea) does not exist
这个是因为扩展imgsmlr安装不正确
\dx命令查看所有已安装的扩展模块
imgsmlr
$ cd imgsmlr$ export PGHOME=/usr/pgsql-9.6$ export PATH=$PGHOME/bin:$PATH:.$ make USE_PGXS=1$ make USE_PGXS=1 install
psql -U postgres -d exampledb -h 127.0.0.1 -p 5432
then exeute the sql.
create extension imgsmlr;
查看下
\dx命令查看所有已安装的扩展模块
然后执行
CREATE TABLE pat AS (SELECT id,	shuffle_pattern(pattern) AS pattern, pattern2signature(pattern) AS signature FROM (	SELECT 	id, jpeg2pattern(data) AS pattern FROM image) x );
记得结束加分号
The idea is to find top N similar images by signature using GiST index. Then find top n (n < N) similar images by pattern from top N similar images by signature.
参考
[https://github.com/digoal/blog/blob/master/201611/20161126_01.md](https://github.com/digoal/blog/blob/master/201611/20161126_01.md)
下一步需要测试增量添加图片
