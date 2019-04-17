# jsoncpp构造json字符串和json数组 - DoubleLi - 博客园






## jsoncpp构造json字符串和json数组

参考文章：[Jsoncpp的简单使用](http://www.cnblogs.com/likebeta/archive/2012/09/10/2678475.html)
下载json文件夹放在c++项目的include目录下，在CMakeLists中include进去，然后就可以在代码中加入#include “json/json.h”使用啦。下载地址：[https://github.com/open-source-parsers/jsoncpp/tree/master/include](https://github.com/open-source-parsers/jsoncpp/tree/master/include)。
- jsoncpp构造json字符串

```
Json::Value root;
    Json::FastWriter writer;
    string name = "abcd";
    root["name"] = name;
    root["number"] = "2010014357";
    root["address"] = "xxxx";
    root["age"] = 100;
    string data= writer.write(root);  //need #include <fstream>
    cout<<"data:\n"<<data<<endl;
```
- jsoncpp构造json数组

```cpp
#include "json/json.h"
    #include <fstream>
    #include <iostream>
    using namespace std;
    int main()
    {
        Json::Value root;
        Json::FastWriter writer;
        Json::Value person;
        person["name"] = "hello world1";
        person["age"] = 100;
        root.append(person);
        person["name"] = "hello world2";
        person["age"] = 200;
        root.append(person);
        string data= writer.write(root);
        cout<<data<<endl;   
        cout<<root.toStyledString()<<endl;
        return 0;
    }
```

输出为： 
![这里写图片描述](https://img-blog.csdn.net/20151204214259939)
前者就是一般的输出，root.toStyledString()比较规整。









