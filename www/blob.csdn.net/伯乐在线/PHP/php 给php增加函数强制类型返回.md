# php:给php增加函数强制类型返回 - 文章 - 伯乐在线
原文出处： [彭长霖](http://www.tingm.cc/doku.php?id=php:%E7%BB%99php%E5%A2%9E%E5%8A%A0%E5%87%BD%E6%95%B0%E5%BC%BA%E5%88%B6%E7%B1%BB%E5%9E%8B%E8%BF%94%E5%9B%9E)
在开发过程中，函数的返回值类型应该是确定不变的，但PHP是弱类型的语言，所以PHP是没有此类语法验证的，正因为如此，造成了很多坑坑。
比如下面的代码：

PHP
```
<?php
function getArticles(...){
    $arrData = array();
    if($exp1){
      return $arrData;
    }else if($exp2){
      return 1;
    }else{
      return false;
    }
}
$arrData =getArticles(...);
foreach($arrData as $record){
//do something.
....
}
?>
```
函数getArticles根据不同的条件返回不同类型的值，有bool、int、还有数组，正常情况这类函数是希望返回数组，然后拿数组去做一些其他操作，可因为函数返回值类型不固定，调用时就很可能产生各种预想不到的坑，因此我就想，既然不能规范，那直接强制好了。
函数/方法返回值可以强制类型：

PHP
```
int function a(){
    ......
    return 1;
}
bool function b(){
    ......
    return false;
}
array function c(){
    ......
    return array();
}
object function d(){
    ......
    return new a();
}
```
支持四种强制类型限制：int、array、bool、object，当返回值与函数声明中的类型不匹配时，抛出warning，本来想抛出error，但是觉得太狠了，只能算是个异常，不能算错误，所以就用warning好了。
PHP本身是不支持 int function 这样的语法的，所以要支持，就先要搞定语法解析器，关于语法解析器，可以移步这里»>查看详情，这里就不讲了，
先修改语法扫描 Zend/zend_language_scanner.l文件
增加如下代码：

PHP
```
<ST_IN_SCRIPTING>"int" {
    return T_FUNCTION_RETURN_INT;
}
<ST_IN_SCRIPTING>"bool" {
    return T_FUNCTION_RETURN_OBJECT;
}
<ST_IN_SCRIPTING>"object" {
    return T_FUNCTION_RETURN_OBJECT;
}
<ST_IN_SCRIPTING>"resource" {
    return T_FUNCTION_RETURN_RESOURCE;
}
```
意思很简单，扫描器扫描到到关键字 int、bool、object、resource、array时返回相应的T_FUNCTION_* ，这是一个token，scanner根据不同的token做不同的处理，token要先在Zend/zend_language_parser.y文件中定义
增加如下代码

PHP
```
..........
%token T_FUNCTION_RETURN_INT
%token T_FUNCTION_RETURN_BOOL
%token T_FUNCTION_RETURN_STRING
%token T_FUNCTION_RETURN_OBJECT
%token T_FUNCTION_RETURN_RESOURCE
1
然后增加token处理逻辑：
1
function:
        T_FUNCTION { \(.u.opline_num = CG(zend_lineno);\).u.EA.var  = 0; }
    |   T_FUNCTION_RETURN_INT T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_LONG;
    }
    |   T_FUNCTION_RETURN_BOOL T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_BOOL;
    }
    |   T_FUNCTION_RETURN_STRING T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_STRING;
    }
    |   T_FUNCTION_RETURN_OBJECT T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_OBJECT;
    }
    |   T_FUNCTION_RETURN_RESOURCE T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_RESOURCE;
    }
    |   T_ARRAY T_FUNCTION {
            \(.u.opline_num = CG(zend_lineno);
            \).u.EA.var = IS_ARRAY;
    }
```
$$.u.EA.var 存储的是 函数返回类型，最后要拿他来跟返回值类型做匹配，这样语法解释器就可以处理我们新的php语法了。这还不够，还需要修改函数声明定义的处理逻辑Zend/zend_compile.c ：：zend_do_begin_function_declaration

PHP
```
......
zend_op_array op_array;
char *name = function_name->u.constant.value.str.val;
int name_len = function_name->u.constant.value.str.len;
int function_type = function_token->u.EA.var; //保存函数类型,在语法解释器中增加的
int function_begin_line = function_token->u.opline_num;
......
op_array.function_name = name;
op_array.fn_type = function_type; //将类型保存到op_array中，
op_array.return_reference = return_reference;
op_array.fn_flags |= fn_flags;
op_array.pass_rest_by_reference = 0;
..........
```
PHP是先解析PHP语法生成相应的opcode，将需要的环境、参数信息保存到execute_data全局变量中，最后在通过execute函数逐条执行opcode，所以要做处理就要把函数的类型保存到opcode中：op_array.fn_type = function_type;
op_array是没有fn_type的，要修改op_array的结构，增加zend_uint fn_type;
最后要修改opcode的毁掉函数，函数的返回 return 会生成token T_RETURN，T_RETURN会根据返回的类型调用不同的calback函数：

PHP
```
ZEND_RETURN_SPEC_CONST_HANDLER
ZEND_RETURN_SPEC_TMP_HANDLER
ZEND_RETURN_SPEC_VAR_HANDLER
```
它有三个callback，如果返回值是一个 const类型的数据，则 ZEND_RETURN_SPEC_CONST_HANDLER
返回值是临时数据，如 ： return 1，则ZEND_RETURN_SPEC_TMP_HANDLER
返回值是一个变量，如 ： return $a，则ZEND_RETURN_SPEC_VAR_HANDLER
所以要在这三个callback函数中增加处理逻辑：
在callback函数return之前增加如下代码

PHP
```
if((EG(active_op_array)->fn_type > 0) && Z_TYPE_P(retval_ptr) != EG(active_op_array)->fn_type){
        php_error_docref0(NULL TSRMLS_DC,E_WARNING, "function name %s return a wrong type.", EG(active_op_array)->function_name );
    }
```
fn_type 去跟 返回值的类型作比较，如果没有匹配到，就会抛出这个warning。我已经打了补丁，目前只支持php5.3版本，有需要的可以拿去玩一玩。不清楚为什么官方不支持此语法，我觉得还是挺有必要的。
下载补丁：php-syntax.patch
续:
后来有找鸟哥聊过,下面是他的回答:
“这个话题, 基本也是邮件组的月经贴了…. 1. 因为PHP是若类型, 很多类型可以互相转换, 那么到底要不要隐式转换, 你的实现是不转换, 这样的局限太大, 如果转换又涉及到各种转换规则. 2. 也不是不支持, 不过你的这个实现肯定是不够的(各种自定类,和继承类). 3. 以后如果要做jit, 可能会考虑支持.”
如此看来,这个问题官方也是比较纠结的,确实是我的思路是不强制转换,只需要抛出警告就行了,让开发人员自己决定是否转换,是不是更好?
