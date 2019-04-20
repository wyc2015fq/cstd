# flink一次对整个窗口进行聚合操作-ProcessWindowFunction - Spark高级玩法 - CSDN博客
2019年03月05日 21:09:14[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：306
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVJcjrBibyo2Jw7XGtGhIRstgIj92HW2kdyxWxq3WhGqZIv9TKfrmaUQeVhhyiaDdZGtCmia83psK43Q/640?wx_fmt=png)
前面说了很多关于flink的windows相关原理的内容，今天讲一个flink的window相关操作的一个实战内容吧。
首先，回顾一下Spark Streaming的windows操作，实际上就是在将微批增加若干倍（窗口大小处以批处理大小），这样就形成了窗口，那对于与kafka的结合这种方式，原理我在星球的源码里也说过了，实际上并没有真实的去kafka取数据，而是计算了offset，这种情况下，实际上窗口计算的时候并没有一批次缓存全部数据，当然基于receiver那种就不行了因为是blockrdd，receiver直接接受了数据。
而对于flink 与kafka的区别也说过，flink是数据在拓扑图里流动，那么窗口操作也是增量的计算。实际上有时候我们可能想要针对一整个窗口计算，然后灵活的控制时间及窗口状态，flink神奇就神奇在可以很容易实现我们的操作。
神奇的ProcessWindowFunction。
ProcessWindowFunction一次性迭代整个窗口里的所有元素，比较重要的一个对象是Context，可以获取到事件和状态信息，这样我们就可以实现更加灵活的控制，这实际上是process的主要特点吧。该算子会浪费很多性能吧，主要原因是不增量计算，要缓存整个窗口然后再去处理，所以要设计好内存。
牛叉的地方是ProcessWindowFunction可以结合 ReduceFunction,  AggregateFunction, 或者 FoldFunction来做增量计算。
本文就举一个简单的例子，主要是实现针对窗口的分组统计功能。
```
package org.datastream.windows;
import org.apache.flink.api.common.functions.FoldFunction;
import org.apache.flink.api.common.restartstrategy.RestartStrategies;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.api.java.tuple.Tuple2;
import org.apache.flink.streaming.api.CheckpointingMode;
import org.apache.flink.streaming.api.TimeCharacteristic;
import org.apache.flink.streaming.api.datastream.SingleOutputStreamOperator;
import org.apache.flink.streaming.api.environment.CheckpointConfig;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.functions.windowing.ProcessWindowFunction;
import org.apache.flink.streaming.api.windowing.assigners.SlidingProcessingTimeWindows;
import org.apache.flink.streaming.api.windowing.time.Time;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.streaming.connectors.kafka.FlinkKafkaConsumer010;
import org.apache.flink.util.Collector;
import org.datastream.trigger.CustomProcessingTimeTrigger;
import org.datastream.util.Split2KV;
import java.util.Properties;
public class TumblingWindowsProcessFunction {
public static void main(String[] args) throws Exception {
// set up the streaming execution environment
        final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
// 设置最少一次处理语义和恰一次处理语义
        env.enableCheckpointing(20000,CheckpointingMode.EXACTLY_ONCE);
//    checkpoint 也可以分开设置
        env.enableCheckpointing(20000);
        env.getCheckpointConfig() // checkpoint的清楚策略
                .enableExternalizedCheckpoints(CheckpointConfig.
                        ExternalizedCheckpointCleanup.RETAIN_ON_CANCELLATION);
//    设置重启策略
        env.setRestartStrategy(RestartStrategies.
                fixedDelayRestart(5,//5次尝试
50000)); //每次尝试间隔50s
// 选择设置事件事件和处理事件
        env.setStreamTimeCharacteristic(TimeCharacteristic.ProcessingTime);
        Properties properties = new Properties();
        properties.setProperty("bootstrap.servers", "mt-mdh.local:9093");
        properties.setProperty("group.id", "TumblingWindowsProcessFunction");
        FlinkKafkaConsumer010<String> kafkaConsumer010 = new FlinkKafkaConsumer010<>("KV",
new SimpleStringSchema(),
                properties);
        kafkaConsumer010.setStartFromLatest();
        SingleOutputStreamOperator<String> process = env
                .addSource(kafkaConsumer010)
                .map(new Split2KV())
                .keyBy(value -> value.f0) //注意我这里的写法
                .timeWindow(Time.minutes(5)) //滚动窗口
                .trigger(CustomProcessingTimeTrigger.create())
                .process(new MyProcessWindowFunction());
        process.print();
        env.execute("Flink Streaming Java API Skeleton");
    }
/*
        ProcessWindowFunction一次性迭代整个窗口里的所有元素，比较重要的一个对象是Context，可以获取到事件和状态信息
        ，这样我们就可以实现更加灵活的控制，这实际上是process的主要特点吧。
        该算子会浪费很多性能吧，主要原因是不是增量计算，要缓存整个窗口然后再去处理，所以要设计好内存占比。
        当然了rocessWindowFunction可以结合 ReduceFunction, an AggregateFunction, or a FoldFunction来做增量计算。
     */
private static  class MyProcessWindowFunction
extends ProcessWindowFunction<Tuple2<String, Long>, String, String, TimeWindow> {
@Override
public void process(String key, Context context, Iterable<Tuple2<String, Long>> input, Collector<String> out) {
            long count = 0;
for (Tuple2<String, Long> in: input) {
                count++;
            }
            out.collect("Window: " + context.window() + "count: " + count);
        }
    }
}
```
推荐阅读
[漫谈数据质量监控](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486497&idx=1&sn=b5a6d160f5ae64f50a9618ff595bc33c&chksm=9f38ef09a84f661f842d492ffdef5dca01b7bb7222f6d6ab18c8b642889f9cd9cf569953838b&scene=21#wechat_redirect)
[blink解决的一个flink分析痛点](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486489&idx=1&sn=07b99aa88f75c3418187a590276d81f9&chksm=9f38ef31a84f662712c16ceab8ace21118543487ff72121ed54347b25bb7da83611043e2b1fe&scene=21#wechat_redirect)
[flink自定义trigger-实现窗口随意输出](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486478&idx=1&sn=00281d77b2caf7e802dc435d4b58b5f6&chksm=9f38ef26a84f663085c8fddda2d2eaf485fae2ad3f699ae8c263c9c7d0dcddc20dae8e2e4f86&scene=21#wechat_redirect)
更多复杂的flink计算，请加入浪尖知识星球与近600位好友一起学习～
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
