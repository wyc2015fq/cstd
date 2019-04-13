
# spark SQL读取MySQL中的dept和hive中的emp表，做join和分组查询，然后写到json文件 - leofionn的博客 - CSDN博客


2018年05月26日 21:28:43[leofionn](https://me.csdn.net/qq_36142114)阅读数：100


val jdbcDF = spark.read.format("jdbc").option("url", "jdbc:mysql://hadoop000:3306").option("dbtable", "hive.dept").option("user", "root").option("password", "123456").load()
jdbcDF.createOrReplaceTempView("dept")
val hiveDF = sql("SELECT * FROM emp")
val sqlDF = sql("SELECT * FROM emp e JOIN dept d ON e.deptno = d.deptno").show
sqlDF.write.format("json").save("file:///empJoinDept.json")



【来自@若泽大数据】


