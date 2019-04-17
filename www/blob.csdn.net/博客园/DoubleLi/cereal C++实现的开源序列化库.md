# cereal:C++实现的开源序列化库 - DoubleLi - 博客园






闲来无事发现了一个基于C++实现的序列化工具，相比于其他（比如Boost serialization或Google protobuf，恰巧都用过，以后再介绍），使用简单，感觉不错，下面做个摸索。

cereal介绍

[cereal](http://uscilab.github.io/cereal/index.html)是一个开源的（BSD License）、轻量级的、支持C++11特性的、仅仅包含头文件实现的、跨平台的C++序列化库。它可以将任意的数据类型序列化成不同的表现形式，比如二进制、XML格式或JSON。cereal的设计目标是快速、轻量级、易扩展——它没有外部的依赖关系，而且可以很容易的和其他代码封装在一块或者单独使用。

cereal支持标准库的几乎每一个类型的序列化。cereal也完全支持继承和多态。由于cereal被设计为一个精简、快速的库，它不像其他序列化库（比如Boost）在同一层次上会进行对象跟踪，这也导致了它不支持原始指针(raw pointer)和引用，但是智能指针（比如std::shared_ptr和std​​::unique_ptr）是没有问题的。

cereal适用于基于C++11标准的各种编译器 
cereal使用了一些C++11的新特性，因此需要一个兼容性更好的的C++编译器才能正常工作。已被验证可用的编译器有g++4.7.3、clang++3.3、MSVC2013，或者更新版本。
它也可能可以在老版本编译器上工作，但并不保证完全支持。当使用g++或clang++编译器时，cereal同时需要libstdc++和libc++库。 

cereal：更快速，更好的压缩
在简单的性能测试中，cereal通常比Boost的序列化库速度更快，而且产生的二进制形式占用更少的空间，尤其是针对更小的对象。cereal使用了C++中的速度最快的[XML](http://rapidxml.sourceforge.net/)和[JSON](https://code.google.com/p/rapidjson/)解析器和包装器。

cereal是可扩展的 
cereal提供了对标准库的序列化支持，比如二进制的，XML和JSON序列化器。
cereal的源代码相比Boost来讲，更容易理解和扩展。 如果你需要别的东西，cereal可以很容易地扩展，比如添加自定义序列化存档或类型。

cereal是易于使用的 
在代码增加cereal序列化功能可以简化为包含一个头文件，写一个序列化函数。无论是从概念上还是代码层次上，cereal的功能都是自文档化的。
如果你使用错误，cereal尽可能的在编译期触发静态断言。

对于Boost使用者来说，cereal提供了相似的语法，如果你使用过Boost的序列化库，你会发现cereal的语法看起来很熟悉。

如果你是从Boost转向使用cereal，一定要阅读这个过渡指南：[http://uscilab.github.io/cereal/transition_from_boost.html](http://uscilab.github.io/cereal/transition_from_boost.html)

简单的使用 

好吧，废话就这么多，先上一个简单的事例：

```
std::ofstream os("my.xml");
cereal::XMLOutputArchive archive(os);
int age = 26;
std::string name = "lizheng";
archive(CEREAL_NVP(age), cereal::make_nvp("Name", name));
```

以上代码完成了对一个int类型和string类型的xml序列化实现。结果如下：

```
<?xml version="1.0" encoding="utf-8"?>
<cereal>
    <age>26</age>
    <Name>lizheng</Name>
</cereal>
```

注意上面代码中的cereal::XMLOutputArchive，其实还有针对JSON、二进制序列化的类，如果是序列化为JSON串，结果如下（代码在最下面）：

```
{
    "age": 26,
    "Name": "lizheng"
}
```

我的Demo

完整代码如下（或点此下载[完整工程](http://files.cnblogs.com/lizhenghn/cereal_test.zip)，或者从我的github下载包括cereal头文件在内的[整个项目](https://github.com/lizhenghn123/myCpp11Study)）：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

  1 #include <iostream>
  2 #include <fstream>
  3 #include <string>
  4 #include "cereal/archives/binary.hpp"
  5 #include "cereal/archives/xml.hpp"
  6 #include "cereal/archives/json.hpp"
  7 #include "cereal/types/unordered_map.hpp"
  8 #include "cereal/types/memory.hpp"
  9 #include "cereal/types/string.hpp"  //一定要包含此文件，否则无法将std::string序列化为二进制形式，请看：https://github.com/USCiLab/cereal/issues/58
 10 
 11 using namespace std;
 12 
 13 struct MyRecord
 14 {
 15     int x, y;
 16     float z;
 17 
 18     template <class Archive>
 19     void serialize(Archive & ar)
 20     {
 21         ar(x, y, z);
 22     }
 23 
 24     friend std::ostream& operator<<(std::ostream& os, const MyRecord& mr);
 25 };
 26 
 27 std::ostream& operator<<(std::ostream& os, const MyRecord& mr)
 28 {
 29     os << "MyRecord(" << mr.x << ", " << mr.y << "," << mr.z << ")\n";
 30     return os;
 31 }
 32 
 33 struct SomeData
 34 {
 35     int32_t id;
 36     std::shared_ptr<std::unordered_map<uint32_t, MyRecord>> data;
 37 
 38     SomeData(int32_t id_=0) : id(id_), data(new std::unordered_map<uint32_t, MyRecord>)
 39     {
 40         
 41     }
 42 
 43     template <class Archive>
 44     void save(Archive & ar) const
 45     {
 46         ar(id, data);
 47     }
 48 
 49     template <class Archive>
 50     void load(Archive & ar)
 51     {
 52         ar(id, data);
 53     }
 54 
 55     void push(uint32_t, const MyRecord& mr)
 56     {
 57         data->insert(std::make_pair(100, mr));
 58     }
 59 
 60     void print()
 61     {
 62         std::cout << "ID : " << id << "\n";
 63         if (data->empty())
 64             return;
 65         for (auto& item : *data)
 66         {
 67             std::cout << item.first << "\t" << item.second << "\n";
 68         }
 69     }
 70 };
 71 
 72 void Serialization_XML()
 73 {
 74     {
 75         std::ofstream os("my.xml");
 76 
 77         cereal::XMLOutputArchive archive(os);
 78 
 79         int age = 26;
 80         std::string name = "lizheng";
 81 
 82         //#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)
 83         archive(CEREAL_NVP(age), cereal::make_nvp("Name", name));
 84 
 85         //os.close();  //注意：这里不能显示关闭ofstream，否则序列化无法写入到文件
 86     }
 87 
 88     {
 89         std::ifstream is("my.xml");
 90         cereal::XMLInputArchive archive(is);
 91 
 92         int age;
 93         std::string name;
 94 
 95         archive(age, name);
 96         std::cout << "Age: " << age << "\n" << "Name: " << name << "\n";
 97     }
 98 }
 99 
100 void Serialization_JSON()
101 {
102     {
103         std::ofstream os("my.json");
104         cereal::JSONOutputArchive archive(os);
105 
106         int age = 26;
107         std::string name = "lizheng";
108 
109         archive(CEREAL_NVP(age), cereal::make_nvp("Name", name));
110     }
111 
112     {
113         std::ifstream is("my.json");
114         cereal::JSONInputArchive archive(is);
115 
116         int age;
117         std::string name;
118 
119         archive(age, name);
120         std::cout << "Age: " << age << "\n" << "Name: " << name << "\n";
121     }
122 }
123 
124 
125 void Serialization_Binary()
126 {
127     {
128         std::ofstream os("my.binary", std::ios::binary);
129         cereal::BinaryOutputArchive archive(os);
130         
131         int age = 26;
132         std::string name = "lizheng";
133 
134         archive(CEREAL_NVP(age), CEREAL_NVP(name));
135     }
136     {
137         std::ifstream is("my.binary", std::ios::binary);
138         cereal::BinaryInputArchive archive(is);
139 
140         int age;
141         std::string name;
142 
143         archive(age, name);
144         std::cout << "Age: " << age << "\n" << "Name: " << name << "\n";
145     }
146 }
147 
148 void Serialization_Obj()
149 {
150     {
151         std::ofstream os("obj.cereal", std::ios::binary);
152         cereal::BinaryOutputArchive archive(os);
153 
154         MyRecord mr = { 1, 2, 3.0 };
155 
156         SomeData myData(1111);
157         myData.push(100, mr);
158     
159         archive(myData);
160     }
161     {
162         std::ifstream is("obj.cereal", std::ios::binary);
163         cereal::BinaryInputArchive archive(is);
164 
165         SomeData myData;
166         archive(myData);
167         myData.print();
168     }
169 }
170 
171 
172 int main()
173 {
174     Serialization_XML();     std::cout << "----------------------\n";
175 
176     Serialization_JSON();    std::cout << "----------------------\n";
177 
178     Serialization_Binary();  std::cout << "----------------------\n";
179 
180     Serialization_Obj();     std::cout << "----------------------\n";
181 
182     getchar();
183     return 0;
184 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```









