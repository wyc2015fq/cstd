# Hadoop I/O - Orisun - 博客园







# [Hadoop I/O](https://www.cnblogs.com/zhangchaoyang/articles/2647905.html)





在Hadoop集群中，数据在网络上传输，保证数据完整性的通常做法使用checksum，比如常用的 CRC-32 (cyclic redundancy check)。

Hadoop上支持的文件压缩格式有：gzip  ZIP  bzip2  LZO。例如在UNIX上可以使用命令：gzip -1 file，会生成file.gz，但是原来的file就没有了。压缩算法都要在执行速度和压缩比上做一个权衡，-1表示只注意速度，-9表示只注重压缩比。

CompressionOutputStream和CompressionInputStream很类似于java.util.zip.DeflaterOutputStream 和 java.util.zip.DeflaterInputStream。

```
Interface CompressionCodec{
　　CompressionInputStream createInputStream(InputStream in);　　　　　　//解压
　　CompressionOutputStream createOutputStream(OutputStream out);　　　　//压缩

　　……

}
```

实现了CompressionCodec接口的类有：BZip2Codec, DefaultCodec, GzipCodec, SnappyCodec

```
package io;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.CompressionCodecFactory;
import org.apache.hadoop.io.compress.CompressionOutputStream;
import org.apache.hadoop.io.compress.GzipCodec;

public class FileCmpress {

    public static void main(String[] args) throws IOException{
        //解压示例codec.createInputStream
        String uri=args[0];
        Configuration conf=new Configuration();
        FileSystem fs=FileSystem.get(URI.create(uri),conf);
        Path inputPath=new Path(uri);
        CompressionCodecFactory factory=new CompressionCodecFactory(conf);
        CompressionCodec codec=factory.getCodec(inputPath);        //根据文件名的后缀来选择生成哪种类型的CompressionCodec
        if(codec==null){
            System.err.println("No codec found for "+uri);
            System.exit(1);
        }
        String outputUri=CompressionCodecFactory.removeSuffix(uri, codec.getDefaultExtension());
        InputStream in=null;
        OutputStream out=null;
        try{
            in=codec.createInputStream(fs.open(inputPath));        //对输入流进行解压
            out=fs.create(new Path(outputUri));
            IOUtils.copyBytes(in, out, conf);
        }finally{
            IOUtils.closeStream(in);
            IOUtils.closeStream(out);
        }
        //压缩示例codec.createOutputStream
        CompressionOutputStream outStream=null;
        Path op2=new Path("2.gz");
        try{
            in=fs.open(new Path(outputUri));        //打开原始文件
            GzipCodec gzipCodec=new GzipCodec();    //创建gzip压缩实例
            gzipCodec.setConf(conf);                //给CompressionCodec设置Configuration
            outStream=gzipCodec.createOutputStream(fs.create(op2));        //打开输出文件（最终的压缩文件）
            IOUtils.copyBytes(in, outStream, 4096,false);        //从输入流向输出流拷贝，GzipCodec负责对输出流进行压缩
        }finally{
            IOUtils.closeStream(in);
            IOUtils.closeStream(out);
        }
        
    }
}
```

注意文件经过压缩之后再送给MapReduce时显然就不能再split了，还好gzip和LZO格式的文件（hadoop通过查看文件名后缀）默认就是不支持split的，而bzip2可以被split，ZIP文件则不能作为MapReduce的输入文件。

可以让Reduce的输出文件被压缩。

```
conf.setBoolean("mapred.output.compress", true);　　　　//支持输出被压缩
conf.setClass("mapred.output.compression.codec", GzipCodec.class,CompressionCodec.class);　　　　//指定采用的压缩算法
```

让Map的输出被压缩：

```
conf.setCompressMapOutput(true);
conf.setMapOutputCompressorClass(GzipCodec.class);
```

Java中的基本数据类型在Hadoop中都有对应的Writable类型，Hadoop中还有几中Writable Collections。ArrayWriatble和TwoDArrayWriatble分别是适用于一维数组和二维数组的Wriatble类型，当然数组中元素必须是相同的类型。

ArrayWritable writable = new ArrayWritable(Text.class);

但是ArrayWritable还是不要用了，Hadoop中没有实现ArrayWritable的空构造函数，而作为实现了Writable的类，是必须提供空参数构造函数的----在调用readFields(DataInput in)之前要先调用空构造函数来建立实例，所以每当我使用ArrayWritable作为Mapper的输出类型时就会报错。

MapWriatble实现了java.util.Map<Wriatble,Writable>接口；而SortedMapWritable则实现了java.util.Map<WriatbleComparable,Writable>接口。

```
MapWritable src = new MapWritable();
src.put(new IntWritable(1), new Text("cat"));
src.put(new VIntWritable(2), new LongWritable(163));
```

用户自定义Writable类型

```
package basic;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;

public class TextPair implements WritableComparable<TextPair> {
    private Text first;
    private Text second;

    public TextPair() {
        set(new Text(), new Text());
    }

    public TextPair(String first, String second) {
        set(new Text(first), new Text(second));
    }

    public TextPair(Text first, Text second) {
        set(first, second);
    }

    public void set(Text first, Text second) {
        this.first = first;
        this.second = second;
    }

    public Text getFirst() {
        return first;
    }

    public Text getSecond() {
        return second;
    }

    @Override
    public void write(DataOutput out) throws IOException {
        first.write(out);
        second.write(out);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        first.readFields(in);
        second.readFields(in);
    }

    @Override
    public int hashCode() {
        return first.hashCode() * 163 + second.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof TextPair) {
            TextPair tp = (TextPair) o;
            return first.equals(tp.first) && second.equals(tp.second);
        }
        return false;
    }

    @Override
    public String toString() {
        return first + "\t" + second;
    }

    @Override
    public int compareTo(TextPair tp) {
        int cmp = first.compareTo(tp.first);
        if (cmp != 0) {
            return cmp;
        }
        return second.compareTo(tp.second);
    }
}
```

### SequenceFile

HDFS和MapReduce都是为处理大文件设计的，所以把诸多小文件打包在一个SequenceFile中着实是一个提高效率的好方法。

写入SequenceFile的key或value数据类型没有必要是Writable类型，只要是可以被Serialization序列化和反序列化的类型就可以了。

用SequenceFile.createWriter()创建SequenceFile.Writer，把一个文本文件写入一个二进制文件。代码如下

```
package basic;

import java.io.IOException;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;

public class SequenceFileWriteDemo {
    private static final String[] DATA = { "One, two, buckle my shoe",
            "Three, four, shut the door", "Five, six, pick up sticks",
            "Seven, eight, lay them straight", "Nine, ten, a big fat hen" };

    public static void main(String[] args) throws IOException {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);
        Path path = new Path(uri);
        IntWritable key = new IntWritable();
        Text value = new Text();
        SequenceFile.Writer writer = null;
        try {
            writer = SequenceFile.createWriter(fs, conf, path, key.getClass(),
                    value.getClass());

            for (int i = 0; i < 100; i++) {
                key.set(100 - i);
                value.set(DATA[i % DATA.length]);
                System.out.printf("[%s]\t%s\t%s\n", writer.getLength(), key,
                        value);
                writer.append(key, value);　　　　//采用append方式
            }
        } finally {
            IOUtils.closeStream(writer);
        }
    }
}
```

可以想像上面代码中的DATA是从小文件中读取的，我们从多个小文件中读取文本，转换字节流append到一个大的二进制文件中，既实现了多个小文件的合并，也实现了压缩（二进制文件比文本文件小）。

下面演示读取一个SequenceFile

```
package basic;

import java.io.IOException;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.util.ReflectionUtils;

public class SequenceFileReadDemo {
    public static void main(String[] args) throws IOException {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);
        Path path = new Path(uri);

        SequenceFile.Reader reader = null;
        try {
            reader = new SequenceFile.Reader(fs, path, conf);
            Writable key = (Writable) ReflectionUtils.newInstance(
                    reader.getKeyClass(), conf);
            Writable value = (Writable) ReflectionUtils.newInstance(
                    reader.getValueClass(), conf);
            long position = reader.getPosition();
            while (reader.next(key, value)) {　　　　//循环读取文件
                String syncSeen = reader.syncSeen() ? "*" : "";　　　　//SequenceFile中都有sync标记
                System.out.printf("[%s%s]\t%s\t%s\n", position, syncSeen, key,
                        value);
                position = reader.getPosition(); //下一条record开始的位置
            }
        } finally {
            IOUtils.closeStream(reader);
        }
    }
}
```

SequenceFile.Writer会在流中写入sync pointer，由于某些原因，SequenceFile.Reader可能会找不到record的边界，sync pointer就是用来标记record边界的，但它并不是在每个record的后面都做一个标记，而隔几个做一个。sync pointer不会超过文件长度的1%。

SequenceFile同样支持随机读写：reader.seek(360);

而reader.sync(360)则是定位到360之后的第1个sync pointer处。

在命令行显示一个SequenceFile的内容不能用cat，而要用-text（表示要用文本的形式显示二进制文件）

$ hadoop fs -text myfile.seq

SequenceFile文件的最开始是Header部分，其中包含了record的key-value的数据类型、sync标记所采用的字符、有关压缩的细节、用户自定义的metadata。SequenceFile内置的压缩方式有两种：RecordCompression和Block Compression。

SequenceFile的文件结构：

![](https://pic002.cnblogs.com/images/2012/103496/2012082022144338.png)

![](https://pic002.cnblogs.com/images/2012/103496/2012082022145141.png)

二进制文件具有清晰严谨的文件结构，读写速度自然要比文本文件快，SequenceFile更容易和Hadoop的基本数据类型(IntWritable,FloatWriatble等)进行交互。

下面的代码展示如何把一个文本文件转换为SequenceFile，job.setOutputFormatClass(SequenceFileOutputFormat.class);就可以了。

```
1 package basic;
 2 
 3 import org.apache.hadoop.conf.Configuration;
 4 import org.apache.hadoop.conf.Configured;
 5 import org.apache.hadoop.fs.FileSystem;
 6 import org.apache.hadoop.fs.Path;
 7 import org.apache.hadoop.io.LongWritable;
 8 import org.apache.hadoop.io.SequenceFile.CompressionType;
 9 import org.apache.hadoop.io.Text;
10 import org.apache.hadoop.io.compress.GzipCodec;
11 import org.apache.hadoop.mapreduce.Job;
12 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
13 import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
14 import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
15 import org.apache.hadoop.util.Tool;
16 import org.apache.hadoop.util.ToolRunner;
17 
18 public class ToSeqFile extends Configured implements Tool {
19 
20     @Override
21     public int run(String[] arg0) throws Exception {
22         Job job = new Job();
23         job.setJarByClass(getClass());
24         Configuration conf=getConf();
25         FileSystem fs = FileSystem.get(conf);
26     
27         FileInputFormat.setInputPaths(job, "/user/orisun/input/rdata");
28         Path outDir=new Path("/user/orisun/output");
29         fs.delete(outDir,true);
30         FileOutputFormat.setOutputPath(job, outDir);
31         
32         job.setNumReduceTasks(0);
33         job.setOutputKeyClass(LongWritable.class);
34         job.setOutputValueClass(Text.class);
35         //设置OutputFormat为SequenceFileOutputFormat
36         job.setOutputFormatClass(SequenceFileOutputFormat.class);
37         //允许压缩
38         SequenceFileOutputFormat.setCompressOutput(job, true);
39         //压缩算法为gzip
40         SequenceFileOutputFormat.setOutputCompressorClass(job, GzipCodec.class);
41         //压缩模式为BLOCK
42         SequenceFileOutputFormat.setOutputCompressionType(job, CompressionType.BLOCK);
43 
44 
45         return job.waitForCompletion(true)?0:1;
46     }
47 
48     public static void main(String[] args) throws Exception {
49         int res = ToolRunner.run(new Configuration(), new ToSeqFile(), args);
50         System.exit(res);
51     }
52 }
```

代码中没有设置FileInputFormat，所以采用默认的TextInputFormat。代码中没有设置MapperClass，默认情况下map把从InputFormat中得到的key-value原样输出（新版本的Hadoop中已经没有了IndentityMapper这个类，所以我没有说“默认的Mapper就是IndentityMapper”）。

###  MapFile

MapFile是排序后的SequenceFile，并且它会额外生成一个index文件提供按key的查找。

读写MapFile与读写SequenceFile非常类似，只需要换成MapFie.Reader和MapFile.Writer就可以了。在命令行显示MapFile的文件内容同样要用-text。

与SequenceFile不同的是：由于MapFile需要按key排序，所以它的key必须是WritableComparable类型的。

MapFile会生成2个文件，一个名为data，一个名为index。data中的内容就是按key排序后的SequenceFile中的内容

$ hadoop fs -text numbers.map/data | head

1　　One, two, buckle my shoe
2　　Three, four, shut the door
3　　Five, six, pick up sticks
4　　Seven, eight, lay them straight
5　　Nine, ten, a big fat hen
6　　One, two, buckle my shoe
7　　Three, four, shut the door

$ hadoop fs -text numbers.map/index
1　　128
129　　6079
257　　12054
385　　18030
第1列就是data文件中的key值，第2列是key在data文件中的offset。我们看到并不是所有的key都记录在了index文件中，而是隔128个才记录一个（这个间隔可以在io.map.index.interval属性中设置，或直接在代码中通过MapFile.Writer实例的setIndexInterval()函数来设置）。

下面我们来看一下index文件是怎么发挥作用的。

data文件中存放的每条record都是一个key-value对，我们可以根据下面的函数由key值获取value。

public Writable get(WritableComparable key, Writable val) throws IOException

如果指定的key不存在，则不会给value赋值。

假如我们要检索的key值是300。首先MapFile.Reader把index文件读入内存，在*index文件中*进行二分查找，找到等于或第一个小于300的key-value对：257--12054，然后在data文件中从12054的位置开始查找，直到找到300或第一个大于300的key。可见整个查找的过程需要在内存中做一次二分查找，然后作一次文件扫描，且扫描文件的行数不会超过128行（如果io.map.index.interval属性值为128话）。

当index文件很大时，全部读到内存中也是不现实的。当然你会说我们可以调大io.map.index.interval的值，但那样的话需要重新生成MapFile。在index文件已经生成的情况下我们可以设置io.map.index.skip的值，设为1就表示index中的记录每隔一行才被载入内存。

使用MapFile.fix把一个SequenceFile转换成MapFile：

```
1 package basic;
 2 
 3 import java.net.URI;
 4 
 5 import org.apache.hadoop.conf.Configuration;
 6 import org.apache.hadoop.fs.FileSystem;
 7 import org.apache.hadoop.fs.Path;
 8 import org.apache.hadoop.io.MapFile;
 9 import org.apache.hadoop.io.SequenceFile;
10 
11 public class MapFileFixer {
12     @SuppressWarnings({ "rawtypes", "unchecked" })
13     public static void main(String[] args) throws Exception {
14     String mapUri = args[0];
15     Configuration conf = new Configuration();
16     FileSystem fs = FileSystem.get(URI.create(mapUri), conf);        //MapFile（由data和index组成的目录）也可以自成一个FileSystem
17     Path map = new Path(mapUri);
18     Path mapData = new Path(map, MapFile.DATA_FILE_NAME);
19     //通过SequenceFile.Reader来获取SequenceFile的key和value类型
20     SequenceFile.Reader reader = new SequenceFile.Reader(fs, mapData, conf);
21     Class keyClass = reader.getKeyClass();
22     Class valueClass = reader.getValueClass();
23     reader.close();
24 
25     //使用MapFile.fix把一个SequenceFile转换成MapFile
26     long entries = MapFile.fix(fs, map, keyClass, valueClass, false, conf);
27     System.out.printf("Created MapFile %s with %d entries\n", map, entries);
28 
29     }
30 }
```

程序开始前先新建一个目录假如取名为map

$ hadoop fs -mkdir map

把一个sequencefile放到map目录下，并重命名为data（注意必须重命名为data）

$ hadoop fs -put myseqfile map/data

最后运行程序

$ hadoop jar dm.jar basic.MapFileFixer map

结果：data文件中的record按key值重新排序，并在map目录下生成了index文件。












