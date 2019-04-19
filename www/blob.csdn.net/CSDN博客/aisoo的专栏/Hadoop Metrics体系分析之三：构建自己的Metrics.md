# Hadoop Metrics体系分析之三：构建自己的Metrics - aisoo的专栏 - CSDN博客
2012年12月12日 15:18:41[aisoo](https://me.csdn.net/aisoo)阅读数：1286
大型分布式系统中需要metrics来了解系统状态已成为系统必需的功能之一。其实测试系统甚至测试用例中也同样需要metrics。通过这些指标我们可以了解测试的进度、状况、以及一些过程情况，比如性能指标和一些无法用是否判断数据。下面我们就用一个简单的例子来看看如何使用hadoop
 metrics。
创建Updater
Updater是一个拥有doUpdates方法的接口，将实现了这个接口的类注册到MetricsContext中，context就能周期性的调用doUpdates来收集metrics。因此实现Updater是metrics框架应用中最重要的事情。下面是一个简单的Updater：

``
**public class **ClientMetrics **implements **Updater {
**private **MetricsContext context;
**private **MetricsRegistry registry = **new **MetricsRegistry();
**private **MetricsRecord metricsRecord;
**private ****long **lastUpdate = System.currentTimeMillis();
// 建立一个metrics用于统计1秒内的写请求数
**private final **MetricsRate writeRequests = **new **MetricsRate("writeRequests",
registry);
**public **ClientMetrics() {
// 获取一个名为test-client的context，并启动context的monitor线程开始收集数据
context = MetricsUtil.getContext("test-client");
// 在context中新建一个名为metrics的record
metricsRecord = MetricsUtil.createRecord(context, "metrics");
// 将updater注册到context中
context.registerUpdater(**this**);
}
@Override
// context采集metrics数据
**public ****void **doUpdates(MetricsContext context) {
**synchronized **(**this**) {
// 将一个metrics 写入record
**this**.writeRequests.pushMetric(**this**.metricsRecord);
}
// 将record的数据更新到context
**this**.metricsRecord.update();
}
**public ****void **incrementWriteRequests(**final ****int **inc) {
**this**.writeRequests.inc(inc);
}
**public ****void **stop() {
**if **(context != **null**) {
// 停止一个context
metricsRecord.remove();
context.close();
context = **null**;
}
}
}
创建配置文件
配置文件的名称通常为hadoop-metrics.properties，需要放置在classpath中：
`test-client.class=org.apache.hadoop.metrics.ganglia.GangliaContext31`
test-client.period=15
test-client.servers= hostname:8649
通过以上的操作，配合ganglia，metrics模块就可以运行起来了，这也是最常用的方式，但是也存在着很多不方便的地方。下面就分享下我们metrics应用的一些经验。
数据收集完成之后
Hadoop metrics框架以及Ganglia很好的完成了数据的采集和集群规模的收集工作。但是也有很多让我们不爽的地方：
1、  ganglia仅能按照1小时、1天、1周、1年来浏览数据，显然无法做进一步的分析处理。为了满足我们随意时间查询以及多重数据整合分析的需求，必须将数据从ganglia中取出来另外存储为更加灵活的数据结构。经过同事的一些实践发现最靠谱的就是文件存储。于是就有了以下流程：hadoop
 metrics采集-->ganglia收集汇总-->自定义程序转存为固定格式文件-->web查询分析界面及程序。这个方案充分利用了文件的快速检索和顺序读优势，而且每个指标一个文件也方便迁移和管理。
2、  如果一个集群的指标太多、实时性要求越来越高、分析要求越来越复杂，那我们就不得不放弃ganglia。通过扩展MetricsContext可以实现自己的数据发送算法，将集群数据发送到数据处理中心，通过更加有针对性的数据处理方案来分析海量指标数据。
通过上面的介绍我想大家已经掌握了metrics体系的构建思路，不一定非要使用hadoop的metrics框架我们也完全有能力创造出更适合自己的metrics解决方案。
