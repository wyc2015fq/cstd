# hadoop工作流程分析 - 三少GG - CSDN博客
2013年09月08日 23:19:12[三少GG](https://me.csdn.net/scut1135)阅读数：1145

到目前为止，我们针对wordcount例子，介绍了一个Job从创建，到设置参数，到执行的整个过程。但是hadoop的执行Job的时，内部又是怎么样一个流程呢？
1. Inputformat会从job的INPUT_DIR目录下读入待处理的文件，检查输入的有效性并将文件切分成InputSplit列表。Job实例可以通过setInputFormatClass(Class<? extends InputFormat>)函数来设置所需的inputformat。
2. 当Inputformat对输入文件分片后，会对每个分片构建一个MapperTask实例（MapTask(String, TaskAttemptID, int, TaskSplitIndex, int)）。其实整个Mapper类的调度过程，都是由MapperTask来实现的。MapperTask的run(JobConf, TaskUmbilicalProtocol)方法实现了对于Mapper task调度的整个过程。
2.1  RecordReader会解析InputSplit，生成对应的key/value pair。Inputformat中有一个除了用于分片的getSplits(JobContext)方法外，还有一个方法createRecordReader(InputSplit, TaskAttemptContext)，该方法用于给每一个分片创建一个RecordReader。重写这个方法，可以添加自己的RecordReader。
2.2 Mapper类会对属于一个InputSplit所有key/value pair调用一次map函数。关于Mapper类的作用，在Java doc中描述如下：“Mapper maps input key/value pairs to a set of intermediate key/value pairs”。 Job实例可以通过setMapperClass(Class<? extends Mapper>)函数来设置自己的Mapper类。
2.3 可以通过Job实例的setSortComparatorClass(Class<? extends RawComparator>)方法来为Mapper设定一个Comparator class，用来对Mapper的结果根据key进行排序。
2.4 可以通过Job实例的setPartitionerClass(Class<? extends Partitioner>)方法来为Mapper设定一个Partitioner Class，用来对Mapper的结果根据Reducer进行分片。
2.5 可以通过Job实例的setCombinerClass(Class<? extends Reducer>)方法为Mapper设定一个Combiner Class，用来在本地进行聚集操作，从而减少从Mapper到Reducer的数据传输量。
3. 在Mapper执行结束之后，ReducerTask类会被用来进行整个Reducer操作的调度
3.1 Shuffle类会被调用从而来获取在Mapper输出中属于本Reducer的分片，并将多个分片combine成一个。
3.2 Shuffle类会使用MergeManager根据Job实例的setSortComparatorClass(Class<? extends RawComparator>)所设定的Comparator class对key/value pair进行排序
3.3 在shuffle操作执行结束之后，如果对于Reducer的input数据，有使用特殊分组的需求的话，可以通过Job实例的setGroupingComparatorClass(Class<? extends RawComparator>)方法来实现定制的分组策略，否则，则使用setSortComparatorClass(Class<? extends RawComparator>)的比较方式。
3.4 在分组后的结果中，针对每一个<key, (list of values)> pair 调用Reduce的reduce(K2, Iterator<V2>, OutputCollector<K3, V3>, Reporter)方法。可以通过Job实例的setReducerClass(Class<? extends Reducer>)方法类设置相应的Reduce实现。
4.  Reduce 的结果将由OutputCollector.collect(WritableComparable, Writable)写入文件系统
