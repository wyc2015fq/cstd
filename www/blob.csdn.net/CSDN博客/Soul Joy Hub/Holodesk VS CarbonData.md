# Holodesk VS CarbonData - Soul Joy Hub - CSDN博客

2016年07月24日 20:31:57[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1678


# 总概

## Holodesk

### 应用场景
- 增强在交互分析中Ad-hoc query的高效性
- 支持流应用insert & update & delete

![这里写图片描述](https://img-blog.csdn.net/20160724193647852)

## CarbonData

![这里写图片描述](https://img-blog.csdn.net/20160724193817469)

### 应用场景
- 支持 big scan & 少列结果
- 支持在亚秒级响应主键查找
- 支持大数据上涉及一个query中有许多过滤的interactive OLAP-style query, 并能以秒级响应
- 支持包含全列的单条记录的快速抽取
- 支持 HDFS 以便用户可以管理正存在的Hadoop集群

# 技术
|Holodesk|CarbonData|
|----|----|
|DELTA，RLE，Dictionary|DELTA，RLE，Dictionary，BIT_PACKED，Snappy|
|MinMax Filter，Bloom Filter|内存文件级MDK index tree|
|B+ Tree，Bitmap|倒排索引|

# 性能表现

## Holodesk

![这里写图片描述](https://img-blog.csdn.net/20160724201958747)

## CarbonData

![这里写图片描述](https://img-blog.csdn.net/20160724202158828)

