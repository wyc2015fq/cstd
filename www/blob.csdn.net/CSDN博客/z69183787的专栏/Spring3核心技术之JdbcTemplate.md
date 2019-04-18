# Spring3核心技术之JdbcTemplate - z69183787的专栏 - CSDN博客
2013年01月28日 14:47:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7043
**Spring对JDBC的支持**
       Spring通过抽象JDBC访问并提供一致的API来简化JDBC编程的工作量。我们只需要声明SQL、调用合适的Spring JDBC框架API、处理结果集即可。事务由Spring管理，并将JDBC受查异常转换为Spring一致的非受查异常，从而简化开发。
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- ....  
-     Connection conn = null;    
-     PreparedStatement pstmt = null;  
- try {  
-       conn = getConnection();              //1.获取JDBC连接    
-       String sql = "select * from tableName"; //2.声明SQL
-       pstmt = conn.prepareStatement(sql);    //3.预编译SQL  
-       ResultSet rs = pstmt.executeQuery();   //4.执行SQL  
-       process(rs);                       //5.处理结果集  
-       closeResultSet(rs);                 //5.释放结果集  
-       closeStatement(pstmt);              //6.释放Statement  
-       conn.commit();                    //8.提交事务  
-     } catch (Exception e) {    
- //9.处理异常并回滚事务  
-       conn.rollback();    
- throw e;    
-     } finally {    
- //10.释放JDBC连接，防止JDBC连接不关闭造成的内存泄漏  
-       closeConnection(conn);    
-     }  
- ....  
以上代码片段具有冗长、重复、容易忘记某一步骤从而导致出错、显示控制事务、显示处理受检查异常等等。 
Spring主要提供JDBC模板方式、关系数据库对象化方式和SimpleJdbc方式三种方式来简化JDBC编程，这三种方式就是Spring JDBC的工作模式：
**Spring JDBC抽象框架core包提供了JDBC模板类，其中JdbcTemplate是core包的核心类，所以其他模板类都是基于它封装完成的，JDBC模板类是第一种工作模式。**
JdbcTemplate类通过模板设计模式帮助我们消除了冗长的代码，只做需要做的事情（即可变部分），并且帮我们做哪些固定部分，如连接的创建及关闭。 
![](http://dl.iteye.com/upload/attachment/0073/4669/f555c477-5d63-3ec2-afb4-d84450366bf6.jpg)
- **JDBC模板方式**：Spring JDBC框架提供以下几种模板类来简化JDBC编程，实现GoF模板设计模式，将可变部分和非可变部分分离，可变部分采用回调接口方式由用户来实现：如JdbcTemplate、NamedParameterJdbcTemplate、SimpleJdbcTemplate。
- **关系数据库操作对象化方式**：Spring JDBC框架提供了将关系数据库操作对象化的表示形式，从而使用户可以采用面向对象编程来完成对数据库的访问；如MappingSqlQuery、SqlUpdate、SqlCall、SqlFunction、StoredProcedure等类。这些类的实现一旦建立即可重用并且是线程安全的。
- **SimpleJdbc方式**：Spring JDBC框架还提供了SimpleJdbc方式来简化JDBC编程，SimpleJdbcInsert 、 SimpleJdbcCall用来简化数据库表插入、存储过程或函数访问。
Spring JDBC还提供了一些强大的工具类，如DataSourceUtils来在必要的时候手工获取数据库连接等。
**Spring的JDBC架构**
Spring JDBC抽象框架由四部分组成：datasource、support、core、object如图所示。 
![](http://dl.iteye.com/upload/attachment/0073/4671/8cdec743-5ab7-360b-93c8-258cd83e7b10.jpg)
- **support包**：提供将JDBC异常转换为DAO非检查异常转换类、一些工具类如JdbcUtils等。
- **datasource包**：提供简化访问JDBC 数据源（javax.sql.DataSource实现）工具类，并提供了一些DataSource简单实现类从而能使从这些DataSource获取的连接能自动得到Spring管理事务支持。
- **core包**：提供JDBC模板类实现及可变部分的回调接口，还提供SimpleJdbcInsert等简单辅助类。
- **object包**：提供关系数据库的对象表示形式，如MappingSqlQuery、SqlUpdate、SqlCall、SqlFunction、StoredProcedure等类，该包是基于core包JDBC模板类实现。
![](http://dl.iteye.com/upload/attachment/0073/5832/d2f82897-6872-3d57-b254-e42ea558711b.jpg)
SpringJDbc工程中需要引入以下JAR包： 
![](http://dl.iteye.com/upload/attachment/0073/5849/776be760-e8c5-351a-831e-9658ba581316.jpg)
**Spring JDBC提供如下DaoSupport实现**： 
● JdbcDaoSupport：用于支持一致的JdbcTemplate访问； 
● NamedParameterJdbcDaoSupport:继承JdbcDaoSupport，同时提供NamedParameterJdbcTemplate访问； 
● SimpleJdbcDaoSupport：继承JdbcDaoSupport，同时提供SimpleJdbcTemplate访问。 
由于JdbcTemplate、NamedParameterJdbcTemplate、SimpleJdbcTemplate类使用DataSourceUtils获取及释放连接，而且连接是与线程绑定的，因此这些JDBC模板类是线程安全的，即JdbcTemplate对象可以在多线程中重用
**JdbcTemplate**
JdbcTemplate主要提供以下五类方法： 
- execute()：可以用于执行任何SQL语句，一般用于执行DDL语句；
- update()及batchUpdate()：update方法用于执行新增、修改、删除等语句；batchUpdate方法用于执行批处理相关语句；
- query()及queryForXXX()：用于执行查询相关语句；
- call()：用于执行存储过程、函数相关语句。
JdbcTemplate类支持的回调类：
● 预编译语句及存储过程创建回调：用于根据JdbcTemplate提供的连接创建相应的语句； 
         PreparedStatementCreator：通过回调获取JdbcTemplate提供的Connection，由用户使用该Conncetion创建相关的PreparedStatement； 
         CallableStatementCreator：通过回调获取JdbcTemplate提供的Connection，由用户使用该Conncetion创建相关的CallableStatement； 
● 预编译语句设值回调：用于给预编译语句相应参数设值； 
         PreparedStatementSetter：通过回调获取JdbcTemplate提供的PreparedStatement，由用户来对相应的预编译语句相应参数设值； 
         BatchPreparedStatementSetter：；类似于PreparedStatementSetter，但用于批处理，需要指定批处理大小； 
● 自定义功能回调：提供给用户一个扩展点，用户可以在指定类型的扩展点执行任何数量需要的操作； 
         ConnectionCallback：通过回调获取JdbcTemplate提供的Connection，用户可在该Connection执行任何数量的操作； 
         StatementCallback：通过回调获取JdbcTemplate提供的Statement，用户可以在该Statement执行任何数量的操作； 
         PreparedStatementCallback：通过回调获取JdbcTemplate提供的PreparedStatement，用户可以在该PreparedStatement执行任何数量的操作； 
         CallableStatementCallback：通过回调获取JdbcTemplate提供的CallableStatement，用户可以在该CallableStatement执行任何数量的操作； 
● 结果集处理回调：通过回调处理ResultSet或将ResultSet转换为需要的形式； 
         RowMapper：用于将结果集每行数据转换为需要的类型，用户需实现方法mapRow(ResultSet rs, int rowNum)来完成将每行数据转换为相应的类型。 
         RowCallbackHandler：用于处理ResultSet的每一行结果，用户需实现方法processRow(ResultSet rs)来完成处理，在该回调方法中无需执行rs.next()，该操作由JdbcTemplate来执行，用户只需按行获取数据然后处理即可。 
         ResultSetExtractor：用于结果集数据提取，用户需实现方法extractData(ResultSet rs)来处理结果集，用户必须处理整个结果集；
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- /**
-  * 
-  * @author Chou
-  * @since 2012-9-9
-  * SpringJdbc并没有像Hibernate那样把AutoCommit设置为false，
-  * 所以没有配置事务的情况下，默认是自动提交
-  */
- publicclass JdbcTemplateDao extends JdbcDaoSupport{  
- 
- publicvoid setDadasource(DataSource dataSource) {  
- super.setDataSource(dataSource);  
-     }  
- 
- publicvoid commonSql() {  
-         String insertSql = "insert into tab_item values(?,?,?)";  
-         String updateSql = "update tab_item set itemno = ?, itemname = ? where id = ?";  
-         String deleteSql = "delete tab_item where id = ?";  
- 
-         Assert.isTrue(  
-         getJdbcTemplate().update(insertSql, new Object[]{4, "HP", "PT540"}) == 1, "插入失败");  
-         Assert.isTrue(  
-         getJdbcTemplate().update(updateSql, new Object[]{"HP", "PT555", 4}) == 1, "修改失败");  
-         Assert.isTrue(  
-         getJdbcTemplate().update(deleteSql, new Object[]{4}) == 1, "删除失败");  
-     }  
- 
- /**
-      * 预编译语句设值回调使用
-      * 通过JdbcTemplate的int update(String sql, PreparedStatementSetter pss)执行预编译sql
-      */
- publicvoid preparedStatementSql() {  
-         String sql = "insert into tab_item values(?,?,?)";  
- int row = getJdbcTemplate().update(sql, new PreparedStatementSetter() {  
- @Override
- publicvoid setValues(PreparedStatement ps) throws SQLException {  
-                 ps.setInt(1, 101);// JDBC占位符集合的序号是从1开始的，而不是0
-                 ps.setObject(2, "Apple");  
-                 ps.setString(3, "Ipad4");  
-             }  
-         });  
-         Assert.isTrue(row == 1, "插入失败");  
- 
- // JdbcTemplate也提供一种更简单的方式设值相应的占位符位置的值
-         sql = "delete from tab_item where id = ?";  
-         row = getJdbcTemplate().update(sql, new Object[] { 101 });  
-         Assert.isTrue(row == 1, "删除失败");  
-     }  
- 
- publicvoid queryCallback() {  
-         String listSql = "select * from player";  
- 
- /**
-          * RowMapper接口提供mapRow(ResultSet rs, int rowNum)
-          * 方法将结果集的每一行转换为一个Map，当然可以转换为其他类，如表的对象化形式。
-          */
-         List<Map> result = getJdbcTemplate().query(listSql, new RowMapper<Map>() {  
- @Override
- public Map mapRow(ResultSet rs, int rowNum) throws SQLException {  
-                 System.out.println(rowNum);  
-                 Map<Long,String> map = new HashMap<Long, String>();    
-                 map.put(rs.getLong("id"), rs.getString("name"));  
- return map;  
-             }  
-         });  
-         Iterator it = result.iterator();  
- while (it.hasNext()) {  
-             Map<Long,String> map = (Map<Long, String>) it.next();  
-             System.out.println(map.values());  
-         }  
- 
- /**
-          * RowCallbackHandler接口也提供方法processRow(ResultSet rs)，
-          * 能将结果集的行转换为需要的形式。
-          */
-         getJdbcTemplate().query(listSql, new RowCallbackHandler() {  
- @Override
- publicvoid processRow(ResultSet rs) throws SQLException {  
-                 System.out.println("处理结果集");  
-                 System.out.print("id:" + rs.getInt("id") + "***");  
-                 System.out.println("name:" + rs.getString("name"));  
-             }  
-         });  
- 
- /**
-          * ResultSetExtractor使用回调方法extractData(ResultSet rs)提供整个结果集，
-          * 让用户决定如何处理该结果集
-          */
-         List list = getJdbcTemplate().query(listSql, new ResultSetExtractor<List>() {  
- @Override
- public List extractData(ResultSet rs) throws SQLException,DataAccessException {  
-                 List result = new ArrayList();    
- while(rs.next()) {  
-                     Map row = new HashMap();  
-                     row.put(rs.getInt("id"), rs.getString("name"));  
-                     result.add(row);  
-                     System.out.println(row);  
-                 }  
- return result;  
-             }  
-         });  
-     }  
- 
- /**
-      * JdbcTemplate提供更简单的queryForXXX方法，来简化开发：
-      */
- privatevoid queryForSimple() {  
- //1.查询一行数据并返回int型结果  
- int countRs = getJdbcTemplate().queryForInt("select count(*) from player");  
-         System.out.println(countRs);  
- 
- //2. 查询一行数据并将该行数据转换为Map返回  
-         Map map = getJdbcTemplate().queryForMap("select * from tab_item where id = ?", new Object[]{100});  
-         System.out.println(map.values());  
- 
- //3.查询一行任何类型的数据，最后一个参数指定返回结果类型  
-         countRs = getJdbcTemplate().queryForObject("select count(*) from player", Integer.class);  
-         System.out.println(countRs);  
- 
- //4.查询一批数据，默认将每行数据转换为Map       
-         List<Map<String, Object>> list = getJdbcTemplate().queryForList("select * from player");  
-         System.out.println(list.get(0).values());  
- 
- //5.只查询一列数据列表，列类型是String类型，列名字是name
-         List<String> names = getJdbcTemplate().queryForList("select itemname from tab_item where itemname=?", new Object[]{"雷蛇鼠标"}, String.class);  
-         System.out.println(names.size());  
- 
- //6.查询一批数据，返回为SqlRowSet，类似于ResultSet，但不再绑定到连接上  
-         SqlRowSet rs = getJdbcTemplate().queryForRowSet("select * from player");  
-     }  
- 
- /**
-      * 调用建表，删表语句
-      * 数据库不同，创建存储过程的SQL也不同
-      */
- publicvoid createProcedure() {  
-             String createTableSql = "create memory table test" +    
- "(id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY, " +    
- "name varchar(100))";    
- 
-             String dropTableSql = "drop table test";   
- 
-             String createHsqldbFunctionSql =    
- "CREATE FUNCTION FUNCTION_TEST(str CHAR(100)) " +    
- "returns INT begin atomic return length(str);end";    
-             getJdbcTemplate().update(createHsqldbFunctionSql);  
- 
-             String createProcedureSql =    
- "CREATE PROCEDURE PROCEDURE_TEST" +    
- "(INOUT inOutName VARCHAR(100), OUT outId INT) " +    
- "MODIFIES SQL DATA " +    
- "BEGIN ATOMIC " +    
- "  insert into test(name) values (inOutName); " +    
- "  SET outId = IDENTITY(); " +    
- "  SET inOutName = 'Hello,' + inOutName; " +    
- "END";  
- 
-             getJdbcTemplate().update(createTableSql);    
-             getJdbcTemplate().execute(createProcedureSql);  
- 
-             getJdbcTemplate().execute("DROP FUNCTION FUNCTION_TEST");    
-             getJdbcTemplate().execute("DROP PROCEDURE PROCEDURE_TEST");    
- 
-             getJdbcTemplate().execute(dropTableSql);   
-     }  
- /**
-      * @return DriverManagerDataSource 返回数据源对象dataSource
-      */
- publicstatic DriverManagerDataSource getMyDataSource() {  
-         DriverManagerDataSource dataSource = new DriverManagerDataSource(url,  
-                 username, password);  
-         dataSource.setDriverClassName(driver);  
- return dataSource;  
-     }  
- }  
**NamedParameterJdbcTemplate**
NamedParameterJdbcTemplate类是基于JdbcTemplate类，并对它进行了封装从而支持命名参数特性。NamedParameterJdbcTemplate主要提供以下三类方法： execute方法、 query及queryForXXX方法、update及batchUpdate方法。
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- /**
-  * @author Chou
-  * @since 2012-9-9
-  */
- publicclass NamedParameterJdbcTemplateDao extends NamedParameterJdbcDaoSupport{  
- 
- publicvoid setDadasource(DataSource dataSource) {  
- super.setDataSource(dataSource);  
-     }  
- 
- publicvoid commonSql() {  
- //可以使用DataSource或JdbcTemplate对象作为构造器参数初始化
-         namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(getJdbcTemplate());  
-         String insertSql = "insert into tab_item values(:id, :itemno, :itemname)";  
-         String deleteSql = "delete from tab_item where id = :id";  
-         String selectSql = "select * from player where place = :place";  
- 
- //NamedParameterJdbcTemplate类为命名参数设值有两种方式：java.util.Map和SqlParameterSource
-         Map<String, Object> paramMap = new HashMap<String, Object>();  
-         paramMap.put("id", 101);  
-         paramMap.put("itemno", "Menzu");  
-         paramMap.put("itemname", "MP4");  
- int row = namedParameterJdbcTemplate.update(insertSql, paramMap);  
-         Assert.isTrue(row == 1, "插入失败");  
- 
- /**
-          * 可以使用SqlParameterSource实现作为来实现为命名参数设值，默认有MapSqlParameterSource
-          * 和BeanPropertySqlParameterSource实现；MapSqlParameterSource实现非常简单，
-          * 只是封装了java.util.Map；而BeanPropertySqlParameterSource封装了一个JavaBean对象，
-          * 通过JavaBean对象属性来决定命名参数的值。
-          * 可以看出BeanPropertySqlParameterSource使用能减少很多工作量，
-          * 但命名参数必须和JavaBean属性名称相对应才可以。
-          */
- //Map<String, Object> paramMap2 = new HashMap<String, Object>();
- //paramMap2.put("id", 1);
- //SqlParameterSource paramSource = new MapSqlParameterSource(paramMap2);
- 
-         Item model = new Item();  
-         model.setId(101l);  
-         SqlParameterSource paramSource = new BeanPropertySqlParameterSource(model);  
- 
-         row = namedParameterJdbcTemplate.update(deleteSql, paramSource);  
-         Assert.isTrue(row == 1, "删除失败");  
- 
-         Map<String, Object> paramMap1 = new HashMap<String, Object>();  
-         paramMap1.put("place","PG   ");  
-         namedParameterJdbcTemplate.query(selectSql, paramMap1, new RowCallbackHandler() {  
- @Override
- publicvoid processRow(ResultSet rs) throws SQLException {  
-                 System.out.println(rs.getString("name"));  
-             }  
-         });  
-     }  
- }  
- 
- publicclass Item {  
- private Long id;  
- private String itemno;  
- private String itemname;  
- 
- //省略get和set方法
- }  
**SimpleJdbcTemplate**
SimpleJdbcTemplate类也是基于JdbcTemplate类，但利用Java5+的可变参数列表和自动装箱和拆箱从而获取更简洁的代码。但是SimpleJdbcDaoSupport在Spring3.X中已经不推荐使用了。
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- /**
-  * @author Chou
-  * @since 2012-9-9 
-  */
- publicclass SimpleJdbcTemplateDao extends SimpleJdbcDaoSupport{  
- 
- publicvoid setDadasource(DataSource dataSource) {  
- super.setDataSource(dataSource);  
-     }  
- 
- publicvoid commonSql() {  
-         String insertSql = "update tab_item set itemno = :itemno where itemno = :itemno2";  
- int rows = getSimpleJdbcTemplate().update(insertSql, "APPLE", "Apple");  
-         System.out.println(rows);  
-     }  
- }  
**SimpleJdbc**
Spring JDBC抽象框架提供SimpleJdbcInsert和SimpleJdbcCall类，这两个类通过利用JDBC驱动提供的数据库元数据来简化JDBC操作。下面代码演示SimpleJdbcInsert用法，SimpleJdbcCall用于调用存储过程及自定义函数，可以参考博客：[http://jinnianshilongnian.iteye.com/blog/1428149](http://jinnianshilongnian.iteye.com/blog/1428149)
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- /**
-  * @author Chou
-  * @since 2012-9-9 
-  */
- @SuppressWarnings("deprecation")  
- publicclass SimpleJdbcTemplateDao{  
- 
- privatestatic SimpleJdbcInsert insert;  
- 
- publicstaticvoid main(String[] args) {  
-         insert = new SimpleJdbcInsert(JdbcTemplateTest.getMyDataSource());  
-         simpleInsetId()  
-         System.out.println(insert.getInsertString());  
-     }  
- 
- publicstaticvoid simpleInsetId() {  
-         insert.withTableName("tab_item");  
- //主键是数据库自动生成的情况下
-         insert.setGeneratedKeyName("id");  
- 
-         Map<String, Object> args = new HashMap<String, Object>();  
-         args.put("itemno", "SONY");  
-         args.put("itemname", "SONY");  
- 
- /**
-          * executeAndReturnKey用于执行并获取自动生成主键（注意是Number类型），
-          * 必须首先通过setGeneratedKeyName设置主键然后才能获取，如果想获取复
-          * 合主键请使用setGeneratedKeyNames描述主键然后通过executeReturningKeyHolder
-          * 获取复合主键KeyHolder对象；
-          */
-         Number id = insert.executeAndReturnKey(args);  
-         System.out.println(id);  
-     }  
- }  
**SpringJdbc批量操作**
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- publicclass BatchExecuteDao extends JdbcDaoSupport{  
- 
- publicvoid setDadasource(DataSource dataSource) {  
- super.setDataSource(dataSource);  
-     }  
- 
- @SuppressWarnings("unchecked")  
- publicvoid batchBySimpleJdbcInsert() {  
-          SimpleJdbcInsert insert = new SimpleJdbcInsert(getJdbcTemplate());    
-          insert.withTableName("tab_item");    
-          Map<String, Object> valueMap = new HashMap<String, Object>();    
-          valueMap.put("id", 101);  
-          valueMap.put("itemno", "SONY");  
-          valueMap.put("itemname", "SONY");  
-          Map<String, Object> valueMap2 = new HashMap<String, Object>();    
-          valueMap2.put("id", 102);  
-          valueMap2.put("itemno", "SONY");  
-          valueMap2.put("itemname", "SONY");  
-          insert.executeBatch(new Map[] {valueMap, valueMap2});  
-          System.out.println(insert.getInsertString());  
-     }  
- 
- /**
-      * 使用batchUpdate(String sql, List<Object[]> batchArgs)方法完成占位符批处理，
-      * 当然也支持命名参数批处理等
-      */
- publicvoid batchBySimpleJdbcTemplate() {  
-         SimpleJdbcTemplate simpleJdbcTemplate = new SimpleJdbcTemplate(getJdbcTemplate());    
-         String deleteSql = "delete tab_item where id = :id";    
-         List<Object[]> params = new ArrayList<Object[]>();    
-         params.add(new Object[]{101});  
-         params.add(new Object[]{102});  
-         params.add(new Object[]{103});  
-         params.add(new Object[]{104});  
-         simpleJdbcTemplate.batchUpdate(deleteSql, params);  
- 
-     }  
- 
- publicvoid batchByNamedParameterJdbcTemplate() {  
-         NamedParameterJdbcTemplate namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(getJdbcTemplate());  
-         String updateSql = "update tab_item set itemname = :itemname where id = :id";  
-         Item model1 = new Item();  
-         Item model2 = new Item();  
-         Item model3 = new Item();  
-         Item model4 = new Item();  
-         model1.setId(101l);  
-         model2.setId(102l);  
-         model3.setId(103l);  
-         model4.setId(104l);  
- 
-         model1.setItemname("I9100");  
-         model2.setItemname("I9101");  
-         model3.setItemname("I9102");  
-         model4.setItemname("I9103");  
- 
-         SqlParameterSource[] params = SqlParameterSourceUtils.createBatch(new Object[] {model1, model2, model3, model4});    
-         namedParameterJdbcTemplate.batchUpdate(updateSql, params);  
- 
-     }  
- 
- /**
-      * 通过batchUpdate(String sql, final BatchPreparedStatementSetter pss)方法进行批处理，
-      * 该方式使用预编译语句，然后通过BatchPreparedStatementSetter实现进行设值（setValues）
-      * 及指定批处理大小（getBatchSize）。
-      */
- publicvoid batchByJdbcTemplate() {  
-         String insertSql = "insert into tab_item values(?,?,?)";  
-         Object[] arg1 = {101, "SUMSUNG", "GAS1"};  
-         Object[] arg2 = {102, "SUMSUNG", "GAS2"};  
-         Object[] arg3 = {103, "SUMSUNG", "GAS3"};  
-         Object[] arg4 = {104, "SUMSUNG", "GAS4"};  
- final List<Object[]> list = new ArrayList<Object[]>();  
-         list.add(arg1);  
-         list.add(arg2);  
-         list.add(arg3);  
-         list.add(arg4);  
- 
- int[] rows = getJdbcTemplate().batchUpdate(insertSql, new BatchPreparedStatementSetter() {  
- 
- @Override
- publicvoid setValues(PreparedStatement ps, int i) throws SQLException {  
-                 Object[] obj = list.get(i);  
-                 ps.setInt(1, (Integer) obj[0]);  
-                 ps.setString(2, (String) obj[1]);  
-                 ps.setString(3, (String)obj[2]);  
-             }  
- 
- @Override
- publicint getBatchSize() {//返回批量操作数最大值
- return list.size();  
-             }  
-         });  
-         System.out.println(getJdbcTemplate().getFetchSize());  
-         System.out.println(getJdbcTemplate().getQueryTimeout());  
- for (int i = 0; i < rows.length; i++) {  
-             System.out.println(rows[i]);  
-         }  
- 
-         Assert.isTrue(list.size() == getJdbcTemplate().queryForInt("select count(1) from tab_item where itemno = 'SUMSUNG'"), "批量更新失败");  
-     }  
- }  
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- ...  
- <beanclass="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
- <propertyname="locations">
- <list>
- <value>classpath:application.properties</value>
- </list>
- </property>
- </bean>
- 
- <beanid="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close">
- <!-- Connection Info -->
- <propertyname="driverClassName"value="${db.driver.class}"/>
- <propertyname="url"value="${db.url}"/>
- <propertyname="username"value="${db.username}"/>
- <propertyname="password"value="${db.password}"/>
- 
- <!-- Connection Pooling Info -->
- <propertyname="initialSize"value="${proxool.initialSize}"/>
- <propertyname="maxActive"value="${proxool.maxActive}"/>
- <propertyname="maxIdle"value="${proxool.maxIdle}"/>
- <propertyname="minIdle"value="${proxool.minIdle}"/>
- <propertyname="maxWait"value="500"/>
- <propertyname="poolPreparedStatements"value="false"/>
- </bean>
- 
- <beanid="abstractDao"abstract="true">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- 
- <beanid="dao1"class="com.chou.jdbc.JdbcTemplateDao"parent="abstractDao"/>
- <beanid="dao2"class="com.chou.jdbc.NamedParameterJdbcTemplateDao"parent="abstractDao"/>
- <beanid="dao3"class="com.chou.jdbc.SimpleJdbcTemplateDao"parent="abstractDao"/>
- <beanid="dao4"class="com.chou.jdbc.BatchExecuteDao"parent="abstractDao"/>
- ...  
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- db.driver.class=oracle.jdbc.driver.OracleDriver  
- db.url=jdbc:oracle:thin:@127.0.0.1:1521:orcl  
- db.username=db_test  
- db.password=db_test  
- 
- #连接池连接数初始值  
- proxool.initialSize=2
- 
- #连接池最大连接数量  
- proxool.maxActive=10
- 
- #连接池最大空闲值，释放多余的不用的连接，释放到多少为止  
- proxool.maxIdle=5
- 
- #连接池最小空闲值，当空闲连接数小于这个数的时候，连接池就会去申请一些连接以免高峰期来不及申请  
- proxool.minIdle=1
**Spring JDBC自身提供了以下DataSource实现：**
- DriverManagerDataSource ：简单封装了DriverManager获取数据库连接；通过DriverManager的getConnection方法获取数据库连接；
- SingleConnectionDataSource ：内部封装了一个连接，该连接使用后不会关闭，且不能在多线程环境中使用，一般用于测试；
- LazyConnectionDataSourceProxy ：包装一个DataSource，用于延迟获取数据库连接，只有在真正创建Statement等时才获取连接，因此再说实际项目中最后使用该代理包装原始DataSource从而使得只有在真正需要连接时才去获取。
**Spring也能通过第三方提供的连接池实现主要有C3P0、Proxool、DBCP**
DBCP没有自动的去回收空闲连接的功能 
C3P0有自动回收空闲连接功能,它实现了数据源和JNDI绑定，支持JDBC3规范和JDBC2的标准扩展。 
**DataSourceUtils**： 
Spring JDBC抽象框架内部都是通过它的getConnection(DataSource dataSource)方法获取数据库连接，releaseConnection(Connection con, DataSource dataSource) 用于释放数据库连接，DataSourceUtils用于支持Spring管理事务，只有使用DataSourceUtils获取的连接才具有Spring管理事务。 
**SpringJdbc在插入数据时也可以获取数据库自动生成的主键**，具体用法可以参考博客：[http://jinnianshilongnian.iteye.com/blog/1428149](http://jinnianshilongnian.iteye.com/blog/1428149)

