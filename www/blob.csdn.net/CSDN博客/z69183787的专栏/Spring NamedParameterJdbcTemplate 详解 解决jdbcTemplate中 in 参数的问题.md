# Spring NamedParameterJdbcTemplate 详解 解决jdbcTemplate中 in 参数的问题 - z69183787的专栏 - CSDN博客
2014年10月20日 10:42:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：32243
自己的例子：
```java
List<String> l = new ArrayList<String>();
		l.add("2549");l.add("2959");
		NamedParameterJdbcTemplate namedParameterJdbcTemplate = null;  
		//namedParameterJdbcTemplate =  
//		    new NamedParameterJdbcTemplate(dataSource);  
		namedParameterJdbcTemplate =  
		new NamedParameterJdbcTemplate(dbUtil.getJdbcTemplate());  
		  Map<String, Object> paramMap = new HashMap<String, Object>();  
		    paramMap.put("deptIds", l);  
		String sql = "select z.dept_id,z.sid from z_docs_catalog z "
				+ "where z.state = 1 and z.flag = '000'and z.dept_id in (:deptIds)";
		List<Map<String,Object>> result = namedParameterJdbcTemplate.queryForList(sql, paramMap);
```
 NamedParameterJdbcTemplate类是基于JdbcTemplate类，并对它进行了封装从而支持命名参数特性。
NamedParameterJdbcTemplate主要提供以下三类方法：execute方法、query及queryForXXX方法、update及batchUpdate方法。
首先让我们看个例子吧：
java代码：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Test
- publicvoid testNamedParameterJdbcTemplate1() {  
- NamedParameterJdbcTemplate namedParameterJdbcTemplate = null;  
- //namedParameterJdbcTemplate =
- //    new NamedParameterJdbcTemplate(dataSource);
- namedParameterJdbcTemplate =  
- new NamedParameterJdbcTemplate(jdbcTemplate);  
-     String insertSql = "insert into test(name) values(:name)";  
-     String selectSql = "select * from test where name=:name";  
-     String deleteSql = "delete from test where name=:name";  
-     Map<String, Object> paramMap = new HashMap<String, Object>();  
-     paramMap.put("name", "name5");  
-     namedParameterJdbcTemplate.update(insertSql, paramMap);  
- final List<Integer> result = new ArrayList<Integer>();  
- namedParameterJdbcTemplate.query(selectSql, paramMap,  
- new RowCallbackHandler() {  
- @Override
- publicvoid processRow(ResultSet rs) throws SQLException {  
-             result.add(rs.getInt("id"));  
-         }  
-     });  
- Assert.assertEquals(1, result.size());  
- SqlParameterSource paramSource = new MapSqlParameterSource(paramMap);  
- namedParameterJdbcTemplate.update(deleteSql, paramSource);  
- }  
接下来让我们分析一下代码吧：
**1）NamedParameterJdbcTemplate初始化**：可以使用DataSource或JdbcTemplate 对象作为构造器参数初始化；
**2）insert into test(name) values(:name)：**其中“:name”就是命名参数；
**3） update(insertSql, paramMap)：**其中paramMap是一个Map类型，包含键为“name”，值为“name5”的键值对，也就是为命名参数设值的数据；
**4）query(selectSql, paramMap, new RowCallbackHandler()……)：**类似于JdbcTemplate中介绍的，唯一不同是需要传入paramMap来为命名参数设值；
**5）update(deleteSql, paramSource)：**类似于“update(insertSql, paramMap)”，但使用SqlParameterSource参数来为命名参数设值，此处使用MapSqlParameterSource实现，其就是简单封装java.util.Map。
NamedParameterJdbcTemplate类为命名参数设值有两种方式：java.util.Map和SqlParameterSource：
**1）java.util.Map：**使用Map键数据来对于命名参数，而Map值数据用于设值；
**2）SqlParameterSource：**可以使用SqlParameterSource实现作为来实现为命名参数设值，默认有MapSqlParameterSource和BeanPropertySqlParameterSource实现；MapSqlParameterSource实现非常简单，只是封装了java.util.Map；而BeanPropertySqlParameterSource封装了一个JavaBean对象，通过JavaBean对象属性来决定命名参数的值。
java代码：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- package cn.javass.spring.chapter7;  
- publicclass UserModel {  
- privateint id;  
- private String myName;     
- //省略getter和setter     
- }  
java代码：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Test
- publicvoid testNamedParameterJdbcTemplate2() {  
-     NamedParameterJdbcTemplate namedParameterJdbcTemplate = null;  
-     namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(jdbcTemplate);  
-     UserModel model = new UserModel();  
-     model.setMyName("name5");  
-     String insertSql = "insert into test(name) values(:myName)";  
-     SqlParameterSource paramSource = new BeanPropertySqlParameterSource(model);  
-     namedParameterJdbcTemplate.update(insertSql, paramSource);  
- }  
可以看出BeanPropertySqlParameterSource使用能减少很多工作量，但命名参数必须和JavaBean属性名称相对应才可以。
