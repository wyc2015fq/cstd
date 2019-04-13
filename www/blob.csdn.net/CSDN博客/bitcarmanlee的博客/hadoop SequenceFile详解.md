
# hadoop SequenceFile详解 - bitcarmanlee的博客 - CSDN博客


2017年09月27日 11:46:20[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：3104



## 1. 什么是SequenceFile
1.1.sequenceFile文件是Hadoop用来存储二进制形式的[Key,Value]对而设计的一种平面文件(Flat File)。
1.2.可以把SequenceFile当做是一个容器，把所有的文件打包到SequenceFile类中可以高效的对小文件进行存储和处理。
1.3.SequenceFile文件并不按照其存储的Key进行排序存储，SequenceFile的内部类Writer提供了append功能。
1.4.SequenceFile中的Key和Value可以是任意类型Writable或者是自定义Writable。
1.5.在存储结构上，SequenceFile主要由一个Header后跟多条Record组成，Header主要包含了Key  classname，value classname，存储压缩算法，用户自定义元数据等信息，此外，还包含了一些同步标识，用于快速定位到记录的边界。每条Record以键值对的方式进行存储，用来表示它的字符数组可以一次解析成：记录的长度、Key的长度、Key值和value值，并且Value值的结构取决于该记录是否被压缩。
## 2.SequenceFile支持数据压缩
2.1.SequenceFIle的内部格式取决于是否启用压缩，如果是压缩，则又可以分为记录压缩和块压缩。
2.2.有一下三种类型的压缩：
A.无压缩类型：如果没有启用压缩(默认设置)那么每个记录就由它的记录长度(字节数)、键的长度，键和值组成。长度字段为4字节。
B.记录压缩类型：记录压缩格式与无压缩格式基本相同，不同的是值字节是用定义在头部的编码器来压缩。注意：键是不压缩的。下图为记录压缩：
![这里写图片描述](https://img-blog.csdn.net/20170927114255881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
C.块压缩类型：块压缩一次压缩多个记录，因此它比记录压缩更紧凑，而且一般优先选择。当记录的字节数达到最小大小，才会添加到块。该最小值由io.seqfile.compress.blocksize中的属性定义。默认值是1000000字节。格式为记录数、键长度、键、值长度、值。下图为块压缩：
![这里写图片描述](https://img-blog.csdn.net/20170927114336138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3.优缺点
SequenceFile优点：
A.支持基于记录(Record)或块(Block)的数据压缩。
B.支持splitable，能够作为MapReduce的输入分片。
C.修改简单：主要负责修改相应的业务逻辑，而不用考虑具体的存储格式。
SequenceFile的缺点
A.需要一个合并文件的过程，且合并后的文件不方便查看。
## 4.具体代码实现
以下代码实现了读写SequenceFile的需求。
```python
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.util.ReflectionUtils;
import java.io.IOException;
```
```python
public
```
```python
class
```
```python
SequenceTest {
```
```python
public
```
```python
static
```
```python
final String output_path =
```
```python
"xxx"
```
```python
;
```
```python
private
```
```python
static
```
```python
final String[] DATA = {
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
,
```
```python
"d"
```
```python
};
    @SuppressWarnings(
```
```python
"deprecation"
```
```python
)
```
```python
public
```
```python
static
```
```python
void
```
```python
write
```
```python
(String pathStr) throws IOException {
        Configuration conf =
```
```python
new
```
```python
Configuration();
        FileSystem fs = FileSystem.
```
```python
get
```
```python
(conf);
        Path path =
```
```python
new
```
```python
Path(pathStr);
        SequenceFile.Writer writer = SequenceFile.createWriter(fs, conf, path, Text.class, IntWritable.class);
        Text key =
```
```python
new
```
```python
Text();
        IntWritable
```
```python
value
```
```python
=
```
```python
new
```
```python
IntWritable();
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < DATA.length; i++) {
            key.
```
```python
set
```
```python
(DATA[i]);
```
```python
value
```
```python
.
```
```python
set
```
```python
(i);
            System.
```
```python
out
```
```python
.printf(
```
```python
"[%s]\t%s\t%s\n"
```
```python
, writer.getLength(), key,
```
```python
value
```
```python
);
            writer.append(key,
```
```python
value
```
```python
);
        }
        IOUtils.closeStream(writer);
    }
    @SuppressWarnings(
```
```python
"deprecation"
```
```python
)
```
```python
public
```
```python
static
```
```python
void
```
```python
read
```
```python
(String pathStr) throws IOException {
        Configuration conf =
```
```python
new
```
```python
Configuration();
        FileSystem fs = FileSystem.
```
```python
get
```
```python
(conf);
        Path path =
```
```python
new
```
```python
Path(pathStr);
        SequenceFile.Reader reader =
```
```python
new
```
```python
SequenceFile.Reader(fs, path, conf);
        Writable key = (Writable) ReflectionUtils.newInstance(reader.getKeyClass(), conf);
        Writable
```
```python
value
```
```python
= (Writable) ReflectionUtils.newInstance(reader.getValueClass(), conf);
```
```python
while
```
```python
(reader.next(key,
```
```python
value
```
```python
)) {
            System.
```
```python
out
```
```python
.printf(
```
```python
"%s\t%s\n"
```
```python
, key,
```
```python
value
```
```python
);
        }
        IOUtils.closeStream(reader);
    }
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args) throws IOException {
        write(output_path);
        read(output_path);
    }
}
```
第一个方法为往hdfs上面写，第二个方法则为读。

