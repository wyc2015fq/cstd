# 视频：JDBCRDD源码及自定义JDBCRDD的分区策略 - Spark高级玩法 - CSDN博客
2018年06月29日 07:07:11[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：302
jdbcRDD虽然是鸡肋，但是也值得一讲。帮助大家更进一步理解RDD。
1,JDBCRDD使用
val data = new JdbcRDD(sc, getConnection
, "SELECT id,aa FROM bbb where ? <= ID AND ID <= ?", lowerBound = 3, upperBound =5, numPartitions = 1, mapRow = extractValues)
参数解释:
1，sparkcontext。
2，一个创建链接的函数。
3，sql。必须有? <= ID AND ID <= ?。
4，要取数据的id最小行。
5，要取数据的id最大行号。
6，分区数。
7，一个将ResultSet转化为需要类型的方法。
2,JdbcRDD的getPartition方法
override def getPartitions: Array[Partition] = {
// bounds are inclusive, hence the + 1 here and - 1 on end
val length = BigInt(1) + upperBound - lowerBound
  (0 until numPartitions).map(i => {
val start = lowerBound + ((i * length) / numPartitions)
val end = lowerBound + (((i + 1) * length) / numPartitions) - 1
new JdbcPartition(i, start.toLong, end.toLong)
  }).toArray
}
3,JdbcRDD的compute方法
就是一个通过jdbc获取指定范围数据的过程。
valpart = thePart.asInstanceOf[JdbcPartition]
val conn = getConnection()
val stmt = conn.prepareStatement(sql, ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY)
stmt.setLong(1, part.lower)
stmt.setLong(2, part.upper)
val rs = stmt.executeQuery()
4,重写JDBC方法
重写分区的方法即可。
如:
CustomizedJdbcRDD[T: ClassTag](
                                      sc: SparkContext,
getConnection: () => Connection,
sql: String,
getCustomizedPartitions: () => Array[Partition],
prepareStatement: (PreparedStatement, CustomizedJdbcPartition) => PreparedStatement,
mapRow: (ResultSet) => T = CustomizedJdbcRDD.resultSetToObjectArray _)
同时把getPartition方法重写为:
override def getPartitions: Array[Partition] = {
  getCustomizedPartitions();
}
更多源码视频，请加入知识星球获取：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
