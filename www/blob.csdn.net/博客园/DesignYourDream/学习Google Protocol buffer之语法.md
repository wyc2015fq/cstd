# 学习Google Protocol buffer之语法 - DesignYourDream - 博客园
# [学习Google Protocol buffer之语法](https://www.cnblogs.com/designyourdream/p/4262789.html)
　　上一篇结尾的时候问了几个问题，其实主要就是这个protoBuffer协议的语法，弄清楚语法后边才好开展工作嘛，不然大眼而对小眼儿，互相不认识，就没法玩耍了。其实就是学习怎么用google提供的这套 protocol buffer language 来组织我用的数据，那个神奇的proto文件的来龙去脉也得搞搞清楚。
　　好吧，还是看个例子，也是官方的：
```
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
}
```
这片代码的意思是，定义了一个message类型SearchRequest ，所谓类型，你就要知道，这不是创建对象，而是类定义。这个SearchRequest要查询一个词，然后需要知道这个词在第多少页，还要知道那一页包括了多少个关键词。这就抽象出了三项数据，每一项数据都是一个数据域，像上面就有三个数据域。我们都写过代码，显然这三个域的类型分别是string,int32,int32，这三个数据域的变量名是query,page_number,result_per_page，然后看上去很像分别进行了赋值1，2，3。但完全不知道的是required,optional是什么意思，从字面意思上理解，也许是必须，可选的定义。
　　现在对上面的三个域进行解惑吧，的确每个域都一个类型，这个类型有特别的定义，是谷歌自己定义的，包括有很多：
![](https://images0.cnblogs.com/blog/698097/201501/301711227379883.png)
![](file:///C:\Users\Administrator\AppData\Roaming\Tencent\Users\903442996\QQ\WinTemp\RichOle\]QWGPBLBHCK8B3F9[@EZOI3.png)相信程序员们都不会陌生，接触一个新的语言或者规范，很多时候都会有这样的类型定义。没什么可说的，遇到就查询这个表吧。这个表可不全，最好去查官方的，我这就是个例子。
 　　然后看看那个像赋值语句的右半部分意思，其实那个google的独有定义，叫做**unique numbered tag，**是个域的唯一标记，说明我这个域是的标记是几，这么做也许是为了方便索引，只是看起来有点奇怪。这个Tag的用法还是很讲究的，1到15会用到第一个字节，16到2047会用两个字节，我觉得大部分的应用应该两个字节就够了，要是超2047说明出问题了。官方提到了说你要把最常用的排进这1到15区间，也许第一个字节访问起来会很快？
　　再看看这个required,optional这些修饰词。先看看官方说明吧：
- `required`: a well-formed message must have exactly one of this field.（一个完整的message，必须要至少有一个这样的域）
- `optional`: a well-formed message can have zero or one of this field (but not more than one).（至多有1个这样的域）
- `repeated`: this field can be repeated any number of times (including zero) in a well-formed message. The order of the repeated values will be preserved.（这个域可重复任意次，包括0次）
　　这个修饰词基本就定义了protobuffer的灵活性，也就是说你的一个字段可以有，可以无，也可以有很多。但是，**required是弊大于利**，谷歌官方不推荐使用，因为会造成不兼容的问题，这和protobuffer的设计理念就冲突了，它也不能替你保证逻辑的正确性。这个optional，可以设置默认值，因为是可有可无，所以你可以指定默认值，也可以不指定，如果没有指定的话，在解析出该项后，将会用语言相关的默认值，例如Int ，那就是0了。注意下面语句中默认值的设置位置和分号的位置
```
optional int32 result_per_page = 3 [default = 10];
```
　　基本的语法也就这些了，碰上不懂的就去查百度吧。
　　写完Proto文件后，接下来做什么呢？按照官方说法，需要用protoc.exe这个工具来生成编程语言（例如，C++/Java/Python）用的文件，这个exe的主要作用就是读取proto文件，然后用命令行来驱动：
```
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR path/to/file.proto
```
- `IMPORT_PATH` 指定你编写好的Proto文件路径，不是单独文件的路径，而是文件夹的路径，如果没有配置的话就是当前路径，可以指定多个路径。可以考虑长远一点，这个路径可以从配置文件中读取.
- 关于输出路径，不同的语言可以输出到不同的路径下去
输出路径的指定：
- `--cpp_out` generates C++ code in `DST_DIR`. 
- `--java_out` generates Java code in `DST_DIR`. 
- `--python_out` generates Python code in `DST_DIR`. 

