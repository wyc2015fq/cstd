# Protostuff详解 - z69183787的专栏 - CSDN博客
2016年09月26日 17:39:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2776
### 一、Protostuff介绍
Protostuff是一个开源的、基于Java语言的序列化库，它内建支持向前向后兼容（模式演进）和验证功能。
Protostuff支持的序列化格式包括：
- protobuf
- protostuff
- graph 
即序列化对象图，即带循环引用的protostuff。详见：[http://www.protostuff.io/documentation/object-graphs/](http://www.protostuff.io/documentation/object-graphs/)
- json
- smile 
即二进制json，从protostuff-json模块中使用。Smile数据格式是由Jackson JSON库开发团队于2010年发布的数据格式，并在Jackson 1.6版本开始使用此格式。 
Smile格式规范：[http://wiki.fasterxml.com/SmileFormatSpec](http://wiki.fasterxml.com/SmileFormatSpec)
与此相似的有BSON格式，见：[http://bsonspec.org/](http://bsonspec.org/)
要注意一点，Smile数据格式和BSON数据格式是互不兼容的。比如BSON定义了日期类型，而Smile没有日期类型。而且，有时候BSON数据格式占用的空间比原生JSON占用的空间更多。所以，对于二进制JSON来说，Smile才是更好的选择。 
可以参考通用二进制JSON规范《Universal Binary JSON Specification》：[http://ubjson.org/](http://ubjson.org/)
- xml
- yaml 
只支持序列化
- kvp 
即二进制的uwsgi头部（Header），详见：[http://projects.unbit.it/uwsgi](http://projects.unbit.it/uwsgi)
### 二、Protostuff特征
1、支持protostuff-compiler产生的消息
2、支持现有的POJO
3、支持现有的protoc产生的Java消息
4、与各种移动平台的互操作能力（Android、Kindle、j2me）
5、支持转码
### 三、Protostuff的模块
#### 1、protostuff-api模块
面向消息和POJO（message/pojo）的序列化API，内建了对模式演进的支持。 
可以与现有对象一起工作，只需添加模式Schema。可以通过代码生成、在运行时使用protostuff-runtime生成、或者是手写产生。
#### 2、protostuff-core模块
绑定了三种二进制格式：protostuff、graph、protobuf。
protostuff与protobuf的区别： 
1）protobuf有一个名为“group”的编码类型域，而protostuff使用它作为嵌套的消息。 
2）protostuff可以使用尾部界定符来处理消息流（Stream）。 
3）protostuff的一级类在本地格式中支持循环引用。
#### 3、protostuff-runtime模块
自动启用现有的POJO序列化/反序列化为各种格式。 
可使用运行时序列化策略，w/c通过系统属性进行配置。 
依赖于protostuff-API和protostuff-collectionschema。
#### 4、protostuff-json模块
面向消息或POJO的JSON序列化/反序列化。 
面向标量域的向前向后兼容。 
依赖于protostuff-api、jackson-core-asl-1.7.9。如果使用了Smile格式，还需jackson-smile-1.7.9。
#### 5、protostuff-parser模块
使用了ANTLR V3、面向.proto文件的词法/语法解析器。 
供protostuff-compiler使用。
#### 6、protostuff-compiler模块
面向.proto源文件的编译器。 
可以从文件系统、类路径或从网络HTTP URL中载入.proto源文件。 
可使用参数-Dproto_path=$path告诉编译器从哪里载入源文件。 
可扩展/定制编译的输出。 
要编译源文件，执行命令： 
java -jar protostuff-compiler-1.0.9.jar 
protostuff.properties属性文件定义：
```
modules = foo
foo.source = path/to/your/foo.proto
#java_bean, gwt_overlay, java_v2protoc_schema
foo.output = java_bean
foo.outputDir = generated
foo.options = some_key,key:value,another_key
```
依赖于protostuff-parser模块。
#### 7、protostuff-me模块
对于j2me应用，无需外部依赖 
与api模块和core模块结合一起使用，去掉了Java泛型支持。
### 四、模式Schema
Schema包含： 
1）对象进行序列化的逻辑 
2）对象进行反序列化的逻辑 
3）对象必填字段的验证 
4）对象字段名称到字段编号的映射 
5）对象的实例化
对于现有的对象，必须使用protostuff-runtime来生成Schema——可以通过反射来缓存、使用Schema。 
开发者更喜欢自定义Schema，比如对必填字段的验证等，那么可以手动编码。
