# Spring对jdbc操作的支持 - LC900730的博客 - CSDN博客
2017年08月08日 22:11:51[lc900730](https://me.csdn.net/LC900730)阅读数：170
### Spring与jdbc
Spring对jdbc技术提供了很好的支持，体现在：
```
1.Spring对C3P0的支持
2.Spring对jdbc提供了jdbcTemplate来简化jdbc操作
3.JdbcTemplate模板工具类，类似于Dbutils组件。
```
### 对C3p0的支持
```java
public class UserDao {
    /**
     *保存方法
     *连接管理
     *jdbc操作
     */
        //IOC注入
        private DataSource dataSource;
        public DataSource getDataSource() {
            return dataSource;
        }
        public void setDataSource(DataSource dataSource) {
            this.dataSource = dataSource;
        }
        public void save(){
            String sql="insert into t_dept(deptName) values('test');";
            Connection con=null;
            Statement stmt=null;
            con=datasource.getConnection();
            stmt=con.createstatement();
            stmt.execute(sql);
```
```
<!--数据源对象：c3p0连接池 -->
      <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <property name="driverClass" value="com.mysql.cj.jdbc.Driver"></property>
        <property name="jdbcUrl" value="jdbc:mysql://localhost:3306/test"></property>
        <property name="user" value="root"></property>
            <property name="password" value="qqaazz"></property>
        <property name="initialPoolSize" value="3"></property>
            <property name="maxPoolSize" value="10"></property>
        <property name="maxStatements" value="100"></property>
            <property name="acquireIncrement" value="2"></property>
      </bean>
<bean id="userDao" class="cn.itcast.h_jdbc.UserDao">
            <property name="dataSource" ref="dataSource"></property>
         </bean>
```
### jdbcTemplate
```
import org.springframework.jdbc.core.JdbcTemplate;
    public class UserDao {
        //IOC注入
        private DataSource dataSource;
        public DataSource getDataSource() {
            return dataSource;
        }
        public void setDataSource(DataSource dataSource) {
            this.dataSource = dataSource;
        }
        public void save(){
            String sql="insert into t_dept(deptName) values('test');";
            Connection con=null;
            Statement stmt=null;
            //使用jdbc模板工具类简化jdbc操作
            JdbcTemplate jdbcTemplate=new JdbcTemplate(dataSource);
            jdbcTemplate.update(sql);
            //插入修改删除都可以用这个方法
```
jdbcTemplate
##### queryForMap()
```
public Dept findById(int id){
            String sql="select * from t_dept where deptId=?";
            JdbcTemplate jdbcTemplate=new JdbcTemplate(dataSource);
            //只能是一行
            Map<String,Object> map=jdbcTemplate.queryForMap(sql,id);
            System.out.println(map);
            return null;
        }
```
##### queryForList()
每一行封装成map，在装入list   
```
public Dept findById(int id){
            String sql="select * from t_dept";
            JdbcTemplate jdbcTemplate=new JdbcTemplate(dataSource);
            //只能是一行
            Map<String,Object> map=jdbcTemplate.queryForMap(sql);
            System.out.println(map);
            return null;
        }
```
##### query()
查询全部，每一行封装成map，在装入list  
```java
public Dept findById(int id){
            String sql="select * from t_dept";
            JdbcTemplate jdbcTemplate=new JdbcTemplate(dataSource);
            //如何封装一行记录
            List<Dept> list=
            jdbcTemplate.query(sql,new RowMapper<Dept>(){
@Override
public Dept mapRow(ResultSet rs,int index) throws SQLException{
    Dept dept=new Dept();
    dept.setDeptId(rs.getInt("deptId"));
    dept.setDeptName(rs.getString("deptName"));
    return list;
}
});
            System.out.println(map);
            return null;
        }
```
##### query()
查询一行
```
public Dept findById(int id){
            String sql="select * from t_dept";
            JdbcTemplate jdbcTemplate=new JdbcTemplate(dataSource);
            //如何封装一行记录
            List<Dept> list=
            jdbcTemplate.query(sql,new RowMapper<Dept>(){
@Override
public Dept mapRow(ResultSet rs,int index) throws SQLException{
    Dept dept=new Dept();
    dept.setDeptId(rs.getInt("deptId"));
    dept.setDeptName(rs.getString("deptName"));
    return list;
}
},id);
            System.out.println(map);
            return null;
        }
//可以封装内部类
class MyResult implements RowMapper<Dept>{
        public Dept mapRow(ResultSet rs,int index) throws SQLException{
            Dept dept=new Dept();
            dept.setDeptId(rs.getInt("deptId"));
            dept.setDeptName(rs.getString("deptName"));
            return dept;
        }
    }
那么在上述中，可以jdbcTemplate.query(sql,new MyResult())
```
