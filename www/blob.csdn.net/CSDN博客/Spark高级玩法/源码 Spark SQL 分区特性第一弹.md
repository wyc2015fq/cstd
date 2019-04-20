# 源码:Spark SQL 分区特性第一弹 - Spark高级玩法 - CSDN博客
2018年12月09日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：7238
头条号上说过近期分享Spark SQL系列文章，前面在头条号上分享了Dataset API的基本操作和复杂操作，不知道下面大家有没有自己测试一下。
今天主要是分享Spark SQL Dataset数据源的分区特性，而且是第一弹的数据格式是partquet。
**常见RDD分区**
Spark Core 中的RDD的分区特性大家估计都很了解，这里说的分区特性是指从数据源读取数据的第一个RDD或者Dataset的分区，而后续再介绍转换过程中分区的变化。
举几个浪尖在星球里分享比较多的例子，比如：
- 
Spark Streaming 与kafka 结合 DirectDstream 生成的微批RDD（kafkardd）分区数和kafka分区数一样。
- 
Spark Streaming 与kafka结合 基于receiver的方式，生成的微批RDD（blockRDD），分区数就是block数。
- 
普通的文件RDD，那么分可分割和不可分割，通常不可分割的分区数就是文件数。可分割需要计算而且是有条件的，在星球里分享过了。
这些都很简单，那么今天咱们要谈的是Spark DataSet的分区数的决定因素。
**准备数据**
首先是由Seq数据集合生成一个Dataset
val sales = spark.createDataFrame(Seq(
      ("Warsaw", 2016, 110),
      ("Warsaw", 2017, 10),
      ("Warsaw", 2015, 100),
      ("Warsaw", 2015, 50),
      ("Warsaw", 2015, 80),
      ("Warsaw", 2015, 100),
      ("Warsaw", 2015, 130),
      ("Warsaw", 2015, 160),
      ("Warsaw", 2017, 200),
      ("Beijing", 2017, 100),
      ("Beijing", 2016, 150),
      ("Beijing", 2015, 50),
      ("Beijing", 2015, 30),
      ("Beijing", 2015, 10),
      ("Beijing", 2014, 200),
      ("Beijing", 2014, 170),
      ("Boston", 2017, 50),
      ("Boston", 2017, 70),
      ("Boston", 2017, 110),
      ("Boston", 2017, 150),
      ("Boston", 2017, 180),
      ("Boston", 2016, 30),
      ("Boston", 2015, 200),
      ("Boston", 2014, 20)
    )).toDF("city", "year", "amount")
将Dataset存处为partquet格式的hive表，分两种情况：
用city和year字段分区
sales.write.partitionBy("city","year").mode(SaveMode.Overwrite).saveAsTable("ParquetTestCityAndYear")
用city字段分区
sales.write.partitionBy("city").mode(SaveMode.Overwrite).saveAsTable("ParquetTestCity")
读取数据采用的是
val res = spark.read.parquet("/user/hive/warehouse/parquettestcity")
直接展示，结果发现结果会随着spark.default.parallelism变化而变化。文章里只读取city字段分区的数据，特点就是只有单个分区字段。
**1. spark.default.parallelism =40**
Dataset的分区数是由参数：
`println("partition size = "+res.rdd.partitions.length)`
目录数和生成的FileScanRDD的分区数分别数下面截图的第一行和第二行。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibDu2gaX89M600Ip3MDrVYQdrjhFibsnK47og5cdZ77AXMDFVGAaEZLibeicsR1hw5icr1wC26kM7lRA/640)
这个分区数目正好是文件数，那么假如不了解细节的话，肯定会认为分区数就是由文件数决定的，其实不然。
**2. spark.default.parallelism =4**
Dataset的分区数是由参数：
`println("partition size = "+res.rdd.partitions.length)`
目录数和生成的FileScanRDD的分区数分别数下面截图的第一行和第二行。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibDu2gaX89M600Ip3MDrVYZKkgN5JLzbDDNNMDeKw4dmCGjFCdHvBPia5am813qyQbV0a7p8UgEuQ/640)
那么数据源生成的Dataset的分区数到底是如何决定的呢？
我们这种情况，我只能告诉你是由下面的函数在生成FileScanRDD的时候计算得到的，具体计算细节可以仔细阅读该函数。该函数是类FileSourceScanExec的方法。
private def createNonBucketedReadRDD(
                                        readFile: (PartitionedFile) => Iterator[InternalRow],
                                        selectedPartitions: Seq[PartitionDirectory],
                                        fsRelation: HadoopFsRelation): RDD[InternalRow] = {
    /*
      selectedPartitions 的大小代表目录数目
     */
    println("selectedPartitions.size : "+ selectedPartitions.size)
    val defaultMaxSplitBytes =
      fsRelation.sparkSession.sessionState.conf.filesMaxPartitionBytes
    val openCostInBytes = fsRelation.sparkSession.sessionState.conf.filesOpenCostInBytes
    // spark.default.parallelism
    val defaultParallelism = fsRelation.sparkSession.sparkContext.defaultParallelism
    // 计算文件总大小，单位字节数
    val totalBytes = selectedPartitions.flatMap(_.files.map(_.getLen + openCostInBytes)).sum
    //计算平均每个并行度读取数据大小
    val bytesPerCore = totalBytes / defaultParallelism
    // 首先spark.sql.files.openCostInBytes 该参数配置的值和bytesPerCore 取最大值
    // 然后，比较spark.sql.files.maxPartitionBytes 取小者
    val maxSplitBytes = Math.min(defaultMaxSplitBytes, Math.max(openCostInBytes, bytesPerCore))
    logInfo(s"Planning scan with bin packing, max size: $maxSplitBytes bytes, " +
      s"open cost is considered as scanning $openCostInBytes bytes.")
    // 这对目录遍历
    val splitFiles = selectedPartitions.flatMap { partition =>
      partition.files.flatMap { file =>
        val blockLocations = getBlockLocations(file)
        //判断文件类型是否支持分割，以parquet为例，是支持分割的
if (fsRelation.fileFormat.isSplitable(
          fsRelation.sparkSession, fsRelation.options, file.getPath)) {
//          eg. 0 until 2不包括 2。相当于
//        println(0 until(10) by 3) 输出 Range(0, 3, 6, 9)
          (0L until file.getLen by maxSplitBytes).map { offset =>
            // 计算文件剩余的量
            val remaining = file.getLen - offset
//            假如剩余量不足 maxSplitBytes 那么就剩余的作为一个分区
            val size = if (remaining > maxSplitBytes) maxSplitBytes else remaining
//            位置信息
            val hosts = getBlockHosts(blockLocations, offset, size)
            PartitionedFile(
              partition.values, file.getPath.toUri.toString, offset, size, hosts)
          }
        } else {
//          不可分割的话，那即是一个文件一个分区
          val hosts = getBlockHosts(blockLocations, 0, file.getLen)
          Seq(PartitionedFile(
            partition.values, file.getPath.toUri.toString, 0, file.getLen, hosts))
        }
      }
    }.toArray.sortBy(_.length)(implicitly[Ordering[Long]].reverse)
    val partitions = new ArrayBuffer[FilePartition]
    val currentFiles = new ArrayBuffer[PartitionedFile]
    var currentSize = 0L
    /** Close the current partition andmoveto the next. */
    def closePartition(): Unit = {
if (currentFiles.nonEmpty) {
        val newPartition =
          FilePartition(
            partitions.size,
            currentFiles.toArray.toSeq) // Copy toanew Array.
        partitions += newPartition
      }
      currentFiles.clear()
      currentSize = 0
    }
    // Assign filesto partitions using "Next Fit Decreasing"
    splitFiles.foreach { file =>
if (currentSize + file.length > maxSplitBytes) {
        closePartition()
      }
      // Add the given fileto the current partition.
      currentSize += file.length + openCostInBytes
      currentFiles += file
    }
    closePartition()
    println("FileScanRDD partitions size : "+partitions.size)
new FileScanRDD(fsRelation.sparkSession, readFile, partitions)
  }
这次视频上传到了今日头条了，可以点击**阅读原文**观看。
知识星球球友可以直接获取视频及所有spark SQL所有相关的代码。
欢迎加入知识星球，学习像本文一样直接调试源码，更加深入掌握Spark SQL～
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640)
