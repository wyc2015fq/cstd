# JdbcTemplate的queryForMap方法报错 - z69183787的专栏 - CSDN博客
2013年02月06日 13:33:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：26320
JdbcTemplate的queryForMap方法报错 
queryForMap方法使用不当，就会出错，使用方式如下： 
> 
The queryForMap method in JdbcTemplate only expects a single row to be returned, The return value for this method will be a map of column names to column values for the single result row.
queryForMap方法返回的结果集大小必须是1，并且返回的map中，以列的名字作为key，获取的值作为value 
|`1`|`public``void``test(){`|
|`2`|```String SQL_GET_ATTRIBUTE=``````"select * from test where rownum<1"````;`|
|`3`|```Map<Object,Object> map=getJdbcTemplate().queryForMap(SQL_GET_ATTRIBUTE);```|
|`4`|```Set<Entry<Object, Object>> entrySet=map.entrySet();```|
|`5`|`for````(Entry<Object, Object> entry : entrySet) {```|
|`6`|`System.out.println(````"key is "``````+ entry.getKey());```|
|`7`|`System.out.println(````"value is "``````+ entry.getValue());```|
|`8`|`}`|
|`9`|`}`|
输出会列出test所有字段为key，查询出来的值为vlaue的字符串，这里使用oracle数据库，强制使用“where rownum<1”来限制只返回一行。 
利用spring 的getJdbcTemplate().queryForMap如果返回空集，就会报 
org.springframework.dao.EmptyResultDataAccessException: Incorrect result size: expected 1, actual 0 的异常，解决办法就是捕捉这个异常然后返回null即可。 
Java代码  ![收藏代码](http://chembo.iteye.com/images/icon_star.png)
- try{  
- return getJdbcTemplate().queryForMap("select s.fb,s.pb,s.tu,s.qt,s.fbc,s.pbc,s.tt,s.ft,s.lt from gbc$view_user s where s.ud = ? and ad= ?", new Object[]{ud,ad});  
-         }catch (EmptyResultDataAccessException e) {  
- returnnull;  
-         }  
- 
解决方案 在queryForMap的地方 try catch 一下 即可。
