# textFile构建RDD的分区及compute计算策略 - Spark高级玩法 - CSDN博客
2017年12月30日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：682
1,textFile
A),第一点，就是输入格式，key，value类型及并行度的意义。
deftextFile(
    path: String,
minPartitions: Int = defaultMinPartitions): RDD[String] = withScope {
  assertNotStopped()
//输入文件的格式TextInputFormat，key的类型LongWritable ，value的类型Text
  //最小分区数defaultMinPartitions
hadoopFile(path, classOf[TextInputFormat], classOf[LongWritable], classOf[Text],
minPartitions).map(pair => pair._2.toString).setName(path)
}
并行度
conf.getInt("spark.default.parallelism", math.max(totalCoreCount.get(), 2))
真正意义是啥？实际是决定我们goalSize的值。并不决定我们的分区数。
B),hadoopRDD的getPartition方法。
主要是获取分片的过程通过调用FileInputFormat.getSplits方法来实现分片。主要有一下几个步骤:
1) ,获取所有 FileStatus
FileStatus[] files = listStatus(job);
ListStatus方法里面：
1，判断是否需要递归
boolean recursive = job.getBoolean(INPUT_DIR_RECURSIVE, false);
2，接着是创建路径过滤器，筛选掉一些我们不需要的文件,入以_，.开头的
List<PathFilter> filters = new ArrayList<PathFilter>();
filters.add(hiddenFileFilter);
PathFilter jobFilter = getInputPathFilter(job);
if (jobFilter != null) {
  filters.add(jobFilter);
}
PathFilter inputFilter = new MultiPathFilter(filters);
3，根据mapreduce.input.fileinputformat.list-status.num-threads决定是并发还是单线程
FileStatus[] result;
int numThreads = job
    .getInt(
        org.apache.hadoop.mapreduce.lib.input.FileInputFormat.LIST_STATUS_NUM_THREADS,
org.apache.hadoop.mapreduce.lib.input.FileInputFormat.DEFAULT_LIST_STATUS_NUM_THREADS);
Stopwatch sw = new Stopwatch().start();
if (numThreads == 1) {
  List<FileStatus> locatedFiles = singleThreadedListStatus(job, dirs, inputFilter, recursive); 
result = locatedFiles.toArray(new FileStatus[locatedFiles.size()]);
} else {
  Iterable<FileStatus> locatedFiles = null;
  try {
    LocatedFileStatusFetcher locatedFileStatusFetcher = new LocatedFileStatusFetcher(
        job, dirs, recursive, inputFilter, false);
locatedFiles = locatedFileStatusFetcher.getFileStatuses();
} catch (InterruptedException e) {
throw new IOException("Interrupted while getting file statuses");
}
  result = Iterables.toArray(locatedFiles, FileStatus.class);
}
2) ,获取目标分片goalsize和最小minsize
long goalSize = totalSize / (numSplits == 0 ? 1 : numSplits);
long minSize = Math.max(job.getLong(org.apache.hadoop.mapreduce.lib.input.
  FileInputFormat.SPLIT_MINSIZE, 1), minSplitSize);
3) ,判断文件是否支持切分，不压缩或者压缩方式为BZip2Codec支持切分
protected boolean isSplitable(FileSystem fs, Path file) {
final CompressionCodec codec = compressionCodecs.getCodec(file);
  if (null == codec) {
return true;
}
return codec instanceof SplittableCompressionCodec;
}
支持切分就进行切分分片，切分分片大小为
Math.max(minSize, Math.min(maxSize, blockSize));
不支持切分的话就直接返回一个文件一个分片
最终，用InputSplit构建HadoopPartition
C),接着进入compute方法
重点掌握根据指定分片获取reader
reader = inputFormat.getRecordReader(split.inputSplit.value, jobConf, Reporter.NULL)
实际上是在TextInputFormat构建了
new LineRecordReader(job, (FileSplit) genericSplit,
recordDelimiterBytes);
还有就是识别不同系统的过程，比如hdfs ,本地file，tachyon。
final FileSystem fs = file.getFileSystem(job);
里面会根据uri获取scheme，然后构建为"fs." + scheme + ".impl" 通过反射的到相应的对象。
clazz = (Class<? extends FileSystem>) conf.getClass("fs." + scheme + ".impl", null);
类加载器为Configuration对象里面初始化的
private ClassLoader classLoader;
{
classLoader = Thread.currentThread().getContextClassLoader();
  if (classLoader == null) {
classLoader = Configuration.class.getClassLoader();
}
}
而此，configuration对象是在compute方法中通过jobConf = getJobConf()获得的实际是
从Driver端发送过来的。
val conf: Configuration = broadcastedConf.value.value
由此可以得到结论是tachyon使用是依赖，必须方法系统类加载器的Classpath中去
**推荐阅读：**
1，[spark源码系列之内部通讯的三种机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483733&idx=1&sn=92e7bcf399a8aaa24f7b6c37b98b3b2e&chksm=9f38e27da84f6b6bcd9e26db1f11b83fd4dca9d4e3512a25fd3b4eae012065c939d6ed6ecaad&scene=21#wechat_redirect)
2，[Spark源码系列之spark2.2的StructuredStreaming使用及源码介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483744&idx=1&sn=0b9908624245df2eaa24a88d2a04be58&chksm=9f38e248a84f6b5e206cb0bf3ade7378c00d1cf662125315341a143b63f9e8f72cdf91a0db17&scene=21#wechat_redirect)
3，[Hbase源码系列之scan源码解析及调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483739&idx=1&sn=22647805307d21d6f893bea04c78153c&chksm=9f38e273a84f6b65be8cac2cc619a65e80f31235b43779b056db0b220b91559cf4a477dbb8bb&scene=21#wechat_redirect)
4，[如何将RDD或者MLLib矩阵zhuanzhi](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484349&idx=1&sn=32927ca184a284a0d299fa99a9ba2629&chksm=9f38e095a84f69837217c729583d5a82e9d88d0ed0d4a177b40af9226eb579e9c1db5bff925c&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)

**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

