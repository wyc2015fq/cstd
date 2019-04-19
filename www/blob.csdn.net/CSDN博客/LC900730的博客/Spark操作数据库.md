# Spark操作数据库 - LC900730的博客 - CSDN博客
2017年12月12日 21:39:32[lc900730](https://me.csdn.net/LC900730)阅读数：137
```
val conf=new SparkConf().setAppName("JdbcRDDDemo").setMaster("local[2]")
val sc=new SparkContext(conf)
def getConnection()={
    Class.forName('com.mysql.jdbc.Driver').newInstance()
    DriverManager.getConnection("jdbc:mysql://localhost:3306/bigdata","root","123");
}
val jdbcRDD=new JdbcRDD(sc,getConnection,"SELECT * FROM ta where id >= ? AND id <=?",1,4,2,rs=>{...})
```
