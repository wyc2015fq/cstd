# nhibernate实体类主键ID赋值问题 - 左直拳的马桶_日用桶 - CSDN博客
2016年08月26日 20:22:28[左直拳](https://me.csdn.net/leftfist)阅读数：1907
有个同事忽然来找我，说他遇到了一个问题，在调用nhibernate 进行update数据的时候报错，说是有数据行锁定。
看代码，没啥问题。
直接在PL/SQL developer里对数据库进行插入，也没啥问题。应该不是有事务没提交，表被锁了的情况。但我们水平低，为防万一，还是重启了数据库。好不容易重启后再试，问题依旧在，几度夕阳红。
后来将update改为insert，发现还是错。找到报错的详细信息，提示用到了一个大约叫”seq_hibernate”（记得不是很清楚了）的序列。我们没有定义过这个序列，难道这是一个nhibernate自带的序列？
找来找去，发现实体类代码这里，主键id没有定义如何赋值。
一般地，nhibernate中，实体类映射，对id的赋值有俩方式
`Id(s => s.ID).GeneratedBy.Assigned(); //直接赋值``Id(s => s.ID).GeneratedBy.Sequence("SEQ_WebSite"); //用序列`
但是我那个同事，是这样写的：
`Id(s => s.ID);`
啥方式都没指定。猜测是因为没指定主键ID的赋值方式，nhibernate采用了默认方式，用默认的序列seq_hibernate(?)来为它产生值。天知道这个seq_hibernate可能是很多地方都在请求，于是就报了锁定的错误。
将ID采用类似这种方式，指定序列后，问题解决。
`Id(s => s.ID).GeneratedBy.Sequence("SEQ_WebSite"); //用序列`
