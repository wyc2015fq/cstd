# PHP扩展 - LC900730的博客 - CSDN博客
2017年05月22日 17:05:18[lc900730](https://me.csdn.net/LC900730)阅读数：172标签：[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[php](https://blog.csdn.net/LC900730/article/category/6906542)
## PHP变量
php中数据存储单位是zval，也称为Zend Value，它是一个只有4个成员的结构体，在Zend/zend.h中定义
```
typedef struct _zval_struct{
    zval_value value;
    zend_uint refcount;
    zend_uchar type;
    zend_uchar is_ref;
}zval
```
而value成员实际上是一个定义为union的结构
```
typedef union _zvalue_value{
    long lval;
    double dval;
    struct {
        char *val;
        int len;
    }str;
    HashTable *ht;
    zend_object_value obj;
}
```
union允许Zend使用一个单一的，统一的结构来将许多不同类型的数据存储到一个php变量中
zend定义了8种数据类型
|IS_NULL|这个类型自动赋值给未初始化的变量，直到它第一次被使用。也可以在用户空间使用内建的NULL常量进行显式赋值。这个变量类型提供了一种特殊的“没有数据”的类型，它和布尔的FALSE以及整型0有所不同|
|----|----|
|IS_BOOL|布尔变量是2中状态中的一种，TRE/FALSE。用户空间控制结构if/while/for等中间的条件表达式在评估时都会隐士转换成布尔类型|
|IS_DOUBLE||
|||
|||
|||
|||
数组数据类型内部以HashTable*存储。 
当你创建变量时，为它赋一个值，Zend会将这个值放到符号表中的一个内部数组中。有一个符号表定义了全局作用域，它在请求启动后，扩展的RINIT方法被调用之前初始化，接着在脚本执行完成后，后续的RSHUTDOWN方法被执行之前销毁。 
当一个用户空间的函数或者对象方法被调用，则分配一个新的符号表用于函数或者方法的生命周期，它被定义为激活的符号表。如果当前的脚本执行不在函数或者方法中，则全局符号表被认为是激活的。
```
struct _zend_execution_globals{
    HashTable symbol_table;
    HashTable *active_symbol_table;
}
```
symbol_table:使用EG(symbol_table)访问，它永远是全局变量作用域，和用户空间的$GLOBALS变量类似，用于对应于php脚本的全局作用域。实际上$GLOBALS变量的内部就是对EG(symbol_table)上的一层包装
/*php实现*/ 
\?php $foo=’bar’;?> 
/*C实现*/ 
{ 
    zval *fooval; 
    MAKE_STD_ZVAL(fooval); 
    ZVAL_STRING(fooval,”bar”,1); 
    ZEND_SET_SYMBOL(EG(active_symbol_table),”foo”,fooval); 
} 
首先使用MAKE_STD_ZVAL()分配一个新的zval，它的值被初始化字符串“bar”,接着是一个新的宏调用，它的作用是将fooval这个zval增加到当前激活的符号表中，设置的新变量名为“foo”.此时没有用户空间函数被激活，所以EG(active_symbol_table)==&EG(symbol_table);最终含义是这个变量被存储到了全局作用域中
### 数据取回
{ 
    zval **fooval； 
    if(zend_hash_find(EG(active_symbol_table),”foo”,sizeof(“foo”),(void **)&fooval)==SUCCESS){ 
        php_printf(“Got the value of $foo!”);      }else{          php_printf(“$foo is not defined!”); 
    } 
} 
HashTable并不仅仅用于用户空间变量。HashTable结构用途很广，它还被用于在整个引擎中，甚至还可以完美的存储非指针数据。为了存储任意大小的数据，HashTable将分配一块内存用来放置被存储的数据。对于变量而言，被存储的是一个zval*,因此hashTable的存储机制分配了一块足够保存一个指针的内存。HashTable这个桶使用这个新的指针保存zval *的值，因此在HashTable中保存的是zval **。 
在取回数据的时候,HashTable仅知道有个指针指向某个数据，为了将指针弹出到调用函数的本地存储中，调用函数自然就要取本地指针(变量)的地址，结果就是一个未知的类型的两级间防的指针变量。你可以看到把这种类型传递给zend_hash_find()时，编译器会发现不同，它知道是三级间访而不是二级。这就是我们在前面加上一个强制类型转换的目的。用来抑制编译器的警告。 
如果zend_hash_find()定位到了你要查找的项，那么他就会将所请求的数据第一次被增加到HashTable时候，分配的桶的指针弹出到所提供的指针(zend_hash_find()第4个参数)中，同时返回一个SUCCESS整型常量。如果不能找到，那么就不会修改而是返回整型常量FAILURE
### 数据的创建
需要分配一个块内存给它，并且将它赋值给zval *指针，宏MAKE_STD_ZVAL(pzv);。这个宏将会以一种优化的方式在其他附近为其分配内存，并初始化新zval的refcount和is_ref属性。 
设置zval *的值 
zVal_NULL(pvz);       z_TYPE_P(pzv)=IS_NULL; 
//z_TYPE_P(pzv)=IS_STRING;
zVAL_STRINGL(pzv,str,len,dup); 
z_TYPE_P(pzv)=IS_STRING; 
z_STRLEN_P(pzv)=len; 
if(dup){ 
    Z_STRVAL_P(pzv)=estrndup(str,len+1); 
}else{ 
    Z_STRVAL_P(pzv)=str; 
}
## 错误处理
为了提供从用户脚本的激活请求和所在的扩展函数中跳出的能力，需要存在一种方法跳出整个激活请求。Zend引擎中的处理方式是在请求开始的地方设置一个跳出地址，在所以的die()/exit()调用后，或者碰到一些关键性错误(E_ERROR)时，执行longimp()转向大预先设置的跳出地址。
### 引用计数
变量的值和变量的名是两个不同的概念，它的值是自身一个没有名字的zval *，使用zend_hash_add()将他赋值给$a
