# 学习Google Protocol buffer之概述 - DesignYourDream - 博客园
# [学习Google Protocol buffer之概述](https://www.cnblogs.com/designyourdream/p/4262502.html)
　　XML这种属于非常强大的一种格式，能存储任何你想存的数据，而且编辑起来还是比较方便的。致命的缺陷在于比较庞大，在某些情况下，序列化和解析都会成为瓶颈。这种对于实时性很强的应用来说，就不太适合了，想象下，设备正在紧张地进行运算，这时候来了一个解析XML任务，如果恰好这个XML比较大，那么势必产生卡顿，这个就会导致很糟糕的体验了。
　　二进制格式应该说很普遍的应用了，大家按照字节序列对好，编码和解码都不会有太大问题，但是几乎没什么扩展性，只要一方的格式变了，就得重新对消息，想象下一个有几十个字段的消息，其中有一个出错了，就得从头到尾梳理一遍，很头疼。
　　下面是Google Protocol buffer。首先什么是ProtoBuffer，它是一个可扩展，高效，能自动组装并解析结构化数据（类XML）的机制，比XML更小，更快，更简单。用它你可以对你想要的数据结构定义一次，用protobuffer的工具生成一份源代码，用这份源代码就可以从各种各样的流数据中读取或者解析你定义的数据结构。听起来很牛逼的样子，到底它是怎么工作的，又怎么应用的呢？
　　Google定义了一个名词叫做protocol buffer message，看到这个词，你就该想到这首先是个Message，那么message里一般都会有格式，真正的数据等等，那么官方的解释来了：这个所谓的message代表一块逻辑信息，这个块里是（名称---值）这样的配对关系表。看到这里就需要问个问题，那它是不是等同与C#里的字典呢？后边会继续进行分析。下面是官方给的一段代码，表示一个Message，功能是描述一个人的信息（ information about a person）：
```
message Person {
  required string name = 1;
  required int32 id = 2;
  optional string email = 3;
  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }
  message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }
  repeated PhoneNumber phone = 4;
}
```
这个Message存放在后缀名为.proto的文件里，同一个proto文件里能包含很多这样的message，那么一个message就是一个type吗？既然一个文件里能放很多message，那么放多少合适，什么样的message应该放到一个文件里？Google就想让你把你需要的进行数据交换（包括磁盘读写，网络读写）的信息以这种message的格式存放到这种proto文件里，好吧，那么这里的语法、关键字都有哪些？我又该怎么写进一个文件里呢，难道手写？这些文件又要怎么读取呢？

