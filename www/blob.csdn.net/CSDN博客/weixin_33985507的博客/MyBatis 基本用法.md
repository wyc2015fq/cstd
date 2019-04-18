# MyBatis 基本用法 - weixin_33985507的博客 - CSDN博客
2017年02月18日 07:34:15[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 定义mapping类
MyBatis 有两种定义查询结果到 Java 类的映射关系的方式，一种是通过xml文件定义，一种是通过Java annonation 定义，这里使用第二种方法。
现在我们有一张mysql的表定义如下：
```
CREATE TABLE `MY_BATIS_TEST` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `url` varchar(255) DEFAULT NULL
)
```
首先定义table一条数据在Java中对应的class
```
public class Redord {
    public String url;
}
```
定义sql查询到Java class 结果集合的映射：
```
public interface SimpleMapper {
    @Select("select url from testdb.MY_BATIS_TEST limit 1;")
    Redord selectOneRecord();
    @Select("select url from testdb.MY_BATIS_TEST;")
    Set<Record> selectRecords();
    @Select("select url from testdb.MY_BATIS_TEST where id=#{id};")
    Record selectRecordByID(int id);
}
```
# 初始化并注册mapping类
```
Properties properties = new Properties();
properties.setProperty("driver", "com.mysql.jdbc.Driver");
properties.setProperty("url", "jdbc:mysql://127.0.0.1:3306/testdb");
properties.setProperty("username", "the_user_name");
properties.setProperty("password", "the_password");
PooledDataSourceFactory pooledDataSourceFactory = new PooledDataSourceFactory();
pooledDataSourceFactory.setProperties(properties);
DataSource dataSource = pooledDataSourceFactory.getDataSource();
Environment environment = new Environment("development", new JdbcTransactionFactory(), dataSource);
Configuration configuration = new Configuration(environment);
configuration.addMapper(SimpleMapper.class); //注册mapping类
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(configuration);
```
# 从mysql中查询数据
```
SqlSession session = sqlSessionFactory.openSession();
try{
    PluginMapper mapper = session.getMapper(PluginMapper.class);
    Plugin pl = mapper.selectPluginsByID(1000);
    System.out.println(pl.url);
} finally {
    session.close();
}
```
# 全局唯一以及线程安全
SqlSessionFactory 可以在整个app的生命周期中只创建一次，SqlSession需要在每次执行sql的函数中创建一次并且使用后需要进行关闭：
```
SqlSession session = sqlSessionFactory.openSession();
try {
  // do work
} finally {
  session.close();
}
```
