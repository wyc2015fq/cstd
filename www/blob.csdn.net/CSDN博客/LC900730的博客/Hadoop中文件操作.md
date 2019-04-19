# Hadoop中文件操作 - LC900730的博客 - CSDN博客
2017年11月22日 15:21:32[lc900730](https://me.csdn.net/LC900730)阅读数：118
## hadoop文件的序列化
- Hadoop中序列化机制在每个类对象第一次出现时候保持了每个类的信息，如类名，第二次出现时候会有一个类的reference，导致空间浪费。
- Java序列化不能复用对象，Java反序列化时候每次需要构造出新对象。在Hadoop序列化机制中，反序列化的对象是可以复用的。
#### Hadoop中定义了两个序列化相关的接口：Writable和Comparable，这2个接口可以合成一个WritableComparable接口
### Writable接口
- 将状态写入二进制格式的DataOutput流
- 另一个用于从二进制格式的DataInput流读取其状态
```
public interface Writable{
    void write(DataOutput out);
    void readFields(DataInput in);
}
```
#### IntWritable
```
IntWritable writable =new IntWritable();
//set函数赋值
writable.set(163);
//构造函数来赋值
IntWritable writable =new IntWritable(163);
```
