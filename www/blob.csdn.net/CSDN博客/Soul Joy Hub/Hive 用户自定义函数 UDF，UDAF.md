# Hive 用户自定义函数 UDF，UDAF - Soul Joy Hub - CSDN博客

2016年08月16日 11:01:29[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1008


[http://blog.csdn.net/u011239443/article/details/52189986](http://blog.csdn.net/u011239443/article/details/52189986)

> 
Hive有UDF：（普通）UDF，用户自定义聚合函数（UDAF）以及用户自定义生表函数（UDTF）。它们所接受的输入和生产的输出的数据行的数量的不同的。

# UDF

> 
UDF操作作用于单个数据行，且产生一个数据行作为输出。

## 例：strip

我们这里创造一个strip UDF

```java
package Hive_UDF;

import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hive.ql.exec.UDF;
import org.apache.hadoop.io.Text;

public class Strip extends UDF {
    private Text result = new Text();

    public Text evaluate(Text str) {
        if (str == null) {
            return null;
        }
        result.set(StringUtils.strip(str.toString()));
        return result;
    }

    public Text evaluate(Text str, String stripChars) {
        if (str == null) {
            return null;
        }
        result.set(StringUtils.strip(str.toString(), stripChars));
        return result;
    }
}
```

在hive shell中引入该jar包：
`add jar /home/Strip.jar;`
生成函数：
`create temporary function strip as 'Hive_UDF.Strip';`
使用函数1：
`select strip("   bee   ") from item;`
得到结果：

```
……
bee
bee
bee
Time taken: 0.768 seconds, Fetched: 26000 row(s)
```

使用函数2：
`select strip("banana","ab") from item;`
得到结果：

```matlab
……
nan
nan
nan
Time taken: 0.359 seconds, Fetched: 26000 row(s)
```

可以看到，得到的结果都是2600行。 

我们看下item表有多少行：

```
hive> select count(1) from item;
Query ID = root_20160816104545_9ef6e618-b004-470f-b5a7-15af22075407
Total jobs = 1
Launching Job 1 out of 1
Number of reduce tasks determined at compile time: 1
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1471250735061_0001, Tracking URL = http://holodesk01:8088/proxy/application_1471250735061_0001/
Kill Command = /usr/lib/hadoop/bin/hadoop job  -kill job_1471250735061_0001
Hadoop job information for Stage-1: number of mappers: 1; number of reducers: 1
2016-08-16 10:46:16,504 Stage-1 map = 0%,  reduce = 0%
2016-08-16 10:46:30,241 Stage-1 map = 100%,  reduce = 0%, Cumulative CPU 2.37 sec
2016-08-16 10:46:38,697 Stage-1 map = 100%,  reduce = 100%, Cumulative CPU 4.62 sec
MapReduce Total cumulative CPU time: 4 seconds 620 msec
Ended Job = job_1471250735061_0001
MapReduce Jobs Launched: 
Stage-Stage-1: Map: 1  Reduce: 1   Cumulative CPU: 4.62 sec   HDFS Read: 7326445 HDFS Write: 6 SUCCESS
Total MapReduce CPU Time Spent: 4 seconds 620 msec
OK
26000
Time taken: 53.591 seconds, Fetched: 1 row(s)
```

所以侧面验证了UDF的确是“作用于单个数据行，且产生一个数据行作为输出”

# UDAF

> 
UDAF 接受多个输入数据行，并产生一个输出数据行。 

  一个计算函数必须实现以下5个方法：

- init()：   该方法负责初始化计算函数并重设它的内部状态 。
- iterate()：   每次对一个新值进行聚合计算时会调用该方法。
- terminatePartial()：   Hive需要部分聚合结果时会调用该方法。
- merge()：   Hive需要将部分聚合结果和另外部分聚合结果合并时会调用该方法。
- terminate()：   需要最终结果时会调用该方法

## 例：求最大整数UDAF数据流

![这里写图片描述](https://img-blog.csdn.net/20160816172315881)

## 例：Mean

```java
package Hive_UDF;

import org.apache.hadoop.hive.ql.exec.UDAF;
import org.apache.hadoop.hive.ql.exec.UDAFEvaluator;
import org.apache.hadoop.io.FloatWritable;

@SuppressWarnings("deprecation")
public class Mean extends UDAF {
    public static class MeanFloatUDAFEvaluator implements UDAFEvaluator {
        public static class PartialResult {
            float sum;
            long count;
        }

        private PartialResult partialresult;

        @Override
        public void init() {
            // TODO Auto-generated method stub
            partialresult = null;
        }

        public boolean iterate(FloatWritable value) {
            if (value == null) {
                return true;
            }
            if (partialresult == null) {
                partialresult = new PartialResult();
            }

            partialresult.sum += value.get();
            ++partialresult.count;
            return true;
        }

        public PartialResult terminatePartial() {
            return partialresult;
        }

        public boolean merge(PartialResult other) {
            if (other == null) {
                return true;
            }
            if (partialresult == null) {
                partialresult = new PartialResult();
            }

            partialresult.sum += other.sum;
            partialresult.count += other.count;

            return true;
        }

        public FloatWritable terminate() {
            if (partialresult == null) {
                return null;
            }
            return new FloatWritable(partialresult.sum / partialresult.count);
        }
    }
}
```

使用函数：
`select mymean(i_current_price) from item;`
得到结果：

```
Query ID = root_20160816175757_e063c1f7-5817-406a-b448-3a291a14a4a7
Total jobs = 1
Launching Job 1 out of 1
Number of reduce tasks determined at compile time: 1
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1471250735061_0002, Tracking URL = http://holodesk01:8088/proxy/application_1471250735061_0002/
Kill Command = /usr/lib/hadoop/bin/hadoop job  -kill job_1471250735061_0002
Hadoop job information for Stage-1: number of mappers: 1; number of reducers: 1
2016-08-16 17:57:31,690 Stage-1 map = 0%,  reduce = 0%
2016-08-16 17:57:46,309 Stage-1 map = 100%,  reduce = 0%, Cumulative CPU 3.29 sec
2016-08-16 17:57:54,706 Stage-1 map = 100%,  reduce = 100%, Cumulative CPU 5.77 sec
MapReduce Total cumulative CPU time: 5 seconds 770 msec
Ended Job = job_1471250735061_0002
MapReduce Jobs Launched: 
Stage-Stage-1: Map: 1  Reduce: 1   Cumulative CPU: 5.77 sec   HDFS Read: 7326868 HDFS Write: 10 SUCCESS
Total MapReduce CPU Time Spent: 5 seconds 770 msec
OK
9.4731455
Time taken: 39.621 seconds, Fetched: 1 row(s)
```

![这里写图片描述](https://img-blog.csdn.net/20170514214238797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

