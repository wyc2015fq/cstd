# Spring JDBC 常用批量操作及插入操作 - z69183787的专栏 - CSDN博客
2016年12月16日 13:18:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5214
Spring 常用批量操作有很多种方法，但是大多数人知道一种或者其中的几种，先总结一些spring JDBC常用的批量操作的方法及其效率；
- 
### 使用JDBCTemplate 进行基本的批量操作
这种方法是网上大多数采用的方法， 但是在实际应用中我感觉不太方便，这个方法能不能做成一个通用的接口呢？一直没有仔细研究过
- 
```java
public class JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
	}
	public int[] batchUpdate(final List<Actor> actors) {
		int[] updateCounts = jdbcTemplate.batchUpdate("update t_actor set first_name = ?, " +
				"last_name = ? where id = ?",
			new BatchPreparedStatementSetter() {
				public void setValues(PreparedStatement ps, int i) throws SQLException {
						ps.setString(1, actors.get(i).getFirstName());
						ps.setString(2, actors.get(i).getLastName());
						ps.setLong(3, actors.get(i).getId().longValue());
					}
					public int getBatchSize() {
						return actors.size();
					}
				});
		return updateCounts;
	}
	// ... additional methods
}
```
二、 使用List集合形式的参数的批量操作
    如果使用List集合来进行批量操作，这种方法是比较合适的，spring jdbc core 包中提供了一个SqlParamterSource 对象，然后使用
`SqlParameterSourceUtils.createBatch`
这个方法，把javabean的list  转化成array，spring会循环的进行取值；
```java
public class JdbcActorDao implements ActorDao {
	private NamedParameterTemplate namedParameterJdbcTemplate;
	public void setDataSource(DataSource dataSource) {
		this.namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(dataSource);
	}
	public int[] batchUpdate(final List<Actor> actors) {
		SqlParameterSource[] batch = SqlParameterSourceUtils.createBatch(actors.toArray());
		int[] updateCounts = namedParameterJdbcTemplate.batchUpdate(
				"update t_actor set first_name = :firstName, last_name = :lastName where id = :id",
				batch);
		return updateCounts;
	}
	// ... additional methods
}
```
当然，你还可以使用类似的方法来进行批量操作，如下代码：（代码来自官方网站示例）
public classJdbcActorDaoimplementsActorDao{
	private JdbcTemplate jdbcTemplate;
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
	}
	public int[] batchUpdate(finalList<Actor> actors) {
		List<Object[]> batch = new ArrayList<Object[]>();
		for (Actor actor : actors) {
			Object[] values = newObject[] {
					actor.getFirstName(),
					actor.getLastName(),
					actor.getId()};
			batch.add(values);
		}
		int[] updateCounts = jdbcTemplate.batchUpdate(
				"update t_actor set first_name = ?, last_name = ? where id = ?",
				batch);
		return updateCounts;
	}
	// ... additional methods
}
三、使用多维数组进行批量操作
```java
public class JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
	}
	public int[][] batchUpdate(final Collection<Actor> actors) {
		int[][] updateCounts = jdbcTemplate.batchUpdate(
				"update t_actor set first_name = ?, last_name = ? where id = ?",
				actors,
				100,
				new ParameterizedPreparedStatementSetter<Actor>() {
					public void setValues(PreparedStatement ps, Actor argument) throws SQLException {
						ps.setString(1, argument.getFirstName());
						ps.setString(2, argument.getLastName());
						ps.setLong(3, argument.getId().longValue());
					}
				});
		return updateCounts;
	}
	// ... additional methods
}
```
上面的代码中，100表示一次批量操作的容量；
四、使用SimpleJdbcInsert 来进行简单的插入操作
    一般的，我们会使用update来进行插入操作，但是spring提供了更加简洁面向对象插入方法：
```java
public class JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	private SimpleJdbcInsert insertActor;
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
		this.insertActor = new SimpleJdbcInsert(dataSource).withTableName("t_actor");
	}
	public void add(Actor actor) {
		Map<String, Object> parameters = new HashMap<String, Object>(3);
		parameters.put("id", actor.getId());
		parameters.put("first_name", actor.getFirstName());
		parameters.put("last_name", actor.getLastName());
		insertActor.execute(parameters);
	}
	// ... additional methods
}
```
如果我们需要得到插入的返回值呢？
publicclass JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	private SimpleJdbcInsert insertActor;
	publicvoid setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
		this.insertActor = new SimpleJdbcInsert(dataSource)
				.withTableName("t_actor")
				.usingGeneratedKeyColumns("id");
	}
	publicvoid add(Actor actor) {
		Map<String, Object> parameters = new HashMap<String, Object>(2);
		parameters.put("first_name", actor.getFirstName());
		parameters.put("last_name", actor.getLastName());
		Number newId = insertActor.executeAndReturnKey(parameters);
		actor.setId(newId.longValue());
	}
	// ... additional methods
}
注意了，此处使用了一个map作为参数，字段名作为键，所以我们可以根据传入的对象来自动去获取参数名及参数值的map，提示：可以利用反射原来处理：
示例代码：
/**
	 * 获取名称值的map
	 * @param class1
	 * @return
	 */public static <M> SqlParameterSource getParamsMap(M bean) {
		if(bean==null)returnnull;
		MapSqlParameterSource parameters = new MapSqlParameterSource();
		Class<?> _class = bean.getClass();
		Table _table = _class.getAnnotation(Table.class);
		if(_table==null){
			logger.error(_class.getName()+" not find @Table value!please check it!");
			throw(new NotFoundTableAnnotaionException(_class.getName()+" not find @Table value!please check it!"));
		}
		String primaryKey=_table.primaryKey();
		Field[] allFields=getAllFields(_class);
		Object _primaryKeyValue="";
		for (Field field : allFields) {
			if(Modifier.isStatic(field.getModifiers()))continue;
			field.setAccessible(true);
			try {
				if(StringUtil.isEqual(primaryKey, field.getName())){
					_primaryKeyValue=field.get(bean);
					continue;
				}
				parameters.addValue(field.getName(), field.get(bean));
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return parameters;
	}
提出一个问题：如果我的字段名及数据库的字段名不一样咋办？
解决办法：
publicclass JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	private SimpleJdbcInsert insertActor;
	publicvoid setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
		this.insertActor = new SimpleJdbcInsert(dataSource)
				.withTableName("t_actor")
				.usingColumns("first_name", "last_name")
				.usingGeneratedKeyColumns("id");
	}
	publicvoid add(Actor actor) {
		Map<String, Object> parameters = new HashMap<String, Object>(2);
		parameters.put("first_name", actor.getFirstName());
		parameters.put("last_name", actor.getLastName());
		Number newId = insertActor.executeAndReturnKey(parameters);
		actor.setId(newId.longValue());
	}
	// ... additional methods
}usingColumns("first_name", "last_name") //主要是这句话~
当然，也可以使用我们之前提到过的， 
```
SqlParameterSource
 parameters =
```
`new``MapSqlParameterSource()；而且可以进行链式操作。`
```java
public class JdbcActorDao implements ActorDao {
	private JdbcTemplate jdbcTemplate;
	private SimpleJdbcInsert insertActor;
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
		this.insertActor = new SimpleJdbcInsert(dataSource)
				.withTableName("t_actor")
				.usingGeneratedKeyColumns("id");
	}
	public void add(Actor actor) {
		SqlParameterSource parameters = new MapSqlParameterSource()
				.addValue("first_name", actor.getFirstName())
				.addValue("last_name", actor.getLastName());
		Number newId = insertActor.executeAndReturnKey(parameters);
		actor.setId(newId.longValue());
	}
	// ... additional methods
}
```
[参考文档](http://docs.spring.io/spring/docs/4.0.6.RELEASE/spring-framework-reference/htmlsingle/#jdbc-simple-jdbc-insert-1)
注：使用spring4.0.3
