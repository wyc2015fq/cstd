# PHP学习 - LC900730的博客 - CSDN博客
2017年05月08日 10:02:53[lc900730](https://me.csdn.net/LC900730)阅读数：242标签：[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[php](https://blog.csdn.net/LC900730/article/category/6906542)
## php语言执行
php也是一组符合一定规则的约定地点指令。编程以php语言实现以后，通过php虚拟机(zend引擎)将php指令转变成C语言(可以理解为更深层的一种指令集)，然后C语言转换为汇编语言，汇编语言根据处理器的规则转变成机器码执行。这是一个更高层次抽象的不断具体化，不断细化的过程。
- 从一种语言到另一种语言的转化称为编译，即源语言->目标语言;编译器完成词法分析->语法分析->语义分析->中间代码生成-中间代码生成->代码优化->目标代码生成；
其中(中间代码的生成、代码优化、目标代码生成)的作用是构造目标程序，我们可以称为编译器的后端；
- 
编译类语言：在执行之前会有一个翻译过程，其中关键点是有一个形式上完全不同的等价程序生成；
- 解释型语言：没有这样一个等价的程序生成，php生成的是一种中间代码Opcode码，这是php的一种内部数据结构
执行过程如下
#### 1.Scaning(Lexing):将php代码转换成语言片段(Token)
Lex就是一个词法分析的依据表，对于php开始之前的是Flex，之后改成re2c，Mysql词法分析采用的是Flex，除此之外还有作为UNIX系统标准词法分析器的Lex等；这些工具会读入一个代表词法分析器规则的输入字符串流，然后输出以C语言实做的词法分析器源代码。Zend/zend_language_scanner.c会根据Zend/zend_language_scanner.1来对输入的php代码进行词法分析，从而得到一个一个词；
token_get_all:这个函数可以将一段php代码Scanning成Tokens；
#### 2.Parsing:将Token转换成简单又意义的表达式
Parsing首先会丢弃Tokens Array中的多余的空格；然后将剩余的Tokens转换成一个一个简单的表达式； 
Bison是一种通用目的的分析器生成器，它将LALR上下文无关的描述转化成分析该为文法的c程序。使用它可以生成解释器、编译器、协议等多种程序。Bison向上兼容Yaff ,所有书写正确的Yacc语法都应该可以不加修改地在Bison下工作
#### 3.Compilation:将表达式编译成Opcode
将表达式编译成Opcode码，之后就是Compilation阶段，它会将Tokens编译成一个个op_array,，每个op_arrayd包含如下5个部分；
```
struct _zend_op {  
    opcode_handler_t handler; // 执行该opcode时调用的处理函数  
    znode result;  
    znode op1;  
    znode op2;  
    ulong extended_value;  
    uint lineno;  
    zend_uchar opcode; // opcode代码  
};
```
和CPU指令类似，有一个标示指令的opcode字段，以及这个opcode所操作的操作数。php不像汇编那么底层，在脚本执行过程中可能还需要其他更多信息，extended_value字段就保存了这类信息，其他result域则是保存这指令执行完成后的结果。
PHP脚本编译为opcode保存在op_array中，其内部存储的结构如下
```
struct _zend_op_array {  
    /* Common elements */  
    zend_uchar type;  
    char *function_name; // 如果是用户定义的函数则，这里将保存函数的名字  
    zend_class_entry *scope;  
    zend_uint fn_flags;  
    union _zend_function *prototype;  
    zend_uint num_args;  
    zend_uint required_num_args;  
    zend_arg_info *arg_info;  
    zend_bool pass_rest_by_reference;  
    unsigned char return_reference;  
    /* END of common elements */  
    zend_bool done_pass_two;  
    zend_uint *refcount;  
    zend_op *opcodes; // opcode数组  
    zend_uint last，size;  
    zend_compiled_variable *vars;  
    int last_var，size_var;  
    // ...  
}
```
opcodes都保存在这里，在执行的时候由下面的execute函数执行;
#### Execution:顺序执行Opcode，每次一条，从而实现脚本的功能
ZEND_API void execute(zend_op_array *op_array TSRMLS_DC) 
{ 
    // … 循环执行op_array中的opcode或者执行其他op_array中的opcode 
} 
前面提到每条opcode都有一个opcode_handler_t的函数指针字段，用于执行该opcode 
php有三种方式来进行opcode的处理：Call Switch和goto；
php默认使用CALL方式，也就是函数调用的方式，由于opcode执行是每个php程序需要频繁进行的操作，可以使用switch或者goto方式来分发，通常goto的效率会高一些。
在上面，我们的php代码会被parsing成：
```
* ZEND_ECHO     'Hello World%21'  
* ZEND_ADD       ~0 1 1  
* ZEND_ASSIGN  !0 ~0  
* ZEND_ECHO     !0  
* ZEND_RETURN  1
```
$a去哪里了？
#### 操作数
每个操作数都是由2部分组成： 
a：op_type:为IS_CONST,IS_TMP_VAR,IS_UNUSED,IS_CV,IS_VAR, 
b:u,一个联合体，分别用不同的类型保存了这个操作数的值(const)或者左值(var)
## 中间代码
- 树和有向无环图(DAG):高层表示，适用于程序源代码
- 三地址码:低层表示，靠近目标机器
- 控制流图：更精细的三地址码，程序的图状表示，适合做程序分析
- 静态单赋值形式：更精细的控制流图；同时编码控制流信息和数据流信息
- 连续传递风格(CPS):更一般的SSA
