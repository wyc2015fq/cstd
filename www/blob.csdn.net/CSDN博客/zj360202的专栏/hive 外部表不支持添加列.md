# hive 外部表不支持添加列 - zj360202的专栏 - CSDN博客





2013年04月11日 11:07:26[zj360202](https://me.csdn.net/zj360202)阅读数：1536







`alter table hbase_fct_mcht_item_day add columns(avg_stay_time_bi bigint);`会报如下错误：

```java
FAILED: Error in metadata: Cannot use ALTER TABLE on a non-native table
FAILED: Execution Error, return code 1 from org.apache.hadoop.hive.ql.exec.DDLTask
```


外部表只能重新创建
            


