# JdbcTemplate.queryForObject 返回值怎么判断 - z69183787的专栏 - CSDN博客
2014年05月22日 16:42:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3251
org.springframework.dao.EmptyResultDataAccessException: Incorrect result size: expected 1, actual 0 
spring做了封装, queryForObject回去数据库查询一条记录,如果没有记录符合查询条件,或者有多条记录符合条件,都会报出错误, 
只不过 expected 1, actual 0 意味这没有记录符合这查询条件. 
expected 1 可以理解为期望有一条且只有一条符合条件的记录, 
actual 0 意味着,没有记录符合. 
如果 actual *  , * 为  > 1 ,那么也会出错.(这个方法就是这么规定的..) 
可以这样写,假设一个简单的表,user.字段有: 
id 和  name. 
queryForInt(select count(1) as c from user where id = ?) 
在结果集中取 c 的值 ,如果大于0,就可以认为有1条或者多条记录. 
