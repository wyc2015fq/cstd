## JSON解析--cJSON

# 背景

JSON即JavaScript Object Notation，是一种轻量级的数据交换格式。

JSON建构于两种结构：

- “名称/值”对的集合（A collection of name/value pairs）。不同的语言中，它被理解为*对象（object）*，纪录（record），结构（struct），字典（dictionary），哈希表（hash table），有键列表（keyed list），或者关联数组 （associative array）。
- 值的有序列表（An ordered list of values）。在大部分语言中，它被理解为数组（array）。

关于JSON库的性能评测与标准符合程度，可以参照《[28 个 C/C++ 开源 JSON 程序库性能及标准符合程度评测](http://www.oschina.net/news/61942/cpp-json-compare)》，另一个英文版更详细结果在[nativejson-benchmark](https://rawgit.com/miloyip/nativejson-benchmark/master/sample/conformance.html)。

在阅读了一系列评测之后，结合需求（ROM尽量小，RAM尽量小，带处理的数据比较简单），准备研究一下cJSON和json-c。

从[JSON官网](http://www.json.org/json-zh.html)获得json-c和cJSON的源码地址如下：[cJSON](https://github.com/arnoldlu/cJSON.git)和[json-c](https://github.com/arnoldlu/json-c.git)。

下面就基于这两份代码在Ubuntu进行速度和兼容性测试。

测试标的构造对于测试的覆盖以及测试有效性非常重要，所幸的在AOSP中有一个参考AOSP/external/jsoncpp/test。

# cJSON

## 下载代码:

git clone <https://github.com/arnoldlu/cJSON.git>

```shell
git clone https://github.com/arnoldlu/cJSON.git
```

## 编译安装：

```shell
mkdir build
cd build
cmake .. -DENABLE_CJSON_UTILS=Off -DENABLE_CJSON_TEST=On -DCMAKE_INSTALL_PREFIX=/usr （生成bin+lib）
cmake .. -DENABLE_CJSON_UTILS=Off -DENABLE_CJSON_TEST=On -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_SHARED_LIBS=Off （生成bin）
make
sudo make install （安装libcjson.so）
```

## 使用实例

参考文档：《[使用 CJSON 在C语言中进行 JSON 的创建和解析的实例讲解](http://www.cnblogs.com/fengbohello/p/4033272.html)》、《[cJSON学习笔记](http://blog.csdn.net/xukai871105/article/details/17094113)》。

下面是一个构造JSON数据，然后解析，打印Type-Key-Value的小示例：

#include <stdio.h> #include <stdlib.h> #include <string.h> #include "cJSON.h"void printJson(cJSON *root) {     if(!root)     {         printf("NULL JSON root.\n");         return;     }          printf("Type=0x%02x, %s=%s\n", root->type, root->string, cJSON_Print(root));  cJSON_Print会根据类型打印 }static char * makeJson(void) {     cJSON *pJsonRoot = NULL;     cJSON *pSubJson = NULL;     char *p = NULL;    pJsonRoot = cJSON_CreateObject();     if(NULL == pJsonRoot)     {         printf("%s line=%d NULL\n", __func__, __LINE__);         return NULL;     }     cJSON_AddStringToObject(pJsonRoot, "hello", "hello world");  String类型     cJSON_AddNumberToObject(pJsonRoot, "number", 10010);  Number类型     cJSON_AddBoolToObject(pJsonRoot, "bool", 1);  bool类型     pSubJson = cJSON_CreateObject();  创建一个cJSON，用于嵌套数据     if(NULL == pSubJson)     {         printf("%s line=%d NULL\n", __func__, __LINE__);         cJSON_Delete(pJsonRoot);         return NULL;     }     cJSON_AddStringToObject(pSubJson, "subjsonobj", "a sub json string");  在子cJSON下，增加一个String类型数据     cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);  将子cJSON加入到pJsonRoot    p = cJSON_Print(pJsonRoot);     if(NULL == p)     {         printf("%s line=%d NULL\n", __func__, __LINE__);         cJSON_Delete(pJsonRoot);         return NULL;     }    cJSON_Delete(pJsonRoot);    return p; } static void parseJson(char * pMsg) {     cJSON *pJson;     cJSON *pSub;     cJSON * pSubSub;    if(NULL == pMsg)     {     return;     }    pJson = cJSON_Parse(pMsg);     if(NULL == pJson)                                                                                              {         return ;     }    pSub = cJSON_GetObjectItem(pJson, "hello");     printJson(pSub);    pSub = cJSON_GetObjectItem(pJson, "number");     printJson(pSub);    pSub = cJSON_GetObjectItem(pJson, "bool");     printJson(pSub);    pSub = cJSON_GetObjectItem(pJson, "subobj");     printJson(pSub);    pSubSub = cJSON_GetObjectItem(pSub, "subjsonobj");     printJson(pSubSub);    cJSON_Delete(pJson); }int main(void) {     char *p;    /* print the version */     printf("Version: %s\n", cJSON_Version());  获取cJSON版本号    p = makeJson();  构造JSON数据     if(NULL == p)     {         return 0;     }     printf("p = \n%s\n\n", p);  打印构造的字符串     parseJson(p);  解析JSON数据，并打印Type-Key-Value     free(p);     return 0; }

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

void printJson(cJSON *root)
{
    if(!root)
    {
        printf("NULL JSON root.\n");
        return;
    }
    
    printf("Type=0x%02x, %s=%s\n", root->type, root->string, cJSON_Print(root));  cJSON_Print会根据类型打印
}

static char * makeJson(void)
{
    cJSON *pJsonRoot = NULL;
    cJSON *pSubJson = NULL;
    char *p = NULL;

    pJsonRoot = cJSON_CreateObject();
    if(NULL == pJsonRoot)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        return NULL;
    }
    cJSON_AddStringToObject(pJsonRoot, "hello", "hello world");  String类型
    cJSON_AddNumberToObject(pJsonRoot, "number", 10010);  Number类型
    cJSON_AddBoolToObject(pJsonRoot, "bool", 1);  bool类型
    pSubJson = cJSON_CreateObject();  创建一个cJSON，用于嵌套数据
    if(NULL == pSubJson)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        cJSON_Delete(pJsonRoot);
        return NULL;
    }
    cJSON_AddStringToObject(pSubJson, "subjsonobj", "a sub json string");  在子cJSON下，增加一个String类型数据
    cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);  将子cJSON加入到pJsonRoot

    p = cJSON_Print(pJsonRoot);
    if(NULL == p)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        cJSON_Delete(pJsonRoot);
        return NULL;
    }

    cJSON_Delete(pJsonRoot);

    return p;
}


static void parseJson(char * pMsg)
{
    cJSON *pJson;
    cJSON *pSub;
    cJSON * pSubSub;

    if(NULL == pMsg)
    {
    return;
    }

    pJson = cJSON_Parse(pMsg);
    if(NULL == pJson)                                                                                         
    {
        return ;
    }

    pSub = cJSON_GetObjectItem(pJson, "hello");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "number");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "bool");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "subobj");
    printJson(pSub);

    pSubSub = cJSON_GetObjectItem(pSub, "subjsonobj");
    printJson(pSubSub);

    cJSON_Delete(pJson);
}

int main(void)
{
    char *p;

    /* print the version */
    printf("Version: %s\n", cJSON_Version());  获取cJSON版本号

    p = makeJson();  构造JSON数据
    if(NULL == p)
    {
        return 0;
    }
    printf("p = \n%s\n\n", p);  打印构造的字符串
    parseJson(p);  解析JSON数据，并打印Type-Key-Value
    free(p);
    return 0;
}
```

在安装(sudo make install)cJSON之后，编译的时候需要跟上libcjson库文件：

```
gcc demo.c -o demo -lcjson
```

执行结果如下：

```
Version: 1.3.2
p = 
{
    "hello":    "hello world",
    "number":    10010,
    "bool":    true,
    "subobj":    {
        "subjsonobj":    "a sub json string"
    }
}

Type=0x10, hello="hello world"
Type=0x08, number=10010
Type=0x02, bool=true
Type=0x40, subobj={
    "subjsonobj":    "a sub json string"
}
Type=0x10, subjsonobj="a sub json string"
```



