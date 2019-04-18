# spring批量更新数据 ---- BatchPreparedStatementSetter - z69183787的专栏 - CSDN博客
2012年12月28日 10:29:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4231
  spring给我们提供了jdbc的统一封装，和其它的orm不同，它是对jdbc api的封装，相当于我们自己写的java bean，而没有对对象模型进行封装（比如将po封装为pojo）。它保持了jdbc数据对象的原始性，具有极大程度的灵活性，其性能也极高。
  我们知道在进行增量操作的时候最好是使用PreparedStatement,这样会节省数据库服务器编译sql语句的成本。而Spring也提供了对增量操作的支持，而BatchPreparedStatementSetter接口则是其核心。
  先看看BatchPreparedStatementSetter接口里都定义了些操作。
  public interface BatchPreparedStatementSetter{
public int getBatchSize(）；
public void setValues(PreparedStatement ps,int i)；
}
  该接口是被JdbcTemplate类使用的CallBack接口。批操作中的每个操作都使用相同的sql语句，而该接口则sql语句中的占位符设置具体的值。
  public int getBatchSize(）；用来返回批次的大小
public void setValues(PreparedStatement ps,int i)；
  用来为PreparedStatement设值。
  参数说明：
  ps：我们将要设值的PreparedStatement
   i：在这个批次中，正在执行操作的索引，从0算起。
 以批量更新为例，我们要做的是先实现BatchPreparedStatementSetter接口，然后再调用JdbcTemplate的batchUpdate（sql，setter）操作，参数sql是预编译语句 ,setter是BatchPreparedStatementSetter的一个实例。下面是我写的一段代码：
**[java]**[view
 plain](http://blog.csdn.net/zhouzheng034/article/details/7843707#)[copy](http://blog.csdn.net/zhouzheng034/article/details/7843707#)
- publicvoid batchInsertRows(String sql,final List<Object[]> dataSet) throws Exception{  
- 
-      BatchPreparedStatementSetter setter=new BatchPreparedStatementSetter(){  
- 
- publicint getBatchSize(){  
- return dataSet.size();  
-       }  
- publicvoid setValues(PreparedStatement ps,int i){  
- 
-        Object[] obj = dataSet.get(i);  
- int nextId = getNextId();  
- try{  
-        ps.setLong(1,nextId);  
-        ps.setLong(2,Integer.parseInt(obj[0].toString()));  
-        ps.setLong(3,Integer.parseInt(obj[1].toString()));  
-        ps.setString(4,(String)obj[2]);}  
- catch(Exception e){  
-         e.printStackTrace();  
-        }  
-       }  
- 
- 
-      };  
-    jdbcTemplate.batchUpdate(sql,setter);  
-     }  
