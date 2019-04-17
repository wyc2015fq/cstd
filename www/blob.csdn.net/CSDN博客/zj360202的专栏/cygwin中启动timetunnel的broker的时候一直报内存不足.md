# cygwin中启动timetunnel的broker的时候一直报内存不足 - zj360202的专栏 - CSDN博客





2012年12月12日 11:25:04[zj360202](https://me.csdn.net/zj360202)阅读数：928








![](https://img-my.csdn.net/uploads/201212/12/1355282730_3224.jpg)

一直以为是cygwin或java配置的内存太小，一直去查找修改cygwin内存和java内存的方法，但是都不行，最后才想到是broker的启动脚本broker-start.sh中有一段

script="java -XX:SurvivorRatio=5 -XX:-UseAdaptiveSizePolicy -XX:NewRatio=1 -XX:+PrintGCTimeStamps -XX:+PrintGCDetails -XX:HeapDumpPath=${BASE_LOG}/broker.hprof -XX:ErrorFile=${BASE_LOG}/crash.log -XX:+PrintCommandLineFlags
-Xmx2048m -Xms2048m -XX:+HeapDumpOnOutOfMemoryError -Dtt.log.file=${BASE_LOG}/broker.log -Dlog4j.configuration=file:${BASE_CONF}/log4j.properties -classpath ${BASE_LIB}/*: com.taobao.timetunnel.bootstrap.BrokerBootstrap ${BASE_CONF}/conf.properties";

echo $script

这个是初始化java内存，而java的默认最大是1024M,所以java没有办法给broker分配2048M，因此将2048改成1024就可以了



script="java -XX:SurvivorRatio=5 -XX:-UseAdaptiveSizePolicy -XX:NewRatio=1 -XX:+PrintGCTimeStamps -XX:+PrintGCDetails -XX:HeapDumpPath=${BASE_LOG}/broker.hprof -XX:ErrorFile=${BASE_LOG}/crash.log -XX:+PrintCommandLineFlags -Xmx1024m
 -Xms1024m -XX:+HeapDumpOnOutOfMemoryError -Dtt.log.file=${BASE_LOG}/broker.log -Dlog4j.configuration=file:${BASE_CONF}/log4j.properties -classpath ${BASE_LIB}/*: com.taobao.timetunnel.bootstrap.BrokerBootstrap ${BASE_CONF}/conf.properties";

echo $script





