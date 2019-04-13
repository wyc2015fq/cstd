
# Oracle判断为空 - 追求卓越,做到专业 - CSDN博客


2012年10月26日 13:54:08[Waldenz](https://me.csdn.net/enter89)阅读数：1649


null只能通过is null 或者 is not null 来判断,其他操作符与null操作都是false.
select * from table where col = null (错误的,查不出正确的结果)
select * from table where col is null (正确的)



