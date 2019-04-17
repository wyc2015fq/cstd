# hive压缩配置 - zj360202的专栏 - CSDN博客





2014年08月07日 16:23:13[zj360202](https://me.csdn.net/zj360202)阅读数：576








在代码中加入



```java
conf.set("mapred.compress.map.output", "true")
conf.set("mapred.output.compression.type", "BLOCK");
conf.set("mapred.map.output.compression.codec", "org.apache.hadoop.io.compress.GzipCodec");
```


如果是在hive 客户端，在hive语句执行之前加入



```java
set mapred.compress.map.output=true;
set mapred.output.compression.type=BLOCK;
set mapred.map.output.compression.codec=org.apache.hadoop.io.compress.GzipCodec;
```





