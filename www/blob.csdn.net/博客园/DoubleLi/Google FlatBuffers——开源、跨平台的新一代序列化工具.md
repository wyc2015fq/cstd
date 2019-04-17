# Google FlatBuffers——开源、跨平台的新一代序列化工具 - DoubleLi - 博客园






前段时间刚试用了一个序列化工具cereal，请看[cereal:C++实现的开源序列化库](http://www.cnblogs.com/lizhenghn/p/3762984.html)，打算再总结下我对google proto buf序列化库的使用呢，

结果还没动手，大Google又出了一个新的、开源、跨平台的序列化工具：**[FlatBuffers](http://google.github.io/flatbuffers/index.html)**。那就索性先了解了解这个工具把。

一. 什么是Google FlatBuffers

FlatBuffers是一个开源的、跨平台的、高效的、提供了C++/Java接口的序列化工具库。它是Google专门为游戏开发或其他性能敏感的应用程序需求而创建。尤其更适用于移动平台，这些平台上内存大小及带宽相比桌面系统都是受限的，而应用程序比如游戏又有更高的性能要求。它将序列化数据存储在缓存中，这些数据既可以存储在文件中，又可以通过网络原样传输，而不需要任何解析开销。

代码托管主页：[https://github.com/google/flatbuffers](https://github.com/google/flatbuffers)；

项目介绍主页：[http://google.github.io/flatbuffers/index.html](http://google.github.io/flatbuffers/index.html)；

二. 为什么要使用Google FlatBuffers
- 对序列化数据的访问不需要打包和拆包——它将序列化数据存储在缓存中，这些数据既可以存储在文件中，又可以通过网络原样传输，而没有任何解析开销；
- 内存效率和速度——访问数据时的唯一内存需求就是缓冲区，不需要额外的内存分配。 这里可查看详细的[基准测试](http://google.github.io/flatbuffers/md__benchmarks.html)；
- 扩展性、灵活性——它支持的可选字段意味着不仅能获得很好的前向/后向兼容性（对于长生命周期的游戏来说尤其重要，因为不需要每个新版本都更新所有数据）；
- 最小代码依赖——仅仅需要自动生成的少量代码和一个单一的头文件依赖，很容易集成到现有系统中。再次，看基准部分细节；
- 强类型设计——尽可能使错误出现在编译期，而不是等到运行期才手动检查和修正；
- 使用简单——生成的C++代码提供了简单的访问和构造接口；而且如果需要，通过一个可选功能可以用来在运行时高效解析Schema和类JSON格式的文本；
- 跨平台——支持C++11、Java，而不需要任何依赖库；在最新的gcc、clang、vs2010等编译器上工作良好；

三. 为什么不使用Protocol Buffers的，或者JSON

Protocol Buffers的确和FlatBuffers比较类似，但其主要区别在于FlatBuffers在访问数据前不需要解析/拆包这一步。 而且Protocol Buffers既没有可选的文本导入/导出功能，也没有Schemas语法特性（比如union）。

JSON是非常可读的，而且当和动态类型语言（如JavaScript）一起使用时非常方便。然而在静态类型语言中序列化数据时，JSON不但具有运行效率低的明显缺点，而且会让你写更多的代码来访问数据（这个与直觉相反）。

想了解更多关于FlatBuffers的“为什么”请访问[flatbuffers白皮书](http://google.github.io/flatbuffers/md__white_paper.html)。

四. 内建的数据类型 
- 8 bit: `byte ubyte bool`
- 16 bit: `short ushort`
- 32 bit: `int uint float`
- 64 bit: `long ulong double`
- Vector of any other type (denoted with `[type]`). Nesting vectors is not supported, instead you can wrap the inner vector in a table.
- string, which may only hold UTF-8 or 7-bit ASCII. For other text encodings or general binary data use vectors (`[byte]` or `[ubyte]`) instead.
- References to other tables or structs, enums or unions.

详细介绍请参考：[schema语法格式](http://google.github.io/flatbuffers/md__schemas.html)。 

五. 如何使用
- 编写一个用来定义你想序列化的数据的schema文件（又称IDL），数据类型可以是各种大小的int、float，或者是string、array，或者另一对象的引用，甚至是对象集合；
- 各个数据属性都是可选的，且可以设置默认值。
- 使用FlatBuffer编译器flatc生成C++头文件或者Java类，生成的代码里额外提供了访问、构造序列化数据的辅助类。生成的代码仅仅依赖flatbuffers.h；请看[如何生成](http://google.github.io/flatbuffers/md__compiler.html)；
- 使用FlatBufferBuilder类构造一个二进制buffer。你可以向这个buffer里循环添加各种对象，而且很简单，就是一个单一函数调用；
- 保存或者发送该buffer
- 当再次读取该buffer时，你可以得到这个buffer根对象的指针，然后就可以简单的就地读取数据内容；

六. 一个简单的Schemas（IDL）文件

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

namespace zl.persons;  
  
enum GENDER_TYPE : byte  
{  
    MALE    = 0,  
    FEMALE  = 1,  
    OTHER   = 2  
}  
  
table personal_info  
{  
    id : uint;  
    name : string;  
    age : byte;  
    gender : GENDER_TYPE;  
    phone_num : ulong;  
}  
  
table personal_info_list  
{  
    info : [personal_info];  
}  
  
root_type personal_info_list;

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

注意：这里有table、struct的区别：
table是Flatbuffers中用来定义对象的主要方式，和struct最大的区别在于：它的每个字段都是可选的（类似protobuf中的optional字段），而struct的所有成员都是required。
table除了成员名称和类型之外，还可以给成员一个默认值，如果不显式指定，则默认为0（或空）。struct不能定义scalar成员，比如说string类型的成员。在生成C++代码时，struct的成员顺序会保持和IDL的定义顺序一致，如果有必要对齐，生成器会自动生成用于对齐的额外成员。如以下Schemas代码：

```
struct STest
{
    a : int;
    b : int;
    c : byte;
}
```

在生成为C++代码之后，会补充两个用于padding的成员__padding0与__padding1：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

MANUALLY_ALIGNED_STRUCT(4) STest {
 private:
  int32_t a_;
  int32_t b_;
  int8_t c_;
  int8_t __padding0;
  int16_t __padding1;
 
 public:
  STest(int32_t a, int32_t b, int8_t c)
    : a_(flatbuffers::EndianScalar(a)), b_(flatbuffers::EndianScalar(b)), c_(flatbuffers::EndianScalar(c)), __padding0(0) {}
 
  int32_t a() const { return flatbuffers::EndianScalar(a_); }
  int32_t b() const { return flatbuffers::EndianScalar(b_); }
  int8_t c() const { return flatbuffers::EndianScalar(c_); }
};
STRUCT_END(STest, 12);

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

table的成员顺序是动态调整的，这和struct有区别。在生成C++代码时，生成器会自动调整为最佳顺序以保证它占用最小的内存空间。

七. 一个完整Demo

这里只给一个函数演示如何对对象进行序列化，完整工程请直接[点击下载](http://files.cnblogs.com/lizhenghn/Google_FlatBuffers_Test.zip)，或者前往github查看[google_flatbuffers_test](https://github.com/lizhenghn123/CppLanguagePrograms/tree/master/tools/Google_FlatBuffers_Test)。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

std::string CreateOnePerson()
{
    flatbuffers::FlatBufferBuilder builder;
    fb_offset<fb_string> name = builder.CreateString("hello word");

    zl::persons::personal_infoBuilder pib(builder);
    pib.add_id(1);
    pib.add_age(25);
    pib.add_gender(zl::persons::GENDER_TYPE_MALE);
    pib.add_name(name);
    pib.add_phone_num(1234567890);
    flatbuffers::Offset<zl::persons::personal_info> personinfo = pib.Finish();

    fb_offset<zl::persons::personal_info> info[1];
    info[0] = personinfo;

    fb_offset<fb_vector<fb_offset<zl::persons::personal_info>>> info_array = fb_create_vector(builder, info, sizeof(info) / sizeof(info[0]));
    fb_offset<zl::persons::personal_info_list> info_list = create_personal_info_list(builder, info_array);
    fb_finish(builder, info_list);

    // return the buffer for the caller to use.
    return std::string(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

八. 其他

关于性能，除了Google公布的[基准测试](http://google.github.io/flatbuffers/md__benchmarks.html)外，有人自己测试验证过，上面的IDL文件即来源于该作者的[这篇文章](http://blog.csdn.net/menggucaoyuan/article/details/34409433)。

九. 参考

[http://google.github.io/flatbuffers/index.html](http://google.github.io/flatbuffers/index.html)

[http://powman.org/archives/md__schemas.html](http://powman.org/archives/md__schemas.html)

[http://blog.csdn.net/menggucaoyuan/article/details/34409433](http://blog.csdn.net/menggucaoyuan/article/details/34409433)

[http://liubin.org/2014/06/19/google-flatbuffers-cross-platform-serialization-library/](http://liubin.org/2014/06/19/google-flatbuffers-cross-platform-serialization-library/)









