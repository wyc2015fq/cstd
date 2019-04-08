# json-c库的使用
2013年08月22日 20:39:32 [梦的延长线](https://me.csdn.net/cuishumao) 阅读数：10438   

## 下载代码：

版本：0.11

```
git clone https://github.com/arnoldlu/json-c.git
```



## 编译安装：

```
sh autogen.sh ./configure make make install
```



**头文件1-----json_object.h**

```c
#ifdef__cplusplus
extern"C" {
#endif
......  这里省略了部分
//json c类型
typedef enum json_type {
/* Ifyou change this, be sure to update json_type_to_name() too */
    json_type_null,
    json_type_boolean,
    json_type_double,
    json_type_int,
    json_type_object,
    json_type_array,
    json_type_string,
}json_type;
```
## **一、创建json对象**

```c
（1）函数：externstruct json_object* json_object_new_object(void);

函数说明：创建一个新的json对象，引用计数1，该指针具有唯一的所有权。当使用json_object_object_add         

或者json_object_array_put_idx作用于该对象时，所有权转移到另一方。使用json_object_get作用于该对象的所有权，必须使用json_object_put释放。

（2）函数：externstruct json_object* json_object_new_array(void);

函数说明：创建的json数组类型对象

（3）//创建一个json 的bool对象。

extern struct json_object*json_object_new_boolean(json_bool b);

（4）//创建josn的int 32、64对象

externstruct json_object* json_object_new_int(int32_t i);

externstruct json_object* json_object_new_int64(int64_t i);

（5）//将c字符串转换为json字符串格式的对象，该对象堆上分配

externstruct json_object* json_object_new_string(const char *s);

externstruct json_object* json_object_new_string_len(const char *s, int len);

（6）/* 创建double类型的json对象 */

externstruct json_object* json_object_new_double(double d);

```



## **二、增加、删除、修改**

```
（1）//给json对象增加字段

//引用计数不会增加，val的所有权转移到obj。

externvoid json_object_object_add(struct json_object* obj, const char *key,structjson_object *val);

（2）//删除json对象的指定字段，被删除的对象引用计数减去1，如果这个val没有更多的所有者，这个key对应

//的val被free，否则这个val的引用保存在内存中

externvoid json_object_object_del(struct json_object* obj, const char *key);

（3）//增加一个元素到json数组的末尾。

//obj引用计数不会增加，增加字段的方式更加紧凑。

//如果需要获取val的引用，需要用json_object_get来传递该对象。

externint json_object_array_add(struct json_object *obj, struct json_object *val);

（4）//替换json数组中的值

externint json_object_array_put_idx(struct json_object *obj, int idx,

structjson_object *val);

（5）//json数组的排序，这里需要自己写排序函数

externvoid json_object_array_sort(struct json_object *jso, int(*sort_fn)(const void*, const void *));

```



## **二、取值**

```
（1）//获取json对象的长度，依据字段的数目

extern intjson_object_object_length(struct json_object* obj);

（2）//获取json对象的哈希表

extern struct lh_table*json_object_get_object(struct json_object *obj);

（3）//从obj中获取key对应的json对象，引用计数没有改变

extern struct json_object*json_object_object_get(struct json_object* obj,const char *key);

（4）//获取对象的数组列表

extern struct array_list*json_object_get_array(struct json_object *obj);

（5）//获取json的类型

externenum json_type json_object_get_type(struct json_object *obj);

（6）//获取json数组对象的长度

extern intjson_object_array_length(struct json_object *obj);

（7）//获取json对象bool值。int和double对象是0转换为FALSE，否则返回TRUE。

//非0长度的字符串返回TRUE。其他对象非空的话，返回TRUE。

extern json_booljson_object_get_boolean(struct json_object *obj);

（8）// 获取json对象的长度,如果参数不是string类型的json，返回0

extern intjson_object_get_string_len(struct json_object *obj);

（9）//按照索引获取json数组的对象

externstruct json_object* json_object_array_get_idx(struct json_object *obj, intidx);

```



## **三、类型转换**

```c
（1）      extern const char*json_object_**to_json_string**(struct json_object *obj);
转换json对象到c字符串格式
（2）//如果参数非json类型的字符串，返回thenthe JSON representation of the object is returned.参数对应的json对象内存空间，当其引用计数为0时，该内存free。
 obj是json对象实例；回c格式的字符串；
externconst char* json_object_**get_string**(struct json_object *obj);
（3）//同上，不同的是flags： formattingoptions, see JSON_C_TO_STRING_PRETTY and other constants
externconst char* json_object_to**_json_string_ex**t(struct json_object *obj, int flags);
（4）//如果传递的非int，强制转换为int。double对象返回其int。字符串解析为int。
//如果没有转换存在返回0.并且error置为EINVVAL，0和null等价。
//整形以64位值存储，如果太小或是太大，不足以填充32位，将返回INT32_MAX，INT32_MIN
externint32_t json_object**_get_int**(struct json_object *obj);
（5）//获取json对象的int64_t、double值
externint64_t json_object**_get_int64**(struct json_object *obj);
externdouble json_object_get_double(struct json_object *obj);
（6）将字符串转换为json对象
extern struct json_object* json_tokener_parse(const char *str);
```



## **四、json对象的释放**

```
//以下两个函数配合使用，前者获取该对象指针的所有权， 引用计数加1，如果对象已经被释放，返回1

extern  struct json_object* json_object_get(struct json_object *obj);

//后者引用计数减1，如果对象已经被释放，返回1

extern int  json_object_put(struct json_object *obj);

```



## **五、其他**

（1）//  类型判断

```c
extern int json_object_is_type(struct json_object *obj, enum json_type type);
```

（2）//.....这里省略了json迭代器的使用方法
\#ifdef__cplusplus
}
\#endif
 **头文件2-----json_util.h**
json_util.h文件
这个文件中有关于文件读写操作的函数，这个文件的内容是json格式的

（2）//.....这里省略了json迭代器的使用方法

```c
#ifdef__cplusplus
}
#endif
```

**头文件2-----json_util.h**
json_util.h文件
这个文件中有关于文件读写操作的函数，这个文件的内容是json格式的

```cpp
#ifndef _json_util_h_
#define _json_util_h_
#include "json_object.h"
#ifdef __cplusplus
extern "C" {
#endif
#define JSON_FILE_BUF_SIZE 4096
/* utility functions */
//（1）从文件中读出来，将文件filename中的json格式的字串读到json对象中
extern struct json_object* json_object_from_file(const char *filename);
//（2）写入到文件中，将obj对象的内容写入到文件filename中
extern int json_object_to_file(char *filename, struct json_object *obj);
//（3）
extern int json_object_to_file_ext(char *filename, struct json_object *obj, int flags);
extern int json_parse_int64(const char *buf, int64_t *retval);
extern int json_parse_double(const char *buf, double *retval);
/**
 * Return a string describing the type of the object.
 * e.g. "int", or "object", etc...
 */
// （4）返回类型字符串格式的名字，非枚举对应的整数
extern const char *json_type_to_name(enum json_type o_type);
#ifdef __cplusplus
}
#endif
#endif
```
**例子**

```cpp
#include "stdio.h"
#include "json.h"
int main()
{
	//(1)创建
	//创建一个空的json对象
	json_object *j_NullObj = NULL；
	//创建字符串型的json对象
	json_object *j_str = json_object_new_string("test string ");
    //创建一个数组对象
    json_object *j_array = json_object_new_array();
    //给元素添加到数组末尾
    json_object_array_add(j_array,json_object_new_string("china"));
    json_object_array_add(j_array,json_object_new_string("English"));
    json_object_array_add(j_array,json_object_new_string("Australia"));
    json_object_array_add(j_array,json_object_new_string("French"));
    //将上面创建的对象加入到json对象j_cfg中
    json_object *j_cfg = NULL;
    json_object_object_add(j_cfg,"str1",j_str);
    json_object_object_add(j_cfg,"array1",j_array);
    //打印j_cfg
    printf("j_cfg:%s",json_object_to_json_string(j_cfg)); 
   //（2）修改
    //修改Autralia为Korea,在实际中需要搜索Autralia的位置idx，这个改变传染到了j_cfg
	  json_object_array_put_idx(j_array,2,json_object_new_string("korea"));
	  //删除French
	  json_object_array_put_idx(j_array,3,NULL);
    printf("j_cfg:%s",json_object_to_json_string(j_cfg)); 
    //（3）将j_cfg保存在project的根目录下的文件jcfg.dat中，该文件没有则新建，
    //有则销毁重新写入
    //(4)free
    json_object_to_file("jcfg.dat",j_cfg);
    json_object_put(j_str);
    json_object_put(j_array);
    json_object_put(j_cfg);
}
```
结果：

```json
j_cfg:{"Teststrl":"test string","CountryName":["china","English","Australia","French"]}
jcfg:{ "Teststr1":"test string ","CountryName":[ "china","English","korea",null]}
```

这部分后续再完善!