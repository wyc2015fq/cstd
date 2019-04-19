# 白话 PHP7 扩展开发：创建对象 - 文章 - 伯乐在线
原文出处： [信海龙（@淘宝信海龙）](http://www.bo56.com/%E7%99%BD%E8%AF%9Dphp7%E6%89%A9%E5%B1%95%E5%BC%80%E5%8F%91%E4%B9%8B%E5%88%9B%E5%BB%BA%E5%AF%B9%E8%B1%A1/)
本篇文章主要将如何在扩展中创建一个对象。创建的对象的过程，其实和一个小孩出生，成长的过程有些类似。
### 第一步，办准生证
生孩子第一步，先办准生证。声明我要生孩子了。对象创建的时候，如何办准生证呢？只要定义一个zend_class_entry变量即可。代码如下：

PHP
```
zend_class_entry ce;
```
zend_class_entry 是啥？可以认为它使一个原型，定义了一些对象应该有哪些东西组成。具体代码可以查看./Zend/zend.h文件。
### 第二步，取名字
孩子怎么得有个名字，对象也一样。如何给对象取名字呢？代码如下：

PHP
```
INIT_CLASS_ENTRY(ce, "children", children_methods);
```
其中，children就是我们给对象取的名字。那children_methods是啥？它是这个小孩应该具备的能力（对象所拥有的方法）。
### 第三步，上户口
孩子出生了，名字有了，下面就得上户口了。上户口的过程就是登记入册的过程。代码如下：

PHP
```
zend_class_entry *children_ce;
children_ce = zend_register_internal_class(&ce);
```
zend_register_internal_class方法会返回一个zend_class_entry指针。以后我们对这个对象的操作，可以使用这个指针。
### 培养（定义属性和方法）
孩子已经上户口了。接下来，我们就是认真的培养他。培养成对社会有用的人。那么如何培养呢？主要从两方面入手。第一方面是教授知识（定义属性），另一方面是培养其行为能力（定义方法）。
定义属性，我们使用zend_declare_property*系列方法。代码示例如下：

PHP
```
zend_declare_property_null(children_ce, "memory",       sizeof("memory") - 1, ZEND_ACC_PUBLIC);
```
上面的代码我们就声明了一个名称为memory的属性，并且设置访问类型为 public。
定义方法的过程更简单。还记得我们在上户口的时候，登记了children_methods。这个就是孩子行为的一个集合。这个行为集合如何产生的呢？代码如下：

PHP
```
ZEND_BEGIN_ARG_INFO_EX(arginfo_children_learn, 0, 0, 1)
    ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()
PHP_METHOD(children, learn);
const zend_function_entry children_functions[] = {
PHP_FE(learn, arginfo_children_learn)
{NULL, NULL, NULL}
}
```
ZEND_BEGIN_ARG_INFO_EX的最后一个参数1，是传递的参数的个数。
ZEND_ARG_INFO的第一个参数0，表示是否传引用方式传递。一般默认为0即可。
### 完整代码
至此，我们已经定义了一个对象，拥有属性memory和方法learn。完整的代码如下：

PHP
```
zend_class_entry *children_ce;
ZEND_BEGIN_ARG_INFO_EX(arginfo_children_learn, 0, 0, 1)
    ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()
const zend_function_entry children_functions[] = {
PHP_ME(children, learn, arginfo_children_learn, ZEND_ACC_PUBLIC)
{NULL, NULL, NULL}
}
PHP_MINIT_FUNCTION(children) 
{
zend_class_entry ce;
INIT_CLASS_ENTRY(ce, "children", children_methods);
children_ce = zend_register_internal_class(&ce);
zend_declare_property_null(children_ce, "memory",       sizeof("memory") - 1, ZEND_ACC_PUBLIC);
}
PHP_METHOD(children, learn)
{
char *love;
size_t love_len;
if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "s",&content, &content_len) == FAILURE) {
        return;
}
zend_update_property_string(children_ce,  getThis(), "memory", sizeof("memory") - 1, love);
}
```
### PHP调用代码

PHP
```
memory);
$children->learn("love");
var_dump($children->memory);
?>
```
输出内容如下：

PHP
```
NULL
string(4) "love"
```
### 代码下载
[完整的扩展源代码下载](http://www.bo56.com/download/php7_ext/children.tar.gz)
