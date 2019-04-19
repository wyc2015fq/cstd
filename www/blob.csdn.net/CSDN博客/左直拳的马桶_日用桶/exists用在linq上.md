# exists用在linq上 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月21日 16:56:45[左直拳](https://me.csdn.net/leftfist)阅读数：3815
SQL里面，有时候会用到exists或者not exists。
```
select * from yb t1 where not exists(select 1 from yb t2 where trunc(t1.yubaodate,'dd')=trunc(t2.yubaodate,'dd') and t1.yubaotype=t2.yubaotype and t1.id<t2.id)
order by id desc;
```
然后，LINQ里怎么用？
```
var t1 = ybRepository.GetAll();
var t2 = ybRepository.GetAll();
var list = from s in t1
           where !t2.Any(ss => ss.YUBAODATE.Date == s.YUBAODATE.Date
            && ss.YuBaoType == s.YuBaoType
            && ss.ID > s.ID)
           select s;
```
万幸，fluently nhibernate支持这种写法。
