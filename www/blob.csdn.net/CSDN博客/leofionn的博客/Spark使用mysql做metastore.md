
# Spark使用mysql做metastore - leofionn的博客 - CSDN博客


2018年03月12日 20:57:47[leofionn](https://me.csdn.net/qq_36142114)阅读数：141


方法一：
cp /usr/local/hive/conf/hive-site.xml /usr/local/spark/conf/
./spark-shell --master local[2] --jars /usr/local/hive/lib/mysql-connector-java-5.1.45-bin.jar

方法二：
cp /usr/local/hive/lib/mysql-connector-java-5.1.45-bin.jar /usr/local/spark/jars/
./spark-shell --master local[2]


【来自@若泽大数据】


