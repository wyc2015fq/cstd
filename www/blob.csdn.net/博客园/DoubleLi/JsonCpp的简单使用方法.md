# JsonCpp的简单使用方法 - DoubleLi - 博客园





JsonCpp 是一个C++用来处理JSON 数据的开发包。下面讲一下怎么使用JsonCpp来序列化和反序列化Json对象，以实际代码为例子。

反序列化Json对象

比如一个Json对象的字符串序列如下,其中”array”:[...]表示Json对象中的数组：
{“key1″:”value1″,”array”:[{"key2":"value2"},{"key2":"value3"},{"key2":"value4"}]}，那怎么分别取到key1和key2的值呢，代码如下所示:

    std::string strValue = “{\”key1\”:\”value1\”,\”array\”:[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}”;

    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
    std::string out = value["key1"].asString();
    std::cout << out << std::endl;
    const Json::Value arrayObj = value["array"];
    for (int i=0; i<arrayObj.size(); i++)
    {
    out = arrayObj[i]["key2"].asString();
    std::cout << out;
    if (i != arrayObj.size() – 1 )
    std::cout << std::endl;
    }
    }

序列化Json对象

先构建一个Json对象，此Json对象中含有数组，然后把Json对象序列化成字符串，代码如下：

    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    for (int i=0; i<10; i++)
    {
    item["key"] = i;
    arrayObj.append(item);
    }

    root["key1"] = “value1″;
    root["key2"] = “value2″;
    root["array"] = arrayObj;
    root.toStyledString();
    std::string out = root.toStyledString();
    std::cout << out << std::endl;

删除Json对象
    std::string strContent = "{\"key\":\"1\",\"name\":\"test\"}";

    Json::Reader reader;

    Json::Value value;

    if (reader.parse(strContent, value))
    {
        Json::Value root=value;

        root.removeMember("key");

        printf("%s \n",root.toStyledString().c_str());
![复制代码](https://common.cnblogs.com/images/copycode.gif)









