# Flink 流转换 - sxf_123456的博客 - CSDN博客
2018年02月06日 11:36:34[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：234
Stream Transformations
1.Map:
DataSteam -> DataSteam
dataSteam.map {x => x * 2}
2.FlatMap
DataStream -> DataStream
dataStream.flatMap {str => str.split(" ")}
3.Filter
DataStream -> DataStream
dataStream.filter { _ != 0 }
4.KeyBy
DataStream -> KeyedStream
dataStream.keyBy("someKey")
dataStream.keyBy("0")
5.Reduce
KeyedStream -> DataStream
keyedStream.reduce { _ + _ }
6.Fold
keyedStream -> DataSyteam
val result: DataStream[String] = keyedStream.fold("start")((str, i)) => { str + "-" + i })
7.Aggregation
KeyedStream -> DataStream
keyedStream.sum(0)
KeyedStream.sum("key")
keyedStream.min(0)
keyedStream.min("key")
keyedStream.max(0)
keyedStream.max("key")
keyedStream.minBy(0)
keyedStream.minBy("key")
keyedStream.maxBy(0)
keyedStream.maxBy("key")
8.Window
keyedSteam -> windowedStream
dataStream.keyBy(0).window(TumblingEventTimeWindow.of(Time.second(5)))
9.WindowALl
DataStream -> AllWindowedStream
dataSteam.windowAll(TumblingEventTimeWindows.of(Time.seconds(5)))
10.WindowApply
WindowStream -> DataSteam
AllWindowStream -> DataStream
windowedStream.apply { WindowFunction }
allWindowSteam.apply { AllWindowFunction }
11.Window Reduce
WindowedStream -> DataStream
windowedStream.reduce { _ + _ }
12.Window Fold
WindowedStream -> DataStream
val result: DataStream[String] = windowedStream.fold("start", (str, 1) => { str + "_" + i })
13.Aggregation on windows
WindowedStream -> DataStream
windowedStream.sum(0)
windowedStream.sum("key")
windowedStream.min(0)
windowedStream.min("key")
windowedStream.max(0)
windowedStream.max("key")
windowedStream.minBy(0)
windowedStream.minBy("key")
windowedStream.maxBy(0)
windowedStream.maxBy("key")
14.Union
DataStream* -> DataStream
dataStream.union(otherStream1, otherStream2, ...)
15.Window Join
DataStream, DataStream -> DataStream
dataStream.join(otherStream).where(<key selector>).equalTo(<key selector>).window(TumblingEventWindows.of(Time.second(3))).apply {...}
16.Window CoGroup
DataStrea, DataStream -> DataStream
dataStream.coGroup(otherStream).where(0).equalTo(1).window(TumblingEventWindows.of(Time.seconds(3))).apply {}
17.connect
DataStream, DataStream -> ConnectedStream
someStream: DataStream[Int] = //
otherStream: DataStream[String] = //
val connectedStreams = someStream.connect(otherStream)
18.CoMap, CoFlatMap
ConnectedStream -> DataStream
connectedStream.map( (_:Int) => true, (_:String) => false)
connectedStreams.flatMap( (_:Int) => true, (_:String) => false)
19.Split
DataStream -> SplitStream
val split = someDataStream.split((num: Int) => (num % 2) match {case 0 => List("even") case 1 => List("odd") })
20.Select
SplitStream -> DataStream
val even = split select "even"
val odd = split select "odd"
val all = split select("even", "odd")
21.Iterate
DataStream -> iterateStream -> DataStream
initialStream.iterate {
    iteration => {
        val iterationBody = iteration.map {  }
        (iterationBody.filter(_ > 0), iterationBody.filter( _ <= 0))
    }
}
22.Extract Timestamps
DataStream -> DataStream
stream.assignTimestamps { timestampExtractor }
