# Hive – Group By 的实现 explain分析 - zj360202的专栏 - CSDN博客





2014年08月07日 16:31:00[zj360202](https://me.csdn.net/zj360202)阅读数：2290








## 准备数据


```java
SELECT uid, SUM(COUNT) FROM logs GROUP BY uid;
```




```java
hive> SELECT * FROM logs;
a	苹果	5
a	橙子	3
a      苹果   2
b	烧鸡	1
 
hive> SELECT uid, SUM(COUNT) FROM logs GROUP BY uid;
a	10
b	1
```




## 计算过程

![hive-groupby-cal](http://fatkun.com/wp-content/uploads/hive-groupby-cal.jpg)

默认设置了hive.map.aggr=true，所以会在mapper端先group by一次，最后再把结果merge起来，为了减少reducer处理的数据量。注意看explain的mode是不一样的。mapper是hash，reducer是mergepartial。如果把hive.map.aggr=false，那将groupby放到reducer才做，他的mode是complete.

## Operator

![hive-groupby-op](http://fatkun.com/wp-content/uploads/hive-groupby-op.png)

## Explain



```java
hive> explain SELECT uid, sum(count) FROM logs group by uid;
OK
ABSTRACT SYNTAX TREE:
  (TOK_QUERY (TOK_FROM (TOK_TABREF (TOK_TABNAME logs))) (TOK_INSERT (TOK_DESTINATION (TOK_DIR TOK_TMP_FILE)) (TOK_SELECT (TOK_SELEXPR (TOK_TABLE_OR_COL uid)) (TOK_SELEXPR (TOK_FUNCTION sum (TOK_TABLE_OR_COL count)))) (TOK_GROUPBY (TOK_TABLE_OR_COL uid))))
 
STAGE DEPENDENCIES:
  Stage-1 is a root stage
  Stage-0 is a root stage
 
STAGE PLANS:
  Stage: Stage-1
    Map Reduce
      Alias -> Map Operator Tree:
        logs 
          TableScan // 扫描表
            alias: logs
            Select Operator //选择字段
              expressions:
                    expr: uid
                    type: string
                    expr: count
                    type: int
              outputColumnNames: uid, count
              Group By Operator //这里是因为默认设置了hive.map.aggr=true，会在mapper先做一次聚合，减少reduce需要处理的数据
                aggregations:
                      expr: sum(count) //聚集函数
                bucketGroup: false
                keys: //键
                      expr: uid
                      type: string
                mode: hash //hash方式，processHashAggr()
                outputColumnNames: _col0, _col1
                Reduce Output Operator //输出key，value给reducer
                  key expressions:
                        expr: _col0
                        type: string
                  sort order: +
                  Map-reduce partition columns:
                        expr: _col0
                        type: string
                  tag: -1
                  value expressions:
                        expr: _col1
                        type: bigint
      Reduce Operator Tree:
        Group By Operator
 
          aggregations:
                expr: sum(VALUE._col0)
//聚合
          bucketGroup: false
          keys:
                expr: KEY._col0
                type: string
          mode: mergepartial //合并值
          outputColumnNames: _col0, _col1
          Select Operator //选择字段
            expressions:
                  expr: _col0
                  type: string
                  expr: _col1
                  type: bigint
            outputColumnNames: _col0, _col1
            File Output Operator //输出到文件
              compressed: false
              GlobalTableId: 0
              table:
                  input format: org.apache.hadoop.mapred.TextInputFormat
                  output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat
 
  Stage: Stage-0
    Fetch Operator
      limit: -1

hive> select distinct value from src; 
hive> select max(key) from src; 

因为没有grouping keys，所以只有一个reducer。



  

      2.2 如果有聚合函数或者groupby，做如下处理： 

            插入一个select operator，选取所有的字段，用于优化阶段ColumnPruner的优化 

            2.2.1  hive.map.aggr为true，默认是true，开启的，在map端做部分聚合 

                  2.2.1.1 hive.groupby.skewindata为false，默认是关闭的，groupby的数据没有倾斜。      

                  生成的operator是: GroupByOperator+ReduceSinkOperator+GroupByOperator。 

      GroupByOperator+ReduceSinkOperator用于在map端做操作，第一个GroupByOperator在map端先做部分聚合。第二个用于在reduce端做GroupBy操作 

                  2.2.1.2 hive.groupby.skewindata为true 

                  生成的operator是: GroupbyOperator+ReduceSinkOperator+GroupbyOperator+ReduceSinkOperator +GroupByOperator 

               GroupbyOperator+ReduceSinkOperator（第一个MapredTask的map阶段） 

               GroupbyOperator(第一个MapredTask的reduce阶段) 

               ReduceSinkOperator （第二个MapredTask的map阶段） 

               GroupByOperator（第二个MapredTask的reduce阶段） 

            2.2.2  hive.map.aggr为false 

                   2.2.2.1 hive.groupby.skewindata为true 

                    生成的operator是: ReduceSinkOperator+GroupbyOperator+ReduceSinkOperator +GroupByOperator                    

               ReduceSinkOperator(第一个MapredTask的map阶段) 

               GroupbyOperator(第一个MapredTask的reduce阶段) 

               ReduceSinkOperator （第二个MapredTask的map阶段） 

               GroupByOperator（第二个MapredTask的reduce阶段） 

                   2.2.2.2 hive.groupby.skewindata为false 

                    生成的operator是: ReduceSinkOperator(map阶段运行)+GroupbyOperator（reduce阶段运行） 







第一种情况：

set hive.map.aggr=false;

set hive.groupby.skewindata=false;

SemanticAnalyzer.genGroupByPlan1MR(){

  （1）ReduceSinkOperator： It will put all Group By keys and the 
distinct field (if any) in the map-reduce sort key, and all other fields
 in the map-reduce value.

  （2）GroupbyOperator：GroupByDesc.Mode.COMPLETE，Reducer: iterate/merge (mode = COMPLETE)

}



第二种情况：

set hive.map.aggr=true; 

set hive.groupby.skewindata=false; 

SemanticAnalyzer.genGroupByPlanMapAggr1MR(){

   （1）GroupByOperator：GroupByDesc.Mode.HASH，The agggregation 
evaluation functions are as follows: Mapper: iterate/terminatePartial 
(mode = HASH)

   （2）ReduceSinkOperator：Partitioning Key: grouping key。Sorting Key:
 grouping key if no DISTINCT grouping + distinct key if DISTINCT

   （3）GroupByOperator：GroupByDesc.Mode.MERGEPARTIAL，Reducer: 
iterate/terminate if DISTINCT merge/terminate if NO DISTINCT (mode = 
MERGEPARTIAL)

}



第三种情况：

set hive.map.aggr=false; 

set hive.groupby.skewindata=true; 

SemanticAnalyzer.genGroupByPlan2MR(){

    （1）ReduceSinkOperator：Partitioning Key: random() if no DISTINCT 
grouping + distinct key if DISTINCT。Sorting Key: grouping key if no 
DISTINCT grouping + distinct key if DISTINCT

    （2）GroupbyOperator：GroupByDesc.Mode.PARTIAL1，Reducer: iterate/terminatePartial (mode = PARTIAL1)

    （3）ReduceSinkOperator：Partitioning Key: grouping key。Sorting 
Key: grouping key if no DISTINCT grouping + distinct key if DISTINCT

    （4）GroupByOperator：GroupByDesc.Mode.FINAL，Reducer: merge/terminate (mode = FINAL)

}



第四种情况：

set hive.map.aggr=true; 

set hive.groupby.skewindata=true; 

SemanticAnalyzer.genGroupByPlanMapAggr2MR(){

     （1）GroupbyOperator：GroupByDesc.Mode.HASH，Mapper: iterate/terminatePartial (mode = HASH)

     （2）ReduceSinkOperator： Partitioning Key: random() if no 
DISTINCT grouping + distinct key if DISTINCT。 Sorting Key: grouping key 
if no DISTINCT grouping + distinct key if DISTINCT。

     （3）GroupbyOperator：GroupByDesc.Mode.PARTIALS， Reducer: 
iterate/terminatePartial if DISTINCT merge/terminatePartial if NO 
DISTINCT (mode = MERGEPARTIAL)

     （4）ReduceSinkOperator：Partitioining Key: grouping key。Sorting 
Key: grouping key if no DISTINCT grouping + distinct key if DISTINCT

     （5）GroupByOperator：GroupByDesc.Mode.FINAL，Reducer: merge/terminate (mode = FINAL)

}





ReduceSinkOperator的processOp(Object row, int 
tag)会根据相应的条件设置Key的hash值，如第四种情况的第一个ReduceSinkOperator：Partitioning Key: 
random() if no DISTINCT grouping + distinct key if 
DISTINCT，如果没有DISTINCT字段，那么在OutputCollector.collect前会设置当前Key的hash值为一个随机
数，random = new Random(12345);。如果有DISTINCT字段，那么key的hash值跟grouping + 
distinct key有关。







GroupByOperator：

initializeOp(Configuration hconf)

processOp(Object row, int tag) 

closeOp(boolean abort)

forward(ArrayList<Object> keys, AggregationBuffer[] aggs)





groupby10.q   groupby11.q

set hive.map.aggr=false;

set hive.groupby.skewindata=false;



EXPLAIN

FROM INPUT 

INSERT OVERWRITE TABLE dest1 SELECT INPUT.key, 
count(substr(INPUT.value,5)), count(distinct substr(INPUT.value,5)) 
GROUP BY INPUT.key;



STAGE DEPENDENCIES:

  Stage-1 is a root stage

  Stage-0 depends on stages: Stage-1



STAGE PLANS:

  Stage: Stage-1

    Map Reduce

      Alias -> Map Operator Tree:

        input 

          TableScan

            alias: input

            Select Operator  // insertSelectAllPlanForGroupBy 

              expressions:

                    expr: key

                    type: int

                    expr: value

                    type: string

              outputColumnNames: key, value

              Reduce Output Operator

                key expressions:

                      expr: key

                      type: int

                      expr: substr(value, 5)

                      type: string

                sort order: ++

                Map-reduce partition columns:

                      expr: key

                      type: int

                tag: -1

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(KEY._col1:0._col0)

                expr: count(DISTINCT KEY._col1:0._col0)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: complete

          outputColumnNames: _col0, _col1, _col2

          Select Operator

            expressions:

                  expr: _col0

                  type: int

                  expr: _col1

                  type: bigint

                  expr: _col2

                  type: bigint

            outputColumnNames: _col0, _col1, _col2

            Select Operator

              expressions:

                    expr: _col0

                    type: int

                    expr: UDFToInteger(_col1)

                    type: int

                    expr: UDFToInteger(_col2)

                    type: int

              outputColumnNames: _col0, _col1, _col2

              File Output Operator

                compressed: false

                GlobalTableId: 1

                table:

                    input format: org.apache.hadoop.mapred.TextInputFormat

                    output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                    serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                    name: dest1



  Stage: Stage-0

    Move Operator

      tables:

          replace: true

          table:

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: dest1









set hive.map.aggr=true;

set hive.groupby.skewindata=false;



STAGE DEPENDENCIES:

  Stage-1 is a root stage

  Stage-0 depends on stages: Stage-1



STAGE PLANS:

  Stage: Stage-1

    Map Reduce

      Alias -> Map Operator Tree:

        input 

          TableScan

            alias: input

            Select Operator

              expressions:

                    expr: key

                    type: int

                    expr: value

                    type: string

              outputColumnNames: key, value

              Group By Operator

                aggregations:

                      expr: count(substr(value, 5))

                      expr: count(DISTINCT substr(value, 5))

                bucketGroup: false

                keys:

                      expr: key

                      type: int

                      expr: substr(value, 5)

                      type: string

                mode: hash

                outputColumnNames: _col0, _col1, _col2, _col3

                Reduce Output Operator

                  key expressions:

                        expr: _col0

                        type: int

                        expr: _col1

                        type: string

                  sort order: ++

                  Map-reduce partition columns:

                        expr: _col0

                        type: int

                  tag: -1

                  value expressions:

                        expr: _col2

                        type: bigint

                        expr: _col3

                        type: bigint

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(VALUE._col0)

                expr: count(DISTINCT KEY._col1:0._col0)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: mergepartial

          outputColumnNames: _col0, _col1, _col2

          Select Operator

            expressions:

                  expr: _col0

                  type: int

                  expr: _col1

                  type: bigint

                  expr: _col2

                  type: bigint

            outputColumnNames: _col0, _col1, _col2

            Select Operator

              expressions:

                    expr: _col0

                    type: int

                    expr: UDFToInteger(_col1)

                    type: int

                    expr: UDFToInteger(_col2)

                    type: int

              outputColumnNames: _col0, _col1, _col2

              File Output Operator

                compressed: false

                GlobalTableId: 1

                table:

                    input format: org.apache.hadoop.mapred.TextInputFormat

                    output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                    serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                    name: dest1



  Stage: Stage-0

    Move Operator

      tables:

          replace: true

          table:

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: dest1

















set hive.map.aggr=false;

set hive.groupby.skewindata=true;



STAGE DEPENDENCIES:

  Stage-1 is a root stage

  Stage-2 depends on stages: Stage-1

  Stage-0 depends on stages: Stage-2



STAGE PLANS:

  Stage: Stage-1

    Map Reduce

      Alias -> Map Operator Tree:

        input 

          TableScan

            alias: input

            Select Operator

              expressions:

                    expr: key

                    type: int

                    expr: value

                    type: string

              outputColumnNames: key, value

              Reduce Output Operator

                key expressions:

                      expr: key

                      type: int

                      expr: substr(value, 5)

                      type: string

                sort order: ++

                Map-reduce partition columns:

                      expr: key

                      type: int

                tag: -1

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(KEY._col1:0._col0)

                expr: count(DISTINCT KEY._col1:0._col0)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: partial1

          outputColumnNames: _col0, _col1, _col2

          File Output Operator

            compressed: false

            GlobalTableId: 0

            table:

                input format: org.apache.hadoop.mapred.SequenceFileInputFormat

                output format: org.apache.hadoop.hive.ql.io.HiveSequenceFileOutputFormat



  Stage: Stage-2

    Map Reduce

      Alias -> Map Operator Tree:

        hdfs://localhost:54310/tmp/hive-tianzhao/hive_2011-07-15_21-48-26_387_7978992474997402829/-mr-10002 

            Reduce Output Operator

              key expressions:

                    expr: _col0

                    type: int

              sort order: +

              Map-reduce partition columns:

                    expr: _col0

                    type: int

              tag: -1

              value expressions:

                    expr: _col1

                    type: bigint

                    expr: _col2

                    type: bigint

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(VALUE._col0)

                expr: count(VALUE._col1)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: final

          outputColumnNames: _col0, _col1, _col2

          Select Operator

            expressions:

                  expr: _col0

                  type: int

                  expr: _col1

                  type: bigint

                  expr: _col2

                  type: bigint

            outputColumnNames: _col0, _col1, _col2

            Select Operator

              expressions:

                    expr: _col0

                    type: int

                    expr: UDFToInteger(_col1)

                    type: int

                    expr: UDFToInteger(_col2)

                    type: int

              outputColumnNames: _col0, _col1, _col2

              File Output Operator

                compressed: false

                GlobalTableId: 1

                table:

                    input format: org.apache.hadoop.mapred.TextInputFormat

                    output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                    serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                    name: dest1



  Stage: Stage-0

    Move Operator

      tables:

          replace: true

          table:

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: dest1









set hive.map.aggr=true;

set hive.groupby.skewindata=true;



STAGE DEPENDENCIES:

  Stage-1 is a root stage

  Stage-2 depends on stages: Stage-1

  Stage-0 depends on stages: Stage-2



STAGE PLANS:

  Stage: Stage-1

    Map Reduce

      Alias -> Map Operator Tree:

        input 

          TableScan

            alias: input

            Select Operator

              expressions:

                    expr: key

                    type: int

                    expr: value

                    type: string

              outputColumnNames: key, value

              Group By Operator

                aggregations:

                      expr: count(substr(value, 5))

                      expr: count(DISTINCT substr(value, 5))

                bucketGroup: false

                keys:

                      expr: key

                      type: int

                      expr: substr(value, 5)

                      type: string

                mode: hash

                outputColumnNames: _col0, _col1, _col2, _col3

                Reduce Output Operator

                  key expressions:

                        expr: _col0

                        type: int

                        expr: _col1

                        type: string

                  sort order: ++

                  Map-reduce partition columns:

                        expr: _col0

                        type: int

                  tag: -1

                  value expressions:

                        expr: _col2

                        type: bigint

                        expr: _col3

                        type: bigint

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(VALUE._col0)

                expr: count(DISTINCT KEY._col1:0._col0)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: partials

          outputColumnNames: _col0, _col1, _col2

          File Output Operator

            compressed: false

            GlobalTableId: 0

            table:

                input format: org.apache.hadoop.mapred.SequenceFileInputFormat

                output format: org.apache.hadoop.hive.ql.io.HiveSequenceFileOutputFormat



  Stage: Stage-2

    Map Reduce

      Alias -> Map Operator Tree:

        hdfs://localhost:54310/tmp/hive-tianzhao/hive_2011-07-15_21-49-25_899_4946067838822964610/-mr-10002 

            Reduce Output Operator

              key expressions:

                    expr: _col0

                    type: int

              sort order: +

              Map-reduce partition columns:

                    expr: _col0

                    type: int

              tag: -1

              value expressions:

                    expr: _col1

                    type: bigint

                    expr: _col2

                    type: bigint

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(VALUE._col0)

                expr: count(VALUE._col1)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: final

          outputColumnNames: _col0, _col1, _col2

          Select Operator

            expressions:

                  expr: _col0

                  type: int

                  expr: _col1

                  type: bigint

                  expr: _col2

                  type: bigint

            outputColumnNames: _col0, _col1, _col2

            Select Operator

              expressions:

                    expr: _col0

                    type: int

                    expr: UDFToInteger(_col1)

                    type: int

                    expr: UDFToInteger(_col2)

                    type: int

              outputColumnNames: _col0, _col1, _col2

              File Output Operator

                compressed: false

                GlobalTableId: 1

                table:

                    input format: org.apache.hadoop.mapred.TextInputFormat

                    output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                    serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                    name: dest1



  Stage: Stage-0

    Move Operator

      tables:

          replace: true

          table:

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: dest1

















set hive.map.aggr=false;

set hive.groupby.skewindata=false;



EXPLAIN extended

FROM INPUT 

INSERT OVERWRITE TABLE dest1 SELECT INPUT.key, 
count(substr(INPUT.value,5)), count(distinct substr(INPUT.value,5)) 
GROUP BY INPUT.key;



STAGE DEPENDENCIES:

  Stage-1 is a root stage

  Stage-0 depends on stages: Stage-1



STAGE PLANS:

  Stage: Stage-1

    Map Reduce

      Alias -> Map Operator Tree:

        input 

          TableScan

            alias: input

            Select Operator

              expressions:

                    expr: key

                    type: int

                    expr: value

                    type: string

              outputColumnNames: key, value

              Reduce Output Operator

                key expressions:

                      expr: key

                      type: int

                      expr: substr(value, 5)

                      type: string

                sort order: ++

                Map-reduce partition columns:

                      expr: key

                      type: int

                tag: -1

      Needs Tagging: false

      Path -> Alias:

        hdfs://localhost:54310/user/hive/warehouse/input [input]

      Path -> Partition:

        hdfs://localhost:54310/user/hive/warehouse/input 

          Partition

            base file name: input

            input format: org.apache.hadoop.mapred.TextInputFormat

            output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

            properties:

              bucket_count -1

              columns key,value

              columns.types int:string

              file.inputformat org.apache.hadoop.mapred.TextInputFormat

              file.outputformat org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              location hdfs://localhost:54310/user/hive/warehouse/input

              name input

              serialization.ddl struct input { i32 key, string value}

              serialization.format 1

              serialization.lib org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              transient_lastDdlTime 1310523947

            serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

          

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              properties:

                bucket_count -1

                columns key,value

                columns.types int:string

                file.inputformat org.apache.hadoop.mapred.TextInputFormat

                file.outputformat org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                location hdfs://localhost:54310/user/hive/warehouse/input

                name input

                serialization.ddl struct input { i32 key, string value}

                serialization.format 1

                serialization.lib org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                transient_lastDdlTime 1310523947

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: input

            name: input

      Reduce Operator Tree:

        Group By Operator

          aggregations:

                expr: count(KEY._col1:0._col0)

                expr: count(DISTINCT KEY._col1:0._col0)

          bucketGroup: false

          keys:

                expr: KEY._col0

                type: int

          mode: complete

          outputColumnNames: _col0, _col1, _col2

          Select Operator

            expressions:

                  expr: _col0

                  type: int

                  expr: _col1

                  type: bigint

                  expr: _col2

                  type: bigint

            outputColumnNames: _col0, _col1, _col2

            Select Operator

              expressions:

                    expr: _col0

                    type: int

                    expr: UDFToInteger(_col1)

                    type: int

                    expr: UDFToInteger(_col2)

                    type: int

              outputColumnNames: _col0, _col1, _col2

              File Output Operator

                compressed: false

                GlobalTableId: 1

                directory: hdfs://localhost:54310/tmp/hive-tianzhao/hive_2011-07-15_21-50-38_510_6852880850328147221/-ext-10000

                NumFilesPerFileSink: 1

                table:

                    input format: org.apache.hadoop.mapred.TextInputFormat

                    output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                    properties:

                      bucket_count -1

                      columns key,val1,val2

                      columns.types int:int:int

                      file.inputformat org.apache.hadoop.mapred.TextInputFormat

                      file.outputformat org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                      location hdfs://localhost:54310/user/hive/warehouse/dest1

                      name dest1

                      serialization.ddl struct dest1 { i32 key, i32 val1, i32 val2}

                      serialization.format 1

                      serialization.lib org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                      transient_lastDdlTime 1310523946

                    serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                    name: dest1

                TotalFiles: 1

                MultiFileSpray: false



  Stage: Stage-0

    Move Operator

      tables:

          replace: true

          source: hdfs://localhost:54310/tmp/hive-tianzhao/hive_2011-07-15_21-50-38_510_6852880850328147221/-ext-10000

          table:

              input format: org.apache.hadoop.mapred.TextInputFormat

              output format: org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

              properties:

                bucket_count -1

                columns key,val1,val2

                columns.types int:int:int

                file.inputformat org.apache.hadoop.mapred.TextInputFormat

                file.outputformat org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat

                location hdfs://localhost:54310/user/hive/warehouse/dest1

                name dest1

                serialization.ddl struct dest1 { i32 key, i32 val1, i32 val2}

                serialization.format 1

                serialization.lib org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

                transient_lastDdlTime 1310523946

              serde: org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe

              name: dest1

          tmp directory: hdfs://localhost:54310/tmp/hive-tianzhao/hive_2011-07-15_21-50-38_510_6852880850328147221/-ext-10001







ABSTRACT SYNTAX TREE:

(TOK_QUERY 

	(TOK_FROM (TOK_TABREF INPUT)) 

	(TOK_INSERT 

		(TOK_DESTINATION (TOK_TAB dest1)) 

		(TOK_SELECT 

			(TOK_SELEXPR (. (TOK_TABLE_OR_COL INPUT) key)) 

			(TOK_SELEXPR (TOK_FUNCTION count (TOK_FUNCTION substr (. (TOK_TABLE_OR_COL INPUT) value) 5))) 

			(TOK_SELEXPR (TOK_FUNCTIONDI count (TOK_FUNCTION substr (. (TOK_TABLE_OR_COL INPUT) value) 5)))

		)

		(TOK_GROUPBY (. (TOK_TABLE_OR_COL INPUT) key))

	)

)







SemanticAnalyzer.genBodyPlan(QB qb, Operator input){

       if (qbp.getAggregationExprsForClause(dest).size() != 0

            || getGroupByForClause(qbp, dest).size() > 0) { //如果有聚合函数或者有groupby，则执行下面的操作

          //multiple distincts is not supported with skew in data

          if (conf.getVar(HiveConf.ConfVars.HIVEGROUPBYSKEW)

              .equalsIgnoreCase("true") &&

             qbp.getDistinctFuncExprsForClause(dest).size() > 1) {

            throw new SemanticException(ErrorMsg.UNSUPPORTED_MULTIPLE_DISTINCTS.

                getMsg());

          }

          // insert a select operator here used by the ColumnPruner to reduce

          // the data to shuffle

          curr = insertSelectAllPlanForGroupBy(dest, curr); //生成一个SelectOperator，所有的字段都会选取，selectStar=true。

          if (conf.getVar(HiveConf.ConfVars.HIVEMAPSIDEAGGREGATE)

              .equalsIgnoreCase("true")) {

            if (conf.getVar(HiveConf.ConfVars.HIVEGROUPBYSKEW)

                .equalsIgnoreCase("false")) {

              curr = genGroupByPlanMapAggr1MR(dest, qb, curr);

            } else {

              curr = genGroupByPlanMapAggr2MR(dest, qb, curr);

            }

          } else if (conf.getVar(HiveConf.ConfVars.HIVEGROUPBYSKEW)

              .equalsIgnoreCase("true")) {

            curr = genGroupByPlan2MR(dest, qb, curr);

          } else {

            curr = genGroupByPlan1MR(dest, qb, curr);

          }

       }   

}



distince：

count.q.out

groupby11.q.out

groupby10.q.out

nullgroup4_multi_distinct.q.out

join18.q.out

groupby_bigdata.q.out

join18_multi_distinct.q.out

nullgroup4.q.out

auto_join18_multi_distinct.q.out

auto_join18.q.out



（1）map端部分聚合，数据无倾斜，一个MR生成。

genGroupByPlanMapAggr1MR，生成三个Operator：

（1.1）GroupByOperator：map-side partial aggregation，由genGroupByPlanMapGroupByOperator方法生成：

       处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

       处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入groupByKeys和outputColumnNames

       处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，生成column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.HASH

  outputColumnNames：groupby+Distinct+Aggregation

  keys：groupby+Distinct

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  



（1.2）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby+distinct

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（1.3）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.MERGEPARTIAL

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  



（2）map端部分聚合，数据倾斜，两个MR生成。

genGroupByPlanMapAggr2MR：

（2.1）GroupByOperator：map-side partial aggregation，由genGroupByPlanMapGroupByOperator方法生成：

       处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

       处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入groupByKeys和outputColumnNames

       处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，生成column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.HASH

  outputColumnNames：groupby+Distinct+Aggregation

  keys：groupby+Distinct

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  



（2.2）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby+distinct

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（2.3）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，生成column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.PARTIALS

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  



（2.4）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（2.5）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，需要做聚合的column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.FINAL

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  





（3）map端不部分聚合，数据倾斜，两个MR生成。

genGroupByPlan2MR：



（3.1）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby+distinct

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（3.2）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，生成column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.PARTIAL1

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  



（3.3）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（3.4）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入groupByKeys和outputColumnNames

      处理聚合函数，getAggregationExprsForClause，生成AggregationDesc加入aggregations，需要做聚合的column加入outputColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.FINAL

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  





（4）map端不部分聚合，数据无倾斜，一个MR生成。

genGroupByPlan1MR：

（4.1）ReduceSinkOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理select中的Distinct，getDistinctFuncExprsForClause，Distinct的column，加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

 public ReduceSinkDesc(java.util.ArrayList<ExprNodeDesc> keyCols,

      int numDistributionKeys,

      java.util.ArrayList<ExprNodeDesc> valueCols,

      java.util.ArrayList<java.lang.String> outputKeyColumnNames,

      List<List<Integer>> distinctColumnIndices,

      java.util.ArrayList<java.lang.String> outputValueColumnNames, int tag,

      java.util.ArrayList<ExprNodeDesc> partitionCols, int numReducers,

      final TableDesc keySerializeInfo, final TableDesc valueSerializeInfo) {

    this.keyCols = keyCols; // 为reduceKeys，groupby+distinct

    this.numDistributionKeys = numDistributionKeys; // grpByExprs.size()

    this.valueCols = valueCols; //reduceValues，聚合函数

    this.outputKeyColumnNames = outputKeyColumnNames; //outputKeyColumnNames

    this.outputValueColumnNames = outputValueColumnNames; //outputValueColumnNames

    this.tag = tag; // -1

    this.numReducers = numReducers; // 一般都是-1

    this.partitionCols = partitionCols; // groupby

    this.keySerializeInfo = keySerializeInfo;

    this.valueSerializeInfo = valueSerializeInfo;

    this.distinctColumnIndices = distinctColumnIndices;

  }



（4.2）GroupByOperator

      处理groupby子句，getGroupByForClause，groupby的column加入reduceKeys和outputKeyColumnNames

      处理聚合函数，getAggregationExprsForClause，需要做聚合的column加入reduceValues和outputValueColumnNames

  public GroupByDesc(

      final Mode mode,

      final java.util.ArrayList<java.lang.String> outputColumnNames,

      final java.util.ArrayList<ExprNodeDesc> keys,

      final java.util.ArrayList<org.apache.hadoop.hive.ql.plan.AggregationDesc> aggregators,

      final boolean groupKeyNotReductionKey,float groupByMemoryUsage, float memoryThreshold) {

    this(mode, outputColumnNames, keys, aggregators, groupKeyNotReductionKey,

        false, groupByMemoryUsage, memoryThreshold);

  }

  mode：GroupByDesc.Mode.COMPLETE

  outputColumnNames：groupby+Aggregation

  keys：groupby

  aggregators：Aggregation

  groupKeyNotReductionKey：false

  groupByMemoryUsage：默认为0.5

  memoryThreshold：默认为0.9  







SemanticAnalyzer.genBodyPlan

  optimizeMultiGroupBy  (multi-group by with the same distinct)

  groupby10.q  groupby11.q
```





