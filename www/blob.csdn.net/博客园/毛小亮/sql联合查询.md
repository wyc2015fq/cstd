# sql联合查询 - 毛小亮 - 博客园
# [sql联合查询](https://www.cnblogs.com/xianerwonder/p/4325109.html)
联合查询效率较高.以下例子来说明联合查询的好处
t1表结构(用户名,密码)    userid int         username   varchar(20)     password       varchar(20)
                                                  1                    jack                           jackpwd
                                                   2                    owen                        owenpwd
t3表结构(用户积分,等级)    userid int         jf   int                   dj           int
                                                1                     20                       3
                                                 3                     50                       6
第一:内联(inner join)
如果想把用户信息,积分,等级都列出来.那么一般会这样写
select * from t1 ,t3 where t1.userid = t3.userid   其实这样的结果等同于select * from t1 inner join t3 on t1.userid=t3.userid
就是把两个表中都存在userid的行拼成一行.这是内联.但后者的效率会比前者高很多.建议用后者的写法.
运行结果:userid    username password     userid   jf    dj
                   1             jack        jacjpwd      1          20   3
第二:左联（left outer join）显示左表中的所有行
select * from t1 left outer join t3 on t1.userid=t3.userid
运行结果：userid    username password     userid   jf     dj
　　　　　　1        jack          jackpwd       1         20   3
                          2       owen         owenpwd     NULL NULL   NULL
第三:右联（right outer join）显示右表中的所有行
select * from t1 right outer join t3 on t1.userid=t3.userid
运行结果：userid    username password     userid   jf     dj
　　　　　　1        jack          jackpwd       1         20   3
                         Null    Null           Null              3        50     6
第四:全联（full outer join）显示两边表中所有行
select * from t1 full outer join t3 on t1.userid=t3.userid
运行结果：userid    username password     userid   jf     dj
　　　　　　1        jack          jackpwd       1         20   3
                          2       owen         owenpwd     NULL NULL   NULL
                         Null    Null           Null              3        50     6
总结，关于联合查询，本人已测试过．效率的确比较高，４种联合方式如果可以灵活使用，基本上复杂的语句结构也会简单起来．这４种方式是：
Inner join          left outer join           right outer join        full outer join

