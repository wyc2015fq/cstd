# 浅析 PHP 变量存储结构及弱类型实现机制 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [伯乐在线读者](http://www.jobbole.com/members/jobbole) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
***（本文来自 步迎飞 的自荐投稿）***
根据需要，下载相应php源代码：（[php 下载地址](http://php.net/downloads.php)）
![图示：](http://img.blog.csdn.net/20160901120117535)
当前下载php版本为 php-5.5.37
变量的值存储到以下所示zval结构体中。 zval结构体定义在Zend/zend_types.h文件


```
typedef struct _zval_struct zval;
```
struct _zval_struct 定义在Zend/zend.h文件中


```
struct _zval_struct {
    /* Variable information */
    zvalue_value value;     /* value */
    zend_uint refcount__gc;
    zend_uchar type;    /* active type */
    zend_uchar is_ref__gc;
};
```
|属性名|含义|
|----|----|
|refcount__gc|表示引用计数|
|is_ref__gc|表示是否为引用|
|value|存储变量的值|
|type|变量具体的类型|
### 变量类型:type
**zval结构体的type字段就是实现弱类型最关键的字段了**，type的值可以为： IS_NULL、IS_BOOL、IS_LONG、IS_DOUBLE、IS_STRING、IS_ARRAY、IS_OBJECT和IS_RESOURCE 之一。和他们定义在一起的类型还有IS_CONSTANT和IS_CONSTANT_ARRAY。
### 变量的值存储
变量的值存储在zvalue_value联合体中


```
typedef union _zvalue_value {
    long lval;                  /* long value */
    double dval;                /* double value */
    struct {
        char *val;
        int len;
    } str;
    HashTable *ht;              /* hash table value */
    zend_object_value obj;
} zvalue_value;
```
*各种类型的数据会使用不同的方法来进行变量值的存储*
|变量类型|宏|简介|
|----|----|----|
|boolean|ZVAL_BOOL|布尔型的变量值存储于(zval).value.lval中，其类型也会以相应的IS_*进行存储。|
|integer|ZVAL_LONG|整型的变量值存储于(zval).value.lval中，其类型也会以相应的IS_*进行存储。|
|float|ZVAL_DOUBLE|浮点型的变量值存储于(zval).value.dval中，其类型也会以相应的IS_*进行存储。|
|null|ZVAL_NULL|NULL值的变量值不需要存储，只需要把(zval).type标为IS_NULL。|
|resource|ZVAL_RESOURCE|资源类型的存储与其他一般变量无异，但其初始化及存取实现则不同。|
**字符串String**
字符串的类型标示和其他数据类型一样，不过在存储字符串时多了一个字符串长度的字段。


```
struct {
    char *val;
    int len;
} str;
```
这么看在PHP中strlen()函数可以在常数时间内获取到字符串的长度。 计算机语言中字符串的操作都非常之多，所以大部分高级语言中都会存储字符串的长度。
**数组Array**
数组的值存储在zvalue_value.ht字段中，它是一个HashTable类型的数据。 PHP的数组使用哈希表来存储关联数据。
**对象Object**
PHP的对象是一种复合型的数据，使用一种zend_object_value的结构体来存放。其定义如下
*声明在Zend/zend_types.h*


```
typedef struct _zend_object_value {
    zend_object_handle handle;
    const zend_object_handlers *handlers;
} zend_object_value;
```
PHP的弱变量容器的实现方式是兼容并包的形式体现，针对每种类型的变量都有其对应的标记和存储空间。 使用强类型的语言在效率上通常会比弱类型高。
