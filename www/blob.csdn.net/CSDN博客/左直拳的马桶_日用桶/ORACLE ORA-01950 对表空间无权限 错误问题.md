# ORACLE ORA-01950 对表空间无权限 错误问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年03月22日 15:37:02[左直拳](https://me.csdn.net/leftfist)阅读数：3153
oracle中，插入记录失败，提示无权限，类似下图：
![这里写图片描述](https://img-blog.csdn.net/20180322150909252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我用的是A账号（默认表空间A），在表空间B中的表插入记录。结果就提示了上面的错误。查询是没问题的，插入则不行。
```
INSERT INTO B.Real_Hyz_Oneminute_Water (FID, STATIONNO, DT, WT, SL, WL, CREATETIME, ID) 
VALUES (6, 'eeee', sysdate, 0.0, 0.0, 0.0, sysdate,543);
```
这可能是A账号的权限不够？
```
alter user A quota unlimited on base;
grant  unlimited tablespace to A;
grant insert any table to A;
```
如是这般，都不起作用。
网上寻寻觅觅，答案大同小异，说的无非都是`grant  unlimited tablespace to username`
问题是，这个username是A还是B？现在是A去访问B，按道理应该给A赋权限。但不起作用。后来我用B来访问自己，结果插入也不行，照样提示对表空间B无权限。真奇怪啊。 
试着
`grant  unlimited tablespace to B;`
结果就可以了。其实呢，
`INSERT INTO B.TableName 。。。`
其中的B，并非表空间，而是schema。schema是一个解释不清的东西，可以将它简单地等同于用户，但绝不是表空间。
