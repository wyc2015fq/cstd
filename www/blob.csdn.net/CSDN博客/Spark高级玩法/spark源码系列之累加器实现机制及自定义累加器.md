# spark源码系列之累加器实现机制及自定义累加器 - Spark高级玩法 - CSDN博客
2018年06月04日 08:34:41[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：971
**一，基本概念**
累加器是Spark的一种变量，顾名思义该变量只能增加。有以下特点：
1，累加器只能在Driver端构建及并只能是Driver读取结果，Task只能累加。
2，累加器不会改变Spark Lazy计算的特点。只会在Job触发的时候进行相关累加操作。
3，现有累加器的类型。
**二，累加器的使用**
Driver端初始化，并在Action之后获取值。
**val**accum = sc.accumulator(0, "test Accumulator")
accum.value
Executor端进行计算
accum+=1;
**三，累加器的重点类**
Class Accumulator extends Accumulable
主要是实现了累加器的初始化及封装了相关的累加器操作方法。同时在类对象构建的时候向我们的Accumulators注册了累加器。累加器的add操作的返回值类型和我们传入的值类型可以不一样。所以，我们一定要定义好如何累加和合并值。也即add方法
object Accumulators:
该方法在Driver端管理着我们的累加器，也包含了特定累加器的聚合操作。
trait AccumulatorParam[T] extends AccumulableParam[T, T]:
AccumulatorParam的addAccumulator操作的泛型封装，具体的实现还是要再具体实现类里面实现addInPlace方法。
object AccumulatorParam:
主要是进行隐式类型转换的操作。
TaskContextImpl:
在Executor端管理着我们的累加器。
**四，累加器的源码解析**
1,Driver端的初始化
**val **accum = sc.accumulator(0, "test Accumulator")
**val **acc = **new **Accumulator(initialValue, param, *Some*(name))
主要是在Accumulable(Accumulator)中调用了，这样我们就可以使用Accumulator使用了。
Accumulators.*register*(**this**)
2,Executor端的反序列化得到我们对象的过程
首先，我们的value_ 可以看到其并不支持序列化
@volatile @transient **private var ***value_ *: R = initialValue // Current value on master
其初始化是在我们反序列化的时候做的，反序列化还完成了Accumulator向我们的TaskContextImpl的注册
反序列化是在调用ResultTask的RunTask方法的时候做的
**val **(rdd, func) = ser.deserialize[(RDD[T], (TaskContext, Iterator[T]) => U)](
  ByteBuffer.*wrap*(taskBinary.value), Thread.*currentThread*.getContextClassLoader)
过程中会调用
**private def **readObject(in: ObjectInputStream): Unit = Utils.*tryOrIOException *{
  in.defaultReadObject()
*value_ *= *zero**  deserialized *= **true**// Automatically register the accumulator when it is deserialized with the task closure.
  //
  // Note internal accumulators sent with task are deserialized before the TaskContext is created
  // and are registered in the TaskContext constructor. Other internal accumulators, such SQL
  // metrics, still need to register here.
**val **taskContext = TaskContext.*get*()
**if **(taskContext != **null**) {
    taskContext.registerAccumulator(**this**)
  }
}
3,累加器的累加
accum+=1;
param.addAccumulator(*value_*, term)
根据不同的累加器参数有不同的实现AccumulableParam
如，int类型。最终调用的AccumulatorParam特质的addAccumulator方法。
**trait **AccumulatorParam[T] **extends **AccumulableParam[T, T] {
**def **addAccumulator(t1: T, t2: T): T = {
    addInPlace(t1, t2)
  }
}
然后，调用的是各个具体实现的addInPlace方法
**implicit object **IntAccumulatorParam **extends **AccumulatorParam[Int] {
**def **addInPlace(t1: Int, t2: Int): Int = t1 + t2
**def **zero(initialValue: Int): Int = 0
}
返回后更新了我们的Accumulators的value_的值。
4,Accumulator的各个节点累加的之后的聚合操作
在Task类的run方法里面得到并返回的
(runTask(*context*), *context*.collectAccumulators())
最终在DAGScheduler里面调用了updateAccumulators(event)
在updateAccumulators方法中
Accumulators.*add*(event.accumUpdates)
具体内容如下:
**def **add(values: Map[Long, Any]): Unit = synchronized {
**for **((id, value) <- values) {
**if **(*originals*.contains(id)) {
// Since we are now storing weak references, we must check whether the underlying data
      // is valid.
*originals*(id).get **match **{
**case ***Some*(accum) => accum.asInstanceOf[Accumulable[Any, Any]] ++= value
**case **None =>
**throw new **IllegalAccessError("Attempted to access garbage collected Accumulator.")
      }
    } **else **{
      logWarning(s"Ignoring accumulator update for unknown accumulator id **$**id")
    }
  }
}
5,最后我们就可以获取到累加器的值了
accum.value
**五，累加器使用注意事项**
累加器不会改变我们RDD的Lazy的特性，之后再Action之后完成计算和更新。
但是假如出现两个Action公用一个转化操作，如map，在map里面进行累加器累加，那么每次action都会累加，造成某些我们不需要的结果。
**六，自定义累加器**
自定义累加器输出
七，总结
    主要牵涉点就是序列化及类加载执行，这是深入玩spark的必须，欢迎大家公众号打赏领取相关课程。
更多源码教程，请加知识星球获取。
