# HBase之HFile解析 - zkq_1986的博客 - CSDN博客





2017年03月09日 15:33:38[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：599








![](http://images.cnitblog.com/i/640863/201406/172356352233376.jpg)


                                             图1


    从图1可知，HFile主要分四部分:Scanned block section, Non-scanned block section, Load-on-open-section以及Trailer。
**Scanned block section**: 即存储数据block部分
**Non-scanned block section**:元数据block部分，主要存放meta信息，即BloomFilter信息。
**Load-on-open-section**:这部分数据在RegionServer启动时，实例化Region并创建HStore时会将所有StoreFile的Load-on-open-section加载进内存，主要存放了Root Data Index,meta Index,File Info及BooleamFilter的metadata等。除了Fields for midkey外，每部分都是一个HFileBlock.下面会详细去讲这块。
**Trailer**:文件尾,主要记录version版本，不同的版本Trailer的字段不一样，及Trailer的字段相关信息。


下面详细讲述各部分。

**Trailer**:


    文件最后4位，即一个整型数字，为version信息，我们知道是V2.而V2的Trailer长度为212字节。除去MagicCode(BlockType) 8字节及 Version 4字节外，剩余206字节记录了整个文件的一些重要的字段信息，而这些字段信息是由protobuf组成的。

其格式如下：

![](http://images.cnitblog.com/i/640863/201406/180000583329455.jpg)


至此，Trailer已经完全解析完成，接下来开始下一部分。

**Load-on-open-section:**


    RegionServer托管着0...n个Region,Region管理着一个或多个HStore,其中HStore就管理着一个MemStore及多个StoreFile.

    所在RegionServer启动时,会扫描所有StoreFile，加载StoreFile的相关信息到内存，而这部分内容就是Load-on-open-section，主要包括 Root数据索引，miidKyes(optional),Meta索引,File Info,及BloomFilter metadata等。

    数据索引:

          数据索引是分层的，可以1-3层，其中第一层，即Root level Data Index，这部分数据是处放在内存区的。一开始，文件比较小，只有single-level,rootIndex直接定位到了DataBlock。当StoreFile变大时，rootIndex越来越大，随之所耗内存增大，会以多层结构存储数据索引.当采用multi-level方式，level=2时，使用root index和leaf index chunk,即内存区的rootIndex定位到的是 leafIndex,再由leafIndex定位到Datablock。当一个文件的datablock非常多，采用的是三级索引，即rootIndex定位到intermediate
 index,再由intermediate index定位到leaf index,最后定位到data block.可以看看上面图1所示，各个level的index都是分布在不同的区域的。但每部分index是以HFileBlock格式存放的，后面会比较详细地讲HFileBlock,说白了，就是HFile中的一个块。

    Fileds for midKey:

          这部分数据是Optional的，保存了一些midKey信息，可以快速地定位到midKey,常常在HFileSplit的时候非常有用。

    MetaIndex:

           即meta的索引数据，和data index类似，但是meta存放的是BloomFilter的信息，关于BloomFilter由于篇幅就不深入讨论了.

    FileInfo:

          保存了一些文件的信息，如lastKey,avgKeylen,avgValueLen等等，一会我们将会写程序将这部分内容解析出来并打印看看是什么东西。同样,FileInfo使用了Protobuf来进行序列化。

    Bloom filter metadata:

           分为GENERAL_BLOOM_META及DELETE_FAMILY_BLOOM_META二种。


    OK，下面开始操刀分割下Load-on-open-section的各个小块，看看究竟有什么东西。在开始分析之前，上面提到了一个HFileBlock想先看看。从上面可以看出来，其实基本每个小块都叫HFileBlock（除field for midkey），在Hbase中有一个类叫HFileBlock与之对应。从V2开始，即我们当前用的HFile版本，HFileBlock是支持checksum的，默认地使用CRC32，由此HFileBlock由header,data,checksum三部分内容组成,如下图所示。其中Header占了33个字节，字段是一样的，而每个block的组织会有些小差异。

![](http://images.cnitblog.com/i/640863/201406/180003401145048.jpg)


                                    图2


    了解了HFileBlock的结构，我们下面开始正式解析内存区中的各个index的block内容。首先我们根据图2我们抽象出一个简单的HFileBlock实体。


    开始解析Root Data Index和metaIndex .在Trailer解析后，我们可以得到Load-on-open-section内容的相关信息，可以构造字节数组，将这部分字节码load进内存进行解析,在解析之前先讲下FileInfo


    FileInfo的内容是以ProtoBuf放式存放的，与Trailer类似，我们先创建FileInfo.proto

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

option java_package = "com.bdifn.hbase.hfile.proto";
option java_outer_classname = "FileInfoProtos";
option java_generic_services = true;
option java_generate_equals_and_hash = true;
option optimize_for = SPEED;

message BytesBytesPair {
  required bytes first = 1;
  required bytes second = 2;
}

message FileInfoProto {
    repeated BytesBytesPair map_entry = 1;
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


编译: protoc FileInfo.proto --java_out=.

**Non-scanned block section** 关于bloomfilter先不分析了。

****Scanned block section:****


    这部分内容就是真正的数据块，从图1看出，这部分数据是分datablock存储的，默认地,每个datablock占64K,如果是多层的index的话，部分index block也会存放在这里，由于我的测试数据，是single-level的，所以只针对单级的index分析。

的single-level情况下，内存的rootDataIndex记录了每个datablock的偏移量，大小及startKey信息，主要是为了快速地定位到KeyValue的位置，在HFile中查找或者seek到某个KeyValue时,首先会在内存中，对rootDataIndex进行二分查找，单级的index可以直接定位DataBlock,然后通过迭代datablock定位到KeyValue所在的位置，而2-3层时，上面也略有提及，大家有时间的话，可以做多点研究这部分。

    弱弱提句：在HStore中，会有cache将这些datablock缓存起来，使用LRU算法，这样会提高不少性能。


    每个DataBlock同样也是一个HFileBlock,也包括header,data,checksum信息，可以用我们之前写的BlockIterator就可以搞定。下面使用代码，去遍历一个datablock看看。

实验3:


    编写KeyValue遍历器

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

public class KeyValueIterator {
    public static final int KEY_LENGTH_SIZE = 4;
    public static final int VALUE_LENGTH_SIZE = 4;
    
    private byte [] data ;
    private int currentOffset ;
    
    public KeyValueIterator(byte [] data) {
        this.data = data;
        currentOffset = 0;
    }
    
    public KeyValue nextKeyValue(){
        KeyValue kv = null;
        int keyLen = Bytes.toInt(data,currentOffset,4);
        incrementOffset(KEY_LENGTH_SIZE);
        
        int valueLen = Bytes.toInt(data,currentOffset,4);
        incrementOffset(VALUE_LENGTH_SIZE);

        //1 is memTS
        incrementOffset(keyLen,valueLen,1);
        
        int kvSize = KEY_LENGTH_SIZE + VALUE_LENGTH_SIZE +  keyLen + valueLen ;

        kv = new KeyValue(data , currentOffset - kvSize - 1, kvSize);
        return kv;
    }
    public void incrementOffset(int ... lengths) {
        for(int length : lengths)
            currentOffset = currentOffset + length;
    }
    
    public boolean hasNext() {
        return currentOffset < data.length;
    }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


     编写测试代码：



```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

//从rootDataReader中获取第一块的offset及数据大小
long offset = rootDataReader.getBlockOffsets()[0];
int size = rootDataReader.getBlockDataSizes()[0];

byte[] dataBlockArray = new byte[size];
input.seek(offset);
input.read(dataBlockArray);
//图方便，直接用iterator来解析出来FileBlock
MyHFileBlockIterator dataBlockIter = new MyHFileBlockIterator(dataBlockArray);
MyHFileBlock dataBlock1 = dataBlockIter.nextBlock();
//将data内容给一个keyvalue迭代器
KeyValueIterator kvIter = new KeyValueIterator(dataBlock1.getBlockBuf().array());
while (kvIter.hasNext()) {
    KeyValue kv = kvIter.nextKeyValue();
    //do some with keyvalue. like print the kv.
    System.out.println(kv);
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



    OK，基本上是这些内容了。有点抱歉一开篇讲得有点大了，其实没有方方面面都讲得很详细。meta,bloomfilter部分没有详细分析，大家有时间可以研究后，分享一下。



