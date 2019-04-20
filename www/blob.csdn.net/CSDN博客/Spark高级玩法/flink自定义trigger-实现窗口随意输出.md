# flink自定义trigger-实现窗口随意输出 - Spark高级玩法 - CSDN博客
2019年02月26日 20:45:17[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：174
前面，一篇简单讲了flink的窗口及与Spark Streaming窗口之间的对比。
对于flink的窗口操作，尤其是基于事件时间的窗口操作，大家还要掌三个个重要的知识点：
- 
窗口分配器：就是决定着流入flink的数据，该属于哪个窗口。
- 
时间戳抽取器/watermark生成器：抽取时间戳并驱动着程序正常执行。
- 
trigger：决定着数据啥时候落地。
flink有很多内置的触发器，对于基于事件时间的窗口触发器叫做
EventTimeTrigger。其实，我们要实现基于事件时间的窗口随意输出，比如1000个元素触发一次输出，那么我们就可以通过修改这个触发器来实现。
那么可能你没留意前面说的，为啥需要trigger，因为没有trigger的话，存在允许事件滞后的时候，输出时间延迟比较大，而我们需要尽早看到数据，那么这个时候就可以自己定义窗口触发。
自定义触发器
修改自基于处理时间的触发器，源码如下：
package org.trigger;
import org.apache.flink.streaming.api.windowing.triggers.Trigger;
import org.apache.flink.streaming.api.windowing.triggers.TriggerResult;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
publicclassCustomProcessingTimeTriggerextendsTrigger<Object, TimeWindow> {
privatestaticfinallong serialVersionUID = 1L;
privateCustomProcessingTimeTrigger(){}
privatestaticint flag = 0;
@Override
public TriggerResult onElement(Object element, long timestamp, TimeWindow window, TriggerContext ctx){
        ctx.registerProcessingTimeTimer(window.maxTimestamp());
// CONTINUE是代表不做输出，也即是，此时我们想要实现比如100条输出一次，
// 而不是窗口结束再输出就可以在这里实现。
if(flag > 9){
            flag = 0;
return TriggerResult.FIRE;
        }else{
            flag++;
        }
        System.out.println("onElement : "+element);
return TriggerResult.CONTINUE;
    }
@Override
public TriggerResult onEventTime(long time, TimeWindow window, TriggerContext ctx)throws Exception {
return TriggerResult.CONTINUE;
    }
@Override
public TriggerResult onProcessingTime(long time, TimeWindow window, TriggerContext ctx){
return TriggerResult.FIRE;
    }
@Override
publicvoidclear(TimeWindow window, TriggerContext ctx)throws Exception {
        ctx.deleteProcessingTimeTimer(window.maxTimestamp());
    }
@Override
publicbooleancanMerge(){
returntrue;
    }
@Override
publicvoidonMerge(TimeWindow window,
                        OnMergeContext ctx){
// only register a timer if the time is not yet past the end of the merged window
// this is in line with the logic in onElement(). If the time is past the end of
// the window onElement() will fire and setting a timer here would fire the window twice.
long windowMaxTimestamp = window.maxTimestamp();
if (windowMaxTimestamp > ctx.getCurrentProcessingTime()) {
            ctx.registerProcessingTimeTimer(windowMaxTimestamp);
        }
    }
@Override
public String toString(){
return"ProcessingTimeTrigger()";
    }
/**
     * Creates a new trigger that fires once system time passes the end of the window.
     */
publicstatic CustomProcessingTimeTrigger create(){
returnnew CustomProcessingTimeTrigger();
    }
}
主要实现逻辑是在onElement函数，实现的逻辑是增加了每10个元素触发一次计算结果输出的逻辑。
主函数
package org.trigger;
import org.apache.flink.api.common.functions.RichMapFunction;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.configuration.Configuration;
import org.apache.flink.streaming.api.TimeCharacteristic;
import org.apache.flink.streaming.api.datastream.AllWindowedStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.windowing.windows.TimeWindow;
import org.apache.flink.streaming.connectors.kafka.FlinkKafkaConsumer010;
import java.util.Properties;
/*
  trigger 测试
  滚动窗口，20s
  然后是trigger内部技术，10个元素输出一次。
 */
publicclasskafkaSourceTriggerTest{
publicstaticvoidmain(String[] args)throws Exception {
// set up the streaming execution environment
final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
    env.setStreamTimeCharacteristic(TimeCharacteristic.ProcessingTime);
    Properties properties = new Properties();
    properties.setProperty("bootstrap.servers", "localhost:9093");
    properties.setProperty("group.id", "test");
    FlinkKafkaConsumer010<String> kafkaConsumer010 = new FlinkKafkaConsumer010<>("test",
new SimpleStringSchema(),
        properties);
    AllWindowedStream<Integer, TimeWindow> stream = env
        .addSource(kafkaConsumer010)
        .map(new String2Integer())
        .timeWindowAll(org.apache.flink.streaming.api.windowing.time.Time.seconds(20))
        .trigger(CustomProcessingTimeTrigger.create());
    stream.sum(0)
        .print()
        ;
    env.execute("Flink Streaming Java API Skeleton");
  }
privatestaticclassString2IntegerextendsRichMapFunction<String, Integer> {
privatestaticfinallong serialVersionUID = 1180234853172462378L;
@Override
public Integer map(String event)throws Exception {
return Integer.valueOf(event);
    }
@Override
publicvoidopen(Configuration parameters)throws Exception {
    }
  }
}
代码测试，通过的哦。
**明天知识星球分享，一篇干活和代码案例～**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVXx5SiaxBBEaxyqwGibKh1FVdV1Y4XdZA9icibRPm7lgTjRNPntdG6ic850aBTeBVicGmX945gH28ibW7uw/640?wx_fmt=png)
原文阅读：
[你不了解的flink特性-trigger](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486467&idx=1&sn=7cb8efb98fd368eda18921f9eed560fa&chksm=9f38ef2ba84f663d62eef9b477c4d2e8d4cefc0d5e139cb6ddfd7b25cc246dae61b7050a82a5&scene=21#wechat_redirect)
[浪尖原创flink视频第一弹](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486413&idx=1&sn=627b72d1e65869b9b30ba1ed0c21b780&chksm=9f38e8e5a84f61f36e809c2517192967f2f9c02c62b7d0d3607c59bdefc36ade612ac08df8ae&scene=21#wechat_redirect)
[一文精通flinkOnYarn](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486437&idx=1&sn=b261d8c4d46e78a7a6a46d4386a9d0a3&chksm=9f38e8cda84f61dbefd6a92f5c7e36d6178ff1a0ae76e36a97155fbf3e04a87efa07ea0592a2&scene=21#wechat_redirect)
欢迎2019年与近580好友一起学习～
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
