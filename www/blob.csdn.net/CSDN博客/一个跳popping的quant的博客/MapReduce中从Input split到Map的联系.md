# MapReduce中从Input split到Map的联系 - 一个跳popping的quant的博客 - CSDN博客





2017年08月18日 22:23:16[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：568








上一篇文章搞明白了文件从上传到hdfs，再到通过实现了InputFormat的FileInputFormat类来将文件进行输入分片的划分，而最后计算好了的分片是怎么传递给了map呢？ 

  还是通过源码来解决。在InputFormat中还有一个方法叫做createRecordReader()

```java
/**
   * Create a record reader for a given split. The framework will call
   * {@link RecordReader#initialize(InputSplit, TaskAttemptContext)} before
   * the split is used.
   * @param split the split to be read
   * @param context the information about the task
   * @return a new record reader
   * @throws IOException
   * @throws InterruptedException
   */
  public abstract 
    RecordReader<K,V> createRecordReader(InputSplit split,
                                         TaskAttemptContext context
                                        ) throws IOException, 
                                                 InterruptedException;

}
```

这个方法的返回值是一个RecordReader对象，而RecordReader是用来从一个输入分片中读取一个一个的key-value对的抽象类，可以把它看作是在InputSplit上的迭代器，这个接口中有以下方法：

```java
public abstract   boolean nextKeyValue() throws IOException, InterruptedException;
  public abstract  KEYIN getCurrentKey() throws IOException, InterruptedException;
  public abstract   VALUEIN getCurrentValue() throws IOException, InterruptedException;
  public abstract float getProgress() throws IOException, InterruptedException;
  public abstract void close() throws IOException;
```

其中最主要的方法就是nextKeyValue()的方法，它来从分片上获取下一个key-value对。 

  这样，文件已经分片了，并且文件分片的内容也存放到了RecordReader中，后面也就是Map如何来使用RecordReader了。

在Mapper这个类的源码中有一个内部类和其他几个方法：

```
public abstract class Context
    implements MapContext<KEYIN,VALUEIN,KEYOUT,VALUEOUT> {
  }
  protected void setup(Context context) throws IOException, InterruptedException
  protected void map(KEYIN key, VALUEIN value, Context context) throws IOException,InterruptedException {  }
  protected void cleanup(Context context  ) throws IOException, InterruptedException {  }
  public void run(Context context) throws IOException, InterruptedException {  }
```

在写Mapper的时候，都要继承Mapper.class这个类，并且重写map这个方法，map()这个方法每次接受一个K-V对，然后对这一对K-V进行处理，再将数据分发出去。setup()的作用就是对这个map task进行一个预处理，cleanup()的作用做一些map task执行完之后的操作。举个例子就是：在处理一些数据的时候，可以在setup()中初始化一些属性或者集合，然后在map()中处理每一个key-value对，放入集合中，最后在cleanup()中在集合中取出key-value或者进行一些筛选，再通过context.write(key,value)写出去。 

  最后看下Mapper中的run()方法：

```java
public void run(Context context) throws IOException, InterruptedException {
    setup(context);
    try {
      while (context.nextKeyValue()) {
        map(context.getCurrentKey(), context.getCurrentValue(), context);
      }
    } finally {
      cleanup(context);
    }
  }
```

这个run()方法相当于map task的驱动方法，首先是调用setup()方法进行初始化操作，然后通过context.nextKeyValue()获取key-value对，并把它们交给了map()函数进行处理，最后调用了cleanup()方法做最后的处理。其中context在构造的时候就传递了RecordReader对象，所以context.nextKeyValue()实际上就是调用了RecordReader来获取。 

  ps：除RecordReader对象作为参数传递到了context中，还有Configuration对象也作为了参数传递进来，所以之前在代码中写到的对job的一些配置，例如`job.setInputFormatClass(TextInputFormat.class);//输入分片中按行进行拆分,不用写,默认就是TextInputFormat`

  再看下这个Mapper类中的Context类，它继承了MapContext，并且使用了一个RecordReader进行了构造：
```java
public MapContext(Configuration conf, TaskAttemptID taskid,
                    RecordReader<KEYIN,VALUEIN> reader,
                    RecordWriter<KEYOUT,VALUEOUT> writer,
                    OutputCommitter committer,
                    StatusReporter reporter,
                    InputSplit split) {
    super(conf, taskid, writer, committer, reporter);
    this.reader = reader;
    this.split = split;
  }
```

而这个承载着关键数据信息的RecordReader则是被mapreudce框架在执行时传递进来了。 

过程是： 

在 job提交 任务之后 首先由jobtrack 分发任务，在 任务分发完成之后，执行 task的时候，这时调用了 maptask 中的  runNewMapper在这个方法中调用了 MapContext，至此  这个map 和框架就可以联系起来了。 

借鉴于:[https://zhidao.baidu.com/question/647892511189387885.html](https://zhidao.baidu.com/question/647892511189387885.html)




