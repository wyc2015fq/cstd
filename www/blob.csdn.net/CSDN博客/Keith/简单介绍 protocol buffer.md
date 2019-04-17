# 简单介绍 protocol buffer - Keith - CSDN博客





2017年06月15日 06:48:20[ke1th](https://me.csdn.net/u012436149)阅读数：582
个人分类：[c++](https://blog.csdn.net/u012436149/article/category/6930661)









# protocol buffer

`protocol buffer` 是谷歌的一款序列化结构数据的工具. 

它有几个核心的概念:
- `.proto`文件: 定义`protocol buffer message` 类型的地方,这个类型指定了如何序列化你的结构数据,如下所示, 这个`.proto`文件定义了一个`Person`  结构数据(也可以看作类/结构体).

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
- `protoc`工具: `protocol buffer compiler`,用于编译`.proto` 文件,编译完之后,会出现两个文件 `**.pb.h`和`**.pb.cc` , 这两个文件中就包含了刚才我们在`.proto`中定义的结构数据, 在生成的文件中以`类` 的形式体现, 同时也存在 序列化和解析 这个结构数据的方法.

```
//生成文件中的一小部分
class Person : public ::google::protobuf::Message {
 public:
  Person();
  virtual ~Person();
  Person(const Person& from);
```
- 在`c++` 代码中 包含这个头文件就可以创建这个类的对象了, 创建了对象之后,我们可以把这个对象序列化, 保存到 `.pb` 文件中, 我们也可以从`.pb`文件中来还原之前序列化的对象.
- 序列化的时候,有两种方式, 可以序列化成 `.pbtxt`(用这个后缀表示是txt文件,防止混淆),这是一个文本文件,人类可读,但是占用空间大. 也可以序列化成 `pb`(用这个表示二进制文件)文件, 这个是二进制文件,人类不可读,但是占用空间小,传输方便.

## 参考资料

[https://developers.google.com/protocol-buffers/docs/cpptutorial](https://developers.google.com/protocol-buffers/docs/cpptutorial)



