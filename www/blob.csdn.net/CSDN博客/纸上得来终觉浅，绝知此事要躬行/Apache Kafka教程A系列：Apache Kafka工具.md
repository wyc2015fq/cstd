# Apache Kafka教程A系列：Apache Kafka工具 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月09日 16:36:50[boonya](https://me.csdn.net/boonya)阅读数：164








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_tools.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_tools.htm)

卡夫卡工具包在“org.apache.kafka.tools.*”下。 工具分为系统工具和复制工具。


##### 系统工具

系统工具可以使用run class脚本从命令行运行。 语法如下 -


bin/kafka-run-class.sh package.class-- options下面提到了一些系统工具 - 
- Kafka迁移工具 - 此工具用于将代理从一个版本迁移到另一个版本。
- Mirror Maker - 此工具用于将一个Kafka集群镜像到另一个。
- 消费者偏移量检查器 - 此工具显示指定的一组主题和使用者组的消费者组，主题，分区，偏移量，日志大小，所有者。



##### 复制工具

卡夫卡复制是一个高层次的设计工具。 添加复制工具的目的是提供更强的耐用性和更高的可用性。 下面提到了一些复制工具 -


- 创建主题工具 - 这会创建一个包含默认分区数量，复制因子的主题，并使用Kafka的默认方案执行副本分配。
- 列表主题工具 - 此工具列出给定主题列表的信息。 如果在命令行中没有提供主题，该工具将查询Zookeeper以获取所有主题并列出它们的信息。 该工具显示的字段是主题名称，分区，领导，副本，isr。
- 添加分区工具 - 创建主题时，必须指定主题的分区数量。 稍后，当话题量增加时，话题可能需要更多分区。 此工具有助于为特定主题添加更多分区，还可以手动添加分区的副本分配。



