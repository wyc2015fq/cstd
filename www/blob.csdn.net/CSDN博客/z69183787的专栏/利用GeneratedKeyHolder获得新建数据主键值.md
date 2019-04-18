# 利用GeneratedKeyHolder获得新建数据主键值 - z69183787的专栏 - CSDN博客
2016年07月11日 17:31:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1208
Spring利用GeneratedKeyHolder，提供了一个可以返回新增记录对应主键值的方法： 
int update(PreparedStatementCreator psc, KeyHolder generatedKeyHolder)
Spring为KeyHolder接口指代了一个通用的实现类GeneratedKeyHolder，该类返回新增记录时的自增长主键值。假设我们希望在新增论坛板块对象后，希望将主键值加载到对象中，则可以按以下代码进行调整：
Java代码  ![收藏代码](http://xuganggogo.iteye.com/images/icon_star.png)
- final String INSERT_SQL = "insert into gtable(username,callback) values(?,?)";  
-         KeyHolder keyHolder = new GeneratedKeyHolder();  
-         jdbcTemplate.update(new PreparedStatementCreator() {  
- public PreparedStatement createPreparedStatement(  
-                     Connection connection) throws SQLException {  
-                 PreparedStatement ps = connection.prepareStatement(INSERT_SQL,  
- new String[] { "id" });  
-                 ps.setString(1, request.getUsername());  
-                 ps.setString(2, request.getCallback());  
- return ps;  
-             }  
-         }, keyHolder);  
- logger.debug("id:" + keyHolder.getKey()+"long:"+keyHolder.getKey().longValue());  
