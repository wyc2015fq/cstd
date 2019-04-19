# 深入理解PHP内核（一）：生命周期和zend引擎 - LC900730的博客 - CSDN博客
2017年06月12日 11:11:07[lc900730](https://me.csdn.net/LC900730)阅读数：526
## 1.SAPI
脚本执行的开始都是SAPI接口实现开始的。只是不同的SAPI接口实现会完成他们的特定工作，例如Apache的mod_php SAPI实现需要初始化从Apache获取的一些信息，在输出内容则是将内容返回给Apache。
## 2.开始和结束
PHP开始执行以后会经历两个主要阶段：处理请求之前的开始阶段和请求之后 结束阶段。 
开始阶段：模块初始化MINIT；第二个过程是RINIT请求开始。(如PHP注册了一些扩展模块，则在MINIT阶段会回调所有模块的MINIT函数)，模块在这个阶段可以进行一些初始化工作，例如注册常量、定义模块使用类等。模块在实现
```bash
PHP_MINIT_FUNCTION(myphpextension){
    注册常量或者类等初始化工作
    return SUCCESS；
}
```
请求到达之后PHP初始化脚本的执行环境，例如创建一个执行环境，包括保存PHP运行过程中变量名称和值内容的符号表以及当前所以的函数以及类等信息的符号表，然后PHP会调用所以模块的RINT函数，在这个阶段各个模块也可以执行一些相关操作，模块的RINT函数和MINIT回调函数类似:
```
PHP_RINT_FUNCTION(myphpextension){
    //注册常量或者类等初始化工作
    //例如记录请求开始时间
    //随后在请求结束的时候记录结束时间，这样就可以记录处理请求所花费的时间
    return SUCCESS；
}
```
请求处理完成就进入了结束阶段，一般脚本执行到末尾或者通过调用exit或die函数。结束也分为2个环节，一个在请求结束后停用模块(RSHUTDOWN，对应RINIT)，一个在SAPI生命周期结束(web服务器退出或者命令行脚本执行完毕退出)时关闭模块(MSHUTDOWN，对应MINIT)
整个SAPI类似于一个面向对象中的模板方法模式的应用。SAPI.c和SAPI.h文件所包含的一些函数就是模板方法中的抽象模板，各个服务器对于sapi_module的定义和相关实现是一个个具体模板； 
php扩展中，每个扩展都需要定义一个zend_module_entry结构体，在PHP生命周期中，如果需要调用某个扩展，其调用的方法都是zend_module_entry结构中指定的方法；
## Apache的php模块
当php需要在Apache下运行时，一般来说是mod_php5模块的形式集成的。接收apache传递来的php文件请求。如配置好模块，php模块通过注册apache2的ap_hook_post_config挂钩，在Apache启动时候启动此模块以接受PHP文件请求。
Zend引擎词法分析
### 变量类型
标量类型：boolean integer float string 
复合类型：array object 
特殊类型：resource NULL
#### 1.变量存储结构
变量的值存储到以下所示zval结构体中。zval结构体定义在zend/zend.h文件； 
php在存储变量时将PHP用户空间的变量类型也保存在同一个结构体中。
|属性名|含义|默认值|
|----|----|----|
|refcount_gc|表示引用计数|1|
|is_ref_gc|表示是否为引用|0|
|value|存储变量的值||
|type|变量具体类型||
php5.3之前是refcount和is_ref; 
变量的值存储在zvalue_value
#### 2.变量类型
type字段就是实现弱类型最关键的字段。值为IS_NULL,IS_BOOL,IS_LONG,IS_DOUBLE,IS_STRING,IS_ARRAY,IS_OBJECT 
,IS_RESOURCE。
#### 3.变量值存储
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
使用union是为了空间利用率的考虑。
数组的值存储在zvalue_value.ht字段中，它是一个HashTable类型的数据。PHP使用哈希表来存储关联数据。哈希表是一种高效的键值对存储结构，PHP哈希表实现中使用了两个数据结构HashTable和Bucket。
#### 对象Object
```
typedef struct _zend_object_value {
    zend_object_handle handle;  //  unsigned int类型，EG(objects_store).object_buckets的索引
    zend_object_handlers *handlers;
} zend_object_value;
```
数据结构： 
首先我们需要一个容器来保存我们的 哈希表，哈希表需要保存的内容主要是保存进来的数据，同时为了方便的得知哈希表中存储的元素个数，需要保存一个大小字段。第二个就是需要保存数据的容器了。
