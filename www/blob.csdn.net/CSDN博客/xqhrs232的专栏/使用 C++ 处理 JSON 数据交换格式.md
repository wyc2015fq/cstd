# 使用 C++ 处理 JSON 数据交换格式 - xqhrs232的专栏 - CSDN博客
2015年01月07日 16:27:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：646
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/xt_xiaotian/article/details/5648388](http://blog.csdn.net/xt_xiaotian/article/details/5648388)
相关文章
1、[C++ Jsoncpp源代码编译与解析Json](http://www.cnblogs.com/xudong-bupt/p/3696329.html)----[http://www.cnblogs.com/xudong-bupt/p/3696329.html](http://www.cnblogs.com/xudong-bupt/p/3696329.html)
2、[JSON 数据格式](http://www.cnblogs.com/SkySoot/archive/2012/04/17/2453010.html)----[http://www.cnblogs.com/SkySoot/archive/2012/04/17/2453010.html](http://www.cnblogs.com/SkySoot/archive/2012/04/17/2453010.html)
**使用 C++ 处理 JSON 数据交换格式**
**一、摘要**
    JSON 的全称为：JavaScript Object Notation，顾名思义，JSON 是用于标记 Javascript 对象的，JSON 官方的解释为：JSON 是一种轻量级的数据传输格式。
本文并不详细介绍 JSON 本身的细节，旨在讨论如何使用 C++ 语言来处理 JSON。关于 JSON更具体的信息，可参见 JSON 官网：http://www.json.org。
**二、本文选择处理 JSON的 C++ 库**
    本文选择一个第三方库 jsoncpp 来解析 JSON。jsoncpp 是比较出名的 C++ JSON 解析库。在JSON 官网也是首推的。
下载地址为：http://sourceforge.net/projects/jsoncpp。本文使用的 jsoncpp 版本为：0.5.0。
**三、jsoncpp 在 Windows 下的编译**
要使用第三方源码库，第一步少不了的就是编译，将源码文件编译成我们方便使用的动态链接库、静态链接库或者静态导入库[1]。
jsconcpp 进行 JSON 解析的源码文件分布在 include/json、src/lib_json 下。其实 jsoncpp 源码并不多，为了方便产品管理，此处没必要将其编译为动态链接库或者静态导入库，所以我们选择使用静态链接库[2]。
jsoncpp 已经处理的很完善了，所有编译选项都已经配置好，打开makefiles/vs71/jsoncpp.sln 便可以开始编译（默认是使用 VS2003 编译器的，打开时直接按照 VS2005 提示转换即可）。
**四、jsoncpp 使用详解**
    jsoncpp 主要包含三种类型的 class：Value、Reader、Writer。jsoncpp 中所有对象、类名都在namespace Json 中，包含 json.h 即可。
    Json::Value 只能处理 ANSI 类型的字符串，如果 C++ 程序是用 Unicode 编码的，最好加一个Adapt 类来适配。
**1、Value**
    Json::Value 是jsoncpp 中最基本、最重要的类，用于表示各种类型的对象，jsoncpp 支持的对象类型可见 Json::ValueType 枚举值。
可如下是用 Json::Value 类：
Json::Value json_temp;      // 临时对象，供如下代码使用
json_temp["name"] = Json::Value("huchao");
json_temp["age"] = Json::Value(26);
Json::Value root;  // 表示整个 json 对象
root["key_string"] = Json::Value("value_string");         // 新建一个 Key（名为：key_string），赋予字符串值："value_string"。
root["key_number"] = Json::Value(12345);            // 新建一个 Key（名为：key_number），赋予数值：12345。
root["key_boolean"] = Json::Value(false);              // 新建一个 Key（名为：key_boolean），赋予bool值：false。
root["key_double"] = Json::Value(12.345);            // 新建一个 Key（名为：key_double），赋予double 值：12.345。
root["key_object"] = Json_temp;                           // 新建一个 Key（名为：key_object），赋予json::Value 对象值。
root["key_array"].append("array_string");             // 新建一个 Key（名为：key_array），类型为数组，对第一个元素赋值为字符串："array_string"。
root["key_array"].append(1234);                           // 为数组 key_array 赋值，对第二个元素赋值为：1234。
Json::ValueType type = root.type();                       // 获得 root 的类型，此处为objectValue 类型。
注：跟C++ 不同，JavaScript 数组可以为任意类型的值，所以 jsoncpp 也可以。
    如上几个用法已经可以满足绝大部分 json 应用了，当然 jsoncpp 还有一些其他同能，比如说设置注释、比较 json 大小、交换 json 对象等，都很容易使用，大家自己尝试吧。
**2、Writer**
如上说了 Json::Value 的使用方式，现在到了该查看刚才赋值内容的时候了，查看 json 内容，使用 Writer 类即可。
Jsoncpp 的 Json::Writer 类是一个纯虚类，并不能直接使用。在此我们使用 Json::Writer 的子类：Json::FastWriter、Json::StyledWriter、Json::StyledStreamWriter。
顾名思义，用 Json::FastWriter 来处理 json 应该是最快的，下面我们来试试。
Json::FastWriter fast_writer;
std::cout << fast_writer.write(root) << std::endl;
输出结果为：
{"key_array":["array_string",1234],"key_boolean":false,"key_double":12.3450,"key_number":12345,"key_object":{"age":26,"name":"huchao"},"key_string":"value_string"}
再次顾名思义，用 Json::StyledWriter 是格式化后的 json，下面我们来看看 Json::StyledWriter 是怎样格式化的。
Json::StyledWriter styled_writer;
std::cout << styled_writer.write(root) << std::endl;
输出结果为：
{
   "key_array" : [ "array_string", 1234 ],
   "key_boolean" : false,
   "key_double" : 12.3450,
   "key_number" : 12345,
   "key_object" : {
      "age" : 26,
      "name" : "huchao"
   },
   "key_string" : "value_string"
}
**3、Reader**
    Json::Reader 是用于读取的，说的确切点，是用于将字符串转换为 Json::Value 对象的，下面我们来看个简单的例子。
  Json::Reader reader;
  Json::Value json_object;
  const char* json_document = "{/"age/" : 26,/"name/" : /"huchao/"}";
  if (!reader.parse(json_document, json_object))
    return 0;
  std::cout << json_object["name"] << std::endl;
  std::cout << json_object["age"] << std::endl;
输出结果为：
"huchao"
26
可见，上述代码已经解析出了 json 字符串。
--------------------------------------
[1]：使用第三方源码最简单的方法是直接将文件加入工程，但这样不利于源码、软件产品管理，对于一般软件开发来说，不建议使用。
[2]：如果真需要编译成动态链接库、静态导入库的话，可以使用 VS 新建一个工程属性，然后在Project --> Properties中进行相应的设置即可。
